//
//  main.cpp
//  SudokuSolution
//
//  Created by 张雨 on 2019/7/11.
//  Copyright © 2019 张雨. All rights reserved.
//

/*-------------------------------------------------------------
 | 1 类的成员变量 xxxx_xxxxx_                                    |
 | 2 一普通变量一 xxx_xxx_xxx                                    |
 | 3 一函数名称一 xxxXxxXxxxx                                    |
 | 4 一函数形参一 _xxxx_xxxxx                                    |
 | 5 一一类名一一 XxxXxxxXxxx                                    |
 -------------------------------------------------------------*/

#include <iostream>
#include <vector>
using namespace std;

class SudokuSolution
{
public:
    SudokuSolution()
    {
        for(int r = 0;r < 9;r++)
        {
            for(int c = 0;c < 9;c++)
            {
                sudoku_[r][c] = 0;
            }
        }
    }
    SudokuSolution(vector<vector<int>> & _soduko)
    {
        
    }
    SudokuSolution(int _soduko[9][9])
    {
        for(int r = 0;r < 9;r++)
        {
            for(int c = 0; c< 9;c++)
            {
                sudoku_[r][c] = _soduko[r][c];
            }
        }
    }
    bool solution();
    void printSudoku();
private:
    struct PathNode
    {
        int row;
        int column;
        int num;
    };
    struct Coordinate
    {
        int row;
        int column;
    };
    
    typedef void (^ TraversalBlock)(int _r,int _c);
    
    int  sudoku_[9][9];
    
    bool available_num_[9][9][9];
    int  available_num_count_[9][9];
    
    int  remaining_pending_cell_count_;
    int  min_available_count_row_;
    int  min_available_count_column_;
    int  min_available_count_;
    
    static const Coordinate groups_[27][9];
    static const int associate_group_id_[9][9][3];
private:
    inline void initNumsAvailabel()
    {
        for(int r = 0;r <9;r++)
        {
            for(int c = 0;c <9;c++)
            {
                for(int n = 0;n<9;n++)
                    available_num_[r][c][n] = true;
            }
        }
    }
    inline void initAvailableNumCount()
    {
        for(int r = 0;r <9;r++)
        {
            for (int c = 0; c < 9; c++) {
                available_num_count_[r][c] = 9;
            }
        }
    }
    void getMinAvailable();
    
    void init();
    bool check();
    bool setNum(int row,int column,int num);
    void unsetNum(int row,int column);
    void avaliableOn(int row,int column);
    bool avaliable(int row,int column,int num);
    int  nextAvailableNum(int _row,int column,int _num);
    
    void traversalAssociateCell(int _row,int _column,TraversalBlock _block );
};

const SudokuSolution::Coordinate SudokuSolution::groups_[27][9] = {
    {{0,0},{0,1},{0,2},{0,3},{0,4},{0,5},{0,6},{0,7},{0,8}},
    {{1,0},{1,1},{1,2},{1,3},{1,4},{1,5},{1,6},{1,7},{1,8}},
    {{2,0},{2,1},{2,2},{2,3},{2,4},{2,5},{2,6},{2,7},{2,8}},
    {{3,0},{3,1},{3,2},{3,3},{3,4},{3,5},{3,6},{3,7},{3,8}},
    {{4,0},{4,1},{4,2},{4,3},{4,4},{4,5},{4,6},{4,7},{4,8}},
    {{5,0},{5,1},{5,2},{5,3},{5,4},{5,5},{5,6},{5,7},{5,8}},
    {{6,0},{6,1},{6,2},{6,3},{6,4},{6,5},{6,6},{6,7},{6,8}},
    {{7,0},{7,1},{7,2},{7,3},{7,4},{7,5},{7,6},{7,7},{7,8}},
    {{8,0},{8,1},{8,2},{8,3},{8,4},{8,5},{8,6},{8,7},{8,8}},
    
    {{0,0},{1,0},{2,0},{3,0},{4,0},{5,0},{6,0},{7,0},{8,0}},
    {{0,1},{1,1},{2,1},{3,1},{4,1},{5,1},{6,1},{7,1},{8,1}},
    {{0,2},{1,2},{2,2},{3,2},{4,2},{5,2},{6,2},{7,2},{8,2}},
    {{0,3},{1,3},{2,3},{3,3},{4,3},{5,3},{6,3},{7,3},{8,3}},
    {{0,4},{1,4},{2,4},{3,4},{4,4},{5,4},{6,4},{7,4},{8,4}},
    {{0,5},{1,5},{2,5},{3,5},{4,5},{5,5},{6,5},{7,5},{8,5}},
    {{0,6},{1,6},{2,6},{3,6},{4,6},{5,6},{6,6},{7,6},{8,6}},
    {{0,7},{1,7},{2,7},{3,7},{4,7},{5,7},{6,7},{7,7},{8,7}},
    {{0,8},{1,8},{2,8},{3,8},{4,8},{5,8},{6,8},{7,8},{8,8}},
    
    {{0,0},{0,1},{0,2},{1,0},{1,1},{1,2},{2,0},{2,1},{2,2}},
    {{3,0},{3,1},{3,2},{4,0},{4,1},{4,2},{5,0},{5,1},{5,2}},
    {{6,0},{6,1},{6,2},{7,0},{7,1},{7,2},{8,0},{8,1},{8,2}},
    {{0,3},{0,4},{0,5},{1,3},{1,4},{1,5},{2,3},{2,4},{2,5}},
    {{3,3},{3,4},{3,5},{4,3},{4,4},{4,5},{5,3},{5,4},{5,5}},
    {{6,3},{6,4},{6,5},{7,3},{7,4},{7,5},{8,3},{8,4},{8,5}},
    {{0,6},{0,7},{0,8},{1,6},{1,7},{1,8},{2,6},{2,7},{2,8}},
    {{3,6},{3,7},{3,8},{4,6},{4,7},{4,8},{5,6},{5,7},{5,8}},
    {{6,6},{6,7},{6,8},{7,6},{7,7},{7,8},{8,6},{8,7},{8,8}}
    
};

const int SudokuSolution::associate_group_id_[9][9][3] = {{{0,9,18},{0,10,18},{0,11,18},{0,12,21},{0,13,21},{0,14,21},{0,15,24},{0,16,24},{0,17,24}},
    {{1,9,18},{1,10,18},{1,11,18},{1,12,21},{1,13,21},{1,14,21},{1,15,24},{1,16,24},{1,17,24}},
    {{2,9,18},{2,10,18},{2,11,18},{2,12,21},{2,13,21},{2,14,21},{2,15,24},{2,16,24},{2,17,24}},
    {{3,9,19},{3,10,19},{3,11,19},{3,12,22},{3,13,22},{3,14,22},{3,15,25},{3,16,25},{3,17,25}},
    {{4,9,19},{4,10,19},{4,11,19},{4,12,22},{4,13,22},{4,14,22},{4,15,25},{4,16,25},{4,17,25}},
    {{5,9,19},{5,10,19},{5,11,19},{5,12,22},{5,13,22},{5,14,22},{5,15,25},{5,16,25},{5,17,25}},
    {{6,9,20},{6,10,20},{6,11,20},{6,12,23},{6,13,23},{6,14,23},{6,15,26},{6,16,26},{6,17,26}},
    {{7,9,20},{7,10,20},{7,11,20},{7,12,23},{7,13,23},{7,14,23},{7,15,26},{7,16,26},{7,17,26}},
    {{8,9,20},{8,10,20},{8,11,20},{8,12,23},{8,13,23},{8,14,23},{8,15,26},{8,16,26},{8,17,26}}};

/*-------------------------public function-------------------------*/
bool SudokuSolution::solution()
{
    init();
    if(!check())
        cout << "无解" <<endl;
    PathNode node;
    node.row    = min_available_count_row_;
    node.column = min_available_count_column_;
    node.num    = nextAvailableNum(node.row, node.column, 0);
    vector<PathNode> path;
    path.push_back(node);
    while (path.size() > 0) {
        if(path.back().row == 1 && path.back().column == 2 && path.back().num == 3)
            cout << "bug" << endl;
        cout << "row = "<<path.back().row << "column = " << path.back().column <<"num = "<<path.back().num <<endl;
        if( setNum(path.back().row, path.back().column, path.back().num) )
        {
            printSudoku();
            if(remaining_pending_cell_count_ == 0)
            {
                return true;
            }
            else
            {
                node.row    = min_available_count_row_;
                node.column = min_available_count_column_;
                node.num    = nextAvailableNum(node.row, node.column, 0);
                path.push_back(node);
            }
        }
        else
        {
            while ( (path.back().num  = nextAvailableNum(path.back().row, path.back().column, path.back().num) ) > 9 && path.size() > 0) {
                unsetNum(path.back().row, path.back().column);
                path.pop_back();
                
            }
            if(path.size()>0)
            {
                unsetNum(path.back().row,path.back().column);
            }
        }
    }
    return false;
}
void SudokuSolution::printSudoku()
{
    for(int r = 0;r < 9 ;r++)
    {
        for(int c = 0;c < 9 ;c++)
        {
            cout << sudoku_[r][c] << " ";
        }
        cout << endl;
    }
    cout << "剩余" << remaining_pending_cell_count_ << "个空格" << endl;
    cout << endl;
}
/*-------------------------public function-------------------------*/

/*-------------------------private function-------------------------*/
bool SudokuSolution::check()
{
    bool nums[9];
    for(int g = 0;g<27;g++)
    {
        for(int n = 0;n<9;n++)
            nums[n] = false;
        for(int i = 0;i <9;i++)
        {
            int row    = groups_[g][i].row;
            int column = groups_[g][i].column;
            int num    = sudoku_[row][column];
            if(num!= 0)
            {
                if(!nums[num-1])
                    nums[num-1] = true;
                else
                    return false;
            }
        }
    }
    return true;
}
void SudokuSolution::getMinAvailable()
{
    min_available_count_ = 9;
    for(int r = 0;r < 9;r++)
    {
        for(int c = 0; c < 9;c++)
        {
            if(sudoku_[r][c] == 0)
            {
                if(available_num_count_[r][c] < min_available_count_)
                {
                    min_available_count_     = available_num_count_[r][c];
                    min_available_count_row_ = r;
                    min_available_count_column_ = c;
                }
            }
        }
    }
}
void SudokuSolution::init()
{
    initNumsAvailabel();
    initAvailableNumCount();
    min_available_count_ = 9;
    remaining_pending_cell_count_ = 0;
    for(int r = 0;r < 9;r++)
    {
        for(int c =0;c <9;c++)
        {
            if(sudoku_[r][c] != 0)
            {
                for(int n = 0;n<9;n++)
                    available_num_[r][c][n] = false;
                available_num_count_[r][c] = 0;
            }
            else
            {
                remaining_pending_cell_count_ ++;
                
                for(int ix = 0;ix<3;ix++)
                {
                    int group_id = associate_group_id_[r][c][ix];
                    for(int i = 0;i<9;i++)
                    {
                        int s_r = groups_[group_id][i].row;
                        int s_c = groups_[group_id][i].column;
                        
                        int num = sudoku_[s_r][s_c];
                        if(num != 0)
                        {
                            if(available_num_[r][c][num-1] )
                            {
                                available_num_[r][c][num-1] = false;
                                available_num_count_[r][c] --;
                            }
                        }
                    }
                }
                if(available_num_count_[r][c] < min_available_count_)
                {
                    min_available_count_        = available_num_count_[r][c];
                    min_available_count_row_    = r;
                    min_available_count_column_ = c;
                }
            }
        }
    }
}
bool SudokuSolution::setNum(int row,int column,int num)
{
    int old_available_num_count[9][9];
    bool old_available_num[9][9][9];
    for(int i=0;i<9;i++)
    {
        for(int j=0;j<9;j++)
        {
            old_available_num_count[i][j] = available_num_count_[i][j];
            for(int k = 0;k<9;k++)
            {
                old_available_num[i][j][k] = available_num_[i][j][k];
            }
        }
    }
    sudoku_[row][column] = num;
    int old_available_count = min_available_count_;
    min_available_count_ = 9;
    for(int ix = 0;ix<3;ix++)
    {
        int group_id = associate_group_id_[row][column][ix];
        for(int i = 0;i<9;i++)
        {
            int r = groups_[group_id][i].row;
            int c = groups_[group_id][i].column;
            if(r != row || c != column)
            {
                if(sudoku_[r][c] == 0)
                {
                    if(available_num_[r][c][num-1])
                    {
                        available_num_count_[r][c] --;
                        available_num_[r][c][num-1] = false;
                        if(available_num_count_[r][c] == 0)
                        {
                            for(int i=0;i<9;i++)
                            {
                                for(int j=0;j<9;j++)
                                {
                                    available_num_count_[i][j] = old_available_num_count[i][j];
                                    for(int k = 0;k<9;k++)
                                    {
                                        available_num_[i][j][k] = old_available_num[i][j][k];
                                    }
                                }
                            }
                            min_available_count_ = old_available_count;
                            sudoku_[row][column] = 0;
                            return false;
                        }
                    }
                }
            }
        }
    }
    
    remaining_pending_cell_count_ --;
    available_num_count_[row][column] = 0;
    getMinAvailable();
    return true;
}
void SudokuSolution::unsetNum(int row,int column)
{
    
    int old_count = sudoku_[row][column];
    if(old_count == 0)
        return;
    sudoku_[row][column] = 0;

    remaining_pending_cell_count_ ++;

    available_num_count_[row][column] = 9;
    for(int num = 0;num < 9;num++)
        available_num_[row][column][num] = true;
    sudoku_[row][column] = 0;
    
   /* traversalAssociateCell(row, column, ^(int _r, int _c) {
        int num = sudoku_[_r][_c];
        if(num == 0)
        {
            if(avaliable(_r, _c, old_count))
            {
                available_num_count_[_r][_c] ++;
                available_num_[_r][_c][old_count-1] = true;
            }
        }
        else
        {
            if(available_num_[row][column][num-1])
            {
                available_num_[row][column][num-1] = false;
                available_num_count_[row][column] --;
            }
        }
    });*/
    
    for(int ix = 0;ix<3;ix++)
    {
        int group_id = associate_group_id_[row][column][ix];
        for(int i = 0;i<9;i++)
        {
            int r = groups_[group_id][i].row;
            int c = groups_[group_id][i].column;
            if(r != row || c != column)
            {
                int num = sudoku_[r][c];
                if(num == 0)
                {
                    if(avaliable(r, c, old_count))
                    {
                        available_num_count_[r][c] ++;
                        available_num_[r][c][old_count-1] = true;
                    }
                }
                else
                {
                    if(available_num_[row][column][num-1])
                    {
                        available_num_[row][column][num-1] = false;
                        available_num_count_[row][column] --;
                    }
                }
            }
        }
    }
}
void SudokuSolution::avaliableOn(int row,int column)
{
    for(int n = 0;n < 9;n++)
        available_num_[row][column][n] = true;
    available_num_count_[row][column]  = 9;
    
    traversalAssociateCell(row, column, ^(int _r, int _c) {
        int num = sudoku_[_r][_c];
        if(num != 0)
        {
            if(available_num_[_r][_c][num-1])
            {
                available_num_[_r][_c][num-1] = false;
                available_num_count_[_r][_c] --;
            }
        }
    });
}
bool SudokuSolution::avaliable(int row,int column,int num)
{
    for(int ix = 0;ix<3;ix++)
    {
        int group_id = associate_group_id_[row][column][ix];
        for(int i = 0;i<9;i++)
        {
            int r = groups_[group_id][i].row;
            int c = groups_[group_id][i].column;
            if(r!= row || c!=column)
            {
                if(sudoku_[r][c] == num)
                    return false;
            }
        }
    }
    return true;
}
int SudokuSolution::nextAvailableNum(int _row,int _column,int _num)
{
    int next_num = _num + 1;
    for(;next_num<=9;next_num++)
    {
        if(available_num_[_row][_column][next_num-1])
            break;
    }
    return next_num;
}
void SudokuSolution::traversalAssociateCell(int _row, int _column, TraversalBlock _block)
{
    for(int ix = 0;ix < 3;ix++)
    {
        int group_id = associate_group_id_[_row][_column][ix];
        for(int i = 0;i <9;i++)
        {
            int r = groups_[group_id][i].row;
            int c = groups_[group_id][i].column;
            if(r!= _row || c!= _column)
            {
                _block(r,c);
            }
        }
    }
}
/*-------------------------private function-------------------------*/
int main(int argc, const char * argv[]) {
    // insert code here...
    int sudoku[9][9] = {
        {0,0,0,0,6,0,0,2,0},
        {9,0,0,0,0,0,0,0,8},
        {5,0,3,0,0,0,0,0,0},
        {0,1,2,0,0,0,9,3,0},
        {6,8,4,9,0,5,1,7,0},
        {0,0,5,7,1,2,0,4,0},
        {4,0,8,3,2,0,7,6,0},
        {1,7,6,8,5,4,0,9,3},
        {2,0,9,6,0,1,5,8,0}
    }; // 入门
    int sudoku2[9][9] = {
        {0,1,0,0,6,0,0,0,0},
        {0,0,0,0,0,0,0,8,5},
        {0,6,9,0,2,4,0,1,0},
        {7,0,0,0,0,0,5,6,0},
        {0,5,0,6,1,0,0,2,0},
        {0,0,6,0,0,2,0,0,0},
        {0,0,2,0,0,0,0,9,0},
        {0,0,7,1,0,0,2,0,0},
        {4,0,0,0,8,9,0,0,0}
    }; // 地狱难度
    SudokuSolution s(sudoku2);
    s.solution();
    s.printSudoku();
    std::cout << "Hello, World!\n";
    return 0;
}
