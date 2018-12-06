//
//  main.cpp
//  MissionImprobable
//
//  Created by Philipp Gomez on 2018/09/20.
//  Copyright © 2018 Chaz. All rights reserved.
//

#include <iostream>
#include <string>
using namespace std;

// (100 * 11) + 99 + (buffer)
// 10^9: 11 chars
// 99 spaces
#define BUFF_SIZE 1280

double** get_input(int *rows, int *cols);
void free_input(double** heights, int rows);
void display_output(double** heights, int rows, int cols);
double steal_crates(double** heights, int rows, int cols);


double** get_input(int *rows, int *cols) {
    char buff[BUFF_SIZE];
    int line_idx=0;
    double **heights=NULL;
    int i=0, j=0;
    std::string line;
    char *t;
    double height=0;
    
    while (std::getline(std::cin, line)) {
        memset(buff, 0, sizeof(char) * BUFF_SIZE);
        strcpy(buff, line.c_str());
        if (line_idx == 0) {
            t = strtok(buff, " ");
            i=0;
            while (t) {
                if (i++ == 0) {
                    *rows = std::stoi(t);
                } else {
                    *cols = std::stoi(t);
                }
                t = strtok(NULL, " ");
            }
            if ((*rows < 1) || (*rows > 100) ||
                (*cols < 1) || (*cols > 100)) {
                // invalid rows or columns
                break;
            }
            
            heights = (double**) malloc(sizeof(double*) * (*rows));
            for (i=0; i<(*rows); i++) {
                heights[i] = (double*) malloc(sizeof(double) * (*cols));
            }
            
            line_idx++;
        } else {
            if (line_idx > (*rows)) {
                // excess rows
                free_input(heights, *rows);
                *rows = *cols = 0;
                break;
            }
            
            t= strtok(buff, " ");
            i=line_idx - 1;
            j=0;
            while (t) {
                height = std::stod(t);
                if ((height >= 0) && (height <= 10000000000)) {
                    // invalid rows or columns
                    heights[i][j++] = height;
                }
                t = strtok(NULL, " ");
            }
            
            line_idx++;
        }
        
        if (line_idx == (*rows + 1)) {
            break;
        }
    }
    
    return heights;
}

void free_input(double** heights, int rows) {
    for (int i=0; i<rows; i++) {
        free(heights[i]);
        heights[i] = NULL;
    }
    
    **heights = NULL;
}

void display_output(double** heights, int rows, int cols) {
    int i=0, j=0;
    
    cout << std::endl;
    for (i=0; i<rows; i++) {
        cout << "{" << i << "} ";
        for (j=0; j<cols; j++) {
            cout << "[" << j << "]" << heights[i][j] << " ";
        }
        cout << std::endl;
    }
}

double steal_crates(double** heights, int rows, int cols) {
    int i=0, j=0, k=0;
    int *max_row_heights=NULL;
    int *max_col_heights=NULL;
    double stolen=0;
    
    max_row_heights = (int*)malloc(sizeof(int) * rows);
    memset(max_row_heights, 0, sizeof(int) * rows);
    max_col_heights = (int*)malloc(sizeof(int) * cols);
    memset(max_col_heights, 0, sizeof(int) * cols);

    // get maximum heights
    for (i=0; i<rows; i++) {
        for (j=0; j<cols; j++) {
            if (heights[i][j] > heights[i][max_row_heights[i]]) {
                max_row_heights[i] = j;
            } else if (heights[i][j] == heights[i][max_row_heights[i]]) {
                for (k=0; k<i; k++) {
                    // check if column index already exists; replace with new column index
                    if (max_row_heights[i] == max_row_heights[k]) {
                        max_row_heights[i] = j;
                    }
                }
            }
            if (heights[i][j] > heights[max_col_heights[j]][j]) {
                max_col_heights[j] = i;
            } else if (heights[i][j] == heights[max_col_heights[j]][j]) {
                for (k=0; k<j; k++) {
                    // check if row index already exists; replace with new row index
                    if (max_col_heights[j] == max_col_heights[k]) {
                        max_col_heights[j] = i;
                    }
                }
            }
        }
    }
    
//    cout << std::endl;
//    for (i=0; i<rows; i++) {
//        cout << max_row_heights[i] << " ";
//    }
//    cout << std::endl;
//    for (i=0; i<cols; i++) {
//        cout << max_col_heights[i] << " ";
//    }
    
    for (i=0; i<rows; i++) {
        for (j=0; j<cols; j++) {
            if ((i != max_col_heights[j]) && (j != max_row_heights[i]) && (heights[i][j] > 1)) {
                stolen += (heights[i][j] - 1);
                heights[i][j] = 1;
            }
        }
    }
    
    if (max_col_heights != NULL) {
        free(max_col_heights);
    }
    if (max_row_heights != NULL) {
        free(max_row_heights);
    }
    return stolen;
}

int main(int argc, const char * argv[]) {
    double** heights=NULL;
    int rows=0;
    int cols=0;
    double stolen=0;
    
    heights = get_input(&rows, &cols);
    display_output(heights, rows, cols);
    stolen = steal_crates(heights, rows, cols);
    cout << stolen;
    cout << std::endl;
    display_output(heights, rows, cols);
    
    return 0;
}
