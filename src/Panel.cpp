#include "Panel.h"

Panel::Panel::Panel(std::shared_ptr<Hotline::ActionSet> set)
	: _set(std::move(set)), _config(std::make_unique<Config>()) {
    InitFromXml();
}

Panel::Panel::Panel(std::shared_ptr<Hotline::ActionSet> set, std::unique_ptr<Config> config)
	: _set(std::move(set)), _config(std::move(config)) {
    InitFromXml();
}

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
    ImGui::Begin("Panel", 0, _config->windowFlags);
    ImVec2 windowSize = ImGui::GetContentRegionAvail();
    ImGui::BeginChild("header", {windowSize.x * 0.8f, windowSize.y * 0.04f}, true, _config->windowFlags);
    ImGui::Text("Panel - lightweight framework for action executing with xml layout");
    ImGui::EndChild();
    ImGui::SameLine();
    if (ImGui::Button("Close", {ImGui::GetContentRegionAvail().x, windowSize.y * 0.04f})){
        Toggle();
    }
    ImGui::BeginChild("Context", ImGui::GetContentRegionAvail(), true, _config->windowFlags);
    _rootElement->Update();
    ImGui::EndChild();
    ImGui::End();
    ImGui::PopStyleVar(3);
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

void Panel::Panel::InitFromXml() {
    auto verTabGroup = std::make_shared<VerticalTabGroup>();
    std::vector<std::string> test{"alpha", "bravo", "charlie", "delta", "echo"};
    for (const auto& item: test){
        auto horTabGroup = std::make_shared<HorizontalTabGroup>();
        horTabGroup->AddElement(item + " 1", std::make_shared<ContextElement>());
        horTabGroup->AddElement(item + " 2", std::make_shared<ContextElement>());
        horTabGroup->AddElement(item + " 3", std::make_shared<ContextElement>());

        verTabGroup->AddElement(item, horTabGroup);
    }

    _rootElement = std::move(verTabGroup);
}
