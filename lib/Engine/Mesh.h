#pragma once
#include <vector>
#include "Engine/Shader.h"
#include "Engine/Texture.h"
#include "Engine/Vertex.h"

class Mesh
{
public:

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	// ~Mesh();

	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned>& indices,
		const std::vector<Texture>& textures, bool instanced = false, int numInstances = -1, glm::mat4* matrices = nullptr);

	void Draw(Shader shader, int instanceID = -1);
	void SetupInstancing();
	unsigned int VAO, VBO, EBO;


private:
	GLenum drawType = GL_TRIANGLES;

	// numInstances is only used with instancing enabled
	int numInstances;
	bool instanced;
	glm::mat4* matrices;

	void setupMesh();
	
};
