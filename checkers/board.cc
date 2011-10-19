#include <asm/types.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <strings.h>
#include "board.h"

using namespace std;

static inline __u32 rol(__u32 word, unsigned int shift) {
    return (word << shift) | (word >> (32 - shift));
}

static inline __u32 ror(__u32 word, unsigned int shift) {
    return (word >> shift) | (word << (32 - shift));
}

static inline int bits(__u32 word) {
	int c;
	for (c = 0; word; c++) word &= word - 1;
	return c;
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

__u32 LEFT_COL   = 0x04040404;
__u32 RIGHT_COL  = 0x02020202;
__u32 TOP_ROW    = 0x82000820;
__u32 BOTTOM_ROW = 0x00041041;

__u32 l[] = { 0x0, 0x800, 0x20, 0x80000000, 0x2000000, 0x400, 0x10, 0x40000000,
			0x1000000, 0x8, 0x20000000, 0x800000, 0x20000, 0x4, 0x10000000,
			0x400000, 0x10000, 0x8000000, 0x200000, 0x8000, 0x200, 0x4000000,
			0x100000, 0x4000, 0x100, 0x80000, 0x2000, 0x80, 0x2, 0x40000, 0x1000, 0x40, 0x1
		  };
int loc[] = {32, 28, 13, 9, 6, 2, 31, 27, 24, 20, 5, 1, 30, 26, 23, 19, 16, 12, 29, 25, 22, 18, 15, 11, 8, 4, 21, 17, 14, 10, 7, 3};

void x(__u32 b) {
	for (int i=0; i<32; i++) {
		int mask = l[i+1];
		if (i / 4 % 2 == 0) cerr << ' ';
		cerr << (mask & b ? 'X' : '.');
		if (i / 4 % 2 != 0) cerr << ' ';
		if (i % 4 == 3) cerr << endl;
	}
	cerr << endl;
}

Board::Board(Player p) {
	player = p;
	red   = 0xE3820C38;
	black = 0x041C71C3;
	kings = 0;
	init();
}

Board::Board(Player p, const char *board_as_str) {
	player = p;
	red = black = kings = 0;
	int i = 0;
	const char *b = board_as_str;
	while (*b != '\0' && i < 32) {
		if      (*b == '.') { i++;                               }
		else if (*b == 'r') { i++;  red  |= l[i];                }
		else if (*b == 'R') { i++;  red  |= l[i]; kings |= l[i]; }
		else if (*b == 'b') { i++; black |= l[i];                }
		else if (*b == 'B') { i++; black |= l[i]; kings |= l[i]; }
		b++;
	}
	init();
}

#define clear_jumped_piece(shift) \
{ \
	__u32 bit = ~shift(mask); \
	kings &= bit; \
	if (parent->player == RED_PLAYER) \
		black &= bit; \
	else \
		red &= bit; \
}

Board::Board(Board *parent, Move *move) {
	player = parent->player == RED_PLAYER ? BLACK_PLAYER : RED_PLAYER;
	red = parent->red;
	black = parent->black;
	kings = parent->kings;
	if (parent->kings & l[(int)move->tiles[0]]) {
		kings ^= l[(int)move->tiles[0]];
		kings ^= l[(int)move->tiles[move->moves-1]];
	}
	if (parent->player == RED_PLAYER) {
		red ^= l[(int)move->tiles[0]];
		red ^= l[(int)move->tiles[move->moves-1]];
		if (l[(int)move->tiles[move->moves-1]] & BOTTOM_ROW)
			kings |= l[(int)move->tiles[move->moves-1]];
	}
	else {
		black ^= l[(int)move->tiles[0]];
		black ^= l[(int)move->tiles[move->moves-1]];
		if (l[(int)move->tiles[move->moves-1]] & TOP_ROW)
			kings |= l[(int)move->tiles[move->moves-1]];
	}
	for (int i=0; i<move->moves-1; i++) {
		int diff = move->tiles[i+1] - move->tiles[i];
		__u32 mask = l[(int)move->tiles[i]];
		     if (diff == -9) clear_jumped_piece(shift_ul)
		else if (diff == -7) clear_jumped_piece(shift_ur)
		else if (diff ==  9) clear_jumped_piece(shift_dr)
		else if (diff ==  7) clear_jumped_piece(shift_dl)
	}
	init();
}

void Board::init() {
	value = bits(red) + bits(red & kings) - bits(black) - bits(black & kings);
	min = max = value;
	refcount = 0;
	parents = NULL;
	children = NULL;
}

Board::~Board() {
	g_list_free(parents);
	g_list_free(children);
}

ostream &operator<<(ostream &out, const Board &b) {
	out << "Player: " << b.player << endl;
	for (int i=0; i<32; i++) {
		int mask = l[i+1];
		if (i / 4 % 2 == 0) out << ' ';
		     if (mask &  b.red ) out << (mask & b.kings ? 'R' : 'r');
		else if (mask & b.black) out << (mask & b.kings ? 'B' : 'b');
		else                   out << '.';
		if (i / 4 % 2 != 0) out << ' ';
		if (i % 4 == 3) out << endl;
	}
	return out;
}

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
					more_jumps = jumps(board, stack, f(ind), land, p, red^piece, black^valid, king); \
				else \
					more_jumps = jumps(board, stack, f(ind), land, p, red^valid, black^piece, king); \
				if (!more_jumps) { \
					Child *c = new Child; \
					vector<int>::iterator it; \
					for ( it=stack->begin() ; it < stack->end(); it++ ) \
						c->move.addTile(*it); \
					c->move.addTile(loc[f(ind)]); \
					c->board = new Board(board, &c->move); \
					board->children = g_list_prepend(board->children, c); \
				} \
			} \
		} \
	}

bool jumps(Board *board, vector<int> *stack, int ind, __u32 piece, Player p, __u32 red, __u32 black, bool king) {
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

void Board::add_jump_moves() {
	__u32 t = 1;
	__u32 pieces = player == RED_PLAYER ? red : black;
	vector<int> stack;
	for (int i=0; pieces; i++) {
		if (t & pieces) {
			jumps(this, &stack, i, t, player, red, black, kings&t);
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
					Child *c = (Child*)malloc(sizeof(Child)); \
					c->move = Move(); \
					c->move.addTile(loc[f(i)]); \
					c->move.addTile(loc[i]); \
					c->board = new Board(this, &c->move); \
					children = g_list_prepend(children, c); \
					valid ^= t; \
				} \
				t <<= 1; \
			} \
		} \
	}

void Board::add_normal_moves() {
	__u32 empty = ~(red | black);

	__u32 pieces = player == RED_PLAYER ? red & kings : black;
	if (pieces) {
		easy(BOTTOM_ROW | RIGHT_COL, shift_ul, ind_dr);// ↖
		easy(BOTTOM_ROW | LEFT_COL, shift_ur, ind_dl);// ↗
	}

	pieces = player == RED_PLAYER ? red : black & kings;
	if (pieces) {
		easy(TOP_ROW | LEFT_COL, shift_dr, ind_ul);// ↘
		easy(TOP_ROW | RIGHT_COL, shift_dl, ind_ur);// ↙
	}
}

void Board::generate_moves() {
	add_jump_moves();
	if (children == NULL)
		add_normal_moves();
}

bool Board::operator< (const Board &that) const {
	if ( red   < that.red)    return true;
	if (black  < that.black)  return true;
	if (kings  < that.kings)  return true;
	if (player < that.player) return true;
	return false;
}

void Board::operator= (const Board &that) {
	player = that.player;
	red   = that.red;
	black = that.black;
	kings = that.kings;
	value = that.value;
	min = that.min;
	max = that.max;
}
