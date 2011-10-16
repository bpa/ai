#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "ai.h"
 
using namespace std;

int main(int argc, char *argv[]) {
	float time_limit;
	int max_depth, len;
	Player me;
    char buf[1028];

    time_limit = (float) atof(argv[1]);
    max_depth = (argc == 4) ? atoi(argv[3]) : -1;

	cerr <<  argv[0] << " SecPerMove: " << time_limit << endl;

	len = read(STDIN_FILENO, buf, 1028);
	buf[len] = '\0';
    if(!strncmp(buf, "Player1", strlen("Player1"))) {
        cerr << "I'm Player 1" << endl;
        me = RED_PLAYER; 
    }
    else {
        cerr << "I'm Player 2" << endl;
        me = BLACK_PLAYER;
    }

    AI ai(me, time_limit);
    if (me == RED_PLAYER) goto determine_next_move;

    for(;;) {
		len = read(STDIN_FILENO, buf, 1028);
		buf[len] = '\0';
        ai.move(buf);
        
determine_next_move:
        Move *move = ai.choose_next_move();
        if (move != NULL)
			cout << *move << flush;
        else
			return 1;
    }
    return 0;
}
