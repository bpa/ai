#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "state.h"

State::State() {
}

State::State(const Board *board) {
	memcpy(&this->board, board, sizeof(Board));
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
State::State(State *state, Move *move) {
    int i,j,x,y,x1,y1,x2,y2;
	int mlen = strlen(move->tiles);

    NumberToXY(move->tiles[0],&x,&y);
    NumberToXY(move->tiles[mlen-1],&x1,&y1);
    //CopyState(&state->board[y1][x1],state->board[y][x]);
    //if(y1 == 0 || y1 == 7) state->board[y1][x1] |= King;
    //state->board[y][x] &= Clear;
    NumberToXY(move->tiles[1],&x2,&y2);
    if(abs(x2-x) == 2) {
        for(i=0,j=1; j<mlen; i++,j++) {
            if(move->tiles[i] > move->tiles[j]) {
                y1 = -1; 
                if((move->tiles[i]-move->tiles[j]) == 9) x1 = -1; else x1 = 1;
            }
            else {
                y1 = 1;
                if((move->tiles[j]-move->tiles[i]) == 7) x1 = -1; else x1 = 1;
            }
            NumberToXY(move->tiles[i],&x,&y);
            //state->board[y+y1][x+x1] &= Clear;
        }
    }
}

vector<Move> *State::moves_for_player(Player p) {
	return &moves[(int)p];
}
