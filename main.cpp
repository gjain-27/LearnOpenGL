#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 

#include "Shader.hpp"
#include "Camera.hpp"

#include <iostream>

void framebuffer_size_callback(GLFWwindow*, int width, int height);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);

void processInput(GLFWwindow* window);
void processMovement(GLFWwindow* window, Camera& camera, float deltaTime);

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetScrollCallback(window, scroll_callback);

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	glEnable(GL_DEPTH_TEST);

	Shader lightingShader("./shaders/colors.vert", "./shaders/colors.frag");
	Shader lightCubeShader("./shaders/light_cube.vert", "./shaders/light_cube.frag");

	float vertices[] = {
		-0.5f,-0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		 0.5f,-0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		 0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

		-0.5f,-0.5f,-0.5f, 0.0f, 0.0f,-1.0f,
		 0.5f,-0.5f,-0.5f, 0.0f, 0.0f,-1.0f,
		 0.5f, 0.5f,-0.5f, 0.0f, 0.0f,-1.0f,
		-0.5f, 0.5f,-0.5f, 0.0f, 0.0f,-1.0f,

		-0.5f,-0.5f,-0.5f,-1.0f, 0.0f, 0.0f,
		-0.5f,-0.5f, 0.5f,-1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f,-1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f,-0.5f,-1.0f, 0.0f, 0.0f,

		 0.5f,-0.5f,-0.5f, 1.0f, 0.0f, 0.0f,
		 0.5f,-0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
		 0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
		 0.5f, 0.5f,-0.5f, 1.0f, 0.0f, 0.0f,

		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f, 0.5f,-0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f,-0.5f, 0.0f, 1.0f, 0.0f,

		-0.5f,-0.5f, 0.5f, 0.0f,-1.0f, 0.0f,
		 0.5f,-0.5f, 0.5f, 0.0f,-1.0f, 0.0f,
		 0.5f,-0.5f,-0.5f, 0.0f,-1.0f, 0.0f,
		-0.5f,-0.5f,-0.5f, 0.0f,-1.0f, 0.0f
	};

	unsigned int indices[] = {
		 0,1,2, 2,3,0,
		 4,5,6, 6,7,4,
		 8,9,10, 10,11,8,
		 12,13,14, 14,15,12,
		 16,17,18, 18,19,16,
		 20,21,22, 22,23,20
	};

	unsigned int cubeVAO, VBO, EBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(cubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	unsigned int lightCubeVAO;
	glGenVertexArrays(1, &lightCubeVAO);
	glBindVertexArray(lightCubeVAO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	Camera camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
	glfwSetWindowUserPointer(window, &camera);

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	float deltaTime = 0.0f;
	float previousTime = 0.0f;

	glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f);

	while (!glfwWindowShouldClose(window)) {

		float currentTime = static_cast<float>(glfwGetTime());
		deltaTime = currentTime - previousTime;
		previousTime = currentTime;

		processInput(window);
		processMovement(window, camera, deltaTime);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		view = camera.GetViewMatrix();
		projection = glm::perspective(glm::radians(camera.GetZoom()), 800.0f / 600.0f, 0.1f, 100.0f);

		lightingShader.use();
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);

		lightingShader.setVec3("light.position", lightPos);
		lightingShader.setVec3("viewPos", camera.getCameraPos());

		lightingShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
		lightingShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
		lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

		lightingShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
		lightingShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
		lightingShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
		lightingShader.setFloat("material.shininess", 32.0f);

		model = glm::mat4(1.0f);
		lightingShader.setMat4("model", model);

		glBindVertexArray(cubeVAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		lightCubeShader.use();
		lightCubeShader.setMat4("projection", projection);
		lightCubeShader.setMat4("view", view);

		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		lightCubeShader.setMat4("model", model);

		glBindVertexArray(lightCubeVAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightCubeVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void processMovement(GLFWwindow* window, Camera& camera, float deltaTime) {
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(CameraMovement::Forward, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(CameraMovement::Backward, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(CameraMovement::Left, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(CameraMovement::Right, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos) {
	static float lastX = 400.0f;
	static float lastY = 300.0f;
	static bool firstMouse = true;

	if (firstMouse) {
		lastX = static_cast<float>(xPos);
		lastY = static_cast<float>(yPos);
		firstMouse = false;
	}

	float xOffset = static_cast<float>(xPos) - lastX;
	float yOffset = lastY - static_cast<float>(yPos);

	lastX = static_cast<float>(xPos);
	lastY = static_cast<float>(yPos);

	Camera* cam = static_cast<Camera*>(glfwGetWindowUserPointer(window));
	cam->ProcessMouseMovement(xOffset, yOffset);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
	Camera* cam = static_cast<Camera*>(glfwGetWindowUserPointer(window));
	cam->ProcessMouseScroll(static_cast<float>(yOffset));
}