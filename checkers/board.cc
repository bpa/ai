#include <asm/types.h>
#include <iostream>
#include <strings.h>
#include "board.h"

using namespace std;

static inline __u32 rol32(__u32 word, unsigned int shift)
{
    return (word << shift) | (word >> (32 - shift));
}

static inline __u32 ror32(__u32 word, unsigned int shift)
{
    return (word >> shift) | (word << (32 - shift));
}

static __u32 LEFT_COL   = 0x04040404;
static __u32 RIGHT_COL  = 0x02020202;
static __u32 TOP_ROW    = 0x82000820;
static __u32 BOTTOM_ROW = 0x00041041;

int l[] = { 0x800, 0x20, 0x80000000, 0x2000000, 0x400, 0x10, 0x40000000,
			0x1000000, 0x8, 0x20000000, 0x800000, 0x20000, 0x4, 0x10000000,
			0x400000, 0x10000, 0x8000000, 0x200000, 0x8000, 0x200, 0x4000000,
			0x100000, 0x4000, 0x100, 0x80000, 0x2000, 0x80, 0x2, 0x40000, 0x1000, 0x40, 0x1
		  };
int loc[] = {11,5,31,25,10,4,30,24,3,29,23,17,2,28,22,16,27,21,15,9,26,20,14,8,19,13,7,1,18,12,6,0};

Board::Board() {
	red   = 0xE3820C38;
	black = 0x041C71C3;
	kings = 0;
}

Board::Board(const char *board_as_str) {
	int i = 0;
	const char *b = board_as_str;
	red = black = kings = 0;
	while (*b != '\0' && i < 32) {
		if      (*b == '.') {                               i++; }
		else if (*b == 'r') {  red  |= l[i];                i++; }
		else if (*b == 'R') {  red  |= l[i]; kings |= l[i]; i++; }
		else if (*b == 'b') { black |= l[i];                i++; }
		else if (*b == 'B') { black |= l[i]; kings |= l[i]; i++; }
		b++;
	}
}

void Board::say() {
	for (int i=0; i<32; i++) {
		int mask = l[i];
		if (i / 4 % 2 == 0) cout << ' ';
		     if (mask &  red ) cout << (mask & kings ? 'R' : 'r');
		else if (mask & black) cout << (mask & kings ? 'B' : 'b');
		else                   cout << '.';
		if (i / 4 % 2 != 0) cout << ' ';
		if (i % 4 == 3) cout << endl;
	}
}

vector<Move> *Board::generate_moves(Player p) {
	vector<Move> *moves = new vector<Move>();
	__u32 empty = ~(red | black);
	if (p == RED_PLAYER) {
		__u32 mask = ror32(red, 1);
		__u32 valid = mask & empty & ~LEFT_COL;
		if (valid) {
			__u32 t = 1;
			for (int i=0; i<32; i++) {
				if (t & valid) {
					__u32 start = r_ror32(i);
					Move m;
					m.addTile(loc[start]);
					m.addTile(loc[i]);
					moves->push_back(m);
				}
				t <<= 1;
			}
		}
	}
	return moves;
}

/* Finds legal non-jump moves for the King at position x,y */
//void move_list_add_king_moves(Board *board, int x, int y) {
//    int i,j,x1,y1;
//    Move move;
//    memset(move,0,sizeof(Move));
//
//    /* Check the four adjacent squares */
//    for(j=-1; j<2; j+=2)
//    for(i=-1; i<2; i+=2)
//    {
//        y1 = y+j; x1 = x+i;
//        /* Make sure we're not off the edge of the board */
//        if(y1<0 || y1>7 || x1<0 || x1>7) continue; 
//        if(empty(*board[y1][x1])) {  /* The square is empty, so we can move there */
//            move[0] = number(*board[y][x])+1;
//            move[1] = number(*board[y1][x1])+1;    
//            move_list_add(moves, &move);
//        }
//    }
//}

/* Finds legal non-jump moves for the Piece at position x,y */
//void move_list_add_moves(MoveList *moves, Board *board, Player player, int x, int y) 
//{
//    int i,j,x1,y1;
//    Move move;
//
//    memset(move,0,sizeof(Move));
//
//    /* Check the two adjacent squares in the forward direction */
//    if(player == 0) j = 1; else j = -1;
//    for(i=-1; i<2; i+=2)
//    {
//        y1 = y+j; x1 = x+i;
//        /* Make sure we're not off the edge of the board */
//        if(y1<0 || y1>7 || x1<0 || x1>7) continue; 
//        if(empty(*board[y1][x1])) {  /* The square is empty, so we can move there */
//            move[0] = number(*board[y][x])+1;
//            move[1] = number(*board[y1][x1])+1;    
//            move_list_add(moves, &move);
//        }
//    }
//}

/* Determines all of the legal moves possible for a given state */
//void FindLegalMoves(MoveList *moves, Board *b, Player player) {
/*
    int x,y;
    char move[12], board[8][8];

    memset(move,0,sizeof(Move));

    for(y=0; y<8; y++)
    for(x=0; x<8; x++)
    {
        if(x%2 != y%2 && color(board[y][x]) == player && !empty(board[y][x])) {
            if(king(board[y][x])) {
                move[0] = number(*board[y][x])+1;
                FindKingJump(player,board,move,1,x,y);
                if(!jumpptr) move_list_add_king_moves(board,x,y);
            } 
            else if(piece(*board[y][x])) {
                move[0] = number(*board[y][x])+1;
                FindJump(player,board,move,1,x,y);
                if(!jumpptr) move_list_add_moves(moves, board, player, x, y);
            }
        }    
    }
    if(jumpptr) {
        for(x=0; x<jumpptr; x++) 
        for(y=0; y<12; y++) 
        //state->movelist[x][y] = jumplist[x][y];
        //state->numLegalMoves = jumpptr;
    } 
    else {
        for(x=0; x<numLegalMoves; x++) 
        for(y=0; y<12; y++) 
        //state->movelist[x][y] = movelist[x][y];
        //state->numLegalMoves = numLegalMoves;
    }
    return (jumpptr+numLegalMoves);
*/
//}
