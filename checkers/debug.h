#ifndef DEBUG_H
#define DEBUG_H
#include "board.h"

ostream &operator<< (ostream &, const Board &);
ostream &operator<< (ostream &, const Move &);
bool operator< (const Board &, const Board &);
bool operator< (const Move &, const Move &);

#endif
