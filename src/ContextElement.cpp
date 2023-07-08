#include "ContextElement.h"

Panel::ContextElement::ContextElement() : _name("emptyName"){
}

Panel::ContextElement::ContextElement(const std::string& name) : _name(name){}

void Panel::ContextElement::SetActive(bool isActive) {
    _state = isActive ? ContextElementState::Active : ContextElementState::Inactive;
}

void Panel::ContextElement::SetFocused(bool isFocused) {
    _isFocused = isFocused;
}

Panel::ContextElementGroup::ContextElementGroup() : ContextElement() {}

Panel::ContextElementGroup::ContextElementGroup(const std::string& name) : ContextElement(name) {}

void Panel::ContextElementGroup::Update() {
	for (const auto& element : _elements) {
		element._element->Update();
	} 
}

void Panel::ContextElementGroup::AddElement(const std::string& name, std::shared_ptr<ContextElement> element) {
    _elements.push_back({name, std::move(element)});
}

Panel::HorizontalTabGroup::HorizontalTabGroup() : ContextElementGroup() {}

Panel::HorizontalTabGroup::HorizontalTabGroup(const std::string& name) : ContextElementGroup(name) {}

void Panel::HorizontalTabGroup::Update() {
    HandleKeyInput();
    auto contextSize = ImGui::GetContentRegionAvail();
    ImGui::BeginChild("tabs", {contextSize.x, contextSize.y * 0.05f}, false, ImGuiWindowFlags_NoTitleBar
                                                                           | ImGuiWindowFlags_NoMove
                                                                           | ImGuiWindowFlags_AlwaysAutoResize
                                                                           | ImGuiWindowFlags_NoScrollbar
                                                                           | ImGuiWindowFlags_NoNav);

    contextSize = ImGui::GetContentRegionAvail();
    ImVec2 buttonSize{contextSize.x / _elements.size() - _elements.size() * 2.0f, contextSize.y};
    for(int i = 0; i < _elements.size(); i++){
        // ImVec4 selected = {0.5f, 0.5f, 0.4f, 1.0f};
        // ImVec4 nonSelected = {0.25f, 0.25f, 0.25f, 1.0f};
        // ImVec4 color = (i == _selectedIndex) ? selected : nonSelected;
        ImVec4 zeroColor = {0.f, 0.f, 0.f, 0.0f};
        ImGui::PushStyleColor(ImGuiCol_Button, zeroColor);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, zeroColor);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, zeroColor);
        ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, {500.f,500.f});
        auto cursorPos = ImGui::GetCursorPos();
        ImGui::BeginChild((_elements[i]._name + "child").c_str(), buttonSize, false, ImGuiWindowFlags_NoTitleBar
                                                                           | ImGuiWindowFlags_NoMove
                                                                           | ImGuiWindowFlags_AlwaysAutoResize
                                                                           | ImGuiWindowFlags_NoScrollbar
                                                                           | ImGuiWindowFlags_NoNav);
        auto contextSize = ImGui::GetContentRegionAvail();
        ImGui::BeginChild((_elements[i]._name + "top").c_str(), {contextSize.x, contextSize.y * 0.7f}, false, ImGuiWindowFlags_NoTitleBar
                                                                           | ImGuiWindowFlags_NoMove
                                                                           | ImGuiWindowFlags_AlwaysAutoResize
                                                                           | ImGuiWindowFlags_NoScrollbar
                                                                           | ImGuiWindowFlags_NoNav);
        ImVec4 selected = {1.f, 1.f, 1.f, 1.0f};
        ImVec4 nonSelected = {0.3f, 0.3f, 0.3f, 1.0f};
        ImVec4 color = (i == _selectedIndex) ? selected : nonSelected;
    	ImGui::TextColored(color, _elements[i]._name.c_str());
        ImGui::EndChild();

        ImVec4 selectedLine = {0.8f, 0.2f, 0.2f, 1.0f};
        ImVec4 nonSelectedLine = {0.25f, 0.25f, 0.25f, 0.3f};
        ImVec4 lineColor = (i == _selectedIndex) ? selectedLine : nonSelectedLine;
        ImGui::PushStyleColor(ImGuiCol_ChildBg, lineColor);
        ImGui::PushStyleColor(ImGuiCol_Border, lineColor);
    	ImGui::BeginChild((_elements[i]._name + "bottom").c_str(), {contextSize.x, contextSize.y * 0.1f}, false, ImGuiWindowFlags_NoTitleBar
                                                                           | ImGuiWindowFlags_NoMove
                                                                           | ImGuiWindowFlags_AlwaysAutoResize
                                                                           | ImGuiWindowFlags_NoScrollbar
                                                                           | ImGuiWindowFlags_NoNav);
        ImGui::EndChild();
        ImGui::PopStyleColor();
        ImGui::PopStyleColor();

        ImGui::EndChild();

        ImGui::SetCursorPos(cursorPos);
        ImGui::BeginChild((_elements[i]._name + "button_child").c_str(), buttonSize);
    	if (ImGui::Button(_elements[i]._name.c_str(), buttonSize)) {
            if(_selectedIndex != i){
                _elements[_selectedIndex]._element->Reset();
            }
            _selectedIndex = i;
        }
        ImGui::EndChild();
        ImGui::PopStyleVar();

        if(i != _elements.size() - 1){
            ImGui::SameLine();
        }
        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
    }
    ImGui::EndChild();
    ImGui::BeginChild("context", ImGui::GetContentRegionAvail(), true, ImGuiWindowFlags_NoTitleBar
                                                                       | ImGuiWindowFlags_NoMove
                                                                       | ImGuiWindowFlags_AlwaysAutoResize
                                                                       | ImGuiWindowFlags_NoScrollbar
                                                                       | ImGuiWindowFlags_NoNav);
    _elements[_selectedIndex]._element->Update();
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

Panel::VerticalTabGroup::VerticalTabGroup() : ContextElementGroup() {}

Panel::VerticalTabGroup::VerticalTabGroup(const std::string& name) : ContextElementGroup(name) {}

void Panel::VerticalTabGroup::Update() {
    HandleKeyInput();
    auto contextSize = ImGui::GetContentRegionAvail();
    ImGui::BeginChild("tabs", {contextSize.x * 0.2f, contextSize.y}, false, ImGuiWindowFlags_NoTitleBar
                                                                           | ImGuiWindowFlags_NoMove
                                                                           | ImGuiWindowFlags_AlwaysAutoResize
                                                                           | ImGuiWindowFlags_NoScrollbar
                                                                           | ImGuiWindowFlags_NoNav);

    contextSize = ImGui::GetContentRegionAvail();
    for(int i = 0; i < _elements.size(); i++){
        ImVec4 selected = {0.15f, 0.15f, 0.15f, 1.0f};
        ImVec4 nonSelected = {0.25f, 0.25f, 0.25f, 1.0f};
        ImVec4 color = (i == _selectedIndex) ? selected : nonSelected;
        ImVec2 buttonSelectedSize{contextSize.x, 60.f};
        ImVec2 buttonSize{contextSize.x * 0.95f, 60.f};
	    ImVec2 buttonOrigSize = (i == _selectedIndex) ? buttonSelectedSize : buttonSize;
        ImGui::PushStyleColor(ImGuiCol_Button, color);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, color);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color);
        ImGui::NewLine();
        ImGui::SameLine((i == _selectedIndex) ? 10.f : 0.f, 0.f);
        if (ImGui::Button(_elements[i]._name.c_str(), buttonOrigSize)){
            if(_selectedIndex != i){
                _elements[_selectedIndex]._element->Reset();
            }
            _selectedIndex = i;
        }
        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
    }
    ImGui::EndChild();
    ImGui::SameLine(contextSize.x * 0.98f, 0.f);
	ImGui::PushStyleColor(ImGuiCol_ChildBg, {0.15f, 0.15f, 0.15f, 1.0f});
	ImGui::PushStyleColor(ImGuiCol_Border, {0.15f, 0.15f, 0.15f, 1.0f});
    ImGui::BeginChild("context", ImGui::GetContentRegionAvail(), true, ImGuiWindowFlags_NoTitleBar
                                                                       | ImGuiWindowFlags_NoMove
                                                                       | ImGuiWindowFlags_AlwaysAutoResize
                                                                       | ImGuiWindowFlags_NoScrollbar
                                                                       | ImGuiWindowFlags_NoNav);
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
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
    if (ImGui::Button(_label.c_str())) {
	    //todo action
    }
}

void Panel::ButtonElement::HandleKeyInput() {
    ContextElement::HandleKeyInput();
}

Panel::TextElement::TextElement(const std::string &label) : _label(label) {}

void Panel::TextElement::Update() {
    ImGui::Text(_label.c_str());
}
