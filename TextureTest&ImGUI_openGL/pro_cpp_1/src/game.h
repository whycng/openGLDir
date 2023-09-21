#pragma once

namespace game {  
	/*º¯ÊýÉùÃ÷function*/
	bool isNum(std::string str);
	void showPoints();
	int setChipNpc(int Round);
	void initParamters();
	int pointsGet();
	template<typename T>
	void Log(T x);
	void test();
	void runGame();
	void mainGame();
	void endShow();
	std::string loadPlayerData();
}
