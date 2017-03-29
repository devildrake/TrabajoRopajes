#pragma once
#include <GL\glew.h>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <cstdio>
#include <cassert>
#include <stdlib.h>
#include <iostream>

#include "GL_framework.h"

class Particle
{
public:
	class Plane {
	public:
		float d;
		glm::vec3 n;
		Plane() {
			d = n.x = n.y = n.z = 0;
		}
		Plane(float nx, float ny, float nz, float d) {
			this->n.x = nx;
			this->n.y = ny;
			this->n.z = nz;
			this->d = d;
		}
		void SetPlaneStats(float nx, float ny, float nz, float d) {
			n.x = nx;
			n.y = ny;
			n.z = nz;
			this->d = d;
		}
	};
	Plane planos[6];

	glm::vec3 orgPos;
	glm::vec3 pos;
	glm::vec3 prevPos;
	glm::vec3 gravityForce;
	glm::vec3 velocity;
	int index;
	float mass;
	Particle(glm::vec3 = glm::vec3(0,0,0));
	~Particle();
	void StartParticle(glm::vec3);
	void UpdateParticle(float,glm::vec3);
	void Bounce(Plane);
	void CheckCol();
	void Reset();
	glm::vec3 Particle::ApplySpring(glm::vec2,glm::vec3,glm::vec3,glm::vec3,glm::vec3,float);
	glm::vec3 CalculateForce();
};	

