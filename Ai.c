/*******
	Reversi Ai  !!!!!
	Date:       2018/1/7
	Author:     Kaltome
	Version:    #85
	depthmax:   3
********/

#define _CRT_SECURE_NO_WARNINGS
#define NOCHESS 3
#define INSIDE_SCORE 300							//内部子分数
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<limits.h>
#include<math.h>

int test = 0;

int depthmax;										//搜索深度，最高可5层
int chessnum = 4;
char chessboard[16][16];
char testboard[6][16][16];

char iside;											//双方棋子颜色
char oside;

const int direction[8][2] = {						//棋子移动的方向
	{ 1,0 },{ 1,-1 },{ 0,-1 },{ -1,-1 },{ -1,0 },{ -1,1 },{ 0,1 },{ 1,1 },
};


//得分表：记录可以下的位置，并记录该位置的得分
struct mine {
	char position[2];								//储存你可以下棋的位置,[0]为行，[1]为列
	int value;										//储存该位置的得分
}canmove[6][256];

//	估值表
//	（为什么两个表搞反了棋力会更高？我也不知道啊………）
int table_start[16][16] = {
	{ 1000000,-100000,150   ,100   ,100   ,100   ,100   ,100   ,100   ,100   ,100   ,100   ,100   ,150   ,-100000,1000000 },
	{ -100000,-200000,60    ,50    ,50    ,50    ,50    ,50    ,50    ,50    ,50    ,50    ,50    ,60    ,-200000,-100000 },
	{ 150    ,60     ,100   ,100   ,100   ,100   ,100   ,100   ,100   ,100   ,100   ,100   ,100   ,100   ,60     ,150     },
	{ 100    ,50     ,100   ,500   ,500   ,500   ,500   ,500   ,500   ,500   ,500   ,500   ,500   ,100   ,50     ,100     },
	{ 100    ,50     ,100   ,500   ,750   ,750   ,750   ,750   ,750   ,750   ,750   ,750   ,500   ,100   ,50     ,100     },
	{ 100    ,50     ,100   ,500   ,750   ,900   ,900   ,900   ,900   ,900   ,900   ,750   ,500   ,100   ,50     ,100     },
	{ 100    ,50     ,100   ,500   ,750   ,900   ,975   ,975   ,975   ,975   ,900   ,750   ,500   ,100   ,50     ,100     },
	{ 100    ,50     ,100   ,500   ,750   ,900   ,975   ,1000  ,1000  ,975   ,900   ,750   ,500   ,100   ,50     ,100     },
	{ 100    ,50     ,100   ,500   ,750   ,900   ,975   ,1000  ,1000  ,975   ,900   ,750   ,500   ,100   ,50     ,100     },
	{ 100    ,50     ,100   ,500   ,750   ,900   ,975   ,975   ,975   ,975   ,900   ,750   ,500   ,100   ,50     ,100     },
	{ 100    ,50     ,100   ,500   ,750   ,900   ,900   ,900   ,900   ,900   ,900   ,750   ,500   ,100   ,50     ,100     },
	{ 100    ,50     ,100   ,500   ,750   ,750   ,750   ,750   ,750   ,750   ,750   ,750   ,500   ,100   ,50     ,100     },
	{ 100    ,50     ,100   ,500   ,500   ,500   ,500   ,500   ,500   ,500   ,500   ,500   ,500   ,100   ,50     ,100     },
	{ 150    ,60     ,100   ,100   ,100   ,100   ,100   ,100   ,100   ,100   ,100   ,100   ,100   ,100   ,60     ,150     },
	{ -100000,-200000,60    ,50    ,50    ,50    ,50    ,50    ,50    ,50    ,50    ,50    ,50    ,60    ,-200000,-100000 },
	{ 1000000,-100000,150   ,100   ,100   ,100   ,100   ,100   ,100   ,100   ,100   ,100   ,100   ,150   ,-100000,1000000 },
};
int table_end[16][16] = {
	{ 1000000,-100000,250   ,200   ,200   ,200   ,200   ,200   ,200   ,200   ,200   ,200   ,200   ,250   ,-100000,1000000 },
	{ -100000,-200000,50    ,40    ,40    ,40    ,40    ,40    ,40    ,40    ,40    ,40    ,40    ,50    ,-200000,-100000 },
	{ 250    ,50     ,10    ,10    ,10    ,10    ,10    ,10    ,10    ,10    ,10    ,10    ,10    ,10    ,50     ,250     },
	{ 20     ,40     ,10    ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,10    ,40     ,200     },
	{ 200    ,40     ,10    ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,10    ,40     ,200     },
	{ 200    ,40     ,10    ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,10    ,40     ,200     },
	{ 200    ,40     ,10    ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,10    ,40     ,200     },
	{ 200    ,40     ,10    ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,10    ,40     ,200     },
	{ 200    ,40     ,10    ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,10    ,40     ,200     },
	{ 200    ,40     ,10    ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,10    ,40     ,200     },
	{ 200    ,40     ,10    ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,10    ,40     ,200     },
	{ 200    ,40     ,10    ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,10    ,40     ,200     },
	{ 200    ,40     ,10    ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,10    ,40     ,200     },
	{ 250    ,50     ,10    ,10    ,10    ,10    ,10    ,10    ,10    ,10    ,10    ,10    ,10    ,10    ,50     ,250     },
	{ -100000,-200000,50    ,40    ,40    ,40    ,40    ,40    ,40    ,40    ,40    ,40    ,40    ,50    ,-200000,-100000 },
	{ 1000000,-100000,250   ,200   ,200   ,200   ,200   ,200   ,200   ,200   ,200   ,200   ,200   ,250   ,-100000,1000000 },
};
int(*table)[16];
char choosstable = 0;																	//转换估值表的标志
int check_choosstable();																//转换估值表的判定（玄学系列）
char board_stabilizer[2][16][16] = { 0 };												//记录稳定子的位置
int depthvalue[6];																		//储存Alpha - Beta 值，最高5层

void reset_canmove(int depth);															//清空一层得分表的位置
void printcboard(char bbb[16][16]);														//打印棋盘（Debug用）
void flipchess(char board[][16], int mrow, int mcol, char side);						//下棋
void Brain();																			//弱智的大脑！
int find(int depth_find, char board[16][16], char side);								//寻找可以下子的位置（双方的棋子，不同深度，作用的棋盘）
int search(int depth);																	//深度搜索主函数

/*   估值函数系列   */
int findvalue(int canmove_i, int depth);												//收集所有的估值数据
int caltable(char board[][16]);															//估值表估值
int findstabilizer(int i, int j, int boardnum);											//寻找稳定子
int stabilizer();																		//调用稳定子函数，并估值
int fine_canact(char side, int acti, int actj);											//寻找行动力
int calaction();																		//行动力估值
int potential_action();																	//寻找潜在行动力，并估值
int diffnum();																			//记录双方棋子间数目差异，并估值
int caltable(char board[][16]);															//估值表估值


int main()
{
	char order[6];
	int getorow, getocol;
	printf("********************\n\n    JP Reversi~    \n\n********************\n\n\n");
	printf("请选择你的棋子颜色！！！\n\n1. 黑 ··· X\n2. 白 ··· O\n\n");

	while (1){
		char getcolor;
		scanf("%c", &getcolor);
		if (getcolor == '1') {
			oside = 'X';
			iside = 'O';
			break;
		}
		else if (getcolor == '2') {
			oside = 'O';
			iside = 'X';
			break;
		}
		else printf("请重新选择！\n");
		while (getchar() != '\n');
	}

	printf("\n\n请选择难度！！！\n\n1. 炒鸡简单！\n2. 来玩玩吧！\n3. 请你吃饭！\n\n");
	while (1) {
		scanf("%d", &depthmax);
		if (0 < depthmax && depthmax < 4) {
			printf("\n\n");
			break;
		}
		printf("请重新选择！！！\n");
		while (getchar() != '\n');
	}

	memset(chessboard, NOCHESS, sizeof(chessboard));									//初始化棋盘
	chessboard[7][8] = chessboard[8][7] = 'X';
	chessboard[7][7] = chessboard[8][8] = 'O';

	printcboard(chessboard);

	//收集指令
	while (1)
	{
		while (scanf("%s", order))
		{
			if (order[0] == 'P') {
				scanf("%d%d", &getorow, &getocol);
				flipchess(chessboard, getorow, getocol, oside);
				chessnum++;
				//printcboard(chessboard);
			}
			else if (order[0] == 'T') break;
			else if (order[0] == 'E') {
				getchar();
				exit(0);
			}
			else printf("DEBUG The order is wrong, please entry again.\n");
			fflush(stdout);
		}
		Brain();
		//printcboard(chessboard);
	}
	return 0;
}


void Brain()
{
	int find_canmove = 0;
	int find_value;
																		//此处定义搜索深度！
	if ((256 - chessnum) < 6) depthmax = 256 - chessnum;
	
	for (int i = 0; i < 6; i++)															//初始化Alpha-Beta值
	{
		if ((i % 2) == 1) depthvalue[i] = INT_MIN;
		else depthvalue[i] = INT_MAX;
	}
	for (int i = 0; i < 16; i++)
	{
		strncpy(testboard[0][i], chessboard[i], 16);									//把棋盘复制到测试棋盘
	}
	if (choosstable == 0) {																//终局转换估值表table_start为table_end（其实搞反了，这反而棋力提升了……）
		if (check_choosstable()) {
			choosstable = 1;
			table = table_start;
		}
		else table = table_end;
	}
	if (chessboard[0][0] != NOCHESS) {													//角落被占子时估值表改良（都被占了……靠近的位置还怎么是危险位置呢……）
		table[0][1] = 2500; table[1][0] = 2500; table[1][1] = 2000;
	}
	if (chessboard[15][0] != NOCHESS) {
		table[15][1] = 2500; table[14][0] = 2500; table[14][1] = 2000;
	}
	if (chessboard[0][15] != NOCHESS) {
		table[0][14] = 2500; table[1][15] = 2500; table[1][14] = 2000;
	}
	if (chessboard[15][15] != NOCHESS) {
		table[14][15] = 2500; table[15][14] = 2500; table[14][14] = 2000;
	}
	
	//深度搜索！
	find_value = search(0);

	for (int i = 0; i < 256 && (canmove[1][i].position[0] != NOCHESS); i++)				//寻找得分最高的位置
	{
		if (canmove[1][i].value == find_value) {
			find_canmove = i;
		}
	}
	printf("%d %d\n", canmove[1][find_canmove].position[0], canmove[1][find_canmove].position[1]);
	fflush(stdout);
	flipchess(chessboard, canmove[1][find_canmove].position[0], canmove[1][find_canmove].position[1], iside);
	chessnum++;
}

//搜索博弈树
int search(int depth)
{
	depth++;																			//当前深度
	char color;
	if (depth % 2 == 0) color = oside;													//由当前深度决定下棋的颜色
	else color = iside;
	reset_canmove(depth);																//清空当前层保存的位置和估值
	if (find(depth, testboard[depth - 1], color) == 0)									//寻找上一层的棋盘可以下的位置，找不到可下的位置时返回极高或极低的估值
	{
		if (depth % 2 == 1) return -300000;
		else return 400000;
	}

	int canmove_i = 0;																	//对每一个下棋的步法进行再搜索或估值
	for (; canmove[depth][canmove_i].position[0] != NOCHESS; canmove_i++)
	{
		for (int i = 0; i < 16; i++)													//将上一层的棋盘复制到这一层才再测试下棋
		{
			strncpy(testboard[depth][i], testboard[depth - 1][i], 16);
		}
		flipchess(testboard[depth], canmove[depth][canmove_i].position[0], canmove[depth][canmove_i].position[1], color);

		if (depth != depthmax) {
			if (depth % 2 == 0) depthvalue[depth+1] = INT_MIN;							//进入下一层搜索前，先初始化下一层的Alpha - Beta值
			else depthvalue[depth+1] = INT_MAX;
			canmove[depth][canmove_i].value = search(depth);
		}
		if (depth == depthmax) {														//达到最大深度便开始估值
			//printcboard(testboard[depth]);
			canmove[depth][canmove_i].value = findvalue(canmove_i, depth);			
			//printf("%d\n", canmove[depth][canmove_i].value);
		}
		
		//根据棋子颜色，和上一层的 Alpha / Beta值 作比较，符合就 break.
		//Alpha - Beta 剪枝
		//若遇到剪枝，将下一个收录的位置变为空
		if (depth % 2 == 0) {
			if (canmove[depth][canmove_i].value <= depthvalue[depth - 1]) {
				canmove[depth][canmove_i + 1].position[0] = NOCHESS;					
				break;
			}
		}
		else if (depth % 2 == 1) {
			if (canmove[depth][canmove_i].value >= depthvalue[depth - 1]) {				
				canmove[depth][canmove_i + 1].position[0] = NOCHESS;
				break;
			}
		}
		int aaaa = 0;
	}

	//寻找每一层最大/最小值
	int return_value;
	if (depth % 2 == 1) {
		return_value = INT_MIN;
		for (int i = 0; i < 256 && (canmove[depth][i].position[0] != NOCHESS); i++)
		{
			if (canmove[depth][i].value >= return_value) {
				return_value = canmove[depth][i].value;
			}
		}
	}
	else if (depth % 2 == 0) {
		return_value = INT_MAX;
		for (int i = 0; i < 256 && (canmove[depth][i].position[0] != NOCHESS); i++)
		{
			if (canmove[depth][i].value <= return_value) {
				return_value = canmove[depth][i].value;
			}
		}
	}
	if (depth % 2 == 1) {																	//根据该层的估值情况改变上一层的 Alpha - Beta值
		if (depthvalue[depth - 1] > return_value) depthvalue[depth - 1] = return_value;
	}
	else {
		if (depthvalue[depth - 1] < return_value) depthvalue[depth - 1] = return_value;
	}
	return return_value;																	//返回该层估值
}

//收集各方面的估值
int findvalue(int canmove_i, int depth)

 
{
	int actionscore = 0, tablescore = 0, pactionscore = 0, diffscore = 0, stablescore = 0;

	tablescore   = caltable(testboard[depth]);																		
	actionscore  = calaction();
	pactionscore = potential_action();
	diffscore    = diffnum();
	if (chessboard[15][15] == iside || chessboard[0][15] == iside || chessboard[15][0] == iside || chessboard[0][0] == iside || chessnum > 236) stablescore = stabilizer();
	
	//玄学时间开始！
	if (chessnum <= 16)        canmove[depth][canmove_i].value = tablescore + 50   * actionscore + 40  * pactionscore - 100 * diffscore +               8 * table[canmove[depth][canmove_i].position[0]][canmove[depth][canmove_i].position[1]];
	else if (choosstable == 0) canmove[depth][canmove_i].value = tablescore + 160  * actionscore + 110 * pactionscore - 160 * diffscore + stablescore + 6 * table[canmove[depth][canmove_i].position[0]][canmove[depth][canmove_i].position[1]];
	else if (chessnum <= 236)  canmove[depth][canmove_i].value = tablescore + 50   * actionscore + 50  * pactionscore - 20  * diffscore + stablescore;
	else                       canmove[depth][canmove_i].value = tablescore +                                           800 * diffscore + stablescore;

	return canmove[depth][canmove_i].value;
}

//用估值表对棋盘估值
int caltable(char board[][16])
{
	int calresult = 0;
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			int inside = 0;
			if (testboard[depthmax][i][j] == iside) {
				calresult += table[i][j];
				for (int dir = 0; dir < 8; dir++)				//判断内部子（四周没有空格）
				{
					int ii = i + direction[dir][0];
					int jj = j + direction[dir][1];
					if (ii >= 0 && jj >= 0 && ii < 16 && jj < 16) {
						if (testboard[depthmax][ii][jj] == NOCHESS) inside++;
					}
				}
				if (inside == 0) calresult += INSIDE_SCORE;
			}
			if (testboard[depthmax][i][j] == oside) {
				calresult -= table[i][j];
				for (int dir = 0; dir < 8; dir++)				//判断内部子（四周没有空格）
				{
					int ii = i + direction[dir][0];
					int jj = j + direction[dir][1];
					if (ii >= 0 && jj >= 0 && ii < 16 && jj < 16) {
						if (testboard[depthmax][ii][jj] == NOCHESS) inside++;
					}
				}
				if (inside == 0) calresult -= INSIDE_SCORE;
			}
		}
	}
	return calresult;
}

//行动力估值，调用寻找行动力函数
int calaction()
{
	int calresult = 0;
	for (int find_action_i = 0; find_action_i < 16; find_action_i++)
	{
		for (int find_action_j = 0; find_action_j < 16; find_action_j++)
		{
			if (testboard[depthmax][find_action_i][find_action_j] != NOCHESS) continue;
			if (fine_canact(iside, find_action_i, find_action_j)) calresult++;
			if (fine_canact(oside, find_action_i, find_action_j)) calresult--;
		}
	}
	return calresult;
}

//计算潜在行动力
int potential_action()
{
	int paresult = 0;
	for (int pi = 0; pi < 16; pi++)
	{
		for (int pj = 0; pj < 16; pj++)
		{
			if (testboard[depthmax][pi][pj] == NOCHESS) continue;
			int num = 0;
			for (int dir = 0; dir < 8; dir++)
			{
				if (testboard[depthmax][pi + direction[dir][0]][pj + direction[dir][1]] == NOCHESS) num++;
			}
			if (testboard[depthmax][pi][pj] == iside) paresult -= num;
			else paresult += num;
		}
	}
	return paresult;
}

//检查双方棋子间的数目差异
int diffnum()
{
	int diffresult = 0;
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			if      (testboard[depthmax][i][j] == iside) diffresult++;
			else if (testboard[depthmax][i][j] == oside) diffresult--;
		}
	}
	return diffresult;
}

//稳定子估值
int stabilizer()
{
	int stableresult = 0;
	for (int i = 0; i < 16; i++)											//复制确定子棋盘到测试层
	{
		strncpy(board_stabilizer[1][i], board_stabilizer[0][i], 16);
	}
	if (chessboard[0][0] == iside || chessnum > 236) {						//当某角落是自己的棋子时，启动对应方向的搜索，当进入终局时，全方向搜索
		for (int i = 0; i < 16; i++)										
		{
			for (int j = 0; j < 16; j++)
			{
				findstabilizer(i, j, 1);									//左上—右下搜索
			}
		}
	}
	if (chessboard[15][15] == iside || chessnum > 236) {
		for (int i = 15; i >= 0; i--)
		{
			for (int j = 15; j >= 0; j--)
			{
				findstabilizer(i, j, 1);									//右下—左上搜索
			}
		}
	}
	if (chessboard[0][15] == iside || chessnum > 236) {
		for (int i = 0; i < 16; i++)
		{
			for (int j = 15; j >= 0; j--)
			{
				findstabilizer(i, j, 1);									//右上—左下搜索
			}
		}
	}
	if (chessboard[15][0] == iside || chessnum > 236) {
		for (int i = 15; i >= 0; i--)
		{
			for (int j = 0; j < 15; j++)
			{
				findstabilizer(i, j, 1);									//左下—右上搜索
			}
		}
	}
	for (int i = 0; i < 16; i++)											//统计分数
	{
		for (int j = 0; j < 16; j++)
		{
			if (board_stabilizer[1][i][j] != 0) {
				if      (board_stabilizer[1][i][j] == iside) stableresult += 2500;
				else if (board_stabilizer[1][i][j] == oside) stableresult -= 2500;
			}
		}
	}
	return stableresult;
}

//寻找稳定子
int findstabilizer(int i, int j, int boardnum)
{
	if (testboard[depthmax][i][j] == NOCHESS || board_stabilizer[boardnum][i][j] != 0) return 0;
	char me;											
	if (testboard[depthmax][i][j] == iside) me = iside;													//判断是寻找我方的稳定子还是对方的稳定子
	else me = oside;
	
	//当四个方向中都满足  其中 一端 是自己的稳定子或者是边缘  或者是  两端 都是对方的稳定子，才是稳定子
	int found = 0;																						//其中一个方向满足条件时 found++
	for (int dir = 0; dir < 4; dir++)
	{
		int save_dir = dir;
		int found_sign = 0;																				//一个方向上找到满足的要求的标志
		int ostab = 0;
		for (int a = 0; a < 2; a++) {

			//当一个方向其中一端是边界或者是同方的稳定子时，这个方向就满足了，直接found_sign = 2
			if ((i + direction[dir][0] < 0) || (i + direction[dir][0] > 15) || (j + direction[dir][1] > 15) || (j + direction[dir][1] < 0) || board_stabilizer[boardnum][i + direction[dir][0]][j + direction[dir][1]] == me) {
				found_sign = 2;
				break;
			}
			if (board_stabilizer[boardnum][i + direction[dir][0]][j + direction[dir][1]] != 0) {		//当一个方向一端是对方的稳定子，还要看另一端满不满足需求
				found_sign++;
			}
			dir += 4;
		}
		if (found_sign >= 2) found++;
		dir = save_dir;
	}
	if (found == 4) board_stabilizer[boardnum][i][j] = me;
	return 0;
}

//寻找自己可以下棋的位置，并保存于得分表中的position[2]中
int find(int depth_find, char board[16][16], char side)
{
	int canrow, cancol;
	int can_move_step = 0;
	int found = 0;

	for (canrow = 0; canrow < 16; canrow++)
	{
		for (cancol = 0; cancol < 16; cancol++)
		{
			if (board[canrow][cancol] != NOCHESS) continue;
			for (int dir = 0; dir < 8; dir++)			//各方向搜索
			{
				int i = canrow; int j = cancol;			//i,j:search
				int flip = 0;
				i += direction[dir][0];
				j += direction[dir][1];
				if (board[i][j] == side) continue;
				while (i < 16 && j < 16 && i >= 0 && j >= 0)
				{
					if (board[i][j] == NOCHESS) break;
					if (board[i][j] == side) {
						canmove[depth_find][can_move_step].position[0] = canrow;
						canmove[depth_find][can_move_step].position[1] = cancol;
						can_move_step++;
						found++;
						break;
					}
					i += direction[dir][0];
					j += direction[dir][1];
				}
			}
		}
	}
	return found;
}

//寻找可以下的地方（行动力）
int fine_canact(char side, int acti, int actj)
{
	int found = 0;
	for (int dir = 0; dir < 8; dir++)			//各方向搜索
	{
		int i = acti; int j = actj;				//i,j:search
		int flip = 0;
		i += direction[dir][0];
		j += direction[dir][1];
		if (testboard[depthmax][i][j] == side) continue;
		while (i < 16 && j < 16 && i >= 0 && j >= 0)
		{
			if (testboard[depthmax][i][j] == NOCHESS) break;
			if (testboard[depthmax][i][j] == side) return 1;
			i += direction[dir][0];
			j += direction[dir][1];
		}
	}
	return 0;
}

//下棋后，改变棋盘
void flipchess(char board[][16], int mrow, int mcol, char side)
{
	board[mrow][mcol] = side;
	for (int dir = 0; dir < 8; dir++)		//各方向搜索
	{
		int search_i = mrow; int search_j = mcol;			//i,j:search
		int flip = 0; int flipnum = 0;						//已翻棋子数，可翻棋子数
		int fi = search_i; int fj = search_j;				//fi,fj:flip
		search_i += direction[dir][0];
		search_j += direction[dir][1];
		if (board[search_i][search_j] == side) continue;
		while (search_i < 16 && search_j < 16 && search_i >= 0 && search_j >= 0)
		{
			flipnum++;
			if (board[search_i][search_j] == NOCHESS) break;
			if (board[search_i][search_j] == side)
			{
				while (flip < flipnum) {
					fi += direction[dir][0];
					fj += direction[dir][1];
					board[fi][fj] = side;
					flip++;
				}
				break;
			}
			search_i += direction[dir][0];
			search_j += direction[dir][1];
		}
	}
}

//寻找盘中央棋子的数量
//搞反了棋力反而更高，故此为玄学系列
int check_choosstable()
{
	int num = 0;
	for (int i = 2; i <= 14; i++)
	{
		for (int j = 2; j <= 14; j++)
		{
			if (chessboard[i][j] == NOCHESS) num++;
		}
	}
	if (num < 10 || chessnum > 190) return 1;						//当中盘棋子较少或边界上棋子过多时，转换估值表
	else return 0;
}

//清空结构canmove
void reset_canmove(int depth)
{
	for (int i = 0; i < 256; i++)
	{
		memset(canmove[depth][i].position, NOCHESS, sizeof(canmove[depth][i].position));
		canmove[depth][i].value = 0;
	}
}

void printcboard(char bbb[16][16])
{
	int i, j;
	printf("  A  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5\n");
	for (i = 0; i < 16; i++) {
		printf("  %-3d", i);
		for (j = 0; j < 16; j++) {
			printf("%c ", bbb[i][j]);
		}
		printf(" \n");
	}
	printf("chessnum = %d\n", chessnum);
}
