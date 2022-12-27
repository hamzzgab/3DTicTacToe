#include "Imports.h"

bool hierarchical = false;
glm::mat4 grid_models[4];

// Function Prototypes
glm::mat4 model_create(GLfloat grid_translate=0.0f);
void createObject(const std::string& objectName, Shader *shader, Model *draw, glm::mat4 projection, glm::mat4 view, glm::mat4 model, glm::vec4 extra_color = glm::vec4(1.0f));
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode );
void MouseCallback( GLFWwindow *window, double xPos, double yPos );
void DoMovement( );

glm::mat4 model_create(GLfloat grid_translate){
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate( model, glm::vec3( grid_translate, 0.0f, -2.5f ));
    model = glm::rotate(model, glm::radians(degreeRotation), glm::vec3(1.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(-18.5f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(scaling));
    return model;
}

void createObject(const std::string& objectName, Shader *shader, Model *draw, glm::mat4 projection, glm::mat4 view, glm::mat4 model, glm::vec4 extra_color){
    glUniformMatrix4fv( glGetUniformLocation( shader->Program, "projection" ), 1, GL_FALSE, glm::value_ptr( projection ) );
    glUniformMatrix4fv( glGetUniformLocation( shader->Program, "view" ), 1, GL_FALSE, glm::value_ptr( view ) );
    glUniformMatrix4fv( glGetUniformLocation( shader->Program, "model" ), 1, GL_FALSE, glm::value_ptr( model ) );
    glUniform4f( glGetUniformLocation( shader->Program, "extra_color" ), extra_color.r, extra_color.g, extra_color.b, extra_color.a);
    draw->Draw(*shader);
}

GameLogic game;

float grid_rot = 0.0f;

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
        
        glm::mat4 view = camera.GetViewMatrix( );
        
        grid_rot += 50.0f * deltaTime;
        for (int a=0; a<4; a++){
            grid_models[a] = model_create(grid_translate[a]);
            grid_models[a] = glm::translate( grid_models[a], glm::vec3( curr_grid[a] ) );
            if (hierarchical){
                grid_models[a] = glm::rotate(grid_models[a], glm::radians(grid_rot), glm::vec3(0.0f, 1.0f, 0.0f));
            }
            createObject("Grid", &shader, &modelGrid, projection, view, grid_models[a]);
        }

        flux_alpha += 1.0f * deltaTime;
        glm::mat4 planeVector = glm::mat4(1.0f);
        planeVector = glm::translate( planeVector, grid_pos[i][j][k]);
        planeVector = grid_models[i] * planeVector;
        createObject("Plane", &shader, &planeMesh, projection, view, planeVector, glm::vec4(0.5f,0.5f,0.5f,abs(sinf(flux_alpha))));


        if (store_i.size() > 0){
            for (GLint a = 0; a < store_i.size(); a++){
                glm::mat4 tacs = glm::mat4(1.0f);
                tacs = glm::translate( tacs, grid_pos[store_i[a]][store_j[a]][store_k[a]]);
                tacs = grid_models[store_i[a]]  * tacs;
                
                if (store_tac[a]){
                    createObject("X", &shader, &modelX, projection, view, tacs);
                }else{
                    createObject("O", &shader, &modelO, projection, view, tacs);
                }
            }
        }
        
        
        text.RenderText(textShader, "3D Tic Tac Toe", 10.0f, 560.0f, 0.65f, glm::vec3(0.0f, 0.0f, 0.0f));
        
        if (gameControls){
            text.RenderText(textShader, "W: Up", 10.0f, 205.0f, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));
            text.RenderText(textShader, "A: Left", 10.0f, 180.0f, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));
            text.RenderText(textShader, "S: Down", 10.0f, 155.0f, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));
            text.RenderText(textShader, "D: Right", 10.0f, 130.0f, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));
            text.RenderText(textShader, "E: Grid Up", 10.0f, 105.0f, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));
            text.RenderText(textShader, "Q: Grid Down", 10.0f, 80.0f, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));
            text.RenderText(textShader, "SPACE: Confirm", 10.0f, 55.0f, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));
            
            text.RenderText(textShader, "C: Camera", 10.0f, 25.0f, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));
            text.RenderText(textShader, "P: Perspective", 10.0f, 0.0f, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));
            
            text.RenderText(textShader, "R: Game Reset", 640.0f, 0.0f, 0.5f, glm::vec3(1.0f, 0.0f, 0.0f));
        }else{
            text.RenderText(textShader, "Y: Controls", 10.0f, 0.0f, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));
        }

        
        if (gameWin){
            if (firstTac){
                text.RenderText(textShader, "Player O Won!", 10.0f, 530.0f, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));
            }else{
                text.RenderText(textShader, "Player X Won!", 10.0f, 530.0f, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));
            }
        }else{
            if (firstTac){
                text.RenderText(textShader, "Current Player X", 10.0f, 530.0f, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));
            }else{
                text.RenderText(textShader, "Current Player O", 10.0f, 530.0f, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));
            }
            
            if (!moveAval){
                text.RenderText(textShader, "Move Not Available!", 10.0f, 500.0f, 0.5f, glm::vec3(1.0f, 0.0f, 0.0f));
            }
            
            game.check_win_status();
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
        
        if( keys[GLFW_KEY_H] ) {
            hierarchical = !hierarchical;
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
