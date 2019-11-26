#pragma once

// Std. Includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

// GL Includes
#include <glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/constants.hpp>
#include <gtc/type_ptr.hpp>
#include <vec3.hpp>

/* GLFW */
#include <glfw3.h>

/* SOIL loader */
#include <SOIL2.h>

#include <stb_image.h>

/* Assimp Model Loader */
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

/* AntTweakBar - ui */
#include <AntTweakBar.h>

#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

#define GLEW_STATIC
