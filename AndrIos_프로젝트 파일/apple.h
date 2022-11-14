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


class APPLE {
	// �⺻ �������� 6��ü
	GLuint vaoBox[6];
	GLuint vboBox[18];
	GLuint eboBox[6];
	GLuint backGroundTexture; // �������� ���

	GLfloat boxHeight{ 1.0f };	// �������� ����
	GLfloat boxSize{ 0.5f };		// �������� ũ��

	float boxMoveY{ 0.0f }; // �ڽ� y�� ������
	float boxAnimationMoveY{ 0.0f }; // �ڽ� y�� ������
	float boxAnimationRotateY{ 0.0f }; // �ڽ� y�� ������
	float animeMoveSpeed{ 0.005f };
	float boxAnimationMoveY_2{ 0.0f }; // �ڽ� y�� ������
	bool bGetApple{ false };

	GLfloat vBoxPositionList[4][3] = {
		{boxSize, 0.0f, 0.0f },			// �� �ϴ�
		{boxSize, boxHeight, 0.0f },	// �� ���
		{-boxSize,boxHeight, 0.0f},		// �� ���
		{-boxSize,0.0f, 0.0f}			// �� �ϴ�
	};

	GLfloat vBoxPositionList_TopBottom[4][3] = {
		{boxSize, 0.0f, 0.0f },			// �� �ϴ�
		{boxSize, boxHeight, 0.0f },	// �� ���
		{-boxSize,boxHeight, 0.0f},		// �� ���
		{-boxSize,0.0f, 0.0f}			// �� �ϴ�
	};


	unsigned int boxIndex[6] = {
		// �ո�
		0,1,2,
		2,3,0
	};

	GLfloat vBoxColor[12] = {
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
	};

	GLfloat vBoxTex[4][2] = {
		{1, 0},
		{1, 1},
		{0, 1},
		{0, 0}
	};

public:
	void ReturnSetData();
	void InitBuffer();
	void InitTexture(GLuint* shaderProgram);
	void DrawScene(GLuint* shaderProgram, unsigned int* modelLocation, glm::mat4* WorldPosition, unsigned int* objectColorLocation, glm::vec3* objColor, GLfloat p_X, GLfloat p_Z); // ���� �׸���
	void BoxTimerFunction(GLfloat p_X, GLfloat p_Z);
};