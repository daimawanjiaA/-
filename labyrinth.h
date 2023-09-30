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
int mark_1; //判断是否已为迷宫分配空间
typedef struct
{
    int x;
    int y;
    int len;
    int life;
    int time;
    int **maze;
} location; //用户用来与地图互动，，，以及储存地图
typedef struct Snode
{
    char record[10];
    struct Snode *next;
} chain;
chain *head = NULL;
Status game_record(int m);               //记录玩家游戏记录
void menu(location *loc);                //游戏菜单
void game_explain();                     //游戏说明
void game_start(location *loc);          //开始游戏
Status Creat_Map(location *loc);         //开始创建迷宫（分配空间，制造迷宫墙）
void Map_dig(int **maze, int x, int y);  //为迷宫挖通道
void special_thing(int **maze, int len); //迷宫标记特殊事件
void game_underway(location *loc);       //玩家操作
void print_map(location *loc);           //打印迷宫当前状态以及标记玩家位置
void Destroy_map(location *loc);         //销毁迷宫
void record_print(location *loc);        //打印游戏记录
void color(int i)
{
    HANDLE hOut;

    //  获取输出流的句柄
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (i == 1)
        SetConsoleTextAttribute(hOut,
                                FOREGROUND_RED |           // 前景色_红色
                                    FOREGROUND_BLUE |      // 前景色_蓝色
                                    FOREGROUND_INTENSITY); // 加强
    if (i == 2)
        SetConsoleTextAttribute(hOut,
                                FOREGROUND_RED |           // 前景色_红色
                                    FOREGROUND_INTENSITY); // 加强
    if (i == 3)
        SetConsoleTextAttribute(hOut,
                                FOREGROUND_BLUE |          // 前景色_蓝色
                                    FOREGROUND_INTENSITY); // 加强
    if (i == 4)                                            //
        SetConsoleTextAttribute(hOut,
                                FOREGROUND_RED |           // 前景色_红色
                                    FOREGROUND_GREEN |     //
                                    FOREGROUND_INTENSITY); // 加强
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
        printf("             |     1 查看历史游戏记录（H）   |\n");
        printf("             |                               |\n");
        printf("             |     2 返回主菜单(space)       |\n");
        printf("             |                               |\n");
        printf("             ---------------------------------\n");
        printf("\n这次程序小C没有检测到主人记录>o<.....\n");
        printf("\n不过小C可以带领主人看一下历史游戏记录哦^o^.....\n");
    }
    else
    {
        printf("\n\n");
        printf("             -------------------------------\n");
        printf("             |                             |\n");
        printf("             |     1 本次游戏记录:         |\n");
        printf("             |                             |\n");
        for (p = head; p != NULL; p = p->next)
            printf("             |                 (%d) %s |\n", ++i, p->record);
        printf("             |                             |\n");
        printf("             |                             |\n");
        printf("             |                             |\n");
        printf("             |                             |\n");
        printf("             |     2 查看历史游戏记录(H)   |\n");
        printf("             |                             |\n");
        printf("             |     3 返回主菜单(space)     |\n");
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
                printf("小C访问主人的游戏记录失败！！");
                menu(loc);
                return;
            }
            printf("\n\n");
            printf("               -----------------------\n");
            printf("               |       历史记录：    |\n");
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
void game_explain(location *loc) //游戏说明
{
    system("cls");
    printf("\t\t\t 游戏说明\n\n");
    printf("\t*************************************************\n");
    printf("\t*\t\t\t\t\t\t*\n");
    printf("\t*\t欢迎来到奇幻森林之迷雾幻境\t\t*\n\t*\t\t\t\t\t\t*\n");
    printf("\t*\t W\t\t\t\t\t*\n\t*\t\t\t\t\t\t*\n");
    printf("\t*\tASD键控制移动\t\t\t\t*\n\t*\t\t\t\t\t\t*\n");
    printf("\t*\t游戏中会遇到神秘NPC，会有神秘馈送哦\t*\n\t*\t\t\t\t\t\t*\n");
    printf("\t*\t游戏过程中返回主菜单(space)\t\t*\n\t*\t\t\t\t\t\t*\n");
    printf("\t*\t退出 (space)\t\t\t\t*\n\t*\t\t\t\t\t\t*\n");
    printf("\t*************************************************\n\n");
    char ch = getchar();
    while (ch != 32)
        ch = getchar();
    menu(loc);
}
void Destroy_map(location *loc) //销毁迷宫
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
void Map_dig(int **maze, int x, int y) //为迷宫挖通道
{
    int ran;
    maze[x][y] = ROUTE;
    int direction[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}}; //表明起始位置以及移动方向
    int i;
    for (i = 0; i < 4; i++) //每个格子有随机分配的四次方向的移动
    {
        ran = rand() % 4;
        int num1 = direction[0][0];
        int num2 = direction[0][1];
        direction[0][0] = direction[ran][0];
        direction[0][1] = direction[ran][1];
        direction[ran][0] = num1;
        direction[ran][1] = num2;
    }
    int px = x, py = y; //用于判断下一步是否满足凿墙条件
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
                    Map_dig(maze, px, py); //满足挖墙条件，挖墙
                    break;
                }
                else
                    break;
            }
        }
    }
}
void special_thing(int **maze, int len) //迷宫标记特殊事件
{
    int x, y, ran;
    for (x = 2; x < len - 2; x++)
        for (y = 2; y < len - 2; y++)
        {
            int n, m, mark = 2;
            for (m = -1; m < 2; m++) //为迷宫死胡同标记特殊事件
                for (n = -1; n < 2; n++)
                    if (abs(m + n) == 1)
                        if (maze[x + m][y + n] < 7 && maze[x + m][y + n] > 0)
                            mark--;
            ran = rand() % 15; //添加特殊事件
            if (mark > 0)
            {
                switch (ran)
                {
                case 1:
                    maze[x][y] = 2; //代表陷阱，生命值减1
                    break;
                case 2:
                    maze[x][y] = 3; //代表特殊任务，有NPC
                    break;
                case 3:
                    maze[x][y] = 4; //代表特殊传送
                    break;
                case 4:
                    maze[x][y] = 4; //代表特殊传送
                    break;
                case 5:
                    maze[x][y] = 4; //代表特殊传送
                    break;
                }
                if (ran >= 4 && ran <= 7) //代表时间加成
                    maze[x][y] = 5;
            }
            mark = 3;
            for (m = -1; m < 2; m++) //为迷宫死胡同标记特殊事件
                for (n = -1; n < 2; n++)
                    if (abs(m + n) == 1)
                        if (maze[x + m][y + n] < 7 && maze[x + m][y + n] > 0)
                            mark--;
            if (mark > 0 && ran == 7)
                maze[x][y] = 3; //代表特殊任务，有NPC
        }
}
Status Creat_Map(location *loc) //开始创建迷宫（分配空间，制造迷宫墙）
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
    } //为地图格分配空间分配完后全地图元素不等于1
    for (i = 0; i < loc->len; i++)
    {
        loc->maze[0][i] = ROUTE;
        loc->maze[loc->len - 1][i] = ROUTE;
        loc->maze[i][0] = ROUTE;
        loc->maze[i][loc->len - 1] = ROUTE;
    }                                   //让地图最外一层为路径，防止地图被凿串
    Map_dig(loc->maze, loc->x, loc->y); //凿地图
    for (i = loc->len - 3; i > 1; i--)  //寻找出口
    {
        if (loc->maze[i][loc->len - 3] == ROUTE)
        {
            loc->maze[i][loc->len - 2] = 6;
            break;
        }
    }
    special_thing(loc->maze, loc->len); //放置特殊事件
    return OK;
}
void print_map(location *loc) //打印迷宫当前状态以及标记玩家位置
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
                printf("→");
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
                printf("我");
                break;
            default:
                color(1);
                printf("回");
                break;
            }
        }
        printf("\n");
    }
}
void game_underway(location *loc) //玩家游戏进行中
{
    loc->maze[loc->x][loc->y] = 7; //标记玩家初始位置
    int direction[2];
    char point;
    print_map(loc); //打印迷宫
    do
    {
        scanf("%c", &point);
        if (point == '\n')
            continue;
        switch (point) //将用户输入转变为迷宫移动
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
            printf("\a\t该通道禁止通行\n");
            system("pause");
            print_map(loc);
            continue;
        }
        int i = 1;
        switch (loc->maze[loc->x + direction[0]][loc->y + direction[1]])
        {
        case ROUTE: // 道路
            loc->maze[loc->x][loc->y] = ROUTE;
            loc->x += direction[0];
            loc->y += direction[1];
            loc->time--;
            break;
        case 2: //血包
            loc->maze[loc->x][loc->y] = ROUTE;
            loc->x += direction[0];
            loc->y += direction[1];
            loc->life++;
            printf("\a\t汀~~~让小C来给主人疗伤吧\n");
            loc->time--;
            break;
        case 3: //陷阱
            loc->maze[loc->x][loc->y] = ROUTE;
            loc->x += direction[0];
            loc->y += direction[1];
            printf("\a\t您被捕兽夹夹伤了，生命值减一，小C爱莫能助<~o~>\n");
            loc->life--;
            loc->time--;
            break;
        case 4: //传送
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
                    printf("\t等小C帮主人传送到东边！！\n");
                    system("pause");
                    if (loc->maze[loc->x][loc->y + i] == 6)
                    {
                        printf("\t\a主人，你终于出来了，小C好开心！！！\n");
                        mark_1 = 0;
                        game_record(1);
                        system("pause");
                        Destroy_map(loc);
                        menu(loc);
                        return;
                    }
                    else if (loc->maze[loc->x][loc->y + i] != 1)
                        printf("\t主人，路上的东西小C帮你清理干净了！！\n");
                    loc->y += i;
                    loc->maze[loc->x][loc->y] = ROUTE;
                    break;
                }
                i++;
            }
            break;
        case 5: //时间加成
            loc->maze[loc->x][loc->y] = ROUTE;
            loc->x += direction[0];
            loc->y += direction[1];
            loc->time += 10;
            printf("\t\t小C：生命之神，时间之神，赐予我力量！！！\a");
            system("pause");
            system("cls");
            printf("\a\t\t\t汀~~恭喜你获得时间之神的赐福，获得增加十步的奖励！！！");
            loc->life++;
            break;
        case 6:
            printf("\n\n\n\n\t\t\a艾米设置的迷雾幻境^o^成功被主人击破了，三秒内小C带你返回主菜单！！！\n");
            if (!game_record(1))
            {
                printf("小C出现内部错误-o-");
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
        loc->maze[loc->x][loc->y] = 7; //标记玩家位置
        print_map(loc);                //打印迷宫
        if (loc->y == loc->len - 1)
            printf("哇偶，主人，小C带你去墙外的世界转转！！\n");
        printf("                         生命值:%d\t剩余步数:%d\n", loc->life, loc->time);
        if (loc->time == 0)
        {
            printf("\t艾米：你没时间了....哈哈哈哈哈最终还是我赢了吗！！>o<\n");
            if (!game_record(0))
            {
                printf("小C出现内部错误-o-");
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
            printf("\t艾米：你的小小命已经被祸害殆尽了，哈哈哈哈哈！！>o<\n");
            if (!game_record(0))
            {
                printf("小C出现内部错误-o-！");
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
void game_start(location *loc) //开始游戏
{
    color(4);
    system("cls");
    printf("\n                   ***********************\n");
    printf("                   **    请选择难度：   **\n");
    printf("                   **                   **\n");
    printf("                   **      困难（A）    **\n");
    printf("                   **                   **\n");
    printf("                   **      普通（B）    **\n");
    printf("                   **                   **\n");
    printf("                   **      简单（C）    **\n");
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
        printf("地图加载成功\n祝您游戏愉快！！！\n");
    else
    {
        printf("地图加载失败！！\n");
        return;
    }
    system("cls");
    game_underway(loc);
}
void menu(location *loc) //游戏菜单
{
    color(4);
    system("cls");
    printf("\n\n\n\n");
    printf("                ———————————————————\n");
    printf("               |                                      |\n");
    if (mark_1 == 1)
    {
        printf("               |        -----继续游戏 (A)-----        |\n");
        printf("               |                                      |\n");
        printf("               |        -----重新游戏 (N)-----        |\n");
    }
    else
        printf("               |        -----开始游戏 (A)-----        |\n");
    printf("               |                                      |\n");
    printf("               |        -----游戏规则 (B)-----        |\n");
    printf("               |                                      |\n");
    printf("               |        -----退出游戏 (M)-----        |\n");
    printf("               |                                      |\n");
    printf("               |        -----游戏记录 (P)-----        |\n");
    printf("               |                                      |\n");
    printf("                ———————————————————\n");
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