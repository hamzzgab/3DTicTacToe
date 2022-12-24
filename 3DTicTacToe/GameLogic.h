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


class GameLogic{
public:
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
        if ( keys[GLFW_KEY_Q] )
        {
            if ( i > 0 ){
                i--;
            }
            
            moveAval = check_move_available(moveAval, i, j, k);
        }
        
        // MOVING DOWN THE GRID
        if ( keys[GLFW_KEY_E] )
        {
            if ( i < 3 ){
                i++;
            }
            
            moveAval = check_move_available(moveAval, i, j, k);
        }
        
        if(keys[GLFW_KEY_SPACE]) {
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
        
        if(keys[GLFW_KEY_R]) {
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
        }
    }
    
};


#endif /* GameLogic_h */
