#pragma once
#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Engine/Component.h"

struct Texture;
class Mesh;
class Shader;

class Model : public Component
{
public:

    Model(char* path);
    void Draw(Shader shader);

private:

    std::vector<Texture> texturesLoaded;
    std::vector<Mesh> meshes;
    std::string directory;
    
    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
        std::string typeName);

};
