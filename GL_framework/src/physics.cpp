#include <imgui\imgui.h>
#include <imgui\imgui_impl_glfw_gl3.h>
#include "..\include\Particle.h"
#include <math.h>

bool show_test_window = false;
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

		//ImGui::DragFloat3("gravity", &gravity.x, 0.1f, -10.f, 10.f);

		ImGui::SmallButton("Pointless Button - Beware, pointless");

		ImGui::SliderFloat3("gravity", &gravity.x, -10, 10);

		ImGui::DragFloat2("k_strech", &k_Strech.x, 0.002f, 0, 10);
		ImGui::DragFloat2("k_shear", &k_Shear.x, 0.002f, 0, 10);
		ImGui::DragFloat2("k_bend", &k_bend.x, 0.002f, 0, 10);


		//ImGui::DragFloat("Particle Link Di");
		ImGui::DragFloat("Max Elongation",&m_elongation,0.05f,0.05f,0.2f);

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

void CorrectPosLeft(int i,float l) {
	float distanciaACorregir = l - dist.x;
	glm::vec3 vectorA = arrayParts[i].pos-arrayParts[i-1].pos;
	vectorA = vectorA / (glm::length(vectorA));

	if (i != 0 && i != 13) {
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

	contador += dt;

	if (contador > resetTime) {
		for (int i = 0; i < ClothMesh::numVerts; i++) {
			arrayParts[i].Reset();
		}
		contador = 0;
	}

	for (int j = 0; j < ClothMesh::numVerts; j++) {
		//Actualización de velocidades/posiciones/checkCol
		arrayParts[j].Particle::UpdateParticle(dt, gravity);
	}

	for (int i = 0; i < 100; i++) {
		Sphere::updateSphere(sphereC, sphereR);
		//TODO


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
				float longitud = glm::length(arrayParts[j].pos - arrayParts[j - ClothMesh::numCols].pos);
				if (longitud > dist.z + m_elongation) {
					CorrectPosUp(j,longitud);
				}
					//Comprobar si se corrige por aqrriba
			}
		}



		//Se guardan en el array de posiciones las posiciones nuevas de cada particula
		for (int j = 0; j < ClothMesh::numVerts; j++) {
			arrayPos[j * 3] = arrayParts[j].pos.x;
			arrayPos[j * 3 + 1] = arrayParts[j].pos.y;
			arrayPos[j * 3 + 2] = arrayParts[j].pos.z;
		}

		//Metodo que pinta las particulas. Recibe un array con las posiciones de las particulas
		ClothMesh::updateClothMesh(arrayPos);
	}
}
void PhysicsCleanup() {
	//TODO
}