#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>

#include "shader.h"
#include "engine.h"

const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;

void process_input(GLFWwindow* window);

class MyTestEngine : public pix_eng::Engine {

public:
	bool create() override {
		return true;
	}

	bool update(double delta_time) override {
		return true;
	}

};

int test();

int main() {
	MyTestEngine eng;
	eng.start();
	eng.point(100, 100, pix_eng::Pixel(255, 0, 0, 255));
	// if (eng.initialise(640, 480, "test")) {
	// 	eng.start();
	// }
}

int test() {
	// opengl setup
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Learn OpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialise GLAD" << std::endl;
		return EXIT_FAILURE;
	}

	// create the window and capture inputs
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	// vertex data for a square (will scale and transform later)
	float vertices[] = {
		-0.5f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f
	};

	// index data for a square
	int indices[] {
		0, 1, 2,
		0, 2, 3
	};

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	Shader shader("res/shaders/square.vert", "res/shaders/square.frag");

    while (!glfwWindowShouldClose(window)) {
		process_input(window);

		// background fill
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// rendering stuff goes here
		glBindVertexArray(VAO);
		shader.use();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
		glBindVertexArray(0);

		// swap buffers and draw
		glfwSwapBuffers(window);
		glfwPollEvents();
    }

	glfwTerminate();
	return EXIT_SUCCESS;
}

void process_input(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
    }
}