#ifndef MOVE_H
#define MOVE_H
#include <string>
class Move;
#include "state.h"

using namespace std;

class Move {
public:
	char tiles[13];
	int moves;
	Move();
	Move(const char *);
	void addTile(char);
	void to_string(char *);
};

#endif
