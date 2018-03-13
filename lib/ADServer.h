#ifndef ADSERVER_H
#define ADSERVER_H

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#include "Logger.h"
#include "GO.h"

#define ANSWER_OK "ok"
#define _DEFAULT_LOG_ID "1"

class ADServer {
	public:
		static String answerOk;
		static void init();
		static String getDate();
		static String getTasks();
		static String getTasksUpdates(String ids);
		static String setTaskDone(String id);
		static String setTaskUpdated(String id);
		static String setTaskDoneDuration(String taskId, String interval);
		static String log(String text, String logId);

	private:
		static const String _id;
		static const String _url;
		static ESP8266WiFiMulti _WiFiMulti;
		static String getHTTPData(String url);
};

String ADServer::answerOk = ANSWER_OK;
const String ADServer::_id = GO::DevId;
const String ADServer::_url = GO::Url;
ESP8266WiFiMulti ADServer::_WiFiMulti;

void ADServer::init() {
	//init wifi
	WiFi.mode(WIFI_STA);
  _WiFiMulti.addAP(GO::Ssid, GO::Password);
}

String ADServer::getDate() {
	return getHTTPData(_url + "date");
}

String ADServer::getTasks() {
	return getHTTPData(_url + "tasks/" + _id);
}

String ADServer::getTasksUpdates(String ids) {
	return getHTTPData(_url + "tasks/" + _id + "/" + ids);
}

String ADServer::setTaskDone(String id) {
	return getHTTPData(_url + "task/done/" + id);
}

String ADServer::setTaskUpdated(String id) {
	return getHTTPData(_url + "task/updated/" + id);
}

String ADServer::setTaskDoneDuration(String taskId, String interval) {
	return getHTTPData(_url + "task/update?deviceId=" + _id +
											"&taskId=" + taskId +
											"&interval=" + interval);
}

String ADServer::log(String text, String logId = _DEFAULT_LOG_ID) {
	Logger::log(text);
	text.replace(" ", "%20");
	return getHTTPData(_url + "device/log?deviceId=" + _id +
														"&logId=" + logId +
														"&text=" + text);
}

String ADServer::getHTTPData(String url) {
  String result = "";
	//wait for WiFi connection
  if(_WiFiMulti.run() == WL_CONNECTED) {
			HTTPClient http;
      http.begin(url.c_str());

      int httpCode = http.GET();
      if(httpCode > 0) {
        if(httpCode == HTTP_CODE_OK) {
            result = http.getString();
        } else {
					Logger::log("[HTTP] GET... with code : " + String(httpCode));
				}
      } else {
          Logger::log("[HTTP] GET... failed, error: " + http.errorToString(httpCode));
      }
      http.end();
  }
  return result;
}

#endif

