#include "Animation system/RiggedModel.h"
#include "Animation system/Rig.h"
#include <iostream>
#include <map>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Engine/GameInstance.h"
#include "Helpers/aiHelpers.h"
#include "Helpers/TextureLoader.h"

RiggedModel::RiggedModel(const std::string& modelPath, const std::string& animPath)
{
	loadModel(modelPath, RIG);
	loadModel(animPath, ANIM);
}

void RiggedModel::Draw(Shader shader)
{
	time += GameInstance::Get().GetDeltaTime();

	// Multiply by inv bind pose
	glm::mat4 skinningMatrices[512] = { glm::mat4(1) };

	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		rig.LocalToModel(modelPose, localPose);

		for (int j = 0; j < rig.numBones; j++)
		{
			xform inverseBindPoseXForm, skinnedXForm;
			glm::vec3 scale;
			glm::quat rotation;
			glm::vec3 translation;
			glm::vec3 skew;
			glm::vec4 perspective;
			glm::decompose(rig.inverseBindPose[j], scale, rotation, translation, skew, perspective);

			inverseBindPoseXForm.position = translation;
			inverseBindPoseXForm.rotation = rotation;

			skinnedXForm = modelPose[j] * inverseBindPoseXForm;

			glm::mat4 skinningMatrix = glm::mat4(1.0f);

			skinningMatrix = glm::translate(skinningMatrix, skinnedXForm.position);
			skinningMatrix = skinningMatrix * glm::toMat4(skinnedXForm.rotation);
			skinningMatrix = glm::scale(skinningMatrix, glm::vec3(1.0f));

			if(j == 4)
			{

				glm::mat4 rot = glm::mat4(1.0f);
				rot = glm::rotate(rot, glm::sin(time), glm::vec3(0, 1, 0));
				skinningMatrices[j] = glm::inverse(rig.inverseBindPose[j]) * rot * rig.inverseBindPose[j];
			}
			else
				skinningMatrices[j] = glm::inverse(rig.inverseBindPose[j]) * rig.inverseBindPose[j];
		}

		// I FINISHED HERE: Maybe I should somehow load poses of T-pose bones at the veeery beginning?

		shader.setSkinningMatrices(skinningMatrices);			// TOGGLE THIS, skin?
		meshes[i].Draw(shader);
	}
}

void RiggedModel::loadModel(const std::string& path, LoadMode mode)
{
	Assimp::Importer import;
	scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_PopulateArmatureData);

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
	if (mode == ANIM) // If we are loading animation, not rig
	{
		// Just a pose for now
		// TODO: Cut out to separate function
		// TODO: Attach sampler of course lol

		std::cout << rig.numBones << std::endl;

		for (int i = 0; i < rig.numBones; i++)
		{
			xform transform;

			// const aiAnimation* animation = scene->mAnimations[0]; // Get the first animation
			// const aiNodeAnim* channel = animation->mChannels[i]; // Get the first channel
			//
			// // Get the position, rotation, and scaling keyframes from the first keyframe
			// const aiVector3D position = channel->mPositionKeys[0].mValue;
			// const aiQuaternion rotation = channel->mRotationKeys[0].mValue;
			//
			// transform.position = aiPosToGLMVec3(position);
			// transform.rotation = aiQuatToGLMQuat(rotation);

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

	glm::ivec4 skinIndices = { -1, -1, -1, -1 };
	glm::vec4 skinWeights = { 0, 0, 0, 0 };

	std::map<unsigned int, glm::ivec4> vertexToBoneIndices;
	std::map<unsigned int, glm::vec4> vertexToBoneWeights;
	Vertex vertex;
	unsigned int vertexID = 0;
	unsigned int boneID = 0;
	float weight = 0.0f;

	// TODO: Optimize, reduce number of for loops used. Now I just want to make it work.

	for (int i = 0; i < mesh->mNumBones; i++)
	{
		for (int j = 0; j < mesh->mBones[i]->mNumWeights; j++)
		{
			vertexID = mesh->mBones[i]->mWeights[j].mVertexId;
			boneID = i;
			weight = mesh->mBones[i]->mWeights[j].mWeight;

			// Initialize the ivec4 with -1s and vec4 with 0s - default values

			if (!vertexToBoneIndices.contains(vertexID))
			{
				vertexToBoneIndices.try_emplace(vertexID, skinIndices);
				vertexToBoneWeights.try_emplace(vertexID, skinWeights);
			}

			// TODO: Get rid of this way of filling the ivec4.
			if (vertexToBoneIndices[vertexID].x == -1)
			{
				vertexToBoneIndices[vertexID].x = i;
				vertexToBoneWeights[vertexID].x = weight;
			}
			else if (vertexToBoneIndices[vertexID].y == -1)
			{
				vertexToBoneIndices[vertexID].y = i;
				vertexToBoneWeights[vertexID].y = weight;
			}
			else if (vertexToBoneIndices[vertexID].z == -1)
			{
				vertexToBoneIndices[vertexID].z = i;
				vertexToBoneWeights[vertexID].z = weight;
			}
			else if (vertexToBoneIndices[vertexID].w == -1)
			{
				vertexToBoneIndices[vertexID].w = i;
				vertexToBoneWeights[vertexID].w = weight;
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

	auto x = vertexToBoneIndices;
	auto x1 = vertexToBoneWeights;

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
		// Extract bone name, parent index, and bind pose
		const aiBone* bone = mesh->mBones[i];
		hstring parentBoneName;
		hstring boneName = hash(bone->mName.C_Str());

		if (bone->mNode == nullptr || bone->mNode->mParent == nullptr)
			parentBoneName = -1;
		else
			parentBoneName = hash(bone->mNode->mParent->mName.C_Str());

		// Find parent bone in the mBones array
		int parentIndex = -1;

		for (unsigned int j = 0; j < mesh->mNumBones; j++)
		{
			const hstring checkedBoneName = hash(mesh->mBones[j]->mName.C_Str());
			if (i != j && parentBoneName == checkedBoneName)
			{
				// Found the parent bone
				parentIndex = j;
				break;
			}
		}

		const aiMatrix4x4 aiInverseBindPose = bone->mOffsetMatrix;
		glm::mat4 inverseBindPose = aiMatrix4x4ToGlm(&aiInverseBindPose);

		rig.boneNames.push_back(boneName);
		rig.parents.push_back(parentIndex);
		rig.inverseBindPose.push_back(inverseBindPose);
	}
	rig.numBones += mesh->mNumBones;
}
