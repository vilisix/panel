#pragma once
#include <imgui.h>
#include <memory>
#include "ContextElement.h"

namespace Hotline {
	class ActionSet;
}

namespace Panel {
	enum State {
		Inactive,
		Active,
		WaitingForAction
	};

	struct Config {
		//  main
        ImGuiKey toggleKey = ImGuiKey_F2;
        //  window
        float scaleFactor = 1.0f;
        float windowFontScale = 1.6f;
        float windowHeaderScale = 1.f;

        float windowRounding = 10.0f;
        float frameRounding = 5.0f;
        float childRounding = 8.0f;
        ImVec2 windowPos = {0.5f, 0.5f};   // relative to display size
        ImVec2 windowPivot = {0.5f, 0.5f};
        ImVec2 windowSize = {0.98f, 0.98f};  // relative to display size
        ImVec2 actionWindowSize = {0.7f, 0.5f};  // relative to display size
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar
                                       | ImGuiWindowFlags_NoMove
                                       | ImGuiWindowFlags_AlwaysAutoResize
                                       | ImGuiWindowFlags_NoScrollbar
                                       | ImGuiWindowFlags_NoNav;
		ImVec2 headerPanelSize = {0.8f, 0.04f}; // relative to window size
		ImVec4 exitButtonColor = {0.8f, 0.2f, 0.25f, 0.8f};
		ImVec4 exitButtonHoveredColor = {0.8f, 0.2f, 0.25f, 1.0f};
	};

	static Config config;

	class Panel {
	public:
		explicit Panel(std::shared_ptr<Hotline::ActionSet> set);

		virtual ~Panel() = default;

        void InitFromXml();
		void NormalUpdate();
		void ActionUpdate();
		virtual void Update();

		void Reset();
		void Toggle();

	private:
        void HandleKeyInput();

		State _state = Inactive;
        std::shared_ptr<Hotline::ActionSet> _set;

        std::shared_ptr<ContextElement> _rootElement;
	};
}