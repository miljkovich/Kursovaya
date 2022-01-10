using namespace std;
#include <fstream>
#include <iostream>
#include <string>

#define BOARD_ROWS 8
#define BOARD_COLS 8

#define EMPTY_FIELD 0

#define WHITE_PAWN 1
#define WHITE_ROOK 2
#define WHITE_HORSE 3
#define WHITE_BISHOP 4
#define WHITE_QUEEN 5
#define WHITE_KING 6

#define BLACK_PAWN -1
#define BLACK_ROOK -2
#define BLACK_HORSE -3
#define BLACK_BISHOP -4
#define BLACK_QUEEN -5
#define BLACK_KING -6

//GLOBAL VARIABLE
int MOVES;
string FILE_ADDRESS;

struct variation
	/*STRUCT CONTAINS INFORMATION ABOUT ALL THE MOVES*/
{
	variation* pNext = nullptr;
	int** board = nullptr;
};

extern variation* STRUCT_HEAD = nullptr;

//FUNCTION IDENTIFIERS
void white_moves(int** board);
void black_moves(int** board);
void next_move(int** old_board, int old_row, int old_col, int new_row, int new_col, int PIECE, bool piece_is_white);
bool is_checked(int** board, bool white_is_checked);
void STRUCT_show_variation();
bool board_is_valid(int** board);




/*PRINT FUNCTIONS*/
void print_board(int** board)
{
	for (int row = 0; row < 8; row++)
	{
		for (int col = 0; col < 8; col++)
		{
			cout.width(3);
			cout << board[row][col];
		}
		cout << endl;
	}
	cout << endl << endl << endl;
}

/*READ FILE FUNCTIONS*/
int** get_2d_array(int rows = 8, int cols = 8)
{
	// creating 2d array
	int** arr = new int* [rows];
	for (int row = 0; row < 8; row++)
		arr[row] = new int[cols];

	return arr;
}
void delete_2d_array(int** arr,int rows=8)
{
	for (int i = 0; i < rows; i++)
		delete[] arr[i];

	delete[] arr;
}
int** read_file(string file_address)
{
	int** board = get_2d_array();


	//reading from file
	ifstream f;
	f.open(file_address);
	if (!f.is_open())
	{
		cout << "Не существует такой файл." << endl;
		system("exit");
	}
	
	// reading board
	for (int row = 0; row < 8; row++)
	{
		int value;
		for (int col = 0; col < 8; col++)
		{
			f >> value;
			board[row][col] = value;
		}
	}

	if (!board_is_valid(board))
	{
		cout << "Позиция на доске указана неправильно. " << endl;
		exit(0);
	}

	f.close();

	return board;
}
void copy_array(int** arr1, int** arr2)
{
	for (int row = 0; row < 8; row++)
		for (int col = 0; col < 8; col++)
			arr2[row][col] = arr1[row][col];
}

/* STRUCT AND ITS FUNCTIONS*/
struct variation* STRUCT_get_head(int** board)
{
	variation* head = new variation;
	head->board = board;
	return head;
}
void STRUCT_add_board(int** new_board)
{
	int** struct_board = get_2d_array();
	copy_array(new_board, struct_board);

	variation* new_variation = new variation;

	new_variation->board = struct_board;
	new_variation->pNext = STRUCT_HEAD;

	STRUCT_HEAD = new_variation;
	
}
void STRUCT_delete_board()
{
	variation* temp = STRUCT_HEAD;
	STRUCT_HEAD = STRUCT_HEAD->pNext;

	delete_2d_array(temp->board);
	delete temp;

}
void STRUCT_show_variation()
{
	variation* temp = STRUCT_HEAD;
	bool white_moves = true;
	int moves = 1;
	temp = STRUCT_HEAD;
	while (temp->pNext)
	{
		if (white_moves)
			cout << "White moves: (moves left = " << div(moves, 2).quot << ")" << endl;
		else
			cout << "Black moves: (moves left = " << div(moves,2).quot << ")" << endl;

		moves++;
		white_moves = !white_moves;
		print_board(temp->board);
		temp = temp->pNext;
	}

	cout << "Начальная позиция:" << endl;
	print_board(temp->board);
	
}

/* BOOL FUNCTIONS  */
bool is_white(int piece)
{
	return piece > 0;
}
bool is_black(int piece)
{
	return piece < 0;
}
bool is_piece(int piece)
{
	return piece != 0;
}
bool field_is_valid(int row, int col)
{
	return row >= 0 && row < 8 && col >= 0 && col < 8;
}


//Checks if board is valid
bool board_is_valid(int** board)
{
	int white_king = 0, black_king = 0 , white_pawns = 0, black_pawns = 0;
	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 8; col++) {
			int piece = board[row][col];
			switch (piece)
			{
			case WHITE_KING:
				white_king++;
				break;

			case BLACK_KING:
				black_king++;
				break;

			case WHITE_PAWN:
				white_pawns++;
				break;

			case BLACK_PAWN:
				black_pawns++;
				break;
			}
			if (black_king > 1 || white_king > 1 || black_pawns > 8 || white_pawns > 8) {
				return false;
			}
		}
	}
	return true;
}

/*
	Function checks if there is enemy king in certain direction
	direction is declared with row_dir, col_dir values that can be -1,0,+1
*/
bool king_in_direction(int** board, int row, int col, int row_dir, int col_dir, bool white_king_in_direction)
{
	int KING;
	if (white_king_in_direction)
		KING = WHITE_KING;
	else
		KING = BLACK_KING;

	// _row , _col are target row and col that are being increased by row_dir and col_dir every while itterration
	short int _row = row + row_dir; short int _col = col + col_dir;
	while (field_is_valid(_row, _col) && (board[_row][_col] == EMPTY_FIELD || board[_row][_col] == KING))
	{
		if (board[_row][_col] == KING)
			return true;

		_row += row_dir;
		_col += col_dir;
	}
	return false;

}

/* king_is_white - kralj u odnosu na koga se proverava da li je drugi kralj blizu
// row, col - koordinate polja u odnosu na koje se proverava da li je neprijateljski kralj blizu*/
bool king_is_not_near(int** board, int row, int col, bool king_is_white)

{
	int ENEMY_KING;
	// finding enemy king coordinates
	if (king_is_white)
		ENEMY_KING = BLACK_KING;
	else
		ENEMY_KING = WHITE_KING;

	int en_row, en_col;
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			if (board[i][j] == ENEMY_KING) {
				en_row = i; en_col = j; break;
			}

	int row_diff, col_diff;
	row_diff = abs(en_row - row);
	col_diff = abs(en_col - col);

	if ((row_diff <=1 ) && ( col_diff <=1))
		return false;
	return true;
}

/*
	Function checks if king can freely move in certain direction
	direction is declared with row_dir, col_dir values that can be -1,0,+1
*/
bool king_can_move(int** board, int king_row, int king_col, int row_dir, int col_dir, bool king_is_white)
{
	int** temp_board = get_2d_array();
	copy_array(board, temp_board);
	int KING;
	bool(*is_enemy)(int enemy_piece);
	if (king_is_white)
	{
		KING = WHITE_KING;
		is_enemy = &is_black;
	}
	else
	{
		KING = BLACK_KING;
		is_enemy = &is_white;
	}
	// _row , _col are target row and col for the field that king is trying to move to
	int _row, _col;
	_row = king_row + row_dir;
	_col = king_col + col_dir;


	if (field_is_valid(_row, _col)) {
		if ((temp_board[_row][_col] == EMPTY_FIELD || is_enemy(temp_board[_row][_col]))) {
			temp_board[_row][_col] = KING;
			temp_board[king_row][king_col] = EMPTY_FIELD;
			if (!is_checked(temp_board, false) && king_is_not_near(temp_board, _row, _col, king_is_white))
				return true;
		}
	}

	return false;
}

bool is_checked(int** board, bool white_is_checked)
{
	int king_col = 0; int king_row = 0;
	short int KING, QUEEN, BISHOP, HORSE, PAWN, ROOK;
	short int pawn_foward_row;

	// ADDIN GENERAL VALUES SO THAT FUNCTION WILL WORK IN BOTH CASES:
	// if we are checking if white is checked
	if (white_is_checked)
	{
		KING = WHITE_KING;		QUEEN = BLACK_QUEEN; ROOK = WHITE_ROOK;
		BISHOP = BLACK_BISHOP;  HORSE = BLACK_HORSE; PAWN = BLACK_PAWN;
		pawn_foward_row = 1;
	}
	// if we are checking if black king is checked
	else
	{
		KING = BLACK_KING;		QUEEN = WHITE_QUEEN; ROOK = WHITE_ROOK;
		BISHOP = WHITE_BISHOP;  HORSE = WHITE_HORSE; PAWN = WHITE_PAWN;
		pawn_foward_row = -1;

	}

	//finding king coordinates
	for (int row = 0; row < 8; row++)
	{
		for (int col = 0; col < 8; col++)
		{
			short int piece = board[row][col];
			if (piece == KING)
			{
				king_col = col;
				king_row = row;
				break;
			}

		}
	}

	//checking if king is under attack
	for (int row = 0; row < 8; row++)
	{
		for (int col = 0; col < 8; col++)
		{
			short int piece = board[row][col];
			if (piece == EMPTY_FIELD || piece == KING || piece == -KING)
				continue;

			if (piece == PAWN)
			{
				if ((col + 1 == king_col && row + pawn_foward_row == king_row) ||
					(col - 1 == king_col && row + pawn_foward_row == king_row))
					return true;

			}

			if (piece == HORSE)
			{
				if ((row - 2 == king_row && col == king_col) ||
					(row - 2 == king_row && col + 1 == king_col) ||
					(row - 1 == king_row && col + 2 == king_col) ||
					(row + 1 == king_row && col + 2 == king_col) ||
					(row + 2 == king_row && col + 1 == king_col) ||
					(row + 2 == king_row && col - 1 == king_col) ||
					(row + 1 == king_row && col - 2 == king_col) ||
					(row - 1 == king_row && col - 1 == king_col))
					return true;
			}

			if (piece == BISHOP || piece == QUEEN)
			{
				short int row_dir;
				short int col_dir;
				//^> UPPER - RIGHT DIAGONALE
				row_dir = -1; col_dir = +1;
				if (king_in_direction(board, row, col, row_dir, col_dir, white_is_checked))
					return true;

				// v>  DOWN - RIGHT DIAGONALE
				row_dir = +1; col_dir = +1;
				if (king_in_direction(board, row, col, row_dir, col_dir, white_is_checked))
					return true;

				//  <v DOWN - LEFT DIAGONALE
				row_dir = +1; col_dir = -1;
				if (king_in_direction(board, row, col, row_dir, col_dir, white_is_checked))
					return true;

				//<^ UPPER_LEFT DIAGONALE
				row_dir = -1; col_dir = -1;
				if (king_in_direction(board, row, col, row_dir, col_dir, white_is_checked))
					return true;

			}
			if (piece == ROOK || piece == QUEEN)
			{
				short int row_dir;
				short int col_dir;
				// -> RIGHT ->
				row_dir = 0; col_dir = +1;
				if (king_in_direction(board, row, col, row_dir, col_dir, white_is_checked))
					return true;

				// v DOWN v
				row_dir = +1; col_dir = 0;
				if (king_in_direction(board, row, col, row_dir, col_dir, white_is_checked))
					return true;

				// <- LEFT <-
				row_dir = 0; col_dir = -1;
				if (king_in_direction(board, row, col, row_dir, col_dir, white_is_checked))
					return true;

				// ^ UP   ^ 	
				row_dir = -1; col_dir = 0;
				if (king_in_direction(board, row, col, row_dir, col_dir, white_is_checked))
					return true;
			}
		}
	}
	return false;
}

bool is_mate(int** board, bool black_mated = true)
{
	bool(*is_enemy)(int enemy_piece);
	int MATED_KING;
	bool king_is_white;

	// MATED_KING - king that is being checked
	// is_enemy - KING's enemy piece 
	if (black_mated)
	{
		is_enemy = &is_white;
		MATED_KING = BLACK_KING;
		king_is_white = false;
	}
	else
	{
		is_enemy = &is_black;
		MATED_KING = WHITE_KING;
		king_is_white = true;
	}

	short int col, row, king_col, king_row;
	bool king_found = false;
	//finding king coordinates
	for (row = 0; row < 8 && !king_found; row++) {
		for (col = 0; col < 8; col++) {
			short int piece = board[row][col];
			if (piece == MATED_KING)
			{
				king_col = col;
				king_row = row;
				king_found = true;
				break;
			}
		}
	}

	if (!is_checked(board, king_is_white))
		return false;

	//If there is at least one free field that king can move to, return false
	//UP

	if (king_can_move(board, king_row, king_col, -1, 0, king_is_white))
		return false;

	// UP - RIGHT
	if (king_can_move(board, king_row, king_col, -1, +1, king_is_white))
		return false;

	//RIGHT
	if (king_can_move(board, king_row, king_col, 0, +1, king_is_white))
		return false;

	//RIGHT-DOWN
	if (king_can_move(board, king_row, king_col, +1, +1, king_is_white))
		return false;

	//DOWN
	if (king_can_move(board, king_row, king_col, +1, 0, king_is_white))
		return false;

	//LEFT-DOWN
	if (king_can_move(board, king_row, king_col, +1, -1, king_is_white))
		return false;

	//LEFT
	if (king_can_move(board, king_row, king_col, 0, -1, king_is_white))
		return false;

	//LEFT-UP
	if (king_can_move(board, king_row, king_col, -1, -1, king_is_white))
		return false;

	return true;
}

/* PIECE COVERAGE */
/* Function covers all the fields on the board in certain direction
	direction is declared with row_dir, col_dir values that can be -1,0,+1
*/
void cover_fields(int** board, int row, int col, int row_dir, int col_dir, bool piece_is_white)

{
	// Pointer to the function that is going to call the next move
	bool(*is_enemy_piece)(int enemy_piece);

	int PIECE = board[row][col];
	if (piece_is_white)
		is_enemy_piece = &is_black;
	else
		is_enemy_piece = &is_white;

	short int _row = row + row_dir; short int _col = col + col_dir;
	/* _row & _col are temp values for row and col.
	*  they are getting increased every while itteration
		by the direction value: row_dir, col_dir
	*/

	while (field_is_valid(_row, _col) &&
		(board[_row][_col] == EMPTY_FIELD ||
			is_enemy_piece(board[_row][_col])))
	{
		next_move(board, row, col, _row, _col, PIECE, piece_is_white);
		if (is_enemy_piece(board[_row][_col])) {
			break;
		}
		_row += row_dir; _col += col_dir;
	}
}



/* PIECE MOVES*/
void next_move(int** old_board, int old_row, int old_col, int new_row, int new_col, int PIECE, bool piece_is_white)
{

	//creating new board
	int** new_board = get_2d_array();
	copy_array(old_board, new_board);

	// making changes to the new board
	new_board[old_row][old_col] = EMPTY_FIELD;
	new_board[new_row][new_col] = PIECE;

	// Extends the structure by 1 move (1 board) 
	STRUCT_add_board(new_board);
	MOVES -= 1;

	// If piece is white and new move does not make white king checked:
	if (piece_is_white && !is_checked(new_board, true))
		black_moves(new_board);

	// If piece is black and new move does not make black king checked:
	if (!piece_is_white && !is_checked(new_board, false))
		white_moves(new_board);

	// Deletes last struct item
	STRUCT_delete_board();
	MOVES += 1;

	return;
}
void pawn_promotes(int** old_board, int old_row, int old_col, int promote_row, int promote_col, int PIECE, bool pawn_is_white)
{
	int HORSE, BISHOP, ROOK, QUEEN;
	//white pawn promotes
	if (pawn_is_white)
	{
		HORSE = WHITE_HORSE; BISHOP = WHITE_BISHOP; ROOK = WHITE_ROOK; QUEEN = WHITE_QUEEN;
	}
	else
	{
		HORSE = BLACK_HORSE; BISHOP = BLACK_BISHOP; ROOK = BLACK_ROOK; QUEEN = BLACK_QUEEN;
	}
	next_move(old_board, old_row, old_col, promote_row, promote_col, HORSE, pawn_is_white);
	next_move(old_board, old_row, old_col, promote_row, promote_col, BISHOP, pawn_is_white);
	next_move(old_board, old_row, old_col, promote_row, promote_col, ROOK, pawn_is_white);
	next_move(old_board, old_row, old_col, promote_row, promote_col, QUEEN, pawn_is_white);
}
void pawn_moves(int** board, int row, int col, bool pawn_is_white = true)
{
	short int PAWN, FORWARD_ROW, FORWARD_ROW_x2, LEFT_COL, RIGHT_COL, PROMOTE_ROW, PRE_PROMOTE_ROW, START_ROW;
	bool(*is_enemy)(int piece);
	if (pawn_is_white)
	{
		PAWN = WHITE_PAWN;   FORWARD_ROW = row-1;   FORWARD_ROW_x2 = row - 2;
		LEFT_COL = col - 1;  RIGHT_COL = col + 1;
		PRE_PROMOTE_ROW = 1; PROMOTE_ROW = 0;       START_ROW = 6;
		is_enemy = &is_black;
	}
	else
	{
		PAWN = BLACK_PAWN;    FORWARD_ROW = row + 1;    FORWARD_ROW_x2 = row + 2;
		LEFT_COL = col + 1;   RIGHT_COL = col-1;
		PRE_PROMOTE_ROW = 6;  PROMOTE_ROW = 7;    START_ROW = 1;
		is_enemy = &is_white;
	}

	// moves_forward 2 fields
	if (row == START_ROW && board[FORWARD_ROW][col] == EMPTY_FIELD && board[FORWARD_ROW_x2][col] == EMPTY_FIELD)
		next_move(board, row, col, FORWARD_ROW_x2, col, PAWN, false);

	// moves forward 1 field
	if (board[FORWARD_ROW][col] == EMPTY_FIELD)
	{
		//if pawn promotes
		if (row == PRE_PROMOTE_ROW)
			pawn_promotes(board, row, col, PROMOTE_ROW, col, PAWN, pawn_is_white);
		else
			next_move(board, row, col, FORWARD_ROW, col, PAWN, pawn_is_white);
	}

	// eats right
	if (field_is_valid(FORWARD_ROW, RIGHT_COL) && is_enemy(board[FORWARD_ROW][RIGHT_COL]))
	{
		if (row == PRE_PROMOTE_ROW)
			pawn_promotes(board,row,col, FORWARD_ROW, RIGHT_COL,PAWN, pawn_is_white);
		else
			next_move(board, row, col, FORWARD_ROW, RIGHT_COL, PAWN, pawn_is_white);
	}

	//eats left 
	if (field_is_valid (FORWARD_ROW, LEFT_COL) && is_enemy(board[FORWARD_ROW][LEFT_COL]))
	{
		if (row == PRE_PROMOTE_ROW)
			pawn_promotes(board, row, col, FORWARD_ROW, LEFT_COL, PAWN, pawn_is_white);
		else
			next_move(board, row, col, FORWARD_ROW, LEFT_COL, PAWN, pawn_is_white);
	}
}
void horse_moves(int** board, int row, int col, bool white = true)
{
	short int HORSE;

	// Pointer to the function that is going to call the next move
	bool(*is_enemy_piece)(int piece);
	if (white)
	{
		HORSE = WHITE_HORSE;
		is_enemy_piece = &is_black;
	}
	else
	{
		HORSE = BLACK_HORSE;
		is_enemy_piece = &is_black;
	}
	//All possible variations for horse to move
	if (field_is_valid(row - 2, col -1) && (board[row-2][col-1] == EMPTY_FIELD || is_enemy_piece(board[row - 2][col - 1])))
		next_move(board, row, col, row - 2, col - 1, HORSE, white);
	if (field_is_valid(row - 2, col + 1) && (board[row-2][col+1] == EMPTY_FIELD || is_enemy_piece(board[row - 2][col + 1])))
		next_move(board, row, col, row - 2, col + 1,HORSE, white);
	if (field_is_valid(row - 1, col + 2) && (board[row - 1][col + 2] == EMPTY_FIELD || is_enemy_piece(board[row - 1][col +2])))
		next_move(board, row, col, row - 1, col + 2, HORSE, white);
	if (field_is_valid(row + 1, col + 2) && (board[row + 1][col + 2] == EMPTY_FIELD || is_enemy_piece(board[row + 1][col + 2])))
		next_move(board, row, col, row + 1, col + 2, HORSE, white);
	if (field_is_valid(row + 2, col + 1) && (board[row + 2][col + 1] == EMPTY_FIELD || is_enemy_piece(board[row + 2][col + 1])))
		next_move(board, row, col, row + 2, col + 1, HORSE, white);
	if (field_is_valid(row+2, col- 1) && (board[row + 2][col - 1] == EMPTY_FIELD || is_enemy_piece(board[row + 2][col - 1])))
		next_move(board, row, col, row + 2, col - 1, HORSE, white);
	if (field_is_valid(row + 1, col - 2) && (board[row + 1][col - 2] == EMPTY_FIELD || is_enemy_piece(board[row +1][col - 2])))
		next_move(board, row, col, row + 1, col - 2, HORSE, white);
	if (field_is_valid(row - 1, col - 2) && (board[row - 1][col - 2] == EMPTY_FIELD || is_enemy_piece(board[row - 1][col - 2])))
		next_move(board, row, col, row - 1, col - 2, HORSE, white);
}
void bishop_moves(int** board, int row, int col, bool bishop_is_white = true)
{
	//^> UPPER - RIGHT DIAGONALE
	cover_fields(board, row,col,  -1, 1, bishop_is_white);

	// v>  DOWN - RIGHT DIAGONALE
	cover_fields(board, row,col, 1, 1, bishop_is_white);

	//  <v DOWN - LEFT DIAGONALE
	cover_fields(board, row, col, 1, -1, bishop_is_white);

	//<^ UPPER_LEFT DIAGONALE
	cover_fields(board, row,col, -1, -1, bishop_is_white);
}
void rook_moves(int** board, int row,int col, bool rook_is_white = true)
{
	//  UP
	cover_fields(board, row, col, -1, 0, rook_is_white);
	// -> RIGHT ->
	cover_fields(board, row, col, 0, +1, rook_is_white);
	// DOWN
	cover_fields(board, row, col, +1, 0, rook_is_white);
	//<- LEFT <-
	cover_fields(board, row, col, 0, -1, rook_is_white);
}
void queen_moves(int** board, int row, int col,bool queen_is_white = true)
{
	short int piece;
	piece = board[row][col];

	//  UP
	cover_fields(board, row, col, -1, 0, queen_is_white);
	//^> UPPER - RIGHT DIAGONALE
	cover_fields(board, row, col, -1, +1, queen_is_white);
	// -> RIGHT ->
	cover_fields(board, row, col, 0, +1, queen_is_white);
	// v>  DOWN - RIGHT DIAGONALE
	cover_fields(board, row, col, +1, +1, queen_is_white);
	// DOWN
	cover_fields(board, row, col, +1, 0, queen_is_white);
	//  <v DOWN - LEFT DIAGONALE
	cover_fields(board, row, col, +1, -1, queen_is_white);
	//<- LEFT <-
	cover_fields(board, row, col, 0, -1, queen_is_white);
	//<^ UPPER_LEFT DIAGONALE
	cover_fields(board, row, col, -1, -1, queen_is_white);

}
void king_moves(int** board, int row, int col, bool king_is_white = true)
{
	// Pointer to the function that is going to call the next move
	bool(*is_enemy_piece)(int enemy_piece);
	short int piece;
	piece = board[row][col];

	if (king_is_white)
		is_enemy_piece = &is_black;
	else
		is_enemy_piece = &is_white;

	//RIGHT-DOWN
	if (king_can_move(board, row, col, +1, +1, king_is_white))
		next_move(board, row, col, row + 1, col + 1, piece, king_is_white);
	//UP
	if (king_can_move(board,  row, col, -1, 0, king_is_white))
		next_move(board, row, col, row - 1, col, piece, king_is_white);

	// UP - RIGHT
	if (king_can_move(board, row, col, -1, +1, king_is_white))
		next_move(board, row, col, row - 1, col + 1, piece, king_is_white);

	//RIGHT
	if (king_can_move(board, row, col, -1, +1, king_is_white))
		next_move(board, row, col, row-1, col+1, piece, king_is_white);

	//DOWN
	if (king_can_move(board, row, col, +1, 0, king_is_white))
		next_move(board, row, col, row + 1, col, piece, king_is_white);
	//LEFT-DOWN
	if (king_can_move(board, row, col, +1, -1, king_is_white))
		next_move(board, row, col, row + 1, col-1, piece, king_is_white);
	//LEFT
	if (king_can_move(board, row, col, 0, -1, king_is_white))
		next_move(board, row, col, row, col-1, piece, king_is_white);
	//LEFT-UP
	if (king_can_move(board, row, col, -1, -1, king_is_white))
		next_move(board, row, col, row - 1, col-1, piece, king_is_white);
}


/* RECURSIVE NEXT MOVE CALLS */
void white_moves(int** board)
{ 
	if (MOVES <= 0 )
		return;

	for (int row = 0; row < 8; row++)
	{
		for (int col = 0; col < 8; col++)
		{
			int piece = board[row][col];
			if (piece == EMPTY_FIELD || is_black(piece))
				continue;

			switch (piece)
			{

			case WHITE_PAWN:
				pawn_moves(board, row,col);
				break;
			case WHITE_HORSE:
				horse_moves(board, row,col);
				break;
			case WHITE_BISHOP:
				bishop_moves(board, row,col);
				break;
			case WHITE_ROOK:
				rook_moves(board, row,col);
				break;
			case WHITE_QUEEN:
				queen_moves(board, row, col);
				break;
			case WHITE_KING:
				king_moves(board, row, col);
				break;
			}
		}
	}
}
void black_moves(int** board)
{
	if (is_mate(board))
	{
		cout << "GG WP! WE HAVE MATE!" << endl;
		STRUCT_show_variation();
		return;
	}
	else
	{
		if (MOVES <= 0)
		{
			///*
			cout << "_________________________________" << endl;
			cout << "ITS NOT MATE! POSSIBLE VARIATIONS:" << endl;
			cout << "_________________________________" << endl;
			STRUCT_show_variation();
			//*/
			return;
		}
	}
	

	for (int row = 0; row < 8; row++)
	{
		for (int col = 0; col < 8; col++)
		{
			int piece = board[row][col];
			if (piece == EMPTY_FIELD || is_white(piece))
				continue;

			switch (piece)
			{
			case BLACK_PAWN:
				pawn_moves(board, row, col,false);
				break;
			case BLACK_HORSE:
				horse_moves(board, row, col, false);
				break;
			case BLACK_BISHOP:
				bishop_moves(board, row, col, false);
				break;
			case BLACK_ROOK:
				rook_moves(board, row, col, false);
				break;
			case BLACK_QUEEN:
				queen_moves(board, row, col, false);
				break;
			case BLACK_KING:
				king_moves(board, row, col, false);
				break;
			}
		}
	}

}
