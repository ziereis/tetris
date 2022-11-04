#pragma once

#include <assets.hpp>

struct vec2i {
    int x;
    int y;
    vec2i(int x, int y);
    vec2i();
};

class tetromino
{
public:
    tetromino(matI shape);
    int x;
    int y;
    int rotation_index{0};
    std::vector<matI> rotations;
    static matI rotate_90(const matI& mat);
    static void print_mat(const matI& mat);
    matI& get_shape();
    void rotate();
private:

};

class Board
{   
public:
    Board();
    int board[BOARD_HEIGHT+1][BOARD_WIDTH+1];
    sf::RectangleShape rect;
    tetromino* shape;
    void draw(sf::RenderWindow& window);
    bool can_move(tetromino shape, int x, int y);
};
