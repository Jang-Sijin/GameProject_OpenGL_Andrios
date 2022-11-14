#pragma once
#define STAGE_COLOR glm::vec3(1.0f, 1.0f, 1.0f)
#define MENU_MAIN_TEXTURE_UNIFORM_NUM 0
#include "UI_MainMenu.h"
#include "stb_image.h"

enum class STAGESTATE { STARTMENU, ONE, TWO, THREE, RESTART, END };

void UI_MAIN_MENU::InitBuffer()
{
	// [MainMenuUI 6�� Const]
	//--- Ÿ�ϵ�����: [VAO 1��, VBO 1���� ����Ͽ� ����]
	glGenVertexArrays(3, this->vaoMainMenuUI); //--- VAO �� �����ϰ� �Ҵ��ϱ�
	glGenBuffers(9, this->vboMainMenuUI); //--- 12���� VBO�� �����ϰ� �Ҵ��ϱ�
	glGenBuffers(3, this->eboMainMenuUI); //--- 2���� EBO�� �����ϰ� �Ҵ��ϱ�

	//--- 1��° VBO�� Ȱ��ȭ�Ͽ� ���ε��ϰ�, [���ؽ� �Ӽ�] (��ǥ��)�� ����
	for (int i = 0; i < 3; ++i)
	{
		glBindVertexArray(this->vaoMainMenuUI[i]); //--- VAO�� ���ε��ϱ�

		glBindBuffer(GL_ARRAY_BUFFER, this->vboMainMenuUI[i * 3]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vMainMenuUIPositionList), vMainMenuUIPositionList, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		//--- 1��° VBO�� Ȱ��ȭ�Ͽ� ���ε��ϰ�, [�÷� �Ӽ�]�� ����
		glBindBuffer(GL_ARRAY_BUFFER, this->vboMainMenuUI[(i * 3) + 1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vUiColor), vUiColor, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);


		glBindBuffer(GL_ARRAY_BUFFER, this->vboMainMenuUI[(i * 3) + 2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vUiTex), vUiTex, GL_STATIC_DRAW);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->eboMainMenuUI[i]); // GL_ELEMENT_ARRAY_BUFFER ���� �������� ���ε�	
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(UiIndex), UiIndex, GL_STATIC_DRAW);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(GLfloat)));
	}
}

void UI_MAIN_MENU::InitTexture(GLuint* shaderProgram, int textureType)
{
	int width;
	int height;
	int channelNum;
	unsigned char* data[3] = { nullptr, nullptr, nullptr };

	stbi_set_flip_vertically_on_load(true);

	glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if(textureType == 0)
		data[0] = stbi_load("./MENU_IMAGE/main_screen.jpg", &width, &height, &channelNum, 0);
	else if (textureType == 1)
		data[0] = stbi_load("./MENU_IMAGE/gameout_screen.jpg", &width, &height, &channelNum, 0);
	else if (textureType == 2)
		data[0] = stbi_load("./MENU_IMAGE/end_screen.jpg", &width, &height, &channelNum, 0);
	

	//std::cout << width << "," << height << endl;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data[0]);

	glUseProgram(*shaderProgram);
	int tLocation = glGetUniformLocation(*shaderProgram, "outTexture"); //--- outTexture ������ ���÷��� ��ġ�� ������
	glUniform1i(tLocation, MENU_MAIN_TEXTURE_UNIFORM_NUM); //--- ���÷��� 0�� �������� ����

	//--

	glGenTextures(1, &ClickTexture);
	glBindTexture(GL_TEXTURE_2D, ClickTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (textureType == 0)
		data[1] = stbi_load("./MENU_IMAGE/game_start.jpg", &width, &height, &channelNum, 0);
	else if (textureType == 1)
		data[1] = stbi_load("./MENU_IMAGE/game_restart.jpg", &width, &height, &channelNum, 0);

	//std::cout << width << "," << height << endl;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data[1]);
	glUniform1i(tLocation, MENU_MAIN_TEXTURE_UNIFORM_NUM); //--- ���÷��� 0�� �������� ����

	//--

	glGenTextures(1, &ClickTexture1);
	glBindTexture(GL_TEXTURE_2D, ClickTexture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (textureType == 0)
		data[2] = stbi_load("./MENU_IMAGE/game_start2.jpg", &width, &height, &channelNum, 0);
	else if (textureType == 1)
		data[2] = stbi_load("./MENU_IMAGE/game_restart2.jpg", &width, &height, &channelNum, 0);

	//std::cout << width << "," << height << endl;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data[2]);
	glUniform1i(tLocation, MENU_MAIN_TEXTURE_UNIFORM_NUM); //--- ���÷��� 0�� �������� ����
}

void UI_MAIN_MENU::DrawScene(GLuint* shaderProgram, unsigned int* modelLocation, glm::mat4* WorldPosition, unsigned int* objColorLocation, glm::vec3* objColor, GLfloat* clickPointX, GLfloat* clickPointY, int textureType) // ���� �׸���
{
	glFrontFace(GL_CW); // ���⼭ ���� �ð� ������ �ո����� �ν��Ѵ�.
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	unsigned int tLocation = glGetUniformLocation(*shaderProgram, "outTexture");
	glUniform1i(tLocation, MENU_MAIN_TEXTURE_UNIFORM_NUM);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);

	// �⺻ ���� ��ǥ��
	glm::mat4 MainMenuUI_MainPosition = glm::mat4(1.0f); //--- position matrix
	MainMenuUI_MainPosition = glm::translate(MainMenuUI_MainPosition, glm::vec3(0.0f, 0.0f, 0.0f)); //--- ������ �̵�

	// BackGround �ؽ�ó ��ġ ����
	glm::mat4 MainMenuUIPosition_1 = glm::mat4(1.0f); //--- position matrix
	MainMenuUIPosition_1 = glm::translate(MainMenuUIPosition_1, glm::vec3(0, -5.0f, -7.0f)); //--- ������ �̵�
	glm::mat4 MainMenuUIRotation_1 = glm::mat4(1.0f); //--- position matrix
	MainMenuUIRotation_1 = glm::rotate(MainMenuUIRotation_1, glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0)); //--- �࿡���Ͽ� ȸ��

	*WorldPosition = MainMenuUI_MainPosition * MainMenuUIPosition_1 * MainMenuUIRotation_1;

	glUniform3fv(*objColorLocation, 1, (float*) & (*objColor));
	glUniformMatrix4fv(*modelLocation, 1, GL_FALSE, glm::value_ptr(*WorldPosition)); //--- modelTransform ������ ��ȯ �� �����ϱ�
	*objColor = STAGE_COLOR;
	glBindVertexArray(vaoMainMenuUI[0]); // ����� VAO �ҷ�����
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 3������ * 2���ﰢ��(1�簢��) * 6��


	if (textureType < 2) // END UI�� �ƴϸ� ���
	{
		if ((*clickPointX > -0.4f && *clickPointX < 0.4f) && (*clickPointY > -0.03f && *clickPointY < 0.33f))
		{
			glUniform1i(tLocation, MENU_MAIN_TEXTURE_UNIFORM_NUM);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, ClickTexture1);

			// Click �ؽ�ó ��ġ ����
			glm::mat4 MainMenuUIPosition_2 = glm::mat4(1.0f); //--- position matrix
			MainMenuUIPosition_2 = glm::translate(MainMenuUIPosition_2, glm::vec3(0, 2.5f, -20.0f)); //--- ������ �̵�
			glm::mat4 MainMenuUIRotation_2 = glm::mat4(1.0f); //--- position matrix
			MainMenuUIRotation_2 = glm::rotate(MainMenuUIRotation_2, glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0)); //--- �࿡���Ͽ� ȸ��
			glm::mat4 MainMenuUIScale_2 = glm::mat4(1.0f);
			MainMenuUIScale_2 = glm::scale(MainMenuUIScale_2, glm::vec3(1.0f, 0.5f, 1.0f));

			*WorldPosition = MainMenuUI_MainPosition * MainMenuUIScale_2 * MainMenuUIPosition_2 * MainMenuUIRotation_2;

			glUniform3fv(*objColorLocation, 1, (float*) & (*objColor));
			glUniformMatrix4fv(*modelLocation, 1, GL_FALSE, glm::value_ptr(*WorldPosition)); //--- modelTransform ������ ��ȯ �� �����ϱ�
			*objColor = STAGE_COLOR;
			glBindVertexArray(vaoMainMenuUI[1]); // ����� VAO �ҷ�����
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 3������ * 2���ﰢ��(1�簢��) * 6��
		}
		else
		{
			glUniform1i(tLocation, MENU_MAIN_TEXTURE_UNIFORM_NUM);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, ClickTexture);

			// Click �ؽ�ó ��ġ ����
			glm::mat4 MainMenuUIPosition_2 = glm::mat4(1.0f); //--- position matrix
			MainMenuUIPosition_2 = glm::translate(MainMenuUIPosition_2, glm::vec3(0, 2.5f, -20.0f)); //--- ������ �̵�
			glm::mat4 MainMenuUIRotation_2 = glm::mat4(1.0f); //--- position matrix
			MainMenuUIRotation_2 = glm::rotate(MainMenuUIRotation_2, glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0)); //--- �࿡���Ͽ� ȸ��
			glm::mat4 MainMenuUIScale_2 = glm::mat4(1.0f);
			MainMenuUIScale_2 = glm::scale(MainMenuUIScale_2, glm::vec3(1.0f, 0.5f, 1.0f));

			*WorldPosition = MainMenuUI_MainPosition * MainMenuUIScale_2 * MainMenuUIPosition_2 * MainMenuUIRotation_2;

			glUniform3fv(*objColorLocation, 1, (float*) & (*objColor));
			glUniformMatrix4fv(*modelLocation, 1, GL_FALSE, glm::value_ptr(*WorldPosition)); //--- modelTransform ������ ��ȯ �� �����ϱ�
			*objColor = STAGE_COLOR;
			glBindVertexArray(vaoMainMenuUI[1]); // ����� VAO �ҷ�����
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 3������ * 2���ﰢ��(1�簢��) * 6��
		}
	}
}