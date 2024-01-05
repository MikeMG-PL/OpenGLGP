#include "Animation system/RiggedModel.h"
#include "Animation system/Rig.h"
#include <iostream>
#include <map>

#include "Helpers/aiHelpers.h"
#include "Helpers/TextureLoader.h"

RiggedModel::RiggedModel(const std::string& modelPath, const std::string& animPath)
{
	loadModel(modelPath, RIG);
	loadModel(animPath, ANIM);
}

void RiggedModel::Draw(Shader shader)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		//auto r = rig;
		// shader.setMat4Array("skin.bones", rig.skinnedPose);        // TOGGLE THIS, skin?
		meshes[i].Draw(shader);
	}
}

void RiggedModel::loadModel(const std::string& path, LoadMode mode)
{
	Assimp::Importer import;
	scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if ((!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) && mode == RIG)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << "::LOADMODE::" << mode << std::endl;
		return;
	}
	if (mode == ANIM)
	{
		if (!scene || !scene->HasAnimations())
		{
			std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << "::LOADMODE::" << mode << std::endl;
			return;
		}
	}

	directory = path.substr(0, path.find_last_of('/'));

	if (mode == RIG)
	{
		// Extract rig data from the scene
		extractBoneData(scene->mRootNode, mode);

		// Process meshes and use the extracted bone data
		processNode(scene->mRootNode);
	}
	else // If we are loading animation, not rig
	{
		// Just a pose for now
		// TODO: Cut out to separate function
		// TODO: Attach sampler of course lol
	
		std::cout << rig.numBones << std::endl;
	
		for (int i = 0; i < rig.numBones; i++)
		{
			xform transform;
	
			const aiAnimation* animation = scene->mAnimations[0]; // Get the first animation
			const aiNodeAnim* channel = animation->mChannels[i]; // Get the first channel
	
			// Get the position, rotation, and scaling keyframes from the first keyframe
			const aiVector3D position = channel->mPositionKeys[0].mValue;
			const aiQuaternion rotation = channel->mRotationKeys[0].mValue;
	
			transform.position = aiPosToGLMVec3(position);
			transform.rotation = aiQuatToGLMQuat(rotation);
	
			localPose.emplace_back(transform);
		}
	}
}

void RiggedModel::processNode(aiNode* node)
{
	// Process each mesh node if exists
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh));
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

	std::map<int, glm::ivec4> vertexToBoneIndices;
	std::map<int, glm::vec4> vertexToBoneWeights;
	Vertex vertex;
	int vertexID = 0;
	glm::ivec4 skinIndices = { -1, -1, -1, -1 };
	glm::vec4 skinWeights = { 0, 0, 0, 0 };

	for (int i = 0; i < mesh->mNumBones; i++)
	{
		for (int j = 0; j < mesh->mBones[i]->mNumWeights; j++)
		{
			vertexID = mesh->mBones[i]->mWeights[j].mVertexId;
			int boneID = i; // Bone ID is the index of the bone in the array
	
			// Initialize the ivec4 with -1s and vec4 with 0s if it doesn't exist in the map
			vertexToBoneIndices.try_emplace(vertexID, skinIndices);
			vertexToBoneWeights.try_emplace(vertexID, skinWeights);
	
			// Get the current skinIndices for the vertex
			skinIndices = vertexToBoneIndices[vertexID];
			skinWeights = vertexToBoneWeights[vertexID];
	
			// Find the first available slot in the ivec4
			int slot = -1;
			for (int k = 0; k < 4; k++)
			{
				if (skinIndices[k] == -1)
				{
					slot = k;
					break;
				}
			}
	
			// If there is an available slot, assign the bone ID to it
			if (slot != -1)
			{
				skinIndices[slot] = boneID;
				float boneWeight = mesh->mBones[boneID]->mWeights[vertexID].mWeight;
				skinWeights[slot] = boneWeight;
				vertexToBoneIndices[vertexID] = skinIndices;
			}
		}
	}


	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		// Process vertex positions, normals, texture coordinates
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

		vertex.skinIndices = vertexToBoneIndices[i];
		vertex.skinWeights = vertexToBoneWeights[i];

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

void RiggedModel::extractBoneData(aiNode* node, LoadMode mode)
{
	// Process each mesh node if it contains bones
	if (node->mNumMeshes > 0)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[0]]; // Assuming one mesh per node for simplicity
		extractBoneDataFromMesh(mesh, mode);
	}
	
	// Recursively process child nodes
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		extractBoneData(node->mChildren[i], mode);
	}
}

void RiggedModel::extractBoneDataFromMesh(aiMesh* mesh, LoadMode mode)
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
	
		const aiMatrix4x4 aiInverseBindPose = bone->mOffsetMatrix;
		glm::mat4 inverseBindPose = aiMatrix4x4ToGlm(&aiInverseBindPose);
	
		// Add the extracted data to the Rig
		rig.boneNames.push_back(boneName);
		rig.parents.push_back(parentIndex);
		rig.inverseBindPose.push_back(inverseBindPose);
	}
	rig.numBones += mesh->mNumBones;
	rig.skinnedPose.resize(rig.numBones);
}
