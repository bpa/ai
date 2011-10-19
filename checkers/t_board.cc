#include <algorithm>
#include <iostream>
#include <sstream>
#include <gtest/gtest.h>
#include <math.h>
#include "board.h"

#define pos(x) (__u32)pow(2, x)

using namespace std;

#define MOVE(c, i) ((Child*)g_list_nth_data(c, i))->move.str()
extern void x(__u32);

gint order_by_moves(gconstpointer a, gconstpointer b) {
	return ((Move*)a)->cmp((Move*)b);
}

GList *children(Board *b, Player p) {
	b->player = p;
	while (b->children != NULL) {
		free(b->children->data);
		b->children = g_list_delete_link(b->children, b->children);
	}
	b->generate_moves();
	b->children = g_list_sort(b->children, order_by_moves);
	return b->children;
}

TEST(board, init) {
	Board b(RED_PLAYER);
	int red=0;
	int black=0;
	__u32 mask = 1;
	for (int i=0; i<32; i++) {
		if (mask & b. red )  red++;
		if (mask & b.black) black++;
		mask <<= 1;
	}
	ASSERT_EQ(12,  red );
	ASSERT_EQ(12, black);
	ASSERT_EQ((__u32)0, b.kings);

}

TEST(board, test_build) {
	Board b(RED_PLAYER,
" . . . r"
". . . . "
" . . . b"
". R B . "
" . . . .");
	ASSERT_FALSE(b.red   & pos(11));

	ASSERT_TRUE (b.red   & pos(25));
	ASSERT_FALSE(b.kings & pos(25));

	ASSERT_TRUE (b.black & pos(17));
	ASSERT_FALSE(b.kings & pos(17));

	ASSERT_TRUE(b.red   & pos(28));
	ASSERT_TRUE(b.kings & pos(28));

	ASSERT_TRUE(b.black & pos(22));
	ASSERT_TRUE(b.kings & pos(22));
}

TEST(board, basic_moves) {
	Board b(RED_PLAYER,
" . . . ."
". r b . ");
	GList *moves = children(&b,RED_PLAYER);
	ASSERT_EQ((uint)2, g_list_length(moves));
	ASSERT_STREQ("6-9", MOVE(moves,0));
	ASSERT_STREQ("6-10", MOVE(moves,1));

	moves = children(&b,BLACK_PLAYER);
	ASSERT_EQ((uint)2, g_list_length(moves));
	ASSERT_STREQ("7-3", MOVE(moves,1));
	ASSERT_STREQ("7-2", MOVE(moves,0));

	b = Board(RED_PLAYER,
" . r . ."
". r r . ");
	moves = children(&b,RED_PLAYER);
	ASSERT_EQ((uint)4, g_list_length(moves));
	ASSERT_STREQ("6-9", MOVE(moves,0));
	ASSERT_STREQ("6-10", MOVE(moves,1));
	ASSERT_STREQ("7-10", MOVE(moves,2));
	ASSERT_STREQ("7-11", MOVE(moves,3));

}

TEST(board, basic_king_moves) {
	Board b(RED_PLAYER,
" . . . ."
". R B . ");
	GList *moves = children(&b,RED_PLAYER);
	ASSERT_EQ((uint)4, g_list_length(moves));
	ASSERT_STREQ("6-1", MOVE(moves,0));
	ASSERT_STREQ("6-2", MOVE(moves,1));
	ASSERT_STREQ("6-9", MOVE(moves,2));
	ASSERT_STREQ("6-10", MOVE(moves,3));

	moves = children(&b,BLACK_PLAYER);
	ASSERT_EQ((uint)4, g_list_length(moves));
	ASSERT_STREQ("7-2", MOVE(moves,0));
	ASSERT_STREQ("7-3", MOVE(moves,1));
	ASSERT_STREQ("7-10", MOVE(moves,2));
	ASSERT_STREQ("7-11", MOVE(moves,3));
}

TEST(board, borders) {
	Board b(RED_PLAYER,
" . . . R"
". . . . "
" . . . ."
". . . . "
" . . . ."
". . . . "
" . . . ."
"B . . . ");
	GList *moves = children(&b,RED_PLAYER);
	ASSERT_EQ((uint)1, g_list_length(moves));
	ASSERT_STREQ("4-8", MOVE(moves,0));

	moves = children(&b,BLACK_PLAYER);
	ASSERT_EQ((uint)1, g_list_length(moves));
	ASSERT_STREQ("29-25", MOVE(moves,0));

	b = Board(RED_PLAYER,
" . R . ."
". . . . "
" . . . ."
". . . . "
" . . . ."
". . . . "
" . . . ."
". . B . ");
	moves = children(&b,RED_PLAYER);
	ASSERT_EQ((uint)2, g_list_length(moves));
	ASSERT_STREQ("2-6", MOVE(moves,0));
	ASSERT_STREQ("2-7", MOVE(moves,1));

	moves = children(&b,BLACK_PLAYER);
	ASSERT_EQ((uint)2, g_list_length(moves));
	ASSERT_STREQ("31-26", MOVE(moves,0));
	ASSERT_STREQ("31-27", MOVE(moves,1));

	b = Board(RED_PLAYER,
" . . . ."
". . . . "
" . . . ."
"R . . . "
" . . . B"
". . . . "
" . . . ."
". . . . ");
	moves = children(&b,RED_PLAYER);
	ASSERT_EQ((uint)2, g_list_length(moves));
	ASSERT_STREQ("13-9", MOVE(moves,0));
	ASSERT_STREQ("13-17", MOVE(moves,1));

	moves = children(&b,BLACK_PLAYER);
	ASSERT_EQ((uint)2, g_list_length(moves));
	ASSERT_STREQ("20-16", MOVE(moves,0));
	ASSERT_STREQ("20-24", MOVE(moves,1));

}

TEST(board, jumps) {
	Board b(RED_PLAYER,
" . r . ."
". . b . "
" . . . .");
	GList *moves = children(&b,RED_PLAYER);
	ASSERT_EQ((uint)1, g_list_length(moves));
	ASSERT_STREQ("2-11", MOVE(moves,0));

	b = Board(RED_PLAYER,
" . . . ."
". b b . "
" . R . ."
". b b . "
" . . . .");
	moves = children(&b,RED_PLAYER);
	ASSERT_EQ((uint)4, g_list_length(moves));
	ASSERT_STREQ("10-1", MOVE(moves,0));
	ASSERT_STREQ("10-3", MOVE(moves,1));
	ASSERT_STREQ("10-17", MOVE(moves,2));
	ASSERT_STREQ("10-19", MOVE(moves,3));

	b = Board(RED_PLAYER,
" . . . R"
". . . b "
" . . b .");
	moves = children(&b,RED_PLAYER);
	ASSERT_EQ((uint)0, g_list_length(moves));
}

TEST(board, multi_jumps) {
	Board b(RED_PLAYER,
" . r . ."
". . b . "
" . . . ."
". . . b ");
	GList *moves = children(&b,RED_PLAYER);
	ASSERT_EQ((uint)1, g_list_length(moves));
	ASSERT_STREQ("2-11-20", MOVE(moves,0));

	b = Board(RED_PLAYER,
" . r . ."
". . b . "
" . . . ."
". . b b ");
	moves = children(&b,RED_PLAYER);
	ASSERT_EQ((uint)2, g_list_length(moves));
	ASSERT_STREQ("2-11-18", MOVE(moves,0));
	ASSERT_STREQ("2-11-20", MOVE(moves,1));

	b = Board(RED_PLAYER,
" . . . R"
". . b b "
" . . . ."
". . . b "
" . . . ."
". . . b ");
	moves = children(&b,RED_PLAYER);
	ASSERT_EQ((uint)2, g_list_length(moves));
	ASSERT_STREQ("4-11-2", MOVE(moves,0));
	ASSERT_STREQ("4-11-20-27", MOVE(moves,1));

	b = Board(RED_PLAYER,
" . R . ."
". b b . "
" . . . ."
". b b . ");
	moves = children(&b,RED_PLAYER);
	ASSERT_EQ((uint)2, g_list_length(moves));
	ASSERT_STREQ("2-9-18-11-2", MOVE(moves,0));
	ASSERT_STREQ("2-11-18-9-2", MOVE(moves,1));
}

TEST(board, king_borders) {
	Board b(RED_PLAYER,
" . . . b"
". . . R "
" . . . ."
". . . . "
" . . . ."
". . . . "
" B . . ."
"r . . . ");
	GList *moves = children(&b,RED_PLAYER);
	ASSERT_EQ((uint)3, g_list_length(moves));

	moves = children(&b,BLACK_PLAYER);
	ASSERT_EQ((uint)3, g_list_length(moves));

	b = Board(RED_PLAYER,
" . b b ."
". . R . "
" . . . ."
". . . . "
" . . . ."
". . . . "
" . B . ."
". r r . ");
	moves = children(&b,RED_PLAYER);
	ASSERT_EQ((uint)2, g_list_length(moves));
	ASSERT_STREQ("7-10", MOVE(moves,0));
	ASSERT_STREQ("7-11", MOVE(moves,1));

	moves = children(&b,BLACK_PLAYER);
	ASSERT_EQ((uint)2, g_list_length(moves));
	ASSERT_STREQ("26-22", MOVE(moves,0));
	ASSERT_STREQ("26-23", MOVE(moves,1));

	b = Board(RED_PLAYER,
" . . . ."
"b . . . "
" R . . r"
"b . . B "
" . . . r"
". . . . "
" . . . ."
". . . . ");
	moves = children(&b,RED_PLAYER);
	ASSERT_EQ((uint)1, g_list_length(moves));
	ASSERT_STREQ("12-19", MOVE(moves,0));

	moves = children(&b,BLACK_PLAYER);
	ASSERT_EQ((uint)1, g_list_length(moves));
	ASSERT_STREQ("13-6", MOVE(moves,0));
}

#define ASSERT_STATE(child, p, RED, BLACK, KINGS) \
	ASSERT_EQ(p,           child->player); \
	ASSERT_EQ((uint) RED , child->red); \
	ASSERT_EQ((uint)BLACK, child->black); \
	ASSERT_EQ((uint)KINGS, child->kings);

#define ASSERT_BOARD(ind, p, RED, BLACK, KINGS) \
{ \
	Child *child = (Child*)g_list_nth_data(moves, ind); \
	Board *board = child->board; \
	ASSERT_STATE(board, p, RED, BLACK, KINGS) \
}
	
TEST(board, apply_move) {
	Board b(RED_PLAYER,
" . . . r");
	GList *moves = children(&b,RED_PLAYER);
	ASSERT_EQ((uint)1, g_list_length(moves));
	ASSERT_BOARD(0, BLACK_PLAYER, pos(24), 0, 0);
	b = Board(BLACK_PLAYER,
" . . . ."
". B . . ");
	moves = children(&b,BLACK_PLAYER);
	ASSERT_EQ((uint)4, g_list_length(moves));
	ASSERT_BOARD(0, RED_PLAYER, 0, pos(11), pos(11));
	ASSERT_BOARD(1, RED_PLAYER, 0, pos(5),  pos(5));
	ASSERT_BOARD(2, RED_PLAYER, 0, pos(3),  pos(3));
	ASSERT_BOARD(3, RED_PLAYER, 0, pos(29), pos(29));

	b = Board(RED_PLAYER,
" . R . ."
". b b . "
" . . . ."
". B B . ");
	moves = children(&b,RED_PLAYER);
	ASSERT_EQ((uint)2, g_list_length(moves));
	ASSERT_BOARD(0, BLACK_PLAYER, pos(5), 0, pos(5));
	ASSERT_BOARD(1, BLACK_PLAYER, pos(5), 0, pos(5));
}
	
TEST(board, apply_string_move) {
	Board b(RED_PLAYER,
" . . . r");
	Board next = Board(&b, new Move("4-8"));
	ASSERT_STATE((&next), BLACK_PLAYER, pos(24), 0, 0);

	b = Board(BLACK_PLAYER,
" . . . ."
". B . . ");
	next = Board(&b, new Move("6-1"));
	ASSERT_STATE((&next), RED_PLAYER, 0, pos(11), pos(11));

	b = Board(RED_PLAYER,
" . R . ."
". b b . "
" . . . ."
". B B . ");
	next = Board(&b, new Move("2-11-18-9-2"));
	ASSERT_STATE((&next), BLACK_PLAYER, pos(5), 0, pos(5));
}

TEST(board, king_me) {
	Board b(RED_PLAYER,
" . . . ."
". . . b "
" . . . ."
". . . . "
" . . . ."
". . . . "
" . . r ."
". . . . ");
	Board next = Board(&b, new Move("27-32"));
	ASSERT_STATE((&next), BLACK_PLAYER, pos(0), pos(24), pos(0));

	b.player = BLACK_PLAYER;
	next = Board(&b, new Move("8-4"));
	ASSERT_STATE((&next), RED_PLAYER, pos(7), pos(25), pos(25));
}

TEST(board, lt) {
	Board a(RED_PLAYER, " . . . r");
	Board b(RED_PLAYER, " . . . r");
	Board c = a;
	ASSERT_FALSE(a < b);
	ASSERT_FALSE(b < a);
	ASSERT_FALSE(a < c);
	ASSERT_FALSE(c < a);
}
