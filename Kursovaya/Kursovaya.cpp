
#include "functions.cpp"

int main()
{
    setlocale(LC_ALL, "RU");
    int** board = read_file(FILE_ADDRESS);
    STRUCT_HEAD = STRUCT_get_head(board);


    MOVES = 2;
    white_moves(board);
}

