#include "lab_m1/lab4/lab4.h"

#include <vector>
#include <string>
#include <iostream>

#include "lab_m1/lab4/transform3D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab4::Lab4() {}


Lab4::~Lab4() {}


void Lab4::Init() {
	polygonMode = GL_FILL;

	Mesh *mesh = new Mesh("box");
	mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
	meshes[mesh->GetMeshID()] = mesh;

	// Initialize tx, ty and tz (the translation steps)
	translateX = 0;
	translateY = 0;
	translateZ = 0;

	// Initialize sx, sy and sz (the scale factors)
	scaleX = 1;
	scaleY = 1;
	scaleZ = 1;

	// Initialize angular steps
	angularStepOX = 0;
	angularStepOY = 0;
	angularStepOZ = 0;

	// Sets the resolution of the small viewport
	glm::ivec2 resolution = window->GetResolution();
	miniViewportArea = ViewportArea(50, 50, resolution.x / 5.f, resolution.y / 5.f);

	ratio = (float)miniViewportArea.width / (float)miniViewportArea.height;
}

void Lab4::FrameStart() {
	// Clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Lab4::RenderScene() {
	modelMatrix = glm::mat4(1);
	modelMatrix *= transform3D::Translate(-2.5f, 0.5f, -1.5f);
	modelMatrix *= transform3D::Translate(translateX, translateY, translateZ);
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

	modelMatrix = glm::mat4(1);
	modelMatrix *= transform3D::Translate(0.0f, 0.5f, -1.5f);
	modelMatrix *= transform3D::Scale(scaleX, scaleY, scaleZ);
	RenderMesh(meshes["box"], shaders["Simple"], modelMatrix);

	modelMatrix = glm::mat4(1);
	modelMatrix *= transform3D::Translate(2.5f, 0.5f, -1.5f);
	modelMatrix *= transform3D::RotateOX(angularStepOX);
	modelMatrix *= transform3D::RotateOY(angularStepOY);
	modelMatrix *= transform3D::RotateOZ(angularStepOZ);
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
}

void Lab4::Update(float deltaTimeSeconds) {
	glLineWidth(3);
	glPointSize(5);
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

	// Sets the screen area where to draw
	glm::ivec2 resolution = window->GetResolution();
	glViewport(0, 0, resolution.x, resolution.y);

	RenderScene();
	
	pair<float, float> margins = { 0, 20 };

	if (sunX <= margins.first) {
		sign = 1;
	} else if (sunX >= margins.second) {
		sign = -1;
	}

	sunSpeed += sign * deltaTimeSeconds;
	sunX = sunSpeed * 2.5;

	angularStep += TO_RADIANS * deltaTimeSeconds * 30;

	modelMatrix = glm::mat4(1);
	modelMatrix *= transform3D::Translate(sunX, 0, 0);
	modelMatrix *= transform3D::Translate(10, 10, 10);
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

	modelMatrix = glm::mat4(1);
	modelMatrix *= transform3D::Translate(sunX, 0, 0);
	modelMatrix *= transform3D::RotateAroundOXLike(angularStep, glm::vec3(10, 10, 10));
	modelMatrix *= transform3D::Translate(10, 12.5, 12.5);
	modelMatrix *= transform3D::Scale(0.8, 0.8, 0.8);
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);


	modelMatrix = glm::mat4(1);
	modelMatrix *= transform3D::Translate(sunX, 0, 0);
	modelMatrix *= transform3D::RotateAroundOXLike(angularStep, glm::vec3(10, 10, 10));
	modelMatrix *= transform3D::RotateAroundOXLike(angularStep * 3, glm::vec3(10, 12.5, 12.5));
	modelMatrix *= transform3D::Translate(10, 13.5, 13.5);
	modelMatrix *= transform3D::Scale(0.5, 0.5, 0.5);
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

	DrawCoordinateSystem();

	glClear(GL_DEPTH_BUFFER_BIT);
	glViewport(miniViewportArea.x, miniViewportArea.y, miniViewportArea.width, miniViewportArea.height);

	// TODO(student): render the scene again, in the new viewport
	DrawCoordinateSystem();
	RenderScene();
}

void Lab4::FrameEnd() {}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab4::OnInputUpdate(float deltaTime, int mods) {
	// TODO(student): Add transformation logic
	if (window->KeyHold(GLFW_KEY_W) && !window->MouseHold(GLFW_MOUSE_BUTTON_2)) {
		translateY += deltaTime;
	}

	if (window->KeyHold(GLFW_KEY_A) && !window->MouseHold(GLFW_MOUSE_BUTTON_2)) {
		translateX -= deltaTime;
	}

	if (window->KeyHold(GLFW_KEY_S) && !window->MouseHold(GLFW_MOUSE_BUTTON_2)) {
		translateY -= deltaTime;
	}

	if (window->KeyHold(GLFW_KEY_D) && !window->MouseHold(GLFW_MOUSE_BUTTON_2)) {
		translateX += deltaTime;
	}

	if (window->KeyHold(GLFW_KEY_R) && !window->MouseHold(GLFW_MOUSE_BUTTON_2)) {
		translateZ += deltaTime;
	}

	if (window->KeyHold(GLFW_KEY_F) && !window->MouseHold(GLFW_MOUSE_BUTTON_2)) {
		translateZ -= deltaTime;
	}

	if (window->KeyHold(GLFW_KEY_1) && !window->MouseHold(GLFW_MOUSE_BUTTON_2)) {
		scaleX += deltaTime;
		scaleY += deltaTime;
		scaleZ += deltaTime;
	}

	if (window->KeyHold(GLFW_KEY_2) && !window->MouseHold(GLFW_MOUSE_BUTTON_2)) {
		scaleX -= deltaTime;
		scaleY -= deltaTime;
		scaleZ -= deltaTime;
	}

	if (window->KeyHold(GLFW_KEY_3) && !window->MouseHold(GLFW_MOUSE_BUTTON_2)) {
		angularStepOX += deltaTime;
	}

	if (window->KeyHold(GLFW_KEY_4) && !window->MouseHold(GLFW_MOUSE_BUTTON_2)) {
		angularStepOX -= deltaTime;
	}

	if (window->KeyHold(GLFW_KEY_5) && !window->MouseHold(GLFW_MOUSE_BUTTON_2)) {
		angularStepOY += deltaTime;
	}

	if (window->KeyHold(GLFW_KEY_6) && !window->MouseHold(GLFW_MOUSE_BUTTON_2)) {
		angularStepOY -= deltaTime;
	}

	if (window->KeyHold(GLFW_KEY_7) && !window->MouseHold(GLFW_MOUSE_BUTTON_2)) {
		angularStepOZ += deltaTime;
	}

	if (window->KeyHold(GLFW_KEY_8) && !window->MouseHold(GLFW_MOUSE_BUTTON_2)) {
		angularStepOZ -= deltaTime;
	}

	if (window->KeyHold(GLFW_KEY_I) && !window->MouseHold(GLFW_MOUSE_BUTTON_2)) {
		miniViewportArea.y += deltaTime * 100;
	}

	if (window->KeyHold(GLFW_KEY_J) && !window->MouseHold(GLFW_MOUSE_BUTTON_2)) {
		miniViewportArea.x -= deltaTime * 100;
	}

	if (window->KeyHold(GLFW_KEY_K) && !window->MouseHold(GLFW_MOUSE_BUTTON_2)) {
		miniViewportArea.y -= deltaTime * 100;
	}

	if (window->KeyHold(GLFW_KEY_L) && !window->MouseHold(GLFW_MOUSE_BUTTON_2)) {
		miniViewportArea.x += deltaTime * 100;
	}

	if (window->KeyHold(GLFW_KEY_O) && !window->MouseHold(GLFW_MOUSE_BUTTON_2)) {
		miniViewportArea.height += deltaTime * 100;
		miniViewportArea.width = miniViewportArea.height * ratio;
	}

	if (window->KeyHold(GLFW_KEY_P) && !window->MouseHold(GLFW_MOUSE_BUTTON_2)) {
		miniViewportArea.height -= deltaTime * 100;
		miniViewportArea.width = miniViewportArea.height * ratio;
	}
}


void Lab4::OnKeyPress(int key, int mods) {
	// Add key press event
	if (key == GLFW_KEY_SPACE) {
		switch (polygonMode) {
			case GL_POINT:
				polygonMode = GL_FILL;
				break;
			case GL_LINE:
				polygonMode = GL_POINT;
				break;
			default:
				polygonMode = GL_LINE;
				break;
		}
	}

	// TODO(student): Add viewport movement and scaling logic
}


void Lab4::OnKeyRelease(int key, int mods) {
	// Add key release event
}


void Lab4::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {
	// Add mouse move event
}


void Lab4::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {
	// Add mouse button press event
}


void Lab4::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {
	// Add mouse button release event
}


void Lab4::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {}


void Lab4::OnWindowResize(int width, int height) {}
