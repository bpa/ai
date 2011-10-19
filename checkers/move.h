#ifndef MOVE_H
#define MOVE_H
#include <string>

using namespace std;

class Move {
public:
	char tiles[13];
	int moves;
	Move();
	Move(const char *);
	void addTile(char);
	const char *str() const;
	bool operator< (const Move &) const;
	int cmp(const Move *) const;
};

ostream& operator<< (ostream& out, const Move &);

#endif
