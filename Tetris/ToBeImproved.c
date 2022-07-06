#pragma warning(disable:4996)   //�� Visual Studio 2019 ���еľ���������IDE����Visual Studio����ɾȥ

# include <stdio.h>
# include <windows.h>
# include <time.h>
# include <conio.h>
//# include <stdlib.h>
//# include <string.h>

# define faceY 29   //��Ϸ������
# define faceX 20   //��Ϸ������
# define WALL  2
# define KONG  0
# define BOX 1      //�Ѿ��ѻ���ϵķ���
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

struct face {                //��ʼ������
    int data[faceY][faceX + 10];
    int color[faceY][faceX + 10];
} face;

struct blocks {              //��ʼ��������Ϣ
    int space[4][4];
} blocks[7][4];  //blocks[base][space_z],base��ʾ7���������飬space��ʾ��ת״̬

void Login();               //��½���� + ��ʼ��Ϸ
void Regist();              //ע����� + �˺Ŵ洢
void color(int co);         //�ı�����ַ�����ɫ
void HideCursor();          //���ع��
void Read_File();            //��ȡ��ߵ÷�
void Write_File();           //д����ߵ÷�
void gotoxy(int x, int y);   //������ת
void inter_face();          //��ʼ������
void inter_blocks();        //��ʼ��������Ϣ
void start_game();          //��ʼ��Ϸ
void drawkong(int base, int space_z, int x, int y);     //����ǰһ��ͼ�Σ������ո�
void drawblocks(int base, int space_z, int x, int y);   //����һ��ͼ�Σ����ǿո�
int bottom(int base, int space_z, int x, int y);        //WALL��BOX��Ϊ�ײ����ж��Ƿ������ײ����������ײ�����1��δ�������ײ�����0
int eliminate();            //һ�жѻ�����������У�����¼��ֵ

//ȫ�ֱ���
int nn, max = 0, grade = 0, speed = 15000, count = 0;

int main()
{
    system("title Tetris(����˹����)");         //����cmd���ڱ���
    system("mode con lines=29 cols=60");  //����cmd���ڸ߶ȺͿ��
    int input = -1;
    do {
        gotoxy(faceX - 4, faceY / 3);
        printf("-------�����˹�����------\n");
        gotoxy(faceX - 4, faceY / 3 + 2);
        printf("|  1.���˺������� <��¼>   |\n");
        gotoxy(faceX - 4, faceY / 3 + 3);
        printf("|  2.û���˺���Ҫ <ע��>   |\n");
        gotoxy(faceX - 4, faceY / 3 + 4);
        printf("|  0.�Ҳ�������Ҫ <�˳�>   |\n");
        gotoxy(faceX - 4, faceY / 3 + 6);
        printf("---------------------------\n");
        gotoxy(faceX, faceY / 3 + 8);
        printf("����ѡ���� ->");
        scanf("%d", &input);
        getchar();
        system("cls");
        char ch;
        int t = 0;
        switch (input) {
        case 1:
            Login();
            gotoxy(faceX, faceY / 3);
            printf("�Ѷ�ѡ��");
            gotoxy(faceX, faceY / 3 + 2);
            printf("a �� ");      // t = 15000
            gotoxy(faceX, faceY / 3 + 4);
            printf("b �е� ");      // t = 10000
            gotoxy(faceX, faceY / 3 + 6);
            printf("c ���� ");      // t = 7000
            gotoxy(faceX, faceY / 3 + 8);
            printf("��ѡ���Ѷȣ�Ĭ��Ϊ�򵥣���");
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
            color(7);               //�ı���������ַ�����ɫ��ǽ���������ɫ��
            HideCursor();           //���ع��
            Read_File();             //��ȡ��ߵ÷�
            inter_face();           //��ʼ������
            inter_blocks();         //��ʼ��������Ϣ
            srand(time(NULL));      //main����������srand((unsigned)time(NULL));ʱ��Ӱ��Ľ������е�rand
            nn = rand() % 7;        //nnȡblcks[base][space_z]�ֵ�base��ʹnn���ȡ0-6�е�һ��ֵ����Ӧ7�ֻ���ͼ��֮һ
            while (1) {
                start_game();
            }
            system("cls");
            break;
        case 2: Regist(); system("cls"); break;
        case 0: puts("�˳��ɹ����ڴ��´μ���");
        default: break;
        }
    } while (input);
    return 0;
}

void Login()            //��½���� + ��ʼ��Ϸ
{
    Users a = { 0 }, b = { 0 };
    FILE* pf = fopen("users.txt", "r");
    if (pf == NULL) {
        printf("�ļ���ʧ��\n");
        return;
    }
    printf("��ӭ�����������¼�ɣ�\n");
    printf("�������˺� ->");
    scanf("%s", a.id);
    fread(&b, sizeof(Users), 1, pf);
    while (1) {
        if (strcmp(a.id, b.id) != 0) {
            if (feof(pf) == 0)      //δ���ļ�β
                fread(&b, sizeof(Users), 1, pf);
            else {
                printf("�˺Ų����ڣ��ǲ�������������������ο�ģʽ\n");
                fclose(pf);
                pf = NULL;
                return;
            }
        }
        else           //�ʺ�ע��� -> ������������
            break;      //�˳�����ѭ����������������
    }
    printf("���������� ->");
    do {
        scanf("%s", a.paw);
        if (strcmp(a.paw, b.paw) != 0)
            printf("�������������һ�ΰ� ->");
        else
            break;
    } while (1);
    system("cls");     //����
}

void Regist()           //ע����� + �˺Ŵ洢
{
    Users a = { 0 }, b = { 0 };
    char tmp[20] = { -1 };
    FILE* pf = NULL;
    pf = fopen("users.txt", "a+");
    if (pf == NULL) {
        printf("ע��ʱ���ļ�ʧ��\n");
        return;
    }
    gotoxy(faceX - 4, faceY / 3);
    printf("��ӭ�����������ע��ɣ�\n\n");
    gotoxy(faceX, faceY / 3 + 2);
    printf("�����˺�");
    scanf("%s", a.id);
    gotoxy(faceX - 4, faceY / 3 + 4);
    printf("�����Ѿ���ס������\n");
    fread(&b, sizeof(Users), 1, pf);
    //�ж� ��û��ע��� -> �Ƚ��ַ����Ƿ����
    //����� -> �Ƿ��ļ�β
    while (1) {
        if (strcmp(a.id, b.id) != 0) {
            if (feof(pf) == 0)      //δ���ļ�β
                fread(&b, sizeof(Users), 1, pf);
            else
                break;
        }
        else {
            gotoxy(faceX - 4, faceY / 3 + 5);
            getchar();
            printf("���˺�ע����ˣ�ֱ��ȥ��¼�ɣ�\n");
            gotoxy(faceX - 4, faceY / 3 + 7);
            system("pause");
            fclose(pf);
            pf = NULL;
            return;
        }
    }
    //ע�����
    gotoxy(faceX, faceY / 3 + 6);
    printf("���������� ->");
    scanf("%s", a.paw);
    gotoxy(faceX, faceY / 3 + 7);
    printf("��������һ������ ->");
    do {
        scanf("%s", tmp);
        if (strcmp(tmp, a.paw) != 0) {
            gotoxy(faceX - 8, faceY / 3 + 8);
            printf("�����������벻һ�£���������һ������ ->");
        }
        else
            break;
    } while (1);             //������������һ��
    fclose(pf);
    pf = NULL;
    pf = fopen("users.txt", "a");
    fwrite(&a, sizeof(Users), 1, pf);       //fwrite���ڵ�ǰ�ļ�ָ���λ��д������
    system("cls");          //����
    gotoxy(faceX, faceY / 2);
    printf("ע��ɹ�����ȥ����Ϸ��!\n");
    fclose(pf);
    pf = NULL;
    return;
}

void color(int co)      //�ı�����ַ�����ɫ
{
    switch (co) {
    case 0: co = 10; break;     //��ɫ
    case 1: co = 3; break;      //ǳ��ɫ
    case 2: co = 14; break;     //ǳ��ɫ
    case 3: co = 11; break;     //ǳ��ɫ
    case 4: co = 9; break;      //��ɫ
    case 5: co = 12; break;     //ǳ��ɫ
    case 6: co = 13; break;     //ǳ��ɫ
    default: co = 7; break;     //��ɫ
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), co);
    //API��Ӧ�ó����̽ӿڣ����������ÿ���̨����������ɫ�ͱ���ɫ
}

void HideCursor()       //���ع�� 
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cci;
    GetConsoleCursorInfo(hOut, &cci);
    cci.bVisible = FALSE;
    SetConsoleCursorInfo(hOut, &cci);
}

void Read_File()         //��ȡ��ߵ÷�
{
    FILE* pfile;
    pfile = fopen("����˹������ߵ÷ּ�¼.txt", "r+");       //r+ �Կɶ�д��ʽ���ļ������ļ��������
    if (pfile == NULL) {
        pfile = fopen("����˹������ߵ÷ּ�¼.txt", "w+");   //�򿪿ɶ�д�ļ�
        fwrite(&max, sizeof(int), 1, pfile);
    }
    fseek(pfile, 0, 0);
    fread(&max, sizeof(int), 1, pfile);
    fclose(pfile);
}

void Write_File()         //д����ߵ÷�
{
    FILE* pfile;
    pfile = fopen("����˹������ߵ÷ּ�¼.txt", "w+");
    fwrite(&grade, sizeof(int), 1, pfile);
    fclose(pfile);
}

void gotoxy(int x, int y)    //������ת
{
    COORD coord;        //COORD��ʾһ���ַ��ڿ���̨��Ļ�ϵ�����
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    //API��Ӧ�ó����̽ӿڣ��������������ÿ���̨�����й���λ��
}

void inter_face()        //��ʼ������
{
    int i, j;
    for (i = 0; i < faceY; i++) {
        for (j = 0; j < faceX + 10; j++) {
            if (j == 0 || j == faceX - 1 || j == faceX + 9) {
                face.data[i][j] = WALL;
                gotoxy(2 * j, i);
                printf("��");
            }
            else if (i == faceY - 1) {
                face.data[i][j] = WALL;
                printf("��");
            }
            else {
                face.data[i][j] = KONG;
            }
        }
    }
    gotoxy(2 * faceX + 2, faceY - 18);
    printf("���ƣ���");

    gotoxy(2 * faceX + 2, faceY - 16);
    printf("���ƣ���");

    gotoxy(2 * faceX + 2, faceY - 14);
    printf("��ת��SPACE");

    gotoxy(2 * faceX + 2, faceY - 12);
    printf("��ͣ��S");

    gotoxy(2 * faceX + 2, faceY - 10);
    printf("�˳���ESC");

    gotoxy(2 * faceX + 2, faceY - 8);
    printf("���¿�ʼ��R");

    gotoxy(2 * faceX + 2, faceY - 6);
    printf("��߼�¼��%d", max);

    gotoxy(2 * faceX + 2, faceY - 4);
    printf("������%d", grade);
}

void inter_blocks()      //��ʼ��������Ϣ
{
    int i;
    for (i = 0; i < 4; i++) {
        //��������״
        blocks[0][0].space[i][1] = 1;
    }

    for (i = 0; i < 2; i++) {
        //�����״
        blocks[1][0].space[1][i + 1] = 1;
        blocks[1][0].space[2][i + 1] = 1;

        //Z����״
        blocks[2][0].space[1][i] = 1;
        blocks[2][0].space[2][i + 1] = 1;

        //��Z����״
        blocks[3][0].space[1][i + 1] = 1;
        blocks[3][0].space[2][i] = 1;
    }

    for (i = 0; i < 3; i++) {
        //������״
        blocks[4][0].space[1][1] = 1;
        blocks[4][0].space[2][i] = 1;

        //L����״
        blocks[5][0].space[1][2] = 1;
        blocks[5][0].space[2][i] = 1;

        //��L����״
        blocks[6][0].space[1][0] = 1;
        blocks[6][0].space[2][i] = 1;
    }

    //7��������״base����ת״̬space_z����ת״̬����7*3+7=21+7=28��
    int j;
    int base, space_z, tem[4][4];
    for (base = 0; base < 7; base++) {  // 7��������״ѭ��
        for (space_z = 0; space_z < 3; space_z++) {  //3����ת״̬ѭ��
            for (i = 0; i < 4; i++) {
                for (j = 0; j < 4; j++) {
                    tem[i][j] = blocks[base][space_z].space[i][j];
                }
            }
            for (i = 0; i < 4; i++) {
                for (j = 0; j < 4; j++) {
                    blocks[base][space_z + 1].space[i][j] = tem[4 - j - 1][i];  //��ʱ����ת
                //����һ�����겻�䣬��һ��Ϊ4 - j - 1��Ȼ�������л������������Ա�֤�Ĵ���ת��ͬ
                //����������л��������ᵼ��ֻ��������ת״̬
                }
            }
        }
    }
}

void start_game()       //��ʼ��Ϸ
{
    int space_z = 0;
    int n, x, y, t;
    int i, j, ch;
    x = faceX / 2 - 2;
    y = 0;              //x,y��ʼֵ����Ļ���˵���ͼ��block��
    t = 0;
    drawkong(nn, space_z, faceX + 3, 4);
    n = nn;
    nn = rand() % 7;    //���������һ��ͼ��block��base
    color(nn);
    drawblocks(nn, space_z, faceX + 3, 4);
    while (1) {
        color(n);       //�������ɫ����ǰͼ��block��base
        drawblocks(n, space_z, x, y);
        if (t == 0)
            t = speed;
        while (--t) {
            if (kbhit() != 0)
                //kbhit()Ϊ����������Ӧ��������ʱ�䣬C��C++����
                //���ܺͷ���ֵ������Ƿ��м������룬�з��ط�0���޷���0
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
                system("cls");          //����
                color(7);
                gotoxy(faceX, faceY / 2);
                printf("   ---��Ϸ����---");
                gotoxy(faceX, faceY / 2 + 1);
                printf("   ---�밴������˳�---");
                system("pause>nu1");    //��ͣ��Ļ
                exit(0);                //�˳�
            case 'S': case 's':
                system("pause>nu1");    //��ͣ��Ļ
                break;
            case 'R': case 'r':
                system("cls");          //���������¿�ʼ��Ϸ
                gotoxy(faceX - 4, faceY / 3 + 5);
                system("pause");
                system("cls");
                //main();
                grade = 0;
                color(7);               //�ı���������ַ�����ɫ��ǽ���������ɫ��
                HideCursor();           //���ع��
                Read_File();             //��ȡ��ߵ÷�
                inter_face();           //��ʼ������
                inter_blocks();         //��ʼ��������Ϣ
                srand(time(NULL));      //main����������srand((unsigned)time(NULL));ʱ��Ӱ��Ľ������е�rand
                nn = rand() % 7;        //nnȡblcks[base][space_z]�ֵ�base��ʹnn���ȡ0-6�е�һ��ֵ����Ӧ7�ֻ���ͼ��֮һ
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

void drawkong(int base, int space_z, int x, int y)      //����ǰһ��ͼ�Σ������ո�
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

void drawblocks(int base, int space_z, int x, int y)      //����һ��ͼ�Σ����ǿո�
{
    int i, j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            gotoxy(2 * (x + j), y + i);
            if (blocks[base][space_z].space[i][j] == 1)
                printf("��");
        }
    }
}

int bottom(int base, int space_z, int x, int y)         //WALL��BOX��Ϊ�ײ����ж��Ƿ������ײ����������ײ�����1��δ�������ײ�����0
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

int marks[100];      //���а�

int eliminate()     //һ�жѻ�����������У�����¼��ֵ
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
            printf("������%d", grade);
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
                        printf("��");
                    }
                }
                if (sum == 0)
                    return 1;
            }
        }
    }
    for (j = 1; j < faceX - 1; j++) {
        if (face.data[1][j] == BOX) {
            Sleep(1000);        //��ʱ������ײ����ӳ�ʱ��
            system("cls");      //����

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
            printf("--------��������а��--------\n");
            gotoxy(faceX - 4, faceY / 3 + 1);
            printf("  ����                  ����\n");
            for (i = 0; i < count; i++) {
                gotoxy(faceX - 4, faceY / 3 + 2 + i);
                printf("%5d%23d", i + 1, marks[i]);
            }
            gotoxy(faceX - 2, faceY - 3);
            system("pause");
            system("cls");          //����

            color(7);
            gotoxy(2 * (faceX / 3), faceY / 2 - 2);
            if (grade > max) {
                printf("Ŀǰ��߼�¼�����%d����ϲ�����Ƽ�¼Ѽ", grade);
                Write_File();
            }
            else if (grade == max)
                printf("���¼��ƽ������ͻ����ļ���Ѽ");
            else
                printf("����Ŭ��Ѽ��������߼�¼ֻ��%d��", max - grade);
            gotoxy(2 * (faceX / 3), faceY / 2);
            printf("GAME OVER!");
            char ch;
            while (1) {
                gotoxy(2 * (faceX / 3), faceY / 2 + 2);
                printf("�㻹Ҫ��һ���y/n����");
                scanf("%c", &ch);
                if (ch == 'Y' || ch == 'y') {
                    system("cls");      //���������¿�ʼ��Ϸ
                    //main();
                    gotoxy(faceX - 4, faceY / 3 + 5);
                    system("pause");
                    system("cls");
                    grade = 0;
                    color(7);               //�ı���������ַ�����ɫ��ǽ���������ɫ��
                    HideCursor();           //���ع��
                    Read_File();             //��ȡ��ߵ÷�
                    inter_face();           //��ʼ������
                    inter_blocks();         //��ʼ��������Ϣ
                    srand(time(NULL));      //main����������srand((unsigned)time(NULL));ʱ��Ӱ��Ľ������е�rand
                    nn = rand() % 7;        //nnȡblcks[base][space_z]�ֵ�base��ʹnn���ȡ0-6�е�һ��ֵ����Ӧ7�ֻ���ͼ��֮һ
                    while (1) {
                        start_game();
                    }
                    system("cls");
                }
                else if (ch == 'N' || ch == 'n')
                    exit(0);
                else {
                    gotoxy(2 * (faceX / 3), faceY / 2 + 4);
                    printf("�����������������������Ѽ");
                }
            }
        }
    }
    return 0;
}
