//头文件：score.h
//包含了对当前局势进行打分的所有函数及其辅助函数

using namespace std;

#define GRIDSIZE 8
#define OBSTACLE 2
#define judge_black 0
#define judge_white 1
#define grid_black 1
#define grid_white -1

int currBotColor; // 我所执子颜色（1为黑，-1为白，棋盘状态亦同）
int gridInfo[GRIDSIZE][GRIDSIZE] = { 0 }; // 先x后y，记录棋盘状态
int dx[8] = { -1,-1,-1,0,0,1,1,1 };
int dy[8] = { -1,0,1,-1,1,-1,0,1 };

int KingMove[GRIDSIZE][GRIDSIZE];
int QueenMove[GRIDSIZE][GRIDSIZE];

// 判断是否在地图内
inline bool inMap(int x, int y)
{
	if (x < 0 || x >= GRIDSIZE || y < 0 || y >= GRIDSIZE)
		return false;
	return true;
}

//判断x0y0能否到达x1y1
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

// 在坐标处落子，检查是否合法或模拟落子
bool ProcStep(int x0, int y0, int x1, int y1, int x2, int y2, int color, bool check_only, bool print_warning)
{
	if ((!inMap(x0, y0)) || (!inMap(x1, y1)) || (!inMap(x2, y2)))
	{
		if (print_warning)
			cout << "Location out of the chessboard, please retry:" << endl;
		return false;
	}
	if (gridInfo[x0][y0] != color)
	{
		if (print_warning)
			cout << "You must move your own chess, please retry:" << endl;
		return false;
	}
	if (gridInfo[x1][y1] != 0)
	{
		if (print_warning)
			cout << "Destination occupied, please retry:" << endl;
		return false;
	}
	if ((gridInfo[x2][y2] != 0) && !(x2 == x0 && y2 == y0))
	{
		if (print_warning)
			cout << "Obstacel location occupied, please retry: " << endl;
		return false;
	}
	if (!Reachable(x0, y0, x1, y1)) {
		if (print_warning)
			cout << "Unable to reach destination, please retry: " << endl;
		return false;
	}
	if (!Reachable(x1, y1, x2, y2)) {
		if (print_warning)
			cout << "Unable to put obstacel there, please retry: " << endl;
		return false;
	}
	if (!check_only)
	{
		gridInfo[x0][y0] = 0;
		gridInfo[x1][y1] = color;
		gridInfo[x2][y2] = OBSTACLE;
	}
	return true;
}

//calkinmove的辅助函数，对一个节点进行BFS，标记其他所有节点的kingmove值。
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

//把每格节点的对应于color的Kingmove值计算出来
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

//calqueenmove的辅助函数，对一个节点进行BFS，标记其他所有节点的kingmove值。
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

//把每格节点的对应于color的queenmove值计算出来
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

//计算当前局势下的territory特征值
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

//计算c1特征值
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

//计算c2特征值
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

//计算每个空格的灵活度
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

//计算某一个棋子的灵活度
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

//计算灵活度特征值m；
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

//根据五个特征值打分
double CalScore(double a = 0.2, double b = 0.2, double c = 0.2, double d = 0.2, double e = 0.2) {
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
	double t1, t2, c1, c2, m;
	t1 = territory(QueenMove_w, QueenMove_b);
	t2 = territory(KingMove_w, KingMove_b);
	c1 = position1(QueenMove_w, QueenMove_b);
	c2 = position2(KingMove_w, KingMove_b);
	CalBlankMobility();
	m = CalColorMobility();
	score = a * t1 + b * t2 + c * c1 + d * c2 + e * m;
	/*cout << "t1= " << t1 << endl;
	cout << "t1= " << t2 << endl;
	cout << "c1= " << c1 << endl;
	cout << "c2= " << c2 << endl;
	cout << " m = " << m << endl;*/
	return score;
}