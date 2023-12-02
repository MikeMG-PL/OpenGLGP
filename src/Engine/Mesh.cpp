#include "Engine/Mesh.h"

#include <iostream>

#include "Engine/Shader.h"
#include "Engine/Vertex.h"
#include "Engine/Texture.h"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned>& indices,
	const std::vector<Texture>& textures, GLenum drawType)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;
    this->drawType = drawType;

	setupMesh();
}

void Mesh::Draw(Shader shader)
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;

    if (textures.empty())
        glBindTexture(GL_TEXTURE_2D, 0);

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
    glBindVertexArray(VAO);

    if(indices.empty())
        glDrawArrays(GL_LINE_LOOP, 0, static_cast<int>(vertices.size()));
    else
		glDrawElements(drawType, static_cast<int>(indices.size()), GL_UNSIGNED_INT, 0);

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
