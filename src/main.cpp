#include "Application.h"
#include "EntryPoint.h"
#include "Drawing.h"
#include "Renderer.h"

class ACExternal final : public Layer
{
public:
    virtual void OnUpdate(const float ts) override
    {
		if (m_renderer.IsAttached())
		{
            m_renderer.InfiniteHealth();
			m_renderer.InfiniteAmmo();
			m_renderer.RapidFire();
		}

		if (GetAsyncKeyState(VK_INSERT) & 1)
		{
			Drawing draw;
			if (draw.IsActive())
			{
				draw.SetActive(false);
			}
			else
			{
				draw.SetActive(true);
			}
		}
    }

    virtual void OnImGuiRender() override
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
			ImGuiWindowFlags windowFlagsMenu;
			ImGuiWindowFlags windowFlagsLoader;
			windowFlagsMenu = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
			windowFlagsLoader = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;

			Style::Theme();

			ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiCond_Once);
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

				ImGui::SetCursorPos(ImVec2(10, 160));
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

		if (m_renderer.ToggleEsp())
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

