#pragma once
#include <imgui.h>
#include <memory>

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
        float windowRounding = 10.0f;
        float frameRounding = 5.0f;
        float childRounding = 8.0f;
        ImVec2 windowPos = {0.5f, 0.5f};   // relative to display size
        ImVec2 windowPivot = {0.5f, 0.5f};
        ImVec2 windowSize = {0.9f, 0.9f};  // relative to display size
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar
                                       | ImGuiWindowFlags_NoMove
                                       | ImGuiWindowFlags_AlwaysAutoResize
                                       | ImGuiWindowFlags_NoScrollbar
                                       | ImGuiWindowFlags_NoNav;
	};

	class Panel {
	public:
		explicit Panel(std::shared_ptr<Hotline::ActionSet> set);

		Panel(std::shared_ptr<Hotline::ActionSet> set, std::unique_ptr<Config> config);

		virtual ~Panel() = default;

		void NormalUpdate();
		void ActionUpdate();
		virtual void Update();

		void Reset();
		void Toggle();

	private:
        void HandleKeyInput();

		State _state = Inactive;
        std::shared_ptr<Hotline::ActionSet> _set;
        std::unique_ptr<Config> _config;
	};
}