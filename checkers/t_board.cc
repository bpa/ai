#include <algorithm>
#include <iostream>
#include <sstream>
#include <gtest/gtest.h>
#include <math.h>
#include "board.h"

#define pos(x) (__u32)pow(2, x)

using namespace std;

TEST(board, init) {
	Board b;
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
	Board b(
" . . . r"
". . . . "
" . . . b"
". R B . "
" . . . .");
	ASSERT_EQ((__u32)0, b.red   & pos(11));

	ASSERT_NE((__u32)0, b.red   & pos(25));
	ASSERT_EQ((__u32)0, b.kings & pos(25));

	ASSERT_NE((__u32)0, b.black & pos(17));
	ASSERT_EQ((__u32)0, b.kings & pos(17));

	ASSERT_NE((__u32)0, b.red   & pos(28));
	ASSERT_NE((__u32)0, b.kings & pos(28));

	ASSERT_NE((__u32)0, b.black & pos(22));
	ASSERT_NE((__u32)0, b.kings & pos(22));
}

TEST(board, basic_moves) {
	Board b(
" . . . ."
". r b . ");
	vector<Move> *moves = b.generate_moves(RED_PLAYER);
	ASSERT_EQ((unsigned int)2, moves->size());
	sort(moves->begin(), moves->end());
	ASSERT_STREQ("6-9", moves->at(0).str().c_str());
	ASSERT_STREQ("6-10", moves->at(1).str().c_str());

	moves = b.generate_moves(BLACK_PLAYER);
	ASSERT_EQ((unsigned int)2, moves->size());
	sort(moves->begin(), moves->end());
	ASSERT_STREQ("7-3", moves->at(1).str().c_str());
	ASSERT_STREQ("7-2", moves->at(0).str().c_str());

	b = Board(
" . r . ."
". r r . ");
	moves = b.generate_moves(RED_PLAYER);
	ASSERT_EQ((unsigned int)4, moves->size());
	sort(moves->begin(), moves->end());
	ASSERT_STREQ("6-9", moves->at(0).str().c_str());
	ASSERT_STREQ("6-10", moves->at(1).str().c_str());
	ASSERT_STREQ("7-10", moves->at(2).str().c_str());
	ASSERT_STREQ("7-11", moves->at(3).str().c_str());

}

TEST(board, basic_king_moves) {
	Board b(
" . . . ."
". R B . ");
	vector<Move> *moves = b.generate_moves(RED_PLAYER);
	ASSERT_EQ((unsigned int)4, moves->size());
	sort(moves->begin(), moves->end());
	ASSERT_STREQ("6-1", moves->at(0).str().c_str());
	ASSERT_STREQ("6-2", moves->at(1).str().c_str());
	ASSERT_STREQ("6-9", moves->at(2).str().c_str());
	ASSERT_STREQ("6-10", moves->at(3).str().c_str());

	moves = b.generate_moves(BLACK_PLAYER);
	ASSERT_EQ((unsigned int)4, moves->size());
	sort(moves->begin(), moves->end());
	ASSERT_STREQ("7-2", moves->at(0).str().c_str());
	ASSERT_STREQ("7-3", moves->at(1).str().c_str());
	ASSERT_STREQ("7-10", moves->at(2).str().c_str());
	ASSERT_STREQ("7-11", moves->at(3).str().c_str());
}

TEST(board, borders) {
	Board b(
" . . . R"
". . . . "
" . . . ."
". . . . "
" . . . ."
". . . . "
" . . . ."
"B . . . ");
	vector<Move> *moves = b.generate_moves(RED_PLAYER);
	ASSERT_EQ((unsigned int)1, moves->size());
	sort(moves->begin(), moves->end());
	ASSERT_STREQ("4-8", moves->at(0).str().c_str());

	moves = b.generate_moves(BLACK_PLAYER);
	ASSERT_EQ((unsigned int)1, moves->size());
	sort(moves->begin(), moves->end());
	ASSERT_STREQ("29-25", moves->at(0).str().c_str());

	b = Board(
" . R . ."
". . . . "
" . . . ."
". . . . "
" . . . ."
". . . . "
" . . . ."
". . B . ");
	moves = b.generate_moves(RED_PLAYER);
	ASSERT_EQ((unsigned int)2, moves->size());
	sort(moves->begin(), moves->end());
	ASSERT_STREQ("2-6", moves->at(0).str().c_str());
	ASSERT_STREQ("2-7", moves->at(1).str().c_str());

	moves = b.generate_moves(BLACK_PLAYER);
	ASSERT_EQ((unsigned int)2, moves->size());
	sort(moves->begin(), moves->end());
	ASSERT_STREQ("31-26", moves->at(0).str().c_str());
	ASSERT_STREQ("31-27", moves->at(1).str().c_str());

	b = Board(
" . . . ."
". . . . "
" . . . ."
"R . . . "
" . . . B"
". . . . "
" . . . ."
". . . . ");
	moves = b.generate_moves(RED_PLAYER);
	ASSERT_EQ((unsigned int)2, moves->size());
	sort(moves->begin(), moves->end());
	ASSERT_STREQ("13-9", moves->at(0).str().c_str());
	ASSERT_STREQ("13-17", moves->at(1).str().c_str());

	moves = b.generate_moves(BLACK_PLAYER);
	ASSERT_EQ((unsigned int)2, moves->size());
	sort(moves->begin(), moves->end());
	ASSERT_STREQ("20-16", moves->at(0).str().c_str());
	ASSERT_STREQ("20-24", moves->at(1).str().c_str());

}

TEST(board, jumps) {
	Board b(
" . r . ."
". . b . "
" . . . .");
	vector<Move> *moves = b.generate_moves(RED_PLAYER);
	ASSERT_EQ((unsigned int)1, moves->size());
	sort(moves->begin(), moves->end());
	ASSERT_STREQ("2-11", moves->at(0).str().c_str());

	b = Board(
" . . . ."
". b b . "
" . R . ."
". b b . "
" . . . .");
	moves = b.generate_moves(RED_PLAYER);
	ASSERT_EQ((unsigned int)4, moves->size());
	sort(moves->begin(), moves->end());
	ASSERT_STREQ("10-1", moves->at(0).str().c_str());
	ASSERT_STREQ("10-3", moves->at(1).str().c_str());
	ASSERT_STREQ("10-17", moves->at(2).str().c_str());
	ASSERT_STREQ("10-19", moves->at(3).str().c_str());

	b = Board(
" . . . R"
". . . b "
" . . b .");
	moves = b.generate_moves(RED_PLAYER);
	ASSERT_EQ((unsigned int)0, moves->size());
}

TEST(board, multi_jumps) {
	Board b(
" . r . ."
". . b . "
" . . . ."
". . . b ");
	vector<Move> *moves = b.generate_moves(RED_PLAYER);
	ASSERT_EQ((unsigned int)1, moves->size());
	sort(moves->begin(), moves->end());
	ASSERT_STREQ("2-11-20", moves->at(0).str().c_str());

	b = Board(
" . r . ."
". . b . "
" . . . ."
". . b b ");
	moves = b.generate_moves(RED_PLAYER);
	ASSERT_EQ((unsigned int)2, moves->size());
	sort(moves->begin(), moves->end());
	ASSERT_STREQ("2-11-18", moves->at(0).str().c_str());
	ASSERT_STREQ("2-11-20", moves->at(1).str().c_str());

	b = Board(
" . . . R"
". . b b "
" . . . ."
". . . b "
" . . . ."
". . . b ");
	moves = b.generate_moves(RED_PLAYER);
	ASSERT_EQ((unsigned int)2, moves->size());
	sort(moves->begin(), moves->end());
	ASSERT_STREQ("4-11-2", moves->at(0).str().c_str());
	ASSERT_STREQ("4-11-20-27", moves->at(1).str().c_str());

	b = Board(
" . R . ."
". b b . "
" . . . ."
". b b . ");
	moves = b.generate_moves(RED_PLAYER);
	ASSERT_EQ((unsigned int)2, moves->size());
	sort(moves->begin(), moves->end());
	ASSERT_STREQ("2-9-18-11-2", moves->at(0).str().c_str());
	ASSERT_STREQ("2-11-18-9-2", moves->at(1).str().c_str());
}

TEST(board, king_borders) {
	Board b(
" . . . b"
". . . R "
" . . . ."
". . . . "
" . . . ."
". . . . "
" B . . ."
"r . . . ");
	vector<Move> *moves = b.generate_moves(RED_PLAYER);
	ASSERT_EQ((unsigned int)3, moves->size());

	moves = b.generate_moves(BLACK_PLAYER);
	ASSERT_EQ((unsigned int)3, moves->size());

	b = Board(
" . b b ."
". . R . "
" . . . ."
". . . . "
" . . . ."
". . . . "
" . B . ."
". r r . ");
	moves = b.generate_moves(RED_PLAYER);
	ASSERT_EQ((unsigned int)2, moves->size());
	sort(moves->begin(), moves->end());
	ASSERT_STREQ("7-10", moves->at(0).str().c_str());
	ASSERT_STREQ("7-11", moves->at(1).str().c_str());

	moves = b.generate_moves(BLACK_PLAYER);
	ASSERT_EQ((unsigned int)2, moves->size());
	sort(moves->begin(), moves->end());
	ASSERT_STREQ("26-22", moves->at(0).str().c_str());
	ASSERT_STREQ("26-23", moves->at(1).str().c_str());

	b = Board(
" . . . ."
"b . . . "
" R . . r"
"b . . B "
" . . . r"
". . . . "
" . . . ."
". . . . ");
	moves = b.generate_moves(RED_PLAYER);
	ASSERT_EQ((unsigned int)1, moves->size());
	sort(moves->begin(), moves->end());
	ASSERT_STREQ("12-19", moves->at(0).str().c_str());

	moves = b.generate_moves(BLACK_PLAYER);
	ASSERT_EQ((unsigned int)1, moves->size());
	sort(moves->begin(), moves->end());
	ASSERT_STREQ("13-6", moves->at(0).str().c_str());
}
