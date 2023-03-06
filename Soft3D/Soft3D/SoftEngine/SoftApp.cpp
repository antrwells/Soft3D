#include "SoftApp.h"

SoftApp::SoftApp(int width, int height, std::string app) {

	if (!glfwInit())
	{
		// Initialization failed
		printf("Failed to create opengl context.\n");
	}
	else {
		printf("Created opengl context successfully.\n");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	GLFWwindow* window = glfwCreateWindow(width, height,app.c_str(), NULL, NULL);
	m_Window = window;
	if (!window)
	{
		// Window or context creation failed
		printf("Failed to create glfw window.\n");
	}
	else {
		printf("Successfully created glfw window.\n");
	}
	glfwMakeContextCurrent(window);
	gladLoadGL(glfwGetProcAddress);
}

void SoftApp::Run() {

	Init();

	while (true) {

		Update();
			
		glClearColor(0.3, 0, 0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Render();
		glfwSwapBuffers(m_Window);

		glfwPollEvents();
	}

}