#include <iostream>
#include <vector>
#include <string>

int col=0;
int row=0;

void dfs(std::vector<std::vector<int> >& matrix, int index_x, int index_y) {
    if(index_x < 0 || index_x >=row || index_y < 0 || index_y >= col) {
        return;
    }
    if(matrix[index_x][index_y] == 0) {
        return;
    }
    if(matrix[index_x][index_y] == 1) {
        matrix[index_x][index_y] = 0;
        dfs(matrix, index_x - 1, index_y);
        dfs(matrix, index_x + 1, index_y);
        dfs(matrix, index_x, index_y - 1);
        dfs(matrix, index_x, index_y + 1);
    }
}

int main() {
    std::string str;

    std::cin>>row>>col;
    std::vector<std::vector<int> > matrix(row, std::vector<int>(col, 0));
    for(int i = 0; i < row; ++i) {
        std::vector<int> arr(col, 0);
        for(int j = 0; j < col; ++j) {
            int val;
            std::cin>> val;
            arr[j] = val;
        }
        matrix[i] = arr;
    }
    int island_num = 0;
    for(int i = 0; i <row; ++i) {
        for(int j = 0; j <col; ++j) {
            if(matrix[i][j] == 1) {
                dfs(matrix,i,j);
                island_num++;
            }
        }    
    }
    std::cout<<island_num<<std::endl;
}
// 5 5
// [[1,1,0,0,0],
// [0,1,0,1,1],
// [0,0,0,1,1],
// [0,0,0,0,0],
// [0,0,1,1,1]]
