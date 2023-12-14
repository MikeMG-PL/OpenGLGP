#include "Engine/Mesh.h"

#include <iostream>

#include "Engine/Shader.h"
#include "Engine/Vertex.h"
#include "Engine/Texture.h"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned>& indices,
	const std::vector<Texture>& textures, bool instanced, int numInstances, glm::mat4* matrices)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;
	this->drawType = GL_TRIANGLES;
	this->instanced = instanced;
	this->numInstances = numInstances;
	this->matrices = matrices;

	setupMesh();
}

void Mesh::Draw(Shader shader, int instanceID)
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		// Activate texture unit before binding
		glActiveTexture(GL_TEXTURE0 + i);

		// Get texture number (N for diffuse_textureN etc.)
		std::string number;
		std::string name = textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);

		shader.setFloat((name + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].ID);
	}
	glActiveTexture(GL_TEXTURE0);

	// Draw mesh
	if (!instanced)
	{
		glBindVertexArray(VAO);

		if (indices.empty())
			glDrawArrays(GL_LINE_LOOP, 0, static_cast<int>(vertices.size()));
		else
			glDrawElements(drawType, indices.size(), GL_UNSIGNED_INT, 0);
	}
	else
	{
		shader.setMat4("instanceMatrix", matrices[instanceID]);
		glBindVertexArray(VAO);
		glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, numInstances);
	}

	glBindVertexArray(0);
}

void Mesh::SetupInstancing()
{
	const GLsizei vec4Size = sizeof(glm::vec4);
	glBindVertexArray(VAO);
	// Vertex attributes
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(vec4Size));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));
	
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);
	glBindVertexArray(0);
}

void Mesh::setupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	if (!indices.empty())
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
			&indices[0], GL_STATIC_DRAW);
	}

	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	// Vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	// Vertex texture coordinates
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

	glBindVertexArray(0);
}

// Mesh::~Mesh()
// {
//     glDeleteVertexArrays(1, &VAO);
//     glDeleteBuffers(1, &VBO);
//     glDeleteBuffers(1, &EBO);
//
//     for (const auto& texture : textures) {
//         glDeleteTextures(1, &texture.ID);
//     }
// }
