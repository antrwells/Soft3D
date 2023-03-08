#pragma once
#include <string>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include "SoftTypes.h"
#include <stack>
#include "AppState.h"
class pixelMap;
class depthBuffer;
class AudioSystem;



class SoftApp
{
public:

	SoftApp(int width, int height, std::string app);
	void InitRenderer(int w, int h);
	virtual void Init() {};
	virtual void Update(float et) {};
	virtual void Render() {};
	virtual void RenderAfter3D() {};
	void Run();
	

	pixelMap* getBackBuffer();
	depthBuffer* getDepthBuffer();

	static SoftApp* m_This;

	int getWidth() {
		return m_Width;
	}
	int getHeight() {
		return m_Height;
	}

	int getWinWidth() {
		return win_Width;
	}
	int getWinHeight() {
		return win_Height;
	}

	void PushState(AppState* state)
	{

		m_States.push(state);
		state->Init();

	}
	 void PopState() {

		 m_States.pop();

	}

private:

	GLFWwindow* m_Window;
	int win_Width;
	int win_Height;
	std::stack<AppState*> m_States;
	pixelMap* m_ColorBuffer;
	depthBuffer* m_DepthBuffer;
	pixelMap* m_StencilBuffer;

	color m_BackColor;
	int m_Width, m_Height;
	AudioSystem* m_Audio;

};

