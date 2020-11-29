#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<limits.h>

void boardPrint(int **game , int N);
bool gameEndCheck(int** game , int N);
int score(int** game, int depth , int N);
bool playerWonCheck(int** game, int player , int N);
bool terminalStateCheck(int** game , int N);
void playbestMove(int** game, int player , int N);
typedef struct result{
int r_move;
int c_move;
int score;
} Result;
Result* minimizersTurn(int** game, int depth, int alpha, int beta, int N);
Result* maximizersTurn(int** game, int depth, int alpha, int beta, int N);
static int X = 1;   
static int O = -1;  
static int E = 0;   
int main(){
    int N;
    printf("Enter 1 for 3x3 and 2 for 4x4 : \n");
    scanf("%d",&N);
    N=N+2;
    int** game=(int**)malloc(sizeof(int*)* N);   
    for(int i=0;i<N;i++){
        game[i]=(int*)malloc(sizeof(int)*N); 
        for(int j=0;j<N;j++){
            game[i][j]=0;
        }
        }
    int currentPlayer=X;
    int row,col;
    printf("You are player with id = X\n");
    printf("example :to choose the first block enter 1,1\n");
while (true) {
    boardPrint(game,N);
    if (currentPlayer == X) {
    printf("It is your turn. Enter row and column  :");
    scanf("%d %d",&row,&col);
        row=row - 1;
        col=col - 1;
        if (game[row][col] == E) {  
            game[row][col]=X;
            currentPlayer=O;
} 
else {
        printf("\nEntered row and column is already filled\n");
    }
} 
else {    
    printf("It is O's turn.\n");
    playbestMove(game, O, N);
    currentPlayer=X;
    }
if (terminalStateCheck(game, N)) {
    break;
}}


if (playerWonCheck(game, X, N)) {
    printf("You have won!\n");
} 
else if (playerWonCheck(game, O, N)) {
    printf("Player O has won!\n");
} 
else {
    printf("It is a draw!\n");
}
boardPrint(game,N);
}
void boardPrint(int **game , int N){
printf("Board State:\n");
for (int i = 0; i < N ; i++) {
for (int j = 0; j < N; j++) {
if (game[i][j] == X) {
printf("X ");
} 
else if (game[i][j] == O) {
printf("O ");} 
else {
printf("_ ");
}}
printf("\n");
}}
bool gameEndCheck(int** game , int N) {
for (int i = 0; i < N; i++) {
for (int j = 0; j < N; j++) {
if (game[i][j] == E) {
return false;
}}}
return true;
}
int score(int** game, int depth , int N) {
if (playerWonCheck(game, X , N)) {
return 10 - depth;
} 
else if (playerWonCheck(game, O , N)) {
return depth - 10;
}
return 0;
}
bool playerWonCheck(int** game, int player , int N) {
for (int i = 0; i < N; ++i) {
bool isRowEqual = true, isColumnEqual = true;
for (int j = 0; j < N; ++j) {
if (game[i][j] != player) {
isRowEqual = false;
}
if (game[j][i] != player) {
isColumnEqual = false;
}}
if (isRowEqual || isColumnEqual) {
return true;
}}
bool isDiagonalEqual = true, isAntiDiagonalEqual = true;
for (int i = 0; i < N; ++i) {
if (game[i][i] != player) {
isDiagonalEqual = false;
}
if (game[N - 1 - i][i] != player) {
isAntiDiagonalEqual = false;
}
}
return isDiagonalEqual || isAntiDiagonalEqual;
}
bool terminalStateCheck(int** game , int N) {
return playerWonCheck(game, X, N) || playerWonCheck(game, O , N) || gameEndCheck(game,N);
}
void playbestMove(int** game, int player , int N) {
Result *result;
Result* currentMove;
int alpha=INT_MIN;
int beta=INT_MAX;
Result *min=(Result*)malloc(sizeof(Result));
min->score=INT_MAX;
min->r_move=-1;
min->c_move=-1;
int c=0;
int depth=0;
if (player == X) {
result=maximizersTurn(game, 0, INT_MIN, INT_MAX, N);
}
else {
if (terminalStateCheck(game,N)) {
Result *res=(Result*)malloc(sizeof(Result));
res->score=score(game,depth,N);
res->r_move=-1;
res->c_move=-1;
result=res;
}
else
{
#pragma omp parallel private(currentMove)
{
#pragma omp for schedule (dynamic)
for (int i=0; i < N; ++i) {
if(c==0)
{
for (int j=0; j < N; ++j) {
#pragma omp critical
{
if (game[i][j] == E) {
game[i][j]=O;
currentMove=maximizersTurn(game, depth + 1, alpha, beta, N);
game[i][j]=E; 
if (currentMove->score < min->score) {
min->score=currentMove->score;
min->r_move=i;
min->c_move=j;
}
if (currentMove->score <= alpha) {
c=1;                        
}
if(c==0)
if (currentMove->score < beta) {
beta=currentMove->score;}
}}}}}
result=min;
}}
if(result->c_move != -1 && result->r_move != -1){
game[result->r_move][result->c_move]=player;
}
else{
return;
}}}
Result* minimizersTurn(int** game, int depth, int alpha, int beta, int N) {
if (terminalStateCheck(game,N)) {
Result *res = (Result*)malloc(sizeof(Result));
res->score = score(game,depth,N);
res->r_move = -1;
res->c_move = -1;
return res;
}
Result* currentMove;
Result *min = (Result*)malloc(sizeof(Result));
min->score = INT_MAX;
min->r_move = -1;
min->c_move = -1;
for (int i = 0; i < N; ++i) {
for (int j = 0; j < N; ++j) {
if (game[i][j] == E) {
game[i][j] = O;
currentMove = maximizersTurn(game, depth + 1, alpha, beta,N);
game[i][j] = E; 
if (currentMove->score < min->score) {
min->score = currentMove->score;
min->r_move = i;
min->c_move = j;
}
if (currentMove->score <= alpha) {
return min;
}
if (currentMove->score < beta) {
beta = currentMove->score;
}}}}
return min;
}
Result* maximizersTurn(int** game, int depth, int alpha, int beta, int N) {
if (terminalStateCheck(game,N)) {
Result *res = (Result*)malloc(sizeof(Result));
res->score = score(game,depth,N);
res->r_move = -1;
res->c_move = -1;
return res;
}
Result* currentMove;
Result *max = (Result*)malloc(sizeof(Result));
max->score = INT_MIN;
max->r_move = -1;
max->c_move = -1;
for (int i = 0; i < N; ++i) {
for (int j = 0; j < N; ++j) {
if (game[i][j] == E) {
game[i][j] = X;
currentMove = minimizersTurn(game, depth + 1, alpha, beta,N);
game[i][j] = E; 
if (currentMove->score > max->score) {
max->score = currentMove->score;
max->r_move = i;
max->c_move = j;
}
if (currentMove->score >= beta) {
return max;
}
if (currentMove->score > alpha) {
alpha = currentMove->score;
}}}}
return max;
}