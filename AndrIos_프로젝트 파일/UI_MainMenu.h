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

class UI_MAIN_MENU {
	// �⺻ �������� 6��ü
	GLuint vaoMainMenuUI[3];
	GLuint vboMainMenuUI[9];
	GLuint eboMainMenuUI[3];

	GLuint Texture; // �������� ���
	GLuint ClickTexture; // �������� ���
	GLuint ClickTexture1; // �������� ���

	GLfloat UIHeight{ 14.0f };	// �������� ����
	GLfloat UIWeight{ 10.0f };	// �������� ũ��

	GLfloat vMainMenuUIPositionList[4][3] = {
		{UIWeight, 0.0f, 0.0f },			// �� �ϴ�
		{UIWeight, UIHeight, 0.0f },	// �� ���
		{-UIWeight,UIHeight, 0.0f},		// �� ���
		{-UIWeight,0.0f, 0.0f}				// �� �ϴ�
	};

	unsigned int UiIndex[6] = {
		// �ո�
		0,1,2,
		2,3,0
	};

	GLfloat vUiColor[12] = {
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
	};

	GLfloat vUiTex[4][2] = {
		{1, 0},
		{1, 1},
		{0, 1},
		{0, 0}
	};

public:
	void InitBuffer();
	void InitTexture(GLuint* shaderProgram, int textureType);
	void DrawScene(GLuint* shaderProgram, unsigned int* modelLocation, glm::mat4* WorldPosition, unsigned int* objectColorLocation, glm::vec3* objColor, GLfloat *clickPointX, GLfloat* clickPointY, int textureType); // ���� �׸���
};