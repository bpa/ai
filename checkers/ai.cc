#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <signal.h>
#include "ai.h"
#include "debug.h"

#define INCREF(board) board->refcount++;

#define DECREF(board) \
	board->refcount--; \
	if (board->refcount == 0) { \
		g_queue_push_tail(garbage, board); \
	}

#define DEBUG \
{ \
	g_timer_stop(timer); \
	cerr.precision(2); \
	double e = g_timer_elapsed(timer, &trashpointer); \
	if (e > .0001) \
		cerr << "ai:" << __LINE__ << " " << e << endl; \
	g_timer_start(timer); \
}

volatile sig_atomic_t have_time_left = 0;

void handle_timeout(int info) {
	have_time_left = 0;
	cerr << "hi" << endl;
}

void set_signal_handlers() {
	struct sigaction new_action;

	new_action.sa_handler = handle_timeout;
	sigemptyset (&new_action.sa_mask);
	new_action.sa_flags = 0;

	sigaction (SIGINT, &new_action, NULL);
	sigaction (SIGTERM, &new_action, NULL);
	sigaction (SIGALRM, &new_action, NULL);
}

gulong trashpointer;
GTimer *timer;

gint best_board(gconstpointer a, gconstpointer b) {
	if (((Child*)a)->board->player == RED_PLAYER)
		return ((Child*)b)->board->best - ((Child*)a)->board->best;
	else
		return ((Child*)b)->board->best - ((Child*)a)->board->best;
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

#define B(r) ((Board*)r)
gint priority_cmp(gconstpointer a, gconstpointer b, gpointer data) {
//	int depth_diff = B(a)->depth - B(b)->depth;
//	if (depth_diff > 4 || depth_diff < -4)
//		return depth_diff;

	int a_weight = B(a)->player == RED_PLAYER ? B(a)->value : -B(a)->value;
	int b_weight = B(b)->player == RED_PLAYER ? B(b)->value : -B(b)->value;
	if (a_weight != b_weight) return a_weight - b_weight;
	return a < b ? -1 : 1;
}

AI::AI(Player me, float time_limit) {
	states = g_tree_new(key_cmp);
	backlog = g_sequence_new(NULL);
	garbage = g_queue_new();

	current = new Board(me);
	g_sequence_insert_sorted(backlog, current, priority_cmp, NULL);
	INCREF(current);
	for (pieces = 0; pieces < 25; pieces++) 
		steps[pieces] = g_tree_new(key_cmp);
	g_tree_insert(states, current, current);
	g_tree_insert(steps[24], current, current);
	pieces = 24;
	timer = g_timer_new();
	g_timer_start(timer);
	set_signal_handlers();
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
	have_time_left = 1;
	//set_alarm();
	gc();
	search();
	if (!have_time_left) {
		cerr << "Got killed" << endl;
		return NULL;
	}
	cerr << g_tree_nnodes(states) << " states in memory" << endl;
	if (!current->processed) {
		cerr << "we are slacking" << endl;
		process_node(current);
	}
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
	Board *cache = (Board*)g_tree_lookup(states, next);
	
	if (cache == NULL) {
		cerr << "move wasn't found" << endl;
		g_tree_insert(states, next, next);
		g_tree_insert(steps[next->pieces], next, next);
		next->depth = current->depth + 1;
		g_sequence_insert_sorted(backlog, next, priority_cmp, NULL);
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

void update_parents(Board *child) {
	for (GList *it=child->parents; it != NULL; it=g_list_next(it)) {
		Board *parent = (Board*)it->data;
		if (parent->player == RED_PLAYER) {
			if (parent->best < child->best) {
				parent->best = child->best;
				update_parents(parent);
			}
		}
		else {
			if (parent->best > child->best) {
				parent->best = child->best;
				update_parents(parent);
			}
		}
	}
}

gboolean delete_node(gpointer key, gpointer node, gpointer ai) {
	Board *b = B(node);
	b->refcount = 0;
	if (b->processed) {
		g_tree_remove(((AI*)ai)->states, b);
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
	return false;
}

void AI::search() {
	int nodes = 0;
	cerr << this << endl;
	cerr << "backlog: " << g_sequence_get_length(backlog) << endl;
	for (int i=0; i<10000; i++) {
POP:
		GSequenceIter *begin = g_sequence_get_begin_iter(backlog);
		if (g_sequence_iter_is_end(begin))
			break;
		Board *b = (Board*)g_sequence_get(begin);
		g_sequence_remove(begin);
		if (b == NULL) {
			cerr << "NULL?" << endl;
			continue;
		}
		if (b->refcount < 1) {
			if (b->processed) {
				delete_node(b, b, this);
			}
			else {
				if(g_tree_remove(states, b)) {
					g_tree_remove(steps[b->pieces], b);
					delete b;
				}
			}
			goto POP;
		}
		if (b->processed) {
			goto POP;
		}
		process_node(b);
		nodes++;
	}
	cerr << "Processed " << nodes << " nodes" << endl;
}

void AI::process_node(Board *b) {
	b->generate_moves();
	for (GList *it=b->children; it; it=it->next) {
		Child *child = (Child*)it->data;
		child->pieces = child->board->pieces;
		Board *cache = (Board*)g_tree_lookup(states, child->board);
		if (cache == NULL) {
			g_tree_insert(states, child->board, child->board);
			g_tree_insert(steps[child->board->pieces], child->board, child->board);
			child->board->depth = b->depth + 1;
			g_sequence_insert_sorted(backlog, child->board, priority_cmp, NULL);
			cache = child->board;
		}
		else {
			delete child->board;
			child->board = cache;
		}
		cache->parents = g_list_prepend(cache->parents, b);

		update_parents(cache);
		INCREF(cache);
	}
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
	g_tree_foreach(steps[pieces], delete_node, this);
	cerr << "Deleted " << g_tree_nnodes(steps[pieces]) << " from level " << pieces << endl;
	g_tree_destroy(steps[pieces]);
	steps[pieces] = g_tree_new(key_cmp);
}

void AI::gc_garbage() {
	Board *b;
	int collected = 0;
	while ((b = (Board*)g_queue_pop_head(garbage))) {
		g_tree_remove(states, b);
		g_tree_remove(steps[b->pieces], b);
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
