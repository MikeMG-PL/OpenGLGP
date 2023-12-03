#pragma once
#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include "assimp/postprocess.h"

#include "Engine/Component.h"
#include "Engine/Texture.h"
#include "Engine/Mesh.h"
#include "Engine/Shader.h"

class Model : public Component
{
public:

	Model(const std::string& path, bool drawOrbit = false, float r = 0);
	Model(float r = 1);
    Model(float sphereRadius, int sectors, int stacks, const std::string& texturePath, GLenum drawType);
    void Draw(Shader shader);

private:

    std::vector<Texture> texturesLoaded;
    std::vector<Mesh> meshes;
    std::string directory;

    // Assimp
	void loadModel(const std::string& path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
        std::string typeName);

    // Generating orbit
    void addOrbit(float r);

    // Generating sphere
    void createSphere();
    Texture loadTextureForSphere();

    GLenum drawType = GL_TRIANGLES;
    int sectors;
    int stacks;
    std::string texturePath;
    float sphereRadius;
};
