#include "ContextFactory.h"

std::shared_ptr<Panel::ContextElement> Panel::ContextFactory::InitContext(pugi::xml_node root) {
	std::string rootName = root.name();
	if (rootName == "VerticalTabGroup") {
		auto element = std::make_shared<VerticalTabGroup>(root.attribute("name").as_string("group"));
		for (pugi::xml_node child : root.children()) {
			if (auto childContext = InitContext(child)) {
				element->AddElement(childContext->GetName(), childContext);
			}
		}
		return element;
	}

	if (rootName == "HorizontalTabGroup") {
		auto element = std::make_shared<HorizontalTabGroup>(root.attribute("name").as_string("group"));
		for (pugi::xml_node child : root.children()) {
			if (auto childContext = InitContext(child)) {
				element->AddElement(childContext->GetName(), childContext);
			}
		}
		return element;
	}

	if (rootName == "ContextElementGroup") {
		auto element = std::make_shared<ContextElementGroup>(root.attribute("name").as_string("group"));
		for (pugi::xml_node child : root.children()) {
			if (auto childContext = InitContext(child)) {
				element->AddElement(childContext->GetName(), childContext);
			}
		}
		return element;
	}

	if (rootName == "ButtonElement") {
		auto element = std::make_shared<ButtonElement>(root.attribute("label").as_string("noName"),
														root.attribute("action").as_string(""));
		return element;
	}

	if (rootName == "TextElement") {
		auto element = std::make_shared<TextElement>(root.attribute("label").as_string("noName"));
		return element;
	}

	return nullptr;
}