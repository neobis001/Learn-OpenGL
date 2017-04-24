#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>


using namespace std;


void CheckForGLError()
{
	for (GLenum error; (error = glGetError()) != GL_NO_ERROR;)
	{
		cout << "OpenGL Error:  \t";
		if (error == GL_INVALID_ENUM)
			cout << "GL_INVALID_ENUM";
		if (error == GL_INVALID_VALUE)
			cout << "GL_INVALID_VALUE";
		if (error == GL_INVALID_OPERATION)
			cout << "GL_INVALID_OPERATION";
		if (error == GL_STACK_OVERFLOW)
			cout << "GL_STACK_OVERFLOW";
		if (error == GL_STACK_UNDERFLOW)
			cout << "GL_STACK_UNDERFLOW";
		if (error == GL_OUT_OF_MEMORY)
			cout << "GL_OUT_OF_MEMORY";
		if (error == GL_INVALID_FRAMEBUFFER_OPERATION)
			cout << "GL_INVALID_FRAMEBUFFER_OPERATION";
		if (error == GL_CONTEXT_LOST)
			cout << "GL_CONTEXT_LOST";
		cout << (char)7 << endl;        /*play sound*/
		//cin.get();
	}
}


void glfw_error_callback(int error, const char* description)
{
	cout << "GLFW ERROR: \t" << description << endl;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

bool test = false;
void Render()
{
	/* clear framebuffer */
	glClearColor(0, 1, 0, 0);
	glClearDepth(1);
	glClearStencil(0);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	/* render here */

	/* check for GL errors */
	CheckForGLError();
}


int main()
{
	// Initialize GLFW
	if (!glfwInit())
		return -1;

	bool fullscreen = false;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_STENCIL_BITS, 8);

	GLFWmonitor* monitor = NULL;
	if (fullscreen)
		monitor = glfwGetPrimaryMonitor();
	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", monitor, nullptr);
	if (!window)
	{
		glfwTerminate();
		return 1;
	}

	glfwMakeContextCurrent(window);
	glfwSetErrorCallback(glfw_error_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	// Initialize GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		glfwTerminate();
		return 2;
	}

	// Setup viewport
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	/**
	If the specified framebuffer is a framebuffer object, attachment must be one of
	GL_DEPTH_ATTACHMENT,
	GL_STENCIL_ATTACHMENT,
	GL_DEPTH_STENCIL_ATTACHMENT,
	or GL_COLOR_ATTACHMENTi, where i is between zero and the value of GL_MAX_COLOR_ATTACHMENTS minus one.

	If the specified framebuffer is a default framebuffer, target, attachment must be one of
	GL_FRONT_LEFT,
	GL_FRONT_RIGHT,
	GL_BACK_LEFT,
	GL_BACK_RIGHT,
	GL_DEPTH
	or GL_STENCIL, identifying the corresponding buffer.
	**/


	GLint depth_bits = 0, stencil_bits = 0;
	glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_DEPTH, GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE, &depth_bits);
	
	GLenum error = glGetError();
	std::cout << (error == GL_INVALID_ENUM) << std::endl;
	glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_STENCIL, GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE, &stencil_bits);

	cout << "depth bits: " << depth_bits << endl;
	cout << "stencil bits: " << stencil_bits << endl;


	// window loop
	while (!glfwWindowShouldClose(window)) {
		Render();
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	return 0;
}