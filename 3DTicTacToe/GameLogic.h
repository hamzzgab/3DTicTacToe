#ifndef GameLogic_h
#define GameLogic_h

// Std. Includes
#define GL_SILENCE_DEPRECATION
#include <iostream>
#include <map>
#include <string>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>


// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

// TicTacToe Properties
bool firstTac = true;
bool moveAval = true;
std::vector< int > store_i;
std::vector< int > store_j;
std::vector< int > store_k;
std::vector<vector<vector< int >>> store_tics;
std::vector< bool > store_tac;
GLint i = 0, j = 0, k = 0;
bool gameWin = false;
bool gameControls = false;

class GameLogic{
public:
    
    void init_game(){
        store_i.clear();
        store_j.clear();
        store_k.clear();
        store_tac.clear();
        i = 0;
        j = 0;
        k = 0;
        store_tics = initialize_tics(store_tics);
        firstTac = true;
        moveAval = check_move_available(moveAval, i, j, k);
        gameWin = false;
        gameControls = false;
    }
    
    std::vector<vector<vector< int >>> initialize_tics(std::vector<vector<vector< int >>> store_tics){
        store_tics.clear();
        for (int i=0; i<4; i++){
            // Insert Grid
            store_tics.push_back(vector<vector< int >>());
            for (int j=0; j<4; j++){
                // Insert Row
                store_tics[i].push_back(std::vector<int>());
                for (int k=0; k<4; k++){
                    // Insert Column
                    store_tics[i][j].push_back(-1);
                }
            }
        }
        return store_tics;
    }
    
    void check_win_status(){
        // WIN \ GRID DOWN
        int flag = 0;
        for ( int a=0; a<3; a++ ){
            for ( int b=0; b<4; b++ ){
                if ( store_tics[a][b][b] != -1){
                    if ( store_tics[a][b][b] == store_tics[a+1][b][b]){
                        flag++;
                        if (flag == 3) gameWin = true;
                    }
                }
            }
        }
        
        // WIN ----
        for ( int a=0; a<4; a++ ){
            for ( int b=0; b<4; b++ ){
                for ( int c=0; c<3; c++){
                    if ( store_tics[a][b][c] != -1){
                        if ( store_tics[a][b][c] == store_tics[a][b][c+1]){
                            if (c == 2) gameWin = true;
                        }
                    }
                }
            }
        }
        
        // WIN DIAG
        flag = 0;
        for ( int a=0; a<4; a++ ){
            for ( int b=0; b<3; b++ ){
                if ( store_tics[a][b][b] != -1){
                    if ( store_tics[a][b][b] == store_tics[a][b+1][b+1]){
                        flag++;
                        if (flag == 3) gameWin = true;
                    }
                }
            }
        }
        
        // WIN HORIZONTAL
        flag = 0;
        for ( int a=0; a<4; a++ ){
            for ( int b=0; b<4; b++ ){
                for ( int c=0; c<3; c++){
                    if ( store_tics[a][c][b] != -1){
                        if ( store_tics[a][c][b] == store_tics[a][c+1][b]){
                            flag++;
                            if (flag == 3) gameWin=true;
                        }
                    }
                }
            }
        }
        
        flag = 0;
        for ( int a=0; a<3; a++ ){
            for ( int b=0; b<4; b++ ){
                if ( store_tics[a][a][b] != -1){
                    if ( store_tics[a][a][b] == store_tics[a+1][a+1][b]){
                        flag++;
                        if (flag == 3) gameWin = true;
                    }
                }
            }
        }
        
        flag = 0;
        for ( int a=0; a<3; a++ ){
            for ( int b=0; b<4; b++ ){
                if ( store_tics[a][b][a] != -1){
                    if ( store_tics[a][b][a] == store_tics[a+1][b][a+1]){
                        flag++;
                        if (flag == 3) gameWin = true;
                    }
                }
            }
        }
    }
    
    bool check_move_available(bool moveAval, GLint i, GLint j, GLint k){
        if (store_tics[i][j][k] != -1){
            moveAval = false;
        }else{
            moveAval = true;
        }
        
        return moveAval;
    }
    
    void movement(bool keys[1024]){
        // MOVING LEFT
        if ( keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT] )
        {
            if (k > 0 ){
                k--;
            }else if (k == 0){
                k = 3;
                if (j > 0){
                    j--;
                }else if (j == 0){
                    j = 3;
                    if (i > 0){
                        i--;
                    }else if (i == 0){
                        i = 3;
                    }
                }
            }
            
            moveAval = check_move_available(moveAval, i, j, k);
        }
        
        // MOVING RIGHT
        if ( keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT] )
        {
            if (k < 3){
                k++;
            }else if (k == 3){
                k = 0;
                if (j < 3){
                    j++;
                }else if (j == 3){
                    j = 0;
                    if (i < 3){
                        i++;
                    }else if (i == 3){
                        i = 0;
                    }
                }
            }
            
            moveAval = check_move_available(moveAval, i, j, k);
        }
        
        // MOVING UP
        if ( keys[GLFW_KEY_W] || keys[GLFW_KEY_UP] )
        {
            if (j > 0){
                j--;
            }
            
            moveAval = check_move_available(moveAval, i, j, k);
        }
        
        // MOVING DOWN
        if ( keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN] )
        {
            if (j < 3){
                j++;
            }
            
            moveAval = check_move_available(moveAval, i, j, k);
        }
        
        // MOVING UP THE GRID
        if ( keys[GLFW_KEY_E] )
        {
            if ( i > 0 ){
                i--;
            }
            
            moveAval = check_move_available(moveAval, i, j, k);
        }
        
        // MOVING DOWN THE GRID
        if ( keys[GLFW_KEY_Q] )
        {
            if ( i < 3 ){
                i++;
            }
            
            moveAval = check_move_available(moveAval, i, j, k);
        }
        
        if(keys[GLFW_KEY_SPACE] && !gameWin) {
            if (store_tics[i][j][k] == -1){
                store_i.push_back(i);
                store_j.push_back(j);
                store_k.push_back(k);
                store_tics[i][j][k] = (int)firstTac;
                store_tac.push_back(firstTac);
                firstTac = !firstTac;
                moveAval = true;
            }else {
                moveAval = false;
            }
        }
        
        if(keys[GLFW_KEY_Y]) {
            gameControls = !gameControls;
        }
        
        if(keys[GLFW_KEY_R]) {
            init_game();
        }
    }
    
};


#endif /* GameLogic_h */
