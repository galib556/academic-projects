#include "functions.h"

int main(){
    srand(time(0));
    px=(int*)malloc(sizeof(int));
    py=(int*)malloc(sizeof(int));
    gx=(int*)malloc(G_CNT*sizeof(int));
    gy=(int*)malloc(G_CNT*sizeof(int));
    score=(int*)malloc(sizeof(int));
    h_score=(int*)malloc(sizeof(int));
    final_lives=(int*)malloc(sizeof(int));
    flag=(int*)malloc(sizeof(int));
    *score=0;
    *final_lives=lives;
    display();
    free(px);
    free(py);
    free(gx);
    free(gy);
    free(score);
    free(h_score);
    free(final_lives);
    free(flag);
    return 0;
}
