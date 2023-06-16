#include<iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <fstream>

using namespace std;

const int SIZE = 9;//数独规格 9x9
enum { EASY, MEDIUM, HARD };// 难度

random_device rd;
mt19937 gen(rd());
// 打印
void printSudoku(const vector<vector<int>> &mysudoku) {
    cout << "---------------print sudoku------------------\n";
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) 
            cout << mysudoku[i][j] << " ";
        cout <<"\n";
    }
}
// 数字是否可以放置在指定位置
bool num_ok(vector<vector<int>>& mysudoku,int row, int col, int curnum) {
    bool isOK = true;
    for (int i = 0; i < SIZE; i++) {
        if (isOK == false)
            break;
        if (mysudoku[row][i] == curnum) {
            //printf("row: no\n");
            isOK = false;
            //break;
        }
    }
    for (int i = 0; i < SIZE; i++) {
        if (isOK == false)
            break;
        if (mysudoku[i][col] == curnum) {
            //printf("col: no\n");
            isOK = false;
            //break;
        }
    }
    int start_row = row - row % 3;
    int strat_col = col - col % 3;
    for (int i = 0; i < 3; i++) {
        if (isOK == false)
            break;
        for (int j = 0; j < 3; j++) {
            if (mysudoku[start_row + i][strat_col + j] == curnum) {
                //printf("9: no\n");
                isOK = false;
                //break;
            }
        }
    }
    return isOK;
}
//填充数独
bool fillSudoku(vector<vector<int>>& mysudoku) {
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            if (mysudoku[row][col] == 0) {
                vector<int> nums{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
                shuffle(nums.begin(), nums.end(), gen);//打乱顺序
                for (int i = 0; i < SIZE; i++) {
                    int cur_num = nums[i];
                    //当前数字是否有效
                    if (num_ok(mysudoku, row, col, cur_num)) {
                        mysudoku[row][col] = cur_num;
                        if (fillSudoku(mysudoku)) //回溯
                            return true;
                        //回溯失败
                        //printf("回溯失败\n");
                        mysudoku[row][col] = 0;
                    }                                        
                }
                return false;
            }
        }
    }
    return true;
}
//生成数独
vector<vector<int>> genSudoku(){
    vector<vector<int>> sudoku(SIZE, vector<int>(SIZE, 0));
    //填充
    fillSudoku(sudoku);
    //printSudoku(sudoku);
    return sudoku;
}
//挖空数字
void removeSudoku(vector<vector<int>>& mysudoku, int remove_num) {
    printf("remove %d from mysudoku\n", remove_num);
    uniform_int_distribution<> dist(0, SIZE - 1);
    int count = SIZE * SIZE - remove_num;
    while (remove_num > 0) {
        int remove_row = dist(gen);
        int remove_col = dist(gen);
        if (mysudoku[remove_row][remove_col] != 0) {
            printf("remove [%d][%d] = %d\n", remove_row, remove_col, mysudoku[remove_row][remove_col]);
            mysudoku[remove_row][remove_col] = 0;
            remove_num--;
        }
    }
}
//随机数
int getNum(int start_num, int end_num) {
    uniform_int_distribution<int> distribution(start_num, end_num);
    return distribution(gen);
}
// 设置难度、挖空数量
void setDifficulty(vector<vector<int>>& mysudoku, int D = -1, int min_blank = -1, int max_blank = -1) {
    // enum { EASY, MEDIUM, HARD };
    printf("---------------set Difficulty----------------\n");
    if (D != -1) {
        switch (D)
        {
        case EASY:
            removeSudoku(mysudoku, 20);
            break;
        case MEDIUM:
            removeSudoku(mysudoku, 30);
            break;
        case HARD:
            removeSudoku(mysudoku, 40);
            break;
        default:
            printf("set D error or use [min_blank, max_blank]\n");
            break;
        }
    }
    if (min_blank > -1 && min_blank <= max_blank) {
        int remove_num = getNum(min_blank, max_blank);
        removeSudoku(mysudoku, remove_num);
    }
    else {
        printf("error:please reset D or [min_blank, max_blank]\n");
    }
}
//生成数独到本地文件
void saveTolocal(const vector<vector<int>>& mysudoku, const string& filename, int index) {
    //ofstream file(filename);
    ofstream file(filename, ios::out | ios::app); //追加到文件末尾
    if (file.is_open()) {
        file << "game" << index << endl;
        for (const auto& row : mysudoku) {
            for (int num : row) {
                if (num == 0)
                    file << "$ ";
                else
                    file << num << " "; 
            }
            file << endl; // 写入换行符
        }
        file.close();
        printf("sudoku has saved to local\n");
    }
}
int main() {
	//cout << "hello world!" << endl;
    vector<vector<int>> su = genSudoku();
    setDifficulty(su, 5, 14, 23);
    printSudoku(su);
    string filename = "game.txt";
    int sudoku_index = 0;
    saveTolocal(su, filename, sudoku_index);
	return 0;
}