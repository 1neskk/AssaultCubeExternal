#include "Application.h"
#include "EntryPoint.h"
#include "Drawing.h"
#include "Renderer.h"

class ACExternal final : public Layer
{
public:
	void OnUpdate(const float ts) override
    {
		if (m_renderer.IsAttached())
		{
			m_renderer.OnUpdate();
		}

		if (GetAsyncKeyState(VK_END) & 1)
		{
			Application::Close();
		}

		if (GetAsyncKeyState(VK_INSERT) & 1)
		{
			if (Drawing::IsActive())
				Drawing::SetActive(false);
			else
				Drawing::SetActive(true);
		}
    }

	void OnImGuiRender() override
    {
		char fpsBuffer[16] = { 0 };
		std::snprintf(fpsBuffer, sizeof(fpsBuffer), "FPS: %.2f", ImGui::GetIO().Framerate);
		ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 100, 15.0f));
		ImGui::SetNextWindowSize(ImVec2(0.f, 0.f));

		ImGui::Begin("FPS", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
		ImGui::Text(fpsBuffer);
		ImGui::End();

		if (Drawing::IsActive())
		{
			ImGuiWindowFlags windowFlagsMenu = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
			ImGuiWindowFlags windowFlagsLoader = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;

			Style::Theme();

			ImGui::SetNextWindowSize(ImVec2(400, 250), ImGuiCond_Once);
			ImGui::SetNextWindowBgAlpha(0.80f);
			if (m_renderer.IsAttached())
			{
				ImGui::Begin("Nesk External", nullptr, windowFlagsMenu);
				ImGui::Text("Process ID: %d", m_renderer.GetProcessId());
				ImGui::Text("Base Address: 0x%X", m_renderer.GetBaseAddress());

				ImGui::Separator();

				ImGui::Checkbox("ESP", &m_renderer.m_bEsp);
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
				ImGui::Checkbox("Team ESP", &m_renderer.m_bTeamEsp);

				ImGui::Spacing();
				ImGui::Spacing();
				ImGui::Checkbox("No Recoil", &m_renderer.m_bNoRecoil);

				ImGui::SetCursorPos(ImVec2(10, 190));
				if (ImGui::Button("Detach"))
				{
					m_renderer.Detach();
					Application::Close();
				}
				ImGui::End();
			}
			else
			{
				ImGui::Begin("Nesk External Loader", nullptr, windowFlagsLoader);
				ImGui::Text("Attach to: ac_client.exe");
				if (ImGui::Button("Attach"))
					m_renderer.Attach(L"ac_client.exe");

				ImGui::SameLine();
				ImGui::SetCursorPosX(90.0f);

				if (ImGui::Button("Exit"))
					Application::Close();
				ImGui::End();
			}
		}

		if (m_renderer.IsAttached() && m_renderer.ToggleEsp())
			m_renderer.RenderEsp();
    }

private:
    Renderer m_renderer;
};

Application* CreateApplication(int argc, char** argv)
{
    Specs spec;
    spec.title = "AssaultCube External";

    auto app = new Application(spec);
    app->PushLayer<ACExternal>();

    return app;
}

