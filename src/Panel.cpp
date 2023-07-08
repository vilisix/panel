#include "Panel.h"

#include <pugixml.hpp>

#include "ContextFactory.h"

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
    ImGui::PushStyleColor(ImGuiCol_Button, {0.8f, 0.2f, 0.25f, 0.8f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {0.8f, 0.2f, 0.25f, 0.8f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, {0.8f, 0.2f, 0.25f, 0.8f});
    if (ImGui::Button("Close", {ImGui::GetContentRegionAvail().x, windowSize.y * 0.04f})){
        Toggle();
    }
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::BeginChild("Context", ImGui::GetContentRegionAvail(), false, _config->windowFlags);
    _rootElement->Update();
    ImGui::EndChild();
    ImGui::End();
    ImGui::PopStyleVar(3);
}

void Panel::Panel::ActionUpdate() {
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, _config->childRounding);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, _config->frameRounding);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, _config->windowRounding);
    auto io = ImGui::GetIO();
    ImVec2 position{io.DisplaySize.x * _config->windowPos.x, io.DisplaySize.y * _config->windowPos.y};
    ImVec2 size{io.DisplaySize.x * _config->windowSize.x, io.DisplaySize.y * _config->windowSize.y};
    ImGui::SetNextWindowPos(position, ImGuiCond_Always, _config->windowPivot);
    ImGui::SetNextWindowSize(size);
    ImGui::Begin("Panel", 0, _config->windowFlags);
    auto updateResult = _set->UpdateActionToFill();
    if (updateResult == Cancelled || updateResult == Provided) {
        _state = Inactive;
    }
    ImGui::End();
    ImGui::PopStyleVar(3);
}

void Panel::Panel::Update() {
    if(_set->HaveActionToFill()){
        _state = WaitingForAction;
    }
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
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("panel.xml");
    auto root = doc.root();
    _rootElement = std::move(ContextFactory::InitContext(doc.root().first_child(), _set));
}
