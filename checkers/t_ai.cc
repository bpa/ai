#include <gtest/gtest.h>
#include "ai.h"

using namespace std;

TEST(ai, cache) {
	AI ai(RED_PLAYER, 1);
	Board a(RED_PLAYER,
" . . . r");
	a.max = 32;
	g_tree_insert(ai.states, &a, &a);

	Board b(RED_PLAYER,
" . . . b");
	g_tree_insert(ai.states, &b, &b);
	b.max = 16;

	Board c(RED_PLAYER,
" . . . b");
	Board *ref = (Board*)g_tree_lookup(ai.states, &c);
	ASSERT_TRUE(ref != NULL);
	ASSERT_TRUE(ref == &b);
	ASSERT_EQ(16, ref->max);

	ref = (Board*)g_tree_lookup(ai.states, &a);
	ASSERT_TRUE(ref != NULL);
	ASSERT_TRUE(ref == &a);
	ASSERT_EQ(32, ref->max);
}
