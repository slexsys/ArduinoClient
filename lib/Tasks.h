#ifndef TASKS_H
#define TASKS_H

#include "stdx.h"
#include "Task.h"
#include "TaskStatus.h"
#include "LinkedList.h"

#define SEPERATOR ":"

class Tasks {
	private:
		static const char separator = '#';
		LinkedList<Task*> _tasks;

	public:
		static const char separator_update = ':';

		Tasks();
		static Tasks* fromString(String data);
		void add(Task *task);
		void append(Tasks *tasks);
		Task* get(int index);
		Task* getById(unsigned long id);
		void remove(int index);
		int size();
		String getIds();
		void setStatus(int index, TaskStatus status);
		String toString();
};

Tasks::Tasks() {
}

Tasks* Tasks::fromString(String data) {
	Tasks* tasks = new Tasks();
	int count = stdx::getCharCount(data, separator);

	for (int i = 0; i < count; ++i) {
		String str = stdx::getSplitByIndex(data, separator, i);
		Task* task = Task::fromString(str);
		tasks->add(task);
	}

	return tasks;
}

void Tasks::add(Task *task) {
	_tasks.add(task);
}

void Tasks::append(Tasks *tasks) {
	for (int i = 0; i < tasks->size(); ++i) {
		this->add(tasks->get(i));
	}
}

Task* Tasks::get(int index) {
	return _tasks.get(index);
}

Task* Tasks::getById(unsigned long id) {
	Task *task = NULL;
	for(int i = 0; i < _tasks.size(); ++i) {
		Task *tsk = _tasks.get(i);
		if (tsk->getId() == id) {
			task = tsk;
			break;
		}
	}
	return task;
}

void Tasks::remove(int index) {
	_tasks.remove(index);
	//
}

String Tasks::getIds() {
	String str = "";
	for(int i = 0; i < _tasks.size(); ++i) {
		Task *task = _tasks.get(i);
		str += String(task->getId()) + SEPERATOR;
	}
	str.remove(str.length() - 1);
	return str;
}

void Tasks::setStatus(int index, TaskStatus status) {
	Task *task = _tasks.get(index);
	task->setStatus(status);
}

String Tasks::toString() {
	String result = "Tasks {\n";
	for (int i = 0; i < _tasks.size(); ++i) {
		Task *task = _tasks.get(i);
		result += "\t[" + task->toString() + "]\n";
	}
	result += "}\n";
	return result;
}

int Tasks::size() {
	return _tasks.size();
}

#endif

