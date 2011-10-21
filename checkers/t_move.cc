#include <gtest/gtest.h>
#include "move.h"

using namespace std;

static char buf[24];

TEST(move, init) {
	Move *m = move_from_string("1-2-3");
	ASSERT_EQ(1, m->tiles[0]);
	ASSERT_EQ(2, m->tiles[1]);
	ASSERT_EQ(3, m->tiles[2]);
	ASSERT_EQ(3, m->moves);
	move_to_string(m, buf);
	ASSERT_STREQ("1-2-3", buf);

	free(m);
	m = move_from_string("32-28");
	ASSERT_EQ(2, m->moves);
	ASSERT_EQ(32, m->tiles[0]);
	ASSERT_EQ(28, m->tiles[1]);
	move_to_string(m, buf);
	ASSERT_STREQ("32-28", buf);
	free(m);
}

TEST(move, to_string) {
	Move *m = move_new();
	move_add_tile(m, 1);
	move_add_tile(m, 6);
	ASSERT_EQ(2, m->moves);
	move_to_string(m, buf);
	ASSERT_STREQ("1-6", buf);
	free(m);

	Move *l = move_new();
	move_add_tile(l, 32);
	move_add_tile(l, 14);
	move_add_tile(l, 3);
	ASSERT_EQ(3, l->moves);
	move_to_string(l, buf);
	ASSERT_STREQ("32-14-3", buf);
	free(l);
}
