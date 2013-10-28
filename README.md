## Overview

I challenged my brother to a programming competition over the summer, and we settled on writing an AI for the game [Othello / Reversi](http://en.wikipedia.org/wiki/Reversi). I had just finished learning C for a class at Yale, so I wrote my AI to play Reversi in C.

## Input

The program can be played as either a human or a computer. If played as a computer, the user inputs the move to make.

Computer Player
  <pre>`<filename-##> <black|white> <time in seconds>`</pre>

Human Player
  <pre>`<filename-##> <black|white> <location to place piece> <some char>`</pre>

## Output

The program writes out the new board configuration to a new file. The new file has the same filename as the input file, and the file number is incremented by 1 e.g., test-01 -> test-02

## Picking A Move

The program builds out the game tree beadth first and uses min-max to pick the best move at each level of each branch. To do breadth first search of the tree the program:
1.	Finds all the valid moves possible from a given configuration
2. 	Adds all the valid moves to the tail of a queue
3. 	Removes the next move at the front of the queue and repeat

This means that I build the game tree from left-to-right top down. 

Before executing step 3 on each loop the program checks that it is not in danger of exceeding the time limit, including a buffer for scoring all the leaves and then picking the best move. 

## Evaluation Function

The program assigns each square on the board a point value and if a piece occupies that square it adds the value to its point total. Each player also then adds to this the total number of pieces they have on the board. 

At the end of the game (when there are less than 4 moves left) the number of pieces is the only factor that counts. 

The point values were not determined with any rigor, nor was 4 moves defining the end game.

The point values for each square on the board are as follows (note: this top-left quadrant is mirrored over the other four quadrants on the board):

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

## Entering A Human Move

The board is printed to stdout as follows:

<pre>
    1   2   3   4   5   6   7   8 
  +---+---+---+---+---+---+---+---+
1 |   |   |   |   |   |   |   |   |
  +---+---+---+---+---+---+---+---+
2 |   |   |   |   |   |   |   |   |
  +---+---+---+---+---+---+---+---+
3 |   |   |   | B | A |   |   |   |
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

When making a move as a human player, the move is entered as XY, where X is the x co-ordinate from 1-8 and Y is the y co-ordinate from 1-8. For example, if a human playing as white wants to make a move at A, the location of the new piece will be: 53.

## Outcomes

My AI has yet to loose to a human player. I am still waiting on the final results of games against my brother!

## To Do

An obvious next step to improve the progam is to use alpha-beta pruning on the tree. This would improve the compute time and leave more time for brute force. 

I would also like to test different point values per square in the evaluation function. 