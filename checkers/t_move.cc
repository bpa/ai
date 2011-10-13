#include <gtest/gtest.h>
#include "move.h"

using namespace std;

TEST(move, init) {
	Move m("1-2-3");
	ASSERT_EQ(1, m.tiles[0]);
	ASSERT_EQ(2, m.tiles[1]);
	ASSERT_EQ(3, m.tiles[2]);

	m = Move("32-28");
	ASSERT_EQ(32, m.tiles[0]);
	ASSERT_EQ(28, m.tiles[1]);
}

TEST(move, to_string) {
	char str[16];
	Move m;
	m.addTile(1);
	m.addTile(6);
	m.to_string(str);
	ASSERT_STREQ("1-6", str);

	Move l;
	l.addTile(32);
	l.addTile(14);
	l.addTile(3);
	l.to_string(str);
	ASSERT_STREQ("32-14-3", str);
}
