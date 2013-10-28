#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <ctype.h>
#include "./getLine.h"
#include "./Tree.h"
#include <time.h>

//	Define the board size
#define X_Max 8
#define Y_Max 8
#define BOARD_SIZE X_Max*Y_Max

//	Print message to stderr and exit if there is an error
#define DIE(msg)    exit (fprintf (stderr, "%s\n", msg))

// For time format
#define SIZE 256

//	Upper bound time to score per node
//	In MicroSeconds
#define SCORE_TIME 1.5

/*
	Notes:
		Boards are not null-terminated
*/

//	Reads in a board and adds it to the queue
void readBoard (char* file_name, Deque* queue, Tree* initial);
//	Prints the board
void printBoard (char* board);
//	Prints out the White score and the Black player score
void printBoardScore(char* board);
// 	Writes the board to file
void writeBoard (char* final_file_name, char* board);
//	Sets the score for the node
void setScores (Tree node, int max_depth, bool is_black);
//	The Evaluation Function for a given board
int scoreBoard (char* board, bool is_black);
//	Returns the number of blank spaces left on the board
int numBlankSpaces (char* board);
//	Makes the final file name
void makeFinalFileName(char* file_name, char** final_file_name);
//	Makes a copy of Board A at Board B
void copyBoardAtoB (char* boardA, char** boardB);

//	Finds all the possible valid moves
void findAllMoves (char* initial, int depth, char own, char opp, Deque* queue, Tree play);
//	Returns true if a valid move is made, false otherwise
bool madeMove(char* board, int pos, int delta, char own, char opp);
//	Executes a valid move
void executeMove(char* board, int pos, int delta, char own, char opp);
// 	Returns true if a move is valid
bool validMove (char* board, int pos, int delta, char own);
//	Returns true if there is still room on the baord to move in the given direction
bool roomOnBoard(int pos, int delta);
//	Picks the best move of the root's children as the final move to play
void pickFinalMove (Tree game_initial, char** final_board);
//	Play as a human taking in the move to make
void humanPlayer(char* file_name, bool is_black, int move);
//	Make the move as a computer
void computerPlayer (char* file_name, bool is_black, float time_limit);

int main (int argc, char* argv[]) {

	if (argc < 4 || argc > 5)
		DIE ("Invalid number of arguments");
	else {
		//	Get the file name
		char* file_name = argv[1];

		//	Determine what color to play as
		bool is_black = false;
		if (strcmp(argv[2], "black")==0)
			is_black = true;

		//	If the number of arguments is 4 then play as a computer
		if (argc == 4) {
			//	Get the time limit for the computer to make a move
			float time_limit = atoi(argv[3]);
			computerPlayer(file_name, is_black, time_limit);
		}
		
		//	Else play as a human
		else {			
			humanPlayer(file_name, is_black, atoi(argv[3]));	
		}

	}
} 

void computerPlayer (char* file_name, bool is_black, float time_limit) {
	//	Create time variables used for ensuring execution within time constraints
	struct timeval t1, t2, t3;
	double elapsedTimeOne, elapsedTimeTwo, elapsedTimeThree;

	//	Establish t1 as the time at the beginning of the program
	gettimeofday(&t1, NULL);

	//	Create a queue that will be used to do breadth first search for best move
	Deque queue;
	createD(&queue);

	//	Create the tree that will be used to score the game and choose the move
	Tree game_initial;
	game_initial = createT();

	//	Read in the initial board & add it to the queue & as the root of the game tree
	readBoard(file_name, &queue, &game_initial);
	
	//	Time Limit in MicroSeconds
	time_limit = time_limit * 1000000;

	//	Remember the max depth reached in building out game tree
	int max_depth = 0;

	//	Keep track of the number of nodes (boards) found and total number in the deepest
	//	complete tree
	int num_nodes_to_compute = 0;
	int rolling_node_count = 0;

	//	The time required to compute the scores is:
	//	the number of nodes in the tree to be scored * the upper time limit per node
	float time_to_compute = num_nodes_to_compute * SCORE_TIME;
	
	//	Explore the game tree as far as possible given the time allowance
	do {
		//	If empty queue then no more viable moves
		if (isEmptyD(&queue)) {
			break;
		}
		/*
			Otherwise take a move of the queue and calculate the viable moves from
			that board, adding each one to the tail of the queue
		*/
		else {
			//	Increment the number of nodes (boards) found at that level
			rolling_node_count++;
			
			//	Get the next node of the queue
			Tree play;
			remD(&queue, &play);

			//	Get the nodes depth
			int depth = getDepth(play);
			//	The depth of the new node is 1 deeper
			int new_depth = depth+1;

			//	Check if completed a new whole level in the tree
			if (depth == (max_depth+2)) {
				max_depth = (depth-1);
				num_nodes_to_compute = num_nodes_to_compute + rolling_node_count;
				rolling_node_count = 0;
			}
			
			//	Establish which color playing for at this depth of the game tree
			bool own_is_black = is_black;
			if (new_depth%2==0)
				own_is_black = !is_black;
			char opp, own;
			if (own_is_black) {
				own = 'B';
				opp = 'W';
			}
			else {
				own = 'W';
				opp = 'B';
			}

			/*
				Find all the valid moves for this board
				Add the moves as children of the Tree
				Add the moves to the queue
			*/
			findAllMoves(getBoard(play),new_depth,own,opp,&queue,play);
			
			//	Get the current time
			gettimeofday(&t2, NULL);

			//	Calculate elapsed time in MicroSeconds
			elapsedTimeOne = ((t2.tv_sec - t1.tv_sec)*1000000) + (t2.tv_usec-t1.tv_usec);

			int total_nodes = num_nodes_to_compute + rolling_node_count;

			float avg_time = elapsedTimeOne / total_nodes;


			//	Calcualte the time required to compute scores for the largest full tree
			time_to_compute = (num_nodes_to_compute * avg_time * 0.2);

		}
	} while (time_to_compute < (time_limit - elapsedTimeOne));
	//	When time to compute scores is greater than time remaining stop making nodes

	// Printouts For Testing Efficiency	
	// printf("Max Depth is: %d\n", max_depth);
	// printf("Elapsed Time For Making Is: %f\n", elapsedTimeOne);
	// printf("Total Number of Nodes Made Is: %d\n", (num_nodes_to_compute + rolling_node_count));
	// printf("Total Number of Nodes Scorable Nodes Is: %d\n", num_nodes_to_compute);

	//	Set the scores for every node in the tree
	setScores(game_initial, max_depth, is_black);
	
	//	Pick the best move from the children of the root
	char* final_board;
	pickFinalMove(game_initial, &final_board);

	printBoard(final_board);

	//	Make the new file name
	char* final_file_name;
	makeFinalFileName(file_name, &final_file_name);

	printf("MADE IT TO PRINT BOARD\n");

	//	Save the final board file
	writeBoard(final_file_name,final_board);
	
	//	Check completed in time
	gettimeofday(&t3, NULL);
	
	//	Printouts For Testing
	// 	elapsedTimeTwo = ((t3.tv_sec - t2.tv_sec)*1000000) + (t3.tv_usec-t2.tv_usec);
	// 	printf("Elapsed Time For Scoring Is: %f\n", elapsedTimeTwo);
	// 	elapsedTimeThree = ((t3.tv_sec - t1.tv_sec)*1000000) + (t3.tv_usec-t1.tv_usec);
	// 	printf("Total Program Run Time: %f\n", elapsedTimeThree);
}
	

void humanPlayer(char* file_name, bool is_black, int move) {

	//	Position of the move
	int x = ((move/10)-1);
	int y = ((move%10)-1);
	int pos = ((y*Y_Max)+x);

	// Establish pieces
	char opp, own;
	if (is_black) {
		own = 'B';
		opp = 'W';
	}
	else {
		own = 'W';
		opp = 'B';
	}

	//	Read in the board
	FILE* fp;
	char* line;
	char board[BOARD_SIZE];
	fp = fopen(file_name,"r");
	if (!fp)
		DIE ("NO SUCH FILE");
	else {
		for (y=0; y<Y_Max; y++) {
			line = getLine(fp);
			for (x=0;x<X_Max;x++)
				board[(y*Y_Max)+x] = line[x];
		}
	}
	fclose(fp);

	//	Check the pos is a black square
	//	If it is, can make a move
	if (board[pos]==' ') {
		//	All possible deltas
		//	Array of all different directions
		int array[8] = {(-X_Max),(-X_Max+1),(1),(X_Max+1),(X_Max),(X_Max-1),(-1),(-X_Max-1)};
		//	Try every move in every direction
		bool made_move = false;	
		for (int i=0; i<8; i++) {
			bool move_success = madeMove(board, pos, array[i], own, opp);
			if (!made_move && move_success)
				made_move = true;
		}
		//	If made a move then save and print board
		if (made_move) {
			printBoard(board);
			//	Make the new file name
			char* final_file_name;
			makeFinalFileName(file_name, &final_file_name);
			//	Save the final board file
			writeBoard(final_file_name,board);
		}
		else
			DIE ("Invalid Move");	
	}
	else
		DIE ("Invalid Move");
}

void pickFinalMove (Tree game_initial, char** final_board) {

	Tree child = getChild(game_initial);
	char* best_board;

	//	If there are moves to make
	if (child!=NULL) {
		best_board = getBoard(child);
		int best_score = getScore(child);
		Tree next_child = getNext(child);
		while(next_child!=NULL) {
			int score = getScore(next_child);
			if (score>best_score) {
				best_board = getBoard(next_child);
				best_score = score;
			}
			next_child = getNext(next_child);
		}
	}
	// No moves to make so make the initial move
	else 
		best_board = getBoard(game_initial);

	*final_board = best_board;
}


void setScores (Tree node, int max_depth, bool is_black) {
	// If reached max depth for the tree score the node
	int depth = getDepth(node);
	if (depth == max_depth) {
		int score = scoreBoard (getBoard(node), is_black);
		setScore(node, score);
	}
	//	Otherwise the nodes scores is the Max/Min of its children
	//	If depth is odd then pick min
	//	If depth is even then pick max
	else {
		bool pick_max = true;
		int best_score = -1000;
		if (depth%2!=0) {
			pick_max = false;
			best_score = 1000;
		}
		Tree child = getChild(node);
		// Figured out the max / min score of children
		while (child!= NULL) {
			if (getScore(child)==NULL)
				setScores(child, max_depth, is_black);
			int child_score = getScore(child);
			if (pick_max && child_score > best_score)
				best_score = child_score;
			else if (!pick_max && child_score < best_score)
				best_score = child_score;
			child = getNext(child);
		}
		// Set nodes score to be that
		setScore(node, best_score);
	}
}

int numBlankSpaces (char* board) {
	int sum = 0;
	int x,y;
	for (y=0; y<Y_Max;y++) {		
		for (x=0;x<X_Max;x++) {
			if (board[(y*Y_Max)+x]==' ')
				sum = sum+1;
		}
	}
	return sum;
}

int scoreBoard (char* board, bool is_black) {
	
	int black_pieces = 0;
	int white_pieces = 0;

	/* The Squres on the board have the following values
		30 -4 12 10
		-4 -8 -4  2
		12 -4  2  2
		10  2  2 -4
	*/
	int square_scores[16] = {30,-4,12,10,-4,-8,-4,2,12,-4,2,2,10,2,2,-4};

	// Create variables to sore the scores
	int b_score = 0;
	int w_score = 0;

	//	Loop through each place in the board
	int x,y;
	for (y=0; y<Y_Max;y++) {		
		for (x=0;x<X_Max;x++) {
			char disk = board[(y*Y_Max)+x];
			//	Only care about squares that have a B or W disk
			if (disk !=' ') {
				//	Match the X,Y co-ordinate to the 4X4 symetrical scoring
				int calc_x = x;
				int calc_y = y;
				if (x>=4)
					calc_x = x-(((x-4)*2)+1);
				if (y>=4)
					calc_y = y-(((y-4)*2)+1);
				int score = square_scores[(calc_y*4)+calc_x];
				
				//	If the disk is Black
				if (disk == 'B') {
					b_score = b_score + score;
					black_pieces = black_pieces + 1;
				}
				//	The disk is White
				else {
					w_score = w_score + score;
					white_pieces = white_pieces + 1;
				}
			}
		}
	}

	//	DETERMINE THE SCORE TO RETURN

	int score;
	if (is_black) 
		score = black_pieces - white_pieces;
	else
		score = white_pieces - black_pieces;

	//	END GAME SOLVER:
	//	When Less Than 4 Total Moves Left just care about piece differential
	
	//	BEGINNING & MID-GAME SOLVER
	if (numBlankSpaces > 4) {
		if (is_black)
			score = (score + (b_score-w_score));
		else
			score = (score + (w_score-b_score));
	}
	
	//	Return the score
	return score;
}


void findAllMoves (char* initial, int depth, char own, char opp, Deque* queue, Tree play) {
	
	//	Array of all different directions
	int array[8] = {(-X_Max),(-X_Max+1),(1),(X_Max+1),(X_Max),(X_Max-1),(-1),(-X_Max-1)};
	
	// Try all the different directions from given square
	int x,y;
	for (y=0; y<Y_Max;y++)
		for (x=0;x<X_Max;x++) {
			//	Check if the board spot is empty
			bool made_move = false;
			if (initial[(y*Y_Max)+x]==' ') {
				char* board;
				copyBoardAtoB(initial, &board);
				// If it is, try every move in every direction
				for (int i=0; i<8; i++) {
					bool move_success = madeMove(board, ((y*Y_Max)+x), array[i], own, opp);
					if (!made_move && move_success)
						made_move = true;
				}
				//	If made a succcessful move at the move to the tree & queue
				if (made_move) {
					// Make a new node for this configuration
					Tree new_node;
					new_node = makeNode(board,depth);
					/* 
						Add the new configuration as a child of the current node
					*/
					Tree child = getChild(play);
					Tree prior_child;
					// If the child == null then this is the first child
					if (child == NULL)
						setChild(play, new_node);
					else {
						while (child != NULL) {
							prior_child = child;
							child = getNext(prior_child);
						}
						// Reached sibling that is NULL
						setSib(prior_child,new_node);
					}
					/*
						Add the new configuration to the queue
					*/
					addD(queue, new_node);
				}
				else
					free(board);	
			}
		}
}

// Try to move in the direction given by deviation
bool madeMove(char* board, int pos, int delta, char own, char opp) {
	
	// Check to see will not move off the board
	if (roomOnBoard(pos, delta)) {
		char disk = board[pos+delta];
		// Check that adjacent disk is opposite
		if (disk==opp) {
			// Try to see if the move works
			if (validMove(board, (pos+delta), delta, own)) {
				// If move is valid, execute the move, save the board
				// Set the current piece to be yours
				board[pos] = own;
				// Turn over all the other pieces before your own
				executeMove(board, (pos+delta), delta, own, opp);
				// printBoard(board);
				return true;
			}
			else 
				return false;
		}
		else
			return false;
	}
	else 
		return false;
}

void executeMove(char* board, int pos, int delta, char own, char opp) {
	char disk = board[pos];
	if (disk == opp) {
		board[pos] = own;
		executeMove(board,(pos+delta), delta, own, opp);
	}
}

bool validMove (char* board, int pos, int delta, char own) {
	if (roomOnBoard(pos, delta)) {
		// Disk in the next spot
		char disk = board[pos+delta]; 
		// If disk is blank, then invalid move
		if (disk == ' ')
			return false;
		// If disk is own then valid
		else if (disk == own)
			return true;
		// Otherwise disk is opposite so keep going in same direction
		else 
			return (validMove(board, (pos+delta), delta, own));
	}
	// Gone of the end of the board
	else 
		return false;
}

// Valid checks that 
bool roomOnBoard(int pos, int delta) {
	// Too far Up or Down
	if ((pos+delta) <0 || (pos+delta) >63) 
		return false;
	// Invalid top right
	else if (delta==(-X_Max+1) && (pos <=7 || (pos%8)==7))
		return false;
	// Invalid right
	else if (delta==(1) && (pos % 8)==7)
		return false;
	// Invalid bottom right
	else if (delta==(X_Max+1) && (pos>=56 || (pos%8)==7))
		return false;
	// Invalid bottom already captured
	// Invalid bottom left
	else if (delta ==(X_Max-1) && (pos>=56 || (pos%8)==0))
		return false;
	// Invalid left
	else if (delta==(-1) && (pos%8)==0)
		return false;
	// Invalid top left
	else if (delta==(-X_Max-1) && (pos<=7 || (pos%8)==0))
		return false;
	else 
		return true;
}

// Copies the first input to the second
void copyBoardAtoB (char* boardA, char** boardB) {
	char* board = malloc(sizeof(char)*BOARD_SIZE);
	int x,y;
	for (y=0; y<Y_Max;y++)
		for (x=0;x<X_Max;x++)
			board[(y*Y_Max)+x] = boardA[(y*Y_Max)+x];
	*boardB = board;
}

void makeFinalFileName(char* file_name, char** final_file_name) {
	char* name = strtok(file_name, "-");
	char* digit = strtok(NULL, "-");
	int num = atoi(digit);
	num++;
	char final_num[33];
	sprintf(final_num, "%02d", num);
	// char* final_file = strcat(name,"-");
	// final_file = strcat(final_file, final_num);
	// *final_file_name = final_file;

	*final_file_name = strcat(name,"-");
	*final_file_name = strcat(*final_file_name, final_num);
}

void printBoard (char* board) {
	char* bline = "  +---+---+---+---+---+---+---+---+";
	// char* line = "|   |   |   |   |   |   |   |   |";
	char* nline = "    1   2   3   4   5   6   7   8 ";
	int x,y;
	//	Print top of board
	printf("%s\n", nline);
	printf("%s\n",bline);
	for (y=0; y<Y_Max;y++) {
		printf("%d ",(y+1));
		for (x=0;x<X_Max;x++) {
			printf("| %c ",board[(y*X_Max)+x]);
		}
		printf("|\n");
		printf("%s\n", bline);
	}
}

void readBoard (char* file_name, Deque* queue, Tree* initial) {
	FILE* fp;
	char* line;
	char* board = malloc(sizeof(char)*BOARD_SIZE);
	fp = fopen(file_name,"r");
	if (!fp)
		DIE ("NO SUCH FILE");
	else {
		for (int y=0; y<Y_Max; y++) {
			line = getLine(fp);
			for (int x=0;x<X_Max;x++)
				board[(y*Y_Max)+x] = line[x];
		}
	}
	// Tree makeNode (char* b, int h)
	*initial = makeNode(board, 0);
	addD(queue, *initial);
	fclose(fp);
}

void writeBoard (char* final_file_name, char* board) {
	FILE* fp;
	fp = fopen(final_file_name,"w");
	int x,y;
	for (y=0; y<Y_Max;y++) {		
		for (x=0;x<X_Max;x++)
			fprintf(fp, "%c",board[(y*Y_Max)+x]);
		fprintf(fp, "\n");
	}
	fclose(fp);
}