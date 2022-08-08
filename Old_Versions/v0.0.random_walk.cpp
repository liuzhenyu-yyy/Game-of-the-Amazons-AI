// ����ѷ�壨Amazons����Ϸ
// ���Ŵ�ֲ���
// ���ߣ�������
// ��Ϸ��Ϣ��http://www.botzone.org/games#Amazons


#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <ctime> 

#define GRIDSIZE 8
#define OBSTACLE 2
#define judge_black 0
#define judge_white 1
#define grid_black 1
#define grid_white -1

using namespace std;

using namespace std;

int currBotColor; // ����ִ����ɫ��1Ϊ�ڣ�-1Ϊ�ף�����״̬��ͬ��
int gridInfo[GRIDSIZE][GRIDSIZE] = { 0 }; // ��x��y����¼����״̬
int dx[8] = { -1,-1,-1,0,0,1,1,1 };
int dy[8] = { -1,0,1,-1,1,-1,0,1 };

// �ж��Ƿ��ڵ�ͼ��
inline bool inMap(int x, int y)
{
	if (x < 0 || x >= GRIDSIZE || y < 0 || y >= GRIDSIZE)
		return false;
	return true;
}

//�ж�x0y0�ܷ񵽴�x1y1
bool Reachable(int x0, int y0, int x1, int y1) {
	int x = x1 - x0, y = y1 - y0;
	if (x != y && x != -y && x != 0 && y != 0) {
		return false;
	}
	int deltax, deltay;
	if (x == 0)
		deltax = 0;
	else if (x > 0)
		deltax = 1;
	else
		deltax = -1;
	if (y == 0)
		deltay = 0;
	else if (y > 0)
		deltay = 1;
	else
		deltay = -1;
	x0 += deltax; y0 += deltay;
	while (1) {
		if (x0 == x1 && y0 == y1)
			return true;
		if (gridInfo[x0][y0] != 0)
			return false;
		x0 += deltax; y0 += deltay;
	}
}



// �����괦���ӣ�����Ƿ�Ϸ���ģ������
bool ProcStep(int x0, int y0, int x1, int y1, int x2, int y2, int color, bool check_only)
{
	if ((!inMap(x0, y0)) || (!inMap(x1, y1)) || (!inMap(x2, y2)))
		return false;
	if (gridInfo[x0][y0] != color || gridInfo[x1][y1] != 0)
		return false;
	if ((gridInfo[x2][y2] != 0) && !(x2 == x0 && y2 == y0))
		return false;
	/*if (!Reachable(x0, y0, x1, y1)) {
		return false;
	}
	if (!Reachable(x1, y1, x2, y2)) {
		return false;
	}*/
	if (!check_only)
	{
		gridInfo[x0][y0] = 0;
		gridInfo[x1][y1] = color;
		gridInfo[x2][y2] = OBSTACLE;
	}
	return true;
}

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
int Cntlocked(int color) {
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


//�����������������
void Lock_random() {
	//cout << "The computer is processing, please wait for a second:" << endl;
	int beginPos[3000][2], possiblePos[3000][2], obstaclePos[3000][2];		//��������ӣ������
	int posCount = 0, choice = 0, cnt1, cnt2;
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
							if (ProcStep(i, j, xx, yy, xxx, yyy, currBotColor, true))
							{
								beginPos[posCount][0] = i;
								beginPos[posCount][1] = j;
								possiblePos[posCount][0] = xx;
								possiblePos[posCount][1] = yy;
								obstaclePos[posCount][0] = xxx;
								obstaclePos[posCount++][1] = yyy;
								cnt1 = Cntlocked(-currBotColor);
								ProcStep(i, j, xx, yy, xxx, yyy, currBotColor, false);
								cnt2 = Cntlocked(-currBotColor);
								if (cnt2 > cnt1) {
									cout << i << ' ' << j << ' ' << xx << ' ' << yy << ' ' << xxx << ' ' << yyy << endl;
									return;
								}
								else {
									gridInfo[i][j] = currBotColor;
									gridInfo[xx][yy] = 0;
									gridInfo[xxx][yyy] = 0;
								}
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
	//ProcStep(startX, startY, resultX, resultY, obstacleX, obstacleY, currBotColor, false);
	cout << startX << ' ' << startY << ' ' << resultX << ' ' << resultY << ' ' << obstacleX << ' ' << obstacleY << endl;
}



int main()
{
	int x0, y0, x1, y1, x2, y2;

	// ��ʼ������
	gridInfo[0][(GRIDSIZE - 1) / 3] = gridInfo[(GRIDSIZE - 1) / 3][0]
		= gridInfo[GRIDSIZE - 1 - ((GRIDSIZE - 1) / 3)][0]
		= gridInfo[GRIDSIZE - 1][(GRIDSIZE - 1) / 3] = grid_black;
	gridInfo[0][GRIDSIZE - 1 - ((GRIDSIZE - 1) / 3)] = gridInfo[(GRIDSIZE - 1) / 3][GRIDSIZE - 1]
		= gridInfo[GRIDSIZE - 1 - ((GRIDSIZE - 1) / 3)][GRIDSIZE - 1]
		= gridInfo[GRIDSIZE - 1][GRIDSIZE - 1 - ((GRIDSIZE - 1) / 3)] = grid_white;


	int turnID;
	cin >> turnID;

	// ���뵽��ǰ�غ�Ϊֹ���Լ��Ͷ��ֵ������ж����Ӷ��Ѿ���ָ�����ǰ�غ�
	currBotColor = grid_white; // �ȼ����Լ��ǰ׷�
	for (int i = 0; i < turnID; i++)
	{
		// ������Щ��������𽥻ָ�״̬����ǰ�غ�

		// �����Ƕ����ж�
		cin >> x0 >> y0 >> x1 >> y1 >> x2 >> y2;
		if (x0 == -1)
			currBotColor = grid_black; // ��һ�غ��յ�������-1, -1��˵�����Ǻڷ�
		else
			ProcStep(x0, y0, x1, y1, x2, y2, -currBotColor, false); // ģ��Է�����

																	// Ȼ�����Լ���ʱ���ж�
																	// �����ж��ܱ��Լ��ж���һ��
		if (i < turnID - 1)
		{
			cin >> x0 >> y0 >> x1 >> y1 >> x2 >> y2;
			if (x0 >= 0)
				ProcStep(x0, y0, x1, y1, x2, y2, currBotColor, false); // ģ�⼺������
		}
	}
	// �������ߣ���ֻ���޸����²��֣�
	//���þ��߷������ڴ����ߴ�����
	Lock_random();
	return 0;
}
