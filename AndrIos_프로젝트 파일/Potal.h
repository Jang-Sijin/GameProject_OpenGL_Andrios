#pragma once
#define DEBUG 1
#if DEBUG == 1
#include <GLM/glm.hpp> // glm ���� ���̺귯��
#include <GLM/ext.hpp> // glm ���� ���̺귯��
#include <GLM/gtc/matrix_transform.hpp> // glm ���� ���̺귯��
#elif DEBUG == 2
#include <GL/glm/glm/glm.hpp> // glm ���� ���̺귯��
#include <GL/glm/glm/ext.hpp> // glm ���� ���̺귯��
#include <GL/glm/glm/gtc/matrix_transform.hpp> // glm ���� ���̺귯��
#endif
#include <GL/glew.h> // �ʿ��� ������� include

class POTAL {
	// �⺻ �������� 6��ü
	GLuint vaoPotal[3];
	GLuint vboPotal[9];
	GLuint eboPotal[3];

	GLuint mainPotalTexture; // main��Ż�̹���
	GLuint backPotalTexture; // back��Ż�̹���

	float potalMove{ 0 };
	bool bAngleMode{ false }; // flase: +, true: -

	GLfloat potalHeight{ 4.0f };	// �������� ����
	GLfloat potalSize{ 2.0f };		// �������� ũ��

	GLfloat vStagePositionList[4][3] = {
		{potalSize, 0.0f, 0.0f },	// �� �ϴ�
		{potalSize, potalHeight, 0.0f },	// �� ���
		{-potalSize,potalHeight, 0.0f},		// �� ���
		{-potalSize,0.0f, 0.0f}		// �� �ϴ�
	};

	unsigned int vPotalIndex[6] = {
		// �ո�
		0,1,2,
		2,3,0
	};

	GLfloat vPotalColor[12] = {
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
	};

	GLfloat vPotalTex[4][2] = {
		{1, 0},
		{1, 1},
		{0, 1},
		{0, 0}
	};

public:
	void InitBuffer();
	void InitTexture(GLuint* shaderProgram);
	void DrawScene(GLuint* shaderProgram, unsigned int* modelLocation, glm::mat4* WorldPosition, unsigned int* objectColorLocation, glm::vec3* objColor); // ���� �׸���
	void PotalTimerFunction();
	void ReturnSetData();
};