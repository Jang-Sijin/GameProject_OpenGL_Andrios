#pragma once
#define DEBUG 1
#if DEBUG == 1
#include <GLM/glm.hpp> // glm 수학 라이브러리
#include <GLM/ext.hpp> // glm 수학 라이브러리
#include <GLM/gtc/matrix_transform.hpp> // glm 수학 라이브러리
#elif DEBUG == 2
#include <GL/glm/glm/glm.hpp> // glm 수학 라이브러리
#include <GL/glm/glm/ext.hpp> // glm 수학 라이브러리
#include <GL/glm/glm/gtc/matrix_transform.hpp> // glm 수학 라이브러리
#endif
#include <GL/glew.h> // 필요한 헤더파일 include

class UI_MAIN_MENU {
	// 기본 스테이지 6면체
	GLuint vaoMainMenuUI[3];
	GLuint vboMainMenuUI[9];
	GLuint eboMainMenuUI[3];

	GLuint Texture; // 스테이지 배경
	GLuint ClickTexture; // 스테이지 배경
	GLuint ClickTexture1; // 스테이지 배경

	GLfloat UIHeight{ 14.0f };	// 스테이지 높이
	GLfloat UIWeight{ 10.0f };	// 스테이지 크기

	GLfloat vMainMenuUIPositionList[4][3] = {
		{UIWeight, 0.0f, 0.0f },			// 우 하단
		{UIWeight, UIHeight, 0.0f },	// 우 상단
		{-UIWeight,UIHeight, 0.0f},		// 좌 상단
		{-UIWeight,0.0f, 0.0f}				// 좌 하단
	};

	unsigned int UiIndex[6] = {
		// 앞면
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
	void DrawScene(GLuint* shaderProgram, unsigned int* modelLocation, glm::mat4* WorldPosition, unsigned int* objectColorLocation, glm::vec3* objColor, GLfloat *clickPointX, GLfloat* clickPointY, int textureType); // 정보 그리기
};