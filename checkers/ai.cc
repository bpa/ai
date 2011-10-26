#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <signal.h>
#include <time.h>
#include "ai.h"
#include "debug.h"

#define INCREF(board) board->refcount++;

#define DECREF(board) \
	board->refcount--; \
	if (board->refcount == 0) { \
		g_queue_push_tail(garbage, board); \
	}

#define C(it) ((Child*)it->data)

volatile sig_atomic_t have_time_left = 0;

void handle_timeout(int info) {
	have_time_left = 0;
}

void AI::init_timer(float limit) {
	limit -= .0001;
	int seconds = limit;
	int nanos = (limit - seconds) * 1000000000;
	turn_time_limit.it_value.tv_sec = seconds;
	turn_time_limit.it_value.tv_nsec = nanos;

	turn_time_limit.it_interval.tv_sec = 0;
	turn_time_limit.it_interval.tv_nsec = 0;

	timer_create (CLOCK_REALTIME, NULL, &timerid);

	struct sigaction new_action;

	new_action.sa_handler = handle_timeout;
	sigemptyset (&new_action.sa_mask);
	new_action.sa_flags = 0;

	sigaction (SIGINT, &new_action, NULL);
	sigaction (SIGTERM, &new_action, NULL);
	sigaction (SIGALRM, &new_action, NULL);
}

gint best_board(gconstpointer a, gconstpointer b) {
	if (((Child*)a)->board->player == RED_PLAYER)
		return ((Child*)a)->board->best - ((Child*)b)->board->best;
	else
		return ((Child*)b)->board->best - ((Child*)a)->board->best;
}

AI::AI(Player me, float time_limit) {
	states = g_hash_table_new(board_hash, board_equal);
	garbage = g_queue_new();

	current = new Board(me);
	INCREF(current);
	for (pieces = 0; pieces < 25; pieces++) 
		steps[pieces] = g_hash_table_new(board_hash, board_equal);
	pieces = 24;
	g_hash_table_insert(states, current, current);
	g_hash_table_insert(steps[24], current, current);
	init_timer(time_limit);
}

void AI::move(const char *str) {
	Move *m = move_from_string(str);
	execute_move(m);
	free(m);
}

void print_it(gpointer child, gpointer trash) {
	Child *c = (Child*)child;
	cerr << "Child: " << c->move << " best " << c->board->best << endl;
}

Move *AI::choose_next_move() {
	timer_settime (timerid, 0, &turn_time_limit, NULL);
	have_time_left = 1;
	gc();
	search();
	cerr << g_hash_table_size(states) << " states in memory" << endl;
	if (current->children == NULL) {
		cerr << "no moves" << endl;
		return NULL;
	}
	current->children = g_list_sort(current->children, best_board);
	g_list_foreach(current->children, print_it, NULL);
	Move *m = &((Child*)current->children->data)->move;
	execute_move(m);
	return m;
}

void AI::execute_move(Move *m) {
	Board *next = new Board(current, m);
	Board *cache = (Board*)g_hash_table_lookup(states, next);
	
	if (cache == NULL) {
		cerr << "move wasn't found" << endl;
		g_hash_table_insert(states, next, next);
		g_hash_table_insert(steps[next->pieces], next, next);
		cache = next;
	}
	else {
		delete next;
	}
	INCREF(cache);
	DECREF(current);
	current = cache;
	cerr << *current << endl;
}

void update_minimax(Board *board) {
	int best;
	if (board->player == RED_PLAYER) {
		best = -GAME_OVER;
		for (GList *child=board->children; child; child=child->next) {
			if (best < C(child)->board->best) {
				best = C(child)->board->best;
			}
		}
	}
	else {
		best = GAME_OVER;
		for (GList *child=board->children; child; child=child->next) {
			if (best > C(child)->board->best) {
				best = C(child)->board->best;
			}
		}
	}
	if (board->best != best) {
		board->best = best;
		for (GList *it=board->parents; it; it=it->next) {
			update_minimax((Board*)it->data);
		}
	}
}

void delete_node(gpointer key, gpointer node, gpointer ai) {
	Board *b = (Board*)node;
	b->refcount = 0;
	if (b->processed) {
		g_hash_table_remove(((AI*)ai)->states, b);
		while (b->children != NULL) {
			Child *child = (Child*)b->children->data;
			b->children = g_list_delete_link(b->children, b->children);
			if (child->pieces != ((AI*)ai)->pieces) {
				child->board->refcount--; \
				if (child->board->refcount == 0) { \
					g_queue_push_tail(((AI*)ai)->garbage, child->board); \
				}
				child->board->parents = g_list_remove(child->board->parents, b);
			}
			free(child);
		}
		delete b;
	}
}

void AI::search() {
	int nodes = 0;
	int depth = 0;
	current->depth = 0;
	GQueue *backlog = g_queue_new();
	g_queue_push_tail(backlog, current);
	while (have_time_left) {
		Board *b = (Board*)g_queue_pop_head(backlog);
		if (b == NULL) {
			break;
		}
		if (depth < b->depth)
			depth = b->depth;
		if (!b->processed)
			process_node(b);
		for (GList *it=b->children; it; it=it->next) {
			if (C(it)->board->depth >= b->depth) {
				C(it)->board->depth = b->depth + 1;
				g_queue_push_tail(backlog, ((Child*)it->data)->board);
				//TODO: Add alpha/beta pruning
			}
		}
		nodes++;
	}
	cerr << "Processed " << nodes << " nodes, max depth " << depth << endl;
	g_queue_free(backlog);
}

void AI::process_node(Board *b) {
	b->generate_moves();
	for (GList *it=b->children; it; it=it->next) {
		Child *child = (Child*)it->data;
		child->pieces = child->board->pieces;
		Board *cache = (Board*)g_hash_table_lookup(states, child->board);
		if (cache == NULL) {
			g_hash_table_insert(states, child->board, child->board);
			g_hash_table_insert(steps[child->board->pieces], child->board, child->board);
			child->board->depth = b->depth + 1;
			cache = child->board;
		}
		else {
			delete child->board;
			child->board = cache;
		}
		cache->parents = g_list_prepend(cache->parents, b);

		INCREF(cache);
	}
	update_minimax(b);
}

void AI::gc() {
	gc_garbage();
	while (pieces > current->pieces) {
		gc_step();
		gc_garbage();
		pieces--;
	}
}

void AI::gc_step() {
	g_hash_table_foreach(steps[pieces], delete_node, this);
	cerr << "Deleted " << g_hash_table_size(steps[pieces]) << " from level " << pieces << endl;
	g_hash_table_remove_all(steps[pieces]);
}

void AI::gc_garbage() {
	Board *b;
	int collected = 0;
	while ((b = (Board*)g_queue_pop_head(garbage))) {
		g_hash_table_remove(states, b);
		g_hash_table_remove(steps[b->pieces], b);
		if (b->processed) {
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
	}
	cerr << "Deleted " << collected << " states" << endl;
}
