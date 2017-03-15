#include "..\include\Particle.h"



Particle::Particle(glm::vec3 posicionOriginal)
{
	pos = orgPos = posicionOriginal;
	accel = velocity = glm::vec3(0);
	mass = 1;
}

void Particle::UpdateParticle(float dt) {
	extern float gravity;

}

void Particle::StartParticle(glm::vec3 posicionOriginal) {
	
	pos = orgPos = posicionOriginal;

}


Particle::~Particle()
{

}
