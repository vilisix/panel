#include "Panel.h"

Panel::Panel::Panel(std::shared_ptr<Hotline::ActionSet> set)
	: _set(std::move(set)), _config(std::make_unique<Config>()) {}

Panel::Panel::Panel(std::shared_ptr<Hotline::ActionSet> set, std::unique_ptr<Config> config)
	: _set(std::move(set)), _config(std::move(config)) {}

void Panel::Panel::NormalUpdate() {
    HandleKeyInput();

    if (_state == Inactive) {
        return;
    }

    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, _config->childRounding);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, _config->frameRounding);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, _config->windowRounding);

    auto io = ImGui::GetIO();
    ImVec2 position{io.DisplaySize.x * _config->windowPos.x, io.DisplaySize.y * _config->windowPos.y};
    ImVec2 size{io.DisplaySize.x * _config->windowSize.x, io.DisplaySize.y * _config->windowSize.y};
    ImGui::SetNextWindowPos(position, ImGuiCond_Always, _config->windowPivot);
    ImGui::SetNextWindowSize(size);
    ImGui::Begin("HotlineWindow", 0, _config->windowFlags);
    ImGui::PopStyleVar(3);
    ImGui::End();
}

void Panel::Panel::ActionUpdate() {

}

void Panel::Panel::Update() {
    if (_state != WaitingForAction) {
        NormalUpdate();
    }else if (_state == WaitingForAction) {
        ActionUpdate();
    }
}

void Panel::Panel::Reset() {

}

void Panel::Panel::Toggle() {
    if(_state == Inactive) _state = Active;
    else if(_state == Active) _state = Inactive;
    if (_state == Active) {
		Reset();
    }
}

void Panel::Panel::HandleKeyInput() {
    if (ImGui::IsKeyPressed(_config->toggleKey, false)) {
        Toggle();
    }

    if (_state == Inactive) {
        return;
    }

    if (ImGui::IsKeyPressed(ImGuiKey_Escape, false)) {
        Toggle();
    }
}