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
Camera camera( glm::vec3( 0.2f, 0.0f, 0.0f ) );

// TicTacToe Properties
bool firstTac = true;
bool moveAval = true;
std::vector< int > store_i;
std::vector< int > store_j;
std::vector< int > store_k;
std::vector<vector<vector< int >>> store_tics;
std::vector< bool > store_tac;
GLint i = 0, j = 0, k = 0;

// Text Properties
struct Character {
    unsigned int TextureID;  // ID handle of the glyph texture
    glm::ivec2   Size;       // Size of glyph
    glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
    unsigned int Advance;    // Offset to advance to next glyph
};

std::map<GLchar, Character> Characters;
unsigned int VAO, VBO;

// Function Prototypes
std::vector<vector<vector< int >>> initialize_tics(std::vector<vector<vector< int >>> store_tics);
void RenderText(Shader &shader, std::string text, float x, float y, float scale, glm::vec3 color);
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
        },
    },{     // SECOND GRID
        {
            glm::vec3(-15.5f, -0.5f, -20.0f),// 1, 0, 0
            glm::vec3(-2.5f, -0.5f, -20.0f), // 1, 0, 1
            glm::vec3(10.5f, -0.5f, -20.0f), // 1, 0, 2
            glm::vec3(23.5f, -0.5f, -20.0f), // 1, 0, 3
        },
        {
            glm::vec3(-15.5f, -0.5f, -7.5f), // 1, 1, 0
            glm::vec3(-2.5f, -0.5f, -7.5f),  // 1, 1, 1
            glm::vec3(10.5f, -0.5f, -7.5f),  // 1, 1, 2
            glm::vec3(23.5f, -0.5f, -7.5f),  // 1, 1, 3
        },
        {
            glm::vec3(-16.0f, 0.5f, 7.5f),   // 1, 2, 0
            glm::vec3(-3.0f, 0.5f, 7.5f),    // 1, 2, 1
            glm::vec3(10.0f, 0.5f, 7.5f),    // 1, 2, 2
            glm::vec3(23.0f, 0.5f, 7.5f),    // 1, 2, 3
        },
        {
            glm::vec3(-16.0f, 0.5f, 20.0f),  // 1, 3, 0
            glm::vec3(-3.0f, 0.5f, 20.0f),   // 1, 3, 1
            glm::vec3(10.0f, 0.5f, 20.0f),   // 1, 3, 2
            glm::vec3(23.0f, 0.5f, 20.0f),   // 1, 3, 3
        },
    },{     // THIRD GRID
        {
            glm::vec3(-12.0f, -0.5f, -20.0f),// 2, 0, 0
            glm::vec3(1.0f, -0.5f, -20.0f),  // 2, 0, 1
            glm::vec3(14.0f, -0.5f, -20.0f), // 2, 0, 2
            glm::vec3(27.0f, -0.5f, -20.0f), // 2, 0, 3
        },
        {
            glm::vec3(-12.0f, -0.5f, -7.5f), // 2, 1, 0
            glm::vec3(1.0f, -0.5f, -7.5f),   // 2, 1, 1
            glm::vec3(14.0f, -0.5f, -7.5f),  // 2, 1, 2
            glm::vec3(27.0f, -0.5f, -7.5f),  // 2, 1, 3
        },
        {
            glm::vec3(-12.5f, 0.5f, 7.5f),   // 2, 2, 0
            glm::vec3(0.5f, 0.5f, 7.5f),     // 2, 2, 1
            glm::vec3(13.5f, 0.5f, 7.5f),    // 2, 2, 2
            glm::vec3(26.5f, 0.5f, 7.5f),    // 2, 2, 3
        },
        {
            glm::vec3(-12.5f, 0.5f, 20.0f),  // 2, 3, 0
            glm::vec3(0.5f, 0.5f, 20.0f),    // 2, 3, 1
            glm::vec3(13.5f, 0.5f, 20.0f),   // 2, 3, 2
            glm::vec3(26.5f, 0.5f, 20.0f),   // 2, 3, 3
        },
    },{     // FOURTH GRID
        {
            glm::vec3(-8.5f, -0.5f, -20.0f), // 3, 0, 0
            glm::vec3(4.5f, -0.5f, -20.0f),  // 3, 0, 1
            glm::vec3(17.5f, -0.5f, -20.0f), // 3, 0, 2
            glm::vec3(30.5f, -0.5f, -20.0f), // 3, 0, 3
        },
        {
            glm::vec3(-8.5f, -0.5f, -7.5f),  // 3, 1, 0
            glm::vec3(4.5f, -0.5f, -7.5f),   // 3, 1, 1
            glm::vec3(17.5f, -0.5f, -7.5f),  // 3, 1, 2
            glm::vec3(30.5f, -0.5f, -7.5f),  // 3, 1, 3
        },
        {
            glm::vec3(-9.0f, 0.5f, 7.5f),    // 3, 2, 0
            glm::vec3(4.0f, 0.5f, 7.5f),     // 3, 2, 1
            glm::vec3(17.0f, 0.5f, 7.5f),    // 3, 2, 2
            glm::vec3(30.0f, 0.5f, 7.5f),    // 3, 2, 3
        },
        {
            glm::vec3(-9.0f, 0.5f, 20.0f),   // 3, 3, 0
            glm::vec3(4.0f, 0.5f, 20.0f),    // 3, 3, 1
            glm::vec3(17.0f, 0.5f, 20.0f),   // 3, 3, 2
            glm::vec3(30.0f, 0.5f, 20.0f),   // 3, 3, 3
        }
    }
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

float flux_alpha = 1.0f;

int main( )
{
    store_tics = initialize_tics(store_tics);
    
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
    
    // FreeType
    // --------
    FT_Library ft;
    // All functions return a value different than 0 whenever an error occurred
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return -1;
    }

    // find path to font
    std::string font_name = "/Users/hamzz/Development/3DTicTacToe/3DTicTacToe/res/fonts/Arcade.ttf";
    if (font_name.empty())
    {
        std::cout << "ERROR::FREETYPE: Failed to load font_name" << std::endl;
        return -1;
    }
    
    
    FT_Face face;
        if (FT_New_Face(ft, font_name.c_str(), 0, &face)) {
            std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
            return -1;
        }
        else {
            // set size to load glyphs as
            FT_Set_Pixel_Sizes(face, 0, 48);

            // disable byte-alignment restriction
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

            // load first 128 characters of ASCII set
            for (unsigned char c = 0; c < 128; c++)
            {
                // Load character glyph
                if (FT_Load_Char(face, c, FT_LOAD_RENDER))
                {
                    std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                    continue;
                }
                // generate texture
                unsigned int texture;
                glGenTextures(1, &texture);
                glBindTexture(GL_TEXTURE_2D, texture);
                glTexImage2D(
                    GL_TEXTURE_2D,
                    0,
                    GL_RED,
                    face->glyph->bitmap.width,
                    face->glyph->bitmap.rows,
                    0,
                    GL_RED,
                    GL_UNSIGNED_BYTE,
                    face->glyph->bitmap.buffer
                );
                // set texture options
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                // now store character for later use
                Character character = {
                    texture,
                    glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                    glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                    static_cast<unsigned int>(face->glyph->advance.x)
                };
                Characters.insert(std::pair<char, Character>(c, character));
            }
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        // destroy FreeType once we're finished
        FT_Done_Face(face);
        FT_Done_FreeType(ft);
    
    // configure VAO/VBO for texture quads
    // -----------------------------------
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
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
        
        RenderText(textShader, "3D Tic Tac Toe", 10.0f, 560.0f, 0.65f, glm::vec3(0.0f, 0.0f, 0.0f));
        if (firstTac){
            RenderText(textShader, "Current Player X", 10.0f, 530.0f, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));
        }else{
            RenderText(textShader, "Current Player O", 10.0f, 530.0f, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));
        }
        
        if (store_tics[i][j][k] != -1){
            moveAval = false;
        }else{
            moveAval = true;
        }
        
        if (!moveAval){
            RenderText(textShader, "Move Not Available!", 10.0f, 500.0f, 0.5f, glm::vec3(1.0f, 0.0f, 0.0f));
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
        
        // MOVING LEFT
        if ( keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT] )
        {
            if (k > 0) k--;
            
            
            if (k == 0 && j > 0){
                k = 4;
                j--;
            }
            
            if (k == 0 && j == 0 && i > 0){
                k = 3;
                j = 3;
                i--;
            }
            
            if (k == 0 && j == 0 && i == 0){
                k = 0;
                j = 0;
            }
        }
        
        if ( keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT] )
        {
            if (k < 4) ++k;
            
            if (k >= 4) {
                k = 0;
                ++j;
            }
            
            if (i == 4 && j == 3 && k == 3){
                i = 3;
                k = 3;
                j = 3;
            }
            
            if (j >= 4) {
                k = 0;
                j = 0;
                ++i;
            }
            
            if (i > 4){
                i = 0;
                k = 0;
                j = 0;
            }
        }
        
        if ( keys[GLFW_KEY_W] || keys[GLFW_KEY_UP] )
        {
            if (j >= 0){
                j--;
            }else{
                j = j;
            }
        }
        if ( keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN] )
        {
            if (j < 3){
                j++;
            }else{
                j = j;
            }
        }
        
        // MOVING UP AND DOWN THE GRID
        if ( keys[GLFW_KEY_Q] )
        {
            if ( i > 0 ){
                i--;
            }else{
                i = i;
            }
        }
        
        if ( keys[GLFW_KEY_E] )
        {
            if ( i < 3 ){
                i++;
            }else{
                i = i;
            }
        }
        if(keys[GLFW_KEY_SPACE]) {
            if (store_tics[i][j][k] == -1){
                store_i.push_back(i);
                store_j.push_back(j);
                store_k.push_back(k);
                store_tics[i][j][k] = (int)firstTac;
                store_tac.push_back(firstTac);
                firstTac = !firstTac;
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
        }
        
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

// render line of text
// -------------------
void RenderText(Shader &shader, std::string text, float x, float y, float scale, glm::vec3 color)
{
    // activate corresponding render state
    shader.Use();
    glm::mat4 projection_text = glm::ortho(0.0f, (float)800.0f, 0.0f, (float)600.0f);
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection_text));
    glUniform3f(glGetUniformLocation(shader.Program, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
    
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };

        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
