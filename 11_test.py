import tkinter as tk

root = tk.Tk()
root.geometry('330x370')

# GUI class


class Interface():
    def __init__(self, window):
        self.window = window
        window.title("Sudoku Solver")

        font = ('Arial', 20)
        color = 'white'

        # Create solve and clear button and link them to Solve and Clear methods
        solve = tk.Button(window, text='Solve', command=self.Solve)
        solve.grid(row=20, column=3)
        clear = tk.Button(window, text='Clear', command=self.Clear)
        clear.grid(row=20, column=5)

        # Initialise empty 2D list
        self.board = []
        for row in range(9):
            self.board += [["", "", "", "", "", "", "", "", ""]]

        for row in range(9):
            for col in range(9):
                # Change color of cells depending on position in grid
                if (row < 3 or row > 5) and (col < 3 or col > 5):
                    color = 'white'
                elif (row >= 3 and row < 6) and (col >= 3 and col < 6):
                    color = 'white'
                else:
                    color = 'grey'

                # Make each cell of grid a entry box and store each user entry into the filledBoard 2D list
                self.board[row][col] = tk.Entry(window, width=2, font=font, bg=color, cursor='arrow', borderwidth=2,
                                                highlightcolor='yellow', highlightthickness=0, highlightbackground='black',
                                                textvariable=filledBoard[row][col])
                # Link to better understand binding statements: https://www.python-course.eu/tkinter_events_binds.php#:~:text=Tkinter%20provides%20a%20mechanism%20to,and%20methods%20to%20an%20event.&text=If%20the%20defined%20event%20occurs,describing%20the%20event.
                self.board[row][col].bind('<FocusIn>', self.gridChecker)
                self.board[row][col].bind('<Motion>', self.gridChecker)
                self.board[row][col].grid(row=row, column=col)

    # Function to check if user enters a value which is not an int between 1 and 9 (valid numbers in Sudoku game).
    # If entry is not valid, clear value
    def gridChecker(self, event):
        for row in range(9):
            for col in range(9):
                if filledBoard[row][col].get() not in ['1', '2', '3', '4', '5', '6', '7', '8', '9']:
                    filledBoard[row][col].set('')

    # Call Sudoku solver class (called by solve button)
    def Solve(self):
        pass

    # Function to clear board (called by clear button)
    def Clear(self):
        for row in range(9):
            for col in range(9):
                filledBoard[row][col].set('')


# Global 2D list which saved in the values the user enters on the GUI
# Each value in the 2D list is set as a StringVar(), a class in Tkinter
# which allows you to save values users enter into the Entry widget
filledBoard = []
for row in range(9):
    filledBoard += [["", "", "", "", "", "", "", "", ""]]
for row in range(9):
    for col in range(9):
        filledBoard[row][col] = tk.StringVar(root)

# Main Loop
Interface(root)
root.mainloop()
