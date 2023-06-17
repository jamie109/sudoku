#include<iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <sstream>
#include "cxxopts.hpp"

using namespace std;

const int SIZE = 9;//������� 9x9
enum { EASY=1, MEDIUM, HARD };// �Ѷ�

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
            if (mysudoku[static_cast<std::vector<std::vector<int, std::allocator<int>>, std::allocator<std::vector<int, std::allocator<int>>>>::size_type>(start_row) + i][static_cast<std::vector<int, std::allocator<int>>::size_type>(strat_col) + j] == curnum) {
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
            printf("set D error or will use [min_blank, max_blank]\n");
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
    ofstream file(filename, /*ios::out | */ios::app); //׷�ӵ��ļ�ĩβ
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
    printf("--read Sudoku\n");
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
                //printf("0 ");
            }
            else {
                int cur_num = stoi(numStr);
                row.push_back(cur_num);
                //printf("%d ", cur_num);
            }
        }
        //printf("\n");
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
                //printf("No valid number can be placed at this cell\n");
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
        ("s,solve", "Solve sudoku games from game.txt", cxxopts::value<string>())
        ("g,games", "Number of sudoku games to solve", cxxopts::value<int>()->default_value("3"))//add
        ("n,generate", "Generate sudoku games", cxxopts::value<int>())
        ("m,difficulty", "Specify difficulty level for generated games", cxxopts::value<int>())
        ("r,blanks", "Specify number of blanks for generated games", cxxopts::value<string>())
        ("u", "Enable unique solution for generated games")
        ("t,test","for test")
        ;
    //options.parse_positional("count");
    try {
        //cout << "ok\n";
        auto result = options.parse(argc, argv);
        if (result.count("test")) {
            //���������վ�
            int final_sudolu_num = 2;
            int k = 0;
            while (final_sudolu_num > 0) {
                vector<vector<int>> su = genSudoku(false);
                saveTolocal(su, "final_sudoku.txt", k);
                k++;
                final_sudolu_num--;
            }
            //���ɸ���������Ϸ
            vector<vector<int>> su1 = genSudoku(true);
            setDifficulty(su1, -1, -1, -1);
            printSudoku(su1);
            saveTolocal(su1, "game.txt", 0);
            vector<vector<int>> su2 = genSudoku(false);
            setDifficulty(su2, -1, -1, -1);
            saveTolocal(su2, "game.txt", 1);

            vector<vector<int>> su3 = genSudoku(true);
            setDifficulty(su3, 1, -1, -1);
            saveTolocal(su3, "game.txt", 2);

            vector<vector<int>> su4 = genSudoku(false);
            setDifficulty(su4, -1, 26, 31);
            saveTolocal(su4, "game.txt", 3);

            vector<vector<int>> su5 = genSudoku(true);
            setDifficulty(su5, 2, -1, -1);
            saveTolocal(su5, "game.txt", 4);
            vector<vector<int>> su6 = genSudoku(true);
            setDifficulty(su6, 3, -1, -1);
            saveTolocal(su6, "game.txt", 5);
            vector<vector<int>> su7 = genSudoku(true);
            setDifficulty(su7, 4, -1, -1);
            saveTolocal(su7, "game.txt", 6);

            vector<vector<int>> su8 = genSudoku(true);
            setDifficulty(su8, -1, 10, 13);
            saveTolocal(su8, "game.txt", 7);

            string outputFile = "sudoku.txt"; // Ĭ������ļ���
            string inputfile_name = "game.txt";
            ifstream inputFile(inputfile_name);
            string game_index_line;
            getline(inputFile, game_index_line);
            int gameIndex = stoi(game_index_line);
            printf("--------------this is %d game-------------------------\n", gameIndex);
            vector<vector<int>> sudoku_todo = readSudoku(inputFile);
            printf("--solve the sudoku game\n");
            solveSudoku(sudoku_todo);
            //printSudoku(sudoku_todo);
            saveTolocal(sudoku_todo, "sudoku.txt", gameIndex);
        }
        // -c number number�������վֵ�final_sudoku.txt
        if (result.count("count")){
            if (result.count("difficulty") || result.count("blanks") || result.count("u")) {
                printf("ERROR: wrong command please retry\n");
                return 1;
            }
            int final_sudolu_num = result["count"].as<int>();
            //generateSudoku(count);
            printf("-c %d\n", final_sudolu_num);
            int k = 0;
            while (final_sudolu_num > 0) {
                vector<vector<int>> su = genSudoku(false);
                //printSudoku(su);
                saveTolocal(su, "final_sudoku.txt", k);
                k++;
                final_sudolu_num--;
            }
        }
        // -s
        else if (result.count("solve")){
            if (result.count("difficulty") || result.count("blanks") || result.count("u")) {
                printf("ERROR: wrong command please retry\n");
                return 1;
            }
            string inputfile_name = result["solve"].as<string>();
            cout << inputfile_name << endl;
            string outputFile = "sudoku.txt"; // Ĭ������ļ���
            //solveSudoku(inputFile, outputFile);
            printf("-s game.txt\n");
            //printf("���������Ϸ:\n");
            ifstream inputFile(inputfile_name);
            string game_index_line;
            //int solve_number = 3;//Ĭ���������
            int solve_number = result["games"].as<int>();
            printf("solve_number is %d\n", solve_number);
            while (solve_number > 0) {
                getline(inputFile, game_index_line);
                int gameIndex = stoi(game_index_line);
                printf("--------------this is %d game-------------------------\n", gameIndex);
                vector<vector<int>> sudoku_todo = readSudoku(inputFile);
                printf("--solve the sudoku game\n");
                solveSudoku(sudoku_todo);
                //printSudoku(sudoku_todo);
                saveTolocal(sudoku_todo, "sudoku.txt", gameIndex);
                solve_number--;
            }
            inputFile.close();
        }
        // -n [-m/-r -u]
        else if (result.count("generate")) {
            int sudoku_number = result["generate"].as<int>();
            printf("-n %d\n", sudoku_number);
            int difficulty = -1;
            int minBlanks = -1;
            int maxBlanks = -1;
            bool flag = true;//Ĭ��û��Ψһ��
            // -m /-r
            if (result.count("difficulty"))
            {
                difficulty = result["difficulty"].as<int>();
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
                    minBlanks = stoi(blanksRange.substr(0, pos));
                    maxBlanks = stoi(blanksRange.substr(pos + 1));
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
                flag = false;
            }
            int stage = 0;
            if (0 < difficulty && difficulty < 4 || 20 <= minBlanks && minBlanks <= maxBlanks && maxBlanks <= 55) {
                printf("very good you set difficulty right!\n");
                stage = 1;
            }
            else if (difficulty == -1 && minBlanks == -1 && maxBlanks == -1) {
                printf("you did not set difficulty so set the EASY game for you\n");
                difficulty = EASY;
                stage = 1;
            }
            else {
                printf("ERROR:wrong diffulty please reset\n");
                stage = -1;
            }
            int sudoku_index = 0;//��ʼ��������Ϊ0
            while (sudoku_number > 0 && stage == 1) {
                //flagĬ��trueû��Ψһ�⣬��� -u flagΪ false����fillSudoku2
                vector<vector<int>> su = genSudoku(flag);
                setDifficulty(su, difficulty, minBlanks, maxBlanks);
                //printSudoku(su);
                saveTolocal(su, "game.txt", sudoku_index);
                sudoku_index++;
                sudoku_number--;
            }
        }
        else {
            printf("ERROR: wrong command please retry\n");
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
    int k = 0;
    while (final_sudolu_num > 0) {
        vector<vector<int>> su = genSudoku(false);
        printSudoku(su);
        saveTolocal(su, "final_sudoku.txt", k);
        k++;
        final_sudolu_num--;
    }
    return 0;
    
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
    */
	return 0;
}