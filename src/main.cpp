#include <iostream>
#include <SFML/Graphics.hpp>
#include <thread>
#include <chrono>

#include "main.hpp"


vec2i::vec2i(int x, int y) : x(x), y(y)
{
}

vec2i::vec2i() 
{
}

tetromino::tetromino(matI shape, int x, int y, int spawn_rotation, Color color) :
rotation_index(spawn_rotation),
x(x),
y(y),
spawn_rotation(spawn_rotation),
spawn_x(x),
spawn_y(y),
color(color)
{
    rotations.push_back(shape);
    for (int i = 0; i < 3; i++) 
        rotations.emplace_back(rotate_90(rotations[i]));

    
}

matI tetromino::rotate_90(const matI& mat)
{
    matI ret_mat = 
        {
        {0,0,0,0,0},
        {0,0,0,0,0}, 
        {0,0,0,0,0}, 
        {0,0,0,0,0}, 
        {0,0,0,0,0} 
        };

    for (int y = 0; y < mat.size(); y++)
    {
        for (int x = 0; x < mat[y].size(); x++)
        {
            ret_mat[y][x] = mat[(mat.size() - 1) - x][y];
        }
    }

    return ret_mat;

}

void tetromino::print_mat(const matI& mat)
{
    for (auto vec : mat)
    {
        for (auto entry : vec)
            std::cout << entry << " ";
        std::cout << std::endl;
    }
}

matI& tetromino::get_shape()
{
    return rotations[rotation_index];
}

void tetromino::rotate()
{
    std::cout << "called rotate\n";
    rotation_index = (rotation_index + 1) % 4;
    
}

void tetromino::reset()
{
    x = spawn_x;
    y = spawn_y;
    rotation_index = spawn_rotation;
}

Board::Board() : 
rect(sf::Vector2f(SQUARE_DIM - OUTLINE_SIZE, SQUARE_DIM - OUTLINE_SIZE))
{
    for(int y = 0; y < GRID_HIEGHT; y++) 
    {
        for (int x = 0; x < GRID_WIDTH; x++)
        {
           grid[y][x] = (x == BOARD_OFFSET_X - 1 || x == (BOARD_END_X + 1)) || y == GRID_HIEGHT -1 ? 1 : 0;
        }

    }

    active_tetromino = 0;
    rect.setOutlineColor(sf::Color::Black);
    rect.setOutlineThickness(OUTLINE_SIZE);

    tetrominos.emplace_back(tetromino(assets::shapes::square, 3, -2, 0, Color::YELLOW));
    tetrominos.emplace_back(tetromino(assets::shapes::N, 3, -2, 0, Color::GREEN));
    tetrominos.emplace_back(tetromino(assets::shapes::N_mirrored, 3, -2, 0, Color::RED));
    tetrominos.emplace_back(tetromino(assets::shapes::L, 3, -2, 0, Color::ORANGE));
    tetrominos.emplace_back(tetromino(assets::shapes::L_mirrored, 3, -2, 0, Color::BLUE));
    tetrominos.emplace_back(tetromino(assets::shapes::line, 3, -2, 0, Color::CYAN));
    tetrominos.emplace_back(tetromino(assets::shapes::T, 3, -2, 0, Color:: PURPLE));
}

bool Board::can_move(int target_x, int target_y) 
{
    for (int y = 0; y < 4; y++)
    {
        int y_board_pos = target_y + BOARD_OFFSET_y + y;
        for (int x = 0; x < 4; x++)
        {   
            int x_board_pos = target_x + BOARD_OFFSET_X+ x;
            if (grid[y_board_pos][x_board_pos] && tetrominos[active_tetromino].get_shape()[y][x])
                return false;
        }
    }

    return true;

}

void Board::add_shape_to_board() {

    tetromino& tetro = tetrominos[active_tetromino];

    for (int y = 0; y < 4; y++)
    {
        int y_board_pos = tetro.y + BOARD_OFFSET_y + y;
        for (int x = 0; x < 4; x++)
        {   
            int x_board_pos = tetro.x + BOARD_OFFSET_X + x;
            if (tetro.get_shape()[y][x]) {
                grid[y_board_pos][x_board_pos] = tetro.color;   
            }
        }
    }

    active_tetromino = rand() % 7;
    tetro.reset();
}

bool Board::move_shape_down()
{
    tetromino& tetro = tetrominos[active_tetromino];

    if (can_move(tetro.x, tetro.y + 1))
    {
        tetro.y++;
        return false;
    }
    else
    {
        add_shape_to_board();
        return true;
    } 
}
void Board::move_shape_left()
{
    tetromino& tetro = tetrominos[active_tetromino];
    if (can_move(tetro.x - 1, tetro.y))
        tetro.x--;
}

void Board::move_shape_right()
{
    tetromino& tetro = tetrominos[active_tetromino];
    if (can_move(tetro.x + 1, tetro.y))
        tetro.x++;
}

void Board::draw(sf::RenderWindow& window)
{
    rect.setFillColor(sf::Color(211,211,211));

    for (int y = 0; y < BOARD_HEIGHT; y++) 
    {
        for (int x = 0; x < BOARD_WIDTH; x++)
        {
            Color color = (Color) grid[y+BOARD_OFFSET_y][x+BOARD_OFFSET_X];
            rect.setFillColor(color_map.at(color));

            rect.setPosition(x*SQUARE_DIM + OUTLINE_SIZE, 
                             y*SQUARE_DIM + OUTLINE_SIZE);
            window.draw(rect);
        }
    }


    tetromino& tetro = tetrominos[active_tetromino];
    rect.setFillColor(color_map.at(tetro.color));
    for (int y = 0; y < 4; y++)
    {
        int y_board_pos = tetro.y + y;
        for (int x = 0; x < 4; x++)
        {   
        int x_board_pos = tetro.x + x;
            if (tetro.get_shape()[y][x])
            {
                rect.setPosition(x_board_pos*SQUARE_DIM + OUTLINE_SIZE,
                                 y_board_pos*SQUARE_DIM + OUTLINE_SIZE);
                window.draw(rect);
            }
        }
    }

}

void Board::print()
{
    for (int y = 0; y < BOARD_HEIGHT + 1 + 5; y++) 
    {
        for (int x = 0; x < BOARD_WIDTH + 2 + 10; x++)
        {
            std::cout << grid[y][x] << ' ';
        } 
        std::cout << std::endl;
    }
}


int main()
{   
    Board board;
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Tetris");
    board.print();
    while (window.isOpen())
    {
        bool collision;
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed) 
            {
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                    board.move_shape_right();
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
                    board.move_shape_left();
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                    if (board.move_shape_down()){
                    }
                }
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
                    board.tetrominos[board.active_tetromino].rotate();
            }
        }


        window.clear();
        board.draw(window);
        window.display();
    }



}