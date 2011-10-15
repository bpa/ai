#include <asm/types.h>
#include <iostream>
#include <strings.h>
#include "board.h"

using namespace std;

static inline __u32 rol(__u32 word, unsigned int shift)
{
    return (word << shift) | (word >> (32 - shift));
}

static inline __u32 ror(__u32 word, unsigned int shift)
{
    return (word >> shift) | (word << (32 - shift));
}

static __u32 LEFT_COL   = 0x04040404;
static __u32 RIGHT_COL  = 0x02020202;
static __u32 TOP_ROW    = 0x82000820;
static __u32 BOTTOM_ROW = 0x00041041;

int l[] = { 0x0, 0x800, 0x20, 0x80000000, 0x2000000, 0x400, 0x10, 0x40000000,
			0x1000000, 0x8, 0x20000000, 0x800000, 0x20000, 0x4, 0x10000000,
			0x400000, 0x10000, 0x8000000, 0x200000, 0x8000, 0x200, 0x4000000,
			0x100000, 0x4000, 0x100, 0x80000, 0x2000, 0x80, 0x2, 0x40000, 0x1000, 0x40, 0x1
		  };
int loc[] = {32, 28, 13, 9, 6, 2, 31, 27, 24, 20, 5, 1, 30, 26, 23, 19, 16, 12, 29, 25, 22, 18, 15, 11, 8, 4, 21, 17, 14, 10, 7, 3};

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
		if      (*b == '.') { i++;                               }
		else if (*b == 'r') { i++;  red  |= l[i];                }
		else if (*b == 'R') { i++;  red  |= l[i]; kings |= l[i]; }
		else if (*b == 'b') { i++; black |= l[i];                }
		else if (*b == 'B') { i++; black |= l[i]; kings |= l[i]; }
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
	cout << endl;
}

static void x(__u32 b) {
	for (int i=0; i<32; i++) {
		int mask = l[i];
		if (i / 4 % 2 == 0) cout << ' ';
		cout << (mask & b ? 'X' : '.');
		if (i / 4 % 2 != 0) cout << ' ';
		if (i % 4 == 3) cout << endl;
	}
	cout << endl;
}

#define shift_ul(mask) rol(mask, 7)
#define shift_ur(mask) rol(mask, 1)
#define shift_dl(mask) ror(mask, 1)
#define shift_dr(mask) ror(mask, 7)

#define ind_ul(i) (i+7)%32
#define ind_ur(i) (i+1)%32
#define ind_dl(i) (i==0?31:i-1)
#define ind_dr(i) (i<7?i+25:i-7)

#define easy(moves, empty, pieces, invalid_moves, shift, f) \
	if (pieces) { \
		__u32 mask = shift(pieces); \
		__u32 valid = empty & mask & ~invalid_moves; \
		if (valid) { \
			__u32 t = 1; \
			for (int i=0; valid; i++) { \
				if (t & valid) { \
					Move m; \
					m.addTile(loc[f(i)]); \
					m.addTile(loc[i]); \
					moves->push_back(m); \
					valid ^= t; \
				} \
				t <<= 1; \
			} \
		} \
	}

vector<Move> *Board::generate_moves(Player p) {
	vector<Move> *moves = new vector<Move>();
	__u32 empty = ~(red | black);

	__u32 pieces = p == RED_PLAYER ? red & kings : black;
	easy(moves, empty, pieces, (BOTTOM_ROW | RIGHT_COL), shift_ul, ind_dr);// ↖
	easy(moves, empty, pieces, (BOTTOM_ROW | LEFT_COL), shift_ur, ind_dl);// ↗

	pieces = p == RED_PLAYER ? red : black & kings;
	easy(moves, empty, pieces, (TOP_ROW | LEFT_COL), shift_dr, ind_ul);// ↘
	easy(moves, empty, pieces, (TOP_ROW | RIGHT_COL), shift_dl, ind_ur);// ↙

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
