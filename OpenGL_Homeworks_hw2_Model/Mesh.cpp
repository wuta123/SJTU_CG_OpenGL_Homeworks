#include "Mesh.h"
#pragma once


glm::vec3 lineAndPlaneConvexPoint(GLfloat a, GLfloat b, GLfloat c, glm::vec3 p1, glm::vec3 p2) {
	// ����ֱ�ߵķ�������
	glm::vec3 direction = p2 - p1;

	// ����ֱ�߷��̵Ĳ���
	GLfloat t = -(a * p1.x + b * p1.y + c * p1.z + 1) / glm::dot(direction, glm::vec3(a, b, c));

	// ���㽻������
	glm::vec3 intersection = p1 + t * direction;
	return intersection;
}

glm::vec3 getCutFace(glm::vec3 min, glm::vec3 max) {
	//min: ��ά������Ŀռ���С�㣬 max����ά������Ŀռ�����
	//����ֵ��glm::vec3(a, b, c)��������ax + by + cz + 1 = 0�����п���minΪ�ռ���С�㡢maxΪ�ռ�����������塣
	// ���������ƽ������

	//�������һ��ƽ��ķ�����
	GLfloat bottom = rand() % 5;
	GLfloat tmp1 = rand() % 3 - 2.468974132 + bottom, tmp2 = rand() % 3 - 2.6311549 + bottom;
	tmp1 += (tmp1 > 0) ? 0.001f : -0.001f;
	tmp2 += (tmp2 > 0) ? 0.001f : -0.001f;
	glm::vec3 res = glm::vec3(1.0f / (bottom+1.0f), 1.0f / (tmp1), 1.0f / (tmp2));
	res = glm::normalize(res);
	//��Min~Max����Խ��������ѡ��һ��k
	glm::vec3 k = min + (max - min) * (1.0f / (rand() % 5 + 1.05f));
	//������Ծ���k��������d
	GLfloat d = -glm::dot(k, res);
	d += (d > 0) ?  0.001f : -0.001f;
	//res��ÿһ�����d
	res /= d;
	return res;
}

std::vector<Mesh> splitMeshByFace(Mesh mesh, GLfloat a, GLfloat b, GLfloat c)
{
	//����ǰ����������淽�̣�ax + by + cz +1 = 0��Ϊ���ࣺ
	// 1. ax + by + cz + 1 >= 0�ģ���ԭ�����������и���������ӳ�䴢����m1�У���ԭ������������λ��->���µ��и�����������λ��+1��
	// 2. ax + by + cz + 1<= 0�ģ���ԭ�����������и���������ӳ�䴢����m2�У���ԭ������������λ��->���µ��и�����������λ��+1��
	//ע�⣬�ڷָ����ϵĵ�Ӧ����Ӹ����е�������
	if (a == 0 && b == 0 && c == 0) {
		std::vector<Mesh> re;
		re.push_back(mesh);
		return re;
	}
	map<GLuint, GLuint> m1, m2;
	std::vector<Vertex> v1, v2;
	std::vector<GLuint> i1, i2;
	std::vector<Texture> t;
	GLuint k1 = 0, k2 = 0;
	GLuint totalCount = mesh.vertices.size();


	//1.������б�
	for (GLuint i = 0; i < totalCount; i++) {
		glm::vec3 vector = mesh.vertices[i].position;
		float re = a* vector.x + b * vector.y + c * vector.z + 1;
		if (re >= 0) {
			if (m1[i] == 0) {
				m1[i] = ++k1;
				v1.push_back(mesh.vertices[i]);
			}
		}
		if(re <= 0){
			if (m2[i] == 0) {
				m2[i] = ++k2;
				v2.push_back(mesh.vertices[i]);
			}
		}
	}

	if (v1.size() == 0 || v2.size() == 0) {
		std::vector<Mesh> re;
		re.push_back(mesh);
		return re;
	}

	//2.���������б�
	//��Ҫ�����£�
	//1. ��ԭ���������ֱ��Ϊ���������е�������
	//2. ��ÿһ���ߣ��ж��Ƿ�һ�ߵ������ڷָ�������࣬��������࣬����ָ���ͱߵĽ��㣬�������������������Ķ������飬����ԭ���ı߷ָ�����顣

	totalCount = mesh.indices.size();
	for (GLuint i = 0; i < totalCount; i += 3) {
		glm::vec3 p1 = mesh.vertices[mesh.indices[i]].position, p2 = mesh.vertices[mesh.indices[i + 1]].position, p3 = mesh.vertices[mesh.indices[i + 2]].position;
		float p1O = (a * p1.x + b * p1.y + c * p1.z + 1), p2O = (a * p2.x + b * p2.y + c * p2.z + 1), p3O = (a * p3.x + b * p3.y + c * p3.z + 1);
		int res = (p1O == 0.0f) + (p2O == 0.0f) + (p3O == 0.0f);
		if (res == 0) {
			//����ͨ�����������������ཻ���߲����������ཻ
			if (p1O > 0 && p2O > 0 && p3O > 0) {
				i1.push_back(m1[mesh.indices[i]] - 1);
				i1.push_back(m1[mesh.indices[i + 1]] - 1);
				i1.push_back(m1[mesh.indices[i + 2]] - 1);
				continue;
			}
			else if (p1O < 0 && p2O < 0 && p3O < 0) {
				i2.push_back(m2[mesh.indices[i]] - 1);
				i2.push_back(m2[mesh.indices[i + 1]] - 1);
				i2.push_back(m2[mesh.indices[i + 2]] - 1);
				continue;
			}


			//�����ж�������������������
			int res2 = (p1O > 0.0f) + (p2O > 0.0f) + (p3O > 0.0f);
			if (res2 > 1) {
				//������
				if (p1O < 0.0f) {
					glm::vec3 convexPoint1 = lineAndPlaneConvexPoint(a, b, c, p1, p2),
						convexPoint2 = lineAndPlaneConvexPoint(a, b, c, p1, p3);
					Vertex newV1 = Vertex(convexPoint1), 
						newV2 = Vertex(convexPoint2);
					v2.push_back(newV1), v2.push_back(newV2);
					i2.push_back(m2[mesh.indices[i]] - 1); i2.push_back(k2++); i2.push_back(k2++);

					v1.push_back(newV1), v1.push_back(newV2);
					i1.push_back(m1[mesh.indices[i + 1]] - 1), i1.push_back(m1[mesh.indices[i + 2]] - 1), i1.push_back(k1);
					i1.push_back(m1[mesh.indices[i + 2]] - 1), i1.push_back(k1 + 1), i1.push_back(k1);
					k1+= 2;
				}
				else if (p2O < 0.0f) {
					glm::vec3 convexPoint1 = lineAndPlaneConvexPoint(a, b, c, p2, p3),
						convexPoint2 = lineAndPlaneConvexPoint(a, b, c, p2, p1);
					Vertex newV1 = Vertex(convexPoint1),
						newV2 = Vertex(convexPoint2);
					v2.push_back(newV1), v2.push_back(newV2);
					i2.push_back(m2[mesh.indices[i + 1]] - 1); i2.push_back(k2++); i2.push_back(k2++);

					v1.push_back(newV1), v1.push_back(newV2);
					i1.push_back(m1[mesh.indices[i + 2]] - 1), i1.push_back(m1[mesh.indices[i]] - 1), i1.push_back(k1);
					i1.push_back(m1[mesh.indices[i]] - 1), i1.push_back(k1 + 1), i1.push_back(k1);
					k1 += 2;
				}
				else if (p3O < 0.0f) {
					glm::vec3 convexPoint1 = lineAndPlaneConvexPoint(a, b, c, p3, p1),
						convexPoint2 = lineAndPlaneConvexPoint(a, b, c, p3, p2);
					Vertex newV1 = Vertex(convexPoint1),
						newV2 = Vertex(convexPoint2);
					v2.push_back(newV1), v2.push_back(newV2);
					i2.push_back(m2[mesh.indices[i + 2]] - 1); i2.push_back(k2++); i2.push_back(k2++);

					v1.push_back(newV1), v1.push_back(newV2);
					i1.push_back(m1[mesh.indices[i]] - 1), i1.push_back(m1[mesh.indices[i + 1]] - 1), i1.push_back(k1);
					i1.push_back(m1[mesh.indices[i + 1]] - 1), i1.push_back(k1 + 1), i1.push_back(k1);
					k1 += 2;
				}
			}
			else {
				//������
				if (p1O > 0.0f) {
					glm::vec3 convexPoint1 = lineAndPlaneConvexPoint(a, b, c, p1, p2),
						convexPoint2 = lineAndPlaneConvexPoint(a, b, c, p1, p3);
					Vertex newV1 = Vertex(convexPoint1),
						newV2 = Vertex(convexPoint2);
					v1.push_back(newV1), v1.push_back(newV2);
					i1.push_back(m1[mesh.indices[i]] - 1); i1.push_back(k1++); i1.push_back(k1++);

					v2.push_back(newV1), v2.push_back(newV2);
					i2.push_back(m2[mesh.indices[i + 1]] - 1), i2.push_back(m2[mesh.indices[i + 2]] - 1), i2.push_back(k2);
					i2.push_back(m2[mesh.indices[i + 2]] - 1), i2.push_back(k2 + 1), i2.push_back(k2);
					k2 += 2;
				}
				else if (p2O > 0.0f) {
					glm::vec3 convexPoint1 = lineAndPlaneConvexPoint(a, b, c, p2, p3),
						convexPoint2 = lineAndPlaneConvexPoint(a, b, c, p2, p1);
					Vertex newV1 = Vertex(convexPoint1),
						newV2 = Vertex(convexPoint2);
					v1.push_back(newV1), v1.push_back(newV2);
					i1.push_back(m1[mesh.indices[i + 1]] - 1); i1.push_back(k1++); i1.push_back(k1++);

					v2.push_back(newV1), v2.push_back(newV2);
					i2.push_back(m2[mesh.indices[i + 2]] - 1), i2.push_back(m2[mesh.indices[i]] - 1), i2.push_back(k2);
					i2.push_back(m2[mesh.indices[i]] - 1), i2.push_back(k2 + 1), i2.push_back(k2);
					k2 += 2;
				}
				else if (p3O > 0.0f) {
					glm::vec3 convexPoint1 = lineAndPlaneConvexPoint(a, b, c, p3, p1),
						convexPoint2 = lineAndPlaneConvexPoint(a, b, c, p3, p2);
					Vertex newV1 = Vertex(convexPoint1),
						newV2 = Vertex(convexPoint2);
					v1.push_back(newV1), v1.push_back(newV2);
					i1.push_back(m1[mesh.indices[i + 2]] - 1); i1.push_back(k1++); i1.push_back(k1++);

					v2.push_back(newV1), v2.push_back(newV2);
					i2.push_back(m2[mesh.indices[i]] - 1), i2.push_back(m2[mesh.indices[i + 1]] - 1), i2.push_back(k2);
					i2.push_back(m2[mesh.indices[i + 1]] - 1), i2.push_back(k2 + 1), i2.push_back(k2);
					k2 += 2;
				}
			}
		}
		else if (res == 1) {
			//�����ε�һ������������
			if (p1O == 0.0f) {
				if (p2O < 0) {
					glm::vec3 convexPoint = lineAndPlaneConvexPoint(a, b, c, p2, p3);
					Vertex newV = Vertex(convexPoint);
					v1.push_back(newV);
					v2.push_back(newV);
					i1.push_back(m1[mesh.indices[i]] - 1);
					i1.push_back(k1++);
					i1.push_back(m1[mesh.indices[i + 2]] - 1);
					i2.push_back(m2[mesh.indices[i]] -1);
					i2.push_back(m2[mesh.indices[i + 1]] - 1);
					i2.push_back(k2++);
				}
				else {
					glm::vec3 convexPoint = lineAndPlaneConvexPoint(a, b, c, p2, p3);
					Vertex newV = Vertex(convexPoint);
					v1.push_back(newV);
					v2.push_back(newV);
					i1.push_back(m1[mesh.indices[i]] -1);
					i1.push_back(m1[mesh.indices[i + 1]] - 1);
					i1.push_back(k1++);
					i2.push_back(m2[mesh.indices[i]] -1);
					i2.push_back(k2++);
					i2.push_back(m2[mesh.indices[i + 2]] -1);

				}
			}
			else if (p2O == 0.0f) {
				if (p1O < 0) {
					glm::vec3 convexPoint = lineAndPlaneConvexPoint(a, b, c, p1, p3);
					Vertex newV = Vertex(convexPoint);
					v1.push_back(newV);
					v2.push_back(newV);
					i1.push_back(m1[mesh.indices[i + 1]] -1);
					i1.push_back(m1[mesh.indices[i + 2]] -1);
					i1.push_back(k1++);
					i2.push_back(m2[mesh.indices[i]] -1);
					i2.push_back(m2[mesh.indices[i + 1]] -1);
					i2.push_back(k2++);
				}
				else {
					glm::vec3 convexPoint = lineAndPlaneConvexPoint(a, b, c, p1, p3);
					Vertex newV = Vertex(convexPoint);
					v1.push_back(newV);
					v2.push_back(newV);
					i1.push_back(m1[mesh.indices[i]] -1);
					i1.push_back(m1[mesh.indices[i + 1]] -1);
					i1.push_back(k1++);
					i2.push_back(m2[mesh.indices[i + 1]] -1);
					i2.push_back(m2[mesh.indices[i + 2]] -1);
					i2.push_back(k2++);
				}
			}
			else if (p3O == 0.0f) {
				if (p1O < 0) {
					glm::vec3 convexPoint = lineAndPlaneConvexPoint(a, b, c, p1, p2);
					Vertex newV = Vertex(convexPoint);
					v1.push_back(newV);
					v2.push_back(newV);
					i1.push_back(m1[mesh.indices[i + 1]] -1);
					i1.push_back(m1[mesh.indices[i + 2]] -1);
					i1.push_back(k1++);
					i2.push_back(m2[mesh.indices[i]] -1);
					i2.push_back(k2++);
					i2.push_back(m2[mesh.indices[i + 2]] -1);
				}
				else {
					glm::vec3 convexPoint = lineAndPlaneConvexPoint(a, b, c, p1, p2);
					Vertex newV = Vertex(convexPoint);
					v1.push_back(newV);
					v2.push_back(newV);
					i1.push_back(m1[mesh.indices[i]] -1);
					i1.push_back(k1++);
					i1.push_back(m1[mesh.indices[i + 2]] - 1);
					i2.push_back(m2[mesh.indices[i + 1]] -1);
					i2.push_back(m2[mesh.indices[i + 2]] -1);
					i2.push_back(k2++);
				}
			}
		}
		else if(res == 2){
			//�����ε��������������ϣ���������������⴦��ֻ��Ҫ��ԭ������������ĳһ�������е���������
			if (p1O > 0 || p2O > 0 || p3O > 0) {
				i1.push_back(m1[mesh.indices[i]] - 1);
				i1.push_back(m1[mesh.indices[i + 1]] - 1);
				i1.push_back(m1[mesh.indices[i + 2]] - 1);
			}
			else {
				i2.push_back(m2[mesh.indices[i]] - 1);
				i2.push_back(m2[mesh.indices[i + 1]] - 1);
				i2.push_back(m2[mesh.indices[i + 2]] - 1);
			}
		}
		else {
			i1.push_back(m1[mesh.indices[i]] - 1);
			i1.push_back(m1[mesh.indices[i + 1]] - 1);
			i1.push_back(m1[mesh.indices[i + 2]] - 1);
			i2.push_back(m2[mesh.indices[i]] - 1);
			i2.push_back(m2[mesh.indices[i + 1]] - 1);
			i2.push_back(m2[mesh.indices[i + 2]] - 1);
		}
	}


	std::vector<Mesh> res;
	if(v1.size() > 3) res.push_back(Mesh(v1, i1, t));
	if(v2.size() > 3) res.push_back(Mesh(v2, i2, t));
	return res;
}

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

std::vector<Mesh> Mesh::splitMesh(GLuint split, glm::vec3 max, glm::vec3 min)
{
	//�ú�����һ��mesh����ֽ�Ϊsplit�������񣬶��ҷָ������¹����ָ����Լ����ǵķ�����
	//max������������㣬 min�����������С��
	std::vector<Mesh> tmp1, tmp2;
	bool t1 = false;
	tmp1.push_back(*this);

	if (split > indices.size() / 3 || split < 0)
		split = indices.size() / 3;
	for(int i = 0; i < split; i++){
		glm::vec3 face = getCutFace(min, max);
		if (t1) {
			for (int j = 0; j < tmp1.size(); j++) {
				std::vector<Mesh> splitRes = splitMeshByFace(tmp1[j], face.x, face.y, face.z);
				for (int k = 0; k < splitRes.size(); k++)
					tmp2.push_back(splitRes[k]);
			}
			tmp1.clear();
		}
		else {
			for (int j = 0; j < tmp2.size(); j++) {
				std::vector<Mesh> splitRes = splitMeshByFace(tmp2[j], face.x, face.y, face.z);
				for (int k = 0; k < splitRes.size(); k++)
					tmp1.push_back(splitRes[k]);
			}
			tmp2.clear();
		}
		t1 = !t1;
	}

	//���㷨��
	return t1 ? tmp1 : tmp2;
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
