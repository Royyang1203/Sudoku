#include <iostream>

#ifndef SUDOKU_DATA_H
#define SUDOKU_DATA_H

class SudokuData
{
private:
    int index = 0;               // 資料編號，前五十個為新遊戲檔，後面為暫存檔
    int board[9][9] = {0};       // 遊戲圖資訊
    bool locked[9][9] = {false}; // 遊戲玩家是否可更改，因為有遊戲存檔，單看數字無法區分是題目還是玩家填的數字
public:
    SudokuData() = default;
    int getIndex() const
    {
        return index;
    }
    void setIndex(int num)
    {
        index = num;
    }
    int getBoard(int r, int c) const
    {
        return board[r][c];
    }
    void setBoard(int num, int r, int c)
    {
        board[r][c] = num;
    }
    bool getLocked(int r, int c) const
    {
        return locked[r][c];
    }
    void setLocked(bool num, int r, int c)
    {
        locked[r][c] = num;
    }

    // 判斷是否有解，暴力遞迴
    bool haveSolution(int r, int c)
    {
        // 填完最後一個數字(底)
        if (r >= 9)
            return true;

        //this->output(std::cout);

        // 如果該格為零，就要猜一個數字填入
        if (board[r][c] == 0)
        {
            // 避免跑太多圈無意義的遞迴，對猜的數字進行優化
            bool col[10] = {false};
            bool row[10] = {false};
            bool box[10] = {false};

            //找出該行與該列已經填過哪些數字
            for (int i = 0; i < 9; ++i)
            {
                if (board[r][i]) // 如果不為零，這個數字就不能填了
                    row[board[r][i]] = true;
                if (board[i][c])
                    col[board[i][c]] = true;
            }

            // 找出所在位置的九宮格裡已經填過哪些數字
            int rr = r - (r % 3);
            int cc = c - (c % 3);
            for (int i = 0; i < 3; ++i)
                for (int j = 0; j < 3; ++j)
                    if (board[rr + i][cc + j])
                        box[board[rr + i][cc + j]] = true;

            // 確認1~9哪些數字能填在這格,如果能填則繼續遞迴
            for (int num = 1; num <= 9; ++num)
            {
                if (!col[num] && !row[num] && !box[num]) // 確認這個數字是否可以填
                {
                    board[r][c] = num;

                    int nxt_r = r, nxt_c = c + 1;
                    if (nxt_c == 9) //換行
                    {
                        ++nxt_r;
                        nxt_c = 0;
                    }
                    if (haveSolution(nxt_r, nxt_c)) // 往下一個格子遞迴
                        return true;

                    board[r][c] = 0; // 此數字無解，返回前將遞迴做的假設擦掉
                }
            }
        }

        // 有數字 1 ~ 9，直接往下一格
        else // board[r][c] != 0
        {
            int nxt_r = r, nxt_c = c + 1;
            if (nxt_c == 9) //換行
            {
                ++nxt_r;
                nxt_c = 0;
            }
            if (haveSolution(nxt_r, nxt_c)) // 往下一個格子遞迴
                return true;
        }
        return false; // 無解
    }

    // 是否為其中一解
    bool isSolution() const
    {
        // 檢查每列是否遵守規則
        for (int i = 0; i < 9; ++i)
        {
            bool row[10] = {false};
            for (int j = 0; j < 9; ++j)
            {
                if (row[board[i][j]] == false)
                    row[board[i][j]] = true;
                else
                {
                    //std::cout << "@" << i << "  " << j << std::endl;
                    return false;
                }
            }
        }

        // 檢查每行是否遵守規則
        for (int i = 0; i < 9; ++i)
        {
            bool col[10] = {false};
            for (int j = 0; j < 9; ++j)
            {
                if (col[board[j][i]] == false)
                    col[board[j][i]] = true;
                else
                {
                    //std::cout << "@@" << i << "  " << j << std::endl;
                    return false;
                }
            }
        }

        // 檢查每個小九宮格是否遵守規則
        for (int i = 0; i < 9; ++i)
        {
            bool box[10] = {false};
            for (int j = 0; j < 3; ++j)
            {
                for (int k = 0; k < 3; ++k)
                {
                    //std::cout << board[i / 3 * 3 + j][i % 3 * 3 + k];
                    if (box[board[i / 3 * 3 + j][i % 3 * 3 + k]] == false)
                        box[board[i / 3 * 3 + j][i % 3 * 3 + k]] = true;
                    else
                    {
                        //std::cout << "@@@" << i << "  " << j << "  " << k << std::endl;
                        return false;
                    }
                }
            }
        }
        return true;
    }

    // 印出遊戲圖
    void output(std::ostream &os)
    {
        os << "-------------------------------------" << std::endl;

        for (int i = 0; i < 9; ++i)
        {
            os << "  ";
            for (int j = 0; j < 9; ++j)
            {
                if (j)
                    os << " ";
                os << board[i][j];
            }
            os << std::endl;
        }
        os << "-------------------------------------" << std::endl;
    }
    friend std::ostream &operator<<(std::ostream &os, SudokuData &that);
};

std::ostream &operator<<(std::ostream &os, SudokuData &that)
{
    that.output(os);
    return os;
}

#endif // SUDOKU_DATA_H
