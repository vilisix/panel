#include "Panel.h"

#include <pugixml.hpp>

#include "ContextFactory.h"
Panel::Panel::Panel() {
    InitFromXml();
}

void Panel::Panel::Draw(hotline::ActionSet& set) {
    HandleKeyInput();

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

    auto cursorPointBeforeButton = ImGui::GetCursorPos();

    ImGui::PushStyleColor(ImGuiCol_Button, config.exitButtonColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, config.exitButtonHoveredColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, config.exitButtonColor);
    ImGui::SetWindowFontScale(config.windowFontScale * config.scaleFactor);

    if (ImGui::Button("Close", {ImGui::GetContentRegionAvail().x, windowSize.y * config.headerPanelSize.y})){
        _onExitCallback();
    }
    ImGui::PopStyleColor(3);

    auto cursorPointAfterButton = ImGui::GetCursorPos();

	ImGui::SetWindowFontScale(contextConfig.windowHotkeyScale * contextConfig.scaleFactor);
    ImGui::SetCursorPos(cursorPointBeforeButton);
    ImGui::TextColored(config.hotkeyColor, ("[esc]"));
	ImGui::SetWindowFontScale(contextConfig.windowFontScale * contextConfig.scaleFactor);
    ImGui::SetCursorPos(cursorPointAfterButton);

    ImGui::BeginChild("Context", ImGui::GetContentRegionAvail(), true, config.windowFlags);
    ImGui::SetWindowFontScale(1.0f);

	_rootElement->Update(set);
    ImGui::EndChild();
    ImGui::End();
    ImGui::PopStyleVar(3);
}

void Panel::Panel::Reset() {
    _rootElement->Reset();
}

void Panel::Panel::SetExitCallback(std::function<void()> callback) {
    _onExitCallback = callback;
}

void Panel::Panel::HandleKeyInput() {
    if (ImGui::IsKeyPressed(ImGuiKey_Escape, false)) {
            _onExitCallback();
    }
}

void Panel::Panel::InitFromXml() {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("panel.xml");
    auto root = doc.root();
    _rootElement = std::move(ContextFactory::InitContext(doc.root().first_child()));
}
