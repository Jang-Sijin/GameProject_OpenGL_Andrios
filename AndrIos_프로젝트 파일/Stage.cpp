#pragma once
#define STAGE_COLOR glm::vec3(1.0f, 1.0f, 1.0f)
#define STAGE_TEXTURE_UNIFORM_NUM 0
#include "Stage.h"

void STAGE::InitBuffer()
{
	// [Stage 6�� Const]
	//--- Ÿ�ϵ�����: [VAO 1��, VBO 1���� ����Ͽ� ����]
	glGenVertexArrays(6, this->vaoStage); //--- VAO �� �����ϰ� �Ҵ��ϱ�
	glGenBuffers(18, this->vboStage); //--- 12���� VBO�� �����ϰ� �Ҵ��ϱ�
	glGenBuffers(6, this->eboStage); //--- 2���� EBO�� �����ϰ� �Ҵ��ϱ�

	//--- 1��° VBO�� Ȱ��ȭ�Ͽ� ���ε��ϰ�, [���ؽ� �Ӽ�] (��ǥ��)�� ����
	for (int i = 0; i < 6; ++i)
	{
		glBindVertexArray(this->vaoStage[i]); //--- VAO�� ���ε��ϱ�

		glBindBuffer(GL_ARRAY_BUFFER, this->vboStage[i * 3]);
		if (i < 4)
			glBufferData(GL_ARRAY_BUFFER, sizeof(vStagePositionList), vStagePositionList, GL_STATIC_DRAW);
		else
			glBufferData(GL_ARRAY_BUFFER, sizeof(vStagePositionList_TopBottom), vStagePositionList_TopBottom, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		//--- 1��° VBO�� Ȱ��ȭ�Ͽ� ���ε��ϰ�, [�÷� �Ӽ�]�� ����
		glBindBuffer(GL_ARRAY_BUFFER, this->vboStage[(i * 3) + 1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vStageColor), vStageColor, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);


		glBindBuffer(GL_ARRAY_BUFFER, this->vboStage[(i * 3) + 2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vStageTex), vStageTex, GL_STATIC_DRAW);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->eboStage[i]); // GL_ELEMENT_ARRAY_BUFFER ���� �������� ���ε�	
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
	int tLocation = glGetUniformLocation(*shaderProgram, "outTexture"); //--- outTexture ������ ���÷��� ��ġ�� ������
	glUniform1i(tLocation, STAGE_TEXTURE_UNIFORM_NUM); //--- ���÷��� 0�� �������� ����
}

void STAGE::DrawScene(GLuint* shaderProgram, unsigned int* modelLocation, glm::mat4* WorldPosition, unsigned int* objColorLocation, glm::vec3* objColor) // ���� �׸���
{
	glFrontFace(GL_CW); // ���⼭ ���� �ð� ������ �ո����� �ν��Ѵ�.
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	unsigned int tLocation = glGetUniformLocation(*shaderProgram, "outTexture");
	glUniform1i(tLocation, STAGE_TEXTURE_UNIFORM_NUM);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, backGroundTexture);

	// �⺻ ���� ��ǥ��
	glm::mat4 Stage_MainPosition = glm::mat4(1.0f); //--- position matrix
	Stage_MainPosition = glm::translate(Stage_MainPosition, glm::vec3(0.0f, 0.0f, 0.0f)); //--- ������ �̵�

	// ���� �� // �������� ��ġ ����
	glm::mat4 StagePosition_1 = glm::mat4(1.0f); //--- position matrix
	StagePosition_1 = glm::translate(StagePosition_1, glm::vec3(0, 0, stageSize)); //--- ������ �̵�
	glm::mat4 StageRotation_1 = glm::mat4(1.0f); //--- position matrix
	StageRotation_1 = glm::rotate(StageRotation_1, glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0)); //--- �࿡���Ͽ� ȸ��

	*WorldPosition = Stage_MainPosition * StagePosition_1 * StageRotation_1;
	glUniform3fv(*objColorLocation, 1, (float*) & (*objColor));
	glUniformMatrix4fv(*modelLocation, 1, GL_FALSE, glm::value_ptr(*WorldPosition)); //--- modelTransform ������ ��ȯ �� �����ϱ�
	*objColor = STAGE_COLOR;
	glBindVertexArray(vaoStage[0]); // ����� VAO �ҷ�����
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 3������ * 2���ﰢ��(1�簢��) * 6��


	// ���� �� // �������� ��ġ ����
	glm::mat4 StagePosition_2 = glm::mat4(1.0f); //--- position matrix
	StagePosition_2 = glm::translate(StagePosition_2, glm::vec3(0, 0, -stageSize)); //--- ������ �̵�
	glm::mat4 StageRotation_2 = glm::mat4(1.0f); //--- position matrix
	StageRotation_2 = glm::rotate(StageRotation_2, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0)); //--- �࿡���Ͽ� ȸ��

	*WorldPosition = Stage_MainPosition * StagePosition_2 * StageRotation_2;

	glUniform3fv(*objColorLocation, 1, (float*) & (*objColor));
	glUniformMatrix4fv(*modelLocation, 1, GL_FALSE, glm::value_ptr(*WorldPosition)); //--- modelTransform ������ ��ȯ �� �����ϱ�
	*objColor = STAGE_COLOR;
	glBindVertexArray(vaoStage[1]); // ����� VAO �ҷ�����
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 3������ * 2���ﰢ��(1�簢��) * 6��


	// ���� �� // �������� ��ġ ����
	glm::mat4 StagePosition_3 = glm::mat4(1.0f); //--- position matrix
	StagePosition_3 = glm::translate(StagePosition_3, glm::vec3(-stageSize, 0, 0)); //--- ������ �̵�
	glm::mat4 StageRotation_3 = glm::mat4(1.0f); //--- position matrix
	StageRotation_3 = glm::rotate(StageRotation_3, glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0)); //--- �࿡���Ͽ� ȸ��

	*WorldPosition = Stage_MainPosition * StagePosition_3 * StageRotation_3;

	glUniform3fv(*objColorLocation, 1, (float*) & (*objColor));
	glUniformMatrix4fv(*modelLocation, 1, GL_FALSE, glm::value_ptr(*WorldPosition)); //--- modelTransform ������ ��ȯ �� �����ϱ�
	*objColor = STAGE_COLOR;
	glBindVertexArray(vaoStage[2]); // ����� VAO �ҷ�����
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 3������ * 2���ﰢ��(1�簢��) * 6��


	// ������ �� // �������� ��ġ ����
	glm::mat4 StagePosition_4 = glm::mat4(1.0f); //--- position matrix
	StagePosition_4 = glm::translate(StagePosition_4, glm::vec3(stageSize, 0, 0)); //--- ������ �̵�
	glm::mat4 StageRotation_4 = glm::mat4(1.0f); //--- position matrix
	StageRotation_4 = glm::rotate(StageRotation_4, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0)); //--- �࿡���Ͽ� ȸ��

	*WorldPosition = Stage_MainPosition * StagePosition_4 * StageRotation_4;

	glUniform3fv(*objColorLocation, 1, (float*) & (*objColor));
	glUniformMatrix4fv(*modelLocation, 1, GL_FALSE, glm::value_ptr(*WorldPosition)); //--- modelTransform ������ ��ȯ �� �����ϱ�
	*objColor = STAGE_COLOR;
	glBindVertexArray(vaoStage[3]); // ����� VAO �ҷ�����
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 3������ * 2���ﰢ��(1�簢��) * 6��


	// �ٴ� �� // �������� ��ġ ����
	glm::mat4 StagePosition_5 = glm::mat4(1.0f); //--- position matrix
	StagePosition_5 = glm::translate(StagePosition_5, glm::vec3(0, 0, stageSize)); //--- ������ �̵�
	glm::mat4 StageRotation_5 = glm::mat4(1.0f); //--- position matrix
	StageRotation_5 = glm::rotate(StageRotation_5, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0)); //--- �࿡���Ͽ� ȸ��
	glm::mat4 StageRotation_5_1 = glm::mat4(1.0f); //--- position matrix
	StageRotation_5_1 = glm::rotate(StageRotation_5_1, glm::radians(180.0f), glm::vec3(0.0, 0.0, 1.0)); //--- �࿡���Ͽ� ȸ��

	*WorldPosition = Stage_MainPosition * StagePosition_5 * StageRotation_5 * StageRotation_5_1;

	glUniform3fv(*objColorLocation, 1, (float*) & (*objColor));
	glUniformMatrix4fv(*modelLocation, 1, GL_FALSE, glm::value_ptr(*WorldPosition)); //--- modelTransform ������ ��ȯ �� �����ϱ�
	*objColor = STAGE_COLOR;
	glBindVertexArray(vaoStage[4]); // ����� VAO �ҷ�����
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 3������ * 2���ﰢ��(1�簢��) * 6��


	// õ�� �� // �������� ��ġ ����
	glm::mat4 StagePosition_6 = glm::mat4(1.0f); //--- position matrix
	StagePosition_6 = glm::translate(StagePosition_6, glm::vec3(0, stageHeight, stageSize)); //--- ������ �̵�
	glm::mat4 StageRotation_6 = glm::mat4(1.0f); //--- position matrix
	StageRotation_6 = glm::rotate(StageRotation_6, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0)); //--- �࿡���Ͽ� ȸ��

	*WorldPosition = Stage_MainPosition * StagePosition_6 * StageRotation_6;

	glUniform3fv(*objColorLocation, 1, (float*) & (*objColor));
	glUniformMatrix4fv(*modelLocation, 1, GL_FALSE, glm::value_ptr(*WorldPosition)); //--- modelTransform ������ ��ȯ �� �����ϱ�
	*objColor = STAGE_COLOR;
	glBindVertexArray(vaoStage[5]); // ����� VAO �ҷ�����
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 3������ * 2���ﰢ��(1�簢��) * 6��
}