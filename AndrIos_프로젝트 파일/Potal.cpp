#pragma once
#define STAGE_COLOR glm::vec3(1.0f, 1.0f, 1.0f)
#define POTAL_MAIN_TEXTURE_UNIFORM_NUM 0
#define POTAL_MASK_TEXTURE_UNIFORM_NUM 1
#define PORTAL_SIZE				0.5
#include "Potal.h"
#include "stb_image.h"

void POTAL::InitBuffer()
{
	// [Stage 6�� Const]
	//--- Ÿ�ϵ�����: [VAO 1��, VBO 1���� ����Ͽ� ����]
	glGenVertexArrays(3, this->vaoPotal); //--- VAO �� �����ϰ� �Ҵ��ϱ�
	glGenBuffers(9, this->vboPotal); //--- 12���� VBO�� �����ϰ� �Ҵ��ϱ�
	glGenBuffers(3, this->eboPotal); //--- 2���� EBO�� �����ϰ� �Ҵ��ϱ�

	//--- 1��° VBO�� Ȱ��ȭ�Ͽ� ���ε��ϰ�, [���ؽ� �Ӽ�] (��ǥ��)�� ����
	for (int i = 0; i < 3; ++i)
	{
		glBindVertexArray(this->vaoPotal[i]); //--- VAO�� ���ε��ϱ�

		glBindBuffer(GL_ARRAY_BUFFER, this->vboPotal[i * 3]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vStagePositionList), vStagePositionList, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		//--- 1��° VBO�� Ȱ��ȭ�Ͽ� ���ε��ϰ�, [�÷� �Ӽ�]�� ����
		glBindBuffer(GL_ARRAY_BUFFER, this->vboPotal[(i * 3) + 1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vPotalColor), vPotalColor, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);


		glBindBuffer(GL_ARRAY_BUFFER, this->vboPotal[(i * 3) + 2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vPotalTex), vPotalTex, GL_STATIC_DRAW);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->eboPotal[i]); // GL_ELEMENT_ARRAY_BUFFER ���� �������� ���ε�	
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
	int tLocation = glGetUniformLocation(*shaderProgram, "outTexture"); //--- outTexture ������ ���÷��� ��ġ�� ������
	glUniform1i(tLocation, POTAL_MAIN_TEXTURE_UNIFORM_NUM); //--- ���÷��� 0�� �������� ����

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
	tLocation = glGetUniformLocation(*shaderProgram, "outTexture"); //--- outTexture ������ ���÷��� ��ġ�� ������
	glUniform1i(tLocation, POTAL_MASK_TEXTURE_UNIFORM_NUM); //--- ���÷��� 1�� �������� ����
}

void POTAL::DrawScene(GLuint* shaderProgram, unsigned int* modelLocation, glm::mat4* WorldPosition, unsigned int* objColorLocation, glm::vec3* objColor) // ���� �׸���
{
	glDisable(GL_CULL_FACE);
	glFrontFace(GL_CCW); // ���⼭ ���� �ð� ������ �ո����� �ν��Ѵ�.
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	unsigned int tLocation = glGetUniformLocation(*shaderProgram, "outTexture");
	glUniform1i(tLocation, POTAL_MAIN_TEXTURE_UNIFORM_NUM);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mainPotalTexture);

	// �⺻ ���� ��ǥ��
	glm::mat4 Stage_MainPosition = glm::mat4(1.0f); //--- position matrix
	Stage_MainPosition = glm::translate(Stage_MainPosition, glm::vec3(0.0f, 0.0f, 0.0f)); //--- ������ �̵�

	//	������ ��Ż // �������� ��ġ ����
	glm::mat4 PotalPosition_1 = glm::mat4(1.0f); //--- position matrix
	PotalPosition_1 = glm::translate(PotalPosition_1, glm::vec3(2.0 + this->potalMove, 0, -9.9)); //--- ������ �̵�

	*WorldPosition = Stage_MainPosition * PotalPosition_1;// * PotalRotation_1;
	glUniform3fv(*objColorLocation, 1, (float*) & (*objColor));
	glUniformMatrix4fv(*modelLocation, 1, GL_FALSE, glm::value_ptr(*WorldPosition)); //--- modelTransform ������ ��ȯ �� �����ϱ�
	*objColor = STAGE_COLOR;
	glBindVertexArray(vaoPotal[0]); // ����� VAO �ҷ�����
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 3������ * 2���ﰢ��(1�簢��) * 6��


	//	���� ��Ż // �������� ��ġ ����
	glm::mat4 PotalPosition_2 = glm::mat4(1.0f); //--- position matrix
	PotalPosition_2 = glm::translate(PotalPosition_2, glm::vec3(-2.0 - this->potalMove, 0.0, -9.9)); //--- ������ �̵�
	glm::mat4 PotalRotation_2 = glm::mat4(1.0f); //--- position matrix
	PotalRotation_2 = glm::rotate(PotalRotation_2, glm::radians(-180.0f), glm::vec3(0.0, 1.0, 0.0)); //--- �࿡���Ͽ� ȸ��

	*WorldPosition = Stage_MainPosition * PotalPosition_2 * PotalRotation_2;// * PotalRotation_1;
	glUniform3fv(*objColorLocation, 1, (float*) & (*objColor));
	glUniformMatrix4fv(*modelLocation, 1, GL_FALSE, glm::value_ptr(*WorldPosition)); //--- modelTransform ������ ��ȯ �� �����ϱ�
	*objColor = STAGE_COLOR;
	glBindVertexArray(vaoPotal[1]); // ����� VAO �ҷ�����
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 3������ * 2���ﰢ��(1�簢��) * 6��


	glUniform1i(tLocation, POTAL_MAIN_TEXTURE_UNIFORM_NUM);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, backPotalTexture);

	// �߾� ��Ż // �������� ��ġ ����
	glm::mat4 PotalPosition_3 = glm::mat4(1.0f); //--- position matrix
	PotalPosition_3 = glm::translate(PotalPosition_3, glm::vec3(0.0, 0.0, -9.9)); //--- ������ �̵�

	*WorldPosition = Stage_MainPosition * PotalPosition_3;// * PotalRotation_1;
	glUniform3fv(*objColorLocation, 1, (float*) & (*objColor));
	glUniformMatrix4fv(*modelLocation, 1, GL_FALSE, glm::value_ptr(*WorldPosition)); //--- modelTransform ������ ��ȯ �� �����ϱ�
	*objColor = STAGE_COLOR;
	glBindVertexArray(vaoPotal[2]); // ����� VAO �ҷ�����
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 3������ * 2���ﰢ��(1�簢��) * 6��

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