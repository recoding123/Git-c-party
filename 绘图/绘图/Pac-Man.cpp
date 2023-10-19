#include <iostream>
#include <graphics.h>
#include <vector>
#include <conio.h>
#include <time.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")
#define WIDTH     510
#define HEIGHT    660
#define ENEMY_NUM 4
#define SIZE      30
using namespace std;
unsigned long tt1, tt2;
IMAGE menuimg;
IMAGE pimg[4];
IMAGE eimg[4];
void playmusic() {
    mciSendString(_T("open begin.mp3 alias bkmusic"), NULL, 0, NULL);
    mciSendString(_T("play bkmusic repeat"), NULL, 0, NULL);
}
void drawMap(int x, int y) {
    setfillcolor(BLUE);
    setcolor(BLUE);
    fillrectangle(x, y, x + 30, y + 30);
}
void drawBean(int x, int y) {
    setfillcolor(YELLOW);
    fillcircle(x+15, y+15, 5);
}
void printMap(int (*m)[17]) {
    for (int i = 0; i < 22; i++) {
        for (int j = 0; j < 17; j++) {
            if (m[i][j] == 1) {
                drawMap(j * 30, i * 30);
            }
        }
    }
}
void printBean(int(*b)[17]) {
    for (int i = 0; i < 22; i++) {
        for (int j = 0; j < 17; j++) {
            if (b[i][j] == 1) {
                drawBean(j * 30, i * 30);
            }
        }
    }
}
class Player {
public:
    int x, y;//玩家坐标
    int dirx, diry;//移动方向
    int count = 0;//得分计数
    bool isdead = false;//判断玩家是否死亡
    //构造函数
    Player() {
        dirx = 1;
        diry = 0;
        x = 45;
        y = 45;
    }
    //判断玩家输入的方向
    void judge() {
        char key;
        if (_kbhit()) {
            key = _getch();
            switch (key)
            {
            case 72://上
                dirx = 0, diry = -1;
                break;
            case 80://下
                dirx = 0, diry = 1;
                break;
            case 75://左
                dirx = -1, diry = 0;
                break;
            case 77://右
                dirx = 1, diry = 0;
                break;
            default:
                break;
            }
        }
    }
    //玩家移动
    void move() {
        x += 2 * dirx;
        y += 2 * diry;
        Sleep(1);
    }
    //碰撞检测
    virtual void hit(int(*m)[17], int(*n)[17]) {
        if (dirx == 1 || dirx == -1 && diry == 0) {
            if (m[y / 30][(x + dirx*16) / 30] == 1) dirx = -dirx;
            else if (m[y / 30][(x + dirx*16) / 30] == 0) {
                if (n[y / 30][(x + dirx) / 30] == 1) {
                    n[y / 30][(x + dirx) / 30] = 0;
                    count++;
                }
            }
        }
        else if (diry == 1 || diry == -1 && dirx == 0) {
            if (m[(y + diry*16) / 30][x / 30] == 1) diry = -diry;
            else if (m[(y + diry*16) / 30][x / 30] == 0) {
                if (n[(y + diry) / 30][x / 30] == 1) {
                    n[(y + diry) / 30][x / 30] = 0;
                    count++;
                }
            }
        }
    }
};
class Enemy :public Player {
public:
    unsigned long tt1, tt2;//用来计算敌人变换方向的间隔时间
    //构造函数
    Enemy() {  
        dirx = 0;
        diry = 0;
        tt1 = GetTickCount();
    }
    //碰撞检测
    void hit(int(*m)[17], int(*n)[17]) {
        if (dirx == 1 || dirx == -1 && diry == 0) {
            if (m[y / 30][(x + dirx*16) / 30] == 1) dirx = -dirx;
        }
        else if (diry == 1 || diry == -1 && dirx == 0) {
            if (m[(y + diry*16) / 30][x / 30] == 1) diry = -diry;
        }
    }
};
void drawPlayer(Player &p) {
    switch (p.dirx) {
    case 1:putimage(p.x-15, p.y-15, &pimg[0]); break;
    case -1:putimage(p.x-15, p.y-15, &pimg[2]); break;
    case 0:if (p.diry == 1) {
        putimage(p.x-15, p.y-15, &pimg[1]);
    }
          else if (p.diry == -1) {
        putimage(p.x-15, p.y-15, &pimg[3]);
    }break;
    default:break;
    }
}
void drawEnemy(Enemy &e,int i) {
    putimage(e.x-15, e.y-15, &eimg[i]);
}
bool gameInit(int(*m)[17],Player& p,Enemy *e) {
    srand(time(0));
    initgraph(WIDTH + 100, HEIGHT);
    loadimage(&menuimg, "menu.jpeg", WIDTH + 100, HEIGHT);
    putimage(0, 0, &menuimg);
    settextcolor(YELLOW);
    setfillcolor(BLUE);
    fillroundrect(220, 300, 360, 340, 10, 10);
    settextstyle(24, 0, "宋体");
    outtextxy(240, 310, "开始游戏");
    fillroundrect(220, 370, 360, 410, 10, 10);
    settextstyle(24, 0, "宋体");
    outtextxy(240, 380, "退出游戏");
    int flag = 0;
    ExMessage msg;
    while (1) {
        if (flag) break;
        if (peekmessage(&msg, EM_MOUSE)) {
            switch (msg.message) {
            case WM_LBUTTONDOWN:
                if (msg.x >= 220 && msg.x <= 360 && msg.y >= 300 && msg.y <= 340) flag = 1;
                else if (msg.x >= 220 && msg.x <= 360 && msg.y >= 370 && msg.y <= 410) return false;
                break;
            default:break;
            }
        }
    }
    playmusic();
    cleardevice();
    loadimage(&pimg[0], "c1.png", 25, 25);
    loadimage(&pimg[1], "c3.png", 25, 25);
    loadimage(&pimg[2], "c5.png", 25, 25);
    loadimage(&pimg[3], "c7.png", 25, 25);
    loadimage(&eimg[0], "enemy.png", 25, 25);
    loadimage(&eimg[1], "enemy1.png", 25, 25);
    loadimage(&eimg[2], "enemy2.png", 25, 25);
    loadimage(&eimg[3], "enemy3.png", 25, 25);
    HWND hWND = GetHWnd();
    SetWindowText(hWND, "GAME");
    //给每个敌人初始化坐标和移动方向
    for (int i = 0; i < ENEMY_NUM; i++) {
        int n = rand() % 4;
        if (n == 0) e[i].dirx = 1, e[i].diry = 0;
        else if (n == 1) e[i].dirx = -1, e[i].diry = 0;
        else if (n == 2) e[i].dirx = 0, e[i].diry = 1;
        else if (n == 3) e[i].dirx = 0, e[i].diry = -1;
        while (1) {
            e[i].x = rand() % (WIDTH - SIZE);
            e[i].y = rand() % (HEIGHT - SIZE);
            if (e[i].x > 100 && e[i].y > 100 && (m[e[i].y / 30][e[i].x / 30] == 0))
                break;
            else
                continue;
        }
    }
    tt1 = GetTickCount();
    return true;
}
void gameDraw(int(*m)[17], int(*n)[17], Player& p,Enemy *e) {
    BeginBatchDraw();//批量绘图
    cleardevice();
    printMap(m);//绘制地图
    printBean(n);//绘制豆子
    drawPlayer(p);//绘制玩家
    p.judge();//玩家方向判断
    p.move();//玩家移动
    p.hit(m, n);//碰撞检测
    setcolor(BLUE);
    rectangle(510, 0, 610, 660);
    settextcolor(YELLOW);
    settextstyle(25, 0, "宋体");
    outtextxy(530, 280, "得分");
    char num[50];
    sprintf_s(num, "%d", p.count);
    outtextxy(535, 340, num);
    for (int i = 0; i < ENEMY_NUM; i++) {
        e[i].move();
        e[i].hit(m,n);
        if (p.x/30==e[i].x/30 && p.y/30==e[i].y/30) p.isdead = true;
        drawEnemy(e[i],i);
    }
    EndBatchDraw();
}
void gameUpdate(Enemy *e) {
    tt2 = GetTickCount();
    if (tt2 - tt1 > 1000) {
        for (int i = 0; i < ENEMY_NUM; i++) {
            int n = rand() % 4;
            if (n == 0) e[i].dirx = 1, e[i].diry = 0;
            else if (n == 1) e[i].dirx = -1, e[i].diry = 0;
            else if (n == 2) e[i].dirx = 0, e[i].diry = 1;
            else if (n == 3) e[i].dirx = 0, e[i].diry = -1;
            tt1 = tt2;
        }
    }
}
int gameEnd() {
    HWND hWND = GetHWnd();
    settextcolor(YELLOW);
    settextstyle(100, 0, "微软雅黑");
    outtextxy(20, 300, "游戏结束！");
    mciSendString(_T("close bkmusic"), NULL, 0, NULL);
    int id = MessageBox(hWND, "退出请点击“确定”，重新开始请点击“取消”", "提示", MB_OKCANCEL);
    return id;
}
int main()
{
    //使用二维数组定义地图和豆子
    begin:int map[22][17] = { {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                        {1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
                        {1,0,1,1,0,1,1,0,1,0,1,1,0,1,1,0,1},
                        {1,0,1,1,0,1,1,0,1,0,1,1,0,1,1,0,1},
                        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                        {1,0,1,1,0,1,0,1,1,1,0,1,0,1,1,0,1},
                        {1,0,1,1,0,1,0,0,1,0,0,1,0,1,1,0,1},
                        {1,0,0,0,0,1,1,0,1,0,1,1,0,0,0,0,1},
                        {1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1},
                        {1,0,0,0,0,1,1,1,0,1,1,1,0,0,0,0,1},
                        {1,0,1,1,0,1,0,0,0,0,0,1,0,1,1,0,1},
                        {1,0,1,1,0,1,1,1,1,1,1,1,0,1,1,0,1},
                        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                        {1,1,1,1,0,1,1,1,1,1,1,1,0,1,1,1,1},
                        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                        {1,0,1,1,1,1,1,0,1,0,1,1,1,1,1,0,1},
                        {1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1},
                        {1,1,1,0,1,0,1,1,1,1,1,0,1,0,1,1,1},
                        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                        {1,0,1,1,0,1,1,0,1,0,1,1,0,1,1,0,1},
                        {1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
                        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1} };
    int bean[22][17];
    for (int i = 0; i < 22; i++) {
        for (int j = 0; j < 17; j++) {
            if (map[i][j] == 0) bean[i][j] = 1;
            else bean[i][j] = 0;
        }
    }
    Player p;//创建玩家
    Enemy enemy[ENEMY_NUM];//创建敌人
    if (gameInit(map,p,enemy)) {
        while (1) {
            if (p.count == 183) goto begin;//吃完所有豆子
            gameDraw(map, bean, p,enemy);//绘制游戏
            gameUpdate(enemy);//更新敌人移动方向
            //判断玩家是否死亡
            if (p.isdead) {
                if (gameEnd() == 2) goto begin;
                else break;
            }
        }
    }
    closegraph();
    return 0;
}