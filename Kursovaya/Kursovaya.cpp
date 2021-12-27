
#include "functions.cpp"

int main(int argC, char** argV)
{
    setlocale(LC_ALL, "RU");
    int** board = read_file(FILE_ADDRESS);
    STRUCT_HEAD = STRUCT_get_head(board);

    MOVES = 3;
    white_moves(board);

}
