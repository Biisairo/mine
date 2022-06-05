#include <stdio.h>
#include <stdlib.h>

int value_in_array(int *array, int num);
void create_map(char ***map);
void print_map(char ***map);
void free_map(char ***map);
int *create_mine();



int main() {
    char ***map = malloc(sizeof(char **) * 10);
    for (int i = 0; i < 10; i++) {
        map[i] = malloc(sizeof(char *) * 15);
    }
    create_map(map);
    print_map(map);
    int *mine = create_mine();
    int mine_cor[20][2];
    for (int i = 0; i < 20; i++) {
        int r = mine[i] / 15;
        int c = mine[i] % 15;
        mine_cor[i][0] = r;
        mine_cor[i][1] = c;
    }
    free(mine);
    for (int i = 0; i < 20; i++) {
        printf("%d, %d\n", mine_cor[i][0], mine_cor[i][1]);
    }

    int dot[2] = {5, 7};










    free_map(map);
    return 0;
}


int value_in_array(int *array, int num) {
    for (int i = 0; i < 20; i++) {
        if (array[i] == num) return 1;
    }
    return 0;
}

void create_map(char ***map) {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 15; j++) {
            map[i][j] = " ░░ ";
        }
    }
}

void print_map(char ***map) {
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

void free_map(char ***map) {
    for (int i = 0; i < 10; i++) {
        free(map[i]);
    }
    free(map);
}

int *create_mine() {
    int *mine = malloc(sizeof(int));
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