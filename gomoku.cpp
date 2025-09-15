#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include<queue>
#include<vector>
#include<map>
#include<cstring>
#include<algorithm>
//#include"new0405.h"
#define radius 5
#define INF -0x7f7f7f7f
#define MAX  0x7f7f7f7f

#define WIN 99999999
#define six 1
#define five 1
#define livefour 2
#define jumpfour 3
#define jumpfour_m 4
#define jumpfour_l 23
#define deadfour 5
#define livethree 6
#define livethree_l 7
#define jumpthree 8
#define deadthree 9
#define dead_jumpthree 10
#define dead_jumpthree_l 11
#define livetwo 12
#define livetwo_l 13
#define livetwo_lm 14
#define jumptwo 15
#define jumptwo_l 16
#define jjumptwo 17
#define deadtwo 18
#define liveone 19
#define liveone_m 20
#define liveone_mm 21
#define deadone 22
using namespace std;
int type[4][3][3][3][3][4] = { 0 };
int typeNumToScore[30] = { 0 };
//int positiontype[15][15][4] = { 0 };

void initial_type();
void initialTypescore();

static int m_dx[8] = { -1,0,1,-1,1,-1,0,1};
static int m_dy[8] = { -1,-1,-1,0,0,1,1,1};
double threshold = 0.95 * (double)CLOCKS_PER_SEC; //CLOCKS_PER_SEC 和 clock()的含义可百度
double start_time = clock();
const int SIZE = 15;
int board[SIZE][SIZE] = { 0 };//本方1，对方2，空白0
int MaxDepth = 11;//最大递归深度，可商量设定
struct QueueNode {
	int x;
	int y;
	int index;
	friend bool operator<(QueueNode q1, QueueNode q2) {
		return q1.index < q2.index;
	}
	QueueNode(int a, int b, int c) {
		x = a;
		y = b;
		index = c;
	}
};
struct Point {
	int x;
	int y;
	Point(int x, int y) {
		this->x = (x >= 0 && x < 15) ? x : (x < 0) ? 0 : 14;
		this->y = (y >= 0 && y < 15) ? y : (y < 0) ? 0 : 14;
	}
	bool operator==(const Point q) const;
	bool operator<(const Point q) const;
};
//六个里面选分最高的棋形的编号
//数组下标为棋形对应的编号 元素储存的是对应的分数
//编号为n的棋形对应的分数

int getScore(Point now, int direction, int turn);
//得到now点周围的棋形放入result，form=1,2,3,4；1=横，2=纵，3=左斜\,4=右斜/
//return now点在子串result中的位置下标
int evaluate(Point now);
//算now点的分值=sum（|现在得到的棋形-存在[x][y][4]中的过去的棋形|）
//不修改[x][y][4]
int getGrade(Point now);
//算now点的分值=sum（|现在得到的棋形-存在[x][y][4]中的过去的棋形|）
//修改[x][y][4]

void saveSite(Point now, int* site, int* already);//保存现场
void recoverSite(Point now, int* site, int* already);//恢复现场
void initialTypes(std::vector<Point>currstate);

int getScore(Point now, int direction, int turn) {//result存棋形，int是now在棋形中的位置
	int x = now.x, y = now.y;
	int big = 0;
	int big2 = 0;//判断是不是死跳四
	int staging = 0;//暂存type
	if (direction == 1) {
		for (int i = y; i >= 0 && i >= y - 5; i--) {
			if (i + 4 > 14) { continue; }
			if (i + 4 == 14) {//右边撞墙，且差一个刚好6个，把墙当成2
				staging = type[board[x][i]][board[x][i + 1]][board[x][i + 2]][board[x][i + 3]][board[x][i + 4]][3];
				if (staging == 23 || staging == 5) {
					big2 = typeNumToScore[staging];
				}
				if (typeNumToScore[staging] > big) {
					big = typeNumToScore[staging];
				}
				continue;
			}
			if (i == 0) {//左边撞墙了
				staging = type[3][board[x][i]][board[x][i + 1]][board[x][i + 2]][board[x][i + 3]][board[x][i + 4]];
				if (staging == 23 || staging == 5) {
					big2 = typeNumToScore[staging];
				}
				if (typeNumToScore[staging] > big) {
					big = typeNumToScore[staging];
				}
			}
			staging = type[board[x][i]][board[x][i + 1]][board[x][i + 2]][board[x][i + 3]][board[x][i + 4]][board[x][i + 5]];
			if (staging == 23 || staging == 5) {
				big2 = typeNumToScore[staging];
			}
			if (typeNumToScore[staging] > big) {
				big = typeNumToScore[staging];

			}
		}
		if (big == 100000000) { return big; }
		else if (big == 999999) { return big; }
		else if (big2 != 0) { return big2; }
		else return big;
	}
	if (direction == 2) {
		for (int i = x; i >= 0 && i >= x - 5; i--) {
			if (i + 4 > 14)continue;
			if (i + 4 == 14) {//下面撞墙而且差一个
				staging = type[board[i][y]][board[i + 1][y]][board[i + 2][y]][board[i + 3][y]][board[i + 4][y]][3];
				if (staging == 23 || staging == 5) {
					big2 = typeNumToScore[staging];
				}
				if (typeNumToScore[staging] > big) {
					big = typeNumToScore[staging];
				}
				continue;
			}
			if (i == 0) {
				staging = type[3][board[i][y]][board[i + 1][y]][board[i + 2][y]][board[i + 3][y]][board[i + 4][y]];
				if (staging == 23 || staging == 5) {
					big2 = typeNumToScore[staging];
				}
				if (typeNumToScore[staging] > big) {
					big = typeNumToScore[staging];

				}
			}
			staging = type[board[i][y]][board[i + 1][y]][board[i + 2][y]][board[i + 3][y]][board[i + 4][y]][board[i + 5][y]];
			if (staging == 23 || staging == 5) {
				big2 = typeNumToScore[staging];
			}
			if (typeNumToScore[staging] > big) {
				big = typeNumToScore[staging];

			}
		}
		if (big == 100000000) { return big; }
		else if (big == 999999) { return big; }
		else if (big2 != 0) { return big2; }
		else return big;
	}
	if (direction == 3) {
		for (int i = x, j = y; i >= 0 && i >= x - 5 && j >= 0 && j >= y - 5; i--, j--) {
			if (i + 4 > 14 || j + 4 > 14)continue;
			if (i + 4 == 14 || j + 4 == 14) {
				staging = type[board[i][j]][board[i + 1][j + 1]][board[i + 2][j + 2]][board[i + 3][j + 3]][board[i + 4][j + 4]][3];
				if (staging == 23 || staging == 5) {
					big2 = typeNumToScore[staging];
				}
				if (typeNumToScore[staging] > big) {
					big = typeNumToScore[staging];

				}
				continue;
			}
			if (i == 0 || j == 0) {
				staging = type[3][board[i][j]][board[i + 1][j + 1]][board[i + 2][j + 2]][board[i + 3][j + 3]][board[i + 4][j + 4]];
				if (staging == 23 || staging == 5) {
					big2 = typeNumToScore[staging];
				}
				if (typeNumToScore[staging] > big) {
					big = typeNumToScore[staging];

				}
			}
			staging = type[board[i][j]][board[i + 1][j + 1]][board[i + 2][j + 2]][board[i + 3][j + 3]][board[i + 4][j + 4]][board[i + 5][j + 5]];
			if (staging == 23 || staging == 5) {
				big2 = typeNumToScore[staging];
			}
			if (typeNumToScore[staging] > big) {
				big = typeNumToScore[staging];

			}
		}
		if (big == 100000000) { return big; }
		else if (big == 999999) { return big; }
		else if (big2 != 0) { return big2; }
		else return big;

	}
	if (direction == 4) {
		for (int i = x, j = y; i >= 0 && i >= x - 5 && j <= 14 && j <= y + 5; i--, j++) {
			if (i + 4 > 14 || j - 4 < 0)continue;
			if (i + 4 == 14 || j - 4 == 0) {
				staging = type[board[i][j]][board[i + 1][j - 1]][board[i + 2][j - 2]][board[i + 3][j - 3]][board[i + 4][j - 4]][3];
				if (staging == 23 || staging == 5) {
					big2 = typeNumToScore[staging];
				}
				if (typeNumToScore[staging] > big) {
					big = typeNumToScore[staging];

				}
				continue;
			}
			if (i == 0 || j == 14) {
				staging = type[3][board[i][j]][board[i + 1][j - 1]][board[i + 2][j - 2]][board[i + 3][j - 3]][board[i + 4][j - 4]];
				if (staging == 23 || staging == 5) {
					big2 = typeNumToScore[staging];
				}
				if (typeNumToScore[staging] > big) {
					big = typeNumToScore[staging];

				}
			}
			staging = type[board[i][j]][board[i + 1][j - 1]][board[i + 2][j - 2]][board[i + 3][j - 3]][board[i + 4][j - 4]][board[i + 5][j - 5]];
			if (staging == 23 || staging == 5) {
				big2 = typeNumToScore[staging];
			}
			if (typeNumToScore[staging] > big) {
				big = typeNumToScore[staging];
			}
		}
		if (big == 100000000) { return big; }
		else if (big == 999999) { return big; }
		else if (big2 != 0) { return big2; }
		else return big;
	}
	return 0;
}

int evaluate(Point now) {
	int prescore[5] = { 0 };
	int score = 0;
	int turn = board[now.x][now.y];
	board[now.x][now.y] = 0;
	//下进去之前
	for (int i = 1; i <= 4; i++) {
		prescore[i] = getScore(now, i, turn);
	}
	//下进去之后
	board[now.x][now.y] = turn;
	for (int i = 1; i <= 4; i++) {
		score += abs(prescore[i] - getScore(now, i, turn));
	}
	return score;
}


int getGrade(Point now) {
	int prescore[5] = { 0 };
	int score = 0;
	int turn = board[now.x][now.y];
	board[now.x][now.y] = 0;
	//下进去之前
	for (int i = 1; i <= 4; i++) {
		prescore[i] = getScore(now, i, turn);
	}
	//下进去之后
	board[now.x][now.y] = turn;
	for (int i = 1; i <= 4; i++) {
		score += abs(prescore[i] - getScore(now, i, turn));
	}
	return (turn == 1) ? abs(score) : -abs(score);
	//turn=1必带来正影响，turn=2必带来负影响
}
bool Point::operator==(const Point q) const {
	return x == q.x && y == q.y;
}
bool Point::operator<(const Point q) const {
	if (x < q.x)return true;
	else if (x == q.x && y < q.y)return true;
	return false;
}
bool cmp(pair<Point, int> a, pair<Point, int> b) {
	return a.second > b.second;
}

map<int, int>zobristMap;
int HashBoard[SIZE][SIZE][3];
int bestMove_L[2];
int deepSearch(Point pre, int depth, map<Point, int>q, int turn, int beta, int alpha, int preZobrist, int preScore, int* already);
int judgement(Point pre);//判断输赢
int getZobristNum_L(int preZobrist, Point newPoint);//得到新zobrist值

void zobristInitial();//初始化，放在主函数
int getNum_L(Point pre, int a, int b);//判断输赢时，得到一行的棋子数////
bool incircle(int x, int y);

bool incircle(int x, int y) {
	// Calculate the square of the distance from the point to the center of the circle
	int dx = x - 7;
	int dy = y - 7;
	int distanceSquared = dx * dx + dy * dy;
	int radiusSquared = radius * radius;
	// Check if the distanceSquared is less than or equal to radiusSquared
	return distanceSquared <= radiusSquared;
}


int main()
{
	int n = 0, x = 0, y = 0;//正常读入
	int x1 = 0, y1 = 0, x2 = 0, y2 = 0;//换手判断
	int new_x = 0, new_y = 0;//我方下棋
	scanf("%d", &n);
	srand(time(0));
	//初始化记录
	Point leftpoint(14, 14);
	Point rightpoint(0, 0);
	vector<Point> currstate;
	zobristInitial();
	initial_type();
	initialTypescore();
	if (n == 1) {
		cin >> x >> y;
		if (x == -1) {
			new_x = 9, new_y = 2;//我方先手 放在棋盘平衡点
		}
		else {
			//我方后手 
			if (!(x!=0&&y!=0&&x!=14&&y!=14)) {
				new_x = 7, new_y = 7;  //对方下的棋子不在圈内 我方不换手 并且将自己的棋子放在最中心
			}
			else {
				new_x = -1, new_y = -1;// 对方下的棋子在圈内 我方选择换手
			}
		}
		printf("%d %d\n", new_x, new_y);
		return 0;
	}
	else {//判断换手情况--》开始
		scanf("%d %d %d %d %d %d", &x, &y, &x1, &y1, &x2, &y2);
		if (x == -1 && x2 == -1) {
			board[x1][y1] = 2;//如果我方先手 对方选择换手 第一颗棋子算做对方下的
			//建议new_x new_y下到9 2

			currstate.push_back(Point(x1, y1));
		}
		else if (x == -1 && x2 != -1) {
			board[x1][y1] = 1;
			board[x2][y2] = 2;//如果我方先手 对方没有选择换手 第一颗棋子是我方下的 第二颗棋子是对方下的

			currstate.push_back(Point(x1, y1));
			currstate.push_back(Point(x2, y2));
		}
		else if (x != -1 && x1 == -1) {
			board[x][y] = 1;
			board[x2][y2] = 2;//如果对方先手 我方选择换手 第一颗棋子是我方下的 第二颗棋子是对方下的

			currstate.push_back(Point(x, y));
			currstate.push_back(Point(x2, y2));
		}
		else if (x != -1 && x1 != -1) {
			board[x][y] = 2;//如果对方先手 我方没有选择换手 第一颗棋子是对方下的 以此类推
			board[x1][y1] = 1;
			board[x2][y2] = 2;

			currstate.push_back(Point(x, y));
			currstate.push_back(Point(x1, y1));
			currstate.push_back(Point(x2, y2));

		}

		for (int i = 0; i < n - 2; i++) {
			scanf("%d %d", &x, &y);
			board[x][y] = 1;
			currstate.push_back(Point(x, y));

			scanf("%d %d", &x, &y);
			board[x][y] = 2;
			currstate.push_back(Point(x, y));

		}


		int already[15][15] = { 0 };
		map<Point, int>q;
		int next_x = 0, next_y = 0;
		int score1, score2;
		for (vector<Point>::iterator it = currstate.begin(); it != currstate.end(); it++) {
			for (int i = 0; i < 8; i++) {
				next_x = it->x + m_dx[i];
				next_y = it->y + m_dy[i];
				if (next_x < 15 && next_x >= 0 && next_y < 15 && next_y >= 0 && board[next_x][next_y] == 0 && already[next_x][next_y] == 0) {
					already[next_x][next_y] = 1;
					Point nowPoint = Point(next_x, next_y);
					board[next_x][next_y] = 1;

					score1 = 3 + evaluate(nowPoint);
					board[next_x][next_y] = 2;
					score2 = 2 + evaluate(nowPoint);
					q.insert(pair<Point, int>(Point(next_x, next_y), max(score1, score2)));

					board[next_x][next_y] = 0;
				}
			}
		}
		vector< pair<Point, int> > vec(q.begin(), q.end());
		sort(vec.begin(), vec.end(), cmp);
		Point result = Point(0, 0);
		int alpha = INF;
		int temp = 0;
		new_x = vec[0].first.x;
		new_y = vec[0].first.y;
		result.x = new_x;
		result.y = new_y;
		for (vector<pair<Point, int>>::iterator it = vec.begin(); it != vec.end() && it != vec.begin() + 4; it++) {
			Point curr = it->first;
			board[curr.x][curr.y] = 1;
			int preScore = q[curr];
			q[curr] = INF;


			temp = -deepSearch(curr, MaxDepth, q, 1, -alpha, INF, 0, 0, &(already[0][0]));
			if (temp > alpha) {
				result.x = curr.x;
				result.y = curr.y;
				alpha = temp;
			}
			q[curr] = preScore;//恢复现场
			board[curr.x][curr.y] = 0;


		}

		new_x = result.x;
		new_y = result.y;
		//cout << new_x << ' ' << new_y;
	 //其余代码*/
	}
	printf("%d %d\n", new_x, new_y);
	return 0;
}
void saveSite(Point now, int* already, int* site) {
	//保存现场
	 //保存nowNode横竖斜四条线，每条线的九个already
	int x = now.x;
	int y = now.y;
	//每行一共9个,没满九个也把位置空出来
	//横向0-8,  
	for (int i = x - 4, k = 0; k <= 8; i++, k++) {
		if (i < 0) { continue; }
		if (i > 14) { break; }
		site[k] = already[15 * i + y];

	}
	//纵向9-17
	for (int j = y - 4, k = 9; k <= 17; j++, k++) {
		if (j < 0) { continue; }
		if (j > 14) { break; }
		site[k] = already[15 * x + j];

	}
	//左斜\18-26
	for (int i = x - 4, j = y - 4, k = 18; k <= 26; i++, j++, k++) {
		if (i < 0 || j < 0) { continue; }
		if (j > 14 || i > 14) { break; }
		site[k] = already[15 * i + j];
	}
	//右斜/27-35
	for (int i = x - 4, j = y + 4, k = 27; k <= 35; i++, j--, k++) {//左下到右上

		if (i < 0 || j > 14) { continue; }
		if (j < 0 || i > 14) { break; }
		site[k] = already[15 * i + j];
	}
}
void recoverSite(Point now, int* already, int* site) {
	//恢复现场
	 //恢复nowNode横竖斜四条线，每条线的九个already
	int x = now.x;
	int y = now.y;
	//每行一共9个,没满九个也把位置空出来
	//横向0-8,  
	for (int i = x - 4, k = 0; k <= 8; i++, k++) {
		if (i < 0) { continue; }
		if (i > 14) { break; }
		already[15 * i + y] = site[k];

	}
	//纵向9-17
	for (int j = y - 4, k = 9; k <= 17; j++, k++) {
		if (j < 0) { continue; }
		if (j > 14) { break; }
		already[15 * x + j] = site[k];

	}
	//左斜\18-26
	for (int i = x - 4, j = y - 4, k = 18; k <= 26; i++, j++, k++) {
		if (i < 0 || j < 0) { continue; }
		if (j > 14 || i > 14) { break; }
		already[15 * i + j] = site[k];
	}
	//右斜/27-35
	for (int i = x - 4, j = y + 4, k = 27; k <= 35; i++, j--, k++) {//左下到右上

		if (i < 0 || j > 14) { continue; }
		if (j < 0 || i > 14) { break; }
		already[15 * i + j] = site[k];
	}


}

int deepSearch(Point pre, int depth, map<Point, int>q, int turn, int beta, int alpha, int preZobrist, int preScore, int* already) {
	//pre已经下在board中，但pre周围的点不在队列，turn是pre的轮次
	//preZobrist不包括pre
	//preScore不包括pre
	int zobristNum = getZobristNum_L(preZobrist, pre);//这局的zobristNum
	if (zobristMap.size() != 0) {
		map<int, int>::iterator searchResult = zobristMap.find(zobristNum);
		if (searchResult != zobristMap.end()) {//search successfully
			return searchResult->second;
		}

	}

	/*int judge = judgement_L(pre);
	//判断输赢
	if (judge == -WIN || judge == WIN) {
		if (judge == WIN && depth % 2 == 1 || (judge == -WIN && depth % 2 == 0)) {
			zobristMap.insert(pair<int, int>(zobristNum, judge));
			return judge;
		}
		else {
			zobristMap.insert(pair<int, int>(zobristNum, -judge));
			return -judge;

		}
	}*/
	int tempScore = getGrade(pre);

	if (abs(tempScore) >= 99000001) {
		return  -WIN;
	}
	//temp=abs(tempScore),temp>=99000001||(temp>=25000-3450-1075&&对方分最高的点<99000001)||(temp>=5000*2-1075*2&&对方分最高的点<20000-3450)
	//如果turn=1，depth%2==1则我方win
	// //如果turn=2,depth%2==0,则对方win
	int nowScore = preScore + tempScore;//修改pre附近[x][y][4]
	//if(tempScore>)
	//判断深度
	if (depth == 0) {
		zobristMap.insert(pair<int, int>(zobristNum, nowScore));
		return nowScore;
	}
	//超时返回
	/**/double current_time = clock();
	if (current_time - start_time > threshold) { //到 0.95 秒立即跳出循环
		return MAX;
	}

	//把pre周围的点都推进q,并排序
	int x = pre.x;
	int y = pre.y;

	int score1, score2;

	for (int i = 4; i > 0; i--) {
		for (int j = i; j >= -i; j -= i) {//j=i,0,-i;i>0
			//[x+i][y+j]
			//[x-i][y+j]
			if (y + j < 0 || y + j > 14)continue;//纵轴过界
			//[x+i][y+j]

			if ((x + i >= 0 && x + i <= 14) && (((already[(x + i) * 15 + y + j] == 1) || (i == 1)) && board[x + i][y + j] == 0)) {
				//在map里or在周围一圈里 ,且没被下

				Point nowPoint = Point(x + i, y + j);
				board[x + i][y + j] = 1;
				score1 = 3 + evaluate(nowPoint);
				board[x + i][y + j] = 2;
				score2 = 2 + evaluate(nowPoint);//两次取值
				q[nowPoint] = max(score1, score2);//插入
				board[x + i][y + j] = 0;//恢复
				already[(x + i) * 15 + y + j] = 1;//标识已经入队
			}
			////[x-i][y+j]

			if ((x - i >= 0 && x - i <= 14) && (((already[(x - i) * 15 + y + j] == 1) || (i == 1)) && board[x - i][y + j] == 0)) {
				//在map里or在周围一圈里 ,且没被下

				Point nowPoint = Point(x - i, y + j);
				board[x - i][y + j] = 1;
				score1 = 3 + evaluate(nowPoint);
				board[x - i][y + j] = 2;
				score2 = 2 + evaluate(nowPoint);//两次取值
				q[nowPoint] = max(score1, score2);//插入
				board[x - i][y + j] = 0;//恢复
				already[(x - i) * 15 + y + j] = 1;//标识已经入队
			}


		}
	}
	for (int j = y - 4; j <= y + 4; j++) {
		//以[x][y]为中心的纵轴
		//[x][y-4]->[x][y+4]
		if (j > 14)break;
		if (j >= 0 && (already[x * 15 + j] == 1 || j == y - 1 || j == y + 1) && board[x][j] == 0) {
			//在map里or在周围一圈里 ,且没被下
			Point nowPoint = Point(x, j);
			board[x][j] = 1;
			score1 = 3 + evaluate(nowPoint);
			board[x][j] = 2;
			score2 = 2 + evaluate(nowPoint);//两次取值
			q[nowPoint] = max(score1, score2);//插入
			board[x][j] = 0;//恢复
			already[x * 15 + j] = 1;//标识已经入队
		}
	}
	vector< pair<Point, int> > vec(q.begin(), q.end());
	sort(vec.begin(), vec.end(), cmp);
	int score;

	for (vector<pair<Point, int>>::iterator it = vec.begin(); it != vec.end() && it != vec.begin() + 4; it++) {
		Point nowNode = it->first;
		int x = nowNode.x;
		int y = nowNode.y;

		int preScore;

		if (board[x][y] == 0) {
			int site[38];//现场

			//下棋[x][y]
			board[x][y] = 3 - turn;		//下nowNode，周围点不用推入队列           
			preScore = q[nowNode];
			q[nowNode] = INF;
			saveSite(nowNode, already, site);//保存nowNode横竖斜四条线，每条线的九个already
			//保存现场

			score = -deepSearch(nowNode, depth - 1, q, 3 - turn, -alpha, -beta, zobristNum, nowScore, already);

			//收回下棋[x][y]
			board[x][y] = 0;
			q[nowNode] = preScore;
			recoverSite(nowNode, already, site);//  恢复nowNode周围点的already
			//恢复现场

			if (score > alpha) {
				alpha = score;

			}
			if (alpha >= beta) {

				break;//剪枝
			}



		}


	}
	

	zobristMap.insert(pair<int, int>(zobristNum, alpha));
	return  alpha;

}

int getZobristNum_L(int preZobrist, Point newPoint) {
	int x = newPoint.x;
	int y = newPoint.y;
	int turn = board[x][y];
	//	int hashPreNum = HashBoard[x][y][0];
	int hashNowNum = (turn == 1) ? HashBoard[x][y][1] : HashBoard[x][y][2];
	int nowZobrist = preZobrist ^ hashNowNum;
	return nowZobrist;
}

void zobristInitial() {
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			for (int k = 1; k <= 2; k++)
				HashBoard[i][j][k] = rand();
		}
	}
}

void initial_type() {
	//-----我方被墙挡住的棋型
	//死五
	type[1][1][1][1][1][3] = five;
	type[3][1][1][1][1][1] = five;
	//死跳四
	 //%
	type[1][1][1][0][1][3] = jumpfour_l;
	type[3][1][0][1][1][1] = jumpfour_l;
	//%
	type[3][1][1][1][0][1] = jumpfour_l;
	type[1][0][1][1][1][3] = jumpfour_l;
	type[3][1][1][0][1][1] = jumpfour_l;
	type[1][1][0][1][1][3] = jumpfour_l;
	//死四
	type[3][1][1][1][1][0] = deadfour;
	type[0][1][1][1][1][3] = deadfour;
	//活三
	type[3][0][1][1][1][0] = livethree_l;
	type[0][1][1][1][0][3] = livethree_l;
	//死三
	type[3][1][1][1][0][0] = deadthree;
	type[0][0][1][1][1][3] = deadthree;

	//死跳三
	type[0][1][1][0][1][3] = dead_jumpthree;
	type[3][1][0][1][1][0] = dead_jumpthree;
	type[3][1][1][0][1][0] = dead_jumpthree_l;
	type[0][1][0][1][1][3] = dead_jumpthree_l;
	//活二
	type[3][0][0][1][1][0] = livetwo_l;
	type[0][1][1][0][0][3] = livetwo_l;
	type[3][0][1][1][0][0] = livetwo_lm;
	type[0][0][1][1][0][3] = livetwo_lm;
	//跳二
	type[0][1][0][1][0][3] = jumptwo_l;
	type[3][0][1][0][1][0] = jumptwo_l;
	//死二
	type[3][1][1][0][0][0] = deadtwo;
	type[0][0][0][1][1][3] = deadtwo;

	//活一
	type[3][0][0][1][0][0] = liveone_m;
	type[0][0][1][0][0][3] = liveone_m;

	type[3][2][0][1][0][0] = liveone_mm;
	type[0][0][1][0][2][3] = liveone_mm;

	type[0][0][0][1][0][3] = liveone_mm;
	type[3][0][1][0][0][0] = liveone_mm;

	//死一
	type[3][0][2][1][0][0] = deadone;
	type[3][2][2][1][0][0] = deadone;
	type[0][0][1][2][0][3] = deadone;
	type[0][0][1][2][2][3] = deadone;

	//-----对方被墙挡住的棋型
	type[2][2][2][2][2][3] = five;
	type[3][2][2][2][2][2] = five;
	//%
	type[2][2][2][0][2][3] = jumpfour_l;
	type[3][2][0][2][2][2] = jumpfour_l;
	//%
	type[3][2][2][2][0][2] = jumpfour_l;
	type[2][0][2][2][2][3] = jumpfour_l;
	type[3][2][2][0][2][2] = jumpfour_l;
	type[2][2][0][2][2][3] = jumpfour_l;

	type[3][2][2][2][2][0] = deadfour;
	type[0][2][2][2][2][3] = deadfour;

	type[3][0][2][2][2][0] = livethree_l;
	type[0][2][2][2][0][3] = livethree_l;

	type[3][2][2][2][0][0] = deadthree;
	type[0][0][2][2][2][3] = deadthree;

	type[0][2][2][0][2][3] = dead_jumpthree;
	type[3][2][0][2][2][0] = dead_jumpthree;

	type[3][2][2][0][2][0] = dead_jumpthree_l;
	type[0][2][0][2][2][3] = dead_jumpthree_l;

	type[3][0][0][2][2][0] = livetwo_l;
	type[0][2][2][0][0][3] = livetwo_l;

	type[3][0][2][2][0][0] = livetwo_lm;
	type[0][0][2][2][0][3] = livetwo_lm;

	type[0][2][0][2][0][3] = jumptwo_l;
	type[3][0][2][0][2][0] = jumptwo_l;


	type[3][2][2][0][0][0] = deadtwo;
	type[0][0][0][2][2][3] = deadtwo;

	type[3][0][0][2][0][0] = liveone_m;
	type[0][0][2][0][0][3] = liveone_m;

	type[3][1][0][2][0][0] = liveone_mm;
	type[0][0][2][0][1][3] = liveone_mm;

	type[0][0][0][2][0][3] = liveone_mm;
	type[3][0][2][0][0][0] = liveone_mm;


	type[3][0][1][2][0][0] = deadone;
	type[3][1][1][2][0][0] = deadone;
	type[0][0][2][1][0][3] = deadone;
	type[0][0][2][1][1][3] = deadone;

	//--我方棋型
	type[1][1][1][1][1][1] = six;

	type[1][1][1][1][1][2] = five;
	type[2][1][1][1][1][1] = five;
	type[0][1][1][1][1][1] = five;
	type[1][1][1][1][1][0] = five;

	//四连
	type[0][1][1][1][1][0] = livefour;

	type[1][0][1][1][1][0] = jumpfour;
	type[0][1][1][1][0][1] = jumpfour;
	//被挡住是活三
	//看不见边界的分数一律被当做挡住
	type[1][1][0][1][1][0] = jumpfour_m;
	type[0][1][1][0][1][1] = jumpfour_m;

	type[1][1][1][0][1][0] = jumpfour_l;
	type[0][1][0][1][1][1] = jumpfour_l;//左右两边都是被挡住的 被挡住的里面是什么都无所谓
	type[1][1][1][0][1][2] = jumpfour_l;
	type[2][1][0][1][1][1] = jumpfour_l;
	//被挡住是左右两边都被挡住的3
	type[1][1][1][1][0][1] = jumpfour_l;
	type[1][0][1][1][1][1] = jumpfour_l;
	//被挡住是左右两边都被挡住的4


	type[2][1][1][1][0][1] = jumpfour_l;
	type[1][0][1][1][1][2] = jumpfour_l;
	type[2][1][1][0][1][1] = jumpfour_l;
	type[1][1][0][1][1][2] = jumpfour_l;

	type[2][1][1][1][1][0] = deadfour;
	type[0][1][1][1][1][2] = deadfour;

	//三连
	type[0][0][1][1][1][0] = livethree;
	type[0][1][1][1][0][0] = livethree;

	type[2][0][1][1][1][0] = livethree_l;
	type[0][1][1][1][0][2] = livethree_l;

	type[0][1][1][0][1][0] = jumpthree;
	type[0][1][0][1][1][0] = jumpthree;//活的jumpthree

	type[2][1][1][1][0][0] = deadthree;
	type[0][0][1][1][1][2] = deadthree;
	type[0][1][1][0][1][2] = dead_jumpthree;
	type[2][1][0][1][1][0] = dead_jumpthree;

	type[2][1][1][0][1][0] = dead_jumpthree_l;
	type[0][1][0][1][1][2] = dead_jumpthree_l;


	type[0][0][0][1][1][0] = livetwo;
	type[0][1][1][0][0][0] = livetwo;
	type[0][0][1][1][0][0] = livetwo;
	type[2][0][0][1][1][0] = livetwo_l;


	type[0][1][1][0][0][2] = livetwo_l;


	type[2][0][1][1][0][0] = livetwo_lm;
	type[0][0][1][1][0][2] = livetwo_lm;

	type[0][1][0][1][0][0] = jumptwo;
	type[0][0][1][0][1][0] = jumptwo;
	type[0][1][0][1][0][2] = jumptwo_l;
	type[2][0][1][0][1][0] = jumptwo_l;

	type[0][1][0][0][1][0] = jjumptwo;


	type[2][1][1][0][0][0] = deadtwo;
	type[0][0][0][1][1][2] = deadtwo;


	type[0][0][0][1][0][0] = liveone;
	type[0][0][1][0][0][0] = liveone;



	//基于第一种棋型
	type[2][0][0][1][0][0] = liveone_m;
	type[0][2][0][1][0][0] = liveone_mm;
	type[2][2][0][1][0][0] = liveone_mm;
	type[0][0][0][1][0][2] = liveone_mm;
	//基于第二种棋型
	type[0][0][1][0][0][2] = liveone_m;
	type[0][0][1][0][2][0] = liveone_mm;
	type[0][0][1][0][2][2] = liveone_mm;
	type[2][0][1][0][0][0] = liveone_mm;

	type[0][0][2][1][0][0] = deadone;
	type[0][0][1][2][0][0] = deadone;
	//基于第一种
	type[2][0][2][1][0][0] = deadone;
	type[0][2][2][1][0][0] = deadone;
	type[2][2][2][1][0][0] = deadone;
	//基于第二种
	type[0][0][1][2][0][2] = deadone;
	type[0][0][1][2][2][0] = deadone;
	type[0][0][1][2][2][2] = deadone;

	//--对方的棋型
	type[2][2][2][2][2][2] = six;
	type[2][2][2][2][2][1] = five;
	type[1][2][2][2][2][2] = five;
	type[0][2][2][2][2][2] = five;
	type[2][2][2][2][2][0] = five;

	//四连
	type[0][2][2][2][2][0] = livefour;

	type[2][0][2][2][2][0] = jumpfour;
	type[0][2][2][2][0][2] = jumpfour;
	type[2][2][0][2][2][0] = jumpfour_m;
	type[0][2][2][0][2][2] = jumpfour_m;

	type[2][2][2][0][2][0] = jumpfour_l;
	type[0][2][0][2][2][2] = jumpfour_l;//左右两边都是被挡住的 被挡住的里面是什么都无所谓
	type[2][2][2][0][2][1] = jumpfour_l;
	type[1][2][0][2][2][2] = jumpfour_l;

	type[1][2][2][2][0][2] = jumpfour_l;
	type[2][0][2][2][2][1] = jumpfour_l;
	type[1][2][2][0][2][2] = jumpfour_l;
	type[2][2][0][2][2][1] = jumpfour_l;

	type[1][2][2][2][2][0] = deadfour;
	type[0][2][2][2][2][1] = deadfour;

	//三连
	type[0][0][2][2][2][0] = livethree;
	type[0][2][2][2][0][0] = livethree;

	type[1][0][2][2][2][0] = livethree_l;
	type[0][2][2][2][0][1] = livethree_l;

	type[0][2][2][0][2][0] = jumpthree;
	type[0][2][0][2][2][0] = jumpthree;//活的jumpthree

	type[1][2][2][2][0][0] = deadthree;
	type[0][0][2][2][2][1] = deadthree;
	type[0][2][2][0][2][1] = dead_jumpthree;
	type[1][2][0][2][2][0] = dead_jumpthree;

	type[1][2][2][0][2][0] = dead_jumpthree_l;
	type[0][2][0][2][2][1] = dead_jumpthree_l;


	type[0][0][0][2][2][0] = livetwo;
	type[0][2][2][0][0][0] = livetwo;
	type[0][0][2][2][0][0] = livetwo;

	type[1][0][0][2][2][0] = livetwo_l;
	type[0][2][2][0][0][1] = livetwo_l;


	type[1][0][2][2][0][0] = livetwo_lm;
	type[0][0][2][2][0][1] = livetwo_lm;

	type[0][2][0][2][0][0] = jumptwo;
	type[0][0][2][0][2][0] = jumptwo;
	type[0][2][0][2][0][1] = jumptwo_l;
	type[1][0][2][0][2][0] = jumptwo_l;

	type[0][2][0][0][2][0] = jjumptwo;


	type[1][2][2][0][0][0] = deadtwo;
	type[0][0][0][2][2][1] = deadtwo;


	type[0][0][0][2][0][0] = liveone;
	type[0][0][2][0][0][0] = liveone;

	//基于第一种棋型
	type[1][0][0][2][0][0] = liveone_m;
	type[0][1][0][2][0][0] = liveone_mm;
	type[1][1][0][2][0][0] = liveone_mm;
	type[0][0][0][2][0][1] = liveone_mm;
	//基于第二种棋型
	type[0][0][2][0][0][1] = liveone_m;
	type[0][0][2][0][1][0] = liveone_mm;
	type[0][0][2][0][1][1] = liveone_mm;
	type[1][0][2][0][0][0] = liveone_mm;

	type[0][0][1][2][0][0] = deadone;
	type[0][0][2][1][0][0] = deadone;
	//基于第一种
	type[1][0][1][2][0][0] = deadone;
	type[0][1][1][2][0][0] = deadone;
	type[1][1][1][2][0][0] = deadone;
	//基于第二种
	type[0][0][2][1][0][1] = deadone;
	type[0][0][2][1][1][0] = deadone;
	type[0][0][2][1][1][1] = deadone;


}
void initialTypescore() {
	typeNumToScore[five] = 100000000;
	typeNumToScore[livefour] = 999999;
	typeNumToScore[jumpfour] = 30000;
	typeNumToScore[jumpfour_m] = 25500;
	typeNumToScore[jumpfour_l] = 24500;
	typeNumToScore[deadfour] = 20000;
	typeNumToScore[livethree] = 10500;
	typeNumToScore[livethree_l] = 7500;
	typeNumToScore[jumpthree] = 5000;
	typeNumToScore[deadthree] = 3450;
	typeNumToScore[dead_jumpthree] = 1425;
	typeNumToScore[livetwo] = 1075;
	typeNumToScore[livetwo_l] = 1025;
	typeNumToScore[livetwo_lm] = 1000;
	typeNumToScore[jumptwo] = 750;
	typeNumToScore[jumptwo_l] = 500;
	typeNumToScore[jjumptwo] = 300;
	typeNumToScore[deadtwo] = 150;
	typeNumToScore[liveone] = 50;
	typeNumToScore[liveone_m] = 35;
	typeNumToScore[liveone_mm] = 25;
	typeNumToScore[deadone] = 5;


}