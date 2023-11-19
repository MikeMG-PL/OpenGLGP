#pragma once
#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Transform.h"
#include "Engine/Component.h"
#include "Engine/Texture.h"
#include "Engine/Mesh.h"
#include "Engine/Shader.h"

class Model : public Component
{
public:
	explicit Model(const std::string& path);
    void Start() override;
    void Draw(Shader shader);
    glm::mat4 GetModelMatrix() const;
    //void SetModelMatrix(const glm::mat4& mat);
    void UpdateModelMatrix();

private:

    std::shared_ptr<Transform> transform;
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    std::vector<Texture> texturesLoaded;
    std::vector<Mesh> meshes;
    std::string directory;
    
    void loadModel(const std::string& path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
        std::string typeName);

};
