#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MINE 4
#define ROW 10
#define COL 10

int value_in_array(int *array, int num);
void create_map(char ***map);
void print_map(char ***map);
void free_map(char ***map);
int *create_mine();
int is_mine(char ***map, int mine_cor[][2], int r, int c);
int count_mine(int range[], int mine_cor[][2]);
void display_count_mine(char ***map, int count_mine, int r, int c);
void find_mine_in_range(char ***map, int range[], int mine_cor[][2]);
int find_mine(char ***map, int mine_cor[][2], int r, int c);
void show_mine(char ***map, int mine_cor[][2]);
void set_flag(char *** map, int r, int c);
int check_win(char ***map);


int main()
{
    // 게임판 생성
    char ***map = malloc(sizeof(char **) * ROW);
    for (int i = 0; i < ROW; i++)
    {
        map[i] = malloc(sizeof(char *) * COL);
    }
    create_map(map);
    print_map(map);

    // 지뢰 데이터 생성
    int *mine = create_mine();
    int mine_cor[MINE][2];
    for (int i = 0; i < MINE; i++)
    {
        int r = mine[i] / COL;
        int c = mine[i] % COL;
        mine_cor[i][0] = r;
        mine_cor[i][1] = c;
    }

    // 지뢰 좌표 입력 후 할당된 데이터 해제
    free(mine);

    int check = 1;
    while (check)
    {
        // 좌표 및 모드 입력 받기
        char mode = 'k';
        int a = ROW, b = COL;
        printf("type Coordinate and Mode\nX_coordinate Y_coordinate f/m\nex)x y m/f\n");
        scanf("%d %d %c", &a, &b, &mode);
        if (a < 0 || a > (ROW - 1) || b < 0 || b > (COL - 1))
            continue;
        if (mode != 'F' && mode != 'f' && mode != 'M' && mode != 'm')
            continue;

        if (mode == 'F' || mode == 'f')
        {
            set_flag(map, a, b);
        }
        else if (mode == 'M' || mode == 'm')
        {
            // 해당 좌표가 지뢰인지 확인
            check = is_mine(map, mine_cor, a, b);
            if (check == 0)
            {
                show_mine(map, mine_cor);
                printf("You LOSE..\n");
                break;
            }
            // 지뢰가 아니라면 맵 열고, 출력
            find_mine(map, mine_cor, a, b);
        }
        print_map(map);

        // 승리 유무 확인
        int is_win = check_win(map);
        if (is_win == MINE)
        {
            print_map(map);
            printf("You WIN!!\n");
            break;
        }

    }

    free_map(map);
    return 0;
}

// 해당 변수가 배열에 있는지 확인, 지뢰 생성 중 중복 방지로 만듬
int value_in_array(int *array, int num)
{
    for (int i = 0; i < MINE; i++)
    {
        if (array[i] == num)
            return 1;
    }
    return 0;
}

// 맴 생성, 각 좌표에 유니코드 삽입
void create_map(char ***map)
{
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            map[i][j] = " ░░ ";
        }
    }
}

// 맵 출력
void print_map(char ***map)
{
    system("clear");
    printf("xy ");
    for (int i = 0; i < 10; i++)
        printf("%d   ", i);
    for (int i = 10; i < COL; i++)
        printf("%d  ", i);
    printf("\n");
    for (int i = 0; i < ROW; i++)
    {
        if (i < 10)
        {
            printf("%d ", i);
        }
        else if (i >= 10)
        {
            printf("%d", i);
        }
        for (int j = 0; j < COL; j++)
        {
            printf("%s", map[i][j]);
        }
        if (i < 10)
        {
            printf("%d ", i);
        }
        else if (i >= 10)
        {
            printf("%d", i);
        }
        printf("\n");
    }
    printf("   ");
    for (int i = 0; i < 10; i++)
        printf("%d   ", i);
    for (int i = 10; i < COL; i++)
        printf("%d  ", i);
    printf("\n");
}

// 맵에 할당된 동적 메모리 해제
void free_map(char ***map)
{
    for (int i = 0; i < ROW; i++)
    {
        free(map[i]);
    }
    free(map);
}

// 난수 생성기로 지뢰 생성
int *create_mine()
{
    int *mine = malloc(sizeof(int));
    srand((unsigned int)time(NULL));
    for (int i = 0; i < MINE; i++)
    {
        int random = rand() % ((ROW * COL) + 1);
        if (!value_in_array(mine, random))
        {
            mine[i] = random;
        }
        else
        {
            i--;
        }
    }
    return mine;
}

// 지뢰 찾기 알고리즘
int find_mine(char ***map, int mine_cor[][2], int r, int c)
{
    // 지뢰가 없다면
    if (is_mine(map, mine_cor, r, c))
    {
        // 탐지 범위를 구하고
        int range[4];
        int r1, r2, c1, c2;
        // r 축의 탐지 범위
        if (r == 0)
        {
            r1 = r;
            r2 = r + 1;
        }
        else if (r == (ROW - 1))
        {
            r1 = r - 1;
            r2 = r;
        }
        else
        {
            r1 = r - 1;
            r2 = r + 1;
        }
        // c 축의 탐지 범위
        if (c == 0)
        {
            c1 = c;
            c2 = c + 1;
        }
        else if (c == (COL - 1))
        {
            c1 = c - 1;
            c2 = c;
        }
        else
        {
            c1 = c - 1;
            c2 = c + 1;
        }
        range[0] = r1;
        range[1] = r2;
        range[2] = c1;
        range[3] = c2;
        // 탐지 범위 내의 지뢰 갯수를 구한 다음
        int count = count_mine(range, mine_cor);
        // 선택한 칸에 탐지한 지뢰의 숫자를 대입
        display_count_mine(map, count, r, c);

        if (count == 0)
        {
            for (int i = r1; i <= r2; i++)
            {
                for (int j = c1; j <= c2; j++)
                {
                    if (strcmp(map[i][j], " ░░ ") == 0)
                    {
                        find_mine(map, mine_cor, i, j);
                    }
                }
            }
        }
    }
    return 1;
}

// 선택 한 값의 지뢰의 유무 판별
int is_mine(char ***map, int mine_cor[][2], int r, int c)
{
    for (int i = 0; i < MINE; i++)
    {
        if (mine_cor[i][0] == r && mine_cor[i][1] == c)
        {
            map[r][c] = " ▓▓ ";
            return 0;
        }
    }
    return 1;
}

// 탐지 범위 내의 지뢰 갯수 구하기
int count_mine(int range[], int mine_cor[][2])
{
    int r1 = range[0];
    int r2 = range[1];
    int c1 = range[2];
    int c2 = range[3];

    int side = 0;
    for (int i = r1; i <= r2; i++)
    {
        for (int j = c1; j <= c2; j++)
        {
            for (int k = 0; k < MINE; k++)
            {
                if (mine_cor[k][0] == i && mine_cor[k][1] == j)
                {
                    side++;
                }
            }
        }
    }
    return side;
}

// 지뢰의 갯수를 맵에 저장
void display_count_mine(char ***map, int side, int r, int c)
{
    switch (side)
    {
    case 0:
        map[r][c] = "    ";
        break;
    case 1:
        map[r][c] = " 1  ";
        break;
    case 2:
        map[r][c] = " 2  ";
        break;
    case 3:
        map[r][c] = " 3  ";
        break;
    case 4:
        map[r][c] = " 4  ";
        break;
    case 5:
        map[r][c] = " 5  ";
        break;
    case 6:
        map[r][c] = " 6  ";
        break;
    case 7:
        map[r][c] = " 7  ";
        break;
    case 8:
        map[r][c] = " 8  ";
        break;
    }
}

// 범위 내에서 지뢰 찾기 알고리즘을 돌리되, 추가로 찾는건  ░░  인 칸에만 적용
void find_mine_in_range(char ***map, int range[], int mine_cor[][2])
{
    int r1 = range[0];
    int r2 = range[1];
    int c1 = range[2];
    int c2 = range[3];
    for (int i = r1; i <= r2; i++)
    {
        for (int j = c1; j <= c2; j++)
        {
            if (strcmp(map[i][j], " ░░ ") == 0)
            {
                find_mine(map, mine_cor, i, j);
            }
        }
    }
}

void show_mine(char ***map, int mine_cor[][2])
{
    system("clear");
    printf("xy ");
    for (int i = 0; i < 10; i++)
        printf("%d   ", i);
    for (int i = 10; i < COL; i++)
        printf("%d  ", i);
    printf("\n");
    for (int i = 0; i < ROW; i++)
    {
        if (i < 10)
        {
            printf("%d ", i);
        }
        else if (i >= 10)
        {
            printf("%d", i);
        }
        for (int j = 0; j < COL; j++)
        {
            if (!is_mine(map, mine_cor, i, j))
            {
                printf(" ▓▓ ");
            }
            else
            {
                printf("%s", map[i][j]);
            }
        }
        if (i < 10)
        {
            printf("%d ", i);
        }
        else if (i >= 10)
        {
            printf("%d", i);
        }
        printf("\n");
    }
    printf("   ");
    for (int i = 0; i < 10; i++)
        printf("%d   ", i);
    for (int i = 10; i < COL; i++)
        printf("%d  ", i);
    printf("\n");
}

void set_flag(char *** map, int r, int c)
{
    if (strcmp(map[r][c], " ░░ ") == 0)
    {
        map[r][c] = " ⚑⚐ ";
    }
}

int check_win(char ***map)
{
    int unchecked = 0;
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            if (strcmp(map[i][j], " ░░ ") == 0 || strcmp(map[i][j], " ⚑⚐ ") == 0)
            {
                unchecked++;
            }
        }
    }
    return unchecked;
}
