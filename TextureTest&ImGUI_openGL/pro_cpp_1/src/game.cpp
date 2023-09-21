#include <iostream> 
#include <random>
#include <string>
#include <vector> 
#include <sstream>
#include <fstream>
#include "game.h"

/*�Ĳ�С��Ϸ*/
namespace game {

	/*����   ��д��.h���汨���ˣ���ʾ�����������const*/
	int playerPointNow;
	int npcPointNow;
	int playerPointSum = 0;
	int npcPointSum = 0;
	std::vector<int> playerPoints;/*�������*/
	std::vector<int> npcPoints;/*��������*/
	int playerChips = 100 ;/*���ӵ�г���*/
	int npcChips = 100;
	int playerChipsSet = 0;/*����Ѿ���ע*/
	int npcChipsSet = 0;
	int Rounds = 4;/*���Ļغ�*/
	int stopGame = 0;
	std::string Username;

	/*���뱾���ļ��Ľṹ��*/
	struct PlayerData {
		int userId;
		std::string userName;
		int chipsSum; 
	};

	bool isNum(std::string str) {/*�ж������Ƿ�Ϊ����*/
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
		playerPoints.reserve(Rounds);/*��������*/
		npcPoints.reserve(Rounds); /*emplace_back*/

		playerPointSum = 0;
		npcPointSum = 0;
		playerChips = 100;/*���ӵ�г���*/
		npcChips = 100;
		playerChipsSet = 0;/*����Ѿ���ע*/
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
	 
	/*����չʾ*/
	void showPoints() {
		std::cout << "--------------------------" << std::endl;
		std::cout << "��ҵ�ǰ���ƣ�"; 
		for (auto it = playerPoints.begin(); it != playerPoints.end(); ++it) {

			int& v = *it; // ���ô���ֵ����

			std::cout << v << ",";

		}
		std::cout << std::endl;
		std::cout << "���Ե�ǰ���ƣ�";
		for (auto it = npcPoints.begin(); it != npcPoints.end(); ++it) {

			int& v = *it; // ���ô���ֵ����

			std::cout << v << ",";

		}
		std::cout << std::endl;
		std::cout << "��ҵ�ǰ�ܵ�����" << playerPointSum << std::endl;
		std::cout << "���Ե�ǰ�ܵ�����" << npcPointSum << std::endl;
		std::cout << " " << std::endl;
		std::cout << "�����Ͷע��" << playerChipsSet << " ʣ����룺" << playerChips << std::endl;
		std::cout << "������Ͷע��" << npcChipsSet << " ʣ����룺" << npcChips << std::endl;
		std::cout << "--------------------------" << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
	}

	/*npc����Ͷע*/
	int setChipNpc(int Round) { /*Round �ڼ��غ�*/
		if (npcPointSum > playerPointSum) {
			int t = npcPointSum - playerPointSum;
			if ( t > 20) {/*�������*/
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
			if (t > 20) {/*�������*/
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

		///*�������д�뱾��*/
		//std::ofstream outfile("userData.txt");
		//outfile << serialize(data);
		//outfile.close();

		if (playerPointSum > npcPointSum) {
			playerChips += npcChips;
			std::cout << "�����ǹ�ϲ�����...��ʤ�ˣ�Ӯ����" << npcChipsSet << "���" << std::endl;
			std::cout << " YOU WIN!!" << std::endl;
		}
		else {
			std::cout << "�����ˣ������� " << playerChipsSet << " ���";
			if (playerChipsSet < 0) { /* BUG */
				std::cout << "���⣬�㻹������ " << playerChipsSet - 100 << " ��ծ��" << std::endl;
			}
		}
		std::cout << std::endl;
		std::cout << "��q�Ƴ���Ϸ,��r�����Ĳ�������" << std::endl; 
		char x;
		std::cin >> x;
		if (x == 'q') {
			stopGame = 1;
		}
		else if (x == 'r') {
			std::cout << "������" << std::endl;
			std::cout << std::endl;
			std::cout << std::endl;
			std::cout << std::endl;
			std::cout << std::endl;
			std::cout << std::endl;
		}
		else {
			std::cout << "�㰴����" << std::endl; 
			std::cout << "-----------------------" << std::endl << std::endl << std::endl;
		}

	}

	void mainGame() {
		int chipNow = 0;
		initParamters();
		/*�Զģ���Ҫ����ƴ�������Ĵγ��ƣ�ǰ�������ƣ�������Ϊ���ƣ���ƴ������
		��˳�ӷ������ж��ӵ���������������һ�����ٷ�����������һ�����ٷ���*/

		std::cout << "����������ʼ��Ϸ" << std::endl;
		std::cin.get();
		for (int lun = 0; lun < Rounds; lun++) {
			/*���ƽ׶�*/
			std::cout << "���Խ���..." << std::endl;
			npcPointNow = pointsGet();/*����*/
			npcPoints.emplace_back(npcPointNow);/*����*/
			
			if (lun == Rounds - 1) { /*��������һ��*/
				std::cout << "�Ѿ������һ�֣����ֵ�������" << std::endl;
				std::cout << "-> " << "x" << std::endl;
				npcPointNow *= 2;
			}
			else {
				std::cout << "-> " << npcPointNow << std::endl;
			} 
			npcPointSum += npcPointNow;
			std::cout << "��ҽ���..." << std::endl;   
			/*std::cin.get();
			std::cin.clear();*/
			playerPointNow = pointsGet();/*����*/
			playerPoints.emplace_back(playerPointNow);/*����*/
			if (lun == Rounds - 1) {/*��������һ��*/
				std::cout << "-> " << playerPointNow << std::endl;
				playerPointNow *= 2;
			}
			else {
				std::cout << "-> " << playerPointNow << std::endl;
			}
			playerPointSum += playerPointNow;
			
			// Log("point: " + std::to_string(playerPointNow));
			/*Ͷע�׶�*/ 
			int t = setChipNpc(lun + 1);
			std::cout << "����Ͷע��" << t << std::endl;
			std::cout << "��ע��" << std::endl;
			// std::cin >> chipNow;  /*���ﲢ���ܱ�֤�����������*/
			std::string strIn;
			std::cin >> strIn;
			while (!isNum(strIn)) {
				std::cout << "������Ĳ�����Ч���֣����ٴ�����.." << std::endl;
				std::cin >> strIn;
			}
			chipNow = std::stoi(strIn);
			
			if (chipNow > playerChips) {
				std::cout << "��û����ô����룬�㽫��ծ����Ϣÿ��10%" << std::endl;
			}
			/*��ע�����ܼ�¼*/
			playerChips -= chipNow;
			playerChipsSet += chipNow;
			npcChips -= t;
			npcChipsSet += t;
			/*�������������¼*/ 
			/*����÷�,չʾ����*/
			showPoints(); 
		}

		/*������Ϸ*/ 
		endShow(); 
	}
}