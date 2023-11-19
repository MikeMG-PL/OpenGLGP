// #include "Components/Orbit.h"
//
// #include <vector>
// #include <glad/glad.h>
// #include <glm/trigonometric.hpp>
// #include <glm/ext/scalar_constants.hpp>
//
// #include "Engine/Vertex.h"
//
// Orbit::Orbit(float centerX, float centerZ, float radiusX, float radiusZ, int segments)
// 	:centerX(centerX), centerZ(centerZ), radiusX(radiusX), radiusZ(radiusZ), segments(segments)
// {
// 	//meshes.emplace_back(createOrbit());
//     createOrbit();
// }
//
// void Orbit::createOrbit() const
// {
//     float const theta = 2 * glm::pi<float>() / static_cast<float>(segments);
//     float const c = glm::cos(theta);
//     float const s = glm::sin(theta);
//
//     float x = 1; // Start at angle = 0 
//     float z = 0;
//
//     std::vector<Vertex> vertices;
//
//     glBegin(GL_LINE_LOOP);
//     for (int i = 0; i < segments; ++i)
//     {
//         Vertex vertex = {};
//         // Apply radius and offset
//         vertex.position = glm::vec3(x * radiusX + centerX, 0.0f, z * radiusZ + centerZ);
//
//         // Apply the rotation matrix
//         float const t = x;
//         x = c * x - s * z;
//         z = s * t + c * z;
//
//         vertices.emplace_back(vertex);
//     }
//     glEnd();
//
//     //return Mesh::Create(vertices, {}, {}, GL_LINE_LOOP);
// }
