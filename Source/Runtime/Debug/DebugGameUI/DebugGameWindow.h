#pragma once

#include <Debug/DebugWindow.h>

#include <imgui/imgui.h>

#include <Record/RecordLibrary.h>

class DebugGameWindow : public DebugWindow {
    RecordLibrary* m_recLib;
protected:

    virtual void OnUpdate() override {
        if(ImGui::IsKeyPressed(ImGuiKey_GraveAccent, false)) {
            SetVisibility(!IsVisible());
        }
    }

    virtual void OnRender() override {
        ImGuiWindowFlags winFlags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove; 
        ImGui::Begin("Debug", nullptr, winFlags);

        ImGui::Text("Debug");
        
        ImGui::End();
    }
public:
    DebugGameWindow() 
    {
        SetVisibility(false);
        m_recLib = GetService<RecordLibrary>();
    }
};