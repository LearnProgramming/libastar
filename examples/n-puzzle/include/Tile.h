#ifndef TILE_H
#define TILE_H

#include <iostream>
/*
 * The Tile class represents a tile in a Puzzle.
 * It's not strictly necessary but provides a better implementation in that it's more conceptually accurate.
 */
class Tile {
    public:
        Tile(size_t value = 0);

    private:
        size_t value;

        friend std::ostream& operator<<(std::ostream& o, const Tile& t);
        friend bool operator== (const Tile&, const Tile&);
        friend bool operator!= (const Tile&, const Tile&);
        friend bool operator< (const Tile&, const Tile&);
};

#endif // TILE_H

