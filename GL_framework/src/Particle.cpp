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

//glm::vec3 Particle::ApplySpring(glm::vec2 k, glm::vec3 p1, glm::vec3 p2, glm::vec3 v1, glm::vec3 v2, float distOrg) {
//	glm::vec3 a = glm::normalize(p1 - p2);
//	return (-(k.x*(glm::length(p1-p2)-distOrg)+k.y*glm::dot((v1 - v2),a))*a);
//}

//glm::vec3 Particle::CalculateForce() {
//	glm::vec3 strechForce = glm::vec3(0, 0, 0);
//	glm::vec3 shearForce = glm::vec3(0, 0, 0);
//	glm::vec3 bendForce = glm::vec3(0, 0, 0);
//
//	extern glm::vec2 k_Strech;
//	extern glm::vec2 k_Shear;
//	extern glm::vec2 k_bend;
//
//	//Valorar las formulas de los muelles
//
//	glm::vec3 resultantForce;
//	extern glm::vec3 dist;
//	//PARA SABER LA I (int)index / 14
//	//PARA SABER LA J index%14
//	int i = (int)index/ClothMesh::numCols;
//	int j = index%ClothMesh::numCols; 
//	extern Particle* arrayParts;
//	//if ((i > 2 && i < ClothMesh::numRows - 2) && (j > 2 && j < ClothMesh::numCols - 2)) {
//	//	//CASO EN EL QUE SE APLICAN TODAS LAS FUERZAS
//	//	strechForce += ApplySpring(k_Strech, pos, arrayParts[index - 1].pos, velocity, arrayParts[index - 1].velocity, dist.x); //IZQUIERDA
//	//	strechForce += ApplySpring(k_Strech, pos, arrayParts[index + 14].pos, velocity, arrayParts[index + 14].velocity, dist.z); //ABAJO
//	//	strechForce += ApplySpring(k_Strech, pos, arrayParts[index + 1].pos, velocity, arrayParts[index + 1].velocity, dist.x);	//DERECHA
//	//	strechForce += ApplySpring(k_Strech, pos, arrayParts[index - 14].pos, velocity, arrayParts[index - 14].velocity, dist.z); //ARRIBA
//
//	//	shearForce += ApplySpring(k_Shear, pos, arrayParts[index + 14 - 1].pos, velocity, arrayParts[index + 14 - 1].velocity, glm::length(dist)); //ABAJO IZQUIERDA
//	//	shearForce += ApplySpring(k_Shear, pos, arrayParts[index + 14 + 1].pos, velocity, arrayParts[index + 14 + 1].velocity, glm::length(dist)); //ABAJO DERECHA
//	//	shearForce += ApplySpring(k_Shear, pos, arrayParts[index - 14 + 1].pos, velocity, arrayParts[index - 14 + 1].velocity, glm::length(dist)); //ARRIBA DERECHA
//	//	shearForce += ApplySpring(k_Shear, pos, arrayParts[index - 14 - 1].pos, velocity, arrayParts[index - 14 - 1].velocity, glm::length(dist)); //ARRIBA IZQUIERDA
//
//	//	bendForce += ApplySpring(k_bend, pos, arrayParts[index - 2].pos, velocity, arrayParts[index - 2].velocity, dist.x * 2); //IZQUIERDAIZQUIERDA
//	//	bendForce += ApplySpring(k_bend, pos, arrayParts[index + 2 * 14].pos, velocity, arrayParts[index + 2 * 14].velocity, dist.z * 2); //ABAJOABAJO
//	//	bendForce += ApplySpring(k_bend, pos, arrayParts[index + 2].pos, velocity, arrayParts[index + 2].velocity, dist.x * 2); //DERECHADERECHA
//	//	bendForce += ApplySpring(k_bend, pos, arrayParts[index - 2 - 14].pos, velocity, arrayParts[index - 2 * 14].velocity, dist.z * 2); //ARRIBAARRIBA
//	//}
//				
//	if (index > ClothMesh::numCols) {
//		strechForce += ApplySpring(k_Strech, pos, arrayParts[index - 14].prevPos, velocity, arrayParts[index - 14].prevVelocity, dist.z); //ARRIBA
//		if (index > ClothMesh::numCols * 2) {
//				bendForce += ApplySpring(k_bend, pos, arrayParts[index - 2 * 14].prevPos, velocity, arrayParts[index - 2 * 14].prevVelocity, dist.z * 2); //ARRIBAARRIBA
//		}
//	}
//
//	if (index < (ClothMesh::numVerts - ClothMesh::numCols)) {
//		strechForce += ApplySpring(k_Strech, pos, arrayParts[index + 14].prevPos, velocity, arrayParts[index + 14].prevVelocity, dist.z); //ABAJO
//		if (index < (ClothMesh::numVerts - 2 * ClothMesh::numCols)) {
//				bendForce += ApplySpring(k_bend, pos, arrayParts[index + 2 * 14].prevPos, velocity, arrayParts[index + 2 * 14].prevVelocity, dist.z * 2); //ABAJOABAJO
//		}
//	}
//
//	if (index % 14 > 0) {
//		strechForce += ApplySpring(k_Strech, pos, arrayParts[index - 1].prevPos, velocity, arrayParts[index - 1].prevVelocity, dist.x); //IZQUIERDA
//
//		if (index % 14 > 1) {
//			bendForce += ApplySpring(k_bend, pos, arrayParts[index - 2].prevPos, velocity, arrayParts[index - 2].prevVelocity, dist.x * 2); //IZQUIERDAIZQUIERDA
//		}
//		if (index > ClothMesh::numCols) {
//				shearForce += ApplySpring(k_Shear, pos, arrayParts[index - 14 - 1].prevPos, velocity, arrayParts[index - 14 - 1].prevVelocity, glm::length(dist)); //ARRIBA IZQUIERDA
//		}
//		if (index < (ClothMesh::numVerts - ClothMesh::numCols)) {
//				shearForce += ApplySpring(k_Shear, pos, arrayParts[index + 14 - 1].prevPos, velocity, arrayParts[index + 14 - 1].prevVelocity, glm::length(dist)); //ABAJO IZQUIERDA
//		}
//	}
//
//	if (index % 14 < 13) {
//		strechForce += ApplySpring(k_Strech, pos, arrayParts[index + 1].prevPos, velocity, arrayParts[index + 1].prevVelocity, dist.x);	//DERECHA
//		if (index % 14 < 12) {
//				bendForce += ApplySpring(k_bend, pos, arrayParts[index + 2].prevPos, velocity, arrayParts[index + 2].prevVelocity, dist.x * 2); //DERECHADERECHA
//		}
//		if (index > ClothMesh::numCols) {
//				shearForce += ApplySpring(k_Shear, pos, arrayParts[index - 14 + 1].prevPos, velocity, arrayParts[index - 14 + 1].prevVelocity, glm::length(dist)); //ARRIBA DERECHA
//		}
//		if (index < (ClothMesh::numVerts - ClothMesh::numCols)) {
//				shearForce += ApplySpring(k_Shear, pos, arrayParts[index + 14 + 1].prevPos, velocity, arrayParts[index + 14 + 1].prevVelocity, glm::length(dist)); //ABAJO DERECHA
//		}
//	}
//	resultantForce = (gravityForce * mass)+bendForce+strechForce+shearForce;
//
//	glm::vec3 acceleration = resultantForce / mass;
//	return acceleration;
//}

void Particle::CheckCol(float dt){
	extern bool colisions;
	if (colisions) {
	extern glm::vec3 sphereC;
	extern float sphereR;

	if (index != 0 && index != 13) { //La primera y la ultima de la primera fila estan fijas.
		for (int i = 0; i < 6; i++) {
			if (((glm::dot(planos[i].n, prevPos)) + planos[i].d) * ((glm::dot(planos[i].n, pos)) + planos[i].d) <= 0) {
				//	std::cout << "Collided" << std::endl;

					Bounce(planos[i]);
				
			}
			if (glm::length(pos-sphereC) < sphereR) {
				//HAY QUE CALCULAR EL PUNTO DE INTERSECCION ENTRE LA ESFERA Y LA EQ DE LA RECTA DE LA 
				//TRAYECTORIA DE LA PARTICULA
				glm::vec3 recta = pos - prevPos;
				//std::cout << sphereC.x << ", " << sphereC.y << ", " << sphereC.z << std::endl;

				float a = (recta.x*recta.x) + (recta.y*recta.y) + (recta.z*recta.z);

				float b = ((2 * prevPos.x*recta.x) - (2 * recta.x*sphereC.x))
					+ ((2 * prevPos.y*recta.y) - (2 * recta.y*sphereC.y))
					+ ((2 * prevPos.z*recta.z) - (2 * recta.z*sphereC.z));

				float c = ((prevPos.x*prevPos.x) + (sphereC.x*sphereC.x) - (2 * prevPos.x*sphereC.x)
					+ (prevPos.y*prevPos.y) + (sphereC.y*sphereC.y) - (2 * prevPos.y*sphereC.y)
					+ (prevPos.z*prevPos.z) + (sphereC.z*sphereC.z) - (2 * prevPos.z*sphereC.z))
					- sphereR*sphereR;

				float alpha1 = (-b - glm::sqrt(b*b - 4*a*c)) / 2 * a;
				float alpha2 = (-b - glm::sqrt(b*b - 4 * a*c)) / 2 * a;

				glm::vec3 colPoint,n;
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

				//std::cout << "Posicion anterior-->" << prevPos.x << ", " << prevPos.y << ", " << prevPos.z << "Posicion actual-->" << pos.x << ", " << pos.y << ", " << pos.z << std::endl;
				
				//VERSION 2 PARA ENCONTRAR EL PUNTO DE COLISION

				//glm::vec3 colPointPositivo, colPointNegativo; //Positivo y negativo de la eq de segundo grado.

				//colPointPositivo.x = sphereC.x + (sphereR*(pos.x - sphereC.x)) / (glm::sqrt(glm::pow(pos.x - sphereC.x, 2) + glm::pow(pos.y - sphereC.y, 2) + glm::pow(pos.z - sphereC.z, 2)));
				//colPointPositivo.y = sphereC.y + (sphereR*(pos.x - sphereC.x)) / (glm::sqrt(glm::pow(pos.x - sphereC.x, 2) + glm::pow(pos.y - sphereC.y, 2) + glm::pow(pos.z - sphereC.z, 2)));
				//colPointPositivo.z = sphereC.z + (sphereR*(pos.x - sphereC.x)) / (glm::sqrt(glm::pow(pos.x - sphereC.x, 2) + glm::pow(pos.y - sphereC.y, 2) + glm::pow(pos.z - sphereC.z, 2)));

				//colPointNegativo.x = sphereC.x - (sphereR*(pos.x - sphereC.x)) / (glm::sqrt(glm::pow(pos.x - sphereC.x, 2) + glm::pow(pos.y - sphereC.y, 2) + glm::pow(pos.z - sphereC.z, 2)));
				//colPointNegativo.y = sphereC.y - (sphereR*(pos.x - sphereC.x)) / (glm::sqrt(glm::pow(pos.x - sphereC.x, 2) + glm::pow(pos.y - sphereC.y, 2) + glm::pow(pos.z - sphereC.z, 2)));
				//colPointNegativo.z = sphereC.z - (sphereR*(pos.x - sphereC.x)) / (glm::sqrt(glm::pow(pos.x - sphereC.x, 2) + glm::pow(pos.y - sphereC.y, 2) + glm::pow(pos.z - sphereC.z, 2)));
				//
				//float dist1, dist2;
				//dist1 = glm::distance(colPointPositivo, pos);
				//dist2 = glm::distance(colPointNegativo, pos);

				//glm::vec3 n;
				//int D;

				//if (dist1 < dist2) {
				//	n = colPointPositivo - sphereC;
				//	D = -glm::dot(n, colPointPositivo);
				//}
				//else {
				//	n = colPointNegativo - sphereC;
				//	n = glm::normalize(n);
				//	D = -glm::dot(n, colPointNegativo);
				//}
				if (n != glm::vec3(0)) {
					Bounce(Plane(n.x, n.y, n.z, D));
				}
				
			}

		}
	
	//if ((abs(sphereC.x - pos.x) < sphereR) && (abs(sphereC.y - pos.y) < sphereR) && (abs(sphereC.z - pos.z) < sphereR)) {
	//	Plane planoTan;
	//	planoTan.SetPlaneStats(pos.x-sphereC.x,pos.y- sphereC.y,pos.z- sphereC.z, -((pos.x - sphereC.x * pos.x) + (pos.y - sphereC.y * pos.y) + (pos.z - sphereC.z * pos.z)));
	//	Bounce(planoTan);
	//}
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
