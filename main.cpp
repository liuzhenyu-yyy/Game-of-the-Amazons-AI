// 亚马逊棋（Amazons）游戏
// 可选难度
// 作者：刘振宇
// 游戏信息：http://www.botzone.org/games#Amazons


#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include<iomanip>
#include<fstream>
#include<cstring> 
#include<cmath>
#include<float.h>

#include"score.h"//头文件：score.h 包含了对当前局势进行打分的所有函数及其辅助函数


using namespace std;



//轮到color落子时，检查游戏是否结束,结束则返回true，并打胜者信息。
bool Check_End(int color, bool print_result) {	
	int flag = 0;
	for (int i = 0; i < GRIDSIZE; i++) {
		for (int j = 0; j < GRIDSIZE; j++) {
			if (gridInfo[i][j] == color) {
				for (int k = 0; k < 8; k++) {
					if (inMap(i + dx[k], j + dy[k]) && gridInfo[i + dx[k]][j + dy[k]] == 0)
						flag = 1;
				}
			}
		}
	}
	if (flag == 0 && print_result) {
		cout << " Game over!!!!!" << endl;
		if (currBotColor == grid_white) {
			cout << " Black is the winner!!!!!" << endl;
		}
		else
			cout << " White is the winner!!!!!" << endl;
		cout << endl;
		return true;
	}
	return false;
}

//计数被锁住的棋子数
int Cntlocked(int color ) {
	int flag = 0, cnt = 0;
	for (int i = 0; i < GRIDSIZE; i++) {
		for (int j = 0; j < GRIDSIZE; j++) {
			if (gridInfo[i][j] == color) {
				flag = 0;
				for (int k = 0; k < 8; k++) {
					if (inMap(i + dx[k], j + dy[k]) && gridInfo[i + dx[k]][j + dy[k]] == 0)
						flag = 1;
				}
				if (flag == 0) {
					cnt++;
				}
			}
		}
	}
	return cnt;
}

 // 在屏幕上打印棋盘，棋子用"○"和"●"表示，障碍用"Δ"表示。
void PrintMap() {
	cout << "    0   1   2   3   4   5   6   7 " << endl;
	cout << " ┌───┬───┬───┬───┬───┬───┬───┬───┐" << endl;
	for (int j = 0; j < 8; j++) {
		cout << j;
		for (int i = 0; i < 8; i++) {
			cout << "│";
			switch (gridInfo[i][j]) {
			case grid_white:cout << " ●"; break;
			case grid_black:cout << " ○"; break;
			case OBSTACLE:cout << " Δ"; break;
			default:cout << "   "; break;
			}
		}
		cout << "│" << endl;
		if (j != 7) {
			cout << " ├───┼───┼───┼───┼───┼───┼───┼───┤" << endl;
		}
		else {
			cout << " └───┴───┴───┴───┴───┴───┴───┴───┘" << endl;
		}
	}
	return;
}

//打印某个数组的信息（仅供调试）
void PrintMatrix(int a[8][8]) {
	cout << "    0   1   2   3   4   5   6   7 " << endl;
	cout << " ┌───┬───┬───┬───┬───┬───┬───┬───┐" << endl;
	for (int j = 0; j < 8; j++) {
		cout << j;
		for (int i = 0; i < 8; i++) {
			cout << "│";
			cout << setw(3) << a[i][j];
		}
		cout << "│" << endl;
		if (j != 7) {
			cout << " ├───┼───┼───┼───┼───┼───┼───┼───┤" << endl;
		}
		else {
			cout << " └───┴───┴───┴───┴───┴───┴───┴───┘" << endl;
		}
	}
	return;
}

//策略1：随机战略下一步棋
void RandStep() {
	cout << "The computer is processing, please wait for a second:" << endl;
	int beginPos[3000][2], possiblePos[3000][2], obstaclePos[3000][2];		//计算机落子：随机数
	int posCount = 0, choice;
	for (int i = 0; i < GRIDSIZE; ++i) {
		for (int j = 0; j < GRIDSIZE; ++j) {	//i,j:起点坐标
			for (int k = 0; k < 8; ++k) {
				for (int delta1 = 1; delta1 < GRIDSIZE; delta1++) {
					int xx = i + dx[k] * delta1;
					int yy = j + dy[k] * delta1;
					if (gridInfo[xx][yy] != 0 || !inMap(xx, yy))	//xx,yy:终点坐标
						break;
					for (int l = 0; l < 8; ++l) {
						for (int delta2 = 1; delta2 < GRIDSIZE; delta2++) {
							int xxx = xx + dx[l] * delta2;
							int yyy = yy + dy[l] * delta2;
							if (!inMap(xxx, yyy))				//xxx,yyy:障碍坐标
								break;
							if (gridInfo[xxx][yyy] != 0 && !(i == xxx && j == yyy))
								break;
							if (ProcStep(i, j, xx, yy, xxx, yyy, currBotColor, true, false))
							{
								beginPos[posCount][0] = i;
								beginPos[posCount][1] = j;
								possiblePos[posCount][0] = xx;
								possiblePos[posCount][1] = yy;
								obstaclePos[posCount][0] = xxx;
								obstaclePos[posCount++][1] = yyy;
							}
						}

					}
				}

			}
		}
	}
	//cout << posCount << endl;
	int startX, startY, resultX, resultY, obstacleX, obstacleY;
	if (posCount > 0)
	{
		srand(time(0));
		choice = rand() % posCount;
		startX = beginPos[choice][0];
		startY = beginPos[choice][1];
		resultX = possiblePos[choice][0];
		resultY = possiblePos[choice][1];
		obstacleX = obstaclePos[choice][0];
		obstacleY = obstaclePos[choice][1];
	}
	else
	{
		startX = -1;
		startY = -1;
		resultX = -1;
		resultY = -1;
		obstacleX = -1;
		obstacleY = -1;
	}
	ProcStep(startX, startY, resultX, resultY, obstacleX, obstacleY, currBotColor, false, false);
	cout << "the movement of the ai is :" << endl;
	cout << startX << ' ' << startY << ' ' << resultX << ' ' << resultY << ' ' << obstacleX << ' ' << obstacleY << endl;

}

//策略2：得分最高策略下一步棋
void ScoreOnly(int color) {
	cout << "The computer is processing, please wait for a second:" << endl;
	int beginPos[3000][2], possiblePos[3000][2], obstaclePos[3000][2];		//计算机落子：随机数
	int posCount = 0, pos = 0;
	double score[3000] = { 0.0 }, max = DBL_MIN;
	int tmpgridinfo[GRIDSIZE][GRIDSIZE];
	for (int i = 0; i < GRIDSIZE; ++i) {
		for (int j = 0; j < GRIDSIZE; ++j) {	//i,j:起点坐标
			for (int k = 0; k < 8; ++k) {
				for (int delta1 = 1; delta1 < GRIDSIZE; delta1++) {
					int xx = i + dx[k] * delta1;
					int yy = j + dy[k] * delta1;
					if (gridInfo[xx][yy] != 0 || !inMap(xx, yy))	//xx,yy:终点坐标
						break;
					for (int l = 0; l < 8; ++l) {
						for (int delta2 = 1; delta2 < GRIDSIZE; delta2++) {
							int xxx = xx + dx[l] * delta2;
							int yyy = yy + dy[l] * delta2;
							if (!inMap(xxx, yyy))				//xxx,yyy:障碍坐标
								break;
							if (gridInfo[xxx][yyy] != 0 && !(i == xxx && j == yyy))
								break;
							if (ProcStep(i, j, xx, yy, xxx, yyy, currBotColor, true, false))
							{
								if (max == 99999)
									break;
								beginPos[posCount][0] = i;
								beginPos[posCount][1] = j;
								possiblePos[posCount][0] = xx;
								possiblePos[posCount][1] = yy;
								obstaclePos[posCount][0] = xxx;
								obstaclePos[posCount][1] = yyy;
								memcpy(tmpgridinfo, gridInfo, sizeof(gridInfo));
								ProcStep(i, j, xx, yy, xxx, yyy, currBotColor, false, false);
								score[posCount] = CalScore();
								if (color == grid_black)
									score[posCount] *= -1;
								if (Check_End(color * -1, false)) {
									score[posCount] = 99999;
								}
								if (score[posCount] > max) {
									pos = posCount;
								}
								memcpy(gridInfo, tmpgridinfo, sizeof(gridInfo));
								posCount++;
							}
						}
					}
				}
			}
		}
	}
	int startX, startY, resultX, resultY, obstacleX, obstacleY;
	startX = beginPos[pos][0];
	startY = beginPos[pos][1];
	resultX = possiblePos[pos][0];
	resultY = possiblePos[pos][1];
	obstacleX = obstaclePos[pos][0];
	obstacleY = obstaclePos[pos][1];
	ProcStep(startX, startY, resultX, resultY, obstacleX, obstacleY, currBotColor, false, false);
	cout << posCount << " possible steps analysised" << endl;
	cout << "the movement of the ai is :" << endl;
	cout << startX << ' ' << startY << ' ' << resultX << ' ' << resultY << ' ' << obstacleX << ' ' << obstacleY << endl;
}

//策略3：优先走能致胜的步，其次检查能不能锁住别人的棋子，在检查能不能减少自己被锁住的棋子，都不行，则打分，取打分最高的步骤
void Lock_Score(int color, int ID) {
	cout << "The computer is processing, please wait for a second:" << endl << endl;
	int beginPos[3000][2], possiblePos[3000][2], obstaclePos[3000][2];		//计算机落子：随机数
	int posCount = 0, pos = 0;
	int cnt1, cnt2;
	int mylock1, mylock2;
	double score[3000], max = -99999;
	for (int i = 0; i < 3000; i++) {
		score[i] = 0.0;
	}
	int tmpgridinfo[GRIDSIZE][GRIDSIZE];
	for (int i = 0; i < GRIDSIZE; ++i) {
		for (int j = 0; j < GRIDSIZE; ++j) {	//i,j:起点坐标
			for (int k = 0; k < 8; ++k) {
				for (int delta1 = 1; delta1 < GRIDSIZE; delta1++) {
					int xx = i + dx[k] * delta1;
					int yy = j + dy[k] * delta1;
					if (gridInfo[xx][yy] != 0 || !inMap(xx, yy))	//xx,yy:终点坐标
						break;
					for (int l = 0; l < 8; ++l) {
						for (int delta2 = 1; delta2 < GRIDSIZE; delta2++) {
							int xxx = xx + dx[l] * delta2;
							int yyy = yy + dy[l] * delta2;
							if (!inMap(xxx, yyy))				//xxx,yyy:障碍坐标
								break;
							if (gridInfo[xxx][yyy] != 0 && !(i == xxx && j == yyy))
								break;
							if (ProcStep(i, j, xx, yy, xxx, yyy, currBotColor, true, false))
							{
								cnt1 = Cntlocked(-color);
								mylock1 = Cntlocked(color);
								memcpy(tmpgridinfo, gridInfo, sizeof(gridInfo));
								ProcStep(i, j, xx, yy, xxx, yyy, currBotColor, false, false);
								if (Check_End(color * -1, false)) {		//对面输了，直接下这里
									ProcStep(i, j, xx, yy, xxx, yyy, currBotColor, false, false);
									cout << posCount << " possible steps analysised" << endl;
									cout << "the movement of the ai is :" << endl;
									cout << i << ' ' << j << ' ' << xx << ' ' << yy << ' ' << xxx << ' ' << yyy << endl;
									return;
								}
								cnt2 = Cntlocked(-currBotColor);
								if (cnt2 > cnt1) {	//不能致胜，但能锁住，直接下
									ProcStep(i, j, xx, yy, xxx, yyy, currBotColor, false, false);
									cout << posCount << " possible steps analysised" << endl;
									cout << "the movement of the ai is :" << endl;
									cout << i << ' ' << j << ' ' << xx << ' ' << yy << ' ' << xxx << ' ' << yyy << endl;
									return;
								}
								mylock2 = Cntlocked(color);
								if (mylock2 < mylock1) {
									ProcStep(i, j, xx, yy, xxx, yyy, currBotColor, false, false);
									//cout << i << ' ' << j << ' ' << xx << ' ' << yy << ' ' << xxx << ' ' << yyy << endl;
									return;
								}
								beginPos[posCount][0] = i;
								beginPos[posCount][1] = j;
								possiblePos[posCount][0] = xx;
								possiblePos[posCount][1] = yy;
								obstaclePos[posCount][0] = xxx;
								obstaclePos[posCount][1] = yyy;
								//cout << posCount << endl;
								//cout << "max=" << max << endl;
								//cout << i << ' ' << j << ' ' << xx << ' ' << yy << ' ' << xxx << ' ' << yyy << endl;
								if (ID <= 6) {			//前中后期比重
									score[posCount] = CalScore(0.22, 0.47, 0.13, 0.13, 0.05);
								}
								else if (ID <= 15) {
									score[posCount] = CalScore(0.30, 0.28, 0.20, 0.20, 0.02);
								}
								else {
									score[posCount] = CalScore(0.80, 0.10, 0.05, 0.05, 0.00);
								}
								if (color == grid_black)
									score[posCount] *= -1;
								if (score[posCount] > max && mylock1 <= mylock2) {
									pos = posCount;
									max = score[posCount];
								}
								//cout << "score=" << score[posCount] << endl << endl;
								memcpy(gridInfo, tmpgridinfo, sizeof(gridInfo));
								posCount++;
							}
						}
					}
				}
			}
		}
	}
	if (posCount == 0) {
		//cout << -1 << " " << -1 << " " << -1 << " " << -1 << " " << -1 << " " << -1 << endl;
		return;
	}
	int startX, startY, resultX, resultY, obstacleX, obstacleY;
	startX = beginPos[pos][0];
	startY = beginPos[pos][1];
	resultX = possiblePos[pos][0];
	resultY = possiblePos[pos][1];
	obstacleX = obstaclePos[pos][0];
	obstacleY = obstaclePos[pos][1];
	ProcStep(startX, startY, resultX, resultY, obstacleX, obstacleY, currBotColor, false, false);
	cout << posCount << " possible steps analysised" << endl;
	cout << "the movement of the ai is :" << endl;
	cout << startX << ' ' << startY << ' ' << resultX << ' ' << resultY << ' ' << obstacleX << ' ' << obstacleY << endl;
	return;
}



int main()
{
	int mode = 0, id = 0;
	char s;
	int x0, y0, x1, y1, x2, y2;
	cout << "The Game of the Amazons" << endl;
	cout << "              by Liu Zhenyu" << endl << endl;
	while (1) {
		//选择游戏模式
		cout << "  Please select game mode:" << endl;
		cout << " 1. man vs computer" << endl;
		cout << " 2. man vs man" << endl;
		cout << " 3. computer vs computer (for debugging)" << endl;
		cout << " 4. load saved game" << endl;
		cout << " 5. quit" << endl;
		cin >> mode;
		id = 0;
		if (mode == 5)
			break;

		// 初始化棋盘
		memset(gridInfo, 0, sizeof(gridInfo));
		gridInfo[0][(GRIDSIZE - 1) / 3] = gridInfo[(GRIDSIZE - 1) / 3][0]
			= gridInfo[GRIDSIZE - 1 - ((GRIDSIZE - 1) / 3)][0]
			= gridInfo[GRIDSIZE - 1][(GRIDSIZE - 1) / 3] = grid_black;
		gridInfo[0][GRIDSIZE - 1 - ((GRIDSIZE - 1) / 3)] = gridInfo[(GRIDSIZE - 1) / 3][GRIDSIZE - 1]
			= gridInfo[GRIDSIZE - 1 - ((GRIDSIZE - 1) / 3)][GRIDSIZE - 1]
			= gridInfo[GRIDSIZE - 1][GRIDSIZE - 1 - ((GRIDSIZE - 1) / 3)] = grid_white;

		switch (mode) {
		case 1: {//man vs computer
			cout << endl;
			cout << " You have chose to play with AI" << endl;
			int turn, diff;	//记录用户选择颜色
			cout << " Please select level of the computer" << endl;
			cout << " 1. very easy" << endl << " 2. middle" << endl << " 3. hard" << endl << " 4. Back to main interface" << endl << endl;
			cin >> diff;
			if (diff == 4) break;
			cout << " Do you want to play white or black?" << endl;
			cout << " Black moves first." << endl;
			cout << "1: Black   2:White    3:Back to main interface" << endl;
			cin >> turn;
			int first = turn;
			if (turn == 3) break;
			currBotColor = grid_black;
			PrintMap();
			char con = 'y';
			while (1) {
				if (turn == 1) {
					if (currBotColor == grid_white) {		//用户落子
						cout << "white's turn(●), please input your command:" << endl;
					}
					else {
						cout << "balck's turn(○), please input your command:" << endl;
					}
					cout << " or you can input 's' to save the game" << endl;
					int flag = 0;
					while (1) {
						cin >> s;
						if (s >= '0' && s < '8') {
							x0 = (int)(s - '0');
							cin >> y0 >> x1 >> y1 >> x2 >> y2;
						}
						else if (s == 's'){		//存盘
							ofstream fout("Archive.txt");
							for (int i = 0; i < GRIDSIZE; i++) {
								for (int j = 0; j < GRIDSIZE; j++) {
									fout << gridInfo[i][j] << " ";
								}
							}
							fout << first << " " << diff;
							fout.close();
							cout << "GridInfo saved, do you want to continue? " << endl;
							cout << " y / n" << endl;
							cin >> con;
							if (con == 'y') {
								if (currBotColor == grid_white) {		//用户落子
									cout << "white's turn(●), please input your command:" << endl;
								}
								else {
									cout << "balck's turn(○), please input your command:" << endl;
								}
								continue;
							}
							else {
								flag = 1;
								break;
							}
						}
						if (ProcStep(x0, y0, x1, y1, x2, y2, currBotColor, false, true)) {
							break;
						}
					}
					if (flag == 1) break;
					id++;
					PrintMap();
					currBotColor *= -1;
					if (Check_End(currBotColor,true)) {//检查是否结束
						break;
					}
				}
				else
					turn = 1;
					system("cls");
				switch (diff) {	//计算机落子
				case(1):RandStep(); break;					//难度为1：随机数
				case(2):ScoreOnly(currBotColor); break;		//难度为2：只考虑打分
				case(3):Lock_Score(currBotColor, id);		//难度为3：优先锁住棋子，剩下打分
				}
				id++;
				PrintMap();
				currBotColor *= -1;
				if (Check_End(currBotColor,true)) {
					break;
				}
			}
			break;
		}
		case 2: {
			PrintMap();
			currBotColor = grid_white;
			while (1) {
				if (currBotColor == grid_white) {
					cout << "white's turn(●), please input your command:" << endl;
				}
				else {
					cout << "balck's turn(○), please input your command:" << endl;
				}
				while (1) {
					cin >> x0 >> y0 >> x1 >> y1 >> x2 >> y2;
					if (ProcStep(x0, y0, x1, y1, x2, y2, currBotColor, false, true)) {
						break;
					}
				}
				PrintMap();
				currBotColor *= -1;
				if (Check_End(currBotColor,true)) {	//检查是否结束
					break;
				}
				if (currBotColor == grid_white) {
					cout << "white's turn(●), please input your command:" << endl;
				}
				else {
					cout << "balck's turn(○), please input your command:" << endl;
				}
				while (1) {
					cin >> x0 >> y0 >> x1 >> y1 >> x2 >> y2;
					if (ProcStep(x0, y0, x1, y1, x2, y2, currBotColor, false, true)) {
						break;
					}
				}
				PrintMap();
				currBotColor *= -1;
				if (Check_End(currBotColor,true)) {
					break;
				}
			}
			break;
		}
		case 3: {
			id = 0;
			currBotColor = grid_black;
			PrintMap();
			while(1) {
				//Lock_Score(currBotColor,id);
				RandStep();
				PrintMap();
				currBotColor *= -1;
				id++;
				if (Check_End(currBotColor, true)) {
					break;
				}
			}
			break;
		}
		case 4: {
			int turn, diff;	//记录用户选择颜色
			char con = 'y';
			ifstream fin("Archive.txt");
			for (int i = 0; i < GRIDSIZE; i++) {
				for (int j = 0; j < GRIDSIZE; j++) {
					fin >> gridInfo[i][j];
				}
			}
			fin >> turn >> diff;
			currBotColor = (turn == 1 ? grid_black : grid_white);
			PrintMap();
			while (1) {
				if (currBotColor == grid_white) {		//用户落子
					cout << "white's turn(●), please input your command:" << endl;
				}
				else {
					cout << "balck's turn(○), please input your command:" << endl;
				}
				cout << " or you can input 's' to save the game" << endl;
				int flag = 0;
				while (1) {
					cin >> s;
					if (s >= '0' && s < '8') {
						x0 = (int)(s - '0');
						cin >> y0 >> x1 >> y1 >> x2 >> y2;
					}
					else if (s == 's') {		//存盘
						ofstream fout("Archive.txt");
						for (int i = 0; i < GRIDSIZE; i++) {
							for (int j = 0; j < GRIDSIZE; j++) {
								fout << gridInfo[i][j] << " ";
							}
						}
						fout << turn << " " << diff;
						fout.close();
						cout << "GridInfo saved, do you want to continue? " << endl;
						cout << " y / n" << endl;
						cin >> con;
						if (con == 'y') {
							if (currBotColor == grid_white) {		//用户落子
								cout << "white's turn(●), please input your command:" << endl;
							}
							else {
								cout << "balck's turn(○), please input your command:" << endl;
							}
							continue;
						}
						else {
							flag = 1;
							break;
						}
					}
					if (ProcStep(x0, y0, x1, y1, x2, y2, currBotColor, false, true)) {
						break;
					}
				}
				if (flag == 1) break;
				id++;
				PrintMap();
				currBotColor *= -1;
				if (Check_End(currBotColor, true)) {//检查是否结束
					break;
				}

				switch (diff) {	//计算机落子
				case(1):RandStep(); break;					//难度为1：随机数
				case(2):ScoreOnly(currBotColor); break;		//难度为2：只考虑打分
				case(3):Lock_Score(currBotColor, id);		//难度为3：优先锁住棋子，剩下打分
				}
				id++;
				PrintMap();
				currBotColor *= -1;
				if (Check_End(currBotColor, true)) {
					break;
				}
			}
			break;
		}
		case 5:break;
		}
	}
	cout << "Thanks for using!" << endl;
	return 0;
}


//以下内容用于对打分函数的调试
/*int main() {	
	memset(gridInfo, 0, sizeof(gridInfo));
	gridInfo[0][(GRIDSIZE - 1) / 3] = gridInfo[(GRIDSIZE - 1) / 3][0]
		= gridInfo[GRIDSIZE - 1 - ((GRIDSIZE - 1) / 3)][0]
		= gridInfo[GRIDSIZE - 1][(GRIDSIZE - 1) / 3] = grid_black;
	gridInfo[0][GRIDSIZE - 1 - ((GRIDSIZE - 1) / 3)] = gridInfo[(GRIDSIZE - 1) / 3][GRIDSIZE - 1]
		= gridInfo[GRIDSIZE - 1 - ((GRIDSIZE - 1) / 3)][GRIDSIZE - 1]
		= gridInfo[GRIDSIZE - 1][GRIDSIZE - 1 - ((GRIDSIZE - 1) / 3)] = grid_white;
	PrintMap();
	//CalKingMove(grid_white);
	//PrintMatrix(KingMove);
	//CalScore(0.2,0.2,0.2,0.2,0.2);
	CalBlankMobility();
	PrintMatrix(BlankMobility);
	system("pause");
	return 0;
}*/
