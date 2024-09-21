#include "Renderer.h"
#include "imgui.h"

Renderer::~Renderer()
{
	if (m_hProcess)
		CloseHandle(m_hProcess);

	m_hProcess = nullptr;
	m_dwProcessId = 0;
	m_dwBaseAddress = 0;
	m_localPlayer = 0;
	m_entityList = 0;

	FreeConsole();
}

auto Renderer::Attach(const WCHAR* processName) -> void
{
#ifdef _DEBUG
	AllocConsole();
	freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);
	freopen_s(reinterpret_cast<FILE**>(stderr), "CONOUT$", "w", stderr);
#endif

	m_dwProcessId = Memory::GetProcessId(processName);
    m_dwBaseAddress = Memory::GetModuleAddress(m_dwProcessId, processName);
	m_hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, m_dwProcessId);

	m_localPlayer = Memory::ReadMemory<uintptr_t>(m_hProcess, m_dwBaseAddress + Offsets::player_base);
	m_entityList = Memory::ReadMemory<uintptr_t>(m_hProcess, m_dwBaseAddress + Offsets::entity_list);
	if (!m_localPlayer || !m_entityList)
	{
		std::cerr << "Failed to get either local player or entity list\n";
		Detach();
	}

	if (IsAttached())
	{
		std::cout << "[+] Attached to process with PID: " << m_dwProcessId << "\n";
		std::cout << "[+] Base address: 0x" << std::format("{:X}", m_dwBaseAddress) << "\n";
	}
	else
		std::cerr << "[-] Failed to attach to process\n";
}

bool Renderer::IsAttached() const
{
	return m_dwProcessId && m_dwBaseAddress && m_hProcess;
}

void Renderer::OnUpdate()
{
	if (GetAsyncKeyState(VK_END) & 1)
		Detach();

	if (!m_localPlayer)
	{
		std::cerr << "[-] LocalPlayer is invalid\n";
		return;
	}

	InfiniteHealth();
	InfiniteAmmo();
	RapidFire();
	NoRecoil();
}

bool Renderer::WorldToScreen(const glm::vec3& pos, glm::vec3& screen, const glm::mat4& viewMatrix)
{
	const glm::vec4 clipCoords = viewMatrix * glm::vec4(pos, 1.f);

	if (clipCoords.w < 0.1f) {
		return false;
	}

	const glm::vec3 ndc = glm::vec3(clipCoords) / clipCoords.w;

	screen.x = (GetSystemMetrics(SM_CXSCREEN) / 2.f) * (ndc.x + 1.f);
	screen.y = (GetSystemMetrics(SM_CYSCREEN) / 2.f) * (1.f - ndc.y);

	return true;
}

void Renderer::InfiniteHealth()
{
    m_oldHealth = Memory::ReadMemory<int>(m_hProcess, m_localPlayer + Offsets::health);
    if (m_oldHealth != m_newHealth)
    {
        Memory::WriteMemory<int>(m_hProcess, m_localPlayer + Offsets::health, m_newHealth);
    }
}

void Renderer::InfiniteAmmo()
{
	m_oldAmmo = Memory::ReadMemory<int>(m_hProcess, m_localPlayer + Offsets::rifle_ammo);
    if (m_oldAmmo != m_newAmmo)
    {
        Memory::WriteMemory<int>(m_hProcess, m_localPlayer + Offsets::rifle_ammo, m_newAmmo);
    }
}

void Renderer::RapidFire()
{
    m_oldCooldown = Memory::ReadMemory<float>(m_hProcess, m_localPlayer + Offsets::rifle_cooldown);
    if (m_oldCooldown != m_newCooldown)
    {
        Memory::WriteMemory<float>(m_hProcess, m_localPlayer + Offsets::rifle_cooldown, m_newCooldown);
    }
}

// Only works on version 1.2.0.2 for now
void Renderer::NoRecoil() const
{
	if constexpr (Offsets::recoil == 0)
		return;

	if (m_bNoRecoil)
		Memory::NopEx(reinterpret_cast<BYTE*>(m_dwBaseAddress + Offsets::recoil), 10, m_hProcess);
	else
		Memory::PatchEx(reinterpret_cast<BYTE*>(m_dwBaseAddress + Offsets::recoil), reinterpret_cast<const BYTE*>("\x50\x8D\x4C\x24\x1C\x51\x8B\xCE\xFF\xD2"),
			10, m_hProcess);
}

void Renderer::RenderEsp()
{
	m_viewMatrix = Memory::ReadMemory<glm::mat4>(m_hProcess, Offsets::view_matrix);

	for (int i = 0; i < 64; i++)
	{
		const auto entity = Memory::ReadMemory<uintptr_t>(m_hProcess, m_entityList + i * 0x4);
		if (!entity)
			continue;

		char nameBuffer[16] = { 0 };
		Memory::ReadString(m_hProcess, entity + Offsets::name, nameBuffer);
		const auto entityName = std::make_unique<std::string>(nameBuffer);

		if (!m_bTeamEsp)
		{
			const auto entityTeam = Memory::ReadMemory<int>(m_hProcess, entity + Offsets::team);
			const auto localTeam = Memory::ReadMemory<int>(m_hProcess, m_localPlayer + Offsets::team);
			if (entityTeam == localTeam || entity == m_localPlayer)
				continue;
		}
		else
		{
			if (entity == m_localPlayer)
				continue;
		}

		const auto entityHealth = Memory::ReadMemory<int>(m_hProcess, entity + Offsets::health);
		if (entityHealth <= 0 || entityHealth > 100)
			continue;

		std::cout << "Entity[" << i << "] " << entityName->data() << " Health: " << entityHealth << "\n";

		const auto entityHead = glm::vec3(
			Memory::ReadMemory<float>(m_hProcess, entity + Offsets::head_posx),
			Memory::ReadMemory<float>(m_hProcess, entity + Offsets::head_posy),
			Memory::ReadMemory<float>(m_hProcess, entity + Offsets::head_posz)
		);

		const auto entityFoot = Memory::ReadMemory<glm::vec3>(m_hProcess, entity + Offsets::feet_posx);

		glm::vec3 top, bottom;
		if (WorldToScreen(entityHead, top, m_viewMatrix) && WorldToScreen(entityFoot, bottom, m_viewMatrix))
		{
			const auto width = (bottom.y - top.y) / 2;
			const auto height = bottom.y - top.y;

			ImGui::GetBackgroundDrawList()->AddRect(ImVec2(top.x - width / 2, top.y), ImVec2(top.x + width / 2, bottom.y),
				IM_COL32(255, 0, 0, 255));

			const auto health = static_cast<float>(entityHealth) / 100.0f;
			const auto color = IM_COL32(255 * (1 - health), 255 * health, 0, 255);

			ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(top.x - width / 2 - 6, top.y),
				ImVec2(top.x - width / 2 - 3, bottom.y), IM_COL32(0, 0, 0, 255));
			ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(top.x - width / 2 - 5, top.y + height * (1 - health)),
				ImVec2(top.x - width / 2 - 4, bottom.y), color);

			ImGui::GetBackgroundDrawList()->AddText(ImVec2(top.x - width / 2, bottom.y), IM_COL32(255, 255, 255, 255),
				entityName->c_str());
		}
	}
}
