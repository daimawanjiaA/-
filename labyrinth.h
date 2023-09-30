#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#define length_A 42
#define length_B 22
#define length_C 12
#define OK 1
#define FALSE 0
#define ROUTE 1
typedef int Status;
int mark_1; //�ж��Ƿ���Ϊ�Թ�����ռ�
typedef struct
{
    int x;
    int y;
    int len;
    int life;
    int time;
    int **maze;
} location; //�û��������ͼ�����������Լ������ͼ
typedef struct Snode
{
    char record[10];
    struct Snode *next;
} chain;
chain *head = NULL;
Status game_record(int m);               //��¼�����Ϸ��¼
void menu(location *loc);                //��Ϸ�˵�
void game_explain();                     //��Ϸ˵��
void game_start(location *loc);          //��ʼ��Ϸ
Status Creat_Map(location *loc);         //��ʼ�����Թ�������ռ䣬�����Թ�ǽ��
void Map_dig(int **maze, int x, int y);  //Ϊ�Թ���ͨ��
void special_thing(int **maze, int len); //�Թ���������¼�
void game_underway(location *loc);       //��Ҳ���
void print_map(location *loc);           //��ӡ�Թ���ǰ״̬�Լ�������λ��
void Destroy_map(location *loc);         //�����Թ�
void record_print(location *loc);        //��ӡ��Ϸ��¼
void color(int i)
{
    HANDLE hOut;

    //  ��ȡ������ľ��
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (i == 1)
        SetConsoleTextAttribute(hOut,
                                FOREGROUND_RED |           // ǰ��ɫ_��ɫ
                                    FOREGROUND_BLUE |      // ǰ��ɫ_��ɫ
                                    FOREGROUND_INTENSITY); // ��ǿ
    if (i == 2)
        SetConsoleTextAttribute(hOut,
                                FOREGROUND_RED |           // ǰ��ɫ_��ɫ
                                    FOREGROUND_INTENSITY); // ��ǿ
    if (i == 3)
        SetConsoleTextAttribute(hOut,
                                FOREGROUND_BLUE |          // ǰ��ɫ_��ɫ
                                    FOREGROUND_INTENSITY); // ��ǿ
    if (i == 4)                                            //
        SetConsoleTextAttribute(hOut,
                                FOREGROUND_RED |           // ǰ��ɫ_��ɫ
                                    FOREGROUND_GREEN |     //
                                    FOREGROUND_INTENSITY); // ��ǿ
                                                           //   system ( "pause" );
}
void record_print(location *loc)
{
    color(4);
    int i = 0;
    chain *p;
    char ch;
    system("cls");
    if (head == NULL)
    {
        printf("\n\n");
        printf("             ---------------------------------\n");
        printf("             |                               |\n");
        printf("             |     1 �鿴��ʷ��Ϸ��¼��H��   |\n");
        printf("             |                               |\n");
        printf("             |     2 �������˵�(space)       |\n");
        printf("             |                               |\n");
        printf("             ---------------------------------\n");
        printf("\n��γ���СCû�м�⵽���˼�¼>o<.....\n");
        printf("\n����СC���Դ������˿�һ����ʷ��Ϸ��¼Ŷ^o^.....\n");
    }
    else
    {
        printf("\n\n");
        printf("             -------------------------------\n");
        printf("             |                             |\n");
        printf("             |     1 ������Ϸ��¼:         |\n");
        printf("             |                             |\n");
        for (p = head; p != NULL; p = p->next)
            printf("             |                 (%d) %s |\n", ++i, p->record);
        printf("             |                             |\n");
        printf("             |                             |\n");
        printf("             |                             |\n");
        printf("             |                             |\n");
        printf("             |     2 �鿴��ʷ��Ϸ��¼(H)   |\n");
        printf("             |                             |\n");
        printf("             |     3 �������˵�(space)     |\n");
        printf("             |                             |\n");
        printf("             -------------------------------\n");
    }
    while (1)
    {
        ch = getchar();
        if (ch == 32)
        {
            menu(loc);
            break;
        }
        else if (ch == 'H')
        {
            system("cls");
            char CH[10];
            FILE *fp;
            if ((fp = fopen("record.txt", "r")) == NULL)
            {
                printf("СC�������˵���Ϸ��¼ʧ�ܣ���");
                menu(loc);
                return;
            }
            printf("\n\n");
            printf("               -----------------------\n");
            printf("               |       ��ʷ��¼��    |\n");
            printf("               |                     |\n");
            //fscanf(fp, "%s", CH);
            while (!feof(fp)) //(!feof(fp))
            {
                fscanf(fp, "%s", CH);
                printf("               |       %s       |\n", CH);
            }
            printf("               -----------------------\n");
            fclose(fp);
            while (ch != 32)
            {
                scanf("%c", &ch);
            }
            menu(loc);
            break;
        }
        else
            continue;
    }
    return;
}
Status game_record(int m)
{
    static chain *q;
    chain *p = (chain *)malloc(sizeof(chain));
    if (p == NULL)
    {
        return FALSE;
    }
    if (m == 1)
        strcpy(p->record, "victory");
    else
        strcpy(p->record, "failure");
    p->next = NULL;
    if (head == NULL)
    {
        head = p;
        q = p;
    }
    else
    {
        q->next = p;
        q = p;
    }
    return OK;
}
void game_explain(location *loc) //��Ϸ˵��
{
    system("cls");
    printf("\t\t\t ��Ϸ˵��\n\n");
    printf("\t*************************************************\n");
    printf("\t*\t\t\t\t\t\t*\n");
    printf("\t*\t��ӭ�������ɭ��֮����þ�\t\t*\n\t*\t\t\t\t\t\t*\n");
    printf("\t*\t W\t\t\t\t\t*\n\t*\t\t\t\t\t\t*\n");
    printf("\t*\tASD�������ƶ�\t\t\t\t*\n\t*\t\t\t\t\t\t*\n");
    printf("\t*\t��Ϸ�л���������NPC��������������Ŷ\t*\n\t*\t\t\t\t\t\t*\n");
    printf("\t*\t��Ϸ�����з������˵�(space)\t\t*\n\t*\t\t\t\t\t\t*\n");
    printf("\t*\t�˳� (space)\t\t\t\t*\n\t*\t\t\t\t\t\t*\n");
    printf("\t*************************************************\n\n");
    char ch = getchar();
    while (ch != 32)
        ch = getchar();
    menu(loc);
}
void Destroy_map(location *loc) //�����Թ�
{
    int i;
    for (i = 0; i < loc->len; i++)
        free(loc->maze[i]);
    free(loc->maze);
    loc->len = -999;
    loc->life = -999;
    loc->x = 999;
    loc->y = 999;
}
void Map_dig(int **maze, int x, int y) //Ϊ�Թ���ͨ��
{
    int ran;
    maze[x][y] = ROUTE;
    int direction[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}}; //������ʼλ���Լ��ƶ�����
    int i;
    for (i = 0; i < 4; i++) //ÿ�����������������Ĵη�����ƶ�
    {
        ran = rand() % 4;
        int num1 = direction[0][0];
        int num2 = direction[0][1];
        direction[0][0] = direction[ran][0];
        direction[0][1] = direction[ran][1];
        direction[ran][0] = num1;
        direction[ran][1] = num2;
    }
    int px = x, py = y; //�����ж���һ���Ƿ�������ǽ����
    for (i = 0; i < 4; i++)
    {
        px = x + direction[i][0];
        py = y + direction[i][1];
        int mark = 2;
        while (1)
        {
            if (maze[px][py] == ROUTE)
                break;
            else
            {
                int n, m;
                for (m = -1; m < 2; m++)
                    for (n = -1; n < 2; n++)
                        if (abs(m + n) == 1)
                            if (maze[px + m][py + n] < 7 && maze[px + m][py + n] > 0)
                                mark--;
                if (mark > 0)
                {
                    Map_dig(maze, px, py); //������ǽ��������ǽ
                    break;
                }
                else
                    break;
            }
        }
    }
}
void special_thing(int **maze, int len) //�Թ���������¼�
{
    int x, y, ran;
    for (x = 2; x < len - 2; x++)
        for (y = 2; y < len - 2; y++)
        {
            int n, m, mark = 2;
            for (m = -1; m < 2; m++) //Ϊ�Թ�����ͬ��������¼�
                for (n = -1; n < 2; n++)
                    if (abs(m + n) == 1)
                        if (maze[x + m][y + n] < 7 && maze[x + m][y + n] > 0)
                            mark--;
            ran = rand() % 15; //��������¼�
            if (mark > 0)
            {
                switch (ran)
                {
                case 1:
                    maze[x][y] = 2; //�������壬����ֵ��1
                    break;
                case 2:
                    maze[x][y] = 3; //��������������NPC
                    break;
                case 3:
                    maze[x][y] = 4; //�������⴫��
                    break;
                case 4:
                    maze[x][y] = 4; //�������⴫��
                    break;
                case 5:
                    maze[x][y] = 4; //�������⴫��
                    break;
                }
                if (ran >= 4 && ran <= 7) //����ʱ��ӳ�
                    maze[x][y] = 5;
            }
            mark = 3;
            for (m = -1; m < 2; m++) //Ϊ�Թ�����ͬ��������¼�
                for (n = -1; n < 2; n++)
                    if (abs(m + n) == 1)
                        if (maze[x + m][y + n] < 7 && maze[x + m][y + n] > 0)
                            mark--;
            if (mark > 0 && ran == 7)
                maze[x][y] = 3; //��������������NPC
        }
}
Status Creat_Map(location *loc) //��ʼ�����Թ�������ռ䣬�����Թ�ǽ��
{
    int i;
    loc->x = 2, loc->y = 1;
    srand((unsigned)time(NULL));
    loc->maze = (int **)malloc(loc->len * sizeof(int *));
    if (loc->maze == NULL)
        return FALSE;
    for (i = 0; i < loc->len; i++)
    {
        loc->maze[i] = (int *)malloc(loc->len * sizeof(int));
        if (loc->maze[i] == NULL)
            return FALSE;
    } //Ϊ��ͼ�����ռ�������ȫ��ͼԪ�ز�����1
    for (i = 0; i < loc->len; i++)
    {
        loc->maze[0][i] = ROUTE;
        loc->maze[loc->len - 1][i] = ROUTE;
        loc->maze[i][0] = ROUTE;
        loc->maze[i][loc->len - 1] = ROUTE;
    }                                   //�õ�ͼ����һ��Ϊ·������ֹ��ͼ���䴮
    Map_dig(loc->maze, loc->x, loc->y); //���ͼ
    for (i = loc->len - 3; i > 1; i--)  //Ѱ�ҳ���
    {
        if (loc->maze[i][loc->len - 3] == ROUTE)
        {
            loc->maze[i][loc->len - 2] = 6;
            break;
        }
    }
    special_thing(loc->maze, loc->len); //���������¼�
    return OK;
}
void print_map(location *loc) //��ӡ�Թ���ǰ״̬�Լ�������λ��
{
    int i, j;
    printf("\n\n\n\n\n\n");
    for (i = 0; i < loc->len; i++)
    {
        printf("                       ");
        for (j = 0; j < loc->len; j++)
        {
            switch (loc->maze[i][j])
            {
            case ROUTE:
                printf("  ");
                break;
            case 2:
                color(3);
                printf("+|");
                break;
            case 3:
                color(3);
                printf("Xx");
                break;
            case 4:
                color(3);
                printf("��");
                break;
            case 5:
                color(3);
                printf("+&");
                break;
            case 6:
                printf("  ");
                break;
            case 7:
                color(2);
                printf("��");
                break;
            default:
                color(1);
                printf("��");
                break;
            }
        }
        printf("\n");
    }
}
void game_underway(location *loc) //�����Ϸ������
{
    loc->maze[loc->x][loc->y] = 7; //�����ҳ�ʼλ��
    int direction[2];
    char point;
    print_map(loc); //��ӡ�Թ�
    do
    {
        scanf("%c", &point);
        if (point == '\n')
            continue;
        switch (point) //���û�����ת��Ϊ�Թ��ƶ�
        {
        case 'W':
            direction[0] = -1;
            direction[1] = 0;
            break;
        case 'S':
            direction[0] = 1;
            direction[1] = 0;
            break;
        case 'A':
            direction[0] = 0;
            direction[1] = -1;
            break;
        case 'D':
            direction[0] = 0;
            direction[1] = 1;
            break;
        case 32:
            mark_1 = 1;
            menu(loc);
            return;
        default:
            continue;
        }
        system("cls");
        if ((loc->y + direction[1]) == 0)
        {
            printf("\a\t��ͨ����ֹͨ��\n");
            system("pause");
            print_map(loc);
            continue;
        }
        int i = 1;
        switch (loc->maze[loc->x + direction[0]][loc->y + direction[1]])
        {
        case ROUTE: // ��·
            loc->maze[loc->x][loc->y] = ROUTE;
            loc->x += direction[0];
            loc->y += direction[1];
            loc->time--;
            break;
        case 2: //Ѫ��
            loc->maze[loc->x][loc->y] = ROUTE;
            loc->x += direction[0];
            loc->y += direction[1];
            loc->life++;
            printf("\a\t͡~~~��СC�����������˰�\n");
            loc->time--;
            break;
        case 3: //����
            loc->maze[loc->x][loc->y] = ROUTE;
            loc->x += direction[0];
            loc->y += direction[1];
            printf("\a\t�������޼м����ˣ�����ֵ��һ��СC��Ī����<~o~>\n");
            loc->life--;
            loc->time--;
            break;
        case 4: //����
            loc->maze[loc->x][loc->y] = ROUTE;
            loc->x += direction[0];
            loc->y += direction[1];
            loc->maze[loc->x][loc->y] = ROUTE;
            loc->time--;
            while (i)
            {
                if (loc->maze[loc->x][loc->y + i] <= 6 && loc->maze[loc->x][loc->y + i] > 0)
                {
                    system("cls");
                    printf("\t��СC�����˴��͵����ߣ���\n");
                    system("pause");
                    if (loc->maze[loc->x][loc->y + i] == 6)
                    {
                        printf("\t\a���ˣ������ڳ����ˣ�СC�ÿ��ģ�����\n");
                        mark_1 = 0;
                        game_record(1);
                        system("pause");
                        Destroy_map(loc);
                        menu(loc);
                        return;
                    }
                    else if (loc->maze[loc->x][loc->y + i] != 1)
                        printf("\t���ˣ�·�ϵĶ���СC��������ɾ��ˣ���\n");
                    loc->y += i;
                    loc->maze[loc->x][loc->y] = ROUTE;
                    break;
                }
                i++;
            }
            break;
        case 5: //ʱ��ӳ�
            loc->maze[loc->x][loc->y] = ROUTE;
            loc->x += direction[0];
            loc->y += direction[1];
            loc->time += 10;
            printf("\t\tСC������֮��ʱ��֮�񣬴���������������\a");
            system("pause");
            system("cls");
            printf("\a\t\t\t͡~~��ϲ����ʱ��֮��Ĵ͸����������ʮ���Ľ���������");
            loc->life++;
            break;
        case 6:
            printf("\n\n\n\n\t\t\a�������õ�����þ�^o^�ɹ������˻����ˣ�������СC���㷵�����˵�������\n");
            if (!game_record(1))
            {
                printf("СC�����ڲ�����-o-");
                exit(1);
            }
            mark_1 = 0;
            system("pause");
            Destroy_map(loc);
            menu(loc);
            return;
        default:
            break;
        }
        loc->maze[loc->x][loc->y] = 7; //������λ��
        print_map(loc);                //��ӡ�Թ�
        if (loc->y == loc->len - 1)
            printf("��ż�����ˣ�СC����ȥǽ�������תת����\n");
        printf("                         ����ֵ:%d\tʣ�ಽ��:%d\n", loc->life, loc->time);
        if (loc->time == 0)
        {
            printf("\t���ף���ûʱ����....�������������ջ�����Ӯ���𣡣�>o<\n");
            if (!game_record(0))
            {
                printf("СC�����ڲ�����-o-");
                exit(1);
            }
            system("pause");
            mark_1 = 0;
            Destroy_map(loc);
            menu(loc);
            return;
        }
        if (loc->life == 0)
        {
            printf("\t���ף����СС���Ѿ������������ˣ���������������>o<\n");
            if (!game_record(0))
            {
                printf("СC�����ڲ�����-o-��");
                exit(1);
            }
            system("pause");
            mark_1 = 0;
            Destroy_map(loc);
            menu(loc);
            return;
        }
    } while (1);
}
void game_start(location *loc) //��ʼ��Ϸ
{
    color(4);
    system("cls");
    printf("\n                   ***********************\n");
    printf("                   **    ��ѡ���Ѷȣ�   **\n");
    printf("                   **                   **\n");
    printf("                   **      ���ѣ�A��    **\n");
    printf("                   **                   **\n");
    printf("                   **      ��ͨ��B��    **\n");
    printf("                   **                   **\n");
    printf("                   **      �򵥣�C��    **\n");
    printf("                   **                   **\n");
    printf("                   ***********************\n");
    char ch = getchar();
    if (ch == '\n')
    {
        game_start(loc);
        return;
    }
    switch (ch)
    {
    case 'A':
        loc->len = length_A;
        loc->time = length_A * 4;
        loc->life = 4;
        break;
    case 'B':
        loc->len = length_B;
        loc->time = length_B * 3;
        loc->life = 3;
        break;
    case 'C':
        loc->len = length_C;
        loc->time = length_C * 2;
        loc->life = 2;
        break;

    case 32:
        menu(loc);
        return;
    default:
        game_start(loc);
        return;
        break;
    }
    if (Creat_Map(loc) == OK)
        printf("��ͼ���سɹ�\nף����Ϸ��죡����\n");
    else
    {
        printf("��ͼ����ʧ�ܣ���\n");
        return;
    }
    system("cls");
    game_underway(loc);
}
void menu(location *loc) //��Ϸ�˵�
{
    color(4);
    system("cls");
    printf("\n\n\n\n");
    printf("                ��������������������������������������\n");
    printf("               |                                      |\n");
    if (mark_1 == 1)
    {
        printf("               |        -----������Ϸ (A)-----        |\n");
        printf("               |                                      |\n");
        printf("               |        -----������Ϸ (N)-----        |\n");
    }
    else
        printf("               |        -----��ʼ��Ϸ (A)-----        |\n");
    printf("               |                                      |\n");
    printf("               |        -----��Ϸ���� (B)-----        |\n");
    printf("               |                                      |\n");
    printf("               |        -----�˳���Ϸ (M)-----        |\n");
    printf("               |                                      |\n");
    printf("               |        -----��Ϸ��¼ (P)-----        |\n");
    printf("               |                                      |\n");
    printf("                ��������������������������������������\n");
    while (1)
    {
        char ch = getchar();
        if (ch == '\n')
            continue;
        switch (ch)
        {
        case 'A':
            if (mark_1 == 1)
            {
                system("cls");
                game_underway(loc);
            }
            else
                game_start(loc);
            return;
        case 'B':
            game_explain(loc);
            return;
        case 'M':
            if (mark_1 == 1)
                Destroy_map(loc);
            return;
        case 'P':
            system("cls");
            record_print(loc);
            return;
        case 'N':
        {
            Destroy_map(loc);
            mark_1 = 0;
            game_start(loc);
        }
        default:
            continue;
        }
    }
}