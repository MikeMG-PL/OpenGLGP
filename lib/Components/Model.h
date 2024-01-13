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

    Model(const std::string& path, bool instanced = false, int numInstances = -1, glm::mat4* matrices = nullptr);
    Model(const std::string& path, glm::vec2 tiling = {1, 1}, bool wireFrame = false);
	// Model(float r = 1);
	// Model(float sphereRadius, int sectors, int stacks, const std::string& texturePath, GLenum drawType);
    void Draw(Shader shader);
    bool IsInstanced() const;
    bool IsReflective() const;
    void SetReflective(bool r);
    std::vector<Mesh>& GetMeshes();
    float dimValue = 0.7f;

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

    glm::vec2 tiling = {1, 1};

    // Reflections
    bool reflective = false;

    // Instancing
    bool instanced;
    glm::mat4* matrices;

    // Generating orbit
    void addOrbit(float r);

    // Generating sphere
    void createSphere();
    Texture loadTextureForSphere();

    // Used only with instancing
    int numInstances;

    GLenum drawType = GL_TRIANGLES;
    bool wireFrame;
    int sectors;
    int stacks;
    std::string texturePath;
    float sphereRadius;
};
