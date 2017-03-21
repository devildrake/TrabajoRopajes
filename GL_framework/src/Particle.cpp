#include "..\include\Particle.h"
#include <iostream>


Particle::Particle(glm::vec3 posicionOriginal)
{
	prevPos = pos = orgPos = posicionOriginal;
	mass = 1;
}

void Particle::StartParticle(glm::vec3 posicionOriginal) {
	planos[0].SetPlaneStats(0.0f, 1.0f, 0.0f, 0.0f); // Parte abajo del cubo
	planos[1].SetPlaneStats(0.0f, 0.0f, -1.0f, -5.0f);
	planos[2].SetPlaneStats(-1.0f, 0.0f, 0.0f, 5.0f);
	planos[3].SetPlaneStats(0.0f, 0.0f, 1.0f, -5.0f);
	planos[4].SetPlaneStats(1.0f, 0.0f, 0.0f, 5.0f);
	planos[5].SetPlaneStats(0.0f, -1.0f, 0.0f, 10.0f);
	pos = orgPos = posicionOriginal;
	//glm::vec3 accel = CalculateForce();
	velocity.x = velocity.y = velocity.z = 0;
}

void Particle::UpdateParticle(float dt, glm::vec3 gravity) {
	//glm::vec3 force = CalculateForce(accel




	if (index != 0 && index != 13) {

		gravityForce = gravity;

		glm::vec3 accel = CalculateForce();
		prevPos = pos;
		pos = pos + velocity*dt;

		velocity += accel*dt;

	}
	CheckCol();
}

glm::vec3 Particle::CalculateForce() {
	//Valorar las formulas de los muelles
	glm::vec3 resultantForce;
	
	resultantForce = gravityForce * mass;

	glm::vec3 acceleration = resultantForce / mass;
	return acceleration;
}

void Particle::CheckCol(){
	extern glm::vec3 sphereC;
	extern float sphereR;

	if (index != 0 && index != 13) {
		for (int i = 0; i < 6; i++) {
			if (((glm::dot(planos[i].n, prevPos)) + planos[i].d) * ((glm::dot(planos[i].n, pos)) + planos[i].d) <= 0) {
				//	std::cout << "Collided" << std::endl;
					Bounce(planos[i]);
			}
		}
	
	//if ((abs(sphereC.x - pos.x) < sphereR) && (abs(sphereC.y - pos.y) < sphereR) && (abs(sphereC.z - pos.z) < sphereR)) {
	//	Plane planoTan;
	//	planoTan.SetPlaneStats(pos.x-sphereC.x,pos.y- sphereC.y,pos.z- sphereC.z, -((pos.x - sphereC.x * pos.x) + (pos.y - sphereC.y * pos.y) + (pos.z - sphereC.z * pos.z)));
	//	Bounce(planoTan);
	//}
	}
}

void Particle::Bounce(Plane plano) {
	pos = pos - 2 * (glm::dot(plano.n, pos) + plano.d)*plano.n;
	velocity = velocity - 2 * (glm::dot(plano.n, velocity))*plano.n;
}

Particle::~Particle()
{

}
