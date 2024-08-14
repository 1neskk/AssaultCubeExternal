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
}

auto Renderer::Attach(const WCHAR* processName) -> void
{
	m_dwProcessId = Utils::GetProcessId(processName);
    m_dwBaseAddress = Utils::GetModuleAddress(m_dwProcessId, processName);
	m_hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, m_dwProcessId);

	m_localPlayer = Utils::ReadMemory<uintptr_t>(m_hProcess, m_dwBaseAddress + Offsets::player_base);
	m_entityList = Utils::ReadMemory<uintptr_t>(m_hProcess, m_dwBaseAddress + Offsets::entity_list);
	if (!m_localPlayer || !m_entityList)
	{
		MessageBoxA(nullptr, "Failed to attach to the game process!", "Error", MB_ICONERROR);
		Detach();
	}
}

bool Renderer::IsAttached() const
{
	return m_dwProcessId && m_dwBaseAddress && m_hProcess;
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
    m_oldHealth = Utils::ReadMemory<int>(m_hProcess, m_localPlayer + Offsets::health);
    if (m_localPlayer && m_oldHealth != m_newHealth)
    {
        Utils::WriteMemory<int>(m_hProcess, m_localPlayer + Offsets::health, m_newHealth);
    }
}

void Renderer::InfiniteAmmo()
{
    if (m_localPlayer && m_oldAmmo != m_newAmmo)
    {
        Utils::WriteMemory<int>(m_hProcess, m_localPlayer + Offsets::rifle_ammo, m_newAmmo);
    }
}

void Renderer::RapidFire()
{
    m_oldCooldown = Utils::ReadMemory<float>(m_hProcess, m_localPlayer + Offsets::rifle_cooldown);
    if (m_localPlayer && m_oldCooldown != m_newCooldown)
    {
        Utils::WriteMemory<float>(m_hProcess, m_localPlayer + Offsets::rifle_cooldown, m_newCooldown);
    }
}

void Renderer::RenderEsp()
{
	m_viewMatrix = Utils::ReadMemory<glm::mat4>(m_hProcess, Offsets::view_matrix);

	for (int i = 0; i < 64; i++)
	{
		const auto entity = Utils::ReadMemory<uintptr_t>(m_hProcess, m_entityList + i * 0x4);
		if (!entity)
			continue;

		char nameBuffer[16] = { 0 };
		ReadProcessMemory(m_hProcess, reinterpret_cast<LPCVOID>(entity + Offsets::name), &nameBuffer,
			sizeof(nameBuffer), nullptr);

		const auto entityName = std::make_unique<std::string>(nameBuffer);

		if (!m_bTeamEsp)
		{
			const auto entityTeam = Utils::ReadMemory<int>(m_hProcess, entity + Offsets::team);
			const auto localTeam = Utils::ReadMemory<int>(m_hProcess, m_localPlayer + Offsets::team);
			if (entityTeam == localTeam || entity == m_localPlayer)
				continue;
		}
		else
		{
			if (entity == m_localPlayer)
				continue;
		}

		const auto entityHealth = Utils::ReadMemory<int>(m_hProcess, entity + Offsets::health);
		if (entityHealth <= 0)
			continue;

		const auto entityHead = glm::vec3(
			Utils::ReadMemory<float>(m_hProcess, entity + Offsets::head_posx),
			Utils::ReadMemory<float>(m_hProcess, entity + Offsets::head_posy),
			Utils::ReadMemory<float>(m_hProcess, entity + Offsets::head_posz)
		);

		const auto entityFoot = Utils::ReadMemory<glm::vec3>(m_hProcess, entity + Offsets::feet_posx);

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

			ImGui::PushFont(nullptr);
			ImGui::GetBackgroundDrawList()->AddText(ImVec2(top.x - width / 2, bottom.y), IM_COL32(255, 255, 255, 255),
				entityName->data());
		}
	}
}
