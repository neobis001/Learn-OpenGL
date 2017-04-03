#include <iostream>
#include <cmath>



// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>


// Other Libs
#include <SOIL.h>
// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other includes
#include "Shader.h"
#include "Camera.h"


// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void do_movement();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool    keys[1024];

// Light attributes
//glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
//glm::vec3 lightPos(0.0f, 0.0f, 3.0f);

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

							// The MAIN function, from here we start the application and run the game loop
int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	glewInit();

	// Define the viewport dimensions
	glViewport(0, 0, WIDTH, HEIGHT);

	// OpenGL options
	glEnable(GL_DEPTH_TEST);

	// Build and compile our shader program
	Shader lightingShader("map.vs", "multi_lights.frag");
	Shader lampShader("lamp.vs", "lamp.frag");

	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] = {
		// Positions          // Normals           // Texture Coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f),
	};

	// First, set the container's VAO (and VBO)
	GLuint VBO, containerVAO;
	glGenVertexArrays(1, &containerVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(containerVAO);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);



	// Then, we set the light's VAO (VBO stays the same. After all, the vertices are the same for the light object (also a 3D cube))
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Set the vertex attributes (only position data for the lamp))
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);


	// Load textures
	GLuint diffuseMap;
	glGenTextures(1, &diffuseMap);
	int width, height;
	unsigned char* image;
	// Diffuse map
	image = SOIL_load_image("container2.png", &width, &height, 0, SOIL_LOAD_RGB);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint specMap;
	glGenTextures(1, &specMap);
	// Diffuse map
	image = SOIL_load_image("container2_specular.png", &width, &height, 0, SOIL_LOAD_RGB);
	glBindTexture(GL_TEXTURE_2D, specMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);


	lightingShader.Use();
	GLint matDiffuseLoc = glGetUniformLocation(lightingShader.Program, "diffuse");
	glUniform1i(matDiffuseLoc, 0);
	GLint matSpecLoc = glGetUniformLocation(lightingShader.Program, "specular");
	glUniform1i(matSpecLoc, 1);

	bool test = true;
	int testInput;
	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		do_movement();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.Position.x, camera.Position.y, camera.Position.z);

		//in the same lighting shader, get and set the material struct
		GLint matShineLoc = glGetUniformLocation(lightingShader.Program, "material.shininess");
		glUniform1f(matShineLoc, 64.0f);

		//activate textures
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specMap);


		//again, in the same lighting shader, get and set the light struct
		GLint lightAmbientLoc = glGetUniformLocation(lightingShader.Program, "dirLight.ambient");
		GLint lightDiffuseLoc = glGetUniformLocation(lightingShader.Program, "dirLight.diffuse");
		GLint lightSpecularLoc = glGetUniformLocation(lightingShader.Program, "dirLight.specular");
		//GLint lightPosLoc = glGetUniformLocation(lightingShader.Program, "light.position");
		GLint lightDirLoc = glGetUniformLocation(lightingShader.Program, "dirLight.direction");
		glUniform3f(lightAmbientLoc, 0.2f, 0.2f, 0.2f);
		glUniform3f(lightDiffuseLoc, 0.5f, 0.5f, 0.5f);
		glUniform3f(lightSpecularLoc, 1.0f, 1.0f, 1.0f);
		//glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(lightDirLoc, -0.2f, -1.0f, -0.3f);

		/*
		for (int i = 0; i < 4; i++) {
			std::string number = std::to_string(i);
			glm::vec3 currentLightPos{pointLightPositions[i]};

			//again, in the same lighting shader, get and set the light struct
			GLint lightAmbientLoc = glGetUniformLocation(lightingShader.Program, ("pointLights[" + number + "].ambient").c_str());
			GLint lightDiffuseLoc = glGetUniformLocation(lightingShader.Program, ("pointLights[" + number + "].diffuse").c_str());
			GLint lightSpecularLoc = glGetUniformLocation(lightingShader.Program, ("pointLights[" + number + "].specular").c_str());
			GLint lightPosLoc = glGetUniformLocation(lightingShader.Program, ("pointLights[" + number + "].position").c_str());
			//GLint lightDirLoc = glGetUniformLocation(lightingShader.Program, "light.direction");
			glUniform3f(lightAmbientLoc, 0.2f, 0.2f, 0.2f);
			glUniform3f(lightDiffuseLoc, 0.5f, 0.5f, 0.5f);
			glUniform3f(lightSpecularLoc, 1.0f, 1.0f, 1.0f);
			glUniform3f(lightPosLoc, currentLightPos.x, currentLightPos.y, currentLightPos.z);
			//glUniform3f(lightDirLoc, -0.2f, -1.0f, -0.3f);

			//in the same lighting shader, set attenuation values
			GLint lightConstantLoc = glGetUniformLocation(lightingShader.Program, ("pointLights[" + number + "].constant").c_str());
			GLint lightLinearLoc = glGetUniformLocation(lightingShader.Program, ("pointLights[" + number + "].linear").c_str());
			GLint lightQuadraticLoc = glGetUniformLocation(lightingShader.Program, ("pointLights[" + number + "].quadratic").c_str());
			glUniform1f(lightConstantLoc, 1.0f);
			glUniform1f(lightLinearLoc, 0.09f);
			glUniform1f(lightQuadraticLoc, 0.032f);

		}
		*/

		
		std::string number = std::to_string(0);
		glm::vec3 currentLightPos{ pointLightPositions[0] };

		//again, in the same lighting shader, get and set the light struct
		lightDiffuseLoc = glGetUniformLocation(lightingShader.Program, "testLight.diffuse");
		lightSpecularLoc = glGetUniformLocation(lightingShader.Program, "testLight.specular");
		GLint lightPosLoc = glGetUniformLocation(lightingShader.Program, "testLight.position");
		//GLint lightDirLoc = glGetUniformLocation(lightingShader.Program, "light.direction");
		glUniform3f(lightAmbientLoc, 0.2f, 0.2f, 0.2f);
		glUniform3f(lightDiffuseLoc, 0.5f, 0.5f, 0.5f);
		glUniform3f(lightSpecularLoc, 1.0f, 1.0f, 1.0f);
		glUniform3f(lightPosLoc, currentLightPos.x, currentLightPos.y, currentLightPos.z);
		//glUniform3f(lightDirLoc, -0.2f, -1.0f, -0.3f);

		GLint lightConstantLoc = glGetUniformLocation(lightingShader.Program, "testLight.constant");
		GLint lightLinearLoc = glGetUniformLocation(lightingShader.Program, "testLight.linear");
		GLint lightQuadraticLoc = glGetUniformLocation(lightingShader.Program, "testLight.quadratic");
		glUniform1f(lightConstantLoc, 1.0f);
		glUniform1f(lightLinearLoc, 0.09f);
		glUniform1f(lightQuadraticLoc, 0.032f);

		/*
		GLint testLoc = glGetUniformLocation(lightingShader.Program, "pointLights[3].position");

		if (test) {
			std::cout << testLoc << std::endl;
			test = false;
		}*/

		/*
		lightDiffuseLoc = glGetUniformLocation(lightingShader.Program, ("pointLights[" + number + "].diffuse").c_str());
		lightSpecularLoc = glGetUniformLocation(lightingShader.Program, ("pointLights[" + number + "].specular").c_str());
		GLint lightPosLoc = glGetUniformLocation(lightingShader.Program, ("pointLights[" + number + "].position").c_str());
		//GLint lightDirLoc = glGetUniformLocation(lightingShader.Program, "light.direction");
		glUniform3f(lightAmbientLoc, 0.2f, 0.2f, 0.2f);
		glUniform3f(lightDiffuseLoc, 0.5f, 0.5f, 0.5f);
		glUniform3f(lightSpecularLoc, 1.0f, 1.0f, 1.0f);
		glUniform3f(lightPosLoc, currentLightPos.x, currentLightPos.y, currentLightPos.z);
		//glUniform3f(lightDirLoc, -0.2f, -1.0f, -0.3f);

		//in the same lighting shader, set attenuation values
		GLint lightConstantLoc = glGetUniformLocation(lightingShader.Program, ("pointLights[" + number + "].constant").c_str());
		GLint lightLinearLoc = glGetUniformLocation(lightingShader.Program, ("pointLights[" + number + "].linear").c_str());
		GLint lightQuadraticLoc = glGetUniformLocation(lightingShader.Program, ("pointLights[" + number + "].quadratic").c_str());
		glUniform1f(lightConstantLoc, 1.0f);
		glUniform1f(lightLinearLoc, 0.09f);
		glUniform1f(lightQuadraticLoc, 0.032f);
		*/

		/*
		//again, in the same lighting shader, get and set the light struct
		GLint lightAmbientLoc = glGetUniformLocation(lightingShader.Program, "light.ambient");
		GLint lightDiffuseLoc = glGetUniformLocation(lightingShader.Program, "light.diffuse");
		GLint lightSpecularLoc = glGetUniformLocation(lightingShader.Program, "light.specular");
		//GLint lightDirLoc = glGetUniformLocation(lightingShader.Program, "light.direction");
		glUniform3f(lightAmbientLoc, 0.2f, 0.2f, 0.2f);
		glUniform3f(lightDiffuseLoc, 0.5f, 0.5f, 0.5f);
		glUniform3f(lightSpecularLoc, 1.0f, 1.0f, 1.0f);
		//glUniform3f(lightDirLoc, -0.2f, -1.0f, -0.3f);

		//in the same lighting shader, set stuff based on camera
		GLint lightPosLoc = glGetUniformLocation(lightingShader.Program, "light.position");
		GLint lightSpotdirLoc = glGetUniformLocation(lightingShader.Program, "light.direction");
		GLint lightInnerCutOffLoc = glGetUniformLocation(lightingShader.Program, "light.innerCutoff");
		GLint lightOuterCutOffLoc = glGetUniformLocation(lightingShader.Program, "light.outerCutoff");
		glUniform3f(lightPosLoc, camera.Position.x, camera.Position.y, camera.Position.z);
		glUniform3f(lightSpotdirLoc, camera.Front.x, camera.Front.y, camera.Front.z);
		glUniform1f(lightInnerCutOffLoc, glm::cos(glm::radians(12.5f)));
		glUniform1f(lightOuterCutOffLoc, glm::cos(glm::radians(17.5f)));
		

		//in the same lighting shader, set attenuation values
		GLint lightConstantLoc = glGetUniformLocation(lightingShader.Program, "light.constant");
		GLint lightLinearLoc = glGetUniformLocation(lightingShader.Program, "light.linear");
		GLint lightQuadraticLoc = glGetUniformLocation(lightingShader.Program, "light.quadratic");
		glUniform1f(lightConstantLoc, 1.0f);
		glUniform1f(lightLinearLoc, 0.09);
		glUniform1f(lightQuadraticLoc, 0.032);
		*/

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");
		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// Draw the container (using container's vertex attributes)
		glm::mat4 model;
		glBindVertexArray(containerVAO);
		for (GLuint i = 0; i < 10; i++) {
			model = glm::mat4();
			model = glm::translate(model, cubePositions[i]);
			GLfloat angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);

		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");
		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		
		glBindVertexArray(lightVAO);
		for (int i = 0; i < 4; i++) {
			model = glm::mat4();
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			
			// Draw the light object (using light's vertex attributes)
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);


		

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void do_movement()
{
	// Camera controls
	if (keys[GLFW_KEY_W])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}