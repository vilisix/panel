#include "ContextElement.h"

#include "ContextFactory.h"

Panel::ContextElement::ContextElement() : _name("emptyName"){
}

Panel::ContextElement::ContextElement(const std::string& name) : _name(name){}

Panel::ContextElementGroup::ContextElementGroup() : ContextElement() {}

Panel::ContextElementGroup::ContextElementGroup(const std::string& name) : ContextElement(name) {}

void Panel::ContextElementGroup::Update(Hotline::ActionSet& set) {
	for (const auto& element : _elements) {
		element.element->Update(set);
	} 
}

void Panel::ContextElementGroup::Reset() {
	ContextElement::Reset();
	_selectedIndex = 0;
	for (const auto& element : _elements) {
		element.element->Reset();
	} 
}

void Panel::ContextElementGroup::AddElement(const std::string& name, std::shared_ptr<ContextElement> element, const std::string& strKey, ImGuiKey hotkey) {
    _elements.push_back({name, std::move(element), strKey, hotkey});
}

Panel::ContextIndexedElementGroup::ContextIndexedElementGroup() : ContextElementGroup() {}

Panel::ContextIndexedElementGroup::ContextIndexedElementGroup(const std::string& name) : ContextElementGroup(name) {}

void Panel::ContextIndexedElementGroup::Update(Hotline::ActionSet& set) {
	for (int i = 0; i < _elements.size(); i++) {
		if (ImGui::IsKeyPressed(_elements[i].key)) {
			_selectedIndex = i;
			_elements[i].element->SetFocused(true);
		}

		if (i != _selectedIndex) {
			_elements[i].element->SetFocused(false);
		}

		_elements[i].element->Update(set);
	}
}

void Panel::ContextIndexedElementGroup::AddElement(const std::string& name, std::shared_ptr<ContextElement> element, const std::string& strKey, ImGuiKey hotkey) {
	std::string newStrKey = "";
	ImGuiKey newKey = ImGuiKey_None;
	if (element->IsSelectable()) {
		if(auto buttonPtr = std::dynamic_pointer_cast<ButtonElement>(element)) {
            if(_index != 0){
                newKey = ContextFactory::KeyFromInt(_index);
                newStrKey = std::to_string(_index);
                buttonPtr->SetKey(newStrKey, newKey);
                if(_index == 9){
                    _index = 0;
                }else{
                    _index++;
                }
            }
		}
    }
	_elements.push_back({name, std::move(element), newStrKey, newKey});
}

Panel::HorizontalTabGroup::HorizontalTabGroup() : ContextElementGroup() {}

Panel::HorizontalTabGroup::HorizontalTabGroup(const std::string& name) : ContextElementGroup(name) {}

void Panel::HorizontalTabGroup::Update(Hotline::ActionSet& set) {
	HandleKeyInput();
	auto contextSize = ImGui::GetContentRegionAvail();
	ImGui::BeginChild("tabs", {contextSize.x, contextSize.y * contextConfig.horizontalTabHeight}, false,
					contextConfig.childFlags);

	contextSize = ImGui::GetContentRegionAvail();
	ImVec2 buttonSize{contextSize.x / _elements.size(), contextSize.y};
	for (int i = 0; i < _elements.size(); i++)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, contextConfig.tabButtonColor);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, contextConfig.tabButtonColor);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, contextConfig.tabButtonColor);
		ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, contextConfig.tabButtonTextOffset);
		auto cursorPos = ImGui::GetCursorPos();
		ImGui::BeginChild((_elements[i].name + "child").c_str(), buttonSize, false, contextConfig.childFlags);
		auto contextSize = ImGui::GetContentRegionAvail();
		ImGui::BeginChild((_elements[i].name + "top").c_str(), {contextSize.x, contextSize.y * contextConfig.tabLinePos},
						false, contextConfig.childFlags);

		ImGui::SetWindowFontScale(contextConfig.windowHeaderScale * contextConfig.scaleFactor);

		ImVec4 textColor = (i == _selectedIndex) ? contextConfig.tabSelectedTextColor : contextConfig.tabNotSelectedColor;
		ImGui::TextColored(textColor, _elements[i].name.c_str());
		ImGui::EndChild();
		ImVec4 lineColor = (i == _selectedIndex) ? contextConfig.tabSelectedLineColor : contextConfig.tabNotSelectedColor;
		ImGui::PushStyleColor(ImGuiCol_ChildBg, lineColor);
		ImGui::PushStyleColor(ImGuiCol_Border, lineColor);
		ImGui::BeginChild((_elements[i].name + "line").c_str(),
						{contextSize.x, contextSize.y * contextConfig.tabLineSize}, false, contextConfig.childFlags);
		ImGui::EndChild();
		ImGui::PopStyleColor(2);
		ImGui::EndChild();

		ImGui::SetCursorPos(cursorPos);
		ImGui::BeginChild((_elements[i].name + "button_over_child").c_str(), buttonSize);
		if (ImGui::Button(_elements[i].name.c_str(), buttonSize))
		{
			if (_selectedIndex != i)
			{
				_elements[_selectedIndex].element->Reset();
			}
			_selectedIndex = i;
		}
		ImGui::EndChild();
		ImGui::PopStyleVar();

		if (i != _elements.size() - 1)
		{
			ImGui::SameLine();
		}
		ImGui::PopStyleColor(3);
	}
	ImGui::EndChild();

	ImGui::BeginChild("context", ImGui::GetContentRegionAvail(), true, contextConfig.childFlags);
	ImGui::SetWindowFontScale(contextConfig.windowFontScale * contextConfig.scaleFactor);

	_elements[_selectedIndex].element->Update(set);
	ImGui::EndChild();
}

void Panel::HorizontalTabGroup::HandleKeyInput() {
    if (ImGui::IsKeyPressed(ImGuiKey_RightArrow, true)) {
        _elements[_selectedIndex].element->Reset();
        _selectedIndex++;
        if(_selectedIndex == _elements.size()) _selectedIndex = 0;
    }
    if (ImGui::IsKeyPressed(ImGuiKey_LeftArrow, true)) {
        _elements[_selectedIndex].element->Reset();
        _selectedIndex--;
        if(_selectedIndex < 0) _selectedIndex = _elements.size() - 1;
    }
}

Panel::VerticalTabGroup::VerticalTabGroup() : ContextElementGroup() {}

Panel::VerticalTabGroup::VerticalTabGroup(const std::string& name) : ContextElementGroup(name) {}

void Panel::VerticalTabGroup::Update(Hotline::ActionSet& set) {
    HandleKeyInput();
    auto contextSize = ImGui::GetContentRegionAvail();
    ImGui::BeginChild("tabs", {contextSize.x * contextConfig.verticalTabWidth, contextSize.y}, false, contextConfig.childFlags);
	ImGui::SetWindowFontScale(contextConfig.windowFontScale * contextConfig.scaleFactor);


    contextSize = ImGui::GetContentRegionAvail();
    for(int i = 0; i < _elements.size(); i++){
        ImVec4 color = (i == _selectedIndex) ? contextConfig.tabSelectedColor : contextConfig.tabNotSelectedColor;
        ImVec4 hoveredColor = (i == _selectedIndex) ? contextConfig.tabSelectedColor : contextConfig.tabNotSelectedHoveredColor;
        ImVec2 buttonSelectedSize{contextSize.x, contextSize.y * contextConfig.verticalTabHeight};
        ImVec2 buttonSize{contextSize.x * contextConfig.verticalTabNotSelectedWidth, contextSize.y * contextConfig.verticalTabHeight};
	    ImVec2 buttonOrigSize = (i == _selectedIndex) ? buttonSelectedSize : buttonSize;
        ImGui::PushStyleColor(ImGuiCol_Button, color);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, color);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, hoveredColor);
        ImGui::NewLine();
        ImGui::SameLine((i == _selectedIndex) ? contextSize.x * contextConfig.verticalTabSelectedOffset : 0.f, 0.f);
        auto cursorPointForHotkey = ImGui::GetCursorPos();
        if (ImGui::Button(_elements[i].name.c_str(), buttonOrigSize)
            || ImGui::IsKeyPressed(_elements[i].key)){
            if(_selectedIndex != i){
                _elements[_selectedIndex].element->Reset();
            }
            _selectedIndex = i;
        }
        auto cursorPointAfterButton = ImGui::GetCursorPos();

        ImGui::PopStyleColor(3);

        if (_elements[i].key != ImGuiKey_None) {
			ImGui::SetWindowFontScale(contextConfig.windowHotkeyScale * contextConfig.scaleFactor);
            ImGui::SetCursorPos(cursorPointForHotkey);
            ImGui::TextColored(contextConfig.tabHotkeyColor, ("[" + _elements[i].stringKey + "]").c_str());
			ImGui::SetWindowFontScale(contextConfig.windowFontScale * contextConfig.scaleFactor);
        }
        ImGui::SetCursorPos(cursorPointAfterButton);
    }
    ImGui::EndChild();
    ImGui::SameLine(contextSize.x * (1 - contextConfig.verticalTabContextMinusOffset), 0.f);
	ImGui::PushStyleColor(ImGuiCol_ChildBg, contextConfig.tabSelectedColor);
	ImGui::PushStyleColor(ImGuiCol_Border, contextConfig.tabSelectedColor);
    ImGui::BeginChild("context", ImGui::GetContentRegionAvail(), true, contextConfig.childFlags);
    ImGui::PopStyleColor(2);
    _elements[_selectedIndex].element->Update(set);
    ImGui::EndChild();
}

void Panel::VerticalTabGroup::HandleKeyInput() {
    if (ImGui::IsKeyPressed(ImGuiKey_DownArrow, true)) {
        _elements[_selectedIndex].element->Reset();
        _selectedIndex++;
        if(_selectedIndex == _elements.size()) _selectedIndex = 0;
    }
    if (ImGui::IsKeyPressed(ImGuiKey_UpArrow, true)) {
        _elements[_selectedIndex].element->Reset();
        _selectedIndex--;
        if(_selectedIndex < 0) _selectedIndex = _elements.size() - 1;
    }
}

Panel::ButtonElement::ButtonElement(const std::string& label, float width, const std::string& action) : _action(action), _label(label), _width(width) {}

void Panel::ButtonElement::Update(Hotline::ActionSet& set) {
    auto contextMax = ImGui::GetContentRegionMax();
    auto buttonBeginPos = ImGui::GetCursorPos();
    if (_key != ImGuiKey_None) {
        ImGui::SetWindowFontScale(contextConfig.windowHotkeyScale * contextConfig.scaleFactor);
        ImGui::TextColored(contextConfig.buttonHotkeyColor, ("[" + _stringKey + "]").c_str());
        ImGui::SetWindowFontScale(contextConfig.windowFontScale * contextConfig.scaleFactor);
    }
    ImGui::SetCursorPos(buttonBeginPos);

	auto color = IsFocused() ? contextConfig.buttonFocusedColor : contextConfig.buttonRegularColor;
	auto hoveredColor = IsFocused() ? contextConfig.buttonFocusedHoveredColor : contextConfig.buttonHoveredColor;
    ImGui::PushStyleColor(ImGuiCol_Button, color);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, hoveredColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, color);
    if (ImGui::Button(_label.c_str(), {contextMax.x * _width, contextMax.y * contextConfig.buttonHeight})
		|| (IsFocused() && ImGui::IsKeyPressed(ImGuiKey_Enter))) {
        set.ExecuteAction(_action);
		SetFocused(false);
    }

    ImGui::PopStyleColor(3);
}

bool Panel::ButtonElement::IsSelectable() {
	return true;
}

void Panel::ButtonElement::SetKey(const std::string& keyStr, ImGuiKey key) {
	_key = key;
	_stringKey = keyStr;
}

void Panel::ButtonElement::Reset() {
    SetFocused(false);
}

Panel::TextElement::TextElement(const std::string &label) : _label(label) {}

void Panel::TextElement::Update(Hotline::ActionSet& set) {
    ImGui::Text(_label.c_str());
}

void Panel::SameLineElement::Update(Hotline::ActionSet& set) {
    ImGui::SameLine();
}
