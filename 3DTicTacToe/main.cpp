// Std. Includes
#define GL_SILENCE_DEPRECATION
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

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"
#include <vector>

// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void createObject(const std::string& objectName, Shader *shader, Model *draw, glm::mat4 projection, glm::mat4 view, glm::mat4 model, glm::vec4 extra_color = glm::vec4(1.0f));
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode );
void MouseCallback( GLFWwindow *window, double xPos, double yPos );
void DoMovement( );

int projectionMode = 0; // Default = Orthographic
int cameraMode = 0; // Default = Disabled

Camera camera( glm::vec3( 0.2f, 0.0f, 0.0f ) );

bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

bool firstTac = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

glm::vec3 lightPos( 0.0f, 0.0f, -8.5f );

glm::mat4 model = glm::mat4(1.0);

std::vector< glm::vec3 > store_pos;
std::vector< bool > store_tac;

glm::vec3 first_grid[] = {
    glm::vec3(-19.5f, 0.0f, -20.0f), // 0, 0, 0
    glm::vec3(-6.5f, 0.0f, -20.0f),  // 0, 0, 1
    glm::vec3(6.5f, 0.0f, -20.0f),   // 0, 0, 2
    glm::vec3(19.5f, 0.0f, -20.0f),  // 0, 0, 3

    glm::vec3(-19.5f, 0.0f, -7.0f),  // 0, 1, 0
    glm::vec3(-6.5f, 0.0f, -7.0f),   // 0, 1, 1
    glm::vec3(6.5f,  0.0f, -7.0f),   // 0, 1, 2
    glm::vec3(19.5f, 0.0f, -7.0f),   // 0, 1, 3

    glm::vec3(-20.0f, 0.5f, 7.5f),   // 0, 2, 0
    glm::vec3(-7.0f, 0.5f, 7.5f),    // 0, 2, 1
    glm::vec3(6.0f, 0.5f, 7.5f),     // 0, 2, 2
    glm::vec3(19.0f, 0.5f, 7.5f),    // 0, 2, 3

    glm::vec3(-20.0f, 0.5f, 20.0f),  // 0, 3, 0
    glm::vec3(-7.0f, 0.5f, 20.0f),   // 0, 3, 1
    glm::vec3(6.0f, 0.5f, 20.0f),    // 0, 3, 2
    glm::vec3(19.0f, 0.5f, 20.0f),   // 0, 3, 3
};

glm::vec3 second_grid[] = {
    glm::vec3(-15.5f, -0.5f, -20.0f),// 1, 0, 0
    glm::vec3(-2.5f, -0.5f, -20.0f), // 1, 0, 1
    glm::vec3(10.5f, -0.5f, -20.0f), // 1, 0, 2
    glm::vec3(23.5f, -0.5f, -20.0f), // 1, 0, 3
    
    glm::vec3(-15.5f, -0.5f, -7.5f), // 1, 1, 0
    glm::vec3(-2.5f, -0.5f, -7.5f),  // 1, 1, 1
    glm::vec3(10.5f, -0.5f, -7.5f),  // 1, 1, 2
    glm::vec3(23.5f, -0.5f, -7.5f),  // 1, 1, 3
    
    glm::vec3(-16.0f, 0.5f, 7.5f),   // 1, 2, 0
    glm::vec3(-3.0f, 0.5f, 7.5f),    // 1, 2, 1
    glm::vec3(10.0f, 0.5f, 7.5f),    // 1, 2, 2
    glm::vec3(23.0f, 0.5f, 7.5f),    // 1, 2, 3

    glm::vec3(-16.0f, 0.5f, 20.0f),  // 1, 3, 0
    glm::vec3(-3.0f, 0.5f, 20.0f),   // 1, 3, 1
    glm::vec3(10.0f, 0.5f, 20.0f),   // 1, 3, 2
    glm::vec3(23.0f, 0.5f, 20.0f),   // 1, 3, 3
};

glm::vec3 third_grid[] = {
    glm::vec3(-12.0f, -0.5f, -20.0f),// 2, 0, 0
    glm::vec3(1.0f, -0.5f, -20.0f),  // 2, 0, 1
    glm::vec3(14.0f, -0.5f, -20.0f), // 2, 0, 2
    glm::vec3(27.0f, -0.5f, -20.0f), // 2, 0, 3
    
    glm::vec3(-12.0f, -0.5f, -7.5f), // 2, 1, 0
    glm::vec3(1.0f, -0.5f, -7.5f),   // 2, 1, 1
    glm::vec3(14.0f, -0.5f, -7.5f),  // 2, 1, 2
    glm::vec3(27.0f, -0.5f, -7.5f),  // 2, 1, 3

    glm::vec3(-12.5f, 0.5f, 7.5f),   // 2, 2, 0
    glm::vec3(0.5f, 0.5f, 7.5f),     // 2, 2, 1
    glm::vec3(13.5f, 0.5f, 7.5f),    // 2, 2, 2
    glm::vec3(26.5f, 0.5f, 7.5f),    // 2, 2, 3
    
    glm::vec3(-12.5f, 0.5f, 20.0f),  // 2, 3, 0
    glm::vec3(0.5f, 0.5f, 20.0f),    // 2, 3, 1
    glm::vec3(13.5f, 0.5f, 20.0f),   // 2, 3, 2
    glm::vec3(26.5f, 0.5f, 20.0f),   // 2, 3, 3
};

glm::vec3 fourth_grid[] = {
    glm::vec3(-8.5f, -0.5f, -20.0f), // 3, 0, 0
    glm::vec3(4.5f, -0.5f, -20.0f),  // 3, 0, 1
    glm::vec3(17.5f, -0.5f, -20.0f), // 3, 0, 2
    glm::vec3(30.5f, -0.5f, -20.0f), // 3, 0, 3

    glm::vec3(-8.5f, -0.5f, -7.5f),  // 3, 1, 0
    glm::vec3(4.5f, -0.5f, -7.5f),   // 3, 1, 1
    glm::vec3(17.5f, -0.5f, -7.5f),  // 3, 1, 2
    glm::vec3(30.5f, -0.5f, -7.5f),  // 3, 1, 3

    glm::vec3(-9.0f, 0.5f, 7.5f),    // 3, 2, 0
    glm::vec3(4.0f, 0.5f, 7.5f),     // 3, 2, 1
    glm::vec3(17.0f, 0.5f, 7.5f),    // 3, 2, 2
    glm::vec3(30.0f, 0.5f, 7.5f),    // 3, 2, 3

    glm::vec3(-9.0f, 0.5f, 20.0f),   // 3, 3, 0
    glm::vec3(4.0f, 0.5f, 20.0f),    // 3, 3, 1
    glm::vec3(17.0f, 0.5f, 20.0f),   // 3, 3, 2
    glm::vec3(30.0f, 0.5f, 20.0f),   // 3, 3, 3
};

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
};

void createObject(const std::string& objectName, Shader *shader, Model *draw, glm::mat4 projection, glm::mat4 view, glm::mat4 model, glm::vec4 extra_color){
    glUniformMatrix4fv( glGetUniformLocation( shader->Program, "projection" ), 1, GL_FALSE, glm::value_ptr( projection ) );
    glUniformMatrix4fv( glGetUniformLocation( shader->Program, "view" ), 1, GL_FALSE, glm::value_ptr( view ) );
    glUniformMatrix4fv( glGetUniformLocation( shader->Program, "model" ), 1, GL_FALSE, glm::value_ptr( model ) );
    glUniform4f( glGetUniformLocation( shader->Program, "extra_color" ), extra_color.r, extra_color.g, extra_color.b, extra_color.a);
    
    draw->Draw(*shader);
}

float flux_alpha = 1.0f;
GLint posLoc = 0;
bool confirm_pos = false;

int main( )
{
    glfwInit( );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );

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
        
        doLightingStuff(shader);
        
        GLfloat degreeRotation = 50.5f;
        GLfloat scaling = 0.015f;
        
        glm::mat4 view = camera.GetViewMatrix( );
        
        model = glm::mat4(1.0f);
        model = glm::translate( model, glm::vec3( 0.0f, 0.0f, -2.5f ) );
        model = glm::rotate(model, glm::radians(degreeRotation), glm::vec3(1.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(-18.5f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(scaling));
        model = glm::translate( model, glm::vec3( 0.0f, 60.0f, 0.0f ) );
        //parentGrid1 = glm::rotate(parentGrid1, (GLfloat)glfwGetTime()*10.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        createObject("Top", &shader, &modelGrid, projection, view, model);
        

        model = glm::mat4(1.0f);
        model = glm::translate( model, glm::vec3(0.05f, 0.0f, -2.5f ) );
        model = glm::rotate(model, glm::radians(degreeRotation), glm::vec3(1.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(-18.5f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(scaling));
        model = glm::translate( model, glm::vec3( 0.0f, 20.0f, 0.0f ) );
        createObject("Top - 2", &shader, &modelGrid, projection, view, model);

        model = glm::mat4(1.0f);
        model = glm::translate( model, glm::vec3( 0.1f, 0.0f, -2.5f ) );
        model = glm::rotate(model, glm::radians(degreeRotation), glm::vec3(1.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(-18.5f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(scaling));
        model = glm::translate( model, glm::vec3( 0.0f, -20.0f, 0.0f ) );
        createObject("Top - 3", &shader, &modelGrid, projection, view, model);

        model = glm::mat4(1.0f);
        model = glm::translate( model, glm::vec3( 0.15f, 0.0f, -2.5f ) );
        model = glm::rotate(model, glm::radians(degreeRotation), glm::vec3(1.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(-18.5f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(scaling));
        model = glm::translate( model, glm::vec3( 0.0f, -60.0f, 0.0f ) );
        createObject("Top- 4 ", &shader, &modelGrid, projection, view, model);
        
        flux_alpha += 1.0f * deltaTime;
        
        glm::mat4 planeVector = glm::mat4(1.0f);
        planeVector = glm::translate( planeVector, glm::vec3( 0.0f, 0.0f, -2.5f ));
        planeVector = glm::rotate(planeVector, glm::radians(degreeRotation), glm::vec3(1.0f, 1.0f, 0.0f));
        planeVector = glm::rotate(planeVector, glm::radians(-18.5f), glm::vec3(0.0f, 1.0f, 0.0f));
        planeVector = glm::scale(planeVector, glm::vec3(scaling));
        planeVector = glm::translate( planeVector, glm::vec3( 0.0f, 60.0f, 0.0f )  );
        planeVector = glm::translate( planeVector, first_grid[posLoc]);
        createObject("|", &shader, &planeMesh, projection, view, planeVector, glm::vec4(0.5f,0.5f,0.5f,abs(sinf(flux_alpha))));

        if (store_pos.size() > 0){
            for (GLint i = 0; i < store_pos.size(); i++){
                glm::mat4 tacs = glm::mat4(1.0f);
                tacs = glm::translate( tacs, glm::vec3( 0.0f, 0.0f, -2.5f ));
                tacs = glm::rotate(tacs, glm::radians(degreeRotation), glm::vec3(1.0f, 1.0f, 0.0f));
                tacs = glm::rotate(tacs, glm::radians(-18.5f), glm::vec3(0.0f, 1.0f, 0.0f));
                tacs = glm::scale(tacs, glm::vec3(scaling));
                tacs = glm::translate( tacs, glm::vec3( 0.0f, 60.0f, 0.0f )  );
                tacs = glm::translate( tacs, store_pos[i]);
                if (store_tac[i]){
                    createObject("|", &shader, &modelX, projection, view, tacs);
                }else{
                    createObject("|", &shader, &modelO, projection, view, tacs);
                }
            }
        }
        
        // BUFFER SWAP
        glfwSwapBuffers( window );
    }
    
    glfwTerminate( );
    return 0;
}

void DoMovement( )
{
    // CAMERA CONTROLS
    if ( keys[GLFW_KEY_W] || keys[GLFW_KEY_UP] )
    {
        camera.ProcessKeyboard( FORWARD, deltaTime );
    }
    
    if ( keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN] )
    {
        camera.ProcessKeyboard( BACKWARD, deltaTime );
    }
    
//    if ( keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT] )
//    {
//        camera.ProcessKeyboard( LEFT, deltaTime );
//    }
//
//    if ( keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT] )
//    {
//        camera.ProcessKeyboard( RIGHT, deltaTime );
//    }
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
        if ( keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT] )
        {
            posLoc += 1;
        }
        
        if(key == GLFW_KEY_P) {
            projectionMode = !projectionMode;
        }
        
        if(key == GLFW_KEY_C) {
            cameraMode = !cameraMode;
            camera = Camera( glm::vec3( 0.2f, 0.0f, 0.0f ) );
        }
        
        if(key == GLFW_KEY_SPACE) {
            store_pos.push_back(first_grid[posLoc]);
            store_tac.push_back(firstTac);
            posLoc += 1;
            firstTac = !firstTac;
        }
        
        if(key == GLFW_KEY_R) {
            store_pos.clear();
            store_tac.clear();
            posLoc = 0;
            firstTac = true;
        }
    }
    
    if (key == GLFW_KEY_T)
    {
        model = glm::translate(model, glm::vec3(1.0f, 1.0f, 1.0f));
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
