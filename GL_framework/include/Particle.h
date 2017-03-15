#pragma once
#include <GL\glew.h>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <cstdio>
#include <cassert>

#include "GL_framework.h"

class Particle
{
public:
	glm::vec3 orgPos;
	glm::vec3 pos;
	glm::vec3 velocity;
	glm::vec3 accel;
	glm::vec3 force;
	float mass;
	Particle(glm::vec3 = glm::vec3(0,0,0));
	~Particle();
	void StartParticle(glm::vec3);
	void UpdateParticle(float);
};	
