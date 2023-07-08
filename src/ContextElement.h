#pragma once
#include <imgui.h>
#include <memory>
#include <vector>
#include <string>
#include "ActionSet.h"

namespace Panel {
	class ContextElement {
	public:
        ContextElement();
        explicit ContextElement(const std::string& name);
		virtual ~ContextElement() = default;
		virtual void Update() {}
		virtual void HandleKeyInput() {}
        virtual void Reset() {}
        std::string& GetName() { return _name; }
	protected:
        std::string _name;
	};

	class ContextElementGroup : public ContextElement {
	public:
        ContextElementGroup();
        explicit ContextElementGroup(const std::string& name);
        virtual ~ContextElementGroup() = default;

        void Update() override;
        void AddElement(const std::string &name, std::shared_ptr<ContextElement> element, const std::string& strKey = "", ImGuiKey hotkey = ImGuiKey_None);

        struct Element{
            std::string name;
            std::shared_ptr<ContextElement> element;
            std::string stringKey;
            ImGuiKey key;
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
        explicit ButtonElement(const std::string& label, const std::string& action, std::shared_ptr<Hotline::ActionSet> set);

        void Update() override;
        void HandleKeyInput() override;
    private:
        std::shared_ptr<Hotline::ActionSet> _set;
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