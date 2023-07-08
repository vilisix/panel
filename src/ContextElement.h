#pragma once
#include <imgui.h>
#include <memory>
#include <vector>
#include <string>

namespace Panel {
	class ContextElement {
	public:
        enum ContextElementState { Inactive, Active };
        ContextElement();
        explicit ContextElement(const std::string& name);
		virtual ~ContextElement() = default;
		virtual void Update() {}
		virtual void HandleKeyInput() {}
        virtual void Reset() {}
        std::string& GetName() { return _name; }
		void SetActive(bool isActive);
		void SetFocused(bool isFocused);
	protected:
        std::string _name;
		bool _isFocused = false;
		ContextElementState _state = ContextElementState::Inactive;
	};

	class ContextElementGroup : public ContextElement {
	public:
        ContextElementGroup();
        explicit ContextElementGroup(const std::string& name);
        virtual ~ContextElementGroup() = default;

        void Update() override;
        void AddElement(const std::string &name, std::shared_ptr<ContextElement> element);

        struct Element{
            std::string _name;
            std::shared_ptr<ContextElement> _element;
        };
    protected:
		int _selectedIndex = 0;
		std::vector<Element> _elements;

    };

	class HorizontalTabGroup : public ContextElementGroup {
    public:
        HorizontalTabGroup();
        explicit HorizontalTabGroup(const std::string& name);
        void Update() override;
        void HandleKeyInput() override;
        void Reset() override;
	};

	class VerticalTabGroup : public ContextElementGroup {
    public:
        VerticalTabGroup();
        explicit VerticalTabGroup(const std::string& name);

        void Update() override;
        void HandleKeyInput() override;
        void Reset() override;
	};

	class ButtonElement : public ContextElement {
    public:
        explicit ButtonElement(const std::string& label, const std::string& action);

        void Update() override;
        void HandleKeyInput() override;
    private:
        std::string _action;
        std::string _label;
	};

	class TextElement : public ContextElement {
    public:
        explicit TextElement(const std::string &label);

        void Update() override;
    private:
        std::string _label;
	};
};