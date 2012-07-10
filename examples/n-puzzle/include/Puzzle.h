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

        /*
         * Compares the current state to the desired solution state
         * and returns whether or not the solution state can
         * be reached from the current state
         */
        bool might_reach(const Puzzle& solution) const;
        size_t get_width() const;
        std::vector<Puzzle> get_neighbors() const;

    private:
        std::vector<Tile> tiles;
        size_t open_pos;
        size_t width;

        size_t count_inversions() const;
        void add_puzzle(std::vector<Puzzle>& puzzles, size_t row, size_t col) const;
        void move_tile(size_t row, size_t col);

        friend std::ostream& operator<< (std::ostream& o, const Puzzle& p);
        friend bool operator==(const Puzzle&, const Puzzle&);
        friend bool operator!=(const Puzzle&, const Puzzle&);
        friend bool operator<(const Puzzle&, const Puzzle&);
        friend int displaced(const Puzzle&, const Puzzle&);
        friend int manhattan(const Puzzle&, const Puzzle&);
        friend int sumdisman(const Puzzle&, const Puzzle&);
};

#endif // PUZZLE_H

