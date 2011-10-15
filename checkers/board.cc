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

__u32 LEFT_COL   = 0x04040404;
__u32 RIGHT_COL  = 0x02020202;
__u32 TOP_ROW    = 0x82000820;
__u32 BOTTOM_ROW = 0x00041041;

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
		int mask = l[i+1];
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

#define jump_ind_ul(i) (i+14)%32
#define jump_ind_ur(i) (i+2)%32
#define jump_ind_dl(i) (i<2?i+30:i-2)
#define jump_ind_dr(i) (i<14?i+18:i-14)

#define ind_ul(i) (i+7)%32
#define ind_ur(i) (i+1)%32
#define ind_dl(i) (i==0?31:i-1)
#define ind_dr(i) (i<7?i+25:i-7)

#define try_jump(invalid_move, shift, f) \
	{ \
		__u32 mask = shift(piece); \
		__u32 valid = (p == RED_PLAYER ? black : red) & mask & ~(invalid_move); \
		if (valid) { \
			__u32 land = shift(valid); \
			if (land & empty & ~(invalid_move)) { \
				added_move = true; \
				bool more_jumps; \
				if (p == RED_PLAYER) \
					more_jumps = jumps(moves, stack, f(ind), land, p, red^piece, black^valid, king); \
				else \
					more_jumps = jumps(moves, stack, f(ind), land, p, red^valid, black^piece, king); \
				if (!more_jumps) { \
					Move m; \
					vector<int>::iterator it; \
					for ( it=stack->begin() ; it < stack->end(); it++ ) \
						m.addTile(*it); \
					m.addTile(loc[f(ind)]); \
					moves->push_back(m); \
				} \
			} \
		} \
	}

bool jumps(vector<Move> *moves, vector<int> *stack, int ind, __u32 piece, Player p, __u32 red, __u32 black, bool king) {
	bool added_move = false;
	__u32 empty = ~(red | black);
	stack->push_back(loc[ind]);

	if (p == BLACK_PLAYER || king) {
		try_jump((BOTTOM_ROW | RIGHT_COL), shift_ul, jump_ind_ul);// ↖
		try_jump((BOTTOM_ROW | LEFT_COL), shift_ur, jump_ind_ur);// ↗
	}

	if (p == RED_PLAYER || king) {
		try_jump((TOP_ROW | LEFT_COL), shift_dr, jump_ind_dr);// ↘
		try_jump((TOP_ROW | RIGHT_COL), shift_dl, jump_ind_dl);// ↙
	}

	stack->pop_back();
	return added_move;
}

void Board::add_jump_moves(vector<Move> *moves, Player p) {
	__u32 t = 1;
	__u32 pieces = p == RED_PLAYER ? red : black;
	vector<int> stack;
	for (int i=0; pieces; i++) {
		if (t & pieces) {
			jumps(moves, &stack, i, t, p, red, black, kings&t);
			pieces ^= t;
		}
		t <<= 1;
	}
}

#define easy(invalid_move, shift, f) \
	{ \
		__u32 mask = shift(pieces); \
		__u32 valid = empty & mask & ~(invalid_move); \
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

void Board::add_normal_moves(vector<Move> *moves, Player p) {
	__u32 empty = ~(red | black);

	__u32 pieces = p == RED_PLAYER ? red & kings : black;
	if (pieces) {
		easy(BOTTOM_ROW | RIGHT_COL, shift_ul, ind_dr);// ↖
		easy(BOTTOM_ROW | LEFT_COL, shift_ur, ind_dl);// ↗
	}

	pieces = p == RED_PLAYER ? red : black & kings;
	if (pieces) {
		easy(TOP_ROW | LEFT_COL, shift_dr, ind_ul);// ↘
		easy(TOP_ROW | RIGHT_COL, shift_dl, ind_ur);// ↙
	}
}

vector<Move> *Board::generate_moves(Player p) {
	vector<Move> *moves = new vector<Move>();
	add_jump_moves(moves, p);
	if (moves->size() == 0)
		add_normal_moves(moves, p);
	return moves;
}

