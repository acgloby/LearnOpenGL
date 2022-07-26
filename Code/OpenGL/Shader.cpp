#include "Shader.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>


unsigned int ID;

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	std::string vertexString;
	std::string fragmentString;
	std::ifstream  vertexFile;
	std::ifstream fragmentFile;
	// 保证ifstream对象可以抛出异常
	vertexFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
	fragmentFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		//打开文件
		vertexFile.open(vertexPath);
		fragmentFile.open(fragmentPath);
		if (!vertexFile.is_open() || !fragmentFile.is_open())
		{
			throw std::exception("open file error");
		}
		std::stringstream vertexShaderStream, fragmentShaderStream;
		//读取文件的缓冲内容到数据流中
		vertexShaderStream << vertexFile.rdbuf();
		fragmentShaderStream << fragmentFile.rdbuf();

		vertexFile.close();
		fragmentFile.close();
		//数据流转为string
		vertexString = vertexShaderStream.str();
		fragmentString = fragmentShaderStream.str();

	}
	catch (const std::exception& e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
	}

	const char* vertexSource = vertexString.c_str();
	const char* fragmentSource = fragmentString.c_str();

	unsigned int vertex, fragment;

	//顶点着色器
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexSource, NULL);
	glCompileShader(vertex);
	// 打印编译错误
	checkCompileErrors(vertex, "VERTEX");
	
	//片元着色器
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragmentSource, NULL);
	glCompileShader(fragment);
	// 打印编译错误
	checkCompileErrors(fragment, "FRAGMENT");

	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	// 打印编译错误
	checkCompileErrors(ID, "PROGRAM");

	// 删除着色器，它们已经链接到我们的程序中了，已经不再需要了
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMatrix4fv(const std::string& name, const GLfloat* value) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, value);
}

/// <summary>
/// 检测编辑错误
/// </summary>
/// <param name="ID"></param>
/// <param name="type"></param>
void Shader::checkCompileErrors(unsigned int ID, std::string type) 
{
	int success;
	char infoLog[512];
	
	if (type != "PROGRAM")
	{
		glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(ID, 512, NULL, infoLog);
			std::cout << "shader compile error：" << infoLog << std::endl;
		};
	}
	else 
	{
		glGetProgramiv(ID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(ID, 512, NULL, infoLog);
			std::cout << "program linking error：" << infoLog << std::endl;
		};
	}
	
}
