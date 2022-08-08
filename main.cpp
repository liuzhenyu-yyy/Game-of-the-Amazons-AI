// ����ѷ�壨Amazons����Ϸ
// ��ѡ�Ѷ�
// ���ߣ�������
// ��Ϸ��Ϣ��http://www.botzone.org/games#Amazons


#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include<iomanip>
#include<fstream>
#include<cstring> 
#include<cmath>
#include<float.h>

#include"score.h"//ͷ�ļ���score.h �����˶Ե�ǰ���ƽ��д�ֵ����к������丨������


using namespace std;



//�ֵ�color����ʱ�������Ϸ�Ƿ����,�����򷵻�true������ʤ����Ϣ��
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

//��������ס��������
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

 // ����Ļ�ϴ�ӡ���̣�������"��"��"��"��ʾ���ϰ���"��"��ʾ��
void PrintMap() {
	cout << "    0   1   2   3   4   5   6   7 " << endl;
	cout << " ���������Щ������Щ������Щ������Щ������Щ������Щ������Щ�������" << endl;
	for (int j = 0; j < 8; j++) {
		cout << j;
		for (int i = 0; i < 8; i++) {
			cout << "��";
			switch (gridInfo[i][j]) {
			case grid_white:cout << " ��"; break;
			case grid_black:cout << " ��"; break;
			case OBSTACLE:cout << " ��"; break;
			default:cout << "   "; break;
			}
		}
		cout << "��" << endl;
		if (j != 7) {
			cout << " ���������੤�����੤�����੤�����੤�����੤�����੤�����੤������" << endl;
		}
		else {
			cout << " ���������ة������ة������ة������ة������ة������ة������ة�������" << endl;
		}
	}
	return;
}

//��ӡĳ���������Ϣ���������ԣ�
void PrintMatrix(int a[8][8]) {
	cout << "    0   1   2   3   4   5   6   7 " << endl;
	cout << " ���������Щ������Щ������Щ������Щ������Щ������Щ������Щ�������" << endl;
	for (int j = 0; j < 8; j++) {
		cout << j;
		for (int i = 0; i < 8; i++) {
			cout << "��";
			cout << setw(3) << a[i][j];
		}
		cout << "��" << endl;
		if (j != 7) {
			cout << " ���������੤�����੤�����੤�����੤�����੤�����੤�����੤������" << endl;
		}
		else {
			cout << " ���������ة������ة������ة������ة������ة������ة������ة�������" << endl;
		}
	}
	return;
}

//����1�����ս����һ����
void RandStep() {
	cout << "The computer is processing, please wait for a second:" << endl;
	int beginPos[3000][2], possiblePos[3000][2], obstaclePos[3000][2];		//��������ӣ������
	int posCount = 0, choice;
	for (int i = 0; i < GRIDSIZE; ++i) {
		for (int j = 0; j < GRIDSIZE; ++j) {	//i,j:�������
			for (int k = 0; k < 8; ++k) {
				for (int delta1 = 1; delta1 < GRIDSIZE; delta1++) {
					int xx = i + dx[k] * delta1;
					int yy = j + dy[k] * delta1;
					if (gridInfo[xx][yy] != 0 || !inMap(xx, yy))	//xx,yy:�յ�����
						break;
					for (int l = 0; l < 8; ++l) {
						for (int delta2 = 1; delta2 < GRIDSIZE; delta2++) {
							int xxx = xx + dx[l] * delta2;
							int yyy = yy + dy[l] * delta2;
							if (!inMap(xxx, yyy))				//xxx,yyy:�ϰ�����
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

//����2���÷���߲�����һ����
void ScoreOnly(int color) {
	cout << "The computer is processing, please wait for a second:" << endl;
	int beginPos[3000][2], possiblePos[3000][2], obstaclePos[3000][2];		//��������ӣ������
	int posCount = 0, pos = 0;
	double score[3000] = { 0.0 }, max = DBL_MIN;
	int tmpgridinfo[GRIDSIZE][GRIDSIZE];
	for (int i = 0; i < GRIDSIZE; ++i) {
		for (int j = 0; j < GRIDSIZE; ++j) {	//i,j:�������
			for (int k = 0; k < 8; ++k) {
				for (int delta1 = 1; delta1 < GRIDSIZE; delta1++) {
					int xx = i + dx[k] * delta1;
					int yy = j + dy[k] * delta1;
					if (gridInfo[xx][yy] != 0 || !inMap(xx, yy))	//xx,yy:�յ�����
						break;
					for (int l = 0; l < 8; ++l) {
						for (int delta2 = 1; delta2 < GRIDSIZE; delta2++) {
							int xxx = xx + dx[l] * delta2;
							int yyy = yy + dy[l] * delta2;
							if (!inMap(xxx, yyy))				//xxx,yyy:�ϰ�����
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

//����3������������ʤ�Ĳ�����μ���ܲ�����ס���˵����ӣ��ڼ���ܲ��ܼ����Լ�����ס�����ӣ������У����֣�ȡ�����ߵĲ���
void Lock_Score(int color, int ID) {
	cout << "The computer is processing, please wait for a second:" << endl << endl;
	int beginPos[3000][2], possiblePos[3000][2], obstaclePos[3000][2];		//��������ӣ������
	int posCount = 0, pos = 0;
	int cnt1, cnt2;
	int mylock1, mylock2;
	double score[3000], max = -99999;
	for (int i = 0; i < 3000; i++) {
		score[i] = 0.0;
	}
	int tmpgridinfo[GRIDSIZE][GRIDSIZE];
	for (int i = 0; i < GRIDSIZE; ++i) {
		for (int j = 0; j < GRIDSIZE; ++j) {	//i,j:�������
			for (int k = 0; k < 8; ++k) {
				for (int delta1 = 1; delta1 < GRIDSIZE; delta1++) {
					int xx = i + dx[k] * delta1;
					int yy = j + dy[k] * delta1;
					if (gridInfo[xx][yy] != 0 || !inMap(xx, yy))	//xx,yy:�յ�����
						break;
					for (int l = 0; l < 8; ++l) {
						for (int delta2 = 1; delta2 < GRIDSIZE; delta2++) {
							int xxx = xx + dx[l] * delta2;
							int yyy = yy + dy[l] * delta2;
							if (!inMap(xxx, yyy))				//xxx,yyy:�ϰ�����
								break;
							if (gridInfo[xxx][yyy] != 0 && !(i == xxx && j == yyy))
								break;
							if (ProcStep(i, j, xx, yy, xxx, yyy, currBotColor, true, false))
							{
								cnt1 = Cntlocked(-color);
								mylock1 = Cntlocked(color);
								memcpy(tmpgridinfo, gridInfo, sizeof(gridInfo));
								ProcStep(i, j, xx, yy, xxx, yyy, currBotColor, false, false);
								if (Check_End(color * -1, false)) {		//�������ˣ�ֱ��������
									ProcStep(i, j, xx, yy, xxx, yyy, currBotColor, false, false);
									cout << posCount << " possible steps analysised" << endl;
									cout << "the movement of the ai is :" << endl;
									cout << i << ' ' << j << ' ' << xx << ' ' << yy << ' ' << xxx << ' ' << yyy << endl;
									return;
								}
								cnt2 = Cntlocked(-currBotColor);
								if (cnt2 > cnt1) {	//������ʤ��������ס��ֱ����
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
								if (ID <= 6) {			//ǰ�к��ڱ���
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
		//ѡ����Ϸģʽ
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

		// ��ʼ������
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
			int turn, diff;	//��¼�û�ѡ����ɫ
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
					if (currBotColor == grid_white) {		//�û�����
						cout << "white's turn(��), please input your command:" << endl;
					}
					else {
						cout << "balck's turn(��), please input your command:" << endl;
					}
					cout << " or you can input 's' to save the game" << endl;
					int flag = 0;
					while (1) {
						cin >> s;
						if (s >= '0' && s < '8') {
							x0 = (int)(s - '0');
							cin >> y0 >> x1 >> y1 >> x2 >> y2;
						}
						else if (s == 's'){		//����
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
								if (currBotColor == grid_white) {		//�û�����
									cout << "white's turn(��), please input your command:" << endl;
								}
								else {
									cout << "balck's turn(��), please input your command:" << endl;
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
					if (Check_End(currBotColor,true)) {//����Ƿ����
						break;
					}
				}
				else
					turn = 1;
					system("cls");
				switch (diff) {	//���������
				case(1):RandStep(); break;					//�Ѷ�Ϊ1�������
				case(2):ScoreOnly(currBotColor); break;		//�Ѷ�Ϊ2��ֻ���Ǵ��
				case(3):Lock_Score(currBotColor, id);		//�Ѷ�Ϊ3��������ס���ӣ�ʣ�´��
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
					cout << "white's turn(��), please input your command:" << endl;
				}
				else {
					cout << "balck's turn(��), please input your command:" << endl;
				}
				while (1) {
					cin >> x0 >> y0 >> x1 >> y1 >> x2 >> y2;
					if (ProcStep(x0, y0, x1, y1, x2, y2, currBotColor, false, true)) {
						break;
					}
				}
				PrintMap();
				currBotColor *= -1;
				if (Check_End(currBotColor,true)) {	//����Ƿ����
					break;
				}
				if (currBotColor == grid_white) {
					cout << "white's turn(��), please input your command:" << endl;
				}
				else {
					cout << "balck's turn(��), please input your command:" << endl;
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
			int turn, diff;	//��¼�û�ѡ����ɫ
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
				if (currBotColor == grid_white) {		//�û�����
					cout << "white's turn(��), please input your command:" << endl;
				}
				else {
					cout << "balck's turn(��), please input your command:" << endl;
				}
				cout << " or you can input 's' to save the game" << endl;
				int flag = 0;
				while (1) {
					cin >> s;
					if (s >= '0' && s < '8') {
						x0 = (int)(s - '0');
						cin >> y0 >> x1 >> y1 >> x2 >> y2;
					}
					else if (s == 's') {		//����
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
							if (currBotColor == grid_white) {		//�û�����
								cout << "white's turn(��), please input your command:" << endl;
							}
							else {
								cout << "balck's turn(��), please input your command:" << endl;
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
				if (Check_End(currBotColor, true)) {//����Ƿ����
					break;
				}

				switch (diff) {	//���������
				case(1):RandStep(); break;					//�Ѷ�Ϊ1�������
				case(2):ScoreOnly(currBotColor); break;		//�Ѷ�Ϊ2��ֻ���Ǵ��
				case(3):Lock_Score(currBotColor, id);		//�Ѷ�Ϊ3��������ס���ӣ�ʣ�´��
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


//�����������ڶԴ�ֺ����ĵ���
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
