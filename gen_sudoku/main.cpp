#include<iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <sstream>
#include "cxxopts.hpp"

using namespace std;

const int SIZE = 9;//数独规格 9x9
enum { EASY=1, MEDIUM, HARD };// 难度

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
//唯一解的数独
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
    // 尝试填入数字
    vector<int> nums{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    shuffle(nums.begin(), nums.end(), gen);//打乱顺序
    for (int num = 0; num < SIZE; num++) {
        int tmp = nums[num];
        if (num_ok(board, row, col, tmp)) {
            board[row][col] = tmp;
            if (fillSudoku2(board, row, col + 1)) {
                return true;  // 成功生成数独局
            }
        // 回溯
            board[row][col] = 0;
        }
    }
    return false;  
}

//生成数独（数独终盘）
vector<vector<int>> genSudoku(bool flag = true) {//flag==0 唯一解
    vector<vector<int>> sudoku(SIZE, vector<int>(SIZE, 0));
    //填充
    if(flag)
        fillSudoku(sudoku);
    else {
        srand(static_cast<unsigned int>(time(0)));
        fillSudoku2(sudoku, 0, 0);
    }
    //printSudoku(sudoku);
    return sudoku;
}
//挖空数字（最终需要求解的数独游戏）
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
    else if (min_blank >= 20 && min_blank <= max_blank && max_blank <= 55) {
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
        file << index << endl;
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
//从文件中读取数独
vector<vector<int>> readSudoku(ifstream& file) {
    printf("----------------------read Sudoku------------------\n");
    vector<std::vector<int>> sudoku;
    string line;
    for (int i = 0; i < SIZE; ++i) {
        getline(file, line);
        istringstream iss(line);
        string numStr;
        //每一行
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
    //file.close(); 不能在这里close，还没读完
    //printSudoku(sudoku);
    return sudoku;
}
//数独求解
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

int main(int argc, char* argv[]) {
    cxxopts::Options options("sudoku.exe", "Sudoku Command Line Utility");
    options.add_options()
        ("c,count", "Generate sudoku solutions", cxxopts::value<int>())
        ("s,solve", "Solve sudoku game", cxxopts::value<std::string>())
        ("n,generate", "Generate sudoku games", cxxopts::value<int>())
        ("m,difficulty", "Specify difficulty level for generated games", cxxopts::value<int>())
        ("r,blanks", "Specify number of blanks for generated games", cxxopts::value<std::string>())
        ("u", "Enable unique solution for generated games")
        ;
    //options.parse_positional("count");
    try {
        //cout << "ok\n";
        auto result = options.parse(argc, argv);
        // -c number number个数独终局到final_sudoku.txt
        if (result.count("count")){
            int final_sudolu_num = result["count"].as<int>();
            //generateSudoku(count);
            printf("-c %d\n", final_sudolu_num);
            int k = 0;
            while (final_sudolu_num > 0) {
                vector<vector<int>> su = genSudoku(false);
                printSudoku(su);
                saveTolocal(su, "final_sudoku.txt", k);
                k++;
                final_sudolu_num--;
            }
        }
        // -s
        else if (result.count("solve"))
        {
            string inputfile_name = result["solve"].as<string>();
            cout << inputfile_name << endl;
            string outputFile = "sudoku.txt"; // 默认输出文件名
            //solveSudoku(inputFile, outputFile);
            printf("-s game.txt\n");
            printf("求解数独游戏:\n\n");
            ifstream inputFile(inputfile_name);
            string game_index_line;
            int solve_number = 3;//默认求解三个
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
        }
        // -n [-m/-r -u]
        else if (result.count("generate")) {
            int num = result["generate"].as<int>();
            printf("-n %d\n", num);
            if (result.count("difficulty"))
            {
                int difficulty = result["difficulty"].as<int>();
                //generateSudokuGamesWithDifficulty(count, difficulty);
                printf("-m %d\n", difficulty);
            }
            else if (result.count("blanks")) {
                //cout << "in!\n";
                string blanksRange = result["blanks"].as<string>();
                //cout << blanksRange << endl;
                size_t pos = blanksRange.find('~');
                //cout << pos << " "<< string::npos<<endl;
                if (pos != string::npos)
                {
                    int minBlanks = stoi(blanksRange.substr(0, pos));
                    int maxBlanks = stoi(blanksRange.substr(pos + 1));
                    //generateSudokuGamesWithBlanks(count, minBlanks, maxBlanks);
                    printf("-r [%d, %d]\n", minBlanks, maxBlanks);
                }
                else
                {
                    cout << "Invalid blanks range. Please provide a range in the format of min~max." << std::endl;
                    return 1;
                }
            }
            if (result.count("u"))
            {
                printf("-u\n");
                // 在此处处理启用唯一解选项的逻辑
            }

        }
    }
    catch (const cxxopts::OptionException& e)
    {
        cout << "Error parsing command line options: " << e.what() << endl;
        return 1;
    }
    //if (argc < 2){
     //   printf("Invalid command. Please provide valid arguments.\n");
      //  return 1;
    //}
   // string command = argv[1];//-c -s -n
   // printf("%d", argc);
    /*
    //生成数独游戏
    printf("生成数独游戏:\n\n");
    string filename = "game.txt";
    int sudoku_index = 0;
    int sudoku_number = 5;//数独游戏数量
    int final_sudolu_num = 2;//数独终盘数量
    if (sudoku_number < 1 || sudoku_number > 1000000) {
        printf("sudoku_number set error\n");
        return -1;
    }
    
    //生成数独终盘
    //生成具有唯一解的数独
    //flag==0 唯一解
    int k = 0;
    while (final_sudolu_num > 0) {
        vector<vector<int>> su = genSudoku(false);
        printSudoku(su);
        saveTolocal(su, "final_sudoku.txt", k);
        k++;
        final_sudolu_num--;
    }
    return 0;
    
    //生成数独游戏
    while (sudoku_number > 0) { 
        vector<vector<int>> su = genSudoku(false);
        setDifficulty(su, 1, 23, 53);
        printSudoku(su);
        saveTolocal(su, filename, sudoku_index);
        sudoku_index ++;
        sudoku_number --;
    }
    //return 0;
    //求解数独
    int solve_number = 5;
    if (solve_number < 1 || solve_number > 10000) {
        printf("solve_number set error\n");
        return -1;
    }
    printf("\n求解数独游戏:\n\n");
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
    */
	return 0;
}