#pragma once
#include <string>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>

class SoftApp
{
public:

	SoftApp(int width, int height, std::string app);
	virtual void Init() {};
	virtual void Update() {};
	virtual void Render() {};
	void Run();

private:

	GLFWwindow* m_Window;

};

