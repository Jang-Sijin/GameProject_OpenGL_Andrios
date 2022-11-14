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


class APPLE {
	// 기본 스테이지 6면체
	GLuint vaoBox[6];
	GLuint vboBox[18];
	GLuint eboBox[6];
	GLuint backGroundTexture; // 스테이지 배경

	GLfloat boxHeight{ 1.0f };	// 스테이지 높이
	GLfloat boxSize{ 0.5f };		// 스테이지 크기

	float boxMoveY{ 0.0f }; // 박스 y축 움직임
	float boxAnimationMoveY{ 0.0f }; // 박스 y축 움직임
	float boxAnimationRotateY{ 0.0f }; // 박스 y축 움직임
	float animeMoveSpeed{ 0.005f };
	float boxAnimationMoveY_2{ 0.0f }; // 박스 y축 움직임
	bool bGetApple{ false };

	GLfloat vBoxPositionList[4][3] = {
		{boxSize, 0.0f, 0.0f },			// 우 하단
		{boxSize, boxHeight, 0.0f },	// 우 상단
		{-boxSize,boxHeight, 0.0f},		// 좌 상단
		{-boxSize,0.0f, 0.0f}			// 좌 하단
	};

	GLfloat vBoxPositionList_TopBottom[4][3] = {
		{boxSize, 0.0f, 0.0f },			// 우 하단
		{boxSize, boxHeight, 0.0f },	// 우 상단
		{-boxSize,boxHeight, 0.0f},		// 좌 상단
		{-boxSize,0.0f, 0.0f}			// 좌 하단
	};


	unsigned int boxIndex[6] = {
		// 앞면
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
	void DrawScene(GLuint* shaderProgram, unsigned int* modelLocation, glm::mat4* WorldPosition, unsigned int* objectColorLocation, glm::vec3* objColor, GLfloat p_X, GLfloat p_Z); // 정보 그리기
	void BoxTimerFunction(GLfloat p_X, GLfloat p_Z);
};