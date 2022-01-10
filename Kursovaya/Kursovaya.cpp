
#include "functions.cpp"

/*
Необходимо оценить позицию на доске и определить возможно ли поставить
гарантированный мат, за белых за k-ходов. Если это возможно, то вывести все
возможные ходы, приводящие к мату.
Программа должна запускаться с командной строки с двумя параметрами: имя
входного файла, в котором хранится позиция на шахматной доске; число ходов k,
за которое необходимо поставить мат. Результаты анализа прогрмаа должна
вывести на экран.
*/
int main(int argC, char** argV)
{
    setlocale(LC_ALL, "RU");


    if (argC != 3) {
        cout << "Неверное количество параметров";
        exit(0);
    }
    FILE_ADDRESS = argV[1];
    int k = stoi(argV[2]);
    MOVES = k * 2 -1;
 
    /*
    MOVES = 1;
    FILE_ADDRESS = "chess_position.txt";
    */

    int** board = read_file(FILE_ADDRESS);
    STRUCT_HEAD = STRUCT_get_head(board);

    white_moves(board);
    system("pause");
}
