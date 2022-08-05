#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>
#include "Shader.h"

struct Vertex
{
	glm::vec3 Position;		//顶点坐标
	glm::vec3 Normal;		//法线方向
	glm::vec3 Tangent;		//切线方向
	glm::vec3 Bitangent;	//副切线方向
	glm::vec2 TexCoords;	//纹理坐标
};

struct Texture
{
	unsigned int id;	//贴图Id
	std::string type;	//贴图类型（漫反射贴图或者是镜面光贴图）
	std::string path;	//贴图路径
};

#pragma once
class Mesh
{
public:
	#pragma region 网格数据
	//顶点数据
	std::vector<Vertex> vertices;
	//顶点索引数据
	std::vector<unsigned int> indices;
	//贴图数据
	std::vector<Texture> textures;
	#pragma endregion
	//构造
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	//DrawCall
	void Draw(Shader shader);
private:
	#pragma region 渲染数据
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
	#pragma endregion
	//设置Mesh
	void setupMesh();
};

