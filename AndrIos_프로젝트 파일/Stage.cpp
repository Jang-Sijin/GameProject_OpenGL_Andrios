#pragma once
#define STAGE_COLOR glm::vec3(1.0f, 1.0f, 1.0f)
#define STAGE_TEXTURE_UNIFORM_NUM 0
#include "Stage.h"

void STAGE::InitBuffer()
{
	// [Stage 6면 Const]
	//--- 타일데이터: [VAO 1개, VBO 1개를 사용하여 구현]
	glGenVertexArrays(6, this->vaoStage); //--- VAO 를 지정하고 할당하기
	glGenBuffers(18, this->vboStage); //--- 12개의 VBO를 지정하고 할당하기
	glGenBuffers(6, this->eboStage); //--- 2개의 EBO를 지정하고 할당하기

	//--- 1번째 VBO를 활성화하여 바인드하고, [버텍스 속성] (좌표값)을 저장
	for (int i = 0; i < 6; ++i)
	{
		glBindVertexArray(this->vaoStage[i]); //--- VAO를 바인드하기

		glBindBuffer(GL_ARRAY_BUFFER, this->vboStage[i * 3]);
		if (i < 4)
			glBufferData(GL_ARRAY_BUFFER, sizeof(vStagePositionList), vStagePositionList, GL_STATIC_DRAW);
		else
			glBufferData(GL_ARRAY_BUFFER, sizeof(vStagePositionList_TopBottom), vStagePositionList_TopBottom, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		//--- 1번째 VBO를 활성화하여 바인드하고, [컬러 속성]을 저장
		glBindBuffer(GL_ARRAY_BUFFER, this->vboStage[(i * 3) + 1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vStageColor), vStageColor, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);


		glBindBuffer(GL_ARRAY_BUFFER, this->vboStage[(i * 3) + 2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vStageTex), vStageTex, GL_STATIC_DRAW);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->eboStage[i]); // GL_ELEMENT_ARRAY_BUFFER 버퍼 유형으로 바인딩	
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(stageIndex), stageIndex, GL_STATIC_DRAW);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(GLfloat)));
	}
}

void STAGE::InitTexture(GLuint* shaderProgram)
{
	int width;
	int height;
	int channelNum;
	unsigned char* data = { nullptr };

	stbi_set_flip_vertically_on_load(true);

	glGenTextures(1, &backGroundTexture);
	glBindTexture(GL_TEXTURE_2D, backGroundTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	data = stbi_load("./IMAGE/BackGround.jpg", &width, &height, &channelNum, 0);

	//std::cout << width << "," << height << endl;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	glUseProgram(*shaderProgram);
	int tLocation = glGetUniformLocation(*shaderProgram, "outTexture"); //--- outTexture 유니폼 샘플러의 위치를 가져옴
	glUniform1i(tLocation, STAGE_TEXTURE_UNIFORM_NUM); //--- 샘플러를 0번 유닛으로 설정
}

void STAGE::DrawScene(GLuint* shaderProgram, unsigned int* modelLocation, glm::mat4* WorldPosition, unsigned int* objColorLocation, glm::vec3* objColor) // 정보 그리기
{
	glFrontFace(GL_CW); // 여기서 부턴 시계 방향을 앞면으로 인식한다.
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	unsigned int tLocation = glGetUniformLocation(*shaderProgram, "outTexture");
	glUniform1i(tLocation, STAGE_TEXTURE_UNIFORM_NUM);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, backGroundTexture);

	// 기본 월드 좌표계
	glm::mat4 Stage_MainPosition = glm::mat4(1.0f); //--- position matrix
	Stage_MainPosition = glm::translate(Stage_MainPosition, glm::vec3(0.0f, 0.0f, 0.0f)); //--- 축으로 이동

	// 앞쪽 벽 // 스테이지 위치 설정
	glm::mat4 StagePosition_1 = glm::mat4(1.0f); //--- position matrix
	StagePosition_1 = glm::translate(StagePosition_1, glm::vec3(0, 0, stageSize)); //--- 축으로 이동
	glm::mat4 StageRotation_1 = glm::mat4(1.0f); //--- position matrix
	StageRotation_1 = glm::rotate(StageRotation_1, glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0)); //--- 축에대하여 회전

	*WorldPosition = Stage_MainPosition * StagePosition_1 * StageRotation_1;
	glUniform3fv(*objColorLocation, 1, (float*) & (*objColor));
	glUniformMatrix4fv(*modelLocation, 1, GL_FALSE, glm::value_ptr(*WorldPosition)); //--- modelTransform 변수에 변환 값 적용하기
	*objColor = STAGE_COLOR;
	glBindVertexArray(vaoStage[0]); // 사용할 VAO 불러오기
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 3꼭지점 * 2개삼각형(1사각형) * 6면


	// 뒷쪽 벽 // 스테이지 위치 설정
	glm::mat4 StagePosition_2 = glm::mat4(1.0f); //--- position matrix
	StagePosition_2 = glm::translate(StagePosition_2, glm::vec3(0, 0, -stageSize)); //--- 축으로 이동
	glm::mat4 StageRotation_2 = glm::mat4(1.0f); //--- position matrix
	StageRotation_2 = glm::rotate(StageRotation_2, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0)); //--- 축에대하여 회전

	*WorldPosition = Stage_MainPosition * StagePosition_2 * StageRotation_2;

	glUniform3fv(*objColorLocation, 1, (float*) & (*objColor));
	glUniformMatrix4fv(*modelLocation, 1, GL_FALSE, glm::value_ptr(*WorldPosition)); //--- modelTransform 변수에 변환 값 적용하기
	*objColor = STAGE_COLOR;
	glBindVertexArray(vaoStage[1]); // 사용할 VAO 불러오기
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 3꼭지점 * 2개삼각형(1사각형) * 6면


	// 왼쪽 벽 // 스테이지 위치 설정
	glm::mat4 StagePosition_3 = glm::mat4(1.0f); //--- position matrix
	StagePosition_3 = glm::translate(StagePosition_3, glm::vec3(-stageSize, 0, 0)); //--- 축으로 이동
	glm::mat4 StageRotation_3 = glm::mat4(1.0f); //--- position matrix
	StageRotation_3 = glm::rotate(StageRotation_3, glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0)); //--- 축에대하여 회전

	*WorldPosition = Stage_MainPosition * StagePosition_3 * StageRotation_3;

	glUniform3fv(*objColorLocation, 1, (float*) & (*objColor));
	glUniformMatrix4fv(*modelLocation, 1, GL_FALSE, glm::value_ptr(*WorldPosition)); //--- modelTransform 변수에 변환 값 적용하기
	*objColor = STAGE_COLOR;
	glBindVertexArray(vaoStage[2]); // 사용할 VAO 불러오기
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 3꼭지점 * 2개삼각형(1사각형) * 6면


	// 오른쪽 벽 // 스테이지 위치 설정
	glm::mat4 StagePosition_4 = glm::mat4(1.0f); //--- position matrix
	StagePosition_4 = glm::translate(StagePosition_4, glm::vec3(stageSize, 0, 0)); //--- 축으로 이동
	glm::mat4 StageRotation_4 = glm::mat4(1.0f); //--- position matrix
	StageRotation_4 = glm::rotate(StageRotation_4, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0)); //--- 축에대하여 회전

	*WorldPosition = Stage_MainPosition * StagePosition_4 * StageRotation_4;

	glUniform3fv(*objColorLocation, 1, (float*) & (*objColor));
	glUniformMatrix4fv(*modelLocation, 1, GL_FALSE, glm::value_ptr(*WorldPosition)); //--- modelTransform 변수에 변환 값 적용하기
	*objColor = STAGE_COLOR;
	glBindVertexArray(vaoStage[3]); // 사용할 VAO 불러오기
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 3꼭지점 * 2개삼각형(1사각형) * 6면


	// 바닥 벽 // 스테이지 위치 설정
	glm::mat4 StagePosition_5 = glm::mat4(1.0f); //--- position matrix
	StagePosition_5 = glm::translate(StagePosition_5, glm::vec3(0, 0, stageSize)); //--- 축으로 이동
	glm::mat4 StageRotation_5 = glm::mat4(1.0f); //--- position matrix
	StageRotation_5 = glm::rotate(StageRotation_5, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0)); //--- 축에대하여 회전
	glm::mat4 StageRotation_5_1 = glm::mat4(1.0f); //--- position matrix
	StageRotation_5_1 = glm::rotate(StageRotation_5_1, glm::radians(180.0f), glm::vec3(0.0, 0.0, 1.0)); //--- 축에대하여 회전

	*WorldPosition = Stage_MainPosition * StagePosition_5 * StageRotation_5 * StageRotation_5_1;

	glUniform3fv(*objColorLocation, 1, (float*) & (*objColor));
	glUniformMatrix4fv(*modelLocation, 1, GL_FALSE, glm::value_ptr(*WorldPosition)); //--- modelTransform 변수에 변환 값 적용하기
	*objColor = STAGE_COLOR;
	glBindVertexArray(vaoStage[4]); // 사용할 VAO 불러오기
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 3꼭지점 * 2개삼각형(1사각형) * 6면


	// 천장 벽 // 스테이지 위치 설정
	glm::mat4 StagePosition_6 = glm::mat4(1.0f); //--- position matrix
	StagePosition_6 = glm::translate(StagePosition_6, glm::vec3(0, stageHeight, stageSize)); //--- 축으로 이동
	glm::mat4 StageRotation_6 = glm::mat4(1.0f); //--- position matrix
	StageRotation_6 = glm::rotate(StageRotation_6, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0)); //--- 축에대하여 회전

	*WorldPosition = Stage_MainPosition * StagePosition_6 * StageRotation_6;

	glUniform3fv(*objColorLocation, 1, (float*) & (*objColor));
	glUniformMatrix4fv(*modelLocation, 1, GL_FALSE, glm::value_ptr(*WorldPosition)); //--- modelTransform 변수에 변환 값 적용하기
	*objColor = STAGE_COLOR;
	glBindVertexArray(vaoStage[5]); // 사용할 VAO 불러오기
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 3꼭지점 * 2개삼각형(1사각형) * 6면
}