#include "ShaderEffect.h"
#include <glad/gl.h>
#include <fstream>
#include <sstream>
#include <string>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
GLuint LoadShader(const std::string& filename, GLenum shader_type) {
    // Open the file
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "File not found.\n";
        return 0;
    }

    // Read the contents of the file into a string
    std::stringstream stream;
    stream << file.rdbuf();
    std::string shader_code = stream.str();

    // Close the file
    file.close();

    // Create the shader object
    GLuint shader = glCreateShader(shader_type);

    // Compile the shader
    const char* shader_code_ptr = shader_code.c_str();
    glShaderSource(shader, 1, &shader_code_ptr, NULL);
    glCompileShader(shader);

    // Check for errors
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar info_log[512];
        glGetShaderInfoLog(shader, 512, NULL, info_log);
        std::cout << "Error compiling shader: " << info_log << std::endl;
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

ShaderEffect::ShaderEffect(std::string vert, std::string frag)
{

    vertexShader = LoadShader(vert, GL_VERTEX_SHADER);
    fragmentShader = LoadShader(frag, GL_FRAGMENT_SHADER);

    programHandle = glCreateProgram();
    glAttachShader(programHandle, vertexShader);
    glAttachShader(programHandle, fragmentShader);
    glLinkProgram(programHandle);
    int suc = 0;
    glGetProgramiv(programHandle, GL_LINK_STATUS, &suc);
    if (suc == 0) {
        std::cout << "Unable to link shaders.\n";
    }
    else {
        std::cout << "Linked shaders.\n";
    }
}


void ShaderEffect::SetMatrix(std::string name, glm::mat4 mat) {

    int loc = glGetUniformLocation(programHandle, name.c_str());
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));

}

void ShaderEffect::SetVec4(std::string name, glm::vec4 val) {
    int loc = glGetUniformLocation(programHandle, name.c_str());
    glUniform4f(loc, val.x, val.y, val.z, val.w);
}

void ShaderEffect::SetVec3(std::string name, glm::vec3 val) {

    int loc = glGetUniformLocation(programHandle, name.c_str());
    glUniform3f(loc, val.x, val.y, val.z);

}

void ShaderEffect::SetInt(std::string name, int val) {
    int loc = glGetUniformLocation(programHandle, name.c_str());
    glUniform1i(loc, val);
}

void ShaderEffect::SetFloat(std::string name, float val)
{
    int loc = glGetUniformLocation(programHandle, name.c_str());
    glUniform1f(loc, val);

}

void ShaderEffect::Bind() {
    GLint validateStatus;
    glGetProgramiv(programHandle, GL_VALIDATE_STATUS, &validateStatus);
    if (validateStatus == GL_FALSE) {
        std::cout << "Shader program is not valid" << std::endl;
    }
    else {
        
    }

    glUseProgram(programHandle);

}

void ShaderEffect::Release() {

    
        glUseProgram(0);
    
}