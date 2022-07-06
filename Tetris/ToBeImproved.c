#pragma warning(disable:4996)   //对 Visual Studio 2019 进行的警告消除，IDE不是Visual Studio可以删去

# include <stdio.h>
# include <windows.h>
# include <time.h>
# include <conio.h>
//# include <stdlib.h>
//# include <string.h>

# define faceY 29   //游戏区行数
# define faceX 20   //游戏区列数
# define WALL  2
# define KONG  0
# define BOX 1      //已经堆积完毕的方块
# define LEFT  75
# define RIGHT 77
# define DOWN 80
# define SPACE 32
# define ESC 27  

typedef struct User {
    char id[15];
    char paw[20];
    int grade;
}Users;

struct face {                //初始化界面
    int data[faceY][faceX + 10];
    int color[faceY][faceX + 10];
} face;

struct blocks {              //初始化方块信息
    int space[4][4];
} blocks[7][4];  //blocks[base][space_z],base表示7个基础方块，space表示旋转状态

void Login();               //登陆界面 + 开始游戏
void Regist();              //注册界面 + 账号存储
void color(int co);         //改变输出字符的颜色
void HideCursor();          //隐藏光标
void Read_File();            //读取最高得分
void Write_File();           //写入最高得分
void gotoxy(int x, int y);   //坐标跳转
void inter_face();          //初始化界面
void inter_blocks();        //初始化方块信息
void start_game();          //开始游戏
void drawkong(int base, int space_z, int x, int y);     //覆盖前一个图形，画出空格
void drawblocks(int base, int space_z, int x, int y);   //画出一个图形，覆盖空格
int bottom(int base, int space_z, int x, int y);        //WALL与BOX称为底部，判断是否触碰到底部，触碰到底部返回1，未触碰到底部返回0
int eliminate();            //一行堆积满后清除该行，并记录分值

//全局变量
int nn, max = 0, grade = 0, speed = 15000, count = 0;

int main()
{
    system("title Tetris(俄罗斯方块)");         //设置cmd窗口标题
    system("mode con lines=29 cols=60");  //设置cmd窗口高度和宽度
    int input = -1;
    do {
        gotoxy(faceX - 4, faceY / 3);
        printf("-------★俄罗斯方块★------\n");
        gotoxy(faceX - 4, faceY / 3 + 2);
        printf("|  1.有账号迈快来 <登录>   |\n");
        gotoxy(faceX - 4, faceY / 3 + 3);
        printf("|  2.没有账号我要 <注册>   |\n");
        gotoxy(faceX - 4, faceY / 3 + 4);
        printf("|  0.我不想玩我要 <退出>   |\n");
        gotoxy(faceX - 4, faceY / 3 + 6);
        printf("---------------------------\n");
        gotoxy(faceX, faceY / 3 + 8);
        printf("快来选择功能 ->");
        scanf("%d", &input);
        getchar();
        system("cls");
        char ch;
        int t = 0;
        switch (input) {
        case 1:
            Login();
            gotoxy(faceX, faceY / 3);
            printf("难度选择");
            gotoxy(faceX, faceY / 3 + 2);
            printf("a 简单 ");      // t = 15000
            gotoxy(faceX, faceY / 3 + 4);
            printf("b 中等 ");      // t = 10000
            gotoxy(faceX, faceY / 3 + 6);
            printf("c 困难 ");      // t = 7000
            gotoxy(faceX, faceY / 3 + 8);
            printf("请选择难度（默认为简单）：");
            getchar();
            scanf("%c", &ch);
            getchar();
            switch (ch) {
            case 'A': case 'a': speed = 15000; break;
            case 'B': case 'b': speed = 10000; break;
            case 'C': case 'c': speed = 7000; break;
            default: break;
            }
            gotoxy(faceX, faceY / 3 + 10);
            system("pause");
            system("cls");
            color(7);               //改变总体输出字符的颜色（墙和字体的颜色）
            HideCursor();           //隐藏光标
            Read_File();             //读取最高得分
            inter_face();           //初始化界面
            inter_blocks();         //初始化方块信息
            srand(time(NULL));      //main函数中设置srand((unsigned)time(NULL));时，影响的将是所有的rand
            nn = rand() % 7;        //nn取blcks[base][space_z]种的base，使nn随机取0-6中的一个值，对应7种基础图形之一
            while (1) {
                start_game();
            }
            system("cls");
            break;
        case 2: Regist(); system("cls"); break;
        case 0: puts("退出成功，期待下次见面");
        default: break;
        }
    } while (input);
    return 0;
}

void Login()            //登陆界面 + 开始游戏
{
    Users a = { 0 }, b = { 0 };
    FILE* pf = fopen("users.txt", "r");
    if (pf == NULL) {
        printf("文件打开失败\n");
        return;
    }
    printf("欢迎来到这里，来登录吧！\n");
    printf("请输入账号 ->");
    scanf("%s", a.id);
    fread(&b, sizeof(Users), 1, pf);
    while (1) {
        if (strcmp(a.id, b.id) != 0) {
            if (feof(pf) == 0)      //未到文件尾
                fread(&b, sizeof(Users), 1, pf);
            else {
                printf("账号不存在，是不是输错啦，即将进入游客模式\n");
                fclose(pf);
                pf = NULL;
                return;
            }
        }
        else           //帐号注册过 -> 跳到输入密码
            break;      //退出无限循环，跳到输入密码
    }
    printf("请输入密码 ->");
    do {
        scanf("%s", a.paw);
        if (strcmp(a.paw, b.paw) != 0)
            printf("密码错误，再输入一次吧 ->");
        else
            break;
    } while (1);
    system("cls");     //清屏
}

void Regist()           //注册界面 + 账号存储
{
    Users a = { 0 }, b = { 0 };
    char tmp[20] = { -1 };
    FILE* pf = NULL;
    pf = fopen("users.txt", "a+");
    if (pf == NULL) {
        printf("注册时打开文件失败\n");
        return;
    }
    gotoxy(faceX - 4, faceY / 3);
    printf("欢迎来到这里，快来注册吧！\n\n");
    gotoxy(faceX, faceY / 3 + 2);
    printf("输入账号");
    scanf("%s", a.id);
    gotoxy(faceX - 4, faceY / 3 + 4);
    printf("电脑已经记住你啦！\n");
    fread(&b, sizeof(Users), 1, pf);
    //判断 有没有注册过 -> 比较字符串是否相等
    //不相等 -> 是否到文件尾
    while (1) {
        if (strcmp(a.id, b.id) != 0) {
            if (feof(pf) == 0)      //未到文件尾
                fread(&b, sizeof(Users), 1, pf);
            else
                break;
        }
        else {
            gotoxy(faceX - 4, faceY / 3 + 5);
            getchar();
            printf("该账号注册过了，直接去登录吧！\n");
            gotoxy(faceX - 4, faceY / 3 + 7);
            system("pause");
            fclose(pf);
            pf = NULL;
            return;
        }
    }
    //注册界面
    gotoxy(faceX, faceY / 3 + 6);
    printf("请输入密码 ->");
    scanf("%s", a.paw);
    gotoxy(faceX, faceY / 3 + 7);
    printf("请再输入一次密码 ->");
    do {
        scanf("%s", tmp);
        if (strcmp(tmp, a.paw) != 0) {
            gotoxy(faceX - 8, faceY / 3 + 8);
            printf("两次输入密码不一致，请再输入一次密码 ->");
        }
        else
            break;
    } while (1);             //两次输入密码一致
    fclose(pf);
    pf = NULL;
    pf = fopen("users.txt", "a");
    fwrite(&a, sizeof(Users), 1, pf);       //fwrite会在当前文件指针的位置写入数据
    system("cls");          //清屏
    gotoxy(faceX, faceY / 2);
    printf("注册成功，快去玩游戏吧!\n");
    fclose(pf);
    pf = NULL;
    return;
}

void color(int co)      //改变输出字符的颜色
{
    switch (co) {
    case 0: co = 10; break;     //绿色
    case 1: co = 3; break;      //浅绿色
    case 2: co = 14; break;     //浅黄色
    case 3: co = 11; break;     //浅蓝色
    case 4: co = 9; break;      //蓝色
    case 5: co = 12; break;     //浅红色
    case 6: co = 13; break;     //浅紫色
    default: co = 7; break;     //白色
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), co);
    //API（应用程序编程接口），用来设置控制台窗口字体颜色和背景色
}

void HideCursor()       //隐藏光标 
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cci;
    GetConsoleCursorInfo(hOut, &cci);
    cci.bVisible = FALSE;
    SetConsoleCursorInfo(hOut, &cci);
}

void Read_File()         //读取最高得分
{
    FILE* pfile;
    pfile = fopen("俄罗斯方块最高得分记录.txt", "r+");       //r+ 以可读写方式打开文件，该文件必须存在
    if (pfile == NULL) {
        pfile = fopen("俄罗斯方块最高得分记录.txt", "w+");   //打开可读写文件
        fwrite(&max, sizeof(int), 1, pfile);
    }
    fseek(pfile, 0, 0);
    fread(&max, sizeof(int), 1, pfile);
    fclose(pfile);
}

void Write_File()         //写入最高得分
{
    FILE* pfile;
    pfile = fopen("俄罗斯方块最高得分记录.txt", "w+");
    fwrite(&grade, sizeof(int), 1, pfile);
    fclose(pfile);
}

void gotoxy(int x, int y)    //坐标跳转
{
    COORD coord;        //COORD表示一个字符在控制台屏幕上的坐标
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    //API（应用程序编程接口），用来设置设置控制台窗口中光标的位置
}

void inter_face()        //初始化界面
{
    int i, j;
    for (i = 0; i < faceY; i++) {
        for (j = 0; j < faceX + 10; j++) {
            if (j == 0 || j == faceX - 1 || j == faceX + 9) {
                face.data[i][j] = WALL;
                gotoxy(2 * j, i);
                printf("■");
            }
            else if (i == faceY - 1) {
                face.data[i][j] = WALL;
                printf("■");
            }
            else {
                face.data[i][j] = KONG;
            }
        }
    }
    gotoxy(2 * faceX + 2, faceY - 18);
    printf("左移：←");

    gotoxy(2 * faceX + 2, faceY - 16);
    printf("右移：→");

    gotoxy(2 * faceX + 2, faceY - 14);
    printf("旋转：SPACE");

    gotoxy(2 * faceX + 2, faceY - 12);
    printf("暂停：S");

    gotoxy(2 * faceX + 2, faceY - 10);
    printf("退出：ESC");

    gotoxy(2 * faceX + 2, faceY - 8);
    printf("重新开始：R");

    gotoxy(2 * faceX + 2, faceY - 6);
    printf("最高纪录：%d", max);

    gotoxy(2 * faceX + 2, faceY - 4);
    printf("分数：%d", grade);
}

void inter_blocks()      //初始化方块信息
{
    int i;
    for (i = 0; i < 4; i++) {
        //长条条形状
        blocks[0][0].space[i][1] = 1;
    }

    for (i = 0; i < 2; i++) {
        //田的形状
        blocks[1][0].space[1][i + 1] = 1;
        blocks[1][0].space[2][i + 1] = 1;

        //Z的形状
        blocks[2][0].space[1][i] = 1;
        blocks[2][0].space[2][i + 1] = 1;

        //反Z的形状
        blocks[3][0].space[1][i + 1] = 1;
        blocks[3][0].space[2][i] = 1;
    }

    for (i = 0; i < 3; i++) {
        //土的形状
        blocks[4][0].space[1][1] = 1;
        blocks[4][0].space[2][i] = 1;

        //L的形状
        blocks[5][0].space[1][2] = 1;
        blocks[5][0].space[2][i] = 1;

        //反L的形状
        blocks[6][0].space[1][0] = 1;
        blocks[6][0].space[2][i] = 1;
    }

    //7个基础形状base的旋转状态space_z，旋转状态共计7*3+7=21+7=28种
    int j;
    int base, space_z, tem[4][4];
    for (base = 0; base < 7; base++) {  // 7个基础形状循环
        for (space_z = 0; space_z < 3; space_z++) {  //3种旋转状态循环
            for (i = 0; i < 4; i++) {
                for (j = 0; j < 4; j++) {
                    tem[i][j] = blocks[base][space_z].space[i][j];
                }
            }
            for (i = 0; i < 4; i++) {
                for (j = 0; j < 4; j++) {
                    blocks[base][space_z + 1].space[i][j] = tem[4 - j - 1][i];  //逆时针旋转
                //控制一边坐标不变，另一边为4 - j - 1，然后再行列互换，这样可以保证四次旋转不同
                //如果仅仅行列互换，将会导致只有两种旋转状态
                }
            }
        }
    }
}

void start_game()       //开始游戏
{
    int space_z = 0;
    int n, x, y, t;
    int i, j, ch;
    x = faceX / 2 - 2;
    y = 0;              //x,y初始值是屏幕顶端掉落图形block处
    t = 0;
    drawkong(nn, space_z, faceX + 3, 4);
    n = nn;
    nn = rand() % 7;    //随机生成下一块图形block的base
    color(nn);
    drawblocks(nn, space_z, faceX + 3, 4);
    while (1) {
        color(n);       //调光标颜色至当前图形block的base
        drawblocks(n, space_z, x, y);
        if (t == 0)
            t = speed;
        while (--t) {
            if (kbhit() != 0)
                //kbhit()为非阻塞的响应键盘输入时间，C和C++函数
                //功能和返回值：检查是否有键盘输入，有返回非0，无返回0
                break;
        }
        if (t == 0) {
            if (bottom(n, space_z, x, y + 1) != 1) {
                drawkong(n, space_z, x, y);
                y++;
            }
            else {
                for (i = 0; i < 4; i++) {
                    for (j = 0; j < 4; j++) {
                        if (blocks[n][space_z].space[i][j] == 1) {
                            face.data[y + i][x + j] = BOX;
                            face.color[y + i][x + j] = n;
                            while (eliminate());
                        }
                    }
                }
                return;
            }
        }
        else {
            ch = getch();
            switch (ch) {
            case LEFT:
                if (bottom(n, space_z, x - 1, y) != 1) {
                    drawkong(n, space_z, x, y);
                    x--;
                }
                break;
            case RIGHT:
                if (bottom(n, space_z, x + 1, y) != 1) {
                    drawkong(n, space_z, x, y);
                    x++;
                }
                break;
            case DOWN:
                if (bottom(n, space_z, x, y + 1) != 1) {
                    drawkong(n, space_z, x, y);
                    y++;
                }
                break;
            case SPACE:
                if (bottom(n, (space_z + 1) % 4, x, y + 1) != 1) {
                    drawkong(n, space_z, x, y);
                    y++;
                    space_z = (space_z + 1) % 4;
                }
                break;
            case ESC:
                system("cls");          //清屏
                color(7);
                gotoxy(faceX, faceY / 2);
                printf("   ---游戏结束---");
                gotoxy(faceX, faceY / 2 + 1);
                printf("   ---请按任意键退出---");
                system("pause>nu1");    //暂停屏幕
                exit(0);                //退出
            case 'S': case 's':
                system("pause>nu1");    //暂停屏幕
                break;
            case 'R': case 'r':
                system("cls");          //清屏，重新开始游戏
                gotoxy(faceX - 4, faceY / 3 + 5);
                system("pause");
                system("cls");
                //main();
                grade = 0;
                color(7);               //改变总体输出字符的颜色（墙和字体的颜色）
                HideCursor();           //隐藏光标
                Read_File();             //读取最高得分
                inter_face();           //初始化界面
                inter_blocks();         //初始化方块信息
                srand(time(NULL));      //main函数中设置srand((unsigned)time(NULL));时，影响的将是所有的rand
                nn = rand() % 7;        //nn取blcks[base][space_z]种的base，使nn随机取0-6中的一个值，对应7种基础图形之一
                while (1) {
                    start_game();
                }
                system("cls");
            default:
                break;
            }
        }
    }
}

void drawkong(int base, int space_z, int x, int y)      //覆盖前一个图形，画出空格
{
    int i, j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            gotoxy(2 * (x + j), y + i);
            if (blocks[base][space_z].space[i][j] == 1)
                printf("  ");
        }
    }
}

void drawblocks(int base, int space_z, int x, int y)      //画出一个图形，覆盖空格
{
    int i, j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            gotoxy(2 * (x + j), y + i);
            if (blocks[base][space_z].space[i][j] == 1)
                printf("■");
        }
    }
}

int bottom(int base, int space_z, int x, int y)         //WALL与BOX称为底部，判断是否触碰到底部，触碰到底部返回1，未触碰到底部返回0
{
    int i, j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            gotoxy(2 * (x + j), y + i);
            if (blocks[base][space_z].space[i][j] == 0)
                continue;
            else if (face.data[y + i][x + j] == WALL || face.data[y + i][x + j] == BOX)
                return 1;
        }
    }
    return 0;
}

int marks[100];      //排行榜

int eliminate()     //一行堆积满后清除该行，并记录分值
{
    int i, j, sum, m, n;
    for (i = faceY - 2; i > 4; i--) {
        sum = 0;
        for (j = 1; j < faceX - 1; j++)
            sum += face.data[i][j];
        if (sum == 0)
            break;
        if (sum == faceX - 2) {
            grade += 1000;
            color(7);
            gotoxy(2 * faceX + 2, faceY - 4);
            printf("分数：%d", grade);
            for (j = 1; j < faceX - 1; j++) {
                face.data[i][j] = KONG;
            }
            for (m = i; m > 1; m--) {
                sum = 0;
                for (n = 1; n < faceX - 1; n++) {
                    sum += face.data[m - 1][n];
                    face.data[m][n] = face.data[m - 1][n];
                    face.color[m][n] = face.color[m - 1][n];
                    if (face.data[m][n] == KONG) {
                        gotoxy(2 * n, m);
                        printf("  ");
                    }
                    else {
                        gotoxy(2 * n, m);
                        color(face.color[m][n]);
                        printf("■");
                    }
                }
                if (sum == 0)
                    return 1;
            }
        }
    }
    for (j = 1; j < faceX - 1; j++) {
        if (face.data[1][j] == BOX) {
            Sleep(1000);        //延时，到达底部后延长时间
            system("cls");      //清屏

            int k = 0, temp = 0;
            marks[count] = grade;
            count++;
            for (i = 0; i < count; i++) {
                k = i;
                for (j = i + 1; j <= count; j++) {
                    if (marks[j] > marks[k])
                        k = j;
                }
                temp = marks[k];
                marks[k] = marks[i];
                marks[i] = temp;
            }
            color(7);
            gotoxy(faceX - 4, faceY / 3);
            printf("--------★个人排行榜★--------\n");
            gotoxy(faceX - 4, faceY / 3 + 1);
            printf("  排名                  分数\n");
            for (i = 0; i < count; i++) {
                gotoxy(faceX - 4, faceY / 3 + 2 + i);
                printf("%5d%23d", i + 1, marks[i]);
            }
            gotoxy(faceX - 2, faceY - 3);
            system("pause");
            system("cls");          //清屏

            color(7);
            gotoxy(2 * (faceX / 3), faceY / 2 - 2);
            if (grade > max) {
                printf("目前最高纪录是你的%d，恭喜您打破纪录鸭", grade);
                Write_File();
            }
            else if (grade == max)
                printf("与记录持平，快来突破你的极限鸭");
            else
                printf("继续努力鸭，你与最高纪录只差%d啦", max - grade);
            gotoxy(2 * (faceX / 3), faceY / 2);
            printf("GAME OVER!");
            char ch;
            while (1) {
                gotoxy(2 * (faceX / 3), faceY / 2 + 2);
                printf("你还要玩一盘嘛（y/n）：");
                scanf("%c", &ch);
                if (ch == 'Y' || ch == 'y') {
                    system("cls");      //清屏，重新开始游戏
                    //main();
                    gotoxy(faceX - 4, faceY / 3 + 5);
                    system("pause");
                    system("cls");
                    grade = 0;
                    color(7);               //改变总体输出字符的颜色（墙和字体的颜色）
                    HideCursor();           //隐藏光标
                    Read_File();             //读取最高得分
                    inter_face();           //初始化界面
                    inter_blocks();         //初始化方块信息
                    srand(time(NULL));      //main函数中设置srand((unsigned)time(NULL));时，影响的将是所有的rand
                    nn = rand() % 7;        //nn取blcks[base][space_z]种的base，使nn随机取0-6中的一个值，对应7种基础图形之一
                    while (1) {
                        start_game();
                    }
                    system("cls");
                }
                else if (ch == 'N' || ch == 'n')
                    exit(0);
                else {
                    gotoxy(2 * (faceX / 3), faceY / 2 + 4);
                    printf("输入错误啦！快来重新输入鸭");
                }
            }
        }
    }
    return 0;
}
