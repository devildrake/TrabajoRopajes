#include <imgui\imgui.h>
#include <imgui\imgui_impl_glfw_gl3.h>
#include "..\include\Particle.h"

bool show_test_window = false;
glm::vec3 gravity;
Particle* arrayParts;
float* arrayPos;
namespace ClothMesh {
	extern void updateClothMesh(float* array_data);
	extern const int numCols;
	extern const int numRows;
	extern const int numVerts;
}

void GUI() {
	{	//FrameRate
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		//TODO
	}

	// ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
	if(show_test_window) {
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
		ImGui::ShowTestWindow(&show_test_window);
	}
}

void PhysicsInit() {
	//TODO
	arrayParts = new Particle[ClothMesh::numVerts]; //array de todas las particulas
	arrayPos = new float[ClothMesh::numVerts*3]; //array de las posiciones de las particulas
	gravity = glm::vec3(0, -9.81, 0);

	//Se instancian las particulas en su posicion inicial
	for (int i = 0; i < ClothMesh::numCols; i++) {
		for (int j = 0; j < ClothMesh::numRows; j++) {
			arrayParts[j * 14 + i].pos.x = j*0.6f-5.f;
			arrayParts[j * 14 + i].pos.y = 5;
			arrayParts[j * 14 + i].pos.z = i*0.4f-2.5f;
		}
	}

}
void PhysicsUpdate(float dt) {
	//TODO
	for (int i = 0; i < ClothMesh::numVerts; i++) {
		arrayParts[i].Particle::UpdateParticle(dt);
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