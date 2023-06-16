#include<iostream>
#include <random>
#include <vector>

using namespace std;
const int SIZE = 9;//数独规格 9x9
enum SudokuDifficulty {
    EASY, //20
    MEDIUM, //30
    HARD //40
};

// 打印
void printSudoku(const vector<vector<int>> &mysudoku) {
    cout << "---------------print sudoku------------------\n";
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) 
            cout << mysudoku[i][j] << " ";
        cout <<"\n";
    }
}
//生成数独
vector<vector<int>> genSudoku() {
    vector<vector<int>> sudoku(SIZE, vector<int>(SIZE, 0));
    //填充
    for (int row = 0; row < SIZE; row++) {

    }
    return sudoku;
}

int main() {
	//cout << "hello world!" << endl;
    vector<vector<int>> su = genSudoku();
    printSudoku(su);
	return 0;
}