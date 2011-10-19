#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include "ai.h"

#define INCREF(board) board->refcount++;

#define DECREF(board) \
	board->refcount--; \
	if (board->refcount == 0) { \
		g_queue_push_tail(garbage, board); \
	}

gint best_board(gconstpointer a, gconstpointer b) {
	if (((Child*)a)->board->player == RED_PLAYER)
		return ((Child*)a)->board->max - ((Child*)b)->board->max;
	else
		return ((Child*)b)->board->min - ((Child*)a)->board->min;
}

#define CMP(field) \
if (((Board*)a)->field != ((Board *)b)->field) \
	return ((Board*)a)->field < ((Board*)b)->field ? -1 : 1;

gint key_cmp(gconstpointer a, gconstpointer b) {
	CMP(red);
	CMP(black);
	CMP(kings);
	CMP(player);
	return 0;
}

AI::AI(Player me, float time_limit) {
	states = g_tree_new(key_cmp);
	backlog = g_queue_new();
	garbage = g_queue_new();

	current = new Board(me);
	g_tree_insert(states, current, current);
	g_queue_push_tail(backlog, current);
	INCREF(current);
	states_in_memory = 1;
}

void AI::move(const char *str) {
	Move *m = new Move(str);
	execute_move(m);
}

Move *AI::choose_next_move() {
	gc();
	search();
	cerr << states_in_memory << " states in memory" << endl;
	if (current->children == NULL) {
		cerr << "no moves" << endl;
		return NULL;
	}
	current->children = g_list_sort(current->children, best_board);
	Move *m = &((Child*)current->children->data)->move;
	execute_move(m);
	return m;
}

void AI::execute_move(Move *m) {
	Board *next = new Board(current, m);
	Board *cache = (Board*)g_tree_lookup(states, next);
	
	if (cache == NULL) {
		cerr << "move wasn't found" << endl;
		g_tree_insert(states, next, next);
		states_in_memory++;
		g_queue_push_head(backlog, next);
		cerr << next << endl;
		cache = next;
	}
	else {
		delete next;
	}
	INCREF(cache);
	DECREF(current);
	current = cache;
}

void update_parents(Board *b, int depth) {
cerr << "updating " << depth << endl;
	for (GList *it=b->parents; it != NULL; it=g_list_next(it)) {
		Board *parent = (Board*)it->data;
		if (b->player == RED_PLAYER) {
			if (b->max < parent->min) {
				parent->min = b->max;
				update_parents(parent, depth+1);
			}
		}
		else {
			if (b->min > parent->max) {
				parent->max = b->min;
				update_parents(parent, depth+1);
			}
		}
	}
}

void AI::search() {
	int nodes = 0;
	cerr << "backlog: " << g_queue_get_length(backlog) << endl;
	for (int i=0; i<1000; i++) {
		Board *b = (Board*)g_queue_pop_head(backlog);
		if (b == NULL)
			continue;
		if (b->refcount < 1)
			continue;
		b->generate_moves();
		for (GList *it=b->children; it != NULL; it=g_list_next(it)) {
			Child *child = (Child*)it->data;
			Board *cache = (Board*)g_tree_lookup(states, child->board);
			if (cache == NULL) {
				g_tree_insert(states, child->board, child->board);
				states_in_memory++;
				g_queue_push_tail(backlog, child->board);
				cache = child->board;
			}
			else {
				delete child->board;
				child->board = cache;
			}
		for (GList *it2=cache->parents; it2 != NULL; it2=g_list_next(it)) {
			cerr << *((Board*)it2) << endl;
		}
			cache->parents = g_list_prepend(cache->parents, b);

			update_parents(child->board, 0);
			INCREF(cache);
		}
		nodes++;
	}
	cerr << "Processed " << nodes << " nodes" << endl;
}

void AI::gc() {
	Board *b;
	int collected = 0;
	while ((b = (Board*)g_queue_pop_head(garbage))) {
		g_tree_remove(states, b);
		states_in_memory--;
		while (b->children != NULL) {
			Child *child = (Child*)b->children->data;
			b->children = g_list_delete_link(b->children, b->children);
			DECREF(child->board);
			child->board->parents = g_list_remove(child->board->parents, b);
			free(child);
		}
		delete b;
		collected++;
	}
	cerr << "Deleted " << collected << " states" << endl;
}
