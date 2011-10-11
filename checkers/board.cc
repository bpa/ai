#include "board.h"

#define Empty 0x00
#define Piece 0x20
#define Red 0x00
#define Black 0x80
void board_setup(Board *board) {
	int x,y;
	char pos;

	pos = 0;
	for(y=0; y<8; y++) {
		for(x=0; x<8; x++) {
			if(x%2 != y%2) {
				*board[y][x] = pos;
				if(y<3 || y>4) *board[y][x] |= Piece; else *board[y][x] |= Empty;
				if(y<3) *board[y][x] |= Red; 
				if(y>4) *board[y][x] |= Black;
				pos++;
			} else *board[y][x] = 0;
		}
	}
}

long int board_hash(Board *board) {
	return 0;
}

int board_evaluate(Board *board) {
	return 1;
}
