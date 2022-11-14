#pragma once
#define STAGE_COLOR glm::vec3(1.0f, 1.0f, 1.0f)
#define MENU_MAIN_TEXTURE_UNIFORM_NUM 0
#include "UI_MainMenu.h"
#include "stb_image.h"

enum class STAGESTATE { STARTMENU, ONE, TWO, THREE, RESTART, END };

void UI_MAIN_MENU::InitBuffer()
{
	// [MainMenuUI 6면 Const]
	//--- 타일데이터: [VAO 1개, VBO 1개를 사용하여 구현]
	glGenVertexArrays(3, this->vaoMainMenuUI); //--- VAO 를 지정하고 할당하기
	glGenBuffers(9, this->vboMainMenuUI); //--- 12개의 VBO를 지정하고 할당하기
	glGenBuffers(3, this->eboMainMenuUI); //--- 2개의 EBO를 지정하고 할당하기

	//--- 1번째 VBO를 활성화하여 바인드하고, [버텍스 속성] (좌표값)을 저장
	for (int i = 0; i < 3; ++i)
	{
		glBindVertexArray(this->vaoMainMenuUI[i]); //--- VAO를 바인드하기

		glBindBuffer(GL_ARRAY_BUFFER, this->vboMainMenuUI[i * 3]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vMainMenuUIPositionList), vMainMenuUIPositionList, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		//--- 1번째 VBO를 활성화하여 바인드하고, [컬러 속성]을 저장
		glBindBuffer(GL_ARRAY_BUFFER, this->vboMainMenuUI[(i * 3) + 1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vUiColor), vUiColor, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);


		glBindBuffer(GL_ARRAY_BUFFER, this->vboMainMenuUI[(i * 3) + 2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vUiTex), vUiTex, GL_STATIC_DRAW);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->eboMainMenuUI[i]); // GL_ELEMENT_ARRAY_BUFFER 버퍼 유형으로 바인딩	
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
	int tLocation = glGetUniformLocation(*shaderProgram, "outTexture"); //--- outTexture 유니폼 샘플러의 위치를 가져옴
	glUniform1i(tLocation, MENU_MAIN_TEXTURE_UNIFORM_NUM); //--- 샘플러를 0번 유닛으로 설정

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
	glUniform1i(tLocation, MENU_MAIN_TEXTURE_UNIFORM_NUM); //--- 샘플러를 0번 유닛으로 설정

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
	glUniform1i(tLocation, MENU_MAIN_TEXTURE_UNIFORM_NUM); //--- 샘플러를 0번 유닛으로 설정
}

void UI_MAIN_MENU::DrawScene(GLuint* shaderProgram, unsigned int* modelLocation, glm::mat4* WorldPosition, unsigned int* objColorLocation, glm::vec3* objColor, GLfloat* clickPointX, GLfloat* clickPointY, int textureType) // 정보 그리기
{
	glFrontFace(GL_CW); // 여기서 부턴 시계 방향을 앞면으로 인식한다.
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	unsigned int tLocation = glGetUniformLocation(*shaderProgram, "outTexture");
	glUniform1i(tLocation, MENU_MAIN_TEXTURE_UNIFORM_NUM);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);

	// 기본 월드 좌표계
	glm::mat4 MainMenuUI_MainPosition = glm::mat4(1.0f); //--- position matrix
	MainMenuUI_MainPosition = glm::translate(MainMenuUI_MainPosition, glm::vec3(0.0f, 0.0f, 0.0f)); //--- 축으로 이동

	// BackGround 텍스처 위치 설정
	glm::mat4 MainMenuUIPosition_1 = glm::mat4(1.0f); //--- position matrix
	MainMenuUIPosition_1 = glm::translate(MainMenuUIPosition_1, glm::vec3(0, -5.0f, -7.0f)); //--- 축으로 이동
	glm::mat4 MainMenuUIRotation_1 = glm::mat4(1.0f); //--- position matrix
	MainMenuUIRotation_1 = glm::rotate(MainMenuUIRotation_1, glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0)); //--- 축에대하여 회전

	*WorldPosition = MainMenuUI_MainPosition * MainMenuUIPosition_1 * MainMenuUIRotation_1;

	glUniform3fv(*objColorLocation, 1, (float*) & (*objColor));
	glUniformMatrix4fv(*modelLocation, 1, GL_FALSE, glm::value_ptr(*WorldPosition)); //--- modelTransform 변수에 변환 값 적용하기
	*objColor = STAGE_COLOR;
	glBindVertexArray(vaoMainMenuUI[0]); // 사용할 VAO 불러오기
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 3꼭지점 * 2개삼각형(1사각형) * 6면


	if (textureType < 2) // END UI가 아니면 출력
	{
		if ((*clickPointX > -0.4f && *clickPointX < 0.4f) && (*clickPointY > -0.03f && *clickPointY < 0.33f))
		{
			glUniform1i(tLocation, MENU_MAIN_TEXTURE_UNIFORM_NUM);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, ClickTexture1);

			// Click 텍스처 위치 설정
			glm::mat4 MainMenuUIPosition_2 = glm::mat4(1.0f); //--- position matrix
			MainMenuUIPosition_2 = glm::translate(MainMenuUIPosition_2, glm::vec3(0, 2.5f, -20.0f)); //--- 축으로 이동
			glm::mat4 MainMenuUIRotation_2 = glm::mat4(1.0f); //--- position matrix
			MainMenuUIRotation_2 = glm::rotate(MainMenuUIRotation_2, glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0)); //--- 축에대하여 회전
			glm::mat4 MainMenuUIScale_2 = glm::mat4(1.0f);
			MainMenuUIScale_2 = glm::scale(MainMenuUIScale_2, glm::vec3(1.0f, 0.5f, 1.0f));

			*WorldPosition = MainMenuUI_MainPosition * MainMenuUIScale_2 * MainMenuUIPosition_2 * MainMenuUIRotation_2;

			glUniform3fv(*objColorLocation, 1, (float*) & (*objColor));
			glUniformMatrix4fv(*modelLocation, 1, GL_FALSE, glm::value_ptr(*WorldPosition)); //--- modelTransform 변수에 변환 값 적용하기
			*objColor = STAGE_COLOR;
			glBindVertexArray(vaoMainMenuUI[1]); // 사용할 VAO 불러오기
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 3꼭지점 * 2개삼각형(1사각형) * 6면
		}
		else
		{
			glUniform1i(tLocation, MENU_MAIN_TEXTURE_UNIFORM_NUM);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, ClickTexture);

			// Click 텍스처 위치 설정
			glm::mat4 MainMenuUIPosition_2 = glm::mat4(1.0f); //--- position matrix
			MainMenuUIPosition_2 = glm::translate(MainMenuUIPosition_2, glm::vec3(0, 2.5f, -20.0f)); //--- 축으로 이동
			glm::mat4 MainMenuUIRotation_2 = glm::mat4(1.0f); //--- position matrix
			MainMenuUIRotation_2 = glm::rotate(MainMenuUIRotation_2, glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0)); //--- 축에대하여 회전
			glm::mat4 MainMenuUIScale_2 = glm::mat4(1.0f);
			MainMenuUIScale_2 = glm::scale(MainMenuUIScale_2, glm::vec3(1.0f, 0.5f, 1.0f));

			*WorldPosition = MainMenuUI_MainPosition * MainMenuUIScale_2 * MainMenuUIPosition_2 * MainMenuUIRotation_2;

			glUniform3fv(*objColorLocation, 1, (float*) & (*objColor));
			glUniformMatrix4fv(*modelLocation, 1, GL_FALSE, glm::value_ptr(*WorldPosition)); //--- modelTransform 변수에 변환 값 적용하기
			*objColor = STAGE_COLOR;
			glBindVertexArray(vaoMainMenuUI[1]); // 사용할 VAO 불러오기
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 3꼭지점 * 2개삼각형(1사각형) * 6면
		}
	}
}