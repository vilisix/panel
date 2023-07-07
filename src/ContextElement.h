#pragma once
#include <imgui.h>
#include <memory>
#include <vector>

namespace Panel {
	enum ContextElementState { Inactive, Active };
	class ContextElement {
	public:
		virtual ~ContextElement() = default;
		virtual void Update() = 0;
		virtual void HandleKeyInput() = 0;
		void SetActive(bool isActive);
	private:
		bool _isFocused = false;
		ContextElementState _state = Inactive;
	};

	class ContextElementGroup : public ContextElement {
	public:

	private:
		int _selectedIndex = 0;
		std::vector<ContextElement> _elements;
	};

	class HorizontalTabGroup : public ContextElementGroup {
		
	};

	class VerticalTabGroup : public ContextElementGroup {
		
	};

	class ButtonElement : public ContextElement {
		
	};

	class TextElement : public ContextElement {
		
	};
};