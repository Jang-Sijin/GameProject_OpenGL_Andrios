#pragma once
#define STAGE_COLOR glm::vec3(1.0f, 1.0f, 1.0f)
#define POTAL_MAIN_TEXTURE_UNIFORM_NUM 0
#define POTAL_MASK_TEXTURE_UNIFORM_NUM 1
#define PORTAL_SIZE				0.5
#include "Potal.h"
#include "stb_image.h"

void POTAL::InitBuffer()
{
	// [Stage 6면 Const]
	//--- 타일데이터: [VAO 1개, VBO 1개를 사용하여 구현]
	glGenVertexArrays(3, this->vaoPotal); //--- VAO 를 지정하고 할당하기
	glGenBuffers(9, this->vboPotal); //--- 12개의 VBO를 지정하고 할당하기
	glGenBuffers(3, this->eboPotal); //--- 2개의 EBO를 지정하고 할당하기

	//--- 1번째 VBO를 활성화하여 바인드하고, [버텍스 속성] (좌표값)을 저장
	for (int i = 0; i < 3; ++i)
	{
		glBindVertexArray(this->vaoPotal[i]); //--- VAO를 바인드하기

		glBindBuffer(GL_ARRAY_BUFFER, this->vboPotal[i * 3]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vStagePositionList), vStagePositionList, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		//--- 1번째 VBO를 활성화하여 바인드하고, [컬러 속성]을 저장
		glBindBuffer(GL_ARRAY_BUFFER, this->vboPotal[(i * 3) + 1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vPotalColor), vPotalColor, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);


		glBindBuffer(GL_ARRAY_BUFFER, this->vboPotal[(i * 3) + 2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vPotalTex), vPotalTex, GL_STATIC_DRAW);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->eboPotal[i]); // GL_ELEMENT_ARRAY_BUFFER 버퍼 유형으로 바인딩	
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vPotalIndex), vPotalIndex, GL_STATIC_DRAW);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(GLfloat)));
	}
}

void POTAL::InitTexture(GLuint* shaderProgram)
{
	int width;
	int height;
	int channelNum;
	unsigned char* data = { nullptr };

	stbi_set_flip_vertically_on_load(true);

	glGenTextures(1, &mainPotalTexture);
	glBindTexture(GL_TEXTURE_2D, mainPotalTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	data = stbi_load("./IMAGE/PotalMain.jpg", &width, &height, &channelNum, 0);

	// std::cout << width << "," << height << std::endl;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	glUseProgram(*shaderProgram);
	int tLocation = glGetUniformLocation(*shaderProgram, "outTexture"); //--- outTexture 유니폼 샘플러의 위치를 가져옴
	glUniform1i(tLocation, POTAL_MAIN_TEXTURE_UNIFORM_NUM); //--- 샘플러를 0번 유닛으로 설정

	//--

	glGenTextures(1, &backPotalTexture);
	glBindTexture(GL_TEXTURE_2D, backPotalTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	data = stbi_load("./IMAGE/Potal_Back_texture.jpg", &width, &height, &channelNum, 0);

	// std::cout << width << "," << height << std::endl;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	glUseProgram(*shaderProgram);
	tLocation = glGetUniformLocation(*shaderProgram, "outTexture"); //--- outTexture 유니폼 샘플러의 위치를 가져옴
	glUniform1i(tLocation, POTAL_MASK_TEXTURE_UNIFORM_NUM); //--- 샘플러를 1번 유닛으로 설정
}

void POTAL::DrawScene(GLuint* shaderProgram, unsigned int* modelLocation, glm::mat4* WorldPosition, unsigned int* objColorLocation, glm::vec3* objColor) // 정보 그리기
{
	glDisable(GL_CULL_FACE);
	glFrontFace(GL_CCW); // 여기서 부턴 시계 방향을 앞면으로 인식한다.
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	unsigned int tLocation = glGetUniformLocation(*shaderProgram, "outTexture");
	glUniform1i(tLocation, POTAL_MAIN_TEXTURE_UNIFORM_NUM);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mainPotalTexture);

	// 기본 월드 좌표계
	glm::mat4 Stage_MainPosition = glm::mat4(1.0f); //--- position matrix
	Stage_MainPosition = glm::translate(Stage_MainPosition, glm::vec3(0.0f, 0.0f, 0.0f)); //--- 축으로 이동

	//	오른쪽 포탈 // 스테이지 위치 설정
	glm::mat4 PotalPosition_1 = glm::mat4(1.0f); //--- position matrix
	PotalPosition_1 = glm::translate(PotalPosition_1, glm::vec3(2.0 + this->potalMove, 0, -9.9)); //--- 축으로 이동

	*WorldPosition = Stage_MainPosition * PotalPosition_1;// * PotalRotation_1;
	glUniform3fv(*objColorLocation, 1, (float*) & (*objColor));
	glUniformMatrix4fv(*modelLocation, 1, GL_FALSE, glm::value_ptr(*WorldPosition)); //--- modelTransform 변수에 변환 값 적용하기
	*objColor = STAGE_COLOR;
	glBindVertexArray(vaoPotal[0]); // 사용할 VAO 불러오기
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 3꼭지점 * 2개삼각형(1사각형) * 6면


	//	왼쪽 포탈 // 스테이지 위치 설정
	glm::mat4 PotalPosition_2 = glm::mat4(1.0f); //--- position matrix
	PotalPosition_2 = glm::translate(PotalPosition_2, glm::vec3(-2.0 - this->potalMove, 0.0, -9.9)); //--- 축으로 이동
	glm::mat4 PotalRotation_2 = glm::mat4(1.0f); //--- position matrix
	PotalRotation_2 = glm::rotate(PotalRotation_2, glm::radians(-180.0f), glm::vec3(0.0, 1.0, 0.0)); //--- 축에대하여 회전

	*WorldPosition = Stage_MainPosition * PotalPosition_2 * PotalRotation_2;// * PotalRotation_1;
	glUniform3fv(*objColorLocation, 1, (float*) & (*objColor));
	glUniformMatrix4fv(*modelLocation, 1, GL_FALSE, glm::value_ptr(*WorldPosition)); //--- modelTransform 변수에 변환 값 적용하기
	*objColor = STAGE_COLOR;
	glBindVertexArray(vaoPotal[1]); // 사용할 VAO 불러오기
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 3꼭지점 * 2개삼각형(1사각형) * 6면


	glUniform1i(tLocation, POTAL_MAIN_TEXTURE_UNIFORM_NUM);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, backPotalTexture);

	// 중앙 포탈 // 스테이지 위치 설정
	glm::mat4 PotalPosition_3 = glm::mat4(1.0f); //--- position matrix
	PotalPosition_3 = glm::translate(PotalPosition_3, glm::vec3(0.0, 0.0, -9.9)); //--- 축으로 이동

	*WorldPosition = Stage_MainPosition * PotalPosition_3;// * PotalRotation_1;
	glUniform3fv(*objColorLocation, 1, (float*) & (*objColor));
	glUniformMatrix4fv(*modelLocation, 1, GL_FALSE, glm::value_ptr(*WorldPosition)); //--- modelTransform 변수에 변환 값 적용하기
	*objColor = STAGE_COLOR;
	glBindVertexArray(vaoPotal[2]); // 사용할 VAO 불러오기
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 3꼭지점 * 2개삼각형(1사각형) * 6면

	glEnable(GL_CULL_FACE);
}

void POTAL::PotalTimerFunction()
{
	if (this->potalMove <= 2.0f)
	{
		this->potalMove += 0.01f;
	}
}

void POTAL::ReturnSetData()
{
	this->potalMove = 0;
	this->bAngleMode = false; // flase: +, true: -
}