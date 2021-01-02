#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "../include/algorithm.h"

using namespace std;

/******************************************************
 * In your algorithm, you can just use the the funcitons
 * listed by TA to get the board information.(functions 
 * 1. ~ 4. are listed in next block)
 * 
 * The STL library functions is not allowed to use.
******************************************************/

/*************************************************************************
 * 1. int board.get_orbs_num(int row_index, int col_index)
 * 2. int board.get_capacity(int row_index, int col_index)
 * 3. char board.get_cell_color(int row_index, int col_index)
 * 4. void board.print_current_board(int row_index, int col_index, int round)
 * 
 * 1. The function that return the number of orbs in cell(row, col)
 * 2. The function that return the orb capacity of the cell(row, col)
 * 3. The function that return the color fo the cell(row, col)
 * 4. The function that print out the current board statement
*************************************************************************/



void safe_for_chase() {

}

void series_boom() {

}

void need_explode_now(Board board, int *opponent, int *my, int *position) {
    int row, col;
    int myrow, mycol;
    row = (opponent[0] % 100) / 10;
    col = opponent[0] % 10;
    myrow = (my[0] % 100) / 10;
    mycol = my[0] % 10;

    for (int i = 0; i < 30; i++) {
        if (my[i] % 100 == 1 && (((my[i] % 100) / 10 ) <= 1) && ((my[i] % 10) <= 1)) {
            position[1] = (my[i] % 100) / 10;
            position[0] = my[i] % 10;
        }
    }
}

void algorithm_A(Board board, Player player, int index[]){

    //////your algorithm design///////////
    int last_position[1];
    bool find;
    int row, col;
    int position[1] = {0};
    int color = player.get_color();
    char my_color;
    char opponent_color;
    int advt[5][6] = {0};
    int best_advt = 0;
    int best_row = 0;
    int best_col = 0;
    int opponent_list[30] = {0};
    int opponent_num = 0;
    int my_list[30] = {0};
    int my_num = 0;
    int empty[30] = {0};
    int empty_num = 0;
    int limit[5][6] = {0};
    int i, key, j;  
    int curr;

    last_position[1] = index[1];
    last_position[0] = index[0];

    cout << "index now " << index[1] << " " << index[0]  << endl;
    for (int i = 0; i < 5; i++ ) {                                      // 針對空白棋盤
        for(int j = 0; j < 6; j++) {
            if (board.get_cell_color(i, j) == 'w') {
                if (i == 2 && j == 2) advt[i][j] = 8;                   // mid 2 (8)
                else if (i == 2 && j == 3) advt[i][j] = 8;
                else if (i == 0 && j == 0) advt[i][j] = 7;              // corner 4
                else if (i == 0 && j == 5) advt[i][j] = 7;
                else if (i == 4 && j == 0) advt[i][j] = 7;
                else if (i == 4 && j == 5) advt[i][j] = 7;
                else if (i == 0 && j == 2) advt[i][j] = 4;              // top slide and down slide (0,2) (0,3) (4,2) (4,3)
                else if (i == 0 && j == 3) advt[i][j] = 4;
                else if (i == 4 && j == 2) advt[i][j] = 4;
                else if (i == 4 && j == 3) advt[i][j] = 4;
                else if (i == 2 && j == 0) advt[i][j] = 3;              // 側邊 (2,0) (2,5)
                else if (i == 2 && j == 5) advt[i][j] = 3;
                else advt[i][j] = 2;
            } else if (board.get_cell_color(i, j) == color) {
                advt[i][j] = 5;                                         // my placement
            } else advt[i][j] = 1;                                      // opponent's placement
        }
    }
    
    if (color == 'r') {
        my_color = 'r';
        opponent_color = 'b';
    } else {
        my_color = 'b';
        opponent_color = 'r';
    }

    for (int i = 0; i < 5; i++ ) {                                                                  // 每一個放入個別的list
        for (int j = 0; j < 6; j++) {
            limit[i][j] = board.get_capacity(i, j) - board.get_orbs_num(i, j);                      // 距離爆炸剩下幾個
            
            if (board.get_cell_color(i, j) == 'w') {
                empty[empty_num++] = advt[i][j] * 1000 + limit[i][j] * 100 + i * 10 + j * 1;            // empty's list and its
            } else if (board.get_cell_color(i, j) == color) {
                my_list[my_num++] = limit[i][j] * 100 + i * 10 + j * 1;                         // my list and its
            } else {
                // cout << "line 112 "<< endl;
                opponent_list[opponent_num++] = limit[i][j] * 100 + i * 10 + j * 1;             // opponent's list and its 
            }
        }
    }

    for (i = 1; i < 30 && empty[i] != 0; i++)                               // insertion sort for empty_list
    {  
        key = empty[i] / 1000;
        curr = empty[i];
        
        j = i - 1;  
    
        /* Move elements of arr[0..i-1], that are  
        greater than key, to one position ahead  
        of their current position */
        while (j >= 0 && (empty[j] / 1000) > key) 
        {  
            empty[j + 1]  = empty[j];
            j = j - 1;
        }  
        empty[j + 1] = key * 1000 + curr % 1000;  
        // cout << ";ine 138 " << endl;
    }  
    for (i = 1; i < 30 && my_list[i] != 0; i++)                             // insertion sort for my_list
    {  
        key = my_list[i] / 100;
        curr = my_list[i];
        
        j = i - 1;  
    
        /* Move elements of arr[0..i-1], that are  
        greater than key, to one position ahead  
        of their current position */
        while (j >= 0 && empty[j] / 100 > key) 
        {  
            my_list[j + 1]  = my_list[j];
            j = j - 1;
        }  
        my_list[j + 1] = key * 100 + curr % 100;  
    } 
    for (i = 1; i < 30 && opponent_list[i] != 0; i++)                       // insertion sort for opponent_list
    {  
        key = opponent_list[i] / 100;
        curr = opponent_list[i];
        
        j = i - 1;  
    
        /* Move elements of arr[0..i-1], that are  
        greater than key, to one position ahead  
        of their current position */
        while (j >= 0 && opponent_list[j] / 100 > key) 
        {  
            opponent_list[j + 1]  = opponent_list[j];
            j = j - 1;
        }  
        opponent_list[j + 1] = key * 100 + curr % 100;  
    } 

    if ( (opponent_list[0] / 100) == 1 && !find) {                                            // about to explode, we need to explode it now
        need_explode_now(board, opponent_list, my_list, position);
        index[0] = position[0];
        index[1] = position[1];
        find = 1;
    }

    if (last_position[1] == 0 && !find ) {  // check last position
        if(last_position[0] == 0) {

        }

    }
    // cout << "line 174" << endl;
    for(int i = 0; i < 30 && !find; i++) {                                  // choose a best place in empty_list
        row = empty[i] % 10 ;
        col = (empty[i] % 100 ) / 10;

        empty[i] = empty[i] % 1000 + 0;
        index[1] = row;
        index[0] = col;
        find = 1;    
    }
    

    
    // cout << "index[1][0] = " << index[0] << index[1] << endl;

}