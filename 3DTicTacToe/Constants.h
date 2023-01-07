//
//  Constants.h
//  3DTicTacToe
//
//  Created by Hamza Gabajiwala on 24/12/2022.
//

#ifndef Constants_h
#define Constants_h

// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera Properties
int projectionMode = 0; // Default = Orthographic
int cameraMode = 0; // Default = Disabled

// Camera Movement
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

glm::vec3 lightPos( 0.0f, 0.0f, -8.5f );
glm::mat4 model = glm::mat4(1.0);

float flux_alpha = 1.0f;

GLfloat degreeRotation = 50.5f;
GLfloat scaling = 0.015f;

GLfloat grid_translate[] = {0.0f, 0.05f, 0.1f, 0.15f};

#endif /* Constants_h */
