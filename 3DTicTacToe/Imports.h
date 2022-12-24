#ifndef Imports_h
#define Imports_h

#define GL_SILENCE_DEPRECATION
#include <iostream>
#include <map>
#include <string>
#include <vector>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Text Rendering
#include <ft2build.h>
#include FT_FREETYPE_H

// Class includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Text.h"
#include "Object.h"
#include "GameLogic.h"
#include "Constants.h"

#endif
