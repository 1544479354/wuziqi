#include <mysql.h>
#include <Windows.h>
#include <winsock.h>
#include <string.h>
#include<graphics.h>
#include<conio.h>
#include<mmsystem.h>
#include<stdio.h>
#include<stdlib.h>
#include<iostream>

#pragma comment(lib,"winmm.lib")
#pragma comment (lib, "libmysql.lib")
using namespace std;

/*数据库连接用宏*/
#define HOST "**********"  //数据库端口号
#define PORT 3306  // 数据库端口号
#define USERNAME "*******"	//数据库用户名
#define PASSWORD "abcdefg" //数据库密码
#define DATABASE "*******"	//数据库实例的名字

#define N 15//定义棋盘大小为15*15

#define WIND_WIDTH 1000//窗口宽度
#define WIND_HEIGHT 700//窗口高度

//以下三行定义棋盘颜色
#define R 200
#define G 160
#define B 30

#define deviation_x 200//偏移棋盘x位置
#define deviation_y 30//偏移棋盘x位置

int Array[N][N];//定义棋盘的二维数组
int chess_x, chess_y;
int array_x, array_y;
int shubiao_x, shubiao_y;
int yemian = 0;//0:游戏登录界面  1：游戏进行界面  -1：游戏说明界面
bool game=false;//游戏标志位

static int user1, user2, pwd1, pwd2;
wchar_t user1_c[10], pwd1_c[10], user2_c[10], pwd2_c[10], new_user_c[10], new_pwd_c[10];



void ceshi() {
	int i = 0, j = 0;
	for (i = 0; i < 15; i++)
	{
		for ( j = 0; j < 15; j++)
		{
			printf("%d ", Array[j][i]);
		}
		printf("\n");
	}
	printf("\n");
}

/*查询数据库，参数为用户名，返回的是密码,没有查询到返回0*/
int select(int user) {
	MYSQL_RES* res; //一个结果集结构体
	MYSQL my_connection; //这是一个数据库连接
	MYSQL_ROW row;  //char** 二维数组，存放一条条记录
	mysql_init(&my_connection);//初始化数据库连接
	char user2[20], pwd[20];
	char toSelectFrom[250] = "select * from user where user=";
	if (NULL != mysql_real_connect(&my_connection, HOST, USERNAME, PASSWORD, DATABASE, 0, NULL, CLIENT_FOUND_ROWS))
	{
		printf("数据库连接成功");
		_itoa_s(user, user2, 15, 10);
		strcat_s(toSelectFrom, user2);//拼接字符串  整型数据转换为字符串然后去拼接
		mysql_query(&my_connection, toSelectFrom);
		printf_s("%s\n",toSelectFrom);
		res = mysql_store_result(&my_connection);
		if (res == NULL) {//若查询结果为空，返回-1
			return -1;
		}
		else {
			while (row = mysql_fetch_row(res))
			{// 根据数据库中的属性列 先后顺序打印字符串
				if (atoi(row[2]) == 69) {
					return 0;
				}
				else {
					return atoi(row[1]);
				}
	
			}
		}
		mysql_free_result(res);//释放结果集
		mysql_close(&my_connection);//关闭数据库
	}
	else
	{
		printf_s("数据库连接失败\n");
		return -1;
	}
}
/*注册功能 两个参数分别是用户名和密码 返回1注册成功，返回-1注册失败*/
int insert(int user, int pwd)
{
	//MYSQL_RES* res; //一个结果集结构体
	MYSQL my_connection; //这是一个数据库连接
	MYSQL_ROW row;  //char** 二维数组，存放一条条记录
	mysql_init(&my_connection);//初始化数据库连接
	char user2[20], pwd2[20];
	char toSelectFrom[250] = "INSERT INTO `user`(`user`, `passwd`) VALUES (";
	if (NULL != mysql_real_connect(&my_connection, HOST, USERNAME, PASSWORD, DATABASE, 0, NULL, CLIENT_FOUND_ROWS))
	{
		//printf_s("数据库连接成功");
		//接下去拼接字符串
		_itoa_s(user, user2, 15, 10);//强制类型转为为字符 结果为user2
		_itoa_s(pwd, pwd2, 15, 10);//同上
		strcat_s(toSelectFrom, user2);//拼接数据库语句
		strcat_s(toSelectFrom, ",");
		strcat_s(toSelectFrom, pwd2);
		strcat_s(toSelectFrom, ")");
		printf_s("%s\n", toSelectFrom);//输出下sql查询语句 调试用 检查拼接是否正确
		mysql_query(&my_connection, toSelectFrom);
		mysql_store_result(&my_connection);
		//mysql_free_result(res);//释放结果集
		mysql_close(&my_connection);//关闭数据库
		return 1;

	}
	else
	{
		printf_s("数据库连接失败");
		return -1;
	}
}

void shubiao_check() {
	while (1) {
		if (MouseHit())//捕捉到鼠标动作
		{
			//int i = -1, j = -1;
			MOUSEMSG msg = GetMouseMsg();//msg储存鼠标动作

			switch (msg.uMsg)//当前鼠标的信息
			{
			case WM_LBUTTONDOWN://鼠标左击
			{
				shubiao_x = msg.x; shubiao_y = msg.y;
				printf("X:%d  Y:%d\n", shubiao_x, shubiao_y);
			}
			}
		}
	}
}

void chushihua() {
	IMAGE login;
	setbkcolor(RGB(124, 83, 201));//设置背景颜色
	cleardevice();
	loadimage(&login, L"include/img/login.jpg", 1000, 700);
	putimage(0,0, &login);//显示图片
	printf("初始化页面成功\n");
}
void bg_img() {
	IMAGE bg;
	setbkcolor(RGB(124, 83, 201));//设置背景颜色
	cleardevice();
	loadimage(&bg, L"include/img/bg.jpg", 1000, 700);
	putimage(0, 0, &bg);//显示图片
}
void shuoming_img() {
	IMAGE bg;
	setbkcolor(RGB(124, 83, 201));//设置背景颜色
	cleardevice();
	loadimage(&bg, L"include/img/shuoming.jpg", 1000, 700);
	putimage(0, 0, &bg);//显示图片
}
void win(int i) {//0代表左边
	IMAGE win;
	loadimage(&win, L"include/img/win.jpg", 167, 292);
	if(i==0)putimage(15, 80+200, &win);//显示图片
	else putimage(1000 - 167 - 15, 80+200, &win);//显示图片
	
}
void fail(int i) {
	IMAGE win;
	loadimage(&win, L"include/img/fail.jpg", 167, 292);
	if (i == 0)putimage(15, 80+200, &win);//显示图片
	else putimage(1000 - 167 - 15, 80+200, &win);//显示图片
}

/*显示用户信息 x代表偏移x多少 y代表偏移y多少*/
void showuser(int x, int y) {

		IMAGE touxiang;
		loadimage(&touxiang, L"include/img/touxiang.jpg", 100, 100);
		putimage(50+x, 200+y, &touxiang);//显示图片
		settextcolor(RGB(255, 255, 255));
		outtextxy(50+x, 200+120+y, L"用户名：");
		outtextxy(50+65+x, 200+120+y, user1_c);
		outtextxy(90+x,320+20+y,L"黑方");

		IMAGE bg;
		loadimage(&bg, L"include/img/touxiang2.jpg", 100, 100);
		putimage(850+x, 200+y, &bg);//显示图片
		settextcolor(RGB(255,255,255));
		outtextxy(850+x, 320+y, L"用户名：");
		outtextxy(850+65+x, 320+y, user2_c);
		outtextxy(890+x, 320 + 20+y, L"白方");

}

void huaqipan() {
	int i;
	setbkcolor(RGB(124, 83, 201));//设置背景颜色
	cleardevice();
	bg_img();
	setfillcolor(RGB(R, G, B));//设置填充颜色
	fillrectangle(15+ deviation_x, 15+deviation_y, 575+deviation_x, 575 + deviation_y);//画出棋盘
	setlinecolor(BLACK);//设置画出线条为黑色
	//接下去画横线
	for (i = 1; i <= N; i++)
	{
		if (i == 1 || i == N) {
			line(15 + 35+deviation_x, 15 + 35 * i - 1 + deviation_y, 575 - 35 + deviation_x, 15 + 35 * i - 1 + deviation_y);//画两遍是粗线
			line(15 + 35 * i + 1 + deviation_x, 15 + 35 + deviation_y, 15 + 35 * i + 1 + deviation_x, 575 - 35 + deviation_y);
		}
		line(15 + 35 + deviation_x, 15 + 35 * i + deviation_y, 575 - 35 + deviation_x, 15 + 35 * i + deviation_y);
		line(15 + 35 * i + deviation_x, 15 + 35 + deviation_y, 15 + 35 * i + deviation_x, 575 - 35 + deviation_y);
	}

	//画棋盘上的五个点
	setfillcolor(BLACK);//设置填充样式为黑色
	solidcircle(15 + 35 * 4 + deviation_x, 15 + 35 * 4 + deviation_y, 5);
	solidcircle(15 + 35 * 4 + deviation_x, 15 + 35 * 12 + deviation_y, 5);
	solidcircle(15 + 35 * 12 + deviation_x, 15 + 35 * 4 + deviation_y, 5);
	solidcircle(15 + 35 * 12 + deviation_x, 15 + 35 * 12 + deviation_y, 5);
	solidcircle(15 + 35 * 8 + deviation_x, 15 + 35 * 8 + deviation_y, 5);

	//输出文字信息
	//outtextxy(900, 60, L"                      五子棋游戏");
	//outtextxy(900, 80, L"功能说明：玩家通过鼠标点击落子。");


	/*下面是下方语音界面*/
	showuser(0,-100);

}
void music_down() {
	mciSendString(L"close DOWN", NULL, 0, NULL);
	mciSendString(L"open include/music/down.mp3 alias DOWN", NULL, 0, NULL);//打开音乐
	mciSendString(L"play DOWN", NULL, 0, NULL);
}
void music(int i,bool kaiguan) 
{
	switch (i)
	{

	case 0://游戏bgm
	{
		mciSendString(L"close BGM", NULL, 0, NULL);
		mciSendString(L"open include/music/bgm.mp3 alias BGM", NULL, 0, NULL);//打开音乐
		if (kaiguan) mciSendString(L"play BGM repeat", NULL, 0, NULL);
		else mciSendString(L"stop BGM", NULL, 0, NULL);
		break;

	}
	case 6://胜利
	{
		mciSendString(L"close WIN", NULL, 0, NULL);
		mciSendString(L"open include/music/win.mp3 alias WIN", NULL, 0, NULL);//打开音乐
		if(kaiguan) mciSendString(L"play WIN", NULL, 0, NULL);
		else mciSendString(L"stop WIN", NULL, 0, NULL);
		break;
		
	}
	case 1://花谢了
	{
		mciSendString(L"close HUA", NULL, 0, NULL);
		mciSendString(L"open include/music/huaxiele.mp3 alias HUA", NULL, 0, NULL);//打开音乐
		if (kaiguan) mciSendString(L"play HUA", NULL, 0, NULL);
		else mciSendString(L"stop HUA", NULL, 0, NULL);
		break;

	}
	case 2://哇哦
	{
		mciSendString(L"close WA", NULL, 0, NULL);
		mciSendString(L"open include/music/wa.mp3 alias WA", NULL, 0, NULL);//打开音乐
		if (kaiguan) mciSendString(L"play WA", NULL, 0, NULL);
		else mciSendString(L"stop WA", NULL, 0, NULL);
		break;

	}
	case 3://偷笑
	{
		mciSendString(L"close XIAO", NULL, 0, NULL);
		mciSendString(L"open include/music/xiao.mp3 alias XIAO", NULL, 0, NULL);//打开音乐
		if (kaiguan) mciSendString(L"play XIAO", NULL, 0, NULL);
		else mciSendString(L"stop XIAO", NULL, 0, NULL);
		break;

	}
	case 4://合作
	{
		mciSendString(L"close HEZUO", NULL, 0, NULL);
		mciSendString(L"open include/music/hezuo.mp3 alias HEZUO", NULL, 0, NULL);//打开音乐
		if (kaiguan) mciSendString(L"play HEZUO", NULL, 0, NULL);
		else mciSendString(L"stop HEZUO", NULL, 0, NULL);
		break;

	}
	case -1://login
	{
		mciSendString(L"close LOGIN", NULL, 0, NULL);
		mciSendString(L"open include/music/login2.mp3 alias LOGIN", NULL, 0, NULL);//打开音乐
		if (kaiguan) mciSendString(L"play LOGIN repeat", NULL, 0, NULL);
		else mciSendString(L"stop LOGIN", NULL, 0, NULL);
		break;

	}
	default:
		break;
	}
}

/*函数名：black（）
功能：画黑色旗子*/
void black(int x, int y, int i, int j) {
	Array[i][j] = 1;//在数组里面放下黑色旗子
	setfillcolor(BLACK);//设置填充颜色
	solidcircle(x , y, 15);//画圆	
}

/*函数名：white（）
功能：画白色旗子*/
void white(int x, int y, int i, int j) {
	Array[i][j] = 2;//在数组里面放下白色旗子
	setfillcolor(WHITE);//设置填充颜色
	solidcircle(x, y, 15);//画圆	
}


bool luozi(int m, int n) {//送进去的参数是鼠标的x坐标和y坐标
	int i, j, x, y;
	bool s=false;
	for ( i = 1; i<=N; i++)
	{
		for (j = 1; j <= N; j++) 
		{
			x = 15 + 35 * i + deviation_x; y = 15 + 35 * j + deviation_y;//遍历数据产生行的棋盘坐标去和鼠标坐标进行比较
			if (m >= x - 17 && m <= x + 17 && n >= y - 17 && n <= y + 17)//判断符合要求的坐标
			{
				m = x, n = y;//坐标重置
				if (Array[i-1][j-1] == 0) { chess_x = m; chess_y = n; array_x = i-1; array_y = j-1; s = true;}//找到用m n返回 退出循环
				break;
			}
		}
		if (s)//找到了外层循环也退出
			break;
	}
	return s;
}

bool Cheek_Shu(int i, int j, int n, int x)
{
	//判断竖线是否连成五子i、j棋子下标 n来表示连子个数 x黑棋为1 白棋为2  
	if (n == 5)//五子连珠 返回真
		return true;
	if (i == 15)//所有位置都看完了 没五子连珠 返回假
		return false;
	if (j == 15)//到达行的末尾
		n = 0, j = 0, i++;
	/*
	棋子个数置为0 从第二行开始处继续找有没有五子连珠
	如果n不置为0 某一行的子会与下一行连成五子 也会判赢
	*/
	if (Array[i][j] == x)//该处为某一种棋子 利用递归 找下一个位置是否为同种棋子
		Cheek_Shu(i, ++j, ++n, x);
	else//该位置没有棋子或是相反的棋子 n为0 重新计算 递归找下一个棋子
	{
		n = 0;
		Cheek_Shu(i, ++j, n, x);
	}
}
bool Cheek_Heng(int i, int j, int n, int x)
{
	//判断纵线是否连成五子i、j棋子下标 n来表示连子个数 x黑棋为1 白棋为2 
	if (n == 5)//五子连珠 返回真
		return true;
	if (j == 15)//所有位置都看完了 没五子连珠 返回假
		return false;
	if (i == 15)//到达纵线末尾
		//棋子个数置为0 从下一条纵线开始处继续找有没有五子连珠 n不置为0 也会出现和横线一样的情况
		n = 0, i = 0, j++;
	if (Array[i][j] == x)//该处为某一种棋子 利用递归 找下一个位置是否为同种棋子
		Cheek_Heng(++i, j, ++n, x);
	else//该位置没有棋子或是相反的棋子 n为0 重新计算 递归找下一个棋子
	{
		n = 0;
		Cheek_Heng(++i, j, n, x);
	}
}

/*
判断从左到右的斜线是否连成五子 i、j棋子下标 n来表示连子个数 x黑棋为1 白棋为2
判断思路：先遍历左上角到右下角 再遍历以此线的上方 到Array[0][14]位置后再跳到
Array[1][0]遍历此线的下方 直至Array[14][0]结束
所谓走.......这条路 就是一条线遍历完 再去遍历其他的线 要想遍历其他的先必须使
位置跳到边末继续斜向遍历 这样的边末全部集合就是一条路
*/
bool Win_Xie1(int i, int j, int n, int x)
{
	if (n == 5)//五子连珠 返回真
		return true;
	if (i == 14 && j == 0)//全部遍历完 没五子连珠 返回假
		return false;
	if (i != 0 && j == 15)//走Array[0][0]到Array[0][14]这条路
		n = 0, j = 16 - i, i = 0;
	if (i == 0 && j == 14)//到达跳转点 从Array[0][14]跳到Array[1][0]
		n = 0, i = 1, j = 0;
	if (i == 15 && j != 0)//走Array[0][0]到Array[14][0]这条路
		n = 0, i = 16 - j, j = 0;
	if (Array[i][j] == x)//该处为某一种棋子 利用递归 找下一个位置是否为同种棋子
		Win_Xie1(++i, ++j, ++n, x);
	else//该位置没有棋子或是相反的棋子 n为0 重新计算 递归找下一个棋子
	{
		n = 0;
		Win_Xie1(++i, ++j, n, x);
	}
}

//判断从右到左的斜线是否连成五子 i、j棋子下标 n来表示连子个数 x黑棋为1 白棋为2 
//判断思路：先遍历右上角到左下角 再遍历以此线的上方 到Array[0][0]位置后再跳到
//Array[1][14]遍历此线的下方 直至Array[14][14]结束
bool Win_Xie2(int i, int j, int n, int x)
{
	if (n == 5)//五子连珠 返回真
		return true;
	if (i == 14 && j == 14)//全部遍历完 没五子连珠 返回假
		return false;
	if (i != 0 && j == -1)//走Array[0][14]到Array[0][0]这条路
		n = 0, j = i - 2, i = 0;
	if (i == 0 && j == 0)//到达跳转点 从Array[0][0]到Array[1][14]
		n = 0, i = 1, j = 14;
	if (i == 15 && j != 14)//走Array[1][14]到Array[14][14]
		n = 0, i = j + 2, j = 14;
	if (Array[i][j] == x)//该处为某一种棋子 利用递归 找下一个位置是否为同种棋子
		Win_Xie2(++i, --j, ++n, x);
	else//该位置没有棋子或是相反的棋子 n为0 重新计算 递归找下一个棋子
	{
		n = 0;
		Win_Xie2(++i, --j, n, x);
	}
}


void shubiao() {
	static int count=0;
	while (1) 
	{
		if (MouseHit()&&game)//捕捉到鼠标动作
		{
			//int i = -1, j = -1;
			MOUSEMSG msg = GetMouseMsg();//msg储存鼠标动作

			switch (msg.uMsg)//当前鼠标的信息
			{
				case WM_LBUTTONDOWN://鼠标左击
				{
					int x = msg.x; int y = msg.y;
					if (luozi(x, y)) 
					{
						count++;
						//white(chess_x, chess_y, array_x, array_y);
						music_down();
						if (count % 2 == 0) {
							Array[array_x][array_y] = 2;//下白棋
							printf("白棋落子\n");
							ceshi();
						}
						else {
							Array[array_x][array_y] = 1;//下黑棋
							printf("黑棋落子\n");
							ceshi();
						}
							
					}
					if(x>333&&x<388&&y>620&&y<662)//第一个表情被点击
					{
						music(1,true);
						printf("第一个表情被点击\n");
					}
					if (x > 426 && x < 485 && y>620 && y < 662)//第二个表情被点击
					{
						music(2, true);
						printf("第二个表情被点击\n");
					}
					if (x > 518 && x < 569 && y>620 && y < 662)//第三个表情被点击
					{
						music(3, true);
						printf("第三个表情被点击\n");
					}
					if (x > 615 && x < 667 && y>620 && y < 662)//第四个表情被点击
					{
						music(4, true);
						printf("第四个表情被点击\n");
					}

					break;
				}
			}
		}
		if (game) {
			if (Cheek_Shu(0, 0, 0, 1) || Cheek_Heng(0, 0, 0, 1) || Win_Xie1(0, 0, 0, 1) || Win_Xie2(0, 14, 0, 1)) {
				//outtextxy(900, 300, L"黑子连成5字");
				music(0, false);
				music(6, true);
				win(0); fail(1);
				game = false;
			}
			if (Cheek_Shu(0, 0, 0, 2) || Cheek_Heng(0, 0, 0, 2) || Win_Xie1(0, 0, 0, 2) || Win_Xie2(0, 14, 0, 2)) {
				//outtextxy(900, 300, L"白子连成5字");
				music(0, false);
				music(6, true);
				win(1); fail(0);
				game = false;
			}
		}
	}

}

void shubiao_0(){
	if (MouseHit() && yemian == 0)//捕捉到鼠标动作
	{
		//int i = -1, j = -1;
		MOUSEMSG msg = GetMouseMsg();//msg储存鼠标动作

		switch (msg.uMsg)//当前鼠标的信息
		{
		case WM_LBUTTONDOWN://鼠标左击
		{
			int x = msg.x; int y = msg.y;
			printf("X:%d,Y:%d\n", x, y);
			static int new_user,new_pwd;
			if (x > 642 && x < 927 && y>203 && y < 237) //玩家账号1区域
			{
				printf("账号1被点击\n");
				InputBox(user1_c, 10, L"请输入玩家1账号：");
				settextcolor(RGB(0, 0, 0, ));//设置文字颜色黑色
				setbkcolor(RGB(255, 255, 255));//设置文字背景颜色白色
				solidrectangle(645, 205, 927, 233);//画白色矩形盖住原来的文字
				outtextxy(655, 210, user1_c);
				user1 = _wtoi(user1_c);

			}
			if (x > 642 && x < 927 && y>203 + 52 && y < 237 + 52) //玩家账号1密码
			{
				printf("账号1密码被点击\n");
				InputBox(pwd1_c, 10, L"请输入玩家1密码：");
				settextcolor(RGB(0, 0, 0, ));//设置文字颜色黑色
				setbkcolor(RGB(255, 255, 255));//设置文字背景颜色白色
				solidrectangle(645, 205 + 52, 927, 233 + 52);//画白色矩形盖住原来的文字
				outtextxy(655, 210 + 56, L"*******");
				pwd1= _wtoi(pwd1_c);

			}
			if (x > 642 && x < 927 && y>203 + 52 * 2 && y < 237 + 52 * 2) //玩家账号2
			{
				printf("账号2被点击\n");
				InputBox(user2_c, 10, L"请输入玩家2账号：");
				settextcolor(RGB(0, 0, 0, ));//设置文字颜色黑色
				setbkcolor(RGB(255, 255, 255));//设置文字背景颜色白色
				solidrectangle(645, 205 + 52 * 2, 927, 233 + 52 * 2);//画白色矩形盖住原来的文字
				outtextxy(655, 210 + 52 * 2, user2_c);
				user2=_wtoi(user2_c);


			}
			if (x > 642 && x < 927 && y>203 + 52 * 3 && y < 237 + 52 * 3) //玩家账号2
			{
				printf("账号2密码被点击\n");
				InputBox(pwd2_c, 10, L"请输入玩家2密码：");
				settextcolor(RGB(0, 0, 0, ));//设置文字颜色黑色
				setbkcolor(RGB(255, 255, 255));//设置文字背景颜色白色
				solidrectangle(645, 205 + 52 * 3, 927, 233 + 52 * 3);//画白色矩形盖住原来的文字
				outtextxy(655, 210 + 53 * 3, L"*******");
				pwd2 = _wtoi(pwd2_c);
			}
			/*登录操作*/
			if (x > 837 && x < 926 && y>436 && y < 464) //登录按钮
			{
				printf("登录按钮被点击\n");

				if (user1 != 0 && user2 != 0) 
				{
					//printf("用户%d密码为%d\n", user1, select(user1));
					//printf("用户%d密码为%d\n", user2, select(user2));
					if (pwd1 == select(user1) && pwd2 == select(user2)&&user1!=user2) {//判断查询到的密码对不对

						cleardevice();//清除屏幕内容
						game = true;
						yemian = 1;
					}
					else
					{
						printf("账号或密码不正确\n");
						MessageBox(NULL, L"账户或密码不正确", L"提示", MB_OKCANCEL | MB_ICONQUESTION);
					}
				}
				else {
					MessageBox(NULL, L"用户未填写", L"提示", MB_OKCANCEL | MB_ICONQUESTION);
					printf("用户未填写\n");
				}
			}
			
			/*注册操作*/
			if (x>643&&x<972&&y>506&&y<540) {
				printf("注册按钮被点击\n");
				InputBox(new_user_c, 10, L"请输入要注册的账号：");
				new_user = _wtoi(new_user_c);
				InputBox(new_pwd_c, 10, L"请输入要注册的密码：");
				new_pwd = _wtoi(new_pwd_c);
				if (insert(new_user, new_pwd) == 1) {
					MessageBox(NULL, L"注册成功", L"提示", MB_OKCANCEL | MB_ICONQUESTION);
				}else MessageBox(NULL, L"注册失败", L"提示", MB_OKCANCEL | MB_ICONQUESTION);
				
			}
			/*游戏说明界面*/
			if (x > 643 && x < 731 && y>435 && y < 540) {
				printf("游戏说明按钮按下\n");
				shuoming_img();
			}
			if (x > 930 && x < 1000 && y>0 && y < 70) {
				printf("返回主页面按钮按下\n");
				chushihua();
			}
			break;
		}
		}
	}
}
/*功能：不断检测数组。数组位置为1就下黑棋，数组位置为2就下白棋*/
void downchess() {
	static int i; static int j;
	while(game)
	{
		for (i = 1; i <= N; i++)
		{
			for (j = 1; j <= N; j++)
			{
				if (i - 1 == array_x && j - 1 == array_y)
				{
					
					switch (Array[array_x][array_y])
					{
					case 1:
					{
						black(chess_x, chess_y, array_x, array_y);
						break;
					}
					case 2:
					{
						white(chess_x, chess_y, array_x, array_y);
						break;
					}
					default:
						break;
					}
				}
			}	
		}
	}
}
void Array_0()//将棋盘棋子全部清除 
{
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			Array[i][j] = 0;
		}
	}
}



int main(){	
	//CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)shubiao, NULL, NULL, NULL);//开启一个线程不断检测鼠标的操作并下棋
	//CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)shubiao_check, NULL, NULL, NULL);//开启一个线程不断检测鼠标的操作
	initgraph(WIND_WIDTH, WIND_HEIGHT, EW_SHOWCONSOLE);//初始化窗口
	
	//initgraph(WIND_WIDTH, WIND_HEIGHT, EW_SHOWCONSOLE);//初始化窗口
	chushihua();
	music(-1,true);
	while (yemian == 0) {
		shubiao_0();
	}
	
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)shubiao, NULL, NULL, NULL);//开启一个线程不断检测鼠标的操作并下棋

	music(-1, false);//关闭登录页面音乐
	FlushMouseMsgBuffer();//清空鼠标
	Array_0();//清空一下数组
	
	music(0, true);//开启游戏界面音乐
	huaqipan();//画出棋盘
	downchess();//自带循环
	
	getchar();
	return 0;
}
