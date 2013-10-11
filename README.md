## Inputs

Reversi can be played as a human or a computer. If played as a computer, the user inputs the move to make.

Computer Player
	<filename> <black|white> <time in seconds>

Human Player
	<filename> <black|white> <location to place piece> <some char>


## Board Representation

The board is printed to stdout as follows. 

    1   2   3   4   5   6   7   8 
  +---+---+---+---+---+---+---+---+
1 |   |   |   |   |   |   |   |   |
  +---+---+---+---+---+---+---+---+
2 |   |   |   |   |   |   |   |   |
  +---+---+---+---+---+---+---+---+
3 |   |   |   | B |   |   |   |   |
  +---+---+---+---+---+---+---+---+
4 |   |   |   | B | B |   |   |   |
  +---+---+---+---+---+---+---+---+
5 |   |   |   | B | W |   |   |   |
  +---+---+---+---+---+---+---+---+
6 |   |   |   |   |   |   |   |   |
  +---+---+---+---+---+---+---+---+
7 |   |   |   |   |   |   |   |   |
  +---+---+---+---+---+---+---+---+
8 |   |   |   |   |   |   |   |   |
  +---+---+---+---+---+---+---+---+

When making a move as a human player, the move is entered as XY, where X is the x co-ordinate from 1-8 and Y is the y co-ordinate from 1-8. 