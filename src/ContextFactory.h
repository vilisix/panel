#pragma once
#include <memory>
#include <pugixml.hpp>
#include <map>

#include "ContextElement.h"

namespace Panel {
	class ContextFactory {
	public:
		static std::shared_ptr<ContextElement> InitContext(pugi::xml_node root,
                                                           std::shared_ptr<Hotline::ActionSet> set,
                                                           std::function<void()> onClose);

        static ImGuiKey KeyFromString(const std::string& strKey);
        static ImGuiKey KeyFromInt(int intKey);
	};
}