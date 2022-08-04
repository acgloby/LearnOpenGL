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
	std::string directory;

	//已加载的贴图
	std::vector<Texture> textures_loaded;
	
	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);
};

