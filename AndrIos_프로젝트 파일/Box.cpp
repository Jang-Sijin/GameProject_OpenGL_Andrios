#pragma once
#define STAGE_COLOR glm::vec3(1.0f, 1.0f, 1.0f)
#define STAGE_TEXTURE_UNIFORM_NUM 0
#include "Box.h"
#include "stb_image.h"

void BOX::InitBuffer()
{
	// [Box 6�� Const]
	//--- Ÿ�ϵ�����: [VAO 1��, VBO 1���� ����Ͽ� ����]
	glGenVertexArrays(6, this->vaoBox); //--- VAO �� �����ϰ� �Ҵ��ϱ�
	glGenBuffers(18, this->vboBox); //--- 12���� VBO�� �����ϰ� �Ҵ��ϱ�
	glGenBuffers(6, this->eboBox); //--- 2���� EBO�� �����ϰ� �Ҵ��ϱ�

	//--- 1��° VBO�� Ȱ��ȭ�Ͽ� ���ε��ϰ�, [���ؽ� �Ӽ�] (��ǥ��)�� ����
	for (int i = 0; i < 6; ++i)
	{
		glBindVertexArray(this->vaoBox[i]); //--- VAO�� ���ε��ϱ�

		glBindBuffer(GL_ARRAY_BUFFER, this->vboBox[i * 3]);
		if (i < 4)
			glBufferData(GL_ARRAY_BUFFER, sizeof(vBoxPositionList), vBoxPositionList, GL_STATIC_DRAW);
		else
			glBufferData(GL_ARRAY_BUFFER, sizeof(vBoxPositionList_TopBottom), vBoxPositionList_TopBottom, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		//--- 1��° VBO�� Ȱ��ȭ�Ͽ� ���ε��ϰ�, [�÷� �Ӽ�]�� ����
		glBindBuffer(GL_ARRAY_BUFFER, this->vboBox[(i * 3) + 1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vBoxColor), vBoxColor, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);


		glBindBuffer(GL_ARRAY_BUFFER, this->vboBox[(i * 3) + 2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vBoxTex), vBoxTex, GL_STATIC_DRAW);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->eboBox[i]); // GL_ELEMENT_ARRAY_BUFFER ���� �������� ���ε�	
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(boxIndex), boxIndex, GL_STATIC_DRAW);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(GLfloat)));
	}
}

void BOX::InitTexture(GLuint* shaderProgram)
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
	data = stbi_load("./IMAGE/Box_texture.jpg", &width, &height, &channelNum, 0);

	//std::cout << width << "," << height << endl;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	glUseProgram(*shaderProgram);
	int tLocation = glGetUniformLocation(*shaderProgram, "outTexture"); //--- outTexture ������ ���÷��� ��ġ�� ������
	glUniform1i(tLocation, STAGE_TEXTURE_UNIFORM_NUM); //--- ���÷��� 0�� �������� ����
}

void BOX::DrawScene(GLuint* shaderProgram, unsigned int* modelLocation, glm::mat4* WorldPosition, unsigned int* objColorLocation, glm::vec3* objColor) // ���� �׸���
{
	glFrontFace(GL_CCW); // ���⼭ ���� �ð� ������ �ո����� �ν��Ѵ�.
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_CULL_FACE); // �ø� �˻�: �ĸ��� ����(backface culling)�Ͽ� �޸��� ��� ������ �� �ִ�.

	unsigned int tLocation = glGetUniformLocation(*shaderProgram, "outTexture");
	glUniform1i(tLocation, STAGE_TEXTURE_UNIFORM_NUM);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, backGroundTexture);

	// �⺻ ���� ��ǥ��
	glm::mat4 Box_MainPosition = glm::mat4(1.0f); //--- position matrix
	Box_MainPosition = glm::translate(Box_MainPosition, glm::vec3(0.0f, 10.0f - this->boxMoveY, -3.0f)); //--- ������ �̵�
	glm::mat4 Box_MainRotation_1 = glm::mat4(1.0f); //--- position matrix
	Box_MainRotation_1 = glm::rotate(Box_MainRotation_1, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0)); //--- �࿡���Ͽ� ȸ��


	// ���� �� // �������� ��ġ ����
	glm::mat4 BoxPosition_1 = glm::mat4(1.0f); //--- position matrix
	BoxPosition_1 = glm::translate(BoxPosition_1, glm::vec3(0, 0, boxSize)); //--- ������ �̵�
	glm::mat4 BoxRotation_1 = glm::mat4(1.0f); //--- position matrix
	BoxRotation_1 = glm::rotate(BoxRotation_1, glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0)); //--- �࿡���Ͽ� ȸ��

	*WorldPosition = Box_MainPosition * Box_MainRotation_1 * BoxPosition_1 * BoxRotation_1;
	glUniform3fv(*objColorLocation, 1, (float*) & (*objColor));
	glUniformMatrix4fv(*modelLocation, 1, GL_FALSE, glm::value_ptr(*WorldPosition)); //--- modelTransform ������ ��ȯ �� �����ϱ�
	*objColor = STAGE_COLOR;
	glBindVertexArray(vaoBox[0]); // ����� VAO �ҷ�����
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 3������ * 2���ﰢ��(1�簢��) * 6��


	// ���� �� // �������� ��ġ ����
	glm::mat4 BoxPosition_2 = glm::mat4(1.0f); //--- position matrix
	BoxPosition_2 = glm::translate(BoxPosition_2, glm::vec3(0, 0, -boxSize)); //--- ������ �̵�
	glm::mat4 BoxRotation_2 = glm::mat4(1.0f); //--- position matrix
	BoxRotation_2 = glm::rotate(BoxRotation_2, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0)); //--- �࿡���Ͽ� ȸ��

	*WorldPosition = Box_MainPosition * Box_MainRotation_1 * BoxPosition_2 * BoxRotation_2;

	glUniform3fv(*objColorLocation, 1, (float*) & (*objColor));
	glUniformMatrix4fv(*modelLocation, 1, GL_FALSE, glm::value_ptr(*WorldPosition)); //--- modelTransform ������ ��ȯ �� �����ϱ�
	*objColor = STAGE_COLOR;
	glBindVertexArray(vaoBox[1]); // ����� VAO �ҷ�����
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 3������ * 2���ﰢ��(1�簢��) * 6��


	// ���� �� // �������� ��ġ ����
	glm::mat4 BoxPosition_3 = glm::mat4(1.0f); //--- position matrix
	BoxPosition_3 = glm::translate(BoxPosition_3, glm::vec3(-boxSize, 0, 0)); //--- ������ �̵�
	glm::mat4 BoxRotation_3 = glm::mat4(1.0f); //--- position matrix
	BoxRotation_3 = glm::rotate(BoxRotation_3, glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0)); //--- �࿡���Ͽ� ȸ��

	*WorldPosition = Box_MainPosition * Box_MainRotation_1 * BoxPosition_3 * BoxRotation_3;

	glUniform3fv(*objColorLocation, 1, (float*) & (*objColor));
	glUniformMatrix4fv(*modelLocation, 1, GL_FALSE, glm::value_ptr(*WorldPosition)); //--- modelTransform ������ ��ȯ �� �����ϱ�
	*objColor = STAGE_COLOR;
	glBindVertexArray(vaoBox[2]); // ����� VAO �ҷ�����
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 3������ * 2���ﰢ��(1�簢��) * 6��


	// ������ �� // �������� ��ġ ����
	glm::mat4 BoxPosition_4 = glm::mat4(1.0f); //--- position matrix
	BoxPosition_4 = glm::translate(BoxPosition_4, glm::vec3(boxSize, 0, 0)); //--- ������ �̵�
	glm::mat4 BoxRotation_4 = glm::mat4(1.0f); //--- position matrix
	BoxRotation_4 = glm::rotate(BoxRotation_4, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0)); //--- �࿡���Ͽ� ȸ��

	*WorldPosition = Box_MainPosition * Box_MainRotation_1 * BoxPosition_4 * BoxRotation_4;

	glUniform3fv(*objColorLocation, 1, (float*) & (*objColor));
	glUniformMatrix4fv(*modelLocation, 1, GL_FALSE, glm::value_ptr(*WorldPosition)); //--- modelTransform ������ ��ȯ �� �����ϱ�
	*objColor = STAGE_COLOR;
	glBindVertexArray(vaoBox[3]); // ����� VAO �ҷ�����
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 3������ * 2���ﰢ��(1�簢��) * 6��


	// �ٴ� �� // �������� ��ġ ����
	glm::mat4 BoxPosition_5 = glm::mat4(1.0f); //--- position matrix
	BoxPosition_5 = glm::translate(BoxPosition_5, glm::vec3(0, 0, boxSize)); //--- ������ �̵�
	glm::mat4 BoxRotation_5 = glm::mat4(1.0f); //--- position matrix
	BoxRotation_5 = glm::rotate(BoxRotation_5, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0)); //--- �࿡���Ͽ� ȸ��
	glm::mat4 BoxRotation_5_1 = glm::mat4(1.0f); //--- position matrix
	BoxRotation_5_1 = glm::rotate(BoxRotation_5_1, glm::radians(180.0f), glm::vec3(0.0, 0.0, 1.0)); //--- �࿡���Ͽ� ȸ��

	*WorldPosition = Box_MainPosition * Box_MainRotation_1 * BoxPosition_5 * BoxRotation_5 * BoxRotation_5_1;

	glUniform3fv(*objColorLocation, 1, (float*) & (*objColor));
	glUniformMatrix4fv(*modelLocation, 1, GL_FALSE, glm::value_ptr(*WorldPosition)); //--- modelTransform ������ ��ȯ �� �����ϱ�
	*objColor = STAGE_COLOR;
	glBindVertexArray(vaoBox[4]); // ����� VAO �ҷ�����
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 3������ * 2���ﰢ��(1�簢��) * 6��


	// õ�� �� // �������� ��ġ ����
	glm::mat4 BoxPosition_6 = glm::mat4(1.0f); //--- position matrix
	BoxPosition_6 = glm::translate(BoxPosition_6, glm::vec3(0, boxHeight, boxSize)); //--- ������ �̵�
	glm::mat4 BoxRotation_6 = glm::mat4(1.0f); //--- position matrix
	BoxRotation_6 = glm::rotate(BoxRotation_6, glm::radians(-90.0f + this->boxRotateY), glm::vec3(1.0, 0.0, 0.0)); //--- �࿡���Ͽ� ȸ��

	*WorldPosition = Box_MainPosition * Box_MainRotation_1 * BoxPosition_6 * BoxRotation_6;

	glUniform3fv(*objColorLocation, 1, (float*) & (*objColor));
	glUniformMatrix4fv(*modelLocation, 1, GL_FALSE, glm::value_ptr(*WorldPosition)); //--- modelTransform ������ ��ȯ �� �����ϱ�
	*objColor = STAGE_COLOR;
	glBindVertexArray(vaoBox[5]); // ����� VAO �ҷ�����
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 3������ * 2���ﰢ��(1�簢��) * 6��

	glEnable(GL_CULL_FACE); // �ø� �˻�: �ĸ��� ����(backface culling)�Ͽ� �޸��� ��� ������ �� �ִ�.
}

void BOX::BoxTimerFunction()
{
	if (this->boxMoveY < 9.9f)
	{
		this->boxMoveY += 0.05f;

		if (this->boxMoveY >= 9.9f)
			this->boxMoveY = 9.9f;
	}

	if (this->boxMoveY >= 9.9f && this->boxRotateY < 240.0f)
	{
		boxRotateY += 1.0f;

		if(this->boxRotateY >= 240.0f)
			this->boxRotateY = 240.0f;
	}
}

void BOX::ReturnSetData()
{
	boxMoveY = 0.0f; // �ڽ� y�� ������
	boxRotateY = 0.0f; // �ڽ� y�� ������
}