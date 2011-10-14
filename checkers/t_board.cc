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
	b.say();
	vector<Move> *moves = b.generate_moves(RED_PLAYER);
	ASSERT_EQ((unsigned int)2, moves->size());
	sort(moves->begin(), moves->end());
	ASSERT_STREQ("6-1", moves->at(0).str().c_str());
	ASSERT_STREQ("6-2", moves->at(1).str().c_str());

	moves = b.generate_moves(BLACK_PLAYER);
	ASSERT_EQ((unsigned int)2, moves->size());
	sort(moves->begin(), moves->end());
	ASSERT_STREQ("7-10", moves->at(0).str().c_str());
	ASSERT_STREQ("7-11", moves->at(1).str().c_str());
}
