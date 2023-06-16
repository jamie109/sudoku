#include<iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <sstream>
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
                        //printf("����ʧ��\n");
                        mysudoku[row][col] = 0;
                    }                                        
                }
                return false;
            }
        }
    }
    return true;
}
//Ψһ�������
bool fillSudoku2(vector<vector<int>>& board, int row, int col) {
    if (col >= SIZE) {
        col = 0;
        row++;
        if (row >= SIZE) 
        return true;     
    }
    if (board[row][col] != 0) {
        return fillSudoku2(board, row, col + 1);  
    }
    // ������������
    vector<int> nums{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    shuffle(nums.begin(), nums.end(), gen);//����˳��
    for (int num = 0; num < SIZE; num++) {
        int tmp = nums[num];
        if (num_ok(board, row, col, tmp)) {
            board[row][col] = tmp;
            if (fillSudoku2(board, row, col + 1)) {
                return true;  // �ɹ�����������
            }
        // ����
            board[row][col] = 0;
        }
    }
    return false;  
}

//�����������������̣�
vector<vector<int>> genSudoku(bool flag = true) {//flag==0 Ψһ��
    vector<vector<int>> sudoku(SIZE, vector<int>(SIZE, 0));
    //���
    if(flag)
        fillSudoku(sudoku);
    else {
        srand(static_cast<unsigned int>(time(0)));
        fillSudoku2(sudoku, 0, 0);
    }
    //printSudoku(sudoku);
    return sudoku;
}
//�ڿ����֣�������Ҫ����������Ϸ��
void removeSudoku(vector<vector<int>>& mysudoku, int remove_num) {
    printf("remove %d from mysudoku\n", remove_num);
    uniform_int_distribution<> dist(0, SIZE - 1);
    //int count = SIZE * SIZE - remove_num;
    while (remove_num > 0) {
        int remove_row = dist(gen);
        int remove_col = dist(gen);
        if (mysudoku[remove_row][remove_col] != 0) {
            //printf("remove [%d][%d] = %d\n", remove_row, remove_col, mysudoku[remove_row][remove_col]);
            mysudoku[remove_row][remove_col] = 0;
            remove_num--;
        }
    }
}
//�����
int getNum(int start_num, int end_num) {
    uniform_int_distribution<int> distribution(start_num, end_num);
    return distribution(gen);
}
// �����Ѷȡ��ڿ�����
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
    else if (min_blank >= 20 && min_blank <= max_blank && max_blank <= 55) {
        int remove_num = getNum(min_blank, max_blank);
        removeSudoku(mysudoku, remove_num);
    }
    else {
        printf("error:please reset D or [min_blank, max_blank]\n");
    }
}
//���������������ļ�
void saveTolocal(const vector<vector<int>>& mysudoku, const string& filename, int index) {
    //ofstream file(filename);
    ofstream file(filename, ios::out | ios::app); //׷�ӵ��ļ�ĩβ
    if (file.is_open()) {
        file << index << endl;
        for (const auto& row : mysudoku) {
            for (int num : row) {
                if (num == 0)
                    file << "$ ";
                else
                    file << num << " "; 
            }
            file << endl; // д�뻻�з�
        }
        file.close();
        printf("sudoku has saved to local\n");
    }
}
//���ļ��ж�ȡ����
vector<vector<int>> readSudoku(ifstream& file) {
    printf("----------------------read Sudoku------------------\n");
    vector<std::vector<int>> sudoku;
    string line;
    for (int i = 0; i < SIZE; ++i) {
        getline(file, line);
        istringstream iss(line);
        string numStr;
        //ÿһ��
        vector<int> row;
        while (iss >> numStr) {
            if (numStr == "$") {
                row.push_back(0);
                printf("0 ");
            }
            else {
                int cur_num = stoi(numStr);
                row.push_back(cur_num);
                printf("%d ", cur_num);
            }
        }
        printf("\n");
        sudoku.push_back(row);               
    }
    //file.close(); ����������close����û����
    //printSudoku(sudoku);
    return sudoku;
}
//�������
bool solveSudoku(vector<vector<int>>& mysudoku) {
    //printf("solve the sudoku game\n");
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            if (mysudoku[row][col] == 0) {
                for (int num = 1; num <= 9; num++) {
                    if (num_ok(mysudoku, row, col, num)) {
                        mysudoku[row][col] = num;  
                        if (solveSudoku(mysudoku)) {
                            //printf("SOLVED!!!\n");
                            return true;  // Sudoku solved                  
                        }
                        mysudoku[row][col] = 0;  
                    }
                }
                printf("No valid number can be placed at this cell\n");
                return false; 
            }
        }
    }
    printf("SOLVED!!!\n");
    return true;
}
int main() {  
    //����������Ϸ
    printf("����������Ϸ:\n\n");
    string filename = "game.txt";
    int sudoku_index = 0;
    int sudoku_number = 5;//������Ϸ����
    int final_sudolu_num = 2;//������������
    if (sudoku_number < 1 || sudoku_number > 1000000) {
        printf("sudoku_number set error\n");
        return -1;
    }
    
    //������������
    //���ɾ���Ψһ�������
    //flag==0 Ψһ��
    /*
    int k = 0;
    while (final_sudolu_num > 0) {
        vector<vector<int>> su = genSudoku(false);
        printSudoku(su);
        saveTolocal(su, "final_sudoku.txt", k);
        k++;
        final_sudolu_num--;
    }
    return 0;
    */
    //����������Ϸ
    while (sudoku_number > 0) { 
        vector<vector<int>> su = genSudoku(false);
        setDifficulty(su, 1, 23, 53);
        printSudoku(su);
        saveTolocal(su, filename, sudoku_index);
        sudoku_index ++;
        sudoku_number --;
    }
    //return 0;
    //�������
    int solve_number = 5;
    if (solve_number < 1 || solve_number > 10000) {
        printf("solve_number set error\n");
        return -1;
    }
    printf("\n���������Ϸ:\n\n");
    ifstream inputFile("game.txt");
    string game_index_line;
    while (solve_number > 0) {
        getline(inputFile, game_index_line);
        int gameIndex = stoi(game_index_line);
        printf("--------------this is %d game-------------------------\n", gameIndex);
        vector<vector<int>> sudoku_todo = readSudoku(inputFile);
        printf("--------------solve the sudoku game--------------------\n");
        solveSudoku(sudoku_todo);
        //printSudoku(sudoku_todo);
        saveTolocal(sudoku_todo, "sudoku.txt", gameIndex);
        solve_number--;
    }
    inputFile.close();
	return 0;
}