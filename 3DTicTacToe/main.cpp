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
#include "Model.h"
#include "Text.h"
#include "Object.h"
#include "GameLogic.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"
#include <vector>

#include <ft2build.h>
#include FT_FREETYPE_H

// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera Properties
int projectionMode = 0; // Default = Orthographic
int cameraMode = 0; // Default = Disabled

// Function Prototypes
void createObject(const std::string& objectName, Shader *shader, Model *draw, glm::mat4 projection, glm::mat4 view, glm::mat4 model, glm::vec4 extra_color = glm::vec4(1.0f));
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode );
void MouseCallback( GLFWwindow *window, double xPos, double yPos );
void DoMovement( );

// Camera Movement
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

glm::vec3 lightPos( 0.0f, 0.0f, -8.5f );
glm::mat4 model = glm::mat4(1.0);

void createObject(const std::string& objectName, Shader *shader, Model *draw, glm::mat4 projection, glm::mat4 view, glm::mat4 model, glm::vec4 extra_color){
    glUniformMatrix4fv( glGetUniformLocation( shader->Program, "projection" ), 1, GL_FALSE, glm::value_ptr( projection ) );
    glUniformMatrix4fv( glGetUniformLocation( shader->Program, "view" ), 1, GL_FALSE, glm::value_ptr( view ) );
    glUniformMatrix4fv( glGetUniformLocation( shader->Program, "model" ), 1, GL_FALSE, glm::value_ptr( model ) );
    glUniform4f( glGetUniformLocation( shader->Program, "extra_color" ), extra_color.r, extra_color.g, extra_color.b, extra_color.a);
    
    draw->Draw(*shader);
}

float flux_alpha = 1.0f;

GameLogic game;

int main( )
{
    store_tics = game.initialize_tics(store_tics);
    
    glfwInit( );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    //glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );

    GLFWwindow *window = glfwCreateWindow( WIDTH, HEIGHT, "Game", nullptr, nullptr );
    glfwSetWindowPos(window, 0, 0);
    
    if ( nullptr == window )
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate( );
        
        return EXIT_FAILURE;
    }
    
    glfwMakeContextCurrent( window );
    glfwGetFramebufferSize( window, &SCREEN_WIDTH, &SCREEN_HEIGHT );
    glfwSetKeyCallback( window, KeyCallback );
    glfwSetCursorPosCallback( window, MouseCallback );
    
    glewExperimental = GL_TRUE;
    if ( GLEW_OK != glewInit( ) )
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }
    
    glViewport( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
    glEnable( GL_DEPTH_TEST );
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    Shader shader( "res/shaders/modelLoading.vs", "res/shaders/modelLoading.frag" );
    
    shader.Use( );
    glUniform1i( glGetUniformLocation(shader.Program, "material.diffuse" ),  0 );
    glUniform1i( glGetUniformLocation(shader.Program, "material.specular" ), 1 );

    Model modelX( "res/models/MarkerX.obj" );
    Model modelO( "res/models/MarkerO.obj" );
    Model modelGrid( "res/models/grid.obj" );
    Model planeMesh( "res/models/Plane.obj" );

    // glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    
    float aspect = (float) SCREEN_WIDTH / SCREEN_HEIGHT;
    glm::mat4 projection = glm::ortho(-aspect, aspect, -1.0f, 1.0f, 0.1f, 50.0f);
    
    // OpenGL state
    // ------------
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // compile and setup the shader
    // ----------------------------
    Shader textShader("res/shaders/text.vs", "res/shaders/text.frag");
    textShader.Use();
    glm::mat4 projection_text = glm::ortho(0.0f, (float)800.0f, 0.0f, (float)600.0f);
    glUniformMatrix4fv(glGetUniformLocation(textShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection_text));
    
    Text text;
    Object object;
    
    while( !glfwWindowShouldClose( window ) )
    {
        if(projectionMode == 0) {
            projection = glm::ortho(-aspect, aspect, -1.0f, 1.0f, 0.001f, 1000.0f);
        }
        else {
            projection = glm::perspective( camera.GetZoom( ), ( float )SCREEN_WIDTH/( float )SCREEN_HEIGHT, 0.1f, 100.0f );
        }
        
        if(cameraMode != 0) {
            glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
        }else{
            glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_NORMAL );
        }
        
        GLfloat currentFrame = glfwGetTime( );
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        glfwPollEvents( );
        DoMovement( );
        
        glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        
        
        shader.Use( );
        
        object.doLightingStuff(shader);
        
        GLfloat degreeRotation = 50.5f;
        GLfloat scaling = 0.015f;
        
        glm::mat4 view = camera.GetViewMatrix( );
        
        GLfloat grid_translate[] = {0.0f, 0.05f, 0.1f, 0.15f};
        for (int i=0; i<4; i++){
            model = glm::mat4(1.0f);
            model = glm::translate( model, glm::vec3( grid_translate[i], 0.0f, -2.5f ) );
            model = glm::rotate(model, glm::radians(degreeRotation), glm::vec3(1.0f, 1.0f, 0.0f));
            model = glm::rotate(model, glm::radians(-18.5f), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::scale(model, glm::vec3(scaling));
            model = glm::translate( model, glm::vec3( curr_grid[i] ) );
            createObject("Top", &shader, &modelGrid, projection, view, model);
        }
        
        flux_alpha += 1.0f * deltaTime;
        
        glm::mat4 planeVector = glm::mat4(1.0f);
        planeVector = glm::translate( planeVector, glm::vec3( 0.0f, 0.0f, -2.5f ));
        planeVector = glm::rotate(planeVector, glm::radians(degreeRotation), glm::vec3(1.0f, 1.0f, 0.0f));
        planeVector = glm::rotate(planeVector, glm::radians(-18.5f), glm::vec3(0.0f, 1.0f, 0.0f));
        planeVector = glm::scale(planeVector, glm::vec3(scaling));
        planeVector = glm::translate( planeVector, curr_grid[i] );
        planeVector = glm::translate( planeVector, grid_pos[i][j][k]);
        createObject("|", &shader, &planeMesh, projection, view, planeVector, glm::vec4(0.5f,0.5f,0.5f,abs(sinf(flux_alpha))));
        
        if (store_i.size() > 0){
            for (GLint i = 0; i < store_i.size(); i++){
                glm::mat4 tacs = glm::mat4(1.0f);
                tacs = glm::translate( tacs, glm::vec3( 0.0f, 0.0f, -2.5f ));
                tacs = glm::rotate(tacs, glm::radians(degreeRotation), glm::vec3(1.0f, 1.0f, 0.0f));
                tacs = glm::rotate(tacs, glm::radians(-18.5f), glm::vec3(0.0f, 1.0f, 0.0f));
                tacs = glm::scale(tacs, glm::vec3(scaling));
                tacs = glm::translate( tacs, curr_grid[store_i[i]] );
                tacs = glm::translate( tacs, grid_pos[store_i[i]][store_j[i]][store_k[i]]);
                if (store_tac[i]){
                    createObject("|", &shader, &modelX, projection, view, tacs);
                }else{
                    createObject("|", &shader, &modelO, projection, view, tacs);
                }
            }
        }
        
        text.RenderText(textShader, "3D Tic Tac Toe", 10.0f, 560.0f, 0.65f, glm::vec3(0.0f, 0.0f, 0.0f));
        if (firstTac){
            text.RenderText(textShader, "Current Player X", 10.0f, 530.0f, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));
        }else{
            text.RenderText(textShader, "Current Player O", 10.0f, 530.0f, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));
        }
        
        if (!moveAval){
            text.RenderText(textShader, "Move Not Available!", 10.0f, 500.0f, 0.5f, glm::vec3(1.0f, 0.0f, 0.0f));
        }

        // BUFFER SWAP
        glfwSwapBuffers( window );
    }
    
    glfwTerminate( );
    return 0;
}

void DoMovement( )
{
    if (projectionMode == 1){
        // CAMERA CONTROLS
        if ( keys[GLFW_KEY_W] || keys[GLFW_KEY_UP] )
        {
            if ( cameraMode ) camera.ProcessKeyboard( FORWARD, deltaTime );
        }

        if ( keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN] )
        {
            if ( cameraMode ) camera.ProcessKeyboard( BACKWARD, deltaTime );
        }
        
        if ( keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT] )
        {
            if ( cameraMode ) camera.ProcessKeyboard( LEFT, deltaTime );
        }

        if ( keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT] )
        {
            if ( cameraMode ) camera.ProcessKeyboard( RIGHT, deltaTime );
        }
    }
}

// CALLED WHENEVER A KEY IS PRESSED/RELEASED VIA GLFW
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode )
{
    if ( GLFW_KEY_ESCAPE == key && GLFW_PRESS == action )
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if ( key >= 0 && key < 1024 )
    {
        if ( action == GLFW_PRESS )
        {
            keys[key] = true;
        }
        else if ( action == GLFW_RELEASE )
        {
            keys[key] = false;
        }
    }
    
    if(action == GLFW_PRESS)
    {
        game.movement(keys);
        
        // CAMERA AND PROJECTION
        if( keys[GLFW_KEY_P] ) {
            projectionMode = !projectionMode;
        }
        
        if(keys[GLFW_KEY_C] ) {
            cameraMode = !cameraMode;
            camera = Camera( glm::vec3( 0.2f, 0.0f, 0.0f ) );
        }
    }
}

void MouseCallback( GLFWwindow *window, double xPos, double yPos )
{
    if(cameraMode != 0) {
        
        if ( firstMouse )
        {
            lastX = xPos;
            lastY = yPos;
            firstMouse = false;
        }
        
        GLfloat xOffset = xPos - lastX;
        GLfloat yOffset = lastY - yPos;
        
        lastX = xPos;
        lastY = yPos;
        
        camera.ProcessMouseMovement( xOffset, yOffset );
    }
}
