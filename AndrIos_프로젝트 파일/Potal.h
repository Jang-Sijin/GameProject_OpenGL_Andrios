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

class POTAL {
	// 기본 스테이지 6면체
	GLuint vaoPotal[3];
	GLuint vboPotal[9];
	GLuint eboPotal[3];

	GLuint mainPotalTexture; // main포탈이미지
	GLuint backPotalTexture; // back포탈이미지

	float potalMove{ 0 };
	bool bAngleMode{ false }; // flase: +, true: -

	GLfloat potalHeight{ 4.0f };	// 스테이지 높이
	GLfloat potalSize{ 2.0f };		// 스테이지 크기

	GLfloat vStagePositionList[4][3] = {
		{potalSize, 0.0f, 0.0f },	// 우 하단
		{potalSize, potalHeight, 0.0f },	// 우 상단
		{-potalSize,potalHeight, 0.0f},		// 좌 상단
		{-potalSize,0.0f, 0.0f}		// 좌 하단
	};

	unsigned int vPotalIndex[6] = {
		// 앞면
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
	void DrawScene(GLuint* shaderProgram, unsigned int* modelLocation, glm::mat4* WorldPosition, unsigned int* objectColorLocation, glm::vec3* objColor); // 정보 그리기
	void PotalTimerFunction();
	void ReturnSetData();
};