#ifndef STDX_H
#define STDX_H

class stdx {
	public:
		static String getSplitByIndex(String data, char separator, int index);
		static int getCharCount(String data, char ch);
};

String stdx::getSplitByIndex(String data, char separator, int index) {
	int found = 0;
	int strIndex[] = { 0, -1 };
	int maxIndex = data.length() - 1;

	for (int i = 0; i <= maxIndex && found <= index; i++) {
		if (data.charAt(i) == separator || i == maxIndex) {
			found++;
			strIndex[0] = strIndex[1] + 1;
			strIndex[1] = (i == maxIndex) ? i+1 : i;
		}
	}
	return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

int stdx::getCharCount(String data, char ch) {
	int count = 0;
	int pos = data.indexOf(ch);
	while(pos != -1) {
		pos = data.indexOf(ch, pos + 1);
		count++;
	}
	return count + 1;
}

#endif

