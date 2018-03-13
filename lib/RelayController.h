#ifndef RELAYCONTROLLER_H
#define RELAYCONTROLLER_H

#include "Tasks.h"
#include "TaskStatus.h"
#include "DigitalPins.h"
#include "ADServer.h"

class RelayController {
	public:
		RelayController(int *pins);

		void doTasks();
		void setTasks(Tasks *tasks);

	private:
		Tasks *_tasks;
		DigitalPins *_digitalPins;
};

RelayController::RelayController(int *pins) {
	_digitalPins = new DigitalPins(pins);
}

void RelayController::setTasks(Tasks *tasks) {
	_tasks = tasks;
}

void RelayController::doTasks() {
	if (_tasks != NULL) {
		for (int i = 0; i < _tasks->size(); ++i) {
			Task *task = _tasks->get(i);
			String log = "Task Id = " + String(task->getId()) + " RelayController::";
			int pinIndex = (int)task->getSubId() - 1;
			switch (task->getRelayState()) {
				case On:
					_digitalPins->pinOn(pinIndex);
					ADServer::log(log + "Pin On");
					_tasks->setStatus(i, TaskStatus::Do);
					break;
				case Off:
					_digitalPins->pinOff(pinIndex);
					ADServer::log(log + "Pin Off");
					break;
				case None: // Do nothing
					ADServer::log(log + "Do nothing");
					break;
				case OffAndDone: // Off and done
					_digitalPins->pinOff(pinIndex);
					ADServer::log(log + "Pin Off And Done Task");
					_tasks->setStatus(i, TaskStatus::Done);
					break;
			}
		}
	} else {
		ADServer::log("RelayController::_tasks is NULL");
	}
}

#endif

