#pragma once

// Stupid windows DLL stuff (welcome to DLL hell)
#ifdef RT_BUILD_DLL
	#define RT_API __declspec( dllexport )
#else
	#define RT_API __declspec( dllimport )
#endif

#include <memory>
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

using std::shared_ptr;
using std::make_shared;
using std::vector;
using std::pair;
using std::make_pair;
using glm::vec3;
using glm::vec2;
using glm::mat4;

const float infinity = std::numeric_limits<float>::infinity();
const float pi = glm::pi<float>();
const float invPi = 1.0f / pi;
const float piOver2 = pi / 2.0f;
const float piOver4 = pi / 4.0f;
const float epsilon = 1e-5;