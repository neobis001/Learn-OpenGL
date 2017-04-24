#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

int main()
{
	// Initialize GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	// Setup viewport
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	// Enable stencil test, and get stencil bits
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_DEPTH_TEST);
	glfwWindowHint(GLFW_STENCIL_BITS, 8);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);

	GLint stencil_bits = 0;
	glGetFramebufferAttachmentParameteriv(GL_DRAW_FRAMEBUFFER,
		GL_STENCIL, GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE, &stencil_bits);
	std::cout << stencil_bits << std::endl;

	GLenum error = glGetError();
	std::cout << (error == GL_INVALID_ENUM) << std::endl;

	glGetIntegerv(GL_STENCIL_BITS, &stencil_bits);
	std::cout << stencil_bits << std::endl;
	
	GLenum error2 = glGetError();
	std::cout << (error2 == GL_INVALID_ENUM) << std::endl;

	// window loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	return 0;
}