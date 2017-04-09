#include "..\include\Particle.h"
#include <iostream>


Particle::Particle(glm::vec3 posicionOriginal)
{
	prevPos = pos = orgPos = posicionOriginal;
	mass = 1;
}

namespace ClothMesh {
	extern void updateClothMesh(float* array_data);
	extern const int numCols;
	extern const int numRows;
	extern const int numVerts;
}

void Particle::StartParticle(glm::vec3 posicionOriginal) {
	planos[0].SetPlaneStats(0.0f, 1.0f, 0.0f, 0.0f); // Parte abajo del cubo
	planos[1].SetPlaneStats(0.0f, 0.0f, -1.0f, -5.0f);
	planos[2].SetPlaneStats(-1.0f, 0.0f, 0.0f, 5.0f);
	planos[3].SetPlaneStats(0.0f, 0.0f, 1.0f, -5.0f);
	planos[4].SetPlaneStats(1.0f, 0.0f, 0.0f, 5.0f);
	planos[5].SetPlaneStats(0.0f, -1.0f, 0.0f, 10.0f);
	pos = orgPos = posicionOriginal;
	velocity.x = velocity.y = velocity.z = 0;
}

void Particle::Reset() {
	pos = orgPos;
	velocity = glm::vec3(0, 0, 0);
	prevPos = glm::vec3(0, 0, 0);
	prevVelocity = glm::vec3(0, 0, 0);
	_force = glm::vec3(0, 0, 0);
}

void Particle::addForce(glm::vec3 force)
{
	_force += force;
}

void Particle::UpdateParticle(float dt, glm::vec3 gravity) {
	if (index != 0 && index != 13) {
		gravityForce = gravity;
		glm::vec3 accel = _force+gravity / mass;
		prevPos = pos;
		prevVelocity = velocity;
		pos = pos + velocity*dt;
		velocity += accel*dt;
	}
	//CheckCol(dt);
}

void Particle::CheckCol(float dt){
	extern bool colisions;
	if (colisions) {
	extern glm::vec3 sphereC;
	extern float sphereR;
	extern bool sphereCol;
	if (index != 0 && index != 13) { //La primera y la ultima de la primera fila estan fijas.
		for (int i = 0; i < 6; i++) {
			if (((glm::dot(planos[i].n, prevPos)) + planos[i].d) * ((glm::dot(planos[i].n, pos)) + planos[i].d) <= 0) {

					Bounce(planos[i]);
				
			}
			if (sphereCol) {
				if (glm::length(pos - sphereC) < sphereR) {

					glm::vec3 recta = pos - prevPos;

					float a = (recta.x*recta.x) + (recta.y*recta.y) + (recta.z*recta.z);

					float b = ((2 * prevPos.x*recta.x) - (2 * recta.x*sphereC.x))
						+ ((2 * prevPos.y*recta.y) - (2 * recta.y*sphereC.y))
						+ ((2 * prevPos.z*recta.z) - (2 * recta.z*sphereC.z));

					float c = ((prevPos.x*prevPos.x) + (sphereC.x*sphereC.x) - (2 * prevPos.x*sphereC.x)
						+ (prevPos.y*prevPos.y) + (sphereC.y*sphereC.y) - (2 * prevPos.y*sphereC.y)
						+ (prevPos.z*prevPos.z) + (sphereC.z*sphereC.z) - (2 * prevPos.z*sphereC.z))
						- sphereR*sphereR;

					float alpha1 = (-b - glm::sqrt(b*b - 4 * a*c)) / 2 * a;
					float alpha2 = (-b - glm::sqrt(b*b - 4 * a*c)) / 2 * a;

					glm::vec3 colPoint, n;
					float D;
					if (alpha1 <= 1 && alpha1 >= 0) {
						colPoint = prevPos + alpha1*recta;
						n = glm::normalize(colPoint - sphereC);
						D = -(glm::dot(n, colPoint));
					}
					else if (alpha2 <= 1 && alpha2 >= 0) {
						colPoint = prevPos + alpha2*recta;
						n = glm::normalize(colPoint - sphereC);
						D = -(glm::dot(n, colPoint));
					}

					if (n != glm::vec3(0)) {
						Bounce(Plane(n.x, n.y, n.z, D));
					}

				}
			}

		}
	
	}
}
}
void Particle::Bounce(Plane plano) {
	extern float elasticCoeficientBounce;
	pos = pos - (1+ elasticCoeficientBounce) * (glm::dot(plano.n, pos) + plano.d)*plano.n;
	velocity = velocity - (1 + elasticCoeficientBounce) * (glm::dot(plano.n, velocity))*plano.n;
}

Particle::~Particle()
{

}
