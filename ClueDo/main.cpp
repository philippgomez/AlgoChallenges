//
//  main.cpp
//  MissionImprobable
//
//  Created by Philipp Gomez on 2018/09/23.
//  Copyright © 2018 Chaz. All rights reserved.
//

#include <iostream>
#include <string>
#include <cmath>
using namespace std;

// 6 chars + 5 spaces + buffer
#define BUFF_SIZE 16
#define CARD_CNT 5

char** get_input(char cards[CARD_CNT], int *rows);
void free_input(double** heights, int rows);
void display_output(char cards[CARD_CNT], char **suggestions, int rows);
void process_clues(char cards[CARD_CNT], char** suggestions, int rows);

char** get_input(char cards[CARD_CNT], int *rows) {
    char buff[BUFF_SIZE];
    int line_idx=0;
    char **suggestions=NULL;
    int i=0, j=0;
    std::string line;
    char *t;
    
    while (1) {
        std::getline(std::cin, line);
        
        memset(buff, 0, sizeof(char) * BUFF_SIZE);
        strcpy(buff, line.c_str());
        if (line_idx == 0) {
            *rows = std::stoi(buff);
            if ((*rows < 1) || (*rows > 50)) {
                break;
            }
            suggestions = (char**)malloc(sizeof(char*) * (*rows));
            for (i=0; i<(*rows); i++) {
                suggestions[i] = (char*)malloc(sizeof(char) * 6);
                memset(suggestions[i], 0, sizeof(char) * 6);
            }
        } else if (line_idx == 1) {
            t = strtok(buff, " ");
            i=0;
            j=0;
            while (t) {
                if ((strlen(t) > 1) || (*t < 'A') || (*t > 'U')) {
                    break;
                }
                
                if (j > (CARD_CNT - 1)) {
                    // too many cards
                    break;
                } else {
                    cards[j++] = *t;
                }
                
                t = strtok(NULL, " ");
            }
        } else {
            t = strtok(buff, " ");
            i=0;
            j=0;
            while (t) {
                if ((strlen(t) > 1) || (!((*t == '-') || (*t == '*') || ((*t >= 'A') && (*t <= 'U'))))) {
                    break;
                }
                
                if (j > 5) {
                    // too many characters
                    break;
                } else {
                    suggestions[line_idx - 2][j++] = *t;
                }
                
                t = strtok(NULL, " ");
            }
        }
        line_idx++;

        if (line_idx == (*rows + 2)) {
            break;
        }
    }
    
    return suggestions;
}

void free_input(double** heights, int rows) {
    for (int i=0; i<rows; i++) {
        free(heights[i]);
        heights[i] = NULL;
    }
    
    **heights = NULL;
}

void display_output(char cards[CARD_CNT], char **suggestions, int rows) {
    int i=0, j=0;
    
    cout << std::endl;
    for (i=0; i<CARD_CNT; i++) {
        cout << cards[i] << " ";
    }
    cout << std::endl;

    for (i=0; i<rows; i++) {
        cout << "{" << i << "} ";
        for (j=0; j<6; j++) {
            cout << suggestions[i][j] << " ";
        }
        cout << std::endl;
    }
}

void process_clues(char cards[CARD_CNT], char** suggestions, int rows) {
    int i=0, j=0;
    int person_cnt=6, weapon_cnt=6, room_cnt=9;
    bool accusation=false;
    unsigned int persons=0x0000003F;
    unsigned int weapons=0x0000003F;
    unsigned int rooms=0x0000001FF;
    char ch=0;
    
    for (i=0; i<CARD_CNT; i++) {
        if ((cards[i] >= 'A') && (cards[i] < 'G')) {
            persons &= ~(1 << (cards[i] - 'A'));
            person_cnt--;
        } else if ((cards[i] > 'F') && (cards[i] < 'M')) {
            weapons &= ~(1 << (cards[i] - 'G'));
            weapon_cnt--;
        } else if ((cards[i] > 'L') && (cards[i] <= 'U')) {
            rooms &= ~(1 << (cards[i] - 'M'));
            room_cnt--;
        }
    }
    
//    cout << std::hex << persons << " " <<
//        std::hex << weapons << " " <<
//        std::hex << rooms << " " << std::endl;

    for (i=0; i<rows; i++) {
        if ((suggestions[i][3] == '-') && (suggestions[i][4] == '-') && (suggestions[i][5] == '-')) {
            accusation=true;
            break;
        }

        for (j=3; j<6; j++) {
            if ((suggestions[i][j] >= 'A') && (suggestions[i][j] < 'G')) {
                if (persons & (1 << (suggestions[i][j] - 'A'))) {
                    persons &= ~(1 << (suggestions[i][j] - 'A'));
                    person_cnt--;
                }
            } else if ((suggestions[i][j] > 'F') && (suggestions[i][j] < 'M')) {
                if (weapons & (1 << (suggestions[i][j] - 'G'))) {
                    weapons &= ~(1 << (suggestions[i][j] - 'G'));
                    weapon_cnt--;
                }
            } else if ((suggestions[i][j] > 'L') && (suggestions[i][j] <= 'U')) {
                if (rooms & (1 << (suggestions[i][j] - 'M'))) {
                    rooms &= ~(1 << (suggestions[i][j] - 'M'));
                    room_cnt--;
                }
            } else if (suggestions[i][j] == '*') {
                if (!(persons & (1 << (suggestions[i][0] - 'A'))) &&
                    !(weapons & (1 << (suggestions[i][1] - 'G')))) {
                    rooms &= ~(1 << (suggestions[i][2] - 'M'));
                    room_cnt--;
                } else if (!(persons & (1 << (suggestions[i][0] - 'A'))) &&
                           !(rooms & (1 << (suggestions[i][2] - 'M')))) {
                    weapons &= ~(1 << (suggestions[i][1] - 'G'));
                    weapon_cnt--;
                } else if (!(weapons & (1 << (suggestions[i][1] - 'G'))) &&
                           !(rooms & (1 << (suggestions[i][2] - 'M')))) {
                    persons &= ~(1 << (suggestions[i][0] - 'A'));
                    person_cnt--;
                }
            }
        }
    }

//    cout << std::hex << persons << " " <<
//    std::hex << weapons << " " <<
//    std::hex << rooms << " " << std::endl;

    if (!accusation) {
        if (person_cnt == 1) {
            ch = 'A'+ (int)log2(persons);
            cout << ch << " ";
        } else {
            cout << "? ";
        }
        if (weapon_cnt == 1) {
            ch = 'G' + (int)log2(weapons);
            cout << ch << " ";
        } else {
            cout << "? ";
        }
        if (room_cnt == 1) {
            ch = 'M' + (int)log2(rooms)   ;
            cout << ch << " ";
        } else {
            cout << "? ";
        }
    } else {
        cout << suggestions[i][0] << " " <<
            suggestions[i][1] << " " <<
            suggestions[i][2] << std::endl;
    }
}

int main(int argc, const char * argv[]) {
    char cards[CARD_CNT];
    char **suggestions=NULL;
    int rows=0;
    
    memset(cards, 0 ,sizeof(char) * CARD_CNT);
    suggestions = get_input(cards, &rows);
    //display_output(cards, suggestions, rows);
    process_clues(cards, suggestions,rows);
    
    return 0;
}
