NOT COMPLETE


## Overview

I challenged my brother to a programming competition over the summer, and we settled on writing AI for the game Othello /
Reversi. I had just finished learning C at Yale and so built the AI in C. 

The AI takes a board input, color for the player, and max time allowed to compute a move. 

It builds out the game tree beadth first and uses min-max to pick the best move at each level for each branch. To do breadth first search of the tree I:
1.	Find all the valid moves possible from a given configuration
2. 	Add all the valid moves to the tail of a queue
3. 	Remove the next move at the front of the queue and repeat

This means that I build the game tree from left-to-right top down. 

Before executing 3 on each loop I check that I have not in danger of exceeding the time limit. 

## Evaluation Function

I assign each square on the board a point value and if a piece occupies that square it adds the value to its point total. Each color also then adds to this the total number of pieces they have on the board. 

At the end of the game (when there are less than 4 moves left) the number of pieces is the only factor that counts. 

The point values were not determined with any sciene, nor was 4 moves defining the end game.

<pre>
    1   2   3   4  
  +---+---+---+---+
1 |30 |-4 |12 |10 |
  +---+---+---+---+
2 |-4 |-8 |-4 | 2 |
  +---+---+---+---+
3 |12 |-4 | 2 | 2 |
  +---+---+---+---+
4 |10 | 2 | 2 |-4 |
  +---+---+---+---+
</pre>

## Inputs

Reversi can be played as a human or a computer. If played as a computer, the user inputs the move to make.

Computer Player
	<pre>`<filename> <black|white> <time in seconds>`</pre>

Human Player
	<pre>`<filename> <black|white> <location to place piece> <some char>`</pre>


## Board Representation

The board is printed to stdout as follows. 

<pre>
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
</pre>

When making a move as a human player, the move is entered as XY, where X is the x co-ordinate from 1-8 and Y is the y co-ordinate from 1-8. 

## To Do

I would like to use alpha-beta pruning on the tree to improve compute time.


