#include <iostream>
#include <fstream>
#include <string.h>
#include "SudokuData.h"
#include "Sudoku_funtion.cpp"

using namespace std;

enum Options {FORMAT = 1, ADDNEW, CHECK, PLAY, STOP};

int main()
{
    while(1)
    {
        cout << "         MENU" << endl
             << "-----------------------" << endl;
        cout << " 1 :format the data and reset the file" << endl
             << " 2 : Add new game file" << endl
             << " 3 : check or update data" << endl
             << " 4 : play game" << endl
             << " 5 : Quit" << endl
             << "Enter your choice -> ";

        int opt;
        cin >> opt;

        switch (opt)
        {
        case FORMAT :
            format_reset();
            break;
        case ADDNEW :
            writedata();
            break;
        case CHECK :
            querydata();
            break;
        case PLAY :
            sudoku(0, true);
            break;
        case STOP :
            exit(EXIT_SUCCESS);
        default:
            cerr << "Invalid Option! Try again." << endl;
            break;
        }
    }
}


