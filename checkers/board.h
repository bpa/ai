#ifndef BOARD_H
#define BOARD_H

typedef char Board[8][8];

void board_setup(Board *);
long int board_hash(Board *);
int board_evaluate(Board *);

#endif
