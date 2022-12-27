#ifndef Object_h
#define Object_h

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

// GL includes
#include "Shader.h"
#include "Camera.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

Camera camera( glm::vec3( 0.2f, 0.0f, 0.0f ) );

// Vector3 Arrays Containing the locations
glm::vec3 curr_grid[] = {
    glm::vec3( 0.0f, 60.0f, 0.0f ),
    glm::vec3( 0.0f, 20.0f, 0.0f ),
    glm::vec3( 0.0f, -20.0f, 0.0f ),
    glm::vec3( 0.0f, -60.0f, 0.0f ),
};

glm::vec3 grid_pos[4][4][4] = {
    {       // FIRST GRID
        {
            glm::vec3(-19.5f, 0.0f, -20.0f), // 0, 0, 0
            glm::vec3(-6.5f, 0.0f, -20.0f),  // 0, 0, 1
            glm::vec3(6.5f, 0.0f, -20.0f),   // 0, 0, 2
            glm::vec3(19.5f, 0.0f, -20.0f)   // 0, 0, 3
        },
        {
            glm::vec3(-19.5f, 0.0f, -7.0f),  // 0, 1, 0
            glm::vec3(-6.5f, 0.0f, -7.0f),   // 0, 1, 1
            glm::vec3(6.5f,  0.0f, -7.0f),   // 0, 1, 2
            glm::vec3(19.5f, 0.0f, -7.0f),   // 0, 1, 3
        },
        {
            glm::vec3(-20.0f, 0.5f, 7.5f),   // 0, 2, 0
            glm::vec3(-7.0f, 0.5f, 7.5f),    // 0, 2, 1
            glm::vec3(6.0f, 0.5f, 7.5f),     // 0, 2, 2
            glm::vec3(19.0f, 0.5f, 7.5f),    // 0, 2, 3
        },
        {
            glm::vec3(-20.0f, 0.5f, 20.0f),  // 0, 3, 0
            glm::vec3(-7.0f, 0.5f, 20.0f),   // 0, 3, 1
            glm::vec3(6.0f, 0.5f, 20.0f),    // 0, 3, 2
            glm::vec3(19.0f, 0.5f, 20.0f),   // 0, 3, 3
        }
    },{     // SECOND GRID
        {
            glm::vec3(-19.5f, 0.0f, -20.0f), // 1, 0, 0
            glm::vec3(-6.5f, 0.0f, -20.0f),  // 1, 0, 1
            glm::vec3(6.5f, 0.0f, -20.0f),   // 1, 0, 2
            glm::vec3(19.5f, 0.0f, -20.0f)   // 1, 0, 3
        },
        {
            glm::vec3(-19.5f, 0.0f, -7.0f),  // 1, 1, 0
            glm::vec3(-6.5f, 0.0f, -7.0f),   // 1, 1, 1
            glm::vec3(6.5f,  0.0f, -7.0f),   // 1, 1, 2
            glm::vec3(19.5f, 0.0f, -7.0f),   // 1, 1, 3
        },
        {
            glm::vec3(-20.0f, 0.5f, 7.5f),   // 0, 2, 0
            glm::vec3(-7.0f, 0.5f, 7.5f),    // 0, 2, 1
            glm::vec3(6.0f, 0.5f, 7.5f),     // 0, 2, 2
            glm::vec3(19.0f, 0.5f, 7.5f),    // 0, 2, 3
        },
        {
            glm::vec3(-20.0f, 0.5f, 20.0f),  // 0, 3, 0
            glm::vec3(-7.0f, 0.5f, 20.0f),   // 0, 3, 1
            glm::vec3(6.0f, 0.5f, 20.0f),    // 0, 3, 2
            glm::vec3(19.0f, 0.5f, 20.0f),   // 0, 3, 3
        }
    },{     // THIRD GRID
        {
            glm::vec3(-19.5f, 0.0f, -20.0f), // 1, 0, 0
            glm::vec3(-6.5f, 0.0f, -20.0f),  // 1, 0, 1
            glm::vec3(6.5f, 0.0f, -20.0f),   // 1, 0, 2
            glm::vec3(19.5f, 0.0f, -20.0f)   // 1, 0, 3
        },
        {
            glm::vec3(-19.5f, 0.0f, -7.0f),  // 1, 1, 0
            glm::vec3(-6.5f, 0.0f, -7.0f),   // 1, 1, 1
            glm::vec3(6.5f,  0.0f, -7.0f),   // 1, 1, 2
            glm::vec3(19.5f, 0.0f, -7.0f),   // 1, 1, 3
        },
        {
            glm::vec3(-20.0f, 0.5f, 7.5f),   // 0, 2, 0
            glm::vec3(-7.0f, 0.5f, 7.5f),    // 0, 2, 1
            glm::vec3(6.0f, 0.5f, 7.5f),     // 0, 2, 2
            glm::vec3(19.0f, 0.5f, 7.5f),    // 0, 2, 3
        },
        {
            glm::vec3(-20.0f, 0.5f, 20.0f),  // 0, 3, 0
            glm::vec3(-7.0f, 0.5f, 20.0f),   // 0, 3, 1
            glm::vec3(6.0f, 0.5f, 20.0f),    // 0, 3, 2
            glm::vec3(19.0f, 0.5f, 20.0f),   // 0, 3, 3
        }
    },{     // FOURTH GRID
        {
            glm::vec3(-19.5f, 0.0f, -20.0f), // 1, 0, 0
            glm::vec3(-6.5f, 0.0f, -20.0f),  // 1, 0, 1
            glm::vec3(6.5f, 0.0f, -20.0f),   // 1, 0, 2
            glm::vec3(19.5f, 0.0f, -20.0f)   // 1, 0, 3
        },
        {
            glm::vec3(-19.5f, 0.0f, -7.0f),  // 1, 1, 0
            glm::vec3(-6.5f, 0.0f, -7.0f),   // 1, 1, 1
            glm::vec3(6.5f,  0.0f, -7.0f),   // 1, 1, 2
            glm::vec3(19.5f, 0.0f, -7.0f),   // 1, 1, 3
        },
        {
            glm::vec3(-20.0f, 0.5f, 7.5f),   // 0, 2, 0
            glm::vec3(-7.0f, 0.5f, 7.5f),    // 0, 2, 1
            glm::vec3(6.0f, 0.5f, 7.5f),     // 0, 2, 2
            glm::vec3(19.0f, 0.5f, 7.5f),    // 0, 2, 3
        },
        {
            glm::vec3(-20.0f, 0.5f, 20.0f),  // 0, 3, 0
            glm::vec3(-7.0f, 0.5f, 20.0f),   // 0, 3, 1
            glm::vec3(6.0f, 0.5f, 20.0f),    // 0, 3, 2
            glm::vec3(19.0f, 0.5f, 20.0f),   // 0, 3, 3
        }
    }
};

class Object{
public:
    void doLightingStuff(Shader shader){
        GLint lightDirLoc = glGetUniformLocation( shader.Program, "light.direction" );
        GLint viewPosLoc  = glGetUniformLocation( shader.Program, "viewPos" );
        
        glUniform3f( lightDirLoc, -0.2f, -1.0f, -0.3f );
        glUniform3f( viewPosLoc,  camera.GetPosition( ).x, camera.GetPosition( ).y, camera.GetPosition( ).z );
        
        // LIGHT PROPERTIES
        glUniform3f( glGetUniformLocation( shader.Program, "light.ambient" ),  0.5f, 0.5f, 0.5f );
        glUniform3f( glGetUniformLocation( shader.Program, "light.diffuse" ),  0.5f, 0.5f, 0.5f );
        glUniform3f( glGetUniformLocation( shader.Program, "light.specular" ), 1.0f, 1.0f, 1.0f );
        
        // MATERIAL PROPERTIES
        glUniform1f( glGetUniformLocation( shader.Program, "material.shininess" ), 32.0f );
    }
};

#endif /* Object_h */
