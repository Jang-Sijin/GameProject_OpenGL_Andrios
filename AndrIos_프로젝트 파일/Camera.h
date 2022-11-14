#pragma once
#include <GL/glew.h> // �ʿ��� ������� include

static int cameraRotateType{ 1 }; // �ٶ󺸴� ����
static int cameraMode{ 2 }; // 1��Ī(value 0), 3��Ī(value 1), 3��Ī(value 2), 3��Ī(value 3)

GLfloat cameraMoveX{ 0 };			// ī�޶� �¿� �̵��� ���� ����
GLfloat cameraMoveY{ -3.5f };		// ī�޶� ���� �̵��� ���� ���� [�⺻���� -3.5f]
GLfloat cameraMoveZ{ -4.0f };		// ī�޶� �յ� �̵��� ���� ���� [�⺻���� -4.0f]
GLfloat cameraRotateY{ 0 };			// ī�޶� Y�� ����[�ڽ�]
GLfloat cameraStaticRotateY{ 0 };	// ī�޶� Y�� ����[(0,0,0) ����]


void DrawStageCamera(GLuint* shaderProgram, GLfloat p_X, GLfloat p_Y, GLfloat p_Z)
{
	//--------------------------------------------------------------------------------------------------------------------------- [���� ��ȯ ����]

	// [2. ���� ��ȯ]
	unsigned int viewLocation = glGetUniformLocation(*shaderProgram, "viewTransform"); //--- ���ؽ� ���̴����� viewTransform ������ġ
	// ī�޶� ��ġ - [nomalize ��ǥ]
	// ī�޶� ���� : ī�޶� �ٶ󺸴� ����(n) // 1. ī�޶� �̵���

	if (cameraMode == 0 || cameraMode == 1)
	{
		glm::vec3 cameraPos = glm::vec3(0, 0, 0); // ��ġ�� z���� 6��ŭ �̵�
		// ī�޶��� ������ ��(u) - ���� ���Ϳ� ī�޶� ���� ���Ϳ��� ����

		glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, -1.0);

		// ī�޶��� ���� ��(v)
		glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

		// �� ��ȯ(ī�޶�) - ����
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]); //--- viewTransform ������ ��ȯ�� �����ϱ�

		// 1.ī�޶� �̵���
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

			cameraMovePoint = glm::translate(cameraMovePoint, glm::vec3(-p_X, -p_Y - 2.0f, -p_Z)); //--- ������ �̵�
		}
		else if (cameraMode == 1)
			cameraMovePoint = glm::translate(cameraMovePoint, glm::vec3(-p_X, -p_Y - 2.0f, -p_Z - 8.0f)); //--- ������ �̵�

		// 2.ī�޶� ȸ����
		//glm::mat4 cameraRotation = glm::mat4(1.0f); //--- rotation matrix
		//cameraRotation = glm::rotate(cameraRotation, glm::radians(cameraRotateY), glm::vec3(0.0, 1.0, 0.0)); //--- �࿡���Ͽ� ȸ��
		//glm::mat4 cameraStaticRotation = glm::mat4(1.0f); //--- rotation matrix
		//cameraStaticRotation = glm::rotate(cameraStaticRotation, glm::radians(cameraStaticRotateY), glm::vec3(0.0, 1.0, 0.0)); //--- �࿡���Ͽ� ȸ��

		// 3.ī�޶� ��ȯ �����
		view = glm::lookAt(cameraPos, cameraDirection, cameraUp) * cameraMovePoint;

		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]); //--- viewTransform ������ ��ȯ�� �����ϱ�
	}
	else if (cameraMode == 2)
	{
		glm::vec3 cameraPos = glm::vec3(0.0f, 9.0f, 7.0f); // ��ġ�� y, z���� 13��ŭ �̵�
		// ī�޶��� ������ ��(u) - ���� ���Ϳ� ī�޶� ���� ���Ϳ��� ����
		glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);
		// ī�޶��� ���� ��(v)
		glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

		// �� ��ȯ(ī�޶�) - ����
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]); //--- viewTransform ������ ��ȯ�� �����ϱ�

		// 1.ī�޶� �̵���
		glm::mat4 cameraMovePoint = glm::mat4(1.0f); //--- rotation matrix
		cameraMovePoint = glm::translate(cameraMovePoint, glm::vec3(-p_X, -p_Y - 1.0f, -p_Z)); //--- ������ �̵�

		// 2.ī�޶� ȸ����
		glm::mat4 cameraRotation = glm::mat4(1.0f); //--- rotation matrix
		cameraRotation = glm::rotate(cameraRotation, glm::radians(cameraRotateY), glm::vec3(0.0, 1.0, 0.0)); //--- �࿡���Ͽ� ȸ��
		glm::mat4 cameraStaticRotation = glm::mat4(1.0f); //--- rotation matrix
		cameraStaticRotation = glm::rotate(cameraStaticRotation, glm::radians(cameraStaticRotateY), glm::vec3(0.0, 1.0, 0.0)); //--- �࿡���Ͽ� ȸ��

		// 3.ī�޶� ��ȯ �����
		view = glm::lookAt(cameraPos, cameraDirection, cameraUp) * cameraMovePoint;

		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]); //--- viewTransform ������ ��ȯ�� �����ϱ�
	}
	else if (cameraMode == 3) // ī�޶� �������� ���
	{
		glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 1.0f); // ��ġ�� y, z���� 6��ŭ �̵�
		// ī�޶��� ������ ��(u) - ���� ���Ϳ� ī�޶� ���� ���Ϳ��� ����
		glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);
		// ī�޶��� ���� ��(v)
		glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

		// �� ��ȯ(ī�޶�) - ����
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]); //--- viewTransform ������ ��ȯ�� �����ϱ�

		// 1.ī�޶� �̵���
		glm::mat4 cameraMovePoint = glm::mat4(1.0f); //--- rotation matrix
		cameraMovePoint = glm::translate(cameraMovePoint, glm::vec3(cameraMoveX, -1.0f, cameraMoveZ - 6.0f)); //--- ������ �̵�

		// 2.ī�޶� ȸ����
		glm::mat4 cameraRotation = glm::mat4(1.0f); //--- rotation matrix
		cameraRotation = glm::rotate(cameraRotation, glm::radians(cameraRotateY), glm::vec3(0.0, 1.0, 0.0)); //--- �࿡���Ͽ� ȸ��
		glm::mat4 cameraStaticRotation = glm::mat4(1.0f); //--- rotation matrix
		cameraStaticRotation = glm::rotate(cameraStaticRotation, glm::radians(cameraStaticRotateY), glm::vec3(0.0, 1.0, 0.0)); //--- �࿡���Ͽ� ȸ��

		// 3.ī�޶� ��ȯ �����
		view = cameraStaticRotation * glm::lookAt(cameraPos, cameraDirection, cameraUp) * cameraMovePoint * cameraRotation;

		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]); //--- viewTransform ������ ��ȯ�� �����ϱ�
	}
	//--------------------------------------------------------------------------------------------------------------------------- [���� ��ȯ ��]
}

void DrawMenuCamera(GLuint* shaderProgram)
{
	//--------------------------------------------------------------------------------------------------------------------------- [���� ��ȯ ����]

	// [2. ���� ��ȯ]
	unsigned int viewLocation = glGetUniformLocation(*shaderProgram, "viewTransform"); //--- ���ؽ� ���̴����� viewTransform ������ġ
	// ī�޶� ��ġ - [nomalize ��ǥ]
	// ī�޶� ���� : ī�޶� �ٶ󺸴� ����(n) // 1. ī�޶� �̵���

	glm::vec3 cameraPos = glm::vec3(0, 0, 0); // ��ġ�� z���� 6��ŭ �̵�
	// ī�޶��� ������ ��(u) - ���� ���Ϳ� ī�޶� ���� ���Ϳ��� ����

	glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, -1.0);

	// ī�޶��� ���� ��(v)
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	// �� ��ȯ(ī�޶�) - ����
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]); //--- viewTransform ������ ��ȯ�� �����ϱ�

	// 1.ī�޶� �̵���
	glm::mat4 cameraMovePoint = glm::mat4(1.0f); //--- rotation matrix
	cameraMovePoint = glm::translate(cameraMovePoint, glm::vec3(0.0f, -2.0f, 0.0f)); //--- ������ �̵�

	// 3.ī�޶� ��ȯ �����
	view = glm::lookAt(cameraPos, cameraDirection, cameraUp) * cameraMovePoint;

	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]); //--- viewTransform ������ ��ȯ�� �����ϱ�
}