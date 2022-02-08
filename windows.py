import tkinter as tk
from tkinter.constants import CENTER, LEFT

root_window = tk.Tk()

# 主視窗設定
root_window.title("Sudoku")
root_window.geometry("500x400+1200+300")
root_window.resizable(False, False)


# 功能按鈕
solve = tk.Button(root_window, text='Solve')
solve.grid(column=3, row=9)
clear = tk.Button(root_window, text='Clear')
clear.grid(column=5, row=9)

# 數獨格
board = []
for row in range(9):
    board += [["", "", "", "", "", "", "", "", ""]]

for row in range(9):
    for col in range(9):
        if (row < 3 or row > 5) and (col < 3 or col > 5):
            board_color = 'white'
        elif (row >= 3 and row < 6) and (col >= 3 and col < 6):
            board_color = 'white'
        else:
            board_color = 'grey'

        board[row][col] = tk.Entry(root_window, width=2, font=('Arial', 20), bg=board_color, cursor='arrow',
                                   borderwidth=2, highlightcolor='yellow', highlightthickness=0, highlightbackground='black')
        board[row][col].grid(row=row, column=col)


root_window.mainloop()
