#pragma once
#include <GL/glew.h> // 필요한 헤더파일 include

static int cameraRotateType{ 1 }; // 바라보는 시점
static int cameraMode{ 2 }; // 1인칭(value 0), 3인칭(value 1), 3인칭(value 2), 3인칭(value 3)

GLfloat cameraMoveX{ 0 };			// 카메라 좌우 이동을 위한 변수
GLfloat cameraMoveY{ -3.5f };		// 카메라 상하 이동을 위한 변수 [기본설정 -3.5f]
GLfloat cameraMoveZ{ -4.0f };		// 카메라 앞뒤 이동을 위한 변수 [기본설정 -4.0f]
GLfloat cameraRotateY{ 0 };			// 카메라 Y축 자전[자신]
GLfloat cameraStaticRotateY{ 0 };	// 카메라 Y축 공전[(0,0,0) 기준]


void DrawStageCamera(GLuint* shaderProgram, GLfloat p_X, GLfloat p_Y, GLfloat p_Z)
{
	//--------------------------------------------------------------------------------------------------------------------------- [뷰잉 변환 시작]

	// [2. 뷰잉 변환]
	unsigned int viewLocation = glGetUniformLocation(*shaderProgram, "viewTransform"); //--- 버텍스 세이더에서 viewTransform 변수위치
	// 카메라 위치 - [nomalize 좌표]
	// 카메라 방향 : 카메라가 바라보는 방향(n) // 1. 카메라 이동★

	if (cameraMode == 0 || cameraMode == 1)
	{
		glm::vec3 cameraPos = glm::vec3(0, 0, 0); // 위치를 z방향 6만큼 이동
		// 카메라의 오른쪽 축(u) - 위쪽 벡터와 카메라 방향 벡터와의 외적

		glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, -1.0);

		// 카메라의 위쪽 축(v)
		glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

		// 뷰 전환(카메라) - 생성
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]); //--- viewTransform 변수에 변환값 적용하기

		// 1.카메라 이동★
		glm::mat4 cameraMovePoint = glm::mat4(1.0f); //--- rotation matrix
		if (cameraMode == 0)
		{
			if (cameraRotateType == 0)
				cameraDirection = glm::vec3(0.0f, 0.0f, 1.0);
			else if (cameraRotateType == 1)
				cameraDirection = glm::vec3(0.0f, 0.0f, -1.0);
			else if (cameraRotateType == 2)
				cameraDirection = glm::vec3(1.0f, 0.0f, 0.0);
			else if (cameraRotateType == 3)
				cameraDirection = glm::vec3(-1.0f, 0.0f, 0.0);

			cameraMovePoint = glm::translate(cameraMovePoint, glm::vec3(-p_X, -p_Y - 2.0f, -p_Z)); //--- 축으로 이동
		}
		else if (cameraMode == 1)
			cameraMovePoint = glm::translate(cameraMovePoint, glm::vec3(-p_X, -p_Y - 2.0f, -p_Z - 8.0f)); //--- 축으로 이동

		// 2.카메라 회전★
		//glm::mat4 cameraRotation = glm::mat4(1.0f); //--- rotation matrix
		//cameraRotation = glm::rotate(cameraRotation, glm::radians(cameraRotateY), glm::vec3(0.0, 1.0, 0.0)); //--- 축에대하여 회전
		//glm::mat4 cameraStaticRotation = glm::mat4(1.0f); //--- rotation matrix
		//cameraStaticRotation = glm::rotate(cameraStaticRotation, glm::radians(cameraStaticRotateY), glm::vec3(0.0, 1.0, 0.0)); //--- 축에대하여 회전

		// 3.카메라 변환 적용★
		view = glm::lookAt(cameraPos, cameraDirection, cameraUp) * cameraMovePoint;

		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]); //--- viewTransform 변수에 변환값 적용하기
	}
	else if (cameraMode == 2)
	{
		glm::vec3 cameraPos = glm::vec3(0.0f, 9.0f, 7.0f); // 위치를 y, z방향 13만큼 이동
		// 카메라의 오른쪽 축(u) - 위쪽 벡터와 카메라 방향 벡터와의 외적
		glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);
		// 카메라의 위쪽 축(v)
		glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

		// 뷰 전환(카메라) - 생성
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]); //--- viewTransform 변수에 변환값 적용하기

		// 1.카메라 이동★
		glm::mat4 cameraMovePoint = glm::mat4(1.0f); //--- rotation matrix
		cameraMovePoint = glm::translate(cameraMovePoint, glm::vec3(-p_X, -p_Y - 1.0f, -p_Z)); //--- 축으로 이동

		// 2.카메라 회전★
		glm::mat4 cameraRotation = glm::mat4(1.0f); //--- rotation matrix
		cameraRotation = glm::rotate(cameraRotation, glm::radians(cameraRotateY), glm::vec3(0.0, 1.0, 0.0)); //--- 축에대하여 회전
		glm::mat4 cameraStaticRotation = glm::mat4(1.0f); //--- rotation matrix
		cameraStaticRotation = glm::rotate(cameraStaticRotation, glm::radians(cameraStaticRotateY), glm::vec3(0.0, 1.0, 0.0)); //--- 축에대하여 회전

		// 3.카메라 변환 적용★
		view = glm::lookAt(cameraPos, cameraDirection, cameraUp) * cameraMovePoint;

		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]); //--- viewTransform 변수에 변환값 적용하기
	}
	else if (cameraMode == 3) // 카메라 자유시점 모드
	{
		glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 1.0f); // 위치를 y, z방향 6만큼 이동
		// 카메라의 오른쪽 축(u) - 위쪽 벡터와 카메라 방향 벡터와의 외적
		glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);
		// 카메라의 위쪽 축(v)
		glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

		// 뷰 전환(카메라) - 생성
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]); //--- viewTransform 변수에 변환값 적용하기

		// 1.카메라 이동★
		glm::mat4 cameraMovePoint = glm::mat4(1.0f); //--- rotation matrix
		cameraMovePoint = glm::translate(cameraMovePoint, glm::vec3(cameraMoveX, -1.0f, cameraMoveZ - 6.0f)); //--- 축으로 이동

		// 2.카메라 회전★
		glm::mat4 cameraRotation = glm::mat4(1.0f); //--- rotation matrix
		cameraRotation = glm::rotate(cameraRotation, glm::radians(cameraRotateY), glm::vec3(0.0, 1.0, 0.0)); //--- 축에대하여 회전
		glm::mat4 cameraStaticRotation = glm::mat4(1.0f); //--- rotation matrix
		cameraStaticRotation = glm::rotate(cameraStaticRotation, glm::radians(cameraStaticRotateY), glm::vec3(0.0, 1.0, 0.0)); //--- 축에대하여 회전

		// 3.카메라 변환 적용★
		view = cameraStaticRotation * glm::lookAt(cameraPos, cameraDirection, cameraUp) * cameraMovePoint * cameraRotation;

		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]); //--- viewTransform 변수에 변환값 적용하기
	}
	//--------------------------------------------------------------------------------------------------------------------------- [뷰잉 변환 끝]
}

void DrawMenuCamera(GLuint* shaderProgram)
{
	//--------------------------------------------------------------------------------------------------------------------------- [뷰잉 변환 시작]

	// [2. 뷰잉 변환]
	unsigned int viewLocation = glGetUniformLocation(*shaderProgram, "viewTransform"); //--- 버텍스 세이더에서 viewTransform 변수위치
	// 카메라 위치 - [nomalize 좌표]
	// 카메라 방향 : 카메라가 바라보는 방향(n) // 1. 카메라 이동★

	glm::vec3 cameraPos = glm::vec3(0, 0, 0); // 위치를 z방향 6만큼 이동
	// 카메라의 오른쪽 축(u) - 위쪽 벡터와 카메라 방향 벡터와의 외적

	glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, -1.0);

	// 카메라의 위쪽 축(v)
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	// 뷰 전환(카메라) - 생성
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]); //--- viewTransform 변수에 변환값 적용하기

	// 1.카메라 이동★
	glm::mat4 cameraMovePoint = glm::mat4(1.0f); //--- rotation matrix
	cameraMovePoint = glm::translate(cameraMovePoint, glm::vec3(0.0f, -2.0f, 0.0f)); //--- 축으로 이동

	// 3.카메라 변환 적용★
	view = glm::lookAt(cameraPos, cameraDirection, cameraUp) * cameraMovePoint;

	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]); //--- viewTransform 변수에 변환값 적용하기
}