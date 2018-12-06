//
//  main.cpp
//  MissionImprobable
//
//  Created by Philipp Gomez on 2018/09/27.
//  Copyright © 2018 Chaz. All rights reserved.
//

#include <iostream>
#include <string>
#include <cmath>
using namespace std;

// 10E6 (8) * 2 + 1 (space) + buffer
#define BUFF_SIZE 24
#define VERTICES 2

float** get_input(int *rows);
void free_input(float** vertices, int rows);
void display_output(float** vertices, int rows);
int get_longest_strip(float** vertices, int rows, int* vertex1, int*vertex2);
float get_x(float slope, float slope_int, float* vertex);
float get_y(float slope, float slope_int, float* vertex);

float** get_input(int *rows) {
    char buff[BUFF_SIZE];
    int line_idx=0;
    float **vertices=NULL;
    int i=0, j=0;
    std::string line;
    char *t;
    
    while (1) {
        std::getline(std::cin, line);
        
        memset(buff, 0, sizeof(char) * BUFF_SIZE);
        strcpy(buff, line.c_str());
        if (line_idx == 0) {
            *rows = std::stoi(buff);
            if ((*rows < 3) || (*rows > 200)) {
                break;
            }
            vertices = (float**)malloc(sizeof(float*) * (*rows));
            for (i=0; i<(*rows); i++) {
                vertices[i] = (float*)malloc(sizeof(float) * VERTICES);
                memset(vertices[i], 0, sizeof(float) * VERTICES);
            }
        } else {
            t = strtok(buff, " ");
            i=0;
            j=0;
            while (t) {
                if (j > VERTICES) {
                    // too many vertices
                    break;
                } else {
                    vertices[line_idx - 1][j++] = std::stof(t);
                }
                
                t = strtok(NULL, " ");
            }
        }
        line_idx++;
        
        if (line_idx == (*rows + 1)) {
            break;
        }
    }
    
    return vertices;
}

void free_input(float** vertices, int rows) {
    for (int i=0; i<rows; i++) {
        free(vertices[i]);
        vertices[i] = NULL;
    }
    
    **vertices = NULL;
}

void display_output(float** vertices, int rows) {
    int i=0, j=0;
    
    cout << std::endl;
    for (i=0; i<rows; i++) {
        cout << "{" << i << "} ";
        for (j=0; j<VERTICES; j++) {
            cout << vertices[i][j] << " ";
        }
        cout << std::endl;
    }
}

int get_longest_strip(float** vertices, int rows, int* vertex1, int* vertex2) {
    int i=0, j=0, k=0;
    float slope=0, slope_int=0;
    int length=0, max_length=0;
    bool valid=true;
    
    for(i=0; i<rows; i++) {
        for (j=i+1; j<rows; j++) {
            // get longest interval from current vertex
            // get slope
            slope = (vertices[j][1] - vertices[i][1])/(vertices[j][0] - vertices[i][0]);
            slope_int = vertices[j][1] - (slope * vertices[j][0]);
            valid = true;
            for (k=i+1; k<j; k++) {
//                if ((slope < 0) && ((vertices[k][1] < get_y(slope, slope_int, vertices[k])) ||
//                                    (vertices[k][0] < get_x(slope, slope_int, vertices[k])))) {
                if (vertices[k][1] < get_y(slope, slope_int, vertices[k])) {
                    valid = false;
                    break;
                }
//                } else if ((slope > 0) && ((vertices[k][1] < get_y(slope, slope_int, vertices[k])) ||
//                                           (vertices[k][0] > get_x(slope, slope_int, vertices[k])))) {
//                    valid = false;
//                    break;
//                }
            }
        
            if (valid) {
                // get length
                length = sqrt(pow(vertices[j][1] - vertices[i][1], 2) +
                              pow(vertices[j][0] - vertices[i][0], 2));
                if (length > max_length) {
                    *vertex1 = i;
                    *vertex2 = j;
                    max_length = length;
                }
            }
        }
    }
    return max_length;
}

float get_x(float slope, float slope_int, float* vertex) {
    return ((vertex[1] - slope_int) / slope);
}

float get_y(float slope, float slope_int, float* vertex) {
    return (slope * vertex[0]) + slope_int;
}

int main(int argc, const char * argv[]) {
    float **vertices=NULL;
    int rows=0;
    int vertex1=0, vertex2=0;
    
    vertices = get_input(&rows);
    //display_output(edges, rows);
    cout << get_longest_strip(vertices, rows, &vertex1, &vertex2) << std::endl;
    cout << "between {" << vertices[vertex1][0] << "," << vertices[vertex1][1] << "} and {" <<
    vertices[vertex2][0] << "," << vertices[vertex2][1] << "}" << std::endl;
    
    return 0;
}
