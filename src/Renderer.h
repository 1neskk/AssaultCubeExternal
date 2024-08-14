#pragma once

#include <iostream>
#include <format>
#include <string>
#include <glm/glm.hpp>

#include "Offsets.h"
#include "Utils.h"

class Renderer
{
public:
    Renderer() = default;
    ~Renderer();

    auto Attach(const WCHAR* processName) -> void;
    void Detach() { this->~Renderer(); }
    bool IsAttached() const;

    void InfiniteHealth();
    void InfiniteAmmo();
    void RapidFire();

    static bool WorldToScreen(const glm::vec3& pos, glm::vec3& screen, const glm::mat4& viewMatrix);

    void RenderEsp();
    bool ToggleEsp() const { return m_bEsp; }
    bool ToggleTeamEsp() const { return m_bTeamEsp; }

    DWORD GetProcessId() const { return m_dwProcessId; }
    DWORD GetBaseAddress() const { return m_dwBaseAddress; }
    HANDLE GetProcess() const { return m_hProcess; }

public:
    bool m_bEsp = false;
    bool m_bTeamEsp = false;

private:
    DWORD m_dwProcessId = 0;
    DWORD m_dwBaseAddress = 0;
    HANDLE m_hProcess = nullptr;

    uintptr_t m_localPlayer = 0;
    uintptr_t m_entityList = 0;
    glm::mat4 m_viewMatrix = NULL;

    int m_oldHealth = 0;
    const int m_newHealth = 9999;

    int m_oldAmmo = 0;
    const int m_newAmmo = 999;

    float m_oldCooldown = 0;
    const float m_newCooldown = 0.0f;
};

