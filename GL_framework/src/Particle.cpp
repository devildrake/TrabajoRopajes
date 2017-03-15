#include "..\include\Particle.h"



Particle::Particle(glm::vec3 posicionOriginal)
{
	pos = orgPos = posicionOriginal;
	accel = velocity = glm::vec3(0);
	mass = 1;
}

void Particle::StartParticle(glm::vec3 posicionOriginal) {

	pos = orgPos = posicionOriginal;

}

void Particle::UpdateParticle(float dt) {
	glm::vec3 force = CalculateForce(accel);
	pos = pos + force*dt;


}

glm::vec3 Particle::CalculateForce(glm::vec3 a) {
	glm::vec3 resultantForce;
	resultantForce = a * mass;
	return resultantForce;
}


Particle::~Particle()
{

}
