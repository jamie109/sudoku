#include<iostream>
#include <random>
#include <vector>
#include <algorithm>

using namespace std;

const int SIZE = 9;//������� 9x9
enum { EASY, MEDIUM, HARD };// �Ѷ�

random_device rd;
mt19937 gen(rd());
// ��ӡ
void printSudoku(const vector<vector<int>> &mysudoku) {
    cout << "---------------print sudoku------------------\n";
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) 
            cout << mysudoku[i][j] << " ";
        cout <<"\n";
    }
}

// �����Ƿ���Է�����ָ��λ��
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
//�������
bool fillSudoku(vector<vector<int>>& mysudoku) {
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            if (mysudoku[row][col] == 0) {
                vector<int> nums{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
                shuffle(nums.begin(), nums.end(), gen);//����˳��
                for (int i = 0; i < SIZE; i++) {
                    int cur_num = nums[i];
                    //��ǰ�����Ƿ���Ч
                    if (num_ok(mysudoku, row, col, cur_num)) {
                        mysudoku[row][col] = cur_num;
                        if (fillSudoku(mysudoku)) //����
                            return true;
                        //����ʧ��
                        mysudoku[row][col] = 0;
                    }
                    
                    
                }
                return false;
            }
        }
    }
    return true;
}
//��������
vector<vector<int>> genSudoku(){
    vector<vector<int>> sudoku(SIZE, vector<int>(SIZE, 0));
    //���
    fillSudoku(sudoku);
    //printSudoku(sudoku);
    return sudoku;
}

int main() {
	//cout << "hello world!" << endl;
    vector<vector<int>> su = genSudoku();
    printSudoku(su);
	return 0;
}