#pragma once
#include <entt/entt.hpp>
#include <entt/fwd.hpp>
#include "Event.hpp"

class Observer
{
public:
	virtual ~Observer() {};
	virtual void OnNotify(entt::entity source, Event event) = 0;
};
