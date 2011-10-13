#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "choice.h"
#include "move.h"
#include "state.h"
 
int main(int argc, char *argv[]) {
	float time_limit;
	int max_depth, len;
	Player me;
    char buf[1028];
	State *state, *new_state;
	Move *move;

    /* Convert command line parameters */
    time_limit = (float) atof(argv[1]);
    max_depth = (argc == 4) ? atoi(argv[3]) : -1;

	fprintf(stderr, "%s SecPerMove == %f\n", argv[0], time_limit);

    len = read(STDIN_FILENO, buf, 1028);
    if(!strncmp(buf,"Player1", strlen("Player1"))) {
        fprintf(stderr, "I'm Player 1\n");
        me = RED; 
    }
    else {
        fprintf(stderr, "I'm Player 2\n");
        me = BLACK;
    }

    /* Set up the board */ 
    state = new State();
    srand((unsigned int)time(0));

    if (me == RED) goto determine_next_move;

    for(;;) {
        len = read(STDIN_FILENO, buf, 1028);
        buf[len]='\0';

        move = new Move(buf);
        new_state = new State(state, move);
		delete(state);
		state = new_state;
		delete(move);
        
determine_next_move:
        move = choice_random(state, me);
        if (move != NULL) {
        	new_state = new State(state, move);
			delete(state);
			state = new_state;
            move->to_string(buf);
			write(STDOUT_FILENO, buf, strlen(buf));
			fflush(stdout);
			delete(move);
        }
        else exit(1); /* No legal moves available, so I have lost */

    }

    return 0;
}
