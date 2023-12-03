#pragma once
#include "Rig.h"
#include "Components/Model.h"

class RiggedModel : Component
{
public:

	RiggedModel(const std::string& path);
	void Draw(Shader shader);
	Rig rig;

	// Not a T-pose, it's desired pose you want to load from an additional file
	std::vector<glm::mat4> modelPose;
	std::vector<glm::mat4> localPose;

private:

	std::vector<Texture> texturesLoaded;
	std::vector<Mesh> meshes;
	std::string directory;

	const aiScene* scene;

	// Assimp
	void loadModel(const std::string& path);
	// void loadAnimation(const std::string& path); ... this will load pose for now
	void processNode(aiNode* node);
	Mesh processMesh(aiMesh* mesh);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
		std::string typeName);

	// Skinning
	void extractBoneData(aiNode* node);
	void extractBoneDataFromMesh(aiMesh* mesh);
};
