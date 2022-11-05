#pragma once

#include <assets.hpp>
#include <unordered_map>


enum Color
{
    CYAN = 10,
    YELLOW,
    PURPLE,
    ORANGE,
    BLUE,
    GREEN,
    RED,
    GREY = 0
};

struct vec2i {
    int x;
    int y;
    vec2i(int x, int y);
    vec2i();
};

class tetromino
{
public:
    tetromino(matI shape, int x, int y, int spawn_rotation, Color color);
    Color color;
    int spawn_x;
    int spawn_y;
    int spawn_rotation;
    int x;
    int y;
    int rotation_index{0};
    std::vector<matI> rotations;
    static matI rotate_90(const matI& mat);
    static void print_mat(const matI& mat);
    matI& get_shape();
    void rotate();
    void reset();
private:

};

class Board
{   
public:
    Board();
    const std::unordered_map<Color, sf::Color> color_map 
    {
        {Color::BLUE, sf::Color::Blue},
        {Color::RED, sf::Color::Red},
        {Color::GREEN, sf::Color::Green},
        {Color::PURPLE, sf::Color(139,0,139)},
        {Color::YELLOW, sf::Color::Yellow},
        {Color::CYAN, sf::Color(0, 100, 100)},
        {Color::ORANGE, sf::Color(255,165,0)},
        {Color::GREY, sf::Color(211,211,211)}

    };
    int active_tetromino;
    int grid[GRID_HIEGHT][GRID_WIDTH];
    sf::RectangleShape rect;
    std::vector<tetromino> tetrominos;
    void draw(sf::RenderWindow& window);
    bool can_move(int target_x, int target_y);
    bool move_shape_down();
    void move_shape_left();
    void move_shape_right();
    void add_shape_to_board();

    void print();
};
