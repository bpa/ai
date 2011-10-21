#ifndef DEBUG_H
#define DEBUG_H
#include <iostream>
#include "board.h"

using namespace std;

ostream &operator<< (ostream &, const Board &);
ostream &operator<< (ostream &, const Move &);
bool operator< (const Board &, const Board &);
bool operator< (const Move &, const Move &);

#endif
