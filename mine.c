#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int value_in_array(int *array, int num);
void create_map(char ***map);
void print_map(char ***map);
void free_map(char ***map);
int *create_mine();
int is_mine(char ***map, int mine_cor[][2], int r, int c);
int count_mine(int range[], int mine_cor[][2]);
void display_count_mine(char ***map, int count_mine, int r, int c);
void check_side(char ***map, int range[], int side);
void find_mine_in_range(char ***map, int range[], int mine_cor[][2]);
int find_mine(char ***map, int mine_cor[][2], int r, int c);



int main() {
    // 게임판 생성
    char ***map = malloc(sizeof(char **) * 10);
    for (int i = 0; i < 10; i++) {
        map[i] = malloc(sizeof(char *) * 15);
    }
    create_map(map);
    print_map(map);

    // 지뢰 데이터 생성
    int *mine = create_mine();
    int mine_cor[20][2];
    for (int i = 0; i < 20; i++) {
        int r = mine[i] / 15;
        int c = mine[i] % 15;
        mine_cor[i][0] = r;
        mine_cor[i][1] = c;
    }
    //생성된 지뢰 확인
    //for (int i = 0; i < 20; i++) {
    //    printf("%d %d\n", mine_cor[i][0], mine_cor[i][1]);
    //}
    // 지뢰 좌표 입력 후 할당된 데이터 해제
    free(mine);

    int check = 1;
    while(check) {
        int a, b;
        scanf("%d %d", &a, &b);
        if (a < 0 || a > 9 || b < 0 || b > 14) continue;

        check = is_mine(map, mine_cor, a, b);
        find_mine(map, mine_cor, a, b);
        print_map(map);
    }











    free_map(map);
    return 0;
}

// 해당 변수가 배열에 있는지 확인, 지뢰 생성 중 중복 방지로 만듬
int value_in_array(int *array, int num) {
    for (int i = 0; i < 20; i++) {
        if (array[i] == num) return 1;
    }
    return 0;
}

// 맴 생성, 각 좌표에 유니코드 삽입
void create_map(char ***map) {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 15; j++) {
            map[i][j] = " ░░ ";
        }
    }
}

// 맵 출력
void print_map(char ***map) {
    system("clear");
    printf("  ");
    for (int i = 0; i < 10; i++) printf("%d   ", i);
    for (int i = 10; i < 15; i++) printf("%d  ", i);
    printf("\n");
    for (int i = 0; i < 10; i++) {
        printf("%d", i);
        for (int j = 0; j < 15; j++) {
            printf("%s", map[i][j]);
        }
        printf("\n");
    }
}

// 맵에 할당된 동적 메모리 해제
void free_map(char ***map) {
    for (int i = 0; i < 10; i++) {
        free(map[i]);
    }
    free(map);
}

// 난수 생성기로 지뢰 생성
int *create_mine() {
    int *mine = malloc(sizeof(int));
    srand((unsigned int)time(NULL));
    for (int i = 0; i < 20; i++) {
        int random = rand() % 151;
        if (!value_in_array(mine, random)) {
            mine[i] = random;
        } else {
            i--;
        }
    }
    return mine;
}

// 지뢰 찾기 알고리즘
int find_mine(char ***map, int mine_cor[][2], int r, int c) {
    // 지뢰가 없다면
    if (is_mine(map, mine_cor, r, c)) {
        // 탐지 범위를 구하고
        int range[4];
        int r1, r2, c1, c2;
        // r 축의 탐지 범위
        if (r == 0) {
            r1 = r;
            r2 = r + 1;
        } else if (r == 9) {
            r1 = r - 1;
            r2 = r;
        } else {
            r1 = r - 1;
            r2 = r + 1;
        }
        // c 축의 탐지 범위
        if (c == 0) {
            c1 = c;
            c2 = c + 1;
        } else if (r == 14) {
            c1 = c - 1;
            c2 = c;
        } else {
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
//        // 이외의 탐지한 칸은 임시 체크를 해 준 뒤
//        check_side(map, range, count);
        // ░░  인 칸만 추가로 다시 지뢰를 찾는다
        find_mine_in_range(map, range, mine_cor);
    }
    return 1;
}

// 선택 한 값의 지뢰의 유무 판별
int is_mine(char ***map, int mine_cor[][2], int r, int c) {
    for (int i = 0; i < 20; i++) {
        if (mine_cor[i][0] == r && mine_cor[i][1] == c) {
            map[r][c] = " ▓▓ ";
            return 0;
        }
    }
    return 1;
}

// 탐지 범위 내의 지뢰 갯수 구하기
int count_mine(int range[], int mine_cor[][2]) {
    int r1 = range[0];
    int r2 = range[1];
    int c1 = range[2];
    int c2 = range[3];

    int side = 0;
    for (int i = r1; i <= r2; i++) {
        for (int j = c1; j <= c2; j++) {
            for (int k = 0; k < 20; k++) {
                if (mine_cor[k][0] == i && mine_cor[k][1] == j) {
                    side++;

                }
            }
        }
    }
    return side;
}

// 지뢰의 갯수를 맵에 저장
void display_count_mine(char ***map, int side, int r, int c) {
    switch (side) {
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

// 근접한 칸에 임시 체크
void check_side(char ***map, int range[], int side) {
    int r1 = range[0];
    int r2 = range[1];
    int c1 = range[2];
    int c2 = range[3];

    if (side == 0) {
        for (int i = r1; i <= r2; i++) {
            for (int j = c1; j <= c2; j++) {
                if (strcmp(map[i][j], " ░░ ") == 0) {
                    map[i][j] = " 00 ";
                }
            }
        }
    }
}

// 범위 내에서 지뢰 찾기 알고리즘을 돌리되, 추가로 찾는건  ░░  인 칸에만 적용
void find_mine_in_range(char ***map, int range[], int mine_cor[][2]) {
    int r1 = range[0];
    int r2 = range[1];
    int c1 = range[2];
    int c2 = range[3];
    for (int i = r1; i <= r2; i++) {
        for (int j = c1; j <= c2; j++) {
            if (strcmp(map[i][j], " ░░ ") == 0) {
                find_mine(map, mine_cor, i, j);
            }
        }
    }
}