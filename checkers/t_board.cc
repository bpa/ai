#include <iostream>
#include <sstream>
#include <gtest/gtest.h>
#include "board.h"

using namespace std;

TEST(board, init) {
	Board b;
	ASSERT_EQ(0x00, b.tiles[0][0]);
	ASSERT_EQ(0x01, b.tiles[1][0]);
	ASSERT_EQ(0x00, b.tiles[1][7]);
	ASSERT_EQ(0x01, b.tiles[2][7]);
	ASSERT_EQ(0x00, b.tiles[3][0]);
	ASSERT_EQ(0x00, b.tiles[4][7]);

	ASSERT_EQ(0x05, b.tiles[5][0]);
	ASSERT_EQ(0x00, b.tiles[6][0]);
	ASSERT_EQ(0x05, b.tiles[7][0]);

	stringstream ss;
	unsigned char *pos = (unsigned char*)&b.hash;
	for (int i=0; i<8; i++) {
		char p = *pos++;
		ss << hex << (int)p;
	}
	ASSERT_STREQ("11111111111100", ss.str().c_str());
}

