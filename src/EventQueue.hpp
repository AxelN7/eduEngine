#pragma once
#include <functional>
#include <string>
#include <array>
#include <cstdint>
#include <stdexcept>
#include <iostream>
#ifndef DEBUG
#define DEBUG

using Listener = std::function<void(const std::string&)>;
class EventQueue
{
private:
	std::array<Listener, 256> listeners;					// Collection of listeners
	std::array<std::string, 256> queuedEvents;				// The event queue
	std::uint8_t numberOfEventsInQueue;
	std::uint8_t numberOfListeners;
public:
	EventQueue() : numberOfEventsInQueue(0), numberOfListeners(0) {}

	std::uint8_t RegisterListener(Listener listener)
	{
		if (numberOfListeners >= 256)
		{
#ifdef DEBUG
			throw std::runtime_error("Max limit of listeners reached!");
#else
			std::cerr << "Listener registration limit reached! \n";
			return 255;
#endif
		}

		listeners[numberOfListeners] = listener;
		return numberOfListeners++;
	}

	void DeregisterListener(uint8_t id)
	{
		if (id < numberOfListeners)
		{
			listeners[id] = nullptr;
		}
	}

	void Enqueue(std::string& event)
	{
		if (numberOfEventsInQueue >= 256)
		{
#ifdef DEBUG
			throw std::runtime_error("Event queue full!");
#else
			std::cerr << "Event queue full!\n";
			return;
#endif
		}

		queuedEvents[numberOfEventsInQueue++] = event;
	}

	void BroadcastAllEvents()
	{
		for (int i = 0; i < numberOfEventsInQueue; ++i)
		{
			const auto& event = queuedEvents[i];
			for (int j = 0; j < numberOfListeners; ++j)
			{
				if (listeners[j]) {
					listeners[j](event);
				}
			}
		}
		numberOfEventsInQueue = 0;							// Clear event queue after broadcasting
	}
};
#endif