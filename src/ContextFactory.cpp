#include "ContextFactory.h"

std::shared_ptr<Panel::ContextElement> Panel::ContextFactory::InitContext(pugi::xml_node root,
                                                                          std::shared_ptr<Hotline::ActionSet> set,
                                                                          std::function<void()> onClose) {
	std::string rootName = root.name();
	if (rootName == "VerticalTabGroup") {
		auto element = std::make_shared<VerticalTabGroup>(root.attribute("name").as_string("group"));
		for (pugi::xml_node child : root.children()) {
			if (auto childContext = InitContext(child, set, onClose)) {
                std::string stringKey = child.attribute("key").as_string("None");
                ImGuiKey hotkey = KeyFromString(stringKey);
                if(hotkey == ImGuiKey_None){
                    element->AddElement(childContext->GetName(), childContext);
                }else{
                    element->AddElement(childContext->GetName(), childContext, stringKey, hotkey);
                }
			}
		}
		return element;
	}

	if (rootName == "HorizontalTabGroup") {
		auto element = std::make_shared<HorizontalTabGroup>(root.attribute("name").as_string("group"));
		for (pugi::xml_node child : root.children()) {
			if (auto childContext = InitContext(child, set, onClose)) {
				element->AddElement(childContext->GetName(), childContext);
			}
		}
		return element;
	}

	if (rootName == "ContextElementGroup") {
		auto element = std::make_shared<ContextElementGroup>(root.attribute("name").as_string("group"));
		for (pugi::xml_node child : root.children()) {
			if (auto childContext = InitContext(child, set, onClose)) {
				element->AddElement(childContext->GetName(), childContext);
			}
		}
		return element;
	}

    if (rootName == "ContextIndexedElementGroup") {
		auto element = std::make_shared<ContextIndexedElementGroup>(root.attribute("name").as_string("group"));
		for (pugi::xml_node child : root.children()) {
			if (auto childContext = InitContext(child, set, onClose)) {
				element->AddElement(childContext->GetName(), childContext);
			}
		}
		return element;
	}

	if (rootName == "ButtonElement") {
		auto element = std::make_shared<ButtonElement>(root.attribute("label").as_string("noName"),
                                                       root.attribute("width").as_float(0.5f),
														root.attribute("action").as_string(""),
                                                        set,
                                                        onClose);
		return element;
	}

	if (rootName == "TextElement") {
		auto element = std::make_shared<TextElement>(root.attribute("label").as_string("noName"));
		return element;
	}

    if (rootName == "SameLineElement"){
        return std::make_shared<SameLineElement>();
    }

	return nullptr;
}

ImGuiKey Panel::ContextFactory::KeyFromString(const std::string &strKey) {
    static std::map<std::string, ImGuiKey> strToKey{
            {"None", ImGuiKey_None},
            {"A", ImGuiKey_A},
            {"B", ImGuiKey_B},
            {"C", ImGuiKey_C},
            {"D", ImGuiKey_D},
            {"E", ImGuiKey_E},
            {"F", ImGuiKey_F},
            {"G", ImGuiKey_G},
            {"H", ImGuiKey_H},
            {"I", ImGuiKey_I},
            {"J", ImGuiKey_J},
            {"K", ImGuiKey_K},
            {"L", ImGuiKey_L},
            {"M", ImGuiKey_M},
            {"N", ImGuiKey_N},
            {"O", ImGuiKey_O},
            {"P", ImGuiKey_P},
            {"Q", ImGuiKey_Q},
            {"R", ImGuiKey_R},
            {"S", ImGuiKey_S},
            {"T", ImGuiKey_T},
            {"U", ImGuiKey_U},
            {"V", ImGuiKey_V},
            {"W", ImGuiKey_W},
            {"X", ImGuiKey_X},
            {"Y", ImGuiKey_Y},
            {"Z", ImGuiKey_Z}
    };

    return strToKey[strKey];
}

ImGuiKey Panel::ContextFactory::KeyFromInt(int intKey) {
	static std::map<int, ImGuiKey> intToKey{
            {0, ImGuiKey_0},
            {1, ImGuiKey_1},
            {2, ImGuiKey_2},
            {3, ImGuiKey_3},
            {4, ImGuiKey_4},
            {5, ImGuiKey_5},
            {6, ImGuiKey_6},
            {7, ImGuiKey_7},
            {8, ImGuiKey_8},
            {9, ImGuiKey_9},
    };

    return intToKey[intKey];
}
