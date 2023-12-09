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
		const std::vector<Texture>& textures, bool instanced);

	void Draw(Shader shader);
	unsigned int VAO, VBO, EBO;
private:
	GLenum drawType = GL_TRIANGLES;
	bool instanceMesh;
	void setupMesh();
	
};
