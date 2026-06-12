#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

/* ─── Macros ───────────────────────────────────────────────── */
#define W           21          /* playfield width              */
#define H           15          /* playfield height             */
#define G_CNT       3           /* number of ghosts             */
#define MAX_PLAYERS 10          /* scoreboard capacity          */
#define MAX_CHAR    32          /* max username / password len  */

/* ─── Global variables ─────────────────────────────────────── */
char playfield[H][W];

int  *px, *py;          /* Pac-Man position (heap)      */
int  *gx, *gy;          /* Ghost positions  (heap)      */
int  *score, *h_score;  /* Scores           (heap)      */
int  *final_lives;      /* Remaining lives  (heap)      */
int  *flag;             /* Game-state flag  (heap)      */

int  lives = 3;
char n[MAX_CHAR];       /* logged-in player name        */

/* ─── Original maze template ───────────────────────────────── */
const char maze_template[H][W + 1] = {
    "####################",
    "#........#.........#",
    "#.##.###.#.###.##.##",
    "#..................#",
    "#.##.#.#####.#.##.##",
    "#....#...#...#.....#",
    "####.### # ###.####",
    "         #         ",
    "####.### # ###.####",
    "#....#...#...#.....#",
    "#.##.#.#####.#.##.##",
    "#..................#",
    "#.##.###.#.###.##.##",
    "#........#.........#",
    "####################"
};

/* ═══════════════════════════════════════════════════════════════
   UTILITY / CONSOLE HELPERS
   ═══════════════════════════════════════════════════════════════ */

void hide_cursor(void) {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO ci = {1, FALSE};
    SetConsoleCursorInfo(h, &ci);
}

void gotoxy(int x, int y) {
    COORD c = {(SHORT)x, (SHORT)y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void set_color(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void reset_color(void) { set_color(7); }

void clear_screen(void) { system("cls"); }

/* ═══════════════════════════════════════════════════════════════
   USER MANAGEMENT
   ═══════════════════════════════════════════════════════════════ */

void signup(void) {
    clear_screen();
    char uname[MAX_CHAR], pass[MAX_CHAR];
    char ch;
    int  i = 0;

    set_color(14);
    printf("\n\n\t\t╔══════════════════════════════╗\n");
    printf("\t\t║         SIGN  UP             ║\n");
    printf("\t\t╚══════════════════════════════╝\n\n");
    reset_color();

    printf("\t\t Enter Username : ");
    scanf("%s", uname);

    /* password masking */
    printf("\t\t Enter Password : ");
    i = 0;
    while (1) {
        ch = _getch();
        if (ch == '\r') break;
        if (ch == '\b' && i > 0) { i--; printf("\b \b"); continue; }
        if (ch != '\b') { pass[i++] = ch; printf("*"); }
    }
    pass[i] = '\0';
    printf("\n");

    FILE *f = fopen("users.txt", "a");
    if (!f) { printf("\t\t[ERROR] Cannot open users.txt\n"); return; }
    fprintf(f, "%s %s\n", uname, pass);
    fclose(f);

    set_color(10);
    printf("\n\t\t Account created successfully!\n");
    reset_color();
    Sleep(1200);
}

int login(void) {
    clear_screen();
    char uname[MAX_CHAR], pass[MAX_CHAR];
    char fu[MAX_CHAR], fp[MAX_CHAR];
    char ch;
    int  i = 0;

    set_color(14);
    printf("\n\n\t\t╔══════════════════════════════╗\n");
    printf("\t\t║           LOG IN             ║\n");
    printf("\t\t╚══════════════════════════════╝\n\n");
    reset_color();

    printf("\t\t Enter Username : ");
    scanf("%s", uname);

    printf("\t\t Enter Password : ");
    i = 0;
    while (1) {
        ch = _getch();
        if (ch == '\r') break;
        if (ch == '\b' && i > 0) { i--; printf("\b \b"); continue; }
        if (ch != '\b') { pass[i++] = ch; printf("*"); }
    }
    pass[i] = '\0';
    printf("\n");

    FILE *f = fopen("users.txt", "r");
    if (!f) { printf("\t\t[ERROR] No accounts found. Please sign up first.\n"); Sleep(1500); return 0; }

    while (fscanf(f, "%s %s", fu, fp) == 2) {
        if (strcmp(fu, uname) == 0 && strcmp(fp, pass) == 0) {
            fclose(f);
            strcpy(n, uname);
            set_color(10);
            printf("\n\t\t Login successful! Welcome, %s\n", n);
            reset_color();
            Sleep(1200);
            return 1;
        }
    }
    fclose(f);
    set_color(12);
    printf("\n\t\t Invalid credentials. Please try again.\n");
    reset_color();
    Sleep(1500);
    return 0;
}

/* ═══════════════════════════════════════════════════════════════
   SCOREBOARD
   ═══════════════════════════════════════════════════════════════ */

void scoreboard(void) {
    clear_screen();

    typedef struct { char name[MAX_CHAR]; int sc; } Entry;
    Entry entries[MAX_PLAYERS];
    int   cnt = 0;

    FILE *f = fopen("scoreboard.txt", "r");
    if (f) {
        while (cnt < MAX_PLAYERS && fscanf(f, "%s %d", entries[cnt].name, &entries[cnt].sc) == 2)
            cnt++;
        fclose(f);
    }

    /* bubble sort descending */
    for (int i = 0; i < cnt - 1; i++)
        for (int j = 0; j < cnt - 1 - i; j++)
            if (entries[j].sc < entries[j + 1].sc) {
                Entry tmp = entries[j]; entries[j] = entries[j + 1]; entries[j + 1] = tmp;
            }

    set_color(14);
    printf("\n\n\t\t╔═══════════════════════════════════╗\n");
    printf("\t\t║           SCOREBOARD              ║\n");
    printf("\t\t╠═════╦══════════════╦══════════════╣\n");
    printf("\t\t║ Rk  ║    Player    ║    Score     ║\n");
    printf("\t\t╠═════╬══════════════╬══════════════╣\n");
    reset_color();

    if (cnt == 0) {
        printf("\t\t║     No scores recorded yet.       ║\n");
    } else {
        for (int i = 0; i < cnt; i++) {
            int col = (i == 0) ? 14 : (i == 1) ? 7 : (i == 2) ? 6 : 8;
            set_color(col);
            printf("\t\t║ %-3d ║ %-12s ║ %-12d ║\n", i + 1, entries[i].name, entries[i].sc);
            reset_color();
        }
    }

    set_color(14);
    printf("\t\t╚═════╩══════════════╩══════════════╝\n");
    reset_color();
    printf("\n\t\t Press any key to return...");
    _getch();
}

/* ═══════════════════════════════════════════════════════════════
   GAMEPLAY
   ═══════════════════════════════════════════════════════════════ */

void init_playfield(void) {
    for (int r = 0; r < H; r++)
        for (int c = 0; c < W; c++)
            playfield[r][c] = maze_template[r][c];
}

int allowed_move(int x, int y) {
    if (x < 0 || x >= W || y < 0 || y >= H) return 0;
    return (playfield[y][x] != '#');
}

void render_map(void) {
    gotoxy(0, 0);

    /* title bar */
    set_color(14);
    printf(" ╔══════════════════════════════════════╗\n");
    printf(" ║      THE MAZE-RUNNER : PAC-MAN       ║\n");
    printf(" ╚══════════════════════════════════════╝\n");

    /* maze */
    for (int r = 0; r < H; r++) {
        printf(" ");
        for (int c = 0; c < W; c++) {
            char ch = playfield[r][c];
            if      (ch == '#') { set_color(9);  printf("#"); }
            else if (ch == 'P') { set_color(14); printf("C"); }   /* Pac-Man = C (mouth) */
            else if (ch == 'G') { set_color(12); printf("G"); }
            else if (ch == '.') { set_color(8);  printf("."); }
            else                { reset_color(); printf(" "); }
        }
        printf("\n");
    }
    reset_color();

    /* HUD */
    set_color(10);
    printf("\n Score  : %-6d", *score);
    set_color(14);
    printf("   High : %-6d", *h_score);
    set_color(12);
    printf("   Lives : ");
    for (int i = 0; i < *final_lives; i++) printf("♥ ");
    printf("   \n");
    reset_color();
    printf(" Controls: W A S D = Move   Q = Quit\n");
}

void update_pacman_position(int nx, int ny) {
    if (!allowed_move(nx, ny)) return;

    /* pellet collection */
    if (playfield[ny][nx] == '.') (*score)++;

    playfield[*py][*px] = ' ';
    *px = nx; *py = ny;
    playfield[*py][*px] = 'P';
}

void update_ghost_position(void) {
    for (int i = 0; i < G_CNT; i++) {
        int dx = (*px > gx[i]) ? 1 : (*px < gx[i]) ? -1 : 0;
        int dy = (*py > gy[i]) ? 1 : (*py < gy[i]) ? -1 : 0;

        int nx = gx[i], ny = gy[i];

        /* prefer axis with greater distance */
        int abs_dx = abs(*px - gx[i]);
        int abs_dy = abs(*py - gy[i]);

        if (abs_dx >= abs_dy) {
            if (allowed_move(gx[i] + dx, gy[i]))      nx = gx[i] + dx;
            else if (allowed_move(gx[i], gy[i] + dy)) ny = gy[i] + dy;
        } else {
            if (allowed_move(gx[i], gy[i] + dy))      ny = gy[i] + dy;
            else if (allowed_move(gx[i] + dx, gy[i])) nx = gx[i] + dx;
        }

        if (nx == gx[i] && ny == gy[i]) {
            /* stuck – try random cardinal */
            int dirs[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};
            int r = rand() % 4;
            if (allowed_move(gx[i] + dirs[r][0], gy[i] + dirs[r][1])) {
                nx = gx[i] + dirs[r][0];
                ny = gy[i] + dirs[r][1];
            }
        }

        /* only move if not occupied by another ghost */
        bool blocked = false;
        for (int j = 0; j < G_CNT; j++)
            if (j != i && gx[j] == nx && gy[j] == ny) { blocked = true; break; }

        if (!blocked && playfield[ny][nx] != '#') {
            if (playfield[gy[i]][gx[i]] == 'G') playfield[gy[i]][gx[i]] = ' ';
            gx[i] = nx; gy[i] = ny;
            playfield[gy[i]][gx[i]] = 'G';
        }
    }
}

int col_check(void) {
    for (int i = 0; i < G_CNT; i++)
        if (gx[i] == *px && gy[i] == *py) return 1;
    return 0;
}

int gameover_check(void) { return (*final_lives <= 0); }

void input_handling(char *move) {
    if (!_kbhit()) { *move = 0; return; }
    *move = (char)_getch();
}

void place_ghosts(void) {
    for (int i = 0; i < G_CNT; i++) {
        int rx, ry;
        do {
            rx = rand() % (W - 2) + 1;
            ry = rand() % (H - 2) + 1;
        } while (playfield[ry][rx] != ' ' && playfield[ry][rx] != '.' );
        /* ensure not on player spawn */
        if (rx == *px && ry == *py) { i--; continue; }
        gx[i] = rx; gy[i] = ry;
        playfield[ry][rx] = 'G';
    }
}

void reset_round(void) {
    init_playfield();
    *px = 1; *py = 1;
    playfield[*py][*px] = 'P';
    place_ghosts();
}

void save_score(void) {
    FILE *f = fopen("scoreboard.txt", "a");
    if (f) { fprintf(f, "%s %d\n", n, *score); fclose(f); }

    /* high score */
    FILE *fh = fopen("highscore.txt", "r");
    if (fh) { fscanf(fh, "%d", h_score); fclose(fh); }
    if (*score > *h_score) {
        *h_score = *score;
        fh = fopen("highscore.txt", "w");
        if (fh) { fprintf(fh, "%d\n", *h_score); fclose(fh); }
    }
}

/* count remaining pellets */
int pellets_left(void) {
    int cnt = 0;
    for (int r = 0; r < H; r++)
        for (int c = 0; c < W; c++)
            if (playfield[r][c] == '.') cnt++;
    return cnt;
}

void game(void) {
    *score      = 0;
    *final_lives = lives;
    *flag       = 1;

    /* read persisted high score */
    FILE *fh = fopen("highscore.txt", "r");
    if (fh) { fscanf(fh, "%d", h_score); fclose(fh); }

    reset_round();
    hide_cursor();
    clear_screen();

    int  ghost_tick  = 0;
    int  ghost_delay = 4;   /* ghost moves every N player moves */

    while (*flag) {
        render_map();

        char move = 0;
        /* wait for key */
        while (!_kbhit()) { Sleep(30); }
        input_handling(&move);

        int nx = *px, ny = *py;
        switch (move) {
            case 'w': case 'W': ny--; break;
            case 's': case 'S': ny++; break;
            case 'a': case 'A': nx--; break;
            case 'd': case 'D': nx++; break;
            case 'q': case 'Q': *flag = 0; continue;
        }

        update_pacman_position(nx, ny);

        /* ghosts move every ghost_delay turns */
        ghost_tick++;
        if (ghost_tick >= ghost_delay) {
            update_ghost_position();
            ghost_tick = 0;
        }

        /* collision */
        if (col_check()) {
            (*final_lives)--;
            if (gameover_check()) { *flag = 0; break; }
            /* restart round, keep score & lives */
            clear_screen();
            set_color(12);
            printf("\n\n\t\t  You lost a life! Lives left: ");
            for (int i = 0; i < *final_lives; i++) printf("♥ ");
            printf("\n\t\t  Restarting round...\n");
            reset_color();
            Sleep(1500);
            reset_round();
            clear_screen();
        }

        /* win condition: all pellets collected */
        if (pellets_left() == 0) {
            clear_screen();
            set_color(10);
            printf("\n\n\t\t ★  YOU CLEARED THE MAZE!  ★\n");
            printf("\t\t    Score : %d\n", *score);
            reset_color();
            Sleep(2000);
            *flag = 0;
        }
    }

    /* game over screen */
    save_score();
    clear_screen();
    set_color(12);
    printf("\n\n\t\t╔══════════════════════════════╗\n");
    printf("\t\t║         GAME  OVER           ║\n");
    printf("\t\t╚══════════════════════════════╝\n\n");
    reset_color();
    printf("\t\t Player    : %s\n", n);
    set_color(10);
    printf("\t\t Your Score: %d\n", *score);
    set_color(14);
    printf("\t\t High Score: %d\n", *h_score);
    reset_color();
    printf("\n\t\t Press any key to return to menu...");
    _getch();
}

/* ═══════════════════════════════════════════════════════════════
   MENUS
   ═══════════════════════════════════════════════════════════════ */

void print_logo(void) {
    set_color(14);
    printf("\n");
    printf("\t  ████████╗██╗  ██╗███████╗\n");
    printf("\t     ██╔══╝██║  ██║██╔════╝\n");
    printf("\t     ██║   ███████║█████╗  \n");
    printf("\t     ██║   ██╔══██║██╔══╝  \n");
    printf("\t     ██║   ██║  ██║███████╗\n");
    printf("\t     ╚═╝   ╚═╝  ╚═╝╚══════╝\n");
    set_color(9);
    printf("\t  MAZE-RUNNER : PAC-MAN\n");
    set_color(8);
    printf("\t  CSE-1102 · KUET · 2023\n\n");
    reset_color();
}

void menu(void) {
    while (1) {
        clear_screen();
        print_logo();
        set_color(10);
        printf("\t  Welcome, %s!\n\n", n);
        reset_color();
        printf("\t  ┌──────────────────────┐\n");
        printf("\t  │  1. Start Game       │\n");
        printf("\t  │  2. Scoreboard       │\n");
        printf("\t  │  3. Logout           │\n");
        printf("\t  │  4. Exit             │\n");
        printf("\t  └──────────────────────┘\n");
        printf("\n\t  Select option: ");

        int choice;
        scanf("%d", &choice);
        switch (choice) {
            case 1: game();      break;
            case 2: scoreboard(); break;
            case 3: return;
            case 4: exit(0);
        }
    }
}

void display(void) {
    while (1) {
        clear_screen();
        print_logo();
        printf("\t  ┌──────────────────────┐\n");
        printf("\t  │  1. Login            │\n");
        printf("\t  │  2. Sign Up          │\n");
        printf("\t  │  3. Exit             │\n");
        printf("\t  └──────────────────────┘\n");
        printf("\n\t  Select option: ");

        int choice;
        scanf("%d", &choice);
        switch (choice) {
            case 1: if (login())  menu(); break;
            case 2: signup();             break;
            case 3: return;
        }
    }
}

/* ═══════════════════════════════════════════════════════════════
   MAIN
   ═══════════════════════════════════════════════════════════════ */

int main(void) {
    srand((unsigned)time(NULL));

    /* dynamic memory allocation */
    px          = (int *)malloc(sizeof(int));
    py          = (int *)malloc(sizeof(int));
    gx          = (int *)malloc(G_CNT * sizeof(int));
    gy          = (int *)malloc(G_CNT * sizeof(int));
    score       = (int *)malloc(sizeof(int));
    h_score     = (int *)malloc(sizeof(int));
    final_lives = (int *)malloc(sizeof(int));
    flag        = (int *)malloc(sizeof(int));

    if (!px || !py || !gx || !gy || !score || !h_score || !final_lives || !flag) {
        fprintf(stderr, "Memory allocation failed.\n");
        return 1;
    }

    *score = 0; *h_score = 0; *final_lives = lives; *flag = 0;

    display();

    /* free allocated memory */
    free(px); free(py); free(gx); free(gy);
    free(score); free(h_score); free(final_lives); free(flag);

    return 0;
}
