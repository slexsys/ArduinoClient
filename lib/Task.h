#ifndef TASK_H
#define TASK_H

#include "stdx.h"
#include "Logger.h"
#include "TaskStatus.h"
#include "RelayState.h"
#include "DateX.h"

class Task {
  private:
		static const char separator = ':';
		unsigned long _id;
		int _subId;
		int _type;
		unsigned long _date;
		unsigned long int _duration;
		TaskStatus _status;
		bool _changed;
		RelayState _lastRelayState;

  public:
		Task(unsigned long id, int subId, int type, unsigned long date, unsigned long  duration, TaskStatus status);
		static Task* fromString(String data);
		unsigned long getId();
		int getSubId();
		void update(Task *task);
		RelayState getRelayState();
		void setStatus(TaskStatus status);
		TaskStatus getStatus();
		bool isWork();
		String getDone();
		String toString();
};

Task::Task(unsigned long id, int subId, int type, unsigned long date, unsigned long  duration, TaskStatus status)
	: _id (id), _subId (subId), _type (type), _date (date), _duration (duration), _status(status), _changed(true), _lastRelayState(RelayState::None) {
}

Task* Task::fromString(String data) {
	int i = 0;
	String sid = stdx::getSplitByIndex(data, separator, i++);
	unsigned long id = strtoul(sid.c_str(), NULL, 10);

	String subId = stdx::getSplitByIndex(data, separator, i++);

	String type = stdx::getSplitByIndex(data, separator, i++);

	String date = stdx::getSplitByIndex(data, separator, i++);
	unsigned long dateul = strtoul(date.c_str(), NULL, 10);
	
	String duration = stdx::getSplitByIndex(data, separator, i++);
	unsigned long durationul = strtoul(duration.c_str(), NULL, 10);

	String status = stdx::getSplitByIndex(data, separator, i++);

	return new Task(id,
									subId.toInt(),
									type.toInt(),
									dateul,
									durationul,
									(TaskStatus)status.toInt());
}

unsigned long Task::getId() {
	return _id;
}

int Task::getSubId() {
	return _subId;
}

void Task::update(Task *task) {
	_subId = task->_subId;
	_type = task->_type;
	_date = task->_date;
	_duration = task->_duration;
	_status = task->_status;
	_changed = true;
}

RelayState Task::getRelayState() {
	RelayState state;
	if (DateX::isDoable(_date + _duration)) {
		state = RelayState::On;
	} else {
		state = RelayState::OffAndDone;//Off;
	}

	if (state != _lastRelayState) {
		_lastRelayState = state;
		return state;
	} else {
		return RelayState::None;
	}
}

bool Task::isWork() {
	if (DateX::isDoable(_date + _duration) && _status == TaskStatus::Do) {
		return true;
	} else {
		return false;
	}
}

void Task::setStatus(TaskStatus status) {
	_status = status;
}

TaskStatus Task::getStatus() {
	return _status;
}

String Task::getDone() {
	return DateX::getDone(_date);
}

String Task::toString() {
	char buf[100];
	sprintf(buf, "id=%lu, subId=%i, type=%i, date=%lu, duration=%lu, status=%i, changed=%s",
				_id, _subId, _type, _date, _duration, (int)_status, (_changed) ? "true" : "false");
	return String(buf);
}

#endif

