#ifndef GO_H
#define GO_H

#define _ID_					"5"
#define _URL_					"http://192.168.0.113:8080/api/"
#define _SSID_        "BS-Studio"
#define _PASSWORD_    "e_%27Hy8k"

class GO {
	public:
		static const String DevId;
		static const String Url;
		static const char* Ssid;
		static const char* Password;
};


const String GO::DevId = _ID_;
const String GO::Url = _URL_;
const char* GO::Ssid = _SSID_;
const char* GO::Password = _PASSWORD_;

#endif
