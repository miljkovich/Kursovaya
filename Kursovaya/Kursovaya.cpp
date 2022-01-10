
#include "functions.cpp"

/*
���������� ������� ������� �� ����� � ���������� �������� �� ���������
��������������� ���, �� ����� �� k-�����. ���� ��� ��������, �� ������� ���
��������� ����, ���������� � ����.
��������� ������ ����������� � ��������� ������ � ����� �����������: ���
�������� �����, � ������� �������� ������� �� ��������� �����; ����� ����� k,
�� ������� ���������� ��������� ���. ���������� ������� �������� ������
������� �� �����.
*/
int main(int argC, char** argV)
{
    setlocale(LC_ALL, "RU");


    if (argC != 3) {
        cout << "�������� ���������� ����������";
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
