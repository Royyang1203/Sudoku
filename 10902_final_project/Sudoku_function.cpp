#include <iostream>
#include <fstream>
#include <string.h>
#include <stdexcept>
#include "SudokuData.h"

using namespace std;

void format_reset();
void writedata();
void savedata(SudokuData &sudoku);
void querydata();
void enterdata(int index);
void deletedata(int ind);
void sudoku(int ind, bool _isnew);
void information(SudokuData &sudoku, bool &_back, int cmd);

// 初始化檔案，並建立一百個物件大小的空間
void format_reset()
{
    // 確認是否執行
    char c; // 是否要執行
    cout << "   WARNING !!!" << endl;
    cout << "This will delete all data, are you sure to do this ? (y / n) >> ";
    cin >> c;
    if (c != 'y' && c != 'Y')
        return;

    // 開檔案，並確認是否成功開啟
    ofstream outCredit("sudoku_data.dat", ios::binary);
    if (!outCredit)
    {
        cout << "File cannot be opend" << endl;
        exit(1);
    }

    // 在檔案中建立一百個 SudokuData 空物件
    SudokuData blank_sudoku;
    for (int i = 0; i < 100; ++i)
    {
        outCredit.write(reinterpret_cast<const char *>(&blank_sudoku), sizeof(SudokuData));
    }
    cout << "format succeed." << endl;

    // 關檔案
    outCredit.close();
}

// 寫入資料，會存在 1 ~ 50 的位置，並檢查是否存在解答，如果題目無解會寫入失敗；已寫入的資料會受到保護，不會令其被覆寫
void writedata()
{
    int num = 0;       // indexNumber，要寫入的位置
    int digit = 0;     // 填入的數字
    SudokuData sudoku; // 暫存物件
    char c = 'y';      // 是否要輸入下一個

    // 執行
    while (c == 'Y' || c == 'y')
    {
        // 選定寫入位置，檢查該位置是否有存檔案
        cout << "Enter the Index number (1 to 50, 0 to end input) >> ";
        cin >> num;
        if (num == 0)
            break;

        // open the file
        fstream outCredit("sudoku_data.dat", ios::in | ios::out | ios::binary);
        if (!outCredit)
        {
            cout << "File cannot be opend" << endl;
            exit(1);
        }

        outCredit.seekg((num - 1) * sizeof(SudokuData), ios::beg);             // 移動指標
        outCredit.read(reinterpret_cast<char *>(&sudoku), sizeof(SudokuData)); // 寫入暫存物件

        // close the file
        outCredit.close();

        // 已存在檔案
        if (sudoku.getIndex())
        {
            cout << "this index already have data" << endl;
        }

        // 存入位置沒問題，可繼續執行
        else if (num > 0 && num <= 50)
        {
            enterdata(num);
        }

        // 無效的 index number
        else
        {
            cout << "the number is out of range" << endl;
        }

        // 確認是否寫入下一個檔案
        cout << "Do you want to enter another test case ? (y / n) >> ";
        cin >> c;
    }
}

// 遊戲中存檔，會存在 51 ~ 100 的位置，如果已存在寫入的資料，會詢問是否覆寫
void savedata(SudokuData &sudoku)
{
    int num = 0;  // indexNumber，要寫入的位置
    char c = 'y'; // 是否要覆寫檔案
    SudokuData temp;

    // open the file
    fstream outCredit("sudoku_data.dat", ios::in | ios::out | ios::binary);
    if (!outCredit)
    {
        cout << "File cannot be opend" << endl;
        exit(1);
    }

    // 選定寫入位置，檢查該位置是否有存檔案
    cout << "Enter the Index number (1 to 50, other to end input) >> ";
    cin >> num;

    // 退出
    if (num < 1 || num > 50)
        return;

    outCredit.seekg((num + 49) * sizeof(SudokuData), ios::beg);
    outCredit.read(reinterpret_cast<char *>(&temp), sizeof(SudokuData));

    // 已經有資料了
    if (temp.getIndex())
    {
        cout << "this index already have data" << endl;
        cout << "do you want to overwrite the file ? (y / n) >> ";
        cin >> c;
    }

    // 退出
    if (!c == 'Y' && !c == 'y')
    {
        // NULL
    }

    // 寫入資料
    else if (num > 0 && num <= 50)
    {
        sudoku.setIndex(num + 50);
        outCredit.seekp(0, ios::beg);
        outCredit.seekp((sudoku.getIndex() - 1) * sizeof(SudokuData), ios::beg);
        outCredit.write(reinterpret_cast<const char *>(&sudoku), sizeof(SudokuData));
        cout << "successful" << endl;
    }

    // 無效輸入
    else
    {
        cout << "the number is out of range" << endl;
    }

    // close the file
    outCredit.close();
}

// 查找資料
void querydata()
{
    int cmd_d = 1; // 指令 for data menu

    while (cmd_d > 0 && cmd_d <= 5)
    {
        enum Data
        {
            IND = 1,
            IND_,
            LIST,
            LIST_,
            BACK_D
        };
        cout << "     DATA MENU" << endl
             << "-----------------------" << endl;
        cout << " 1 : check by index (new game data)" << endl
             << " 2 : check by index (saved game data)" << endl
             << " 3 : list all data (new game data)" << endl
             << " 4 : list all data (saved game data)" << endl
             << " 5 : go back" << endl
             << "Enter your choice -> ";
        cin >> cmd_d;

        SudokuData temp;  // 暫存資料
        int searchNumber; // 搜尋位置
        int dir;          // 往前搜尋或往後搜尋

        switch (cmd_d)
        {
        // 以位置查找資料
        case IND:
        case IND_:
            cout << "Enter the Index number (1 to 50, other to end input) >> ";
            cin >> searchNumber;

            // 無效輸入
            if (searchNumber < 1 || searchNumber > 50)
            {
                break;
            }
            else
            {
                // 遊戲存檔的位置在 51 ~ 100
                if (cmd_d == IND_)
                {
                    searchNumber += 50;
                }

                // open the file
                fstream outCredit("sudoku_data.dat", ios::in | ios::out | ios::binary);
                if (!outCredit)
                {
                    cout << "File cannot be opend" << endl;
                    exit(1);
                }
                outCredit.seekg((searchNumber - 1) * sizeof(SudokuData), ios::beg);
                outCredit.read(reinterpret_cast<char *>(&temp), sizeof(SudokuData));

                // close the file
                outCredit.close();

                // 新遊戲檔
                if (temp.getIndex() != 0 && cmd_d == IND)
                {
                    // 將解答的部分挖成空格
                    for (int i = 0; i < 9; ++i)
                    {
                        for (int j = 0; j < 9; ++j)
                        {
                            if (!temp.getLocked(i, j))
                                temp.setBoard(0, i, j);
                        }
                    }

                    // 印出遊戲資料
                    cout << "Index : " << temp.getIndex() << endl;
                    cout << temp;

                    int n;
                    cout << "Enter 1 to play, 2 to delete, 3 to modify, other to go back >> ";
                    cin >> n;
                    if (n == 1) // play
                    {
                        sudoku(searchNumber, true);
                    }
                    else if (n == 2) // delete
                    {
                        deletedata(temp.getIndex());
                    }
                    else if (n == 3) // modify
                    {
                        enterdata(searchNumber);
                    }
                    else
                        break;
                }

                // 遊戲存檔
                else if (temp.getIndex() != 0 && cmd_d == IND_)
                {
                    // 印出遊戲資料
                    cout << "Index : " << temp.getIndex() << endl;
                    cout << temp;

                    int n;
                    cout << "Enter 1 to play, 2 to delete, other to go back >> ";
                    cin >> n;
                    if (n == 1) // play
                    {
                        sudoku(searchNumber, false);
                    }
                    else if (n == 2) // delete
                    {
                        deletedata(temp.getIndex());
                    }
                    else
                        break;
                }
                else
                {
                    cout << "no data" << endl;
                }
            }
            break;

        // 查看所有資料檔，可選擇下一個或上一個，不會顯示空白資料
        case LIST:
        case LIST_:
            searchNumber = 1; // 初始位置

            // 遊戲存檔的位置在 51 ~ 100
            if (cmd_d == LIST_)
            {
                searchNumber += 50;
            }

            // 繼續執行前會檢查資料點是否在範圍內
            while (cmd_d == LIST && searchNumber > 0 && searchNumber <= 50 || cmd_d == LIST_ && searchNumber > 50 && searchNumber <= 100)
            {
                // open the file
                fstream outCredit("sudoku_data.dat", ios::in | ios::out | ios::binary);
                if (!outCredit)
                {
                    cout << "File cannot be opend" << endl;
                    exit(1);
                }

                // 取出資料
                outCredit.seekg((searchNumber - 1) * sizeof(SudokuData), ios::beg);
                outCredit.read(reinterpret_cast<char *>(&temp), sizeof(SudokuData));

                // 如果當前位置沒有資料，就會往下一個位置找，並檢查是否還在範圍內
                while (temp.getIndex() == 0 && (cmd_d == LIST && searchNumber > 0 && searchNumber <= 50 || cmd_d == LIST_ && searchNumber > 50 && searchNumber <= 100)) // 如果沒有資料就會往下一個位置找
                {
                    if (dir == 1)
                        --searchNumber;
                    else
                        ++searchNumber;
                    outCredit.seekg(0, ios::beg);
                    outCredit.seekg((searchNumber - 1) * sizeof(SudokuData), ios::beg);
                    outCredit.read(reinterpret_cast<char *>(&temp), sizeof(SudokuData));
                }

                // close the file
                outCredit.close();

                // 底部資料還是空白，也就是沒下一個資料了，退出
                if (temp.getIndex() == 0)
                {
                    cout << " no data" << endl;
                    break;
                }

                // 1 ~ 50
                if (cmd_d == LIST)
                {
                    // 將解答的部分挖成空格
                    for (int i = 0; i < 9; ++i)
                    {
                        for (int j = 0; j < 9; ++j)
                        {
                            if (!temp.getLocked(i, j))
                                temp.setBoard(0, i, j);
                        }
                    }

                    // 印出遊戲資料
                    cout << "Index : " << temp.getIndex() << endl;
                    cout << temp;

                    int n;
                    cout << "Enter 1 to play, 2 to delete, 3 to go previous, 4 to go next, 5 to modify, other to go back >> ";
                    cin >> n;
                    if (n == 1) // play
                    {
                        sudoku(temp.getIndex(), true);
                    }
                    else if (n == 2) // delete
                    {
                        deletedata(temp.getIndex());
                    }
                    else if (n == 3) // previous
                    {
                        --searchNumber;
                        dir = 1;
                    }
                    else if (n == 4) // next
                    {
                        ++searchNumber;
                        dir = 2;
                    }
                    else if (n == 5) // modify
                    {
                        enterdata(searchNumber);
                    }
                    else
                        break;
                }

                // 51 ~ 100
                else if (cmd_d == LIST_)
                {
                    // 印出遊戲資料
                    cout << "Index : " << temp.getIndex() << endl;
                    cout << temp;

                    int n;
                    cout << "Enter 1 to play, 2 to delete, 3 to go previous, 4 to go next, other to go back >> ";
                    cin >> n;
                    if (n == 1) // play
                    {
                        sudoku(temp.getIndex(), false);
                    }
                    else if (n == 2) // delete
                    {
                        deletedata(temp.getIndex());
                    }
                    else if (n == 3) // previous
                    {
                        --searchNumber;
                        dir = 1;
                    }
                    else if (n == 4) // next
                    {
                        ++searchNumber;
                        dir = 2;
                    }
                    else
                        break;
                }

                // 無效輸入
                if (!(cmd_d == LIST && searchNumber > 0 && searchNumber <= 50 || cmd_d == LIST_ && searchNumber > 50 && searchNumber <= 100))
                    cout << "out of range" << endl;
            }
            break;

        case BACK_D: // 退出
            return;
            break;
        default: // 無效輸入
            cerr << "Invalid Option! Try again." << endl;
            break;
        }
    }
}

// 寫入特定資料點
void enterdata(int index)
{
    // open the file
    fstream outCredit("sudoku_data.dat", ios::in | ios::out | ios::binary);
    if (!outCredit)
    {
        cout << "File cannot be opend" << endl;
        exit(1);
    }

    SudokuData temp;
    temp.setIndex(index);

    // 輸入數獨的數字
    cout << "Enter the sudoku question, each entries filled with digits from 1 to 9 ( an entries not filled will have 0 )" << endl;
    int digit = 0;
    for (int i = 0; i < 9; ++i)
    {
        for (int j = 0; j < 9; ++j)
        {
            cin >> digit;

            // 檢查是否為合理輸入
            while (digit > 9 || digit < 0)
            {
                cout << "invalid digit" << endl;
                cout << "Enter the sudoku question, each entries filled with digits from 1 to 9 ( an entries not filled will have 0 )" << endl;
                cin >> digit;
            }

            temp.setBoard(digit, i, j);
            if (digit != 0)
                temp.setLocked(true, i, j);
            else
                temp.setLocked(false, i, j);
        }
    }

    // 檢查是否存在解答，如果有就寫入資料
    if (temp.haveSolution(0, 0))
    {
        cout << "successful" << endl;
        outCredit.seekp((index - 1) * sizeof(SudokuData), ios::beg);
        outCredit.write(reinterpret_cast<const char *>(&temp), sizeof(SudokuData));
    }

    // 無解
    else
        cout << "FAILURE TO WRITE IN !!!" << endl
             << "must exist at least one solution in the question";

    // close the file
    outCredit.close();
}

// 刪除特定資料點
void deletedata(int index)
{
    char c;

    // 確認是否執行
    cout << "   WARNING !!!" << endl;
    cout << "This will delete the data, are you sure to do this ? (y / n) >> ";
    cin >> c;
    if (c != 'y' && c != 'Y')
        return;

    // open the file
    fstream outCredit("sudoku_data.dat", ios::in | ios::out | ios::binary);
    if (!outCredit)
    {
        cout << "File cannot be opend" << endl;
        exit(1);
    }

    // 寫入空白資料
    SudokuData blank_sudoku;
    outCredit.seekp((index - 1) * sizeof(SudokuData), ios::beg);
    outCredit.write(reinterpret_cast<const char *>(&blank_sudoku), sizeof(SudokuData));
    cout << "successful" << endl;

    // close the file
    outCredit.close();
}

// 進入遊戲，會將遊戲檔寫到暫存檔中執行，不會更動外部檔案
void sudoku(int ind, bool _isnew) // ind 為要遊玩的遊戲編號，_isnew 會區分此檔案是前五十個還是後五十個
{
    int opt_;           // 選項 for game menu
    SudokuData sudoku;  // 暫存檔
    bool _back = false; // 退出此函式用

    // 未指定玩哪題時，ind 會為零
    while (ind == 0)
    {
        enum Game
        {
            NEW = 1,
            OLD,
            BACK_S
        };
        cout << "     GAME MENU" << endl
             << "-----------------------" << endl;
        cout << " 1 : start new game" << endl
             << " 2 : continue with the saved game file" << endl
             << " 3 : go back" << endl
             << "Enter your choice -> ";
        cin >> opt_;

        switch (opt_)
        {
        case NEW: // 開始新遊戲
            do
            {
                cout << "Enter the Index number (1 to 50, 0 to go back) >> ";
                cin >> ind;
                if (ind == 0)
                    _back = true;
            } while (ind < 1 && ind > 50);
            _isnew = true;
            break;
        case OLD: // 開始舊遊戲
            do
            {
                cout << "Enter the Index number (1 to 50, 0 to go back) >> ";
                cin >> ind;
                if (ind == 0)
                    _back = true;
            } while (ind < 1 && ind > 50);
            ind += 50;
            _isnew = false;
            break;
        case BACK_S: // 退出
            return;
            break;
        default:
            cerr << "Invalid Option! Try again." << endl;
            break;
        }
    }

    // open the file
    ifstream inCredit("sudoku_data.dat", ios::binary);
    if (!inCredit)
    {
        cout << "File could not be opened." << endl;
        exit(1);
    }

    // 取出檔案
    inCredit.seekg((ind - 1) * sizeof(SudokuData), ios::beg);
    inCredit.read(reinterpret_cast<char *>(&sudoku), sizeof(SudokuData));

    // close the file
    inCredit.close();

    // 檔案是空的，直接退出
    if (sudoku.getIndex() == 0)
    {
        cout << "no data" << endl;
        return;
    }

    // 如果是新遊戲題目，因為有檢查是否有解答，存入的檔案為其中一解，遊玩前要將非題目數字的部分挖成空格
    if (_isnew)
        for (int i = 0; i < 9; ++i)
        {
            for (int j = 0; j < 9; ++j)
            {
                if (!sudoku.getLocked(i, j))
                    sudoku.setBoard(0, i, j);
            }
        }

    // 瑱入數字
    int r, c; // 選擇填入的位置
    while (1)
    {
        cout << sudoku;
        // 要求輸入位置，如果輸入無效會要求再次輸入
        do
        {
            if (_back)
                return;
            cout << "choice the blank you want to fill, 0 for more information ( e.g. 6 2 ) >> ";
            cin >> r;

            // 進入遊戲中選單
            if (r == 0)
            {
                information(sudoku, _back, 0);
                continue;
            }
            cin >> c;
        } while (r > 9 || c > 9 || r < 1 || c < 1);

        // 選到題目數字，不予更動
        if (sudoku.getLocked(r - 1, c - 1))
            cout << "this blank can not be change" << endl;

        // 要求輸入更動的數字，如果輸入無效會要求再次輸入
        else
        {
            int n;
            do
            {
                cout << "enter the number you want to fill (0 to 9) >> ";
                cin >> n;
            } while (n < 0 || n > 9);
            sudoku.setBoard(n, r - 1, c - 1);
        }
    }
}

// 遊戲中的選單
void information(SudokuData &sudoku, bool &_back, int cmd) // _back 為是否要退出，cmd for function table
{
    enum Info
    {
        BACK_I = 1,
        SAVE,
        ANS,
        OUT
    };
    while (cmd < 1 || cmd > 4)
    {
        cout << "      function table" << endl
             << "-----------------------" << endl
             << " 1 : go back to game" << endl
             << " 2 : saved the game" << endl
             << " 3 : check my answer" << endl
             << " 4 : leave the game without saving" << endl
             << "Enter your choice -> ";
        cin >> cmd;
    }
    switch (cmd)
    {
    case BACK_I: // 返回遊戲
        break;
    case SAVE: // 存檔
        savedata(sudoku);
        break;
    case ANS: // 檢查答案
        if (sudoku.isSolution())
        {
            cout << "YOU WIN !!!" << endl;
            _back = true; // 會退出遊戲
        }
        else // 不是答案，繼續
        {
            cout << "the answer is wrong" << endl;
            cout << sudoku;
        }
        break;
    case OUT: // 退出遊戲
        _back = true;
        break;
    default:
        throw std::out_of_range("Invalid Option!");
        break;
    }
}
