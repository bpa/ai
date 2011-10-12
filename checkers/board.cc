#include <iostream>
#include "board.h"

using namespace std;

#define NONE     0x00
#define OCCUPIED 0x01
#define KING     0x02
#define BLACK    0x04

void Board::generate_hash() {
	bool even = false;
	char val;
	char *pos = (char*)&hash;
	for (int y=0; y<8; y++) {
		for (int x=0; x<8; x++) {
			if(x % 2 != y % 2) {
				char tile = tiles[y][x];
				if (even) {
					val |= tile;
					*pos = val;
					pos++;
				}
				else {
					val = tile << 4;
				}
				even = !even;
			}
		}
	}
}

Board::Board() {
	int x,y;

	for(y=0; y<8; y++) {
		for(x=0; x<8; x++) {
			tiles[y][x] = 0;
			if(x % 2 != y % 2) {
				if (y<3 || y>4) tiles[y][x] = OCCUPIED;
				if (y>4)        tiles[y][x] |= BLACK;
			}
		}
	}

	generate_hash();
}

void Board::say() {
	for (int y=0; y<8; y++) {
		for (int x=0; x<8; x++) {
			if(x % 2 == y % 2)
				cout << '.';
			else {
				char t = tiles[y][x];
				if ( t == NONE )
					cout << ' ';
				else if ( t & BLACK ) 
					cout << (t & KING ? 'B' : 'b');
				else
					cout << (t & KING ? 'R' : 'r');
			}
		}
		cout << endl;
	}
	cout << hex << hash << endl;
}
