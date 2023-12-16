#include "Components/Model.h"
#include "Engine/Mesh.h"
#include <Engine/Shader.h>
#include "Engine/GameObject.h"
#include "Engine/Texture.h"
#include "Engine/Vertex.h"
#include "Helpers/TextureLoader.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_trigonometric.hpp>

#include "Engine/Editor.h"

Model::Model(const std::string& path, bool instanced, int numInstances, glm::mat4* matrices)
{
	this->instanced = instanced;
	this->numInstances = numInstances;
	this->matrices = matrices;
	loadModel(path);
}

Model::Model(const std::string& path, glm::vec2 tiling, bool wireFrame)
{
	this->tiling = tiling;
	this->wireFrame = wireFrame;
	this->instanced = false;
	this->numInstances = -1;
	this->matrices = nullptr;
	loadModel(path);
}

void Model::Draw(Shader shader)
{
	if (wireFrame)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		shader.setBool("wireframe", true);
		shader.setVector4("customColor", glm::vec4(1, 1, 1, 1));
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		shader.setBool("wireframe", false);
	}

	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].Draw(shader);
}

bool Model::IsInstanced() const
{
	return instanced;
}

std::vector<Mesh>& Model::GetMeshes()
{
	return meshes;
}

void Model::addOrbit(float r)
{
	int segmentCount = 64;
	float radiusX = r;
	float centerX = 0;
	float radiusZ = r;
	float centerZ = 0;

	float const theta = 2 * glm::pi<float>() / static_cast<float>(segmentCount);
	float const c = glm::cos(theta);
	float const s = glm::sin(theta);

	float x = 1; // Start at angle = 0 
	float z = 0;

	std::vector<Vertex> vertices;

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < segmentCount; ++i)
	{
		Vertex vertex = {};
		// Apply radius and offset
		vertex.position = glm::vec3(x * radiusX + centerX, 0.0f, z * radiusZ + centerZ);

		// Apply the rotation matrix
		float const t = x;
		x = c * x - s * z;
		z = s * t + c * z;

		vertices.emplace_back(vertex);
	}
	glEnd();
	meshes.push_back(Mesh(vertices, {}, {}, instanced));
}

void Model::loadModel(const std::string& path)
{
	Assimp::Importer import;
	const aiScene * scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	// Process each mesh node if exists
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	// Same for each child
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
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
			vec.x = mesh->mTextureCoords[0][i].x * tiling.x;
			vec.y = mesh->mTextureCoords[0][i].y * tiling.y;
			vertex.texCoords = vec;
		}
		else
			vertex.texCoords = glm::vec2(0.0f, 0.0f);

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

	return Mesh(vertices, indices, textures, instanced, numInstances, matrices);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
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

void Model::createSphere()
{
	auto constexpr PI = glm::pi<float>();
	float const length_inverse = 1.0f / sphereRadius;

	// stacks = Editor::Get().details;
	// sectors = Editor::Get().details;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for (uint32_t x = 0; x <= stacks; ++x)
	{
		for (uint32_t y = 0; y <= sectors; ++y)
		{
			float const x_segment = static_cast<float>(x) / static_cast<float>(stacks);
			float const y_segment = static_cast<float>(y) / static_cast<float>(sectors);

			float const x_position = sphereRadius * glm::cos(x_segment * 2.0f * PI) * glm::sin(y_segment * PI);
			float const y_position = sphereRadius * glm::cos(y_segment * PI);
			float const z_position = sphereRadius * glm::sin(x_segment * 2.0f * PI) * glm::sin(y_segment * PI);

			Vertex vertex = {};
			vertex.position = glm::vec3(x_position, y_position, z_position);
			vertex.normal = glm::vec3(x_position * length_inverse, y_position * length_inverse, z_position * length_inverse);
			vertex.texCoords = glm::vec2(x_segment, y_segment);
			vertices.emplace_back(vertex);
		}
	}

	bool odd_row = false;
	for (uint32_t y = 0; y < sectors; ++y)
	{
		if (!odd_row)
		{
			for (uint32_t x = 0; x <= stacks; ++x)
			{
				indices.push_back(y * (stacks + 1) + x);
				indices.push_back((y + 1) * (stacks + 1) + x);
			}
		}
		else
		{
			for (int32_t x = stacks; x >= 0; --x)
			{
				indices.push_back((y + 1) * (stacks + 1) + x);
				indices.push_back(y * (stacks + 1) + x);
			}
		}

		odd_row = !odd_row;
	}

	std::vector<Texture> diffuse_maps = { loadTextureForSphere() };
	textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());

	meshes.emplace_back(vertices, indices, textures, GL_TRIANGLE_STRIP);
}

Texture Model::loadTextureForSphere()
{
	Texture texture;
	auto dir = texturePath.substr(0, texturePath.find_last_of('/'));
	texture.ID = textureFromFile(texturePath.c_str(), dir);
	texture.type = "texture_diffuse";
	texture.path = texturePath;
	return texture;
}