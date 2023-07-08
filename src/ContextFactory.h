#pragma once
#include <memory>
#include <pugixml.hpp>

#include "ContextElement.h"

namespace Panel {
	class ContextFactory {
	public:
		static std::shared_ptr<ContextElement> InitContext(pugi::xml_node root);
	};
}