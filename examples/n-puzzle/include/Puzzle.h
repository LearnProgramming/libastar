#ifndef PUZZLE_H
#define PUZZLE_H

#include <iostream>
#include <string>
#include <vector>
#include "Tile.h"

typedef std::vector<Tile>::const_iterator TileIt;

/*
 * A Puzzle is an array of Tiles.
 */
class Puzzle {
    public:
        Puzzle(const std::string& filename);
        Puzzle(Puzzle p, size_t r, size_t c);

        /*
         * Compares the current state to the desired solution state
         * and returns whether or not the solution state can
         * be reached from the current state
         */
        bool might_reach(const Puzzle& solution) const;
        size_t get_width() const;
        void get_neighbors(std::vector<Puzzle>&) const;

    private:
        std::vector<Tile> tiles;
        size_t width;
        size_t open_pos;

        size_t count_inversions() const;

        friend std::ostream& operator<< (std::ostream& o, const Puzzle& p);
        friend bool operator==(const Puzzle&, const Puzzle&);
        friend bool operator!=(const Puzzle&, const Puzzle&);
        friend bool operator<(const Puzzle&, const Puzzle&);
        friend int displaced(const Puzzle&, const Puzzle&);
        friend int manhattan(const Puzzle&, const Puzzle&);
        friend int sumdisman(const Puzzle&, const Puzzle&);
};

#endif // PUZZLE_H

