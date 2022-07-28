#pragma once
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
	void setVec3(const std::string& name, const glm::vec3& value) const;
	void setVec3(const std::string& name, float x, float y, float z) const;

private:
	void checkCompileErrors(unsigned int ID, std::string type);
};

  