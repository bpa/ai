#include <iostream>
#include "debug.h"

extern __u32 l[];

ostream &operator<<(ostream &out, const Board &b) {
	out << "Player: " << b.player << endl;
	for (int i=0; i<32; i++) {
		int mask = l[i+1];
		if (i / 4 % 2 == 0) out << ' ';
		     if (mask &  b.red ) out << (mask & b.kings ? 'R' : 'r');
		else if (mask & b.black) out << (mask & b.kings ? 'B' : 'b');
		else                   out << '.';
		if (i / 4 % 2 != 0) out << ' ';
		if (i % 4 == 3) out << endl;
	}
	return out;
}

bool operator< (const Board &a, const Board &b) {
	if (a. red   < b.red)    return true;
	if (a.black  < b.black)  return true;
	if (a.kings  < b.kings)  return true;
	if (a.player < b.player) return true;
	return false;
}

bool operator< (const Move &a, const Move &b) {
	if (a.moves < b.moves) return true;
	for (int i=0; i<a.moves; i++) {
		if (a.tiles[i] < b.tiles[i]) return true;
	}
	return false;
}

ostream& operator<<(ostream &out, const Move &m) {
  return out << m.str();
}
