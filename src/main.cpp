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

tetromino::tetromino(matI shape)
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


Board::Board() : 
rect(sf::Vector2f(SQUARE_DIM - OUTLINE_SIZE, SQUARE_DIM - OUTLINE_SIZE))
{
    for(int y = 0; y < BOARD_HEIGHT + 1; y++) 
    {
        for (int x = 0; x < BOARD_WIDTH + 1; x++)
        {
            board[y][x] = x == 0 || x == BOARD_WIDTH || y == BOARD_HEIGHT ? 1 : 0;
        }

    }
    rect.setOutlineColor(sf::Color::Black);
    rect.setOutlineThickness(OUTLINE_SIZE);
}

void Board::draw(sf::RenderWindow& window)
{
    rect.setFillColor(sf::Color::Blue);
    for (int y = 0; y < BOARD_HEIGHT; y++) 
    {
        for (int x = 0; x < BOARD_WIDTH; x++)
        {
            rect.setPosition(x*SQUARE_DIM + OUTLINE_SIZE, 
                             y*SQUARE_DIM + OUTLINE_SIZE);
            window.draw(rect);
        }
    }

    rect.setFillColor(sf::Color::Red);
    for (int y = 0; y < 4; y++)
    {
        int y_board_pos = shape->y + y;
        for (int x = 0; x < 4; x++)
        {   
        int x_board_pos = shape->x + x;
            if (shape->get_shape()[y][x])
            {
                rect.setPosition(x_board_pos*SQUARE_DIM + OUTLINE_SIZE,
                                 y_board_pos*SQUARE_DIM + OUTLINE_SIZE);
                window.draw(rect);
            }
        }
    }

}


int main()
{   
    Board board;
    tetromino* shape = new tetromino(assets::shapes::square);
    shape->x = 5;
    shape->y = 5;
    board.shape = shape;
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Tetris");
    tetromino::print_mat(shape->rotations[0]);
    std::cout << std::endl;
    tetromino::print_mat(shape->rotations[1]);
    std::cout << std::endl;
    tetromino::print_mat(shape->rotations[2]);
    std::cout << std::endl;
    tetromino::print_mat(shape->rotations[3]);
    std::cout << std::endl;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed) 
            {
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                    shape->x++;
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
                    shape->x--;
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
                    shape->y++;
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
                    shape->rotate();
            }
        }

        window.clear();
        board.draw(window);
        window.display();
    }



}