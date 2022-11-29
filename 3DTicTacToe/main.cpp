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

// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode );
void MouseCallback( GLFWwindow *window, double xPos, double yPos );
void DoMovement( );

int projectionMode = 0; // Default = Orthographic
int cameraMode = 0; // Default = Disabled


glm::mat4 parentGrid1 = glm::mat4(1.0f);


Camera camera( glm::vec3( 0.2f, 0.0f, 0.0f ) );

bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

glm::vec3 lightPos( 0.0f, 0.0f, -8.5f );

glm::mat4 model = glm::mat4(1.0);


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

void createObject(const std::string& objectName, Shader *shader, Model *draw, glm::mat4 projection, glm::mat4 view, glm::mat4 model){
    glUniformMatrix4fv( glGetUniformLocation( shader->Program, "projection" ), 1, GL_FALSE, glm::value_ptr( projection ) );
    glUniformMatrix4fv( glGetUniformLocation( shader->Program, "view" ), 1, GL_FALSE, glm::value_ptr( view ) );
    glUniformMatrix4fv( glGetUniformLocation( shader->Program, "model" ), 1, GL_FALSE, glm::value_ptr( model ) );
    
    draw->Draw(*shader);
}

int main( )
{
    glfwInit( );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );

    GLFWwindow *window = glfwCreateWindow( WIDTH, HEIGHT, "Game", nullptr, nullptr );
    
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
    
    Shader shader( "res/shaders/modelLoading.vs", "res/shaders/modelLoading.frag" );
    
    shader.Use( );
    glUniform1i( glGetUniformLocation(shader.Program, "material.diffuse" ),  0 );
    glUniform1i( glGetUniformLocation(shader.Program, "material.specular" ), 1 );

    Model modelX( "res/models/MarkerX.obj" );
    Model modelO( "res/models/MarkerO.obj" );
    Model modelGrid( "res/models/grid.obj" );

    // glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    
    float aspect = (float) SCREEN_WIDTH / SCREEN_HEIGHT;
    glm::mat4 projection = glm::ortho(-aspect, aspect, -1.0f, 1.0f, 0.1f, 50.0f);


    glm::vec3 positions[] = {
        glm::vec3(-6.0f, 0.0f, -7.5f),
        glm::vec3(6.0f, 0.0f, -7.5f),
//        glm::vec3(-20.0f, 0.0f, -7.5f),
//        glm::vec3(20.0f, 0.0f, -7.5f),

//        glm::vec3(-6.0f, 0.0f, -20.0f),
//        glm::vec3(6.0f, 0.0f, -20.0f),
//        glm::vec3(-20.0f, 0.0f, -20.0f),
        glm::vec3(20.0f, 0.0f, -20.0f),

        glm::vec3(-6.0f, 0.5f, 7.5f),
//        glm::vec3(6.0f, 0.5f, 7.5f),
//        glm::vec3(-20.0f, 0.5f, 7.5f),
//        glm::vec3(20.0f, 0.5f, 7.5f),
//
//        glm::vec3(-6.0f, 0.5f, 20.0f),
        glm::vec3(6.0f, 0.5f, 20.0f),
        glm::vec3(-20.0f, 0.5f, 20.0f),
//        glm::vec3(20.0f, 0.5f, 20.0f),
//
    };
    
    glm::vec3 positions2[] = {
        glm::vec3(-3.0f, -0.5f, -7.5f),
        glm::vec3(10.0f, -0.5f, -7.5f),
        glm::vec3(-3.0f, 0.5f, 7.5f),
        glm::vec3(10.0f, 0.5f, 7.5f),
    };

    glm::vec3 positions5[] = {
        glm::vec3(0.0f, -0.5f, -7.5f),
        glm::vec3(13.0f, -0.5f, -7.5f),
        glm::vec3(0.0f, 0.5f, 7.5f),
        glm::vec3(13.0f, 0.5f, 7.5f),
        glm::vec3(13.0f, 0.5f, 20.0f),
    };
    
    glm::vec3 positions6[] = {
        glm::vec3(4.0f, -0.5f, -7.5f),
    };
    
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
        
        parentGrid1 = glm::mat4(1.0f);
        parentGrid1 = glm::translate( parentGrid1, glm::vec3( 0.0f, 0.0f, -2.5f ) );
        parentGrid1 = glm::rotate(parentGrid1, glm::radians(degreeRotation), glm::vec3(1.0f, 1.0f, 0.0f));
        parentGrid1 = glm::rotate(parentGrid1, glm::radians(-18.5f), glm::vec3(0.0f, 1.0f, 0.0f));
        parentGrid1 = glm::scale(parentGrid1, glm::vec3(scaling));
        parentGrid1 = glm::translate( parentGrid1, glm::vec3( 0.0f, 60.0f, 0.0f ) );
        //parentGrid1 = glm::rotate(parentGrid1, (GLfloat)glfwGetTime()*10.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        createObject("Top", &shader, &modelGrid, projection, view, parentGrid1);
        

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
        
        for(GLint i=0; i<5; i++){
            
            glm::mat4 childModel = glm::mat4(1.0f);
            
            childModel = glm::translate( childModel, glm::vec3( 0.0f, 0.0f, -2.5f ));
            
            childModel = glm::rotate(childModel, glm::radians(degreeRotation), glm::vec3(1.0f, 1.0f, 0.0f));
            childModel = glm::rotate(childModel, glm::radians(-18.5f), glm::vec3(0.0f, 1.0f, 0.0f));
            childModel = glm::scale(childModel, glm::vec3(scaling));
            childModel = glm::translate( childModel, glm::vec3( 0.0f, 60.0f, 0.0f )  );
            
            childModel = glm::translate( childModel, positions[i]);
            
            parentGrid1 *= childModel;
            createObject("X", &shader, &modelX, projection, view, childModel);
        }
        
        glm::mat4 oneCircle = glm::mat4(1.0f);
        oneCircle = glm::translate( oneCircle, glm::vec3( 0.0f, 0.0f, -2.5f ));
        oneCircle = glm::rotate(oneCircle, glm::radians(degreeRotation), glm::vec3(1.0f, 1.0f, 0.0f));
        oneCircle = glm::rotate(oneCircle, glm::radians(-18.5f), glm::vec3(0.0f, 1.0f, 0.0f));
        oneCircle = glm::scale(oneCircle, glm::vec3(scaling));
        oneCircle = glm::translate( oneCircle, glm::vec3( 0.0f, 60.0f, 0.0f )  );
        oneCircle = glm::translate( oneCircle, positions[5]);
        createObject("O", &shader, &modelO, projection, view, oneCircle);
        
        for(GLint i=0; i<4; i++){
            
            glm::mat4 childModel = glm::mat4(1.0f);
            
            childModel = glm::translate( childModel, glm::vec3( 0.0f, 0.0f, -2.5f ));
            
            childModel = glm::rotate(childModel, glm::radians(degreeRotation), glm::vec3(1.0f, 1.0f, 0.0f));
            childModel = glm::rotate(childModel, glm::radians(-18.5f), glm::vec3(0.0f, 1.0f, 0.0f));
            childModel = glm::scale(childModel, glm::vec3(scaling));
            childModel = glm::translate( childModel, glm::vec3( 0.0f, 20.0f, 0.0f )  );
            
            childModel = glm::translate( childModel, positions2[i]);
            
            parentGrid1 *= childModel;
            createObject("O", &shader, &modelO, projection, view, childModel);
        }
     
        for(GLint i=0; i<5; i++){
            
            glm::mat4 childModel = glm::mat4(1.0f);
            
            childModel = glm::translate( childModel, glm::vec3( 0.0f, 0.0f, -2.5f ));
            
            childModel = glm::rotate(childModel, glm::radians(degreeRotation), glm::vec3(1.0f, 1.0f, 0.0f));
            childModel = glm::rotate(childModel, glm::radians(-18.5f), glm::vec3(0.0f, 1.0f, 0.0f));
            childModel = glm::scale(childModel, glm::vec3(scaling));
            childModel = glm::translate( childModel, glm::vec3( 0.0f, -20.0f, 0.0f )  );
            
            childModel = glm::translate( childModel, positions5[i]);
            
            parentGrid1 *= childModel;
            if (i == 4){
                createObject("X", &shader, &modelX, projection, view, childModel);
            }else{
                createObject("O", &shader, &modelO, projection, view, childModel);
            }
        }
        
        oneCircle = glm::mat4(1.0f);
        oneCircle = glm::translate( oneCircle, glm::vec3( 0.0f, 0.0f, -2.5f ));
        oneCircle = glm::rotate(oneCircle, glm::radians(degreeRotation), glm::vec3(1.0f, 1.0f, 0.0f));
        oneCircle = glm::rotate(oneCircle, glm::radians(-18.5f), glm::vec3(0.0f, 1.0f, 0.0f));
        oneCircle = glm::scale(oneCircle, glm::vec3(scaling));
        oneCircle = glm::translate( oneCircle, glm::vec3( 0.0f, -60.0f, 0.0f )  );
        oneCircle = glm::translate( oneCircle, positions6[0]);
        createObject("X", &shader, &modelX, projection, view, oneCircle);
        
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
    
    if ( keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT] )
    {
        camera.ProcessKeyboard( LEFT, deltaTime );
    }
    
    if ( keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT] )
    {
        camera.ProcessKeyboard( RIGHT, deltaTime );
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
        if(key == GLFW_KEY_P) {
            projectionMode = !projectionMode;
        }
        
        if(key == GLFW_KEY_C) {
            cameraMode = !cameraMode;
            camera = Camera( glm::vec3( 0.2f, 0.0f, 0.0f ) );
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
