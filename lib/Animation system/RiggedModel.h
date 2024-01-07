#pragma once
#include "AnimXForm.h"
#include "Rig.h"
#include "Components/Model.h"

enum LoadMode
{
	RIG,
	ANIM
};

class RiggedModel : public Component
{
public:

	RiggedModel(const std::string& modelPath, const std::string& animPath);
	void Draw(Shader shader);
	Rig rig;

	std::vector<xform> modelPose;
	std::vector<xform> localPose;

private:

	std::vector<Texture> texturesLoaded;
	std::vector<Mesh> meshes;
	std::string directory;

	const aiScene* scene;

	// Assimp
	void loadModel(const std::string& path, LoadMode mode);
	void processNode(aiNode* node);
	Mesh processMesh(aiMesh* mesh);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
		std::string typeName);

	// Skinning
	void extractBoneData(aiNode* node, LoadMode mode);
	void extractBoneDataFromMesh(aiMesh* mesh, LoadMode mode);
};
