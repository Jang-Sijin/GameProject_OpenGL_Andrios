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
#include "stb_image.h"

class STAGE {
	// �⺻ �������� 6��ü
	GLuint vaoStage[6];
	GLuint vboStage[18];
	GLuint eboStage[6];
	GLuint backGroundTexture; // �������� ���

	GLfloat stageHeight{ 10.0f };	// �������� ����
	GLfloat stageSize{ 10.0f };		// �������� ũ��

	GLfloat vStagePositionList[4][3] = {
		{stageSize, 0.0f, 0.0f },			// �� �ϴ�
		{stageSize, stageHeight, 0.0f },	// �� ���
		{-stageSize,stageHeight, 0.0f},		// �� ���
		{-stageSize,0.0f, 0.0f}				// �� �ϴ�
	};

	GLfloat vStagePositionList_TopBottom[4][3] = {
		{stageSize, 0.0f, 0.0f },			// �� �ϴ�
		{stageSize, 2 * stageHeight, 0.0f },// �� ���
		{-stageSize,2 * stageHeight, 0.0f},	// �� ���
		{-stageSize,0.0f, 0.0f}				// �� �ϴ�
	};


	unsigned int stageIndex[6] = {
		// �ո�
		0,1,2,
		2,3,0
	};

	GLfloat vStageColor[12] = {
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
	};

	GLfloat vStageTex[4][2] = {
		{1, 0},
		{1, 1},
		{0, 1},
		{0, 0}
	};

public:
	void InitBuffer();
	void InitTexture(GLuint* shaderProgram);
	void DrawScene(GLuint* shaderProgram, unsigned int* modelLocation, glm::mat4* WorldPosition, unsigned int* objectColorLocation, glm::vec3* objColor); // ���� �׸���
};