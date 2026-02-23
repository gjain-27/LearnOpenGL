#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Shader.hpp"

#include <iostream>

void framebuffer_size_callback(GLFWwindow*, int width, int height);
void processInput(GLFWwindow* window);

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	Shader shader("./shaders/default.vert", "./shaders/default.frag");

	float vertices[] = {
		-0.5f,-0.5f, 0.5f,  0.0f,0.0f,
		 0.5f,-0.5f, 0.5f,  1.0f,0.0f,
		 0.5f, 0.5f, 0.5f,  1.0f,1.0f,
		-0.5f, 0.5f, 0.5f,  0.0f,1.0f,

		-0.5f,-0.5f,-0.5f,  1.0f,0.0f,
		 0.5f,-0.5f,-0.5f,  0.0f,0.0f,
		 0.5f, 0.5f,-0.5f,  0.0f,1.0f,
		-0.5f, 0.5f,-0.5f,  1.0f,1.0f,

		-0.5f,-0.5f,-0.5f,  0.0f,0.0f,
		-0.5f,-0.5f, 0.5f,  1.0f,0.0f,
		-0.5f, 0.5f, 0.5f,  1.0f,1.0f,
		-0.5f, 0.5f,-0.5f,  0.0f,1.0f,

		 0.5f,-0.5f,-0.5f,  1.0f,0.0f,
		 0.5f,-0.5f, 0.5f,  0.0f,0.0f,
		 0.5f, 0.5f, 0.5f,  0.0f,1.0f,
		 0.5f, 0.5f,-0.5f,  1.0f,1.0f,

		-0.5f, 0.5f, 0.5f,  0.0f,0.0f,
		 0.5f, 0.5f, 0.5f,  1.0f,0.0f,
		 0.5f, 0.5f,-0.5f,  1.0f,1.0f,
		-0.5f, 0.5f,-0.5f,  0.0f,1.0f,

		-0.5f,-0.5f, 0.5f,  1.0f,1.0f,
		 0.5f,-0.5f, 0.5f,  0.0f,1.0f,
		 0.5f,-0.5f,-0.5f,  0.0f,0.0f,
		-0.5f,-0.5f,-0.5f,  1.0f,0.0f
	};

	unsigned int indices[] = {
		 0,1,2, 2,3,0,
		 4,5,6, 6,7,4,
		 8,9,10, 10,11,8,
		 12,13,14, 14,15,12,
		 16,17,18, 18,19,16,
		 20,21,22, 22,23,20
	};

	unsigned int VAO, VBO, EBO;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, false, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, false, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	int texWidth, texHeight, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("./resources/textures/container.jpg", &texWidth, &texHeight, &nrChannels, 0);

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	shader.use();
	shader.setInt("texture1", 0);
	glBindVertexArray(VAO);

	int modelLoc = glGetUniformLocation(shader.ID, "model");
	int viewLoc = glGetUniformLocation(shader.ID, "view");
	int projectionLoc = glGetUniformLocation(shader.ID, "projection");

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		model = glm::mat4(1.0f);
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));


		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
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