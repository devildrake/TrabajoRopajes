#include <imgui\imgui.h>
#include <imgui\imgui_impl_glfw_gl3.h>
#include "..\include\Particle.h"

bool show_test_window = false;
glm::vec3 gravity;
Particle* arrayParts;
float* arrayPos;
float resetTime;
glm::vec3 sphereC;
float sphereR;

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
		ImGui::DragFloat3("gravity", &gravity.x, 0.1f, -10.f, 10.f);

		//ImGui::DragFloat2("k_strech");
		//ImGui::DragFloat2("k_shear");
		//ImGui::DragFloat2("k_bend");
		//ImGui::DragFloat("Particle Link Di");
		//ImGui::DragFloat("Max Elongation");

		//ImGui::Checkbox("Use elongation Correction");
		//ImGui::Checkbox("Use Collisions");
		//ImGui::Checkbox("Use Sphere Collider");

		//ImGui::DragFloat("Elastic Coefficient", &resetTime, 0.2f, 0, 20);
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

	sphereC = glm::vec3(0.f,5.f,0.f);
	sphereR = 1;
	Sphere::updateSphere(sphereC, sphereR);

	//TODO
	arrayParts = new Particle[ClothMesh::numVerts]; //array de todas las particulas
	arrayPos = new float[ClothMesh::numVerts*3]; //array de las posiciones de las particulas
	gravity = glm::vec3(0, -9.81, 0);
	resetTime = 20.0f;
	//Se instancian las particulas en su posicion inicial
	for (int i = 0; i < ClothMesh::numCols; i++) {
		for (int j = 0; j < ClothMesh::numRows; j++) {
			arrayParts[j * 14 + i].index = j * 14 + i;
			arrayParts[j * 14 + i].StartParticle(glm::vec3(j*0.2f - 4.f, 5, i*0.2f - 2.5f));
		}
	}

}
void PhysicsUpdate(float dt) {

	Sphere::updateSphere(sphereC,sphereR);
	//TODO
	for (int i = 0; i < ClothMesh::numVerts; i++) {
		arrayParts[i].Particle::UpdateParticle(dt, gravity);
	}
	
	//Se guardan en el array de posiciones las posiciones nuevas de cada particula
	for (int i = 0; i < ClothMesh::numVerts; i ++) {
		arrayPos[i*3] = arrayParts[i].pos.x;
		arrayPos[i*3 + 1] = arrayParts[i].pos.y;
		arrayPos[i*3 + 2] = arrayParts[i].pos.z;
	}

	//Metodo que pinta las particulas. Recibe un array con las posiciones de las particulas
	ClothMesh::updateClothMesh(arrayPos);

}
void PhysicsCleanup() {
	//TODO
}