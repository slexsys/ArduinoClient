#ifndef DATEX_H
#define DATEX_H

class DateX {
	public:
		static void setDate(unsigned long server_date);
		static unsigned long getCurDate();
		static bool isDoable(unsigned long date);
		static String getDone(unsigned long date);

	private:
		static unsigned long _arduino_date;
		static unsigned long _server_date;

		static unsigned long getArduinoRunTime();
};

unsigned long DateX::_arduino_date;
unsigned long DateX::_server_date = 0;

void DateX::setDate(unsigned long server_date) {
	_server_date = server_date;
	_arduino_date = getArduinoRunTime();
}

unsigned long DateX::getCurDate() {
	if (_server_date != 0) {
		return _server_date + getArduinoRunTime() - _arduino_date;
	} else {
		return 0;
	}
}

bool DateX::isDoable(unsigned long date) {
	Logger::log("diff sec = " + String((int)date - (int)getCurDate()));
	if (date >= getCurDate()) {
		return true;
	} else {
		return false;
	}
}

String DateX::getDone(unsigned long date) {
	return String(getCurDate() - date);
}

unsigned long DateX::getArduinoRunTime() {
	return millis() / 1000;//(60 * 1000);
}

#endif

