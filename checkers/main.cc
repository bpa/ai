#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "choice.h"
#include "move.h"
 
using namespace std;

int main(int argc, char *argv[]) {
	float time_limit;
	int max_depth, len;
	Player me;
    char buf[1028];
	Board *state, *new_state;
	Move *move;

    /* Convert command line parameters */
    time_limit = (float) atof(argv[1]);
    max_depth = (argc == 4) ? atoi(argv[3]) : -1;

	cerr <<  argv[0] << " SecPerMove == " << time_limit << endl;

    len = read(STDIN_FILENO, buf, 1028);
    if(!strncmp(buf,"Player1", strlen("Player1"))) {
        cerr << "I'm Player 1" << endl;
        me = RED_PLAYER; 
    }
    else {
        cerr << "I'm Player 2" << endl;
        me = BLACK_PLAYER;
    }

    /* Set up the board */ 
    state = new Board(me);
    srand((unsigned int)time(0));

    if (me == RED_PLAYER) goto determine_next_move;

    for(;;) {
        len = read(STDIN_FILENO, buf, 1028);
        buf[len]='\0';
		state->say();
		cerr << "Them: " << buf << endl;

        move = new Move(buf);
        new_state = new Board(state, move);
		delete(state);
		state = new_state;
		delete(move);
        
determine_next_move:
		state->say();
		state->generate_moves();
        move = choice_random(state);
cerr << "Us: " << *move << endl;
        if (move != NULL) {
        	new_state = new Board(state, move);
			delete(state);
			state = new_state;
			cout << *move << flush;
			delete(move);
        }
        else exit(1); /* No legal moves available, so I have lost */

    }

    return 0;
}
