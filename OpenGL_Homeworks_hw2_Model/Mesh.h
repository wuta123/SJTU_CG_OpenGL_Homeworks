#pragma once
#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include <string>
#include "VAO.h"
#include "EBO.h"
#include "Camera.h"
#include "Texture.h"
#include <map>

using std::map;

class Mesh
{
public:
	std::vector <Vertex> vertices;
	std::vector <GLuint> indices;
	std::vector <Texture> textures;

	VAO VAO;

	Mesh(std::vector <Vertex>& vertices, std::vector<GLuint>& indices, std::vector <Texture>& textures);
	Mesh() = default;
	std::vector<Mesh> splitMesh(GLuint split, glm::vec3 max, glm::vec3 min);

	void Draw(Shader& shader, Camera& camera);
};


#endif // !MESH_CLASS_H
