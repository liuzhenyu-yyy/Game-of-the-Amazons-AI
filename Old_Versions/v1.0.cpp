// ����ѷ�壨Amazons����Ϸ
// ���Ŵ�ֲ���
// ���ߣ�������
// ��Ϸ��Ϣ��http://www.botzone.org/games#Amazons


#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cmath>

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

int KingMove[GRIDSIZE][GRIDSIZE];
int QueenMove[GRIDSIZE][GRIDSIZE];

//calkinmove�ĸ�����������һ���ڵ����BFS������������нڵ��kingmoveֵ��
void King_BFS(int x, int y, int cnt) {
	if (x < 0 || x >= GRIDSIZE || y < 0 || y >= GRIDSIZE) {
		return;
	}
	if (gridInfo[x][y] != 0 && cnt != 0) {
		return;
	}
	if (KingMove[x][y] <= cnt)
		return;
	KingMove[x][y] = cnt;
	for (int i = 0; i < 8; i++) {
		if (gridInfo[x + dx[i]][y + dy[i]] == 0)
			King_BFS(x + dx[i], y + dy[i], cnt + 1);
	}
	return;
}

//��ÿ��ڵ�Ķ�Ӧ��color��Kingmoveֵ�������
void CalKingMove(int color) {
	for (int i = 0; i < GRIDSIZE; i++) {
		for (int j = 0; j < GRIDSIZE; j++) {
			KingMove[i][j] = 999;
		}
	}
	for (int j = 0; j < GRIDSIZE; j++) {
		for (int i = 0; i < GRIDSIZE; i++) {
			if (gridInfo[i][j] == color) {
				King_BFS(i, j, 0);
			}
		}
	}
}

//calqueenmove�ĸ�����������һ���ڵ����BFS������������нڵ��kingmoveֵ��
void Queen_BFS(int x, int y, int cnt) {
	if (x < 0 || x >= GRIDSIZE || y < 0 || y >= GRIDSIZE) {
		return;
	}
	if (gridInfo[x][y] != 0 && cnt != 0) {
		return;
	}
	if (QueenMove[x][y] <= cnt)
		return;
	QueenMove[x][y] = cnt;
	int delta = 1;
	for (int i = 0; i < 8; i++) {
		delta = 1;
		while (inMap(x + dx[i] * delta, y + dy[i] * delta) && gridInfo[x + dx[i] * delta][y + dy[i] * delta] == 0) {
			if (gridInfo[x + dx[i] * delta][y + dy[i] * delta] >(cnt + 1)) {
				gridInfo[x + dx[i] * delta][y + dy[i] * delta] = cnt + 1;
			}
			Queen_BFS(x + dx[i] * delta, y + dy[i] * delta, cnt + 1);
			delta++;
		}
	}
	return;
}

//��ÿ��ڵ�Ķ�Ӧ��color��queenmoveֵ�������
void CalQueenMove(int color) {
	for (int i = 0; i < GRIDSIZE; i++) {
		for (int j = 0; j < GRIDSIZE; j++) {
			QueenMove[i][j] = 999;
		}
	}
	for (int i = 0; i < GRIDSIZE; i++) {
		for (int j = 0; j < GRIDSIZE; j++) {
			if (gridInfo[i][j] == color) {
				Queen_BFS(i, j, 0);
			}
		}
	}
}

//���㵱ǰ�����µ�territory����ֵ
int territory(int w[GRIDSIZE][GRIDSIZE], int b[GRIDSIZE][GRIDSIZE]) {
	int result = 0, p, q;
	for (int j = 0; j < GRIDSIZE; j++) {
		for (int i = 0; i < GRIDSIZE; i++) {
			p = w[i][j];
			q = b[i][j];
			if (p < q) {
				result++;
			}
			else if (p > q) {
				result--;
			}
		}
	}
	return result;
}

//����p1����ֵ
double position1(int w[GRIDSIZE][GRIDSIZE], int b[GRIDSIZE][GRIDSIZE]) {
	double result = 0.0;
	double p, q;
	for (int j = 0; j < GRIDSIZE; j++) {
		for (int i = 0; i < GRIDSIZE; i++) {
			p = w[i][j];
			q = b[i][j];
			result += pow(2.0, 0.0 - p);
			result -= pow(2.0, 0.0 - q);
		}
	}
	return result * 2;
}

//����p2����ֵ
double position2(int w[GRIDSIZE][GRIDSIZE], int b[GRIDSIZE][GRIDSIZE]) {
	double result = 0.0;
	double p, q, tmp;
	for (int j = 0; j < GRIDSIZE; j++) {
		for (int i = 0; i < GRIDSIZE; i++) {
			p = w[i][j];
			q = b[i][j];
			tmp = (q - p) / 6.0;
			tmp = (tmp > -1 ? tmp : -1);
			tmp = (tmp > 1 ? 1 : tmp);
			result += tmp;
		}
	}
	return result;
}

int BlankMobility[GRIDSIZE][GRIDSIZE] = { 0 };

//����ÿ���ո������
void CalBlankMobility() {
	memset(BlankMobility, 0, sizeof(BlankMobility));
	int m = 0;
	for (int j = 0; j < GRIDSIZE; j++) {
		for (int i = 0; i < GRIDSIZE; i++) {
			if (gridInfo[i][j] == 0) {
				m = 0;
				for (int k = 0; k < 8; k++) {
					if (inMap(i + dx[k], j + dy[k]) && gridInfo[i + dx[k]][j + dy[k]] == 0)
						m++;
				}
				BlankMobility[i][j] = m;
			}
		}
	}
}

//����ĳһ�����ӵ�����
double CalPieceMobility(int x, int y) {
	double result = 0.0;
	for (int delta = 1; delta <= 7; delta++) {
		for (int i = 0; i < 8; i++) {
			if (inMap(x + delta * dx[i], y + delta * dy[i]) && Reachable(x, y, x + delta * dx[i], y + delta * dy[i])) {
				result += (BlankMobility[x + delta * dx[i]][y + delta * dy[i]] * 2.0) / (delta * 2.0);
			}
		}
	}
	return result;
}

//������������ֵm��
double CalColorMobility() {
	double result = 0.0;
	for (int j = 0; j < GRIDSIZE; j++) {
		for (int i = 0; i < GRIDSIZE; i++) {
			if (gridInfo[i][j] == grid_white)
				result += CalPieceMobility(i, j);
			else if (gridInfo[i][j] == grid_black)
				result -= CalPieceMobility(i, j);
		}
	}
	return result;
}

//�����������ֵ���
double CalScore(double a = 0.8, double b = 0.1, double c = 0.05, double d = 0.05, double e = 0.00) {
	double score = 0.0;
	int KingMove_w[GRIDSIZE][GRIDSIZE];
	int KingMove_b[GRIDSIZE][GRIDSIZE];
	int QueenMove_w[GRIDSIZE][GRIDSIZE];
	int QueenMove_b[GRIDSIZE][GRIDSIZE];
	CalKingMove(grid_white);
	memcpy(KingMove_w, KingMove, sizeof(KingMove));
	CalKingMove(grid_black);
	memcpy(KingMove_b, KingMove, sizeof(KingMove));
	CalQueenMove(grid_white);
	memcpy(QueenMove_w, QueenMove, sizeof(QueenMove));
	CalQueenMove(grid_black);
	memcpy(QueenMove_b, QueenMove, sizeof(QueenMove));
	double t1, t2, p1, p2, m;
	t1 = territory(QueenMove_w, QueenMove_b);
	t2 = territory(KingMove_w, KingMove_b);
	p1 = position1(QueenMove_w, QueenMove_b);
	p2 = position2(KingMove_w, KingMove_b);
	CalBlankMobility();
	m = CalColorMobility();
	score = a * t1 + b * t2 + c * p1 + d * p2 + e * m;
	/*cout << "t1= " << t1 << endl;
	cout << "t1= " << t2 << endl;
	cout << "p1= " << p1 << endl;
	cout << "p2= " << p2 << endl;
	cout << " m = " << m << endl;
	cout << "score=" << score << endl;*/
	return score;
}

//�÷���߲�����һ����
void ScoreOnly(int color, int ID) {
	//cout << "The computer is processing, please wait for a second:" << endl;
	int beginPos[3000][2], possiblePos[3000][2], obstaclePos[3000][2];		//��������ӣ������
	int posCount = 0, pos = 0;
	double score[3000] = { 0.0 }, max = -99999;
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
							if (ProcStep(i, j, xx, yy, xxx, yyy, currBotColor, true))
							{
								if (max == 9999.9)
									break;
								beginPos[posCount][0] = i;
								beginPos[posCount][1] = j;
								possiblePos[posCount][0] = xx;
								possiblePos[posCount][1] = yy;
								obstaclePos[posCount][0] = xxx;
								obstaclePos[posCount][1] = yyy;
								memcpy(tmpgridinfo, gridInfo, sizeof(gridInfo));
								ProcStep(i, j, xx, yy, xxx, yyy, currBotColor, false);
								if (ID <= 7) {
									score[posCount] = CalScore(0.22, 0.47, 0.13, 0.13, 0.05);
								}
								else if (ID <= 17) {
									score[posCount] = CalScore(0.30, 0.28, 0.20, 0.20, 0.02);
								}
								else {
									score[posCount] = CalScore(0.80, 0.10, 0.05, 0.05, 0.00);
								}
								if (color == grid_black)
									score[posCount] *= -1;
								if (Check_End(color * -1, false)) {
									score[posCount] = 9999.9;
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
	//cout << posCount << " possible steps analysised" << endl;
	if (posCount == 0) {
		cout << -1 << " " << -1 << " " << -1 << " " << -1 << " " << -1 << " " << -1 << endl;
		return;
	}
	int startX, startY, resultX, resultY, obstacleX, obstacleY;
	startX = beginPos[pos][0];
	startY = beginPos[pos][1];
	resultX = possiblePos[pos][0];
	resultY = possiblePos[pos][1];
	obstacleX = obstaclePos[pos][0];
	obstacleY = obstaclePos[pos][1];
	//ProcStep(startX, startY, resultX, resultY, obstacleX, obstacleY, currBotColor, false, false);
	cout << startX << ' ' << startY << ' ' << resultX << ' ' << resultY << ' ' << obstacleX << ' ' << obstacleY << endl;
	return;
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
	ScoreOnly(currBotColor, turnID);
	return 0;
}
