#pragma once
#include "Mesh.h"
#include "Camera.h"
#include <assimp/config.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include "StringUtil.h"

using std::min;
using std::max;
using std::vector;
using std::string;
using std::map;

class Model {
public:
	Model(std::string path, bool useSplit = false) {
			loadModel(path, useSplit);
	}
	Model() = default;
	void Draw(Shader &shader, Camera &cam);
	void revisePosition();
	void DrawInnerMesh(Shader& shader, Camera& cam);
	void DrawCollideBox(Shader& shader, Camera& cam);
	void move(float deltaTime);
	void useForce(float deltaTime = 1.0f / 60.0f, glm::vec3 force = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), GLuint meshNum = 0);
	void hitGround(GLuint meshNum = 0);
	void setVelocity(float x, float y, float z);
	void setVelocity(float x, float y, float z, float v);
	void setAcceleration(float x, float y, float z);
	void setAcceleration(float x, float y, float z, float v);
	void setAngularVelocity(float x, float y, float z);
	void setAngularVelocity(float x, float y, float z, float v);
	void setAngularAcceleration(float x, float y, float z);
	void setAngularAcceleration(float x, float y, float z, float v);
	void setScale(float x, float y, float z);
	bool useGravity = false;
	bool useDecrease = false; //������ٶȺͽ��ٶ��Ƿ������ʱ��˥��
	bool useGround = false; //�Ƿ�ᴥ�ػص�
	float vDecreaseCo = 0.99; //�ٶȵ�˥��ϵ��(ÿ1/60���ٶȱ�Ϊԭ���Ķ���)
	float aDecreaseCo = 0.98; //���ٶȵ�˥��ϵ��
	float vMin = 0.5; //�ٶ�С�����ֵ�ῴ��0
	float aMin = 0.005; //���ٶ�С�����ֵ�ῴ��0;
	const float gravityCoefficient = 9.82f;
	float bounciness = 0.90f; //��ײ�ָ�ϵ��
	float inertia = 225.0f; //ת������
	float extents[3]; //��ײ���������߶��ϵİ볤�ȣ��ֱ���x��y��z
	float mass = 50.0f; //ģ������
	std::vector<glm::vec3> defaultGravityPosition;
	std::vector<glm::vec3> gravityPosition; //����λ�ã�Ĭ��Ϊ0��
	std::vector<glm::mat4> trans;
	std::vector<glm::mat4> rotate;
	glm::mat4 scale = glm::mat4(1.0f);
	glm::vec3 defaultPos = glm::vec3(0.0f, 0.0f, 0.0f);
	static bool detectCollision(Model& m1, Model& m2);
	static void collisionWithNoShatter(Model& m1, Model& m2);
	void collisionWithShatter(Model& m2);
	float deltaTime = 1.0f / 60.0f;
	bool splited = false;
private:
	const glm::vec4 baseColor = glm::vec4(0.7f, 0.7f, 0.7f, 0.7f);
	std::vector<glm::vec3> velocity; //�ٶ�
	std::vector<glm::vec3> acceleration; //���ٶ�
	std::vector<glm::vec3> angularVelocity; //ת�����ٶ�
	std::vector<glm::vec3> angularAcceleration;//ת���Ǽ��ٶ�

	std::vector<Mesh> meshes;
	std::vector<Mesh> innerMeshes;
	std::vector<Mesh> collideBox;
	std::string directory;
	Assimp::Importer importer;
	std::vector<Texture> textures_loaded;
	GLuint meshNum = 0;
	std::vector<float> xMin, xMax, yMin, yMax, zMin, zMax;
	void loadModel(std::string path, bool useSplit);
	void processNode(aiNode* node, const aiScene* scene, bool useSplit);
	void processInnerMesh();
	void processCollideBox();
	void prepareMesh(GLuint num = 1);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, const aiScene* scene);


};

