#include <gtest/gtest.h>
#include "ai.h"

using namespace std;

TEST(ai, cache) {
	AI ai(RED_PLAYER, 1);
	Board a(RED_PLAYER,
" . . . r");
	a.max = 32;
	ai.states[a] = &a;

	Board b(RED_PLAYER,
" . . . b");
	ai.states[b] = &b;
	b.max = 16;

	Board c(RED_PLAYER,
" . . . b");
	Board *ref = ai.states[c];
	ASSERT_TRUE(ref != NULL);
	ASSERT_EQ(16, ref->max);

	ref = ai.states[a];
	ASSERT_TRUE(ref != NULL);
	ASSERT_EQ(32, ref->max);
}
