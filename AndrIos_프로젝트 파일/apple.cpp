#pragma once
#define APPLE_ANIME_T2 0.001f
#define STAGE_COLOR glm::vec3(1.0f, 1.0f, 1.0f)
#define STAGE_TEXTURE_UNIFORM_NUM 0
#include "apple.h"
#include "stb_image.h"

void APPLE::InitBuffer()
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

void APPLE::InitTexture(GLuint* shaderProgram)
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
	data = stbi_load("./IMAGE/Apple.jpg", &width, &height, &channelNum, 0);

	//std::cout << width << "," << height << endl;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	glUseProgram(*shaderProgram);
	int tLocation = glGetUniformLocation(*shaderProgram, "outTexture"); //--- outTexture ������ ���÷��� ��ġ�� ������
	glUniform1i(tLocation, STAGE_TEXTURE_UNIFORM_NUM); //--- ���÷��� 0�� �������� ����
}

void APPLE::DrawScene(GLuint* shaderProgram, unsigned int* modelLocation, glm::mat4* WorldPosition, unsigned int* objColorLocation, glm::vec3* objColor, GLfloat p_X, GLfloat p_Z) // ���� �׸���
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
	if(bGetApple == false)
		Box_MainPosition = glm::translate(Box_MainPosition, glm::vec3(0.0f, 10.2f - this->boxMoveY + boxAnimationMoveY + boxAnimationMoveY_2, -3.0f)); //--- ������ �̵�
	else if (bGetApple == true)
		Box_MainPosition = glm::translate(Box_MainPosition, glm::vec3(p_X, 10.2f - this->boxMoveY + boxAnimationMoveY + boxAnimationMoveY_2, p_Z)); //--- ������ �̵�

	glm::mat4 Box_MainRotation = glm::mat4(1.0f); //--- position matrix
	Box_MainRotation = glm::rotate(Box_MainRotation, glm::radians(boxAnimationRotateY), glm::vec3(0.0, 1.0, 0.0)); //--- �࿡���Ͽ� ȸ��


	// ���� �� // �������� ��ġ ����
	glm::mat4 BoxPosition_1 = glm::mat4(1.0f); //--- position matrix
	BoxPosition_1 = glm::translate(BoxPosition_1, glm::vec3(0, 0, boxSize)); //--- ������ �̵�
	glm::mat4 BoxRotation_1 = glm::mat4(1.0f); //--- position matrix
	BoxRotation_1 = glm::rotate(BoxRotation_1, glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0)); //--- �࿡���Ͽ� ȸ��

	*WorldPosition = Box_MainPosition * Box_MainRotation * BoxPosition_1 * BoxRotation_1;
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

	*WorldPosition = Box_MainPosition * Box_MainRotation * BoxPosition_2 * BoxRotation_2;

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

	*WorldPosition = Box_MainPosition * Box_MainRotation * BoxPosition_3 * BoxRotation_3;

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

	*WorldPosition = Box_MainPosition * Box_MainRotation * BoxPosition_4 * BoxRotation_4;

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

	*WorldPosition = Box_MainPosition * Box_MainRotation * BoxPosition_5 * BoxRotation_5 * BoxRotation_5_1;

	glUniform3fv(*objColorLocation, 1, (float*) & (*objColor));
	glUniformMatrix4fv(*modelLocation, 1, GL_FALSE, glm::value_ptr(*WorldPosition)); //--- modelTransform ������ ��ȯ �� �����ϱ�
	*objColor = STAGE_COLOR;
	glBindVertexArray(vaoBox[4]); // ����� VAO �ҷ�����
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 3������ * 2���ﰢ��(1�簢��) * 6��


	// õ�� �� // �������� ��ġ ����
	glm::mat4 BoxPosition_6 = glm::mat4(1.0f); //--- position matrix
	BoxPosition_6 = glm::translate(BoxPosition_6, glm::vec3(0, boxHeight, boxSize)); //--- ������ �̵�
	glm::mat4 BoxRotation_6 = glm::mat4(1.0f); //--- position matrix
	BoxRotation_6 = glm::rotate(BoxRotation_6, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0)); //--- �࿡���Ͽ� ȸ��

	*WorldPosition = Box_MainPosition * Box_MainRotation * BoxPosition_6 * BoxRotation_6;

	glUniform3fv(*objColorLocation, 1, (float*) & (*objColor));
	glUniformMatrix4fv(*modelLocation, 1, GL_FALSE, glm::value_ptr(*WorldPosition)); //--- modelTransform ������ ��ȯ �� �����ϱ�
	*objColor = STAGE_COLOR;
	glBindVertexArray(vaoBox[5]); // ����� VAO �ҷ�����
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 3������ * 2���ﰢ��(1�簢��) * 6��

	glDisable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE); // �ø� �˻�: �ĸ��� ����(backface culling)�Ͽ� �޸��� ��� ������ �� �ִ�.
}

void APPLE::BoxTimerFunction(GLfloat p_X, GLfloat p_Z)
{
	if (this->boxMoveY < 10.0f)
	{
		this->boxMoveY += 0.05f;

		if (this->boxMoveY >= 10.0f)
			this->boxMoveY = 10.0f;
	}
	else if (this->boxMoveY >= 10.0f && this->boxAnimationMoveY < 2.5f)
	{
		this->boxAnimationMoveY += 0.01;


		if (this->boxAnimationMoveY >= 2.5f)
			this->boxAnimationMoveY = 2.5f;
	}
	else if (this->boxMoveY >= 10.0f && this->boxAnimationMoveY >= 2.5f)
	{
		boxAnimationRotateY += 2.0f;

		boxAnimationMoveY_2 += animeMoveSpeed;

		if (boxAnimationMoveY_2 >= 0.5f)
		{
			boxAnimationMoveY_2 = 0.5f;
			animeMoveSpeed *= -1;
		}
		else if(boxAnimationMoveY_2 <= -0.5f)
		{
			boxAnimationMoveY_2 = -0.5f;
			animeMoveSpeed *= -1;
		}

		if (boxAnimationRotateY >= 360.0f)
			boxAnimationRotateY = 0.0f;

		if ((bGetApple == false) && (p_X > -0.1f && p_X < 0.1f) && (p_Z > -3.1f && p_Z < -2.9f))
		{
			bGetApple = true;
		}
	}
}

void APPLE::ReturnSetData()
{
	boxMoveY = 0.0f; // �ڽ� y�� ������
	boxAnimationMoveY = 0.0f; // �ڽ� y�� ������
	boxAnimationRotateY = 0.0f; // �ڽ� y�� ������
	boxAnimationMoveY_2 = 0.0f; // �ڽ� y�� ������
	animeMoveSpeed = 0.005f;
	bGetApple = false;
}