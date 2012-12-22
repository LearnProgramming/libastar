#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <sstream>
#include <vector>
#include "Puzzle.h"
#include "Tile.h"

using std::ifstream;
using std::istringstream;
using std::ostream;
using std::string;
using std::swap;
using std::vector;

ostream& operator<< (ostream& o, const Puzzle& p) {
    for(size_t i = 0; i < p.tiles.size(); ++i) {
        o << p.tiles[i] << (((i + 1) % p.width) ? ' ' : '\n');
    }
    return o;
}

bool operator==(const Puzzle&a, const Puzzle& b) { return a.tiles == b.tiles; }
bool operator!=(const Puzzle&a, const Puzzle& b) { return !(a == b); }
bool operator<(const Puzzle& a, const Puzzle& b) {
    TileIt ait = a.tiles.begin(), bit = b.tiles.begin();
    while( ait != a.tiles.end() && bit != b.tiles.end() ) {
        if(*ait != *bit) {
            return *ait < *bit;
        }
        ait++, bit++;
    }
    return ait == a.tiles.end() && bit != b.tiles.end();
}


/*
 * There is some basic checking for safety, namely that the
 * file exists and can be opened, that the puzzle is square,
 * and that there is only one blank space.
 * It does not check uniqueness of the values.
 *
 * TODO: Support arbitrary rectangular puzzles
 */
Puzzle::Puzzle(const string& filename) {
    string line;
    ifstream in(filename.c_str());

    // Try to open the file and read the inital state
    if(in.is_open()) {
        getline(in, line);
        in.close();
    }
    else {
        throw "Unable to open file: " + filename + '\n';
    }

    size_t tile = 0, blanks = 0, num = 0, root = 1, square = 1;

    istringstream linestream(line);

    while(linestream >> tile) {
        tiles.push_back(tile);

        if(num == square) {
            width = ++root;
            square = root * root;
        }
        if(tile == 0) {
            open_pos = num;
            blanks++;
        }

        num++;
    }

    if(num != square) {
        throw "Puzzle is not square.";
    }

    if(blanks != 1) {
        throw "There must be exactly one blank tile.";
    }
}

size_t Puzzle::get_width() const { return width; }

vector<Puzzle> Puzzle::get_neighbors() const {
    vector<Puzzle> puzzles;

    size_t r0 = open_pos / width, c0 = open_pos % width;
    size_t row = r0 - 1, col = c0;

    if(row < r0) {
        add_puzzle(puzzles, row, col);
    }

    row = r0 + 1;
    if(row < width) {
        add_puzzle(puzzles, row, col);
    }

    row = r0;
    col = c0 - 1;

    if(col < c0) {
        add_puzzle(puzzles, row, col);
    }

    col = c0 + 1;
    if(col < width) {
        add_puzzle(puzzles, row, col);
    }

    return puzzles;
}

void Puzzle::add_puzzle(vector<Puzzle>& puzzles, size_t row, size_t col) const {
        Puzzle p(*this);
        p.move_tile(row, col);
        puzzles.push_back(p);
}

void Puzzle::move_tile(size_t row, size_t col) {
    size_t new_open_pos = row * width + col;
    swap(tiles[open_pos], tiles[new_open_pos]);
    open_pos = new_open_pos;
}

/**
 * This works by comparing the polarity of the current state to the polarity of the solution state. If they are equal then the solution can be reached.
 * Details can be found at http://www.cs.bham.ac.uk/~mdr/teaching/modules04/java2/TilesSolvability.html
 *
 * This is currently an O(n^2) operation, which is fine for small n, and may even be better than trying to solve the Puzzle and failing when n is large.
 *
 * TODO: See if there's an O(n) implementation
 */
bool Puzzle::might_reach(const Puzzle& p) const {
    if (width != p.width) {
        return false;
    }

    size_t inversions = count_inversions(), p_inversions = p.count_inversions();

    bool odd_width = width % 2;

    bool even_inversions = !(inversions % 2);
    bool odd_row_from_bottom = !((open_pos / width) % 2);

    bool p_even_inversions = !(p_inversions % 2);
    bool p_odd_row_from_bottom = !((p.open_pos / width) % 2);

    bool polarity = (odd_width && even_inversions) || (!odd_width && (odd_row_from_bottom == even_inversions));
    bool p_polarity = (odd_width && p_even_inversions) || (!odd_width && (p_odd_row_from_bottom == p_even_inversions));

    return polarity == p_polarity;
}

size_t Puzzle::count_inversions() const {
    size_t inversions = 0;

    for(TileIt it = tiles.begin(); it != tiles.end(); it++) {
        if(*it != 0) {
            for(TileIt it1 = it + 1; it1 != tiles.end(); it1++) {
                if(*it1 != 0 && *it1 < *it) {
                    inversions++;
                }
            }
        }
    }

    return inversions;
}

