#include <stdlib.h>
#include "choice.h"

Move *choice_random(Board *board) {
	int i;
	i = rand() % board->children.size();
	return &board->children.at(i)->move;
}
