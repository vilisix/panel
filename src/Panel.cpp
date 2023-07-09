#include "Panel.h"

#include <pugixml.hpp>

#include "ContextFactory.h"

Panel::Panel::Panel(std::shared_ptr<Hotline::ActionSet> set)
	: _set(std::move(set)) {
    InitFromXml();
}

void Panel::Panel::NormalUpdate() {
    HandleKeyInput();

    if (_state == Inactive) {
        return;
    }

    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, config.childRounding);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, config.frameRounding);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, config.windowRounding);
    auto io = ImGui::GetIO();
    ImVec2 position{io.DisplaySize.x * config.windowPos.x, io.DisplaySize.y * config.windowPos.y};
    ImVec2 size{io.DisplaySize.x * config.windowSize.x, io.DisplaySize.y * config.windowSize.y};
    ImGui::SetNextWindowPos(position, ImGuiCond_Always, config.windowPivot);
    ImGui::SetNextWindowSize(size);
    ImGui::Begin("Panel", 0, config.windowFlags);
    ImGui::SetWindowFontScale(config.windowHeaderScale * config.scaleFactor);
	ImVec2 windowSize = ImGui::GetContentRegionAvail();
    ImGui::BeginChild("header", {windowSize.x * config.headerPanelSize.x, windowSize.y * config.headerPanelSize.y}, true, config.windowFlags);
    ImGui::Text("Panel - lightweight framework for action executing with xml layout");
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Button, config.exitButtonColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, config.exitButtonHoveredColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, config.exitButtonColor);
    ImGui::SetWindowFontScale(config.windowFontScale * config.scaleFactor);
    if (ImGui::Button("Close", {ImGui::GetContentRegionAvail().x, windowSize.y * config.headerPanelSize.y})){
        Toggle();
    }
    ImGui::PopStyleColor(3);

    ImGui::BeginChild("Context", ImGui::GetContentRegionAvail(), true, config.windowFlags);
    ImGui::SetWindowFontScale(1.0f);

	_rootElement->Update();
    ImGui::EndChild();
    ImGui::End();
    ImGui::PopStyleVar(3);
}

void Panel::Panel::ActionUpdate() {
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, config.childRounding);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, config.frameRounding);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, config.windowRounding);
    auto io = ImGui::GetIO();
    ImVec2 position{io.DisplaySize.x * config.windowPos.x, io.DisplaySize.y * config.windowPos.y};
    ImVec2 size{io.DisplaySize.x * config.actionWindowSize.x, io.DisplaySize.y * config.actionWindowSize.y};
    ImGui::SetNextWindowPos(position, ImGuiCond_Always, config.windowPivot);
    ImGui::SetNextWindowSize(size);
    ImGui::Begin("Panel", 0, config.windowFlags);
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
    if (ImGui::IsKeyPressed(config.toggleKey, false)) {
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
