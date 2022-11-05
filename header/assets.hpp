#pragma once
#include <vector>

using matI = std::vector<std::vector<int>>;
namespace 
{
    const int BOARD_WIDTH = 10;
    const int BOARD_HEIGHT = 20;
    const int GRID_WIDTH = 10 + 2 + 5 + 5;
    const int GRID_HIEGHT = 20 + 1 + 5;
    const int BOARD_OFFSET_X = 6;
    const int BOARD_END_X = 15;
    const int BOARD_OFFSET_y = 5;
    const int BOARD_END_Y = 20;
    const int SQUARE_DIM = 40;
    const int OUTLINE_SIZE = 2;
    const int WINDOW_HEIGHT = BOARD_HEIGHT * SQUARE_DIM + OUTLINE_SIZE;
    const int WINDOW_WIDTH = BOARD_WIDTH * SQUARE_DIM + OUTLINE_SIZE;
}

namespace assets
{
    namespace shapes
    {
        matI square = 
            {
            {0,0,0,0,0},
            {0,0,0,0,0}, 
            {0,0,1,1,0}, 
            {0,0,1,1,0}, 
            {0,0,0,0,0} 
            };

        matI L = 
            {
            {0,0,0,0,0},
            {0,0,1,0,0}, 
            {0,0,1,0,0}, 
            {0,0,1,1,0}, 
            {0,0,0,0,0} 
            };

        matI L_mirrored = 
            {
            {0,0,0,0,0},
            {0,0,1,0,0}, 
            {0,0,1,0,0}, 
            {0,1,1,0,0}, 
            {0,0,0,0,0} 
            };

        matI line = 
            {
            {0,0,0,0,0},
            {0,0,1,0,0}, 
            {0,0,1,0,0}, 
            {0,0,1,0,0}, 
            {0,0,1,0,0} 
            };

        matI N = 
            {
            {0,0,0,0,0},
            {0,0,0,1,0}, 
            {0,0,1,1,0}, 
            {0,0,1,0,0}, 
            {0,0,0,0,0} 
            };

        matI N_mirrored = 
            {
            {0,0,0,0,0},
            {0,0,1,0,0}, 
            {0,0,1,1,0}, 
            {0,0,0,1,0}, 
            {0,0,0,0,0} 
            };

        matI T = 
            {
            {0,0,0,0,0},
            {0,0,1,0,0}, 
            {0,1,1,1,0}, 
            {0,0,0,0,0}, 
            {0,0,0,0,0} 
            };
    }
};
