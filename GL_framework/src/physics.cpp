#include <imgui\imgui.h>
#include <imgui\imgui_impl_glfw_gl3.h>
#include "..\include\Particle.h"
#include <math.h>

bool show_test_window = false;
bool reset = false;
bool colisions = true;
glm::vec3 gravity;
Particle* arrayParts;

float* arrayPos;
float m_elongation;
float contador;

float resetTime;

glm::vec2 k_Strech;
glm::vec2 k_Shear;
glm::vec2 k_bend;

glm::vec3 sphereC;
float sphereR;

float elasticCoeficientBounce;

glm::vec3 dist;


namespace ClothMesh {
	extern void updateClothMesh(float* array_data);
	extern const int numCols;
	extern const int numRows;
	extern const int numVerts;
}

namespace Sphere {
	extern void updateSphere(glm::vec3 pos, float radius = 1.f);
}
void GUI() {
	{	//FrameRate
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::DragFloat("Reset Time", &resetTime,0.2f,0,20);
		ImGui::Checkbox("Enabled Collisons",&colisions);

		//ImGui::DragFloat3("gravity", &gravity.x, 0.1f, -10.f, 10.f);
		
		ImGui::Checkbox("Reset",&reset);


		ImGui::SmallButton("Pointless Button - Beware, pointless");

		ImGui::SliderFloat3("gravity", &gravity.x, -10, 10);

		ImGui::DragFloat2("k_strech", &k_Strech.x, 0.002f, 0, 10);
		ImGui::DragFloat2("k_shear", &k_Shear.x, 0.002f, 0, 10);
		ImGui::DragFloat2("k_bend", &k_bend.x, 0.002f, 0, 10);


		//ImGui::DragFloat("Particle Link Di");
		ImGui::DragFloat("Max Elongation",&m_elongation,0.01f,0.02f,0.2f);

		//ImGui::Checkbox("Use elongation Correction");
		//ImGui::Checkbox("Use Collisions");
		//ImGui::Checkbox("Use Sphere Collider");

		ImGui::SliderFloat("Elasticity", &elasticCoeficientBounce, 0, 1);

		//ImGui::DragFloat("Elastic Coefficient", &elasticCoeficient, 0.1f, 0, 1);
		//ImGui::DragFloat("Friction Coefficient", &gravity.x, 0.1f, -10.f, 10.f);

		//TODO
	}

	// ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
	if(show_test_window) {
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
		ImGui::ShowTestWindow(&show_test_window);
	}
}

void PhysicsInit() {
	contador = 0;
	//k_Strech = k_bend = k_Shear = glm::vec2(0.5f, 0.5f);
	//k_Strech = glm::vec2(1, 1);
	k_Shear = glm::vec2(0.2f, 0.2f);
	//k_bend = glm::vec2(0.2f, 0.2f);


	m_elongation = 0.05f;
	dist.x = dist.z = 0.5f;
	dist.y = 0;
	elasticCoeficientBounce = 0.5f;
	sphereC = glm::vec3(0.f,5.f,0.f);
	sphereR = 1;
	Sphere::updateSphere(sphereC, sphereR);

	//TODO
	arrayParts = new Particle[ClothMesh::numVerts]; //array de todas las particulas
	arrayPos = new float[ClothMesh::numVerts*3]; //array de las posiciones de las particulas
	gravity = glm::vec3(0, -9.8f, 0);
	resetTime = 4.0f;
	//Se instancian las particulas en su posicion inicial
	for (int i = 0; i < ClothMesh::numCols; i++) {
		for (int j = 0; j < ClothMesh::numRows; j++) {
			arrayParts[j * 14 + i].index = j * 14 + i;
			arrayParts[j * 14 + i].StartParticle(glm::vec3(j*dist.x - 4.f, 8, i*dist.z - 2.5f));
		}
	}

}

glm::vec3 ApplySpring(glm::vec2 k, glm::vec3 p1, glm::vec3 p2, glm::vec3 v1, glm::vec3 v2, float distOrg) {
	glm::vec3 a = glm::normalize(p1 - p2);
	return (-(k.x*(glm::length(p1 - p2) - distOrg) + k.y*glm::dot((v1 - v2), a))*a);
}

glm::vec3 spring(Particle part1, Particle part2, float distance)
{
	glm::vec3 force, damp;
	float elast;
	glm::vec3 normalizedPos = ((part2.pos - part1.pos) / glm::length(part2.pos - part1.pos));
	elast = elasticCoeficientBounce * (glm::length(part2.pos - part1.pos) - distance);
	damp = k_Shear.y * (part2.velocity - part1.velocity) * normalizedPos;
	force = (elast + damp) * normalizedPos;

	return force;
}

void CorrectPosLeft(int i,float l) {
	float distanciaACorregir = l - dist.x;
	glm::vec3 vectorA = arrayParts[i].pos-arrayParts[i-1].pos;
	vectorA = vectorA / (glm::length(vectorA));
	

	if (i != 13) {
		if (i - 1 != 0 && i - 1 != 13) {
			distanciaACorregir = distanciaACorregir / 2;
			arrayParts[i].pos -= vectorA*distanciaACorregir;
			arrayParts[i - 1].pos += vectorA*distanciaACorregir;
			
		}
		else {
			arrayParts[i].pos -= vectorA*distanciaACorregir;
			
		}
	}
	else {
		arrayParts[i-1].pos += vectorA*distanciaACorregir;
		
	}
}

void CorrectPosUp(int i,float l) {
	float distanciaACorregir = l - dist.x;
	glm::vec3 vectorA = arrayParts[i].pos - arrayParts[i - ClothMesh::numCols].pos;
	vectorA = vectorA / (glm::length(vectorA));

	if (i != 0 && i != 13) {
		if ((i - ClothMesh::numCols) != 0 && (i - ClothMesh::numCols) != 13) {
			distanciaACorregir = distanciaACorregir / 2;
			arrayParts[i].pos -= vectorA*distanciaACorregir;
			arrayParts[i - ClothMesh::numCols].pos += vectorA*distanciaACorregir;
		}
		else {
			arrayParts[i].pos -= vectorA*distanciaACorregir;
		}


	}
	else {
		arrayParts[i - ClothMesh::numCols].pos += vectorA*distanciaACorregir;
	}
}

void PhysicsUpdate(float dt) {
	Sphere::updateSphere(sphereC, sphereR);
	contador += dt;

	if (contador > resetTime||reset) {
		for (int i = 0; i < ClothMesh::numVerts; i++) {
			arrayParts[i].Reset();
		}
		contador = 0;
		reset = false;
	}

	for (int j = 0; j < ClothMesh::numVerts; j++) {
		//Actualización de velocidades/posiciones/checkCol
		arrayParts[j].Particle::UpdateParticle(dt, gravity);
	}

	for (int i = 0; i < 150; i++) {
		for (int j = 0; j < ClothMesh::numVerts; j++) {
			//Comprobacion del ratio de deformación
			if (arrayParts[j].index % ClothMesh::numCols > 0) {
				//Comprobar si se corrige por la izquierda
				float longitud = glm::length(arrayParts[j].pos-arrayParts[j-1].pos);
				if (longitud > dist.x + m_elongation) {
					CorrectPosLeft(j,longitud);
				}
			}

			if (arrayParts[j].index > (ClothMesh::numCols-1)) {
				//Comprobar si se corrige por aqrriba
				float longitud = glm::length(arrayParts[j].pos - arrayParts[j - ClothMesh::numCols].pos);
				if (longitud > dist.z + m_elongation) {
					CorrectPosUp(j,longitud);
				}
			}
		}	


		for (int j = 0; j < ClothMesh::numVerts; j++) {
			//Actualización de velocidades/posiciones/checkCol
			arrayParts[j].Particle::CheckCol(dt);
		}

		//Se guardan en el array de posiciones las posiciones nuevas de cada particula
		for (int j = 0; j < ClothMesh::numVerts; j++) {
			arrayPos[j * 3] = arrayParts[j].pos.x;
			arrayPos[j * 3 + 1] = arrayParts[j].pos.y;
			arrayPos[j * 3 + 2] = arrayParts[j].pos.z;
		}
	}
	//Metodo que pinta las particulas. Recibe un array con las posiciones de las particulas
	
	for (int i = 0; i < ClothMesh::numRows; i++) {
		for (int j = 0; j < ClothMesh::numCols; j++)
		{
			glm::vec3 springForce;
			int ind = i * ClothMesh::numCols + j;
			int up = (i - 1) * ClothMesh::numCols + j;
			int down = (i + 1) * ClothMesh::numCols + j;
			int left = (i)* ClothMesh::numCols + j - 1;
			int right = (i)* ClothMesh::numCols + j + 1;
			int up2 = (i - 2) * ClothMesh::numCols + j;
			int down2 = (i + 2) * ClothMesh::numCols + j;
			int left2 = (i)* ClothMesh::numCols + j - 2;
			int right2 = (i)* ClothMesh::numCols + j + 2;
			int upL = (i - 1) * ClothMesh::numCols + j - 1;
			int upR = (i - 1) * ClothMesh::numCols + j + 1;
			int downR = (i + 1) * ClothMesh::numCols + j + 1;
			int downL = (i + 1) * ClothMesh::numCols + j - 1;

			if (i == 0 && j == 0) { //TOP LEFT CORNER
				springForce = spring(arrayParts[ind], arrayParts[right], dist.x);

				arrayParts[ind].addForce(springForce);
				arrayParts[right].addForce(-springForce);

				springForce = spring(arrayParts[ind], arrayParts[down], dist.x);
				arrayParts[ind].addForce(springForce);
				arrayParts[down].addForce(-springForce);

			}
			else if (i == ClothMesh::numRows - 1 && j == 0) { //DOWN LEFT CORNER
				springForce = spring(arrayParts[ind], arrayParts[up], dist.x);
				arrayParts[ind].addForce(springForce);
				arrayParts[up].addForce(-springForce);

				springForce = spring(arrayParts[ind], arrayParts[right], dist.x);
				arrayParts[ind].addForce(springForce);
				arrayParts[right].addForce(-springForce);
			}
			else if (i == 0 && j == ClothMesh::numCols - 1) { //TOP RIGHT CORNER
				springForce = spring(arrayParts[ind], arrayParts[left], dist.x);
				arrayParts[ind].addForce(springForce);
				arrayParts[left].addForce(-springForce);

				springForce = spring(arrayParts[ind], arrayParts[down], dist.x);
				arrayParts[ind].addForce(springForce);
				arrayParts[down].addForce(-springForce);
			}
			else if (i == ClothMesh::numRows - 1 && j == ClothMesh::numCols - 1) { //DOWN RIGHT CORNER
				springForce = spring(arrayParts[ind], arrayParts[up], dist.x);
				arrayParts[ind].addForce(springForce);
				arrayParts[up].addForce(-springForce);

				springForce = spring(arrayParts[ind], arrayParts[left], dist.x);
				arrayParts[ind].addForce(springForce);
				arrayParts[left].addForce(-springForce);
			}
			else if (i > 1 && i < ClothMesh::numRows - 2 && j > 1 && j < ClothMesh::numCols - 2) { //INNER SQUARE
				springForce = spring(arrayParts[ind], arrayParts[up], dist.x);
				arrayParts[ind].addForce(springForce);
				arrayParts[up].addForce(-springForce);

				springForce = spring(arrayParts[ind], arrayParts[down], dist.x);
				arrayParts[ind].addForce(springForce);
				arrayParts[down].addForce(-springForce);

				springForce = spring(arrayParts[ind], arrayParts[left], dist.x);
				arrayParts[ind].addForce(springForce);
				arrayParts[left].addForce(-springForce);

				springForce = spring(arrayParts[ind], arrayParts[right], dist.x);
				arrayParts[ind].addForce(springForce);
				arrayParts[right].addForce(-springForce);

				springForce = spring(arrayParts[ind], arrayParts[up2], 2 * dist.x);
				arrayParts[ind].addForce(springForce);
				arrayParts[up2].addForce(-springForce);

				springForce = spring(arrayParts[ind], arrayParts[down2], 2 * dist.x);
				arrayParts[ind].addForce(springForce);
				arrayParts[down2].addForce(-springForce);

				springForce = spring(arrayParts[ind], arrayParts[left2], 2 * dist.x);
				arrayParts[ind].addForce(springForce);
				arrayParts[left2].addForce(-springForce);

				springForce = spring(arrayParts[ind], arrayParts[right2], 2 * dist.x);
				arrayParts[ind].addForce(springForce);
				arrayParts[right2].addForce(-springForce);

				springForce = spring(arrayParts[ind], arrayParts[upR], glm::length(dist));
				arrayParts[ind].addForce(springForce);
				arrayParts[upR].addForce(-springForce);

				springForce = spring(arrayParts[ind], arrayParts[downR], glm::length(dist));
				arrayParts[ind].addForce(springForce);
				arrayParts[downR].addForce(-springForce);

				springForce = spring(arrayParts[ind], arrayParts[upL], glm::length(dist));
				arrayParts[ind].addForce(springForce);
				arrayParts[upL].addForce(-springForce);

				springForce = spring(arrayParts[ind], arrayParts[downL], glm::length(dist));
				arrayParts[ind].addForce(springForce);
				arrayParts[downL].addForce(-springForce);
			}
			else if (((j == 1 || j == ClothMesh::numCols - 2) && (i > 0 && i < ClothMesh::numRows - 1)) || ((i == 1 || i == ClothMesh::numRows - 2) && (j > 0 && j < ClothMesh::numCols - 1))) { //LEFT ONE COLUMN  
				springForce = spring(arrayParts[ind], arrayParts[upL], glm::length(dist));
				arrayParts[ind].addForce(springForce);
				arrayParts[upL].addForce(-springForce);

				springForce = spring(arrayParts[ind], arrayParts[up], dist.x);
				arrayParts[ind].addForce(springForce);
				arrayParts[up].addForce(-springForce);

				springForce = spring(arrayParts[ind], arrayParts[upR], glm::length(dist));
				arrayParts[ind].addForce(springForce);
				arrayParts[upR].addForce(-springForce);

				springForce = spring(arrayParts[ind], arrayParts[left], dist.x);
				arrayParts[ind].addForce(springForce);
				arrayParts[left].addForce(-springForce);

				springForce = spring(arrayParts[ind], arrayParts[right], dist.x);
				arrayParts[ind].addForce(springForce);
				arrayParts[right].addForce(-springForce);

				springForce = spring(arrayParts[ind], arrayParts[downL], glm::length(dist));
				arrayParts[ind].addForce(springForce);
				arrayParts[downL].addForce(-springForce);

				springForce = spring(arrayParts[ind], arrayParts[down], dist.x);
				arrayParts[ind].addForce(springForce);
				arrayParts[down].addForce(-springForce);

				springForce = spring(arrayParts[ind], arrayParts[downR], glm::length(dist));
				arrayParts[ind].addForce(springForce);
				arrayParts[downR].addForce(-springForce);
			}

		}

	}

	ClothMesh::updateClothMesh(arrayPos);
	
}
void PhysicsCleanup() {
	//TODO
	delete[] arrayParts;
	delete[] arrayPos;
}