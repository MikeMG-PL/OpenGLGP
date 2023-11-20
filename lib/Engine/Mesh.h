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

	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned>& indices,
		const std::vector<Texture>& textures, GLenum drawType = GL_TRIANGLES);

	void Draw(Shader shader);

private:
	GLenum drawType = GL_TRIANGLES;
	unsigned int VAO, VBO, EBO;
	void setupMesh();
};
