#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "state.h"

State *state_init(const Board *board) {
	State *state = (State*)malloc(sizeof(State));
	bzero(state, sizeof(State));
	if (board == NULL)
		board_setup(&state->board);
	else
		memcpy(&state->board, board, sizeof(Board));
	state->hash  = board_hash(&state->board);
	state->value = board_evaluate(&state->board);
	return state;
}

#define Clear 0x1f
void CopyState(char *dest, char src)
{
    char state;
    
    *dest &= Clear;
    state = src & 0xE0;
    *dest |= state;
}

void NumberToXY(char num, int *x, int *y)
{
    int i=0,newy,newx;

    for(newy=0; newy<8; newy++)
    for(newx=0; newx<8; newx++)
    {
        if(newx%2 != newy%2) {
            i++;
            if(i==(int) num) {
                *x = newx;
                *y = newy;
                return;
            }
        }
    }
    *x = 0; 
    *y = 0;
}

#define King 0x60
State *state_transition(State *state, Move *move) {
    int i,j,x,y,x1,y1,x2,y2;
	int mlen = strlen(*move);

    NumberToXY(*move[0],&x,&y);
    NumberToXY(*move[mlen-1],&x1,&y1);
    CopyState(&state->board[y1][x1],state->board[y][x]);
    if(y1 == 0 || y1 == 7) state->board[y1][x1] |= King;
    state->board[y][x] &= Clear;
    NumberToXY(*move[1],&x2,&y2);
    if(abs(x2-x) == 2) {
        for(i=0,j=1; j<mlen; i++,j++) {
            if(*move[i] > *move[j]) {
                y1 = -1; 
                if((*move[i]-*move[j]) == 9) x1 = -1; else x1 = 1;
            }
            else {
                y1 = 1;
                if((*move[j]-*move[i]) == 7) x1 = -1; else x1 = 1;
            }
            NumberToXY(*move[i],&x,&y);
            state->board[y+y1][x+x1] &= Clear;
        }
    }
	return state;
}

MoveList *state_moves(State *state, Player p) {
	if (state->moves[p] == NULL) {
		state->moves[p] = move_list_create(&state->board, p);
	}
	return state->moves[p];
}
