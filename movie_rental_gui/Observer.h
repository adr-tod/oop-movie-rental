#pragma once

#include <vector>

class Observer {
public:
	virtual void update() = 0;
};

class Observable {
	std::vector<Observer*> observers;
public:
	void add_observer(Observer* o);
	void notify_observers();
};