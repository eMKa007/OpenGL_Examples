#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <random>

/* OpenGL Extension Wrangler */
#include <glew.h>

#include <glfw3.h>

/* GLM - OpenGL Mathematics */
#include <glm.hpp>
#include <vec2.hpp>
#include <vec3.hpp>
#include <vec4.hpp>
#include <mat4x4.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

/* SOIL2 - Simple OpenGL Image Library */
#include <SOIL2.h>

/* Open Asset Import Library (assimp) */
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Shader.h"
#include "Texture.h"
#include "Material.h"
#include "Vertex.h"
#include "Mesh.h"
#include "Primitives.h"
#include "Model.h"
#include "AssimpLoader.h"

#include "ShadowMapFBO.h"
