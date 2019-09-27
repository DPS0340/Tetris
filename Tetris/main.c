#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>

void gotoxy(int, int);

void print_wall(int, int);

void refresh(int**, int, int);

void check(int**, int, int);

void print_arr(int**, int, int);

void print_block(int**, int**, int, int, int, int, int);

int is_x_collision(int**, int**, int, int, int, int, int);

int is_y_collision(int**, int**, int, int, int, int, int);

int** copy_2d_arr(int**, int, int, int);

void rotate_left(int**, int**, int, int, int, int, int);

void rotate_right(int**, int**, int, int, int, int, int);

void rotate_minus_90(int**, int);

void stick(int**, int**, int, int, int, int, int);

int** make_block(void);

int is_lose(int**, int, int);

int main(void);

int main(void) {
	int x_max, y_max;
	printf("x, y 값을 입력하시오\n");
	scanf("%d %d", &x_max, &y_max);
	system("cls");
	int** arr = (int**)calloc(y_max, sizeof(int*));
	for (int i = 0; i < y_max; i++) {
		arr[i] = (int*)calloc(x_max, sizeof(int));
	}
	int x = 0, y = 0, n = 4;
	int** block = make_block();
	while (1) {
		print_wall(8, 15);
		print_block(arr, block, x, y, x_max, y_max, n);
		print_arr(arr, x_max, y_max);
		if (_kbhit()) {
			int key = getchar();
			if (key == 27) {
				return 0;
			} else if (key == 224 || key == 0) {
				if (_kbhit()) {
					key = getchar();
					if (key == 75) {
						rotate_left(arr, block, x, y, n, x_max, y_max);
					}
					else if (key == 77) {
						rotate_right(arr, block, x, y, n, x_max, y_max);
					}
					else if (key == 80) {
						if (is_y_collision(arr, block, x, y, n, x_max, y_max) == 0) {
							y++;
						}
						else {
							stick(arr, block, x, y, n, x_max, y_max);
							block = make_block();
							x = 0;
							y = 0;
						}
					}
				}
			}
		}
		if (is_y_collision(arr, block, x, y, n, x_max, y_max) == 0) {
			y++;
		}
		else {
			stick(arr, block, x, y, n, x_max, y_max);
			block = make_block();
			x = 0;
			y = 0;
		}
		if (is_lose(arr, x_max, y_max)) {
			gotoxy(0, y_max + 2);
			printf("끝!");
			return 0;
		}
		check(arr, x_max, y_max);
		refresh(arr, x_max, y_max);
		system("cls");
		print_wall(8, 15);
		print_block(arr, block, x, y, x_max, y_max, n);
		print_arr(arr, x_max, y_max);
		Sleep(500);
		system("cls");
	}

	return 0;
}

void print_wall(int x, int y) {
	gotoxy(0, 0);
	for (int i = 0; i < y; i++) {
		printf("■");
		gotoxy(0, i + 1);
	}
	gotoxy(2, y - 1);
	for (int i = 1; i < x; i++) {
		printf("■");
	}
	gotoxy(2 * x, 0);
	for (int i = 0; i < y; i++) {
		printf("■");
		gotoxy(2 * x, i + 1);
	}
}

void refresh(int** arr, int x, int y) {
	for (int i = 0; i < y; i++) {
		for (int j = 0; j < x; j++) {
			if (i >= 1 && arr[i][j] == 1 && arr[i-1][j] == 0) {
				arr[i - 1][j] = 1;
				arr[i][j] = 0;
			}
		}
	}
}

void check(int** arr, int x, int y) {
	for (int i = y - 1; i >= 0; i--) {
		int flag = 1;
		for (int j = 0; j < x; j++) {
			if (arr[i][j] == 0) {
				flag = 0;
			}
		}
		if (flag == 1) {
			for (int j = 0; j < x; j++) {
				arr[i][j] = 0;
			}
		}
	}
}

void print_arr(int** arr, int x, int y) {
	for (int i = 0; i < y; i++) {
		gotoxy(1, i + 1);
		for (int j = 0; j < x; j++) {
			if (arr[i][j] == 1) {
				printf("■");
			}
			else {
				printf("  ");
			}
		}
	}
}

void print_block(int** arr, int** block, int x, int y, int x_max, int y_max, int n) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (block[i][j]) {
				gotoxy(y + i + 1, x + j + 1);
				printf("■");
			}
		}
	}
}

int is_lose(int** arr, int x_max, int y_max) {
	for (int j = 0; j < x_max; j++) {
		if (arr[y_max][j] == 1) {
			return 1;
		}
	}
	return 0;
}

int is_x_collision(int** arr, int** block, int x, int y, int n, int x_max, int y_max) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (block[i][j] == 1 && y + j < y_max && x + i < x_max && arr[y + i][x + j] == block[i][j]) {
				return 1;
			}
		}
	}
	return 0;
}

int is_y_collision(int** arr, int** block, int x, int y, int n, int x_max, int y_max) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (block[i][j] == 1 && y + j < y_max && x + i < x_max && arr[y + j][x + i] == block[i][j]) {
				return 1;
			}
		}
	}
	return 0;
}

void rotate_left(int** arr, int** block, int x, int y, int n, int x_max, int y_max) {
	int** temp = copy_2d_arr(block, n, n, 0);

	rotate_minus_90(temp, n);

	if (!is_y_collision(arr, temp, x, y, n, x_max, y_max) && !is_x_collision(arr, temp, x, y, n, x_max, y_max)) {
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				arr[i][j] = temp[i][j];
			}
		}
	}

	free(temp);
}

void rotate_right(int** arr, int** block, int x, int y, int n, int x_max, int y_max) {
	int** temp = copy_2d_arr(block, n, n, 0);

	for (int i = 0; i < 3; i++) {
		rotate_minus_90(temp, n);
	}

	if (!is_y_collision(arr, temp, x, y, n, x_max, y_max) && !is_x_collision(arr, temp, x, y, n, x_max, y_max)) {
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				arr[i][j] = temp[i][j];
			}
		}
	}

	free(temp);
}

void rotate_minus_90(int** arr, int n) {
	for (int i = 0; i < n / 2; i++) {
		for (int j = i; j < n - i - 1; j++) {
			int temp = arr[i][j];
			arr[i][j] = arr[j][n - 1 - i];
			arr[j][n - 1 - i] = arr[n - 1 - i][n - 1 - j];
			arr[n - 1 - i][n - 1 - j] = arr[n - 1 - j][i];
			arr[n - 1 - j][i] = temp;
		}
	}
}

int** copy_2d_arr(int** arr, int x, int y, int break_arr) {
	int** res = (int**)calloc(y, sizeof(int*));
	for (int i = 0; i < y; i++) {
		res[i] = (int*)calloc(x, sizeof(int));
	}

	for (int i = 0; i < y; i++) {
		for (int j = 0; j < x; j++) {
			res[i][j] = arr[i][j];
		}
	}

	if (break_arr == 1) {
		free(arr);
	}

	return res;
}

void stick(int** arr, int** block, int x, int y, int n, int x_max, int y_max) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (block[i][j] && y + j < y_max && x + i < x_max) {
				arr[x + i][y + j] = block[i][j];
			}
		}
	}
}

int** make_block() {
	int arr[7][4][4] = {
		{
			{
				1, 0, 0, 0
			},

			{
				1, 0, 0, 0
			},

			{
				1, 0, 0, 0
			},

			{
				1, 0, 0, 0
			}
		},
		{
			{
				1, 0, 0, 0
			},

			{
				1, 0, 0, 0
			},

			{
				1, 1, 0, 0
			},

			{
				0, 0, 0, 0
			}
		},
		{
			{
				0, 1, 0, 0
			},

			{
				1, 1, 0, 0
			},

			{
				0, 1, 0, 0
			},

			{
				0, 0, 0, 0
			}
		},
		{
			{
				1, 1, 0, 0
			},

			{
				1, 1, 0, 0
			},

			{
				0, 0, 0, 0
			},

			{
				0, 0, 0, 0
			}
		},
		{
			{
				0, 1, 0, 0
			},

			{
				0, 1, 0, 0
			},

			{
				1, 1, 0, 0
			},

			{
				0, 0, 0, 0
			}
		},
		{
			{
				0, 1, 1, 0
			},

			{
				1, 1, 0, 0
			},

			{
				0, 0, 0, 0
			},

			{
				0, 0, 0, 0
			}
		},
		{
			{
				1, 1, 0, 0
			},

			{
				0, 1, 1, 0
			},

			{
				0, 0, 0, 0
			},

			{
				0, 0, 0, 0
			}
		},
	};

	static int count = 0;

	count++;

	int** res = (int**)calloc(4, sizeof(int*));
	for (int i = 0; i < 4; i++) {
		res[i] = (int*)calloc(4, sizeof(int));
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			res[i][j] = arr[count % 7][i][j];
		}
	}


	return res;
}


void gotoxy(int x, int y) {
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}