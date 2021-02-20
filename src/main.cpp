#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <stb_image.h>

#include <iostream>

#include "shader.h"
#include "engine.h"

const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;

void process_input(GLFWwindow* window);

class MyTestEngine : public pix_eng::Engine {

public:
	int x_pos = 0;
	int y_pos = 50;

	bool create() override {
		return true;
	}

	bool update(double delta_time) override {
		if (get_key(pix_eng::Key::ESCAPE).pressed) {
			return close();
		}

		clear(pix_eng::Pixel());

		std::stringstream ss;
		ss << 1000.0 / delta_time << " fps";
		set_title(ss.str());

		rect(400, 200, 20, 20, pix_eng::Pixel(255, 0, 0, 100), pix_eng::Pixel(255, 0, 0, 100));

		// point(x_pos, 20, pix_eng::Pixel(255, 255, 0, 255));
		rect(x_pos, y_pos, 150, 100, pix_eng::Pixel(255, 255, 0, 128), pix_eng::Pixel(255, 255, 0, 128));

		rect(300, 200, 20, 20, pix_eng::Pixel(255, 0, 0, 100), pix_eng::Pixel(255, 0, 0, 100));

		if (get_key(pix_eng::Key::RIGHT).held || get_mouse_btn(1).held) {
			x_pos = (x_pos + 1) % get_canvas_width();
		}
		if (get_key(pix_eng::Key::LEFT).held || get_mouse_btn(2).held) {
			x_pos--;
			if (x_pos < 0) x_pos = get_canvas_width();
		}

		if (get_key(pix_eng::Key::M).pressed) {
			set_screen_width(get_screen_width() == 640 ? 320 : 640);
		}

		if (get_mouse_btn(0).held) {
			x_pos = (int) floor(get_mouseX_rel() * get_canvas_width());
			y_pos = (int) floor(get_mouseY_rel() * get_canvas_height());
		}

		draw_sprite(100, 100, font_sprite);

		text(50, 50, 2, "THE\nQUICK\nBROWN\nFOX\nJUMPS\nOVER\nTHE\nLAZY\nDOG.", pix_eng::Pixel(0, 255, 0));

		return true;
	}

};

int test();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main() {
	// test();
	MyTestEngine eng;
	if (eng.initialise()) {
		eng.start();
	}
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

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	// vertex data for a square (will scale and transform later)
	float vertices[] = {
		-0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f
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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	Shader shader("res/shaders/canvas.vert", "res/shaders/canvas.frag");

	int width, height, nrChannels;
	unsigned char *data = stbi_load("res/images/awesomeface.png", &width, &height, &nrChannels, 0);
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		std::cout << "Failed to read data" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}
	stbi_image_free(data);

    while (!glfwWindowShouldClose(window)) {
		process_input(window);

		// background fill
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// rendering stuff goes here
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(VAO);
		shader.use();
		shader.set_int("cTex", 0);
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

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
