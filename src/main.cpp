#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <iostream>
#include <string>

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

    active_tetromino = rand() % 7;
    rect.setOutlineColor(sf::Color(211,211,211, 50));
    rect.setOutlineThickness(OUTLINE_SIZE);

    tetrominos.emplace_back(tetromino(assets::shapes::square, 2, -2, 0, Color::YELLOW));
    tetrominos.emplace_back(tetromino(assets::shapes::N, 3, -2, 1, Color::GREEN));
    tetrominos.emplace_back(tetromino(assets::shapes::N_mirrored, 3, -2, 1, Color::RED));
    tetrominos.emplace_back(tetromino(assets::shapes::L, 3, -2, 1, Color::ORANGE));
    tetrominos.emplace_back(tetromino(assets::shapes::L_mirrored, 3, -2, 3, Color::BLUE));
    tetrominos.emplace_back(tetromino(assets::shapes::line, 3, -2, 1, Color::CYAN));
    tetrominos.emplace_back(tetromino(assets::shapes::T, 3, -2, 2, Color:: PURPLE));
}

bool Board::can_move(int target_x, int target_y) 
{
    for (int y = 0; y < 5; y++)
    {
        int y_board_pos = target_y + BOARD_OFFSET_y + y;
        for (int x = 0; x < 5; x++)
        {   
            int x_board_pos = target_x + BOARD_OFFSET_X+ x;
            if (grid[y_board_pos][x_board_pos] && tetrominos[active_tetromino].get_shape()[y][x])
                return false;
        }
    }

    return true;

}

void Board::rotate_shape()
{
    std::cout << "called rotate\n";
    int rotation_index = (tetrominos[active_tetromino].rotation_index + 1) % 4;
    if (can_rotate(rotation_index))
        tetrominos[active_tetromino].rotation_index = rotation_index;
}

bool Board::can_rotate(int rotation)
{
    const tetromino& tetro = tetrominos[active_tetromino];

    for (int y = 0; y < 5; y++)
    {
        int y_board_pos = tetro.y + BOARD_OFFSET_y + y;
        for (int x = 0; x < 5; x++)
        {   
            int x_board_pos = tetro.x + BOARD_OFFSET_X + x;
            if (grid[y_board_pos][x_board_pos] && tetro.rotations[rotation][y][x])
                return false;
        }
    }

    return true;
}

void Board::add_shape_to_board() {

    tetromino& tetro = tetrominos[active_tetromino];

    for (int y = 0; y < 5; y++)
    {
        int y_board_pos = tetro.y + BOARD_OFFSET_y + y;
        for (int x = 0; x < 5; x++)
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

void Board::clear_lines()
{
    int lines{};
    for (int y = BOARD_OFFSET_y; y < GRID_HIEGHT-1; y++) 
    {
        if (is_line(grid[y]))
        {
            std:: cout << "line\n";
            for(int y_shift = y - 1; y_shift > BOARD_OFFSET_y; y_shift--) {
                for (int x = BOARD_OFFSET_X; x < BOARD_END_X + 1; x++) {
                    grid[y_shift + 1][x] = grid[y_shift][x]; 
                    std::cout << "\n\n\n";
                    print();
                }
            }
        ++lines;
        ++m_lines;
        if ((m_lines +1) % 11 == 0)
        {
            ++m_level;
            m_speed = m_speed/2;
        }
        }
    }


    switch(lines)
    {
        case 1:
            std::cout << "line\n";
            m_score+=40;
            break;
        case 2:
            m_score+=100;
            break;
        case 3:
            m_score+=300;
            break;
        case 4:
            m_score+= 1200;
            break;
    }

}

bool Board::is_line(int row[GRID_WIDTH])
{
    for (int x = BOARD_OFFSET_X; x < BOARD_END_X + 1; x++)
        if (row[x] == 0) return false;
    return true;


} 

void Board::is_game_over()
{
    for (int x = BOARD_OFFSET_X; x < BOARD_END_X; x++) {
        if (grid[BOARD_OFFSET_y][x] > 0) 
            game_over = true;
    }
}

void Board::draw(sf::RenderWindow& window)
{

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
    for (int y = 0; y < 5; y++)
    {
        int y_board_pos = tetro.y + y;
        for (int x = 0; x < 5; x++)
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

void main_game(sf::RenderWindow& window)
{
    Board board;

    sf::Font font;

    if(!font.loadFromFile("arial.ttf"));
        std::cout << "couldnt load font\n";

    sf::Text score;
    score.setFont(font);
    score.setCharacterSize(24);
    score.setPosition(450, 50);
    score.setStyle(sf::Text::Bold);

    sf::Text level;
    level.setFont(font);
    level.setCharacterSize(24);
    level.setPosition(450, 100);
    level.setStyle(sf::Text::Bold);

    sf::Text lines;
    lines.setFont(font);
    lines.setCharacterSize(24);
    lines.setPosition(450, 150);
    lines.setStyle(sf::Text::Bold);

    sf::Clock clock;

    while (!board.game_over)
    {
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
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) 
                    board.move_shape_down();
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
                    board.rotate_shape();
            }
        }

        if (clock.getElapsedTime().asMilliseconds() > board.m_speed) 
        {
            board.move_shape_down();
            clock.restart();
        }

        score.setString("Score: " + std::to_string(board.m_score));
        level.setString("Level: " + std::to_string(board.m_level));
        lines.setString("Lines: " + std::to_string(board.m_lines));

        window.clear();
        board.is_game_over();
        board.clear_lines();
        board.draw(window);
        window.draw(score);
        window.draw(level);
        window.draw(lines);
        window.display();
    }
    

}

void after_game(sf::RenderWindow& window)
{
    sf::Font font;

    if(!font.loadFromFile("arial.ttf"));
        std::cout << "couldnt load font\n";

    sf::Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(24);
    gameOverText.setPosition((WINDOW_WIDTH-250)/ 2, WINDOW_HEIGHT/2);
    gameOverText.setStyle(sf::Text::Bold);
    gameOverText.setString("Game Over, Pess Enter to play again");
    bool flag = true;
    while(flag)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed) 
            {
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
                    flag = false;
            }
        }
        window.clear();
        window.draw(gameOverText);
        window.display();

    }

}


int main()
{

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH + 200, WINDOW_HEIGHT), "Tetris");
    while (window.isOpen())
    {
        main_game(window);
        after_game(window);
    }
}