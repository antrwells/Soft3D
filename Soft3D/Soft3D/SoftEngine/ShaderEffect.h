#pragma once
#include <string>
#include <glm/glm.hpp>
class ShaderEffect
{
public:
	ShaderEffect(std::string vertexPath, std::string fragmentPath);
	void Release();
	void Bind();
	void SetMatrix(std::string name, glm::mat4 matrix);
	void SetVec4(std::string name, glm::vec4 val);
	void SetVec3(std::string name, glm::vec3 val);
	void SetInt(std::string name, int val);
	void SetFloat(std::string name, float val);

private:
	int vertexShader;
	int fragmentShader;
	int programHandle;
};




