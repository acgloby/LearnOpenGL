// assimp模型加载库
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
#include <iostream>

#include "Shader.h"
#include "Mesh.h"


#pragma once
class Model
{
public:
	Model(const char* path);
	void Draw(Shader shader);

private:
	// 模型数据
	std::vector<Mesh> meshes;
	//模型资源所在文件夹路径
	std::string directory;
	//已加载的贴图
	std::vector<Texture> textures_loaded;
	//加载模型
	void loadModel(std::string path);
	//处理节点数据
	void processNode(aiNode* node, const aiScene* scene);
	//处理Mesh数据
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	//加载材质贴图
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	//从文件加载贴图
	unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);
};

