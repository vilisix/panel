#include "ContextElement.h"

void Panel::ContextElement::SetActive(bool isActive) {
    _state = isActive ? ContextElementState::Active : ContextElementState::Inactive;
}

void Panel::ContextElement::SetFocused(bool isFocused) {
    _isFocused = isFocused;
}

void Panel::ContextElementGroup::AddElement(const std::string& name, std::shared_ptr<ContextElement> element) {
    _elements.push_back({name, std::move(element)});
}

void Panel::HorizontalTabGroup::Update() {
    HandleKeyInput();
    auto contextSize = ImGui::GetContentRegionAvail();
    ImGui::BeginChild("tabs", {contextSize.x, contextSize.y * 0.1f}, true, ImGuiWindowFlags_NoTitleBar
                                                                           | ImGuiWindowFlags_NoMove
                                                                           | ImGuiWindowFlags_AlwaysAutoResize
                                                                           | ImGuiWindowFlags_NoScrollbar
                                                                           | ImGuiWindowFlags_NoNav);

    contextSize = ImGui::GetContentRegionAvail();
    ImVec2 buttonSize{contextSize.x / _elements.size() - _elements.size() * 2.0f, contextSize.y};
    for(int i = 0; i < _elements.size(); i++){
        ImVec4 selected = {0.4f, 0.5f, 0.2f, 1.0f};
        ImVec4 nonSelected = {0.2f, 0.2f, 0.2f, 1.0f};
        ImVec4 color = (i == _selectedIndex) ? selected : nonSelected;
        ImGui::PushStyleColor(ImGuiCol_Button, color);
        if (ImGui::Button(_elements[i]._name.c_str(), buttonSize)) {
            if(_selectedIndex != i){
                _elements[_selectedIndex]._element->Reset();
            }
            _selectedIndex = i;
        }
        if(i != _elements.size() - 1){
            ImGui::SameLine();
        }
        ImGui::PopStyleColor();
    }
    ImGui::EndChild();
    ImGui::BeginChild("context", ImGui::GetContentRegionAvail(), true, ImGuiWindowFlags_NoTitleBar
                                                                       | ImGuiWindowFlags_NoMove
                                                                       | ImGuiWindowFlags_AlwaysAutoResize
                                                                       | ImGuiWindowFlags_NoScrollbar
                                                                       | ImGuiWindowFlags_NoNav);
    ImGui::EndChild();
}

void Panel::HorizontalTabGroup::HandleKeyInput() {
    if (ImGui::IsKeyPressed(ImGuiKey_RightArrow, true)) {
        _selectedIndex++;
        if(_selectedIndex == _elements.size()) _selectedIndex = 0;
    }
    if (ImGui::IsKeyPressed(ImGuiKey_LeftArrow, true)) {
        _selectedIndex--;
        if(_selectedIndex < 0) _selectedIndex = _elements.size() - 1;
    }
}

void Panel::HorizontalTabGroup::Reset() {
    _selectedIndex = 0;
}

void Panel::VerticalTabGroup::Update() {
    HandleKeyInput();
    auto contextSize = ImGui::GetContentRegionAvail();
    ImGui::BeginChild("tabs", {contextSize.x * 0.2f, contextSize.y}, true, ImGuiWindowFlags_NoTitleBar
                                                                           | ImGuiWindowFlags_NoMove
                                                                           | ImGuiWindowFlags_AlwaysAutoResize
                                                                           | ImGuiWindowFlags_NoScrollbar
                                                                           | ImGuiWindowFlags_NoNav);

    contextSize = ImGui::GetContentRegionAvail();
    for(int i = 0; i < _elements.size(); i++){
        ImVec4 selected = {0.4f, 0.5f, 0.2f, 1.0f};
        ImVec4 nonSelected = {0.2f, 0.2f, 0.2f, 1.0f};
        ImVec4 color = (i == _selectedIndex) ? selected : nonSelected;
        ImGui::PushStyleColor(ImGuiCol_Button, color);
        if (ImGui::Button(_elements[i]._name.c_str(), {contextSize.x, 40.f})){
            if(_selectedIndex != i){
                _elements[_selectedIndex]._element->Reset();
            }
            _selectedIndex = i;
        }
        ImGui::PopStyleColor();
    }
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("context", ImGui::GetContentRegionAvail(), true, ImGuiWindowFlags_NoTitleBar
                                                                       | ImGuiWindowFlags_NoMove
                                                                       | ImGuiWindowFlags_AlwaysAutoResize
                                                                       | ImGuiWindowFlags_NoScrollbar
                                                                       | ImGuiWindowFlags_NoNav);
    _elements[_selectedIndex]._element->Update();
    ImGui::EndChild();
}

void Panel::VerticalTabGroup::HandleKeyInput() {
    if (ImGui::IsKeyPressed(ImGuiKey_DownArrow, true)) {
        _elements[_selectedIndex]._element->Reset();
        _selectedIndex++;
        if(_selectedIndex == _elements.size()) _selectedIndex = 0;
    }
    if (ImGui::IsKeyPressed(ImGuiKey_UpArrow, true)) {
        _elements[_selectedIndex]._element->Reset();
        _selectedIndex--;
        if(_selectedIndex < 0) _selectedIndex = _elements.size() - 1;
    }
}

void Panel::VerticalTabGroup::Reset() {
    _selectedIndex = 0;
}

Panel::ButtonElement::ButtonElement(const std::string& label, const std::string& action) : _action(action), _label(label) {}

void Panel::ButtonElement::Update() {
    ContextElement::Update();
}

void Panel::ButtonElement::HandleKeyInput() {
    ContextElement::HandleKeyInput();
}

Panel::TextElement::TextElement(const std::string &label) : _label(label) {}

void Panel::TextElement::Update() {
    ContextElement::Update();
}
