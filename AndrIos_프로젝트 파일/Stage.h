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
#include "stb_image.h"

class STAGE {
	// 기본 스테이지 6면체
	GLuint vaoStage[6];
	GLuint vboStage[18];
	GLuint eboStage[6];
	GLuint backGroundTexture; // 스테이지 배경

	GLfloat stageHeight{ 10.0f };	// 스테이지 높이
	GLfloat stageSize{ 10.0f };		// 스테이지 크기

	GLfloat vStagePositionList[4][3] = {
		{stageSize, 0.0f, 0.0f },			// 우 하단
		{stageSize, stageHeight, 0.0f },	// 우 상단
		{-stageSize,stageHeight, 0.0f},		// 좌 상단
		{-stageSize,0.0f, 0.0f}				// 좌 하단
	};

	GLfloat vStagePositionList_TopBottom[4][3] = {
		{stageSize, 0.0f, 0.0f },			// 우 하단
		{stageSize, 2 * stageHeight, 0.0f },// 우 상단
		{-stageSize,2 * stageHeight, 0.0f},	// 좌 상단
		{-stageSize,0.0f, 0.0f}				// 좌 하단
	};


	unsigned int stageIndex[6] = {
		// 앞면
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
	void DrawScene(GLuint* shaderProgram, unsigned int* modelLocation, glm::mat4* WorldPosition, unsigned int* objectColorLocation, glm::vec3* objColor); // 정보 그리기
};