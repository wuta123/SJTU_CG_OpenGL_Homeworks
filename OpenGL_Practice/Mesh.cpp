#include "Mesh.h"
#pragma once

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	VAO.Bind();

	VBO VBO(vertices);
	EBO EBO(indices);

	VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	VAO.LinkAttrib(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
	VAO.LinkAttrib(VBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));

	VAO.Unbind();
	VBO.Unbind();
	EBO.Unbind();

}

std::vector<Mesh> Mesh::splitMesh(GLuint split)
{
	//�ú�����һ��mesh����ֽ�Ϊsplit�������񣬶��ҷָ������¹����ָ����Լ����ǵķ�����
	std::vector<Mesh> subMeshes;
	if (split > indices.size() / 3 || split < 0)
		split = indices.size() / 3;

	GLuint trianglesPerSplit = indices.size() / split;

	for (GLuint i = 0; i < split; ++i) {
		std::vector<Vertex> subVertices;
		std::vector<GLuint> subIndices;

		// ������ǰ�������������
		for (GLuint j = i * trianglesPerSplit; j < (i + 1) * trianglesPerSplit; j+= 3) {
			// ��ӵ�ǰ�����ε�����
			subIndices.push_back(indices[j]);
			subIndices.push_back(indices[j+1]);
			subIndices.push_back(indices[j+2]);
		}
		
		map<GLuint, GLuint> indicesMap; //����ԭ�������е�����λ��ӳ�䵽��������
		GLuint indicesNum = 0;

		// ���������������µĶ����б�
		for (GLuint index : subIndices) {
			if (indicesMap[index] == 0) {
				indicesMap[index] = (indicesNum++) + 1;
				subVertices.push_back(vertices[index]);
			}
		}

		for (GLuint& index : subIndices) {
			index = indicesMap[index] - 1;
		}

		// ���������񲢽�����ӵ������
		Mesh subMesh(subVertices, subIndices, textures);

		subMeshes.push_back(subMesh);
	}

	//TODO:�ؽ��ָ���ͷָ���ķ�������

	return subMeshes;
}

void Mesh::Draw(Shader& shader, Camera& camera)
{
	shader.Activate();
	VAO.Bind();

	unsigned int numDiffuse = 0;
	unsigned int numSpecular = 0;

	for (unsigned i = 0; i < textures.size(); i++) {
		std::string num;
		std::string type = textures[i].type;
		if (type == "texture_diffuse") {
			num = std::to_string(numDiffuse++);
		}
		else if (type == "texture_specular") {
			num = std::to_string(numSpecular++);
		}
		textures[i].texUnit(shader, (type + num).c_str(), i);
		textures[i].Bind();
	}

	glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
	camera.Matrix(shader, "camMatrix");

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

}
