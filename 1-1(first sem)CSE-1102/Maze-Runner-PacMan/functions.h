#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <ctype.h>

#define W 60
#define H 30
#define G_CNT 4
#define MAX_PLAYERS 1000
#define MAX_CHAR 100

extern char playfield[H][W];
extern char n[MAX_CHAR];
extern int *px, *py;
extern int *gx, *gy;
extern int *score, *h_score;
extern const int lives;
extern int *final_lives;
extern int *flag;

void display();
void menu();
void signup();
int login();
void game();
void scoreboard();
void score_input_to_scoreboard();
void rendermap();
void updatepacmanposition(int nx, int ny);
int allowedmove(int x, int y);
void updateghostposition();
void col_check();
void inputhandling();
int gameover();

#endif // FUNCTIONS_H
