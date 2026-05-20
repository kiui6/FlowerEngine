#pragma once

#include <Debug/DebugWindow.h>

#include <imgui/imgui.h>

class DebugGameWindow : public DebugWindow {
protected:
    virtual void OnRender() override {
        ImGuiWindowFlags winFlags = ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove; 
        ImGui::Begin("Debug", nullptr, winFlags);

        ImGui::Text("Debug");

        ImGui::End();
    }
};