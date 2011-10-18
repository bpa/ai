#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include "ai.h"

#define INCREF(board) board->refcount++;
#define DECREF(board) \
	if (--board->refcount == 0) { \
		garbage.push(board); \
	}

bool best_board(Child *a, Child *b) {
	if (a->board->player == RED_PLAYER)
		return a->board->max < b->board->max;
	else
		return a->board->min < b->board->min;
}

AI::AI(Player me, float time_limit) {
	current = new Board(me);
	states[*current] = current;
	INCREF(current);
	backlog.push(*current);
}

void AI::move(const char *str) {
	Move *m = new Move(str);
	execute_move(m);
}

Move *AI::choose_next_move() {
	gc();
	search();
	cerr << states.size() << " states in memory" << endl;
	sort(current->children.begin(), current->children.end(), best_board);
	if (current->children.empty())
		return NULL;
	Move *m = &current->children.at(0)->move;
	execute_move(m);
	return m;
}

void AI::execute_move(Move *m) {
	Board *next = new Board(current, m);
	Board *cache = states[*next];
	if (cache == NULL) {
		states[*next] = next;
		backlog.push(*next);
		cache = next;
	}
	INCREF(cache);
	DECREF(current);
	current = cache;
}

void AI::search() {
	int nodes = 0;
	for (int i=0; i<1000; i++) {
	cerr << "backlog: " << backlog.size() << endl;
		if (backlog.empty()) 
			break;
		Board key = backlog.front();
		backlog.pop();
		Board *b = states[key];
		b->generate_moves();
		vector<Child*>::iterator it;
		for (it=b->children.begin(); it < b->children.end(); it++) {
			Child *child = *it;
			Board *cache = states[*child->board];
			if (cache == NULL) {
				states[*child->board] = child->board;
				backlog.push(*child->board);
				cache = child->board;
			}
			INCREF(cache);
			break;
		}
		nodes++;
	}
	cerr << "Processed " << nodes << " nodes" << endl;
}

void AI::gc() {
	Board *b;
	int collected = 0;
	vector<Child*>::iterator it;
	while (!garbage.empty()) {
		b = garbage.front();
		garbage.pop();
		for (it=b->children.begin(); it < b->children.end(); it++) {
			Child *child = *it;
			DECREF(child->board);
		}
		delete b;
		collected++;
	}
	cerr << "Deleted " << collected << " states" << endl;
}
