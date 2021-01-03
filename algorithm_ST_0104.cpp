#include <iostream>
// #include <fstream> // test only
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
// ofstream fileout2("out2.data");

int isSafe(Board board, char opponentColor, int row, int col, bool turn)    // not safe => 0
{                                                                           // safe and no opponent at neighborhood => 1
    int pos = row * 6 + col;                                                // safe, i my limit orband more than one opponent at neighborhood => 2 + i
    int opponent_count = 0;
    int my_limit_count = 0;
    bool my_orbit_beside = false;
    for (int diff = -7; diff < 8; diff++)                                   // check its neighborhood cell
    {
        bool out_of_range = false;

        if (diff == -4)                                                     // 左邊
            diff = -1;
        else if (diff == 0)
            diff = 1;
        else if (diff == 2)                                                 // 左下
            diff = 5;

        if (row == 0 && diff >= -7 && diff <= -5)                           // check out_of_range or not
            out_of_range = true;
        else if (row == 4 && diff >= 5 && diff <= 7)
            out_of_range = true;
        if (col == 0 && (diff == -7 || diff == -1 || diff == 5))
            out_of_range = true;
        else if (col == 5 && (diff == -5 || diff == 1 || diff == 7))
            out_of_range = true;

        if (!out_of_range)
        {
            int LookRow = (pos + diff) / 6;                                 // get the place
            int LookCol = (pos + diff) % 6;                                 // get the place
            int orb_diff_Look = board.get_capacity(LookRow, LookCol) - board.get_orbs_num(LookRow, LookCol);    // check how many to left to explode
            int orb_diff = board.get_capacity(row, col) - board.get_orbs_num(row, col);     // check now place and near place, if need to chase it
            if (board.get_cell_color(LookRow, LookCol) == opponentColor)    // if opponent's cell
            {
                if ((orb_diff_Look < orb_diff) && turn || (orb_diff_Look <= orb_diff) && !turn) // is safe
                    return 0;
                else
                    opponent_count++;
            }
            if (board.get_cell_color(LookRow, LookCol) != 'w')              // my orbit
            {
                my_orbit_beside = true;
                if (orb_diff_Look == 1)
                    my_limit_count++;
            }
        }
    }
    if (opponent_count == 0)
        return 1;
    else if (my_orbit_beside)
        return 2 + my_limit_count;
    return -1;                                                              // opponent at neighborhood and no my orbit
}
int willexplosive(Board board, char Color, int row, int col)
{
    int orb_diff = board.get_capacity(row, col) - board.get_orbs_num(row, col);     // near to explode
    int pos = row * 6 + col;                                                        // 2D -> 1D array
    int opponent_limit_count = 0;

    for (int diff = -7; diff < 8; diff++)                                   // check its neighborhood cell
    {
        bool out_of_range = false;

        if (diff == -4)
            diff = -1;
        else if (diff == 2)
            diff = 5;

        if (row == 0 && diff >= -7 && diff <= -5)
            out_of_range = true;
        else if (row == 4 && diff >= 5 && diff <= 7)
            out_of_range = true;
        if (col == 0 && (diff == -7 || diff == -1 || diff == 5))
            out_of_range = true;
        else if (col == 5 && (diff == -5 || diff == 1 || diff == 7))
            out_of_range = true;

        if (!out_of_range)
        {
            int LookRow = (pos + diff) / 6;                                 // LookRow is now we check cell's row, neighborhood to now cell
            int LookCol = (pos + diff) % 6;                                 // LookCOl is now we check cell's COl, neighborhood to now cell
            int orb_diff_Look = board.get_capacity(LookRow, LookCol) - board.get_orbs_num(LookRow, LookCol);
            if (diff == 0 && orb_diff_Look == 1)
                return diff;
            if (board.get_cell_color(LookRow, LookCol) == Color && orb_diff_Look == 1)
                opponent_limit_count++;
            if ((opponent_limit_count == orb_diff)) // my orbit
                return diff;
        }
    }
    return -100;
}
int diff_to_explosive(Board board, char Color, int row, int col)                // chain reaction
{
    int orb_diff = board.get_capacity(row, col) - board.get_orbs_num(row, col);
    int pos = row * 6 + col;
    int opponent_limit_count = 0;

    for (int diff = -7; diff < 8; diff++)
    {
        bool out_of_range = false;

        if (diff == -4)
            diff = -1;
        else if (diff == 0)
            diff = 1;
        else if (diff == 2)
            diff = 5;

        if (row == 0 && diff >= -7 && diff <= -5)
            out_of_range = true;
        else if (row == 4 && diff >= 5 && diff <= 7)
            out_of_range = true;
        if (col == 0 && (diff == -7 || diff == -1 || diff == 5))
            out_of_range = true;
        else if (col == 5 && (diff == -5 || diff == 1 || diff == 7))
            out_of_range = true;

        if (!out_of_range)
        {
            int LookRow = (pos + diff) / 6;
            int LookCol = (pos + diff) % 6;
            int orb_diff_Look = board.get_capacity(LookRow, LookCol) - board.get_orbs_num(LookRow, LookCol);
            if (board.get_cell_color(LookRow, LookCol) == Color && orb_diff_Look == 1)      // 旁邊有我的顏色且即將可以爆
                orb_diff--;
        }
    }
    if (orb_diff != board.get_capacity(row, col) - board.get_orbs_num(row, col))
        orb_diff++;
    return orb_diff;
}
void algorithm_A(Board board, Player player, int index[])
{
    //futureBoard currentBoard(board, player);
    //currentBoard.findBestMove(board, index);
    int place_row, place_col;
    int i, j, k, row, col, orb_diff;
    int opponent_row, opponent_col, opponent_pos;
    char color = player.get_color(), opponentColor;
    int opponent_limit[30];
    int my_limit[30];
    int empty[30];
    bool found = false, out_of_range;
    int check = 0; //test only
    int opponentSize = 0, mySize = 0, emptySize = 0;
    bool stored = false;
    int stored_row, stored_col;

    static int round = -1; //test only

    round += 2;
    for (i = 0; i < 5; i++)
        for (j = 0; j < 6; j++)
        {
            int orb_diff = board.get_capacity(i, j) - board.get_orbs_num(i, j);
            if (board.get_cell_color(i, j) == 'w')
                empty[emptySize++] = 30 * 30 * orb_diff + 30 * i + j;
            else if (board.get_cell_color(i, j) == color)
                my_limit[mySize++] = 30 * 30 * orb_diff + 30 * i + j;
            else
                opponent_limit[opponentSize++] = 30 * 30 * orb_diff + 30 * i + j;
        }
    if (color == 'r')
        opponentColor = 'b';
    else if (color == 'b')
        opponentColor = 'r';

    //InsertionSort(opponent_limit, opponentSize); // insert sorting to find the orbit which is closest to the capacity
    for (int i = 1; i < opponentSize; i++)
    {
        int key = opponent_limit[i];
        int j = i - 1;
        while ((key / 900) < (opponent_limit[j] / 900) && j >= 0)
        {
            opponent_limit[j + 1] = opponent_limit[j];
            j--;
        }
        opponent_limit[j + 1] = key;
    }
    //InsertionSort(my_limit, mySize);
    for (int i = 1; i < mySize; i++)
    {
        int key = my_limit[i];
        int j = i - 1;
        while ((key / 900) < (my_limit[j] / 900) && j >= 0)
        {
            my_limit[j + 1] = my_limit[j];
            j--;
        }
        my_limit[j + 1] = key;
    }
    //InsertionSort(empty, emptySize);
    for (int i = 1; i < emptySize; i++)
    {
        int key = empty[i];
        int j = i - 1;
        while ((key / 900) < (empty[j] / 900) && j >= 0)
        {
            empty[j + 1] = empty[j];
            j--;
        }
        empty[j + 1] = key;
    }
    //modify the sequence of empty by the priority
    for (k = 1; k < emptySize && empty[k] < 3600; k++);                     // skip the corner
        
    for (i = k + 1; i < emptySize && empty[i] < 5400; i++)                  // modify the edges
    {
        int keyPriority, nextPriority;
        int key = empty[i];
        int j = i - 1;

        int key_row = (key / 30) % 30;                                      // get the row out
        int key_col = key % 30;                                             // get the col out
        if ((key_row == 0 || key_row == 4) && (key_col == 2 || key_col == 3))
            keyPriority = 1;
        else if ((key_row == 2) && (key_col == 0 || key_col == 5))
            keyPriority = 2;
        else if (key_row == 0 || key_row == 4)
            keyPriority = 3;
        else
            keyPriority = 4;
        int next_row = (empty[j] / 30) % 30;
        int next_col = empty[j] % 30;
        if ((next_row == 0 || next_row == 4) && (next_col == 2 || next_col == 3))
            nextPriority = 1;
        else if ((next_row == 2) && (next_col == 0 || next_col == 5))
            nextPriority = 2;
        else if (next_row == 0 || next_row == 4)
            nextPriority = 3;
        else
            nextPriority = 4;

        while (keyPriority < nextPriority && j >= k)
        {
            empty[j + 1] = empty[j];
            j--;
            next_row = (empty[j] / 30) % 30;
            next_col = empty[j] % 30;
            if ((next_row == 0 || next_row == 4) && (next_col == 2 || next_col == 3))
                nextPriority = 1;
            else if ((next_row == 2) && (next_col == 0 || next_col == 5))
                nextPriority = 2;
            else if (next_row == 0 || next_row == 4)
                nextPriority = 3;
            else
                nextPriority = 4;
        }
        empty[j + 1] = key;
    }

    if ((opponent_limit[0] / 900) == 1)                         // next step will be exploded, need to active chain reaction
    {
        for (i = 0; i < opponentSize && !found; i++)            // find the opponent 's weak
        {
            opponent_row = (opponent_limit[i] / 30) % 30;
            opponent_col = opponent_limit[i] % 30;
            opponent_pos = opponent_row * 6 + opponent_col;
            if (willexplosive(board, opponentColor, opponent_row, opponent_col) != -100)
            {
                //fileout2 << "opponent point = " << opponent_row << "  " << opponent_col << endl; //test only
                for (int diff = -7; diff < 8; diff++)
                {
                    out_of_range = false;

                    if (diff == -4)
                        diff = -1;
                    else if (diff == 0)
                        diff = 1;
                    else if (diff == 2)
                        diff = 5;

                    if (opponent_row == 0 && diff >= -7 && diff <= -5)
                        out_of_range = true;
                    else if (opponent_row == 4 && diff >= 5 && diff <= 7)
                        out_of_range = true;
                    if (opponent_col == 0 && (diff == -7 || diff == -1 || diff == 5))
                        out_of_range = true;
                    else if (opponent_col == 5 && (diff == -5 || diff == 1 || diff == 7))
                        out_of_range = true;

                    if (!out_of_range)
                    {
                        row = (opponent_pos + diff) / 6;
                        col = (opponent_pos + diff) % 6;
                        if (board.get_cell_color(row, col) == color)
                        {
                            int lead = willexplosive(board, color, row, col);
                            if (lead != -100)
                            {
                                check = 1; //test only
                                place_row = (row * 6 + col + lead) / 6;
                                place_col = (row * 6 + col + lead) % 6;
                                //fileout2 << "opponent at (" << opponent_row << ", " << opponent_col << ")\n" //test only
                                //         << "place orb at (" << place_row << ", " << place_col << ")\n"      //test only
                                //         << "lead = " << lead << endl;                                       //test only
                                found = true;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }

    // next step won't be exploded
    for (i = 0; i < opponentSize && !found; i++)                    // find the opponent 's weak
    {
        opponent_row = (opponent_limit[i] / 30) % 30;
        opponent_col = opponent_limit[i] % 30;
        opponent_pos = opponent_row * 6 + opponent_col;
        bool opponentSafe = isSafe(board, color, opponent_row, opponent_col, false);
        //fileout2 << "opponent point = " << opponent_row << "  " << opponent_col << endl; //test only
        for (int diff = -7; diff < 8 /*&& opponentSafe != 0*/; diff++)
        {
            out_of_range = false;

            if (diff == -4)
                diff = -1;
            else if (diff == 0)
                diff = 1;
            else if (diff == 2)
                diff = 5;

            if (opponent_row == 0 && diff >= -7 && diff <= -5)
                out_of_range = true;
            else if (opponent_row == 4 && diff >= 5 && diff <= 7)
                out_of_range = true;
            if (opponent_col == 0 && (diff == -7 || diff == -1 || diff == 5))
                out_of_range = true;
            else if (opponent_col == 5 && (diff == -5 || diff == 1 || diff == 7))
                out_of_range = true;

            if (!out_of_range)
            {
                row = (opponent_pos + diff) / 6;                                                        // get row
                col = (opponent_pos + diff) % 6;                                                        // get col
                if (board.get_cell_color(row, col) == color || board.get_cell_color(row, col) == 'w')
                {
                    orb_diff = board.get_capacity(row, col) - board.get_orbs_num(row, col);             // how many needed to explode
                    orb_diff -= diff_to_explosive(board, opponentColor, opponent_row, opponent_col);    // check its neighborhood, if can chain reaction
                    bool Safe = isSafe(board, opponentColor, row, col, true);                           // check safe
                    if (Safe)
                        if ((orb_diff <= 0) && (board.get_cell_color(row, col) == 'w'))
                        {
                            check = 2; //test only
                            place_row = row;
                            place_col = col;
                            found = true;
                            break;
                        }
                        else if ((orb_diff == 0))
                        {
                            check = 2; //test only
                            place_row = row;
                            place_col = col;
                            found = true;
                            break;
                        }
                }
            }
        }
    }

    for (i = 0; i < emptySize && !found; i++)                       // find the zero affect place, neighborhood all empty
    {
        int count_neighbor = 0;
        opponent_row = (empty[i] / 30) % 30;                        // get row
        opponent_col = empty[i] % 30;                               // get col
        opponent_pos = opponent_row * 6 + opponent_col;             // 2D -> 1D
        //fileout2 << "empty point = " << opponent_row << opponent_col << endl;  //test only

        for (int diff = -7; diff < 8; diff++)
        {
            out_of_range = false;

            if (diff == -4)                                         // neighborhood to now cell
                diff = -1;
            else if (diff == 0)
                diff = 1;
            else if (diff == 2)
                diff = 5;

            if (opponent_row == 0 && diff >= -7 && diff <= -5)
                out_of_range = true;
            else if (opponent_row == 4 && diff >= 5 && diff <= 7)
                out_of_range = true;
            if (opponent_col == 0 && (diff == -7 || diff == -1 || diff == 5))
                out_of_range = true;
            else if (opponent_col == 5 && (diff == -5 || diff == 1 || diff == 7))
                out_of_range = true;

            if (!out_of_range)
            {
                row = (opponent_pos + diff) / 6;                    // row
                col = (opponent_pos + diff) % 6;                    // col
                if (board.get_cell_color(row, col) != 'w')           
                    break;
                else
                    count_neighbor++;
                // fileout2 << "count_neighbor = " << count_neighbor << endl;  //test only
                if (count_neighbor == board.get_capacity(opponent_row, opponent_col)) 
                {
                    check = 3;
                    place_row = opponent_row;
                    place_col = opponent_col;
                    found = true;
                    break;
                }
            }
        }
    }

    for (i = 0; i < emptySize && !found; i++)                                    // find the zero affect place
    {
        int count_neighbor = 0;
        opponent_row = (empty[i] / 30) % 30;
        opponent_col = empty[i] % 30;
        opponent_pos = opponent_row * 6 + opponent_col;
        // fileout2 << "empty point = " << opponent_row << opponent_col << endl;  //test only

        for (int diff = -7; diff < 8; diff++)                       // neighborhood to now cell
        {
            out_of_range = false;

            if (diff == -4)                                         // neighborhood to now cell
                diff = -1;
            else if (diff == 0)
                diff = 1;
            else if (diff == 2)
                diff = 5;

            if (opponent_row == 0 && diff >= -7 && diff <= -5)
                out_of_range = true;
            else if (opponent_row == 4 && diff >= 5 && diff <= 7)
                out_of_range = true;
            if (opponent_col == 0 && (diff == -7 || diff == -1 || diff == 5))
                out_of_range = true;
            else if (opponent_col == 5 && (diff == -5 || diff == 1 || diff == 7))
                out_of_range = true;

            if (!out_of_range)
            {
                row = (opponent_pos + diff) / 6;
                col = (opponent_pos + diff) % 6;
                if (board.get_cell_color(row, col) == opponentColor)
                    break;
                else
                    count_neighbor++;
                //fileout2 << "count_neighbor = " << count_neighbor << endl;  //test only
                if (count_neighbor == board.get_capacity(opponent_row, opponent_col))
                {
                    check = 3;
                    place_row = opponent_row;
                    place_col = opponent_col;
                    found = true;
                    break;
                }
            }
        }
    }

    for (j = 0; j < mySize && !found && my_limit[j] < 1800; j++);

    for (int k = 1; k < 7 && !found; k++)
    {
        for (i = j; i < mySize && !found; i++)                          // find the long chain place
        {
            opponent_row = (my_limit[i] / 30) % 30;
            opponent_col = my_limit[i] % 30;
            opponent_pos = opponent_row * 6 + opponent_col;
            int Safe = isSafe(board, opponentColor, opponent_row, opponent_col, true);
            //if (round == 39 && opponent_row == 0 && opponent_col == 1)                                           //test only
            //    fileout2 << "Safe = " << Safe << " at point (" << opponent_row << ", " << opponent_col << ")\n"; //test only
            if (Safe == 2 + k)
            {
                check = 4;
                place_row = opponent_row;
                place_col = opponent_col;
                found = true;
                break;
            }
            else if (Safe == 2 && !stored)
            {
                stored = true;
                stored_row = opponent_row;
                stored_col = opponent_col;
            }
        }
    }

    if (!found && stored)
    {
        check = 5;
        place_row = stored_row;
        place_col = stored_col;
        found = true;
    }

    for (i = 0; i < emptySize && !found; i++)                           // find the zero place beside the chain
    {
        int count_neighbor = 0;
        opponent_row = (empty[i] / 30) % 30;
        opponent_col = empty[i] % 30;
        opponent_pos = opponent_row * 6 + opponent_col;
        //fileout2 << "empty point = " << opponent_row << opponent_col << endl;  //test only

        for (int diff = -7; diff < 8; diff++)                   // neighborhood to now cell
        {
            out_of_range = false;

            if (diff == -4)
                diff = -1;
            else if (diff == 0)
                diff = 1;
            else if (diff == 2)
                diff = 5;

            if (opponent_row == 0 && diff >= -7 && diff <= -5)
                out_of_range = true;
            else if (opponent_row == 4 && diff >= 5 && diff <= 7)
                out_of_range = true;
            if (opponent_col == 0 && (diff == -7 || diff == -1 || diff == 5))
                out_of_range = true;
            else if (opponent_col == 5 && (diff == -5 || diff == 1 || diff == 7))
                out_of_range = true;

            if (!out_of_range)
            {
                row = (opponent_pos + diff) / 6;
                col = (opponent_pos + diff) % 6;
                if (board.get_cell_color(row, col) == opponentColor && willexplosive(board, opponentColor, row, col) != -100)
                {
                    //if (round == 77 && opponent_row == 3 && opponent_col == 1)//test only
                    //    fileout2 << "target point at (" << row << ", " << col << ")\n"; //test only
                    break;
                }
                else
                    count_neighbor++;
                //if (round == 77 && opponent_row == 3 && opponent_col == 1)                                                              //test only
                //    fileout2 << "count_neighbor = " << count_neighbor << " at point (" << opponent_row << ", " << opponent_col << ")\n" //test only
                //             << "target point at (" << row << ", " << col << ")\n";                                                     //test only
                if (count_neighbor == board.get_capacity(opponent_row, opponent_col))
                {
                    check = 6;
                    place_row = opponent_row;
                    place_col = opponent_col;
                    found = true;
                    break;
                }
            }
        }
    }
    for (j = 0; j < mySize && !found && my_limit[j] < 1800; j++) ;

    for (i = j; i < mySize && !found; i++)                              // find the not safe but won't explode long chain place
    {
        int count_neighbor = 0;
        opponent_row = (my_limit[i] / 30) % 30;
        opponent_col = my_limit[i] % 30;
        opponent_pos = opponent_row * 6 + opponent_col;

        for (int diff = -7; diff < 8; diff++)                           // neighborhood to now cell
        {
            out_of_range = false;

            if (diff == -4)
                diff = -1;
            else if (diff == 0)
                diff = 1;
            else if (diff == 2)
                diff = 5;

            if (opponent_row == 0 && diff >= -7 && diff <= -5)
                out_of_range = true;
            else if (opponent_row == 4 && diff >= 5 && diff <= 7)
                out_of_range = true;
            if (opponent_col == 0 && (diff == -7 || diff == -1 || diff == 5))
                out_of_range = true;
            else if (opponent_col == 5 && (diff == -5 || diff == 1 || diff == 7))
                out_of_range = true;

            if (!out_of_range)
            {
                row = (opponent_pos + diff) / 6;
                col = (opponent_pos + diff) % 6;
                if (board.get_cell_color(row, col) == opponentColor && willexplosive(board, opponentColor, row, col) != -100)
                {
                    //if (round == 77 && opponent_row == 3 && opponent_col == 1)//test only
                    //    fileout2 << "target point at (" << row << ", " << col << ")\n"; //test only
                    break;
                }
                else
                    count_neighbor++;
                //if (round == 77 && opponent_row == 3 && opponent_col == 1)                                                              //test only
                //    fileout2 << "count_neighbor = " << count_neighbor << " at point (" << opponent_row << ", " << opponent_col << ")\n" //test only
                //             << "target point at (" << row << ", " << col << ")\n";                                                     //test only
                if (count_neighbor == board.get_capacity(opponent_row, opponent_col))
                {
                    check = 7;
                    place_row = opponent_row;
                    place_col = opponent_col;
                    found = true;
                    break;
                }
            }
        }
    }

    if (!found)
    {
        check = 8;
        place_row = (my_limit[0] / 30) % 30;                                // place orb on my top ceoll
        place_col = my_limit[0] % 30;
    }
    // fileout2 << "At round " << round << ", determine step :" << check << endl;
    if (!(board.get_cell_color(place_row, place_col) == color || board.get_cell_color(place_row, place_col) == 'w')) // test only
    {

        // fileout2 << "error step at (" << place_row << ",  " << place_col << ")" << endl;
    }
    index[0] = place_row;
    index[1] = place_col;
}
