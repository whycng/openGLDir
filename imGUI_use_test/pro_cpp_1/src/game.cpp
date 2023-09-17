#include <iostream> 
#include <random>
#include <string>
#include <vector> 
#include <sstream>
#include <fstream>
#include "game.h"

/*赌博小游戏*/
namespace game {

	/*参数   我写在.h里面报错了，提示环球变量必须const*/
	int playerPointNow;
	int npcPointNow;
	int playerPointSum = 0;
	int npcPointSum = 0;
	std::vector<int> playerPoints;/*玩家手牌*/
	std::vector<int> npcPoints;/*电脑手牌*/
	int playerChips = 100 ;/*玩家拥有筹码*/
	int npcChips = 100;
	int playerChipsSet = 0;/*玩家已经下注*/
	int npcChipsSet = 0;
	int Rounds = 4;/*打四回合*/
	int stopGame = 0;
	std::string Username;

	/*存入本地文件的结构体*/
	struct PlayerData {
		int userId;
		std::string userName;
		int chipsSum; 
	};

	bool isNum(std::string str) {/*判断输入是否为数字*/
		if (std::all_of(str.begin(), str.end(), isdigit)) {
			// is number 
			return true;
		}
		return false;
	}

	//std::string serialize(const PlayerData& playerData) {
	//	std::ostringstream oss;

	//	oss << "\"userId\":\"" << playerData.userId << "\",";
	//	oss << R"("userName":")" << playerData.userName << "\",";
	//	oss << "\"chipsSum\":\"" << playerData.chipsSum << "\"";

	//	return oss.str();
	//}

	void initParamters() {  
		// playerChips = GetPlayerChips(Username) + 100;
		playerPoints.clear();
		npcPoints.clear();
		playerPoints.reserve(Rounds);/*根据轮数*/
		npcPoints.reserve(Rounds); /*emplace_back*/

		playerPointSum = 0;
		npcPointSum = 0;
		playerChips = 100;/*玩家拥有筹码*/
		npcChips = 100;
		playerChipsSet = 0;/*玩家已经下注*/
		npcChipsSet = 0;
	}

	//std::string loadPlayerData() {
	//	std::ifstream infile("userData.txt");
	//	std::string str;
	//	std::getline(infile, str);
	//	return str;
	//}
	//PlayerData deserialize(std::string str) {
	//	PlayerData data;
	//	std::istringstream iss(str);
	//	iss >> data.id >> rest;
	//	std::getline(iss, data.name);
	//	return data;
	//}

	//int GetPlayerChips(std::string Username) {

	//}
	 
	/*分数展示*/
	void showPoints() {
		std::cout << "--------------------------" << std::endl;
		std::cout << "玩家当前手牌："; 
		for (auto it = playerPoints.begin(); it != playerPoints.end(); ++it) {

			int& v = *it; // 引用代替值拷贝

			std::cout << v << ",";

		}
		std::cout << std::endl;
		std::cout << "电脑当前手牌：";
		for (auto it = npcPoints.begin(); it != npcPoints.end(); ++it) {

			int& v = *it; // 引用代替值拷贝

			std::cout << v << ",";

		}
		std::cout << std::endl;
		std::cout << "玩家当前总点数：" << playerPointSum << std::endl;
		std::cout << "电脑当前总点数：" << npcPointSum << std::endl;
		std::cout << " " << std::endl;
		std::cout << "玩家已投注：" << playerChipsSet << " 剩余筹码：" << playerChips << std::endl;
		std::cout << "电脑已投注：" << npcChipsSet << " 剩余筹码：" << npcChips << std::endl;
		std::cout << "--------------------------" << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
	}

	/*npc进行投注*/
	int setChipNpc(int Round) { /*Round 第几回合*/
		if (npcPointSum > playerPointSum) {
			int t = npcPointSum - playerPointSum;
			if ( t > 20) {/*最大优势*/
				int c = npcChips / (Rounds + 1 - Round);
				if (c < 1) c = 1;
				return c;
			} 
			else if (t >14 ){
				int c = npcChips / (Rounds + 1 - Round + 1);
				if (c < 1) c = 1;
				return c;
			}
			else if (t > 10) {
				int c = npcChips / (Rounds + 1 - Round + 2);
				if (c < 1) c = 1;
				return c;
			}
			else if (t > 5) {
				int c = npcChips / (Rounds + 1 - Round + 3);
				if (c < 1) c = 1;
				return c;
			}
			else {
				int c = npcChips / (Rounds + 1 - Round + 4);
				if (c < 1) c = 1;
				return c;
			}
		}
		else {
			int t = playerPointSum - npcPointSum;
			if (t > 20) {/*最大劣势*/
				int c = npcChips / (Rounds + 1 - Round);
				if (c < 1) c = 1;
				return c;
			}
			else if (t > 14) {
				int c = npcChips / (Rounds + 1 - Round + 1);
				if (c < 1) c = 1;
				return c;
			}
			else if (t > 10) {
				int c = npcChips / (Rounds + 1 - Round + 2);
				if (c < 1) c = 1;
				return c;
			}
			else if (t > 5) {
				int c = npcChips / (Rounds + 1 - Round + 3);
				if (c < 1) c = 1;
				return c;
			}
			else {
				int c = npcChips / (Rounds + 1 - Round + 4);
				if (c < 1) c = 1;
				return c;
			}
		}
	}

	int pointsGet() {

		std::random_device rd;
		std::default_random_engine e(rd());

		std::uniform_int_distribution<int> u(1, 12);

		return u(e);

	}

	template<typename T>
	void Log(T x) {
		std::cout << "log: " << x << std::endl;
	}

	void test() {
		std::cout << "test game.cpp " << std::endl;
	}

	void runGame() { 
		while (true)
		{
			if (stopGame == 1) {
				break;
			}
			mainGame();
		} 
	}

	void endShow() {

		//PlayerData playerData;
		//playerData.userId = ;
		//playerData.userName = ;
		//playerData.chipsSum = playerChips;

		///*玩家数据写入本地*/
		//std::ofstream outfile("userData.txt");
		//outfile << serialize(data);
		//outfile.close();

		if (playerPointSum > npcPointSum) {
			playerChips += npcChips;
			std::cout << "让我们恭喜这个比...获胜了，赢得了" << npcChipsSet << "金币" << std::endl;
			std::cout << " YOU WIN!!" << std::endl;
		}
		else {
			std::cout << "你输了，被爆了 " << playerChipsSet << " 金币";
			if (playerChipsSet < 0) { /* BUG */
				std::cout << "另外，你还背上了 " << playerChipsSet - 100 << " 的债务" << std::endl;
			}
		}
		std::cout << std::endl;
		std::cout << "按q推出游戏,按r继续赌博！！！" << std::endl; 
		char x;
		std::cin >> x;
		if (x == 'q') {
			stopGame = 1;
		}
		else if (x == 'r') {
			std::cout << "继续赌" << std::endl;
			std::cout << std::endl;
			std::cout << std::endl;
			std::cout << std::endl;
			std::cout << std::endl;
			std::cout << std::endl;
		}
		else {
			std::cout << "你按错了" << std::endl; 
			std::cout << "-----------------------" << std::endl << std::endl << std::endl;
		}

	}

	void mainGame() {
		int chipNow = 0;
		initParamters();
		/*对赌，主要规则拼点数，四次出牌，前三次明牌，第四张为暗牌，先拼点数，
		有顺子翻倍，有对子点数翻倍，有三张一样的再翻倍，有四张一样的再翻倍*/

		std::cout << "点击任意键开始游戏" << std::endl;
		std::cin.get();
		for (int lun = 0; lun < Rounds; lun++) {
			/*揭牌阶段*/
			std::cout << "电脑揭牌..." << std::endl;
			npcPointNow = pointsGet();/*揭牌*/
			npcPoints.emplace_back(npcPointNow);/*计牌*/
			
			if (lun == Rounds - 1) { /*如果是最后一轮*/
				std::cout << "已经是最后一轮，该轮点数翻倍" << std::endl;
				std::cout << "-> " << "x" << std::endl;
				npcPointNow *= 2;
			}
			else {
				std::cout << "-> " << npcPointNow << std::endl;
			} 
			npcPointSum += npcPointNow;
			std::cout << "玩家揭牌..." << std::endl;   
			/*std::cin.get();
			std::cin.clear();*/
			playerPointNow = pointsGet();/*揭牌*/
			playerPoints.emplace_back(playerPointNow);/*记牌*/
			if (lun == Rounds - 1) {/*如果是最后一轮*/
				std::cout << "-> " << playerPointNow << std::endl;
				playerPointNow *= 2;
			}
			else {
				std::cout << "-> " << playerPointNow << std::endl;
			}
			playerPointSum += playerPointNow;
			
			// Log("point: " + std::to_string(playerPointNow));
			/*投注阶段*/ 
			int t = setChipNpc(lun + 1);
			std::cout << "电脑投注：" << t << std::endl;
			std::cout << "下注：" << std::endl;
			// std::cin >> chipNow;  /*这里并不能保证输入的是数字*/
			std::string strIn;
			std::cin >> strIn;
			while (!isNum(strIn)) {
				std::cout << "你输入的不是有效数字，请再次输入.." << std::endl;
				std::cin >> strIn;
			}
			chipNow = std::stoi(strIn);
			
			if (chipNow > playerChips) {
				std::cout << "你没有这么多筹码，你将负债，利息每轮10%" << std::endl;
			}
			/*下注计入总记录*/
			playerChips -= chipNow;
			playerChipsSet += chipNow;
			npcChips -= t;
			npcChipsSet += t;
			/*点数计入分数记录*/ 
			/*计算得分,展示界面*/
			showPoints(); 
		}

		/*结算游戏*/ 
		endShow(); 
	}
}