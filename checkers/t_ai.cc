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
" . . . r");
	Board *ref = ai.states[b];
	ASSERT_TRUE(ref != NULL);
	ASSERT_EQ(32, ref->max);
}
