#include <iostream>
#include <ctime>
#include <iomanip>

//#define TEST

class MineSweeper
{
private:
    int edge = 0;        // 決定是幾乘幾的大小
    int bomb_number = 0; // 炸彈的數量
    int **bomb;          // -1代表炸彈，正整數和零為周圍的炸彈數
    bool **hide;         // 是否翻開
public:
    MineSweeper() = default;
    MineSweeper(int _edge, int _bomb_number) : edge(_edge), bomb_number(_bomb_number)
    {
        setInit();
    }

    ~MineSweeper()
    {
        for (int i = 0; i < edge; i++)
        {
            delete[] bomb[i];
            delete[] hide[i];
        }
        delete[] bomb;
        delete[] hide;
    }

    // 初始化遊戲
    void setInit()
    {
        // 動態宣告陣列
        bomb = new int *[edge];
        hide = new bool *[edge];

        for (int i = 0; i < edge; i++)
        {
            bomb[i] = new int[edge];
            hide[i] = new bool[edge];
        }

        // 陣列初始化
        for (int i = 0; i < edge; i++)
        {
            std::fill(&bomb[i][0], &bomb[i][0] + edge, 0);
            std::fill(&hide[i][0], &hide[i][0] + edge, 1);
        }

        // 炸彈產生
        srand((unsigned)time(0));
        for (int i = 0; i < bomb_number; /* null */)
        {
            int row = rand() % edge, col = rand() % edge;
            if (bomb[row][col] == 0) // 避免重複設定在同一位置
            {
                bomb[row][col] = -1;
                ++i;
            }
        }

        // 計算每個格子周圍炸彈數 ( bomb )
        for (int i = 0; i < edge; ++i)
        {
            for (int j = 0; j < edge; ++j)
            {
                if (bomb[i][j] != -1)
                {
                    for (int k = -1; k <= 1; ++k)
                    {
                        for (int l = -1; l <= 1; ++l)
                        {
                            if ((i + k >= 0 && i + k < edge && j + l >= 0 && j + l < edge) && bomb[i + k][j + l] == -1)
                                ++bomb[i][j];
                        }
                    }
                }
            }
        }
    }

    // 翻開特定位子，顯示 bomb ，如果翻開的那格是零，呼叫下一函式將其周圍的格子一起翻開
    void open_cell(int r, int c)
    {
        if (hide[r][c] == 1)
        {
            hide[r][c] = 0;
            if (bomb[r][c] == 0)
                open_surrounding(r, c);
        }
        //std::cout << r << c << std::endl;
    }

    // 翻開特定位子周圍的格子(共八個)，跟上面一個函式做配對，如果還有零會遞迴
    void open_surrounding(int r, int c)
    {
        for (int i = -1; i <= 1; ++i)
            for (int j = -1; j <= 1; ++j)
            {
                if (r + i >= 0 && r + i < edge && c + j >= 0 && c + j < edge)
                {
                    //std::cout << "@" << r + i << c + j << std::endl;
                    open_cell(r + i, c + j);
                }
            }
    }

    // 翻開所有格子
    void open_all()
    {
        for (int i = 0; i < edge; ++i)
            for (int j = 0; j < edge; ++j)
                hide[i][j] = 0;
    }

    int getBomb(int row, int col)
    {
        return bomb[row][col];
    }

    int getHide(int row, int col)
    {
        return hide[row][col];
    }

    // 釋放原有的動態記憶體，重新設定大小，呼叫初始化函式 (會重新要求記憶體空間)
    void reset(int _edge, int _bomb_number)
    {
        for (int i = 0; i < this->edge; i++)
        {
            delete[] bomb[i];
            delete[] hide[i];
        }
        delete[] bomb;
        delete[] hide;

        this->edge = edge;
        this->bomb_number = _bomb_number;
        setInit();
    }

    // 如果有給位置，就會檢查是否輸了，其他會計算還未打開的格子數量和炸彈束是否相等，以此為勝利判斷
    int check_state(int r = -1, int c = -1)
    {
        if (bomb[r][c] == -1) // lose
            return 2;
        int count = 0; // win
        for (int i = 0; i < edge; i++)
        {
            for (int j = 0; j < edge; j++)
            {
                if (hide[i][j])
                {
                    ++count;
                }
            }
        }
        if (count == bomb_number) // 勝利
        {
            //std::cout << count << " " << bomb_number << std::endl;
            return 1;
        }
        return 3; // 還未結束
    }

    // 印出遊戲圖
    void show() const
    {
        system("cls");
        for (int i = 0; i <= edge; ++i)
            if (i == 0)
                std::cout << "    ";
            else
                std::cout << std::setw(2) << i << " ";
        std::cout << std::endl;
        for (int i = 0; i <= edge; ++i)
            if (i == 0)
                std::cout << "    ";
            else
                std::cout << "___";
        std::cout << std::endl;
        for (int i = 0; i < edge; ++i)
        {
            std::cout << std::setw(2) << i + 1 << " |";
            for (int j = 0; j < edge; ++j)
            {
                if (hide[i][j])
                    std::cout << std::setw(2) << "口"
                              << " ";
                else if (bomb[i][j] == -1)
                    std::cout << std::setw(2) << "⊕"
                              << " ";
                else
                    std::cout << std::setw(2) << bomb[i][j] << " ";
            }
            std::cout << "|" << std::setw(2) << i + 1;
            std::cout << std::endl;
        }
        for (int i = 0; i <= edge; ++i)
            if (i == 0)
                std::cout << "   |";
            else
                std::cout << "___";
        std::cout << "|" << std::endl;
        for (int i = 0; i <= edge; ++i)
            if (i == 0)
                std::cout << "    ";
            else
                std::cout << std::setw(2) << i << " ";
        std::cout << std::endl;
#ifdef TEST
        for (int i = 0; i <= edge; ++i)
            if (i == 0)
                std::cout << "    ";
            else
                std::cout << "___";
        std::cout << std::endl;
        for (int i = 0; i < edge; ++i)
        {
            std::cout << std::setw(2) << i + 1 << " |";
            for (int j = 0; j < edge; ++j)
            {
                std::cout << std::setw(2) << bomb[i][j] << " ";
            }
            std::cout << "|" << std::setw(2) << i + 1;
            std::cout << std::endl;
        }
        for (int i = 0; i <= edge; ++i)
            if (i == 0)
                std::cout << "   |";
            else
                std::cout << "___";
        std::cout << "|" << std::endl;
        for (int i = 0; i <= edge; ++i)
            if (i == 0)
                std::cout << "    ";
            else
                std::cout << std::setw(2) << i << " ";
        std::cout << std::endl
                  << std::endl;
#endif // TEST
    }
    friend std::ostream &operator<<(std::ostream &out, MineSweeper &that)
    {
        that.show();
        return out;
    }
};

using namespace std;

int main()
{
    int open_r, open_c, edge, bombs;
    char c;
    do
    {
        system("cls");

        // 先輸入 edge, bombs，才能初始化遊戲
        try
        {
            cout << "Enter the edge and bombs (eg: 10 6 ) >> ";
            cin >> edge >> bombs;
            if (edge <= 0)
                throw edge;
        }
        catch (int n)
        {
            cout << "Invalid variable" << endl;
            cout << "You must enter a number greater than zero." << endl;
        }

        // 初始化遊戲圖
        MineSweeper m(edge, bombs);

        cout << m; // 印出遊戲圖

        // 遊戲內，每輸入一個位置，程式就會執行一次，直到判斷出贏或輸才會跳出
        cout << endl
             << "Enter a location, the first number is row,  and the second one is the column (eg: 2 6 ) >> ";
        while (cin >> open_r >> open_c)
        {
            // 避免無效輸入
            while (open_r <= 0 || open_r > edge || open_c <= 0 || open_c > edge)
            {
                cout << "Invalid variable, the number must greater than 0 , and less or equal than " << edge << endl;
                cout << "Enter a location again (eg: 2 6 ) >> ";
                cin >> open_r >> open_c;
            }
            --open_r;
            --open_c;

            // 檢查所選格子是否為炸彈
            if (m.check_state(open_r, open_c) == 2) // lose
            {
                m.open_all();
                cout << m;
                cout << endl
                     << "   YOU LOSE !!!" << endl
                     << endl;
                break; // 跳出迴圈
            }

            // 還未失敗
            m.open_cell(open_r, open_c);
            cout << m;

            // 確認使否勝利
            if (m.check_state(open_r, open_c) == 1) // win
            {
                cout << endl
                     << "   YOU WIN !!!" << endl
                     << endl;
                break; // 跳出迴圈
            }

            // 還未結束，要求輸入下一個位置並繼續迴圈
            cout << endl
                 << "Enter a location, the first number is row,  and the second one is the column (eg: 2 6 ) >> ";
        }

        // 提問是否重玩
        cout << "Want to play again ? (y/n) ";
        cin >> c;
    } while (c == 'y' || c == 'Y' || c == '1');
    system("cls");
    cout << endl
         << "  Thanks for playing." << endl;
    return 0;
}
