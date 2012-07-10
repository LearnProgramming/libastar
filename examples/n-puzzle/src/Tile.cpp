#include <iostream>
#include "Tile.h"

using std::ostream;

ostream& operator<<(ostream& o, const Tile& t) {
	return o << t.value;
}

bool operator==(const Tile& a, const Tile& b) { return a.value == b.value; }
bool operator!=(const Tile& a, const Tile& b) { return !(a == b); }
bool operator<(const Tile& a, const Tile& b) { return a.value < b.value; }

Tile::Tile(size_t v) : value(v) {}

