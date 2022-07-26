#pragma once
#include <string>
#include <GL/glew.h>

class Shader
{
public:
	unsigned int ID;
	std::string vertexString;
	std::string fragmentString;
	const char* vertexSource;
	const char* fragmentSource;
	Shader(const char* vertexPath, const char* fragmentPath);
	void use();
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setMatrix4fv(const std::string& name, const GLfloat* value) const;

private:
	void checkCompileErrors(unsigned int ID, std::string type);
};

  