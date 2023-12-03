#include "Animation system/RiggedModel.h"
#include "Animation system/Rig.h"
#include <iostream>

#include "Helpers/aiMatrixToGLM.h"
#include "Helpers/TextureLoader.h"

RiggedModel::RiggedModel(const std::string& path)
{
	loadModel(path);
    // loadPose(path);
}

void RiggedModel::Draw(Shader shader)
{
    for (unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].Draw(shader);
}

void RiggedModel::loadModel(const std::string& path)
{
    Assimp::Importer import;
    scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));

    // Extract rig data from the scene
    extractBoneData(scene->mRootNode);

    // Process meshes and use the extracted bone data
    processNode(scene->mRootNode);
}

void RiggedModel::processNode(aiNode* node)
{
    // Process each mesh node if exists
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(mesh);
    }

    // Same for each child
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i]);
    }
}

Mesh RiggedModel::processMesh(aiMesh* mesh)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        // Process vertex positions, normals, texture coordinates
        Vertex vertex;
        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;

        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.normal = vector;

        if (mesh->mTextureCoords[0]) // Check if the mesh contains any texture coordinates
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.texCoords = vec;
        }
        else
            vertex.texCoords = glm::vec2(0.0f, 0.0f);

        // Initialize bone weights and indices
        vertex.skinWeights = glm::vec4(0.0f);
        vertex.skinIndices = glm::ivec4(-1);

        // Assign bone weights and indices
        for (int j = 0; j < mesh->mBones[i]->mNumWeights; j++)
        {
            unsigned int boneID = mesh->mBones[i]->mWeights[j].mVertexId;
            float weight = mesh->mBones[i]->mWeights[j].mWeight;

            if (j < 4)
            {
                vertex.skinIndices[j] = boneID;
                vertex.skinWeights[j] = weight;
            }
        }

        vertices.push_back(vertex);
    }

    // Process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    // Process materials
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material,
            aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<Texture> specularMaps = loadMaterialTextures(material,
            aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return Mesh(vertices, indices, textures);
}

std::vector<Texture> RiggedModel::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for (unsigned int j = 0; j < texturesLoaded.size(); j++)
        {
            if (std::strcmp(texturesLoaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(texturesLoaded[j]);
                skip = true;
                break;
            }
        }
        if (!skip)
        {   // If texture hasn't been loaded yet, add it
            Texture texture;
            texture.ID = textureFromFile(str.C_Str(), directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            texturesLoaded.push_back(texture); // Add to texturesLoaded vector
        }
    }
    return textures;
}

///////////////////////////////////////////////////////

void RiggedModel::extractBoneData(aiNode* node)
{
    // Process each mesh node if it contains bones
    if (node->mNumMeshes > 0)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[0]]; // Assuming one mesh per node for simplicity
        extractBoneDataFromMesh(mesh);
    }

    // Recursively process child nodes
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        extractBoneData(node->mChildren[i]);
    }
}

void RiggedModel::extractBoneDataFromMesh(aiMesh* mesh)
{
    // Extract bone data from the mesh
    for (unsigned int i = 0; i < mesh->mNumBones; i++)
    {
        aiBone* bone = mesh->mBones[i];

        // Extract bone name, parent index, and bind pose
        hstring boneName = hash(bone->mName.C_Str());
        
        // Find parent bone in the mBones array
        int parentIndex = -1;

        for (unsigned int j = 0; j < mesh->mNumBones; j++)
        {
            if (i != j && strcmp(mesh->mBones[j]->mName.C_Str(), bone->mName.C_Str()) == 0)
            {
                // Found the parent bone
                parentIndex = j;
                break;
            }
        }

        const aiMatrix4x4 aiBindPose = bone->mOffsetMatrix;
        glm::mat4 bindPose = aiMatrix4x4ToGlm(&aiBindPose);

        // Add the extracted data to the Rig
        rig.boneNames.push_back(boneName);
        rig.parents.push_back(parentIndex);
        rig.bindPose.push_back(bindPose);
        rig.numBones = mesh->mNumBones;
    }
}
