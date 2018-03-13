#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include "ADServer.h"
#include "RelayController.h"
#include "Tasks.h"
#include "stdx.h"
#include "DateX.h"

class TasksController {
	public:
		TasksController(int *pins);
		void start();
		void stop();
		void restart();
		void setFirstSyncTime(int second);
		void setSecondSyncTime(int second);
		String getTasksList() { return _tasks->toString(); }

	private:
		int _dateSyncTime = 2000;
		int _firstSyncTime = 5000;//10000;
		int _secondSyncTime = 15000;//60000;
		int _syncCount = 20;

		Tasks *_tasks = NULL;
		RelayController *_relayController;

		bool _started = false;
		bool _dateSyncStatus = false;
		bool _firstSyncStatus = false;

		void startSync();
		void dateSync();
		void firstSync();
		void secondSync();
		void doTasks();
		void updateTasks(String data);
		void checkTasks();
		bool updateServerStatusAndDeleteTask(Task *task);
		void updateDoneTime();
};

TasksController::TasksController(int *pins) {
	ADServer::init();
	_relayController = new RelayController(pins);
}

void TasksController::start() {
	startSync();
}

void TasksController::stop() {
	_started = false;
	_dateSyncStatus = false;
	_firstSyncStatus = false;
}

void TasksController::restart() {
	stop();
	start();
}

void TasksController::setFirstSyncTime(int second) {
	_firstSyncTime = second * 1000;
}

void TasksController::setSecondSyncTime(int second) {
	_secondSyncTime = second * 1000;
}

void TasksController::startSync() {
	_started = true;
	while (_started) {
		if (!_dateSyncStatus) {
			dateSync();
		} else if (!_firstSyncStatus) {
			firstSync();
		} else {
			secondSync();			
			doTasks();
		}
	}
}

void TasksController::dateSync() {
	delay(_dateSyncTime);
	String data = ADServer::getDate();
	if (data != "") {
		Logger::log("sync date = " + data);
		ADServer::log("sync date [ok]");
		_dateSyncStatus = true;
		unsigned long date = strtoul(data.c_str(), NULL, 10);
		DateX::setDate(date);
	}
}

void TasksController::firstSync() {
	delay(_firstSyncTime);
	String data = ADServer::getTasks();
	if (data != "") {
		_firstSyncStatus = true;
		if (NULL != _tasks) { // update task for next interval
			Tasks *tasks = Tasks::fromString(data);
			_tasks->append(tasks);
			ADServer::log("sync first task update [ok]");
		} else { //first task init
			_tasks = Tasks::fromString(data);
			ADServer::log("sync first task [ok]");
		}
		_relayController->setTasks(_tasks);
		Logger::log(_tasks->toString());
	} else {
		Logger::log("no task in sync first");// restart...."); //exception ???
		//restart();
	}
}

void TasksController::secondSync() {
	delay(_secondSyncTime);
	String ids = _tasks->getIds();
	if (_tasks->size() > 0) {
		Logger::log("Ids : " + ids);
	} else {
		Logger::log("No task found");
	}
	String data = ADServer::getTasksUpdates(ids);
	if (data != "") {
		Logger::log("Loading data = [" + data + "]");
		ADServer::log("sync second task [ok]");
		updateTasks(data);
	} else {
		ADServer::log("sync second task: No data to update, restart controller");
		restart();
	}
}

void TasksController::updateTasks(String data) {
	char separator = Tasks::separator_update;
	int count = stdx::getCharCount(data, separator);
	
	for (int i = 0; i < count; ++i) {
		String str = stdx::getSplitByIndex(data, separator, i);
		//need to update task
	}
}

void TasksController::doTasks() {
	_relayController->doTasks();
	checkTasks();
	updateDoneTime();
}

void TasksController::checkTasks() {
	for (int i = _tasks->size() - 1; i >= 0; --i) {
		Task *task = _tasks->get(i);
		Logger::log("Check Tasks step: " + String(i));
		if (task->getStatus() == TaskStatus::Done) {
			Logger::log("Task number: " + String(i) + "is be update and delete");
			bool result = updateServerStatusAndDeleteTask(task);
			if (result) {
				_tasks->remove(i);
				Logger::log("Task remove is \t\t[ok]");
				ADServer::log("Task Id: [" + String(task->getId()) + "] is be DONE and delete from device tasks");
			}
		}
	}
}

bool TasksController::updateServerStatusAndDeleteTask(Task *task) {
	String id = String(task->getId());
	int count = _syncCount;
	Logger::log("Try connect ADServer to update id: " + id);
	while (count-- > 0) {
		Logger::logx(String(count));
		String data = ADServer::setTaskDone(id);
		if (data == ADServer::answerOk) {
			Logger::log("\nTask update from ADServer is \t\t[ok]");
			return true;
		}
		delay(100);
	}
	Logger::log("");
	return false;
}

void TasksController::updateDoneTime() {
	for (int i = 0; i < _tasks->size(); ++i) {
		Task *task = _tasks->get(i);
		if (task->isWork()) {
			Logger::log("updateDoneTime (is work) TaskId: " + String(task->getId()));
			String taskId = String(task->getId());
			String interval = task->getDone();
			ADServer::setTaskDoneDuration(taskId, interval);
		}
	}
}

#endif

