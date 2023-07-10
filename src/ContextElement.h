#pragma once
#include <imgui.h>
#include <memory>
#include <vector>
#include <string>
#include "ActionSet.h"

namespace Panel {
    struct ContextConfig {
        // general
        float scaleFactor = 1.0f;
        float windowFontScale = 1.6f;
        float windowHeaderScale = 1.3f;
        float windowHotkeyScale = 1.0f;

	    ImGuiWindowFlags childFlags = ImGuiWindowFlags_NoTitleBar
                                   | ImGuiWindowFlags_NoMove
                                   | ImGuiWindowFlags_AlwaysAutoResize
                                   | ImGuiWindowFlags_NoScrollbar
                                   | ImGuiWindowFlags_NoNav;
        ImVec4 tabNotSelectedColor = {0.3f, 0.3f, 0.3f, 1.0f};
        ImVec4 tabNotSelectedHoveredColor = {0.35f, 0.35f, 0.35f, 1.0f};

        // horizontal tabs
        float horizontalTabHeight = 0.05f;
        ImVec4 tabButtonColor = {0.f, 0.f, 0.f, 0.0f};
        ImVec4 tabSelectedTextColor = {1.f, 1.f, 1.f, 1.0f};
        ImVec4 tabSelectedLineColor = {0.8f, 0.2f, 0.2f, 1.0f};
        ImVec2 tabButtonTextOffset = {500.f,500.f};
        float tabLinePos = 0.7f;
        float tabLineSize = 0.1f;

        // vertical tabs
        float verticalTabWidth = 0.2f;
        float verticalTabHeight = 0.05f;
        float verticalTabNotSelectedWidth = 0.93f;
        float verticalTabSelectedOffset = 0.035f;
        float verticalTabContextMinusOffset = 0.01f;
        ImVec4 tabSelectedColor = {0.15f, 0.15f, 0.15f, 1.0f};
        ImVec4 tabHotkeyColor = {0.5f, 0.5f, 0.5f, 1.0f};
        ImVec4 buttonHotkeyColor = {0.8f, 0.8f, 0.8f, 1.0f};

    	//button
        ImVec4 buttonHoveredColor = {0.35f, 0.35f, 0.35f, 0.8f};
        ImVec4 buttonRegularColor = {0.3f, 0.3f, 0.3f, 0.65f};
        ImVec4 buttonFocusedColor = {0.2f, 0.2f, 0.4f, 0.7f};
        ImVec4 buttonFocusedHoveredColor = {0.3f, 0.3f, 0.5f, 0.9f};
        float buttonHeight = 0.1f;

    };

    static ContextConfig contextConfig;

	class ContextElement {
	public:
        ContextElement();
        explicit ContextElement(const std::string& name);
		virtual ~ContextElement() = default;
		virtual void Update() {}
		virtual void HandleKeyInput() {}
		virtual bool IsSelectable() { return false; }
        virtual void Reset() { _isFocused = false; }
        std::string& GetName() { return _name; }
        //for focusing on buttons
        void SetFocused(bool isFocused) { _isFocused = isFocused; }
        bool IsFocused() { return _isFocused; }
	private:
        std::string _name;
        bool _isFocused = false;
	};

    class TextElement : public ContextElement {
    public:
        explicit TextElement(const std::string &label);

        void Update() override;
    private:
        std::string _label;
	};

    class ButtonElement : public ContextElement {
    public:
        explicit ButtonElement(const std::string& label, float width, const std::string& action, std::shared_ptr<Hotline::ActionSet> set, std::function<void()> onClose);

        void Update() override;
        void Reset() override;
        bool IsSelectable() override;
        void SetKey(const std::string& keyStr, ImGuiKey key);
    private:
        std::shared_ptr<Hotline::ActionSet> _set;
        std::string _action;
        std::string _label;
        std::string _stringKey;
        float _width = 0.5f;
        std::function<void()> _onCloseCallback;
        ImGuiKey _key = ImGuiKey_None;
	};

	class ContextElementGroup : public ContextElement {
	public:
        ContextElementGroup();
        explicit ContextElementGroup(const std::string& name);

        void Update() override;
        void Reset() override;
        virtual void AddElement(const std::string &name, std::shared_ptr<ContextElement> element, const std::string& strKey = "", ImGuiKey hotkey = ImGuiKey_None);

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

    class ContextIndexedElementGroup : public ContextElementGroup {
	public:
        ContextIndexedElementGroup();
        explicit ContextIndexedElementGroup(const std::string& name);

        void Update() override;
        void AddElement(const std::string &name, std::shared_ptr<ContextElement> element, const std::string& strKey = "", ImGuiKey hotkey = ImGuiKey_None) override;
    protected:
		int _index = 1;
    };

	class HorizontalTabGroup : public ContextElementGroup {
    public:
        HorizontalTabGroup();
        explicit HorizontalTabGroup(const std::string& name);
        void Update() override;
        void HandleKeyInput() override;
	};

	class VerticalTabGroup : public ContextElementGroup {
    public:
        VerticalTabGroup();
        explicit VerticalTabGroup(const std::string& name);

        void Update() override;
        void HandleKeyInput() override;
	};

    class SameLineElement : public ContextElement {
        void Update() override;
    };
};