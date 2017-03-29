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

glm::vec3 Particle::ApplySpring(glm::vec2 k, glm::vec3 p1, glm::vec3 p2, glm::vec3 v1, glm::vec3 v2, float distOrg) {
	return (-(k.x*(glm::length(p1-p2)-distOrg)+k.y*(v1-v2)*((p1-p2)/glm::length(p1-p2)))*(p1-p2/(glm::length(p1 - p2))));
}

glm::vec3 Particle::CalculateForce() {
	glm::vec3 strechForce = glm::vec3(0, 0, 0);
	glm::vec3 shearForce = glm::vec3(0, 0, 0);
	glm::vec3 bendForce = glm::vec3(0, 0, 0);

	extern glm::vec2 k_Strech;
	extern glm::vec2 k_Shear;
	extern glm::vec2 k_bend;

	//Valorar las formulas de los muelles

	glm::vec3 resultantForce;
	extern glm::vec3 dist;

	//PARA SABER LA I (int)index / 14
	//PARA SABER LA J index%14
	int i = (int)index/14;
	int j = index%14; 
	extern Particle* arrayParts;
	if ((i > 2 && i < ClothMesh::numRows - 2) && (j > 2&&j<ClothMesh::numCols-2)) {
		//CASO EN EL QUE SE APLICAN TODAS LAS FUERZAS
		strechForce += ApplySpring(k_Strech,pos,arrayParts[index+1].pos,velocity,arrayParts[index+1].velocity,dist.x);
		strechForce += ApplySpring(k_Strech, pos, arrayParts[index - 1].pos, velocity, arrayParts[index - 1].velocity, dist.x);
		strechForce += ApplySpring(k_Strech, pos, arrayParts[index + 14].pos, velocity, arrayParts[index + 14].velocity, dist.z);
		strechForce += ApplySpring(k_Strech, pos, arrayParts[index -14].pos, velocity, arrayParts[index -14].velocity, dist.z);
		
		shearForce += ApplySpring(k_Shear, pos, arrayParts[index + 14 + 1].pos, velocity, arrayParts[index + 14 + 1].velocity, glm::length(dist));
		shearForce += ApplySpring(k_Shear, pos, arrayParts[index - 14 + 1].pos, velocity, arrayParts[index - 14 + 1].velocity, glm::length(dist));
		shearForce += ApplySpring(k_Shear, pos, arrayParts[index + 14 - 1].pos, velocity, arrayParts[index + 14 - 1].velocity, glm::length(dist));
		shearForce += ApplySpring(k_Shear, pos, arrayParts[index - 14 - 1].pos, velocity, arrayParts[index - 14 - 1].velocity, glm::length(dist));

		bendForce += ApplySpring(k_bend, pos, arrayParts[index + 2].pos, velocity, arrayParts[index + 2].velocity, dist.x * 2);
		bendForce += ApplySpring(k_bend, pos, arrayParts[index - 2].pos, velocity, arrayParts[index - 2].velocity, dist.x * 2);
		bendForce += ApplySpring(k_bend, pos, arrayParts[index + 2*14].pos, velocity, arrayParts[index + 2*14].velocity, dist.z * 2);
		bendForce += ApplySpring(k_bend, pos, arrayParts[index - 2-14].pos, velocity, arrayParts[index - 2*14].velocity, dist.z * 2);

	}
	
	resultantForce = gravityForce * mass+bendForce+strechForce+shearForce;

	glm::vec3 acceleration = resultantForce / mass;
	return acceleration;
}

void Particle::CheckCol(float dt){
	extern glm::vec3 sphereC;
	extern float sphereR;

	if (index != 0 && index != 13) { //La primera y la ultima de la primera fila estan fijas.
		for (int i = 0; i < 6; i++) {
			if (((glm::dot(planos[i].n, prevPos)) + planos[i].d) * ((glm::dot(planos[i].n, pos)) + planos[i].d) <= 0) {
				//	std::cout << "Collided" << std::endl;
					Bounce(planos[i]);
			}
			if (glm::abs(glm::length(pos-sphereC)) < sphereR) {
				//HAY QUE CALCULAR EL PUNTO DE INTERSECCION ENTRE LA ESFERA Y LA EQ DE LA RECTA DE LA 
				//TRAYECTORIA DE LA PARTICULA
				glm::vec3 antPos = pos - velocity*dt;
				glm::vec3 vRecta = pos - antPos;

				glm::vec3 n = (pos - sphereC) / (glm::length(pos - sphereC));
				int D = -(glm::dot(n, pos));
				Bounce(Plane(n.x, n.y, n.z, D));
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
	extern float elasticCoeficientBounce;
	pos = pos - (1+ elasticCoeficientBounce) * (glm::dot(plano.n, pos) + plano.d)*plano.n;
	velocity = velocity - (1 + elasticCoeficientBounce) * (glm::dot(plano.n, velocity))*plano.n;
}

Particle::~Particle()
{

}
