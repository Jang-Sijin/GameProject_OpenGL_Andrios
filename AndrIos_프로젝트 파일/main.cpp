#define _CRT_SECURE_NO_WARNINGS
#define DEBUG 1
#if DEBUG == 1
#include <GLM/glm.hpp> // glm ���� ���̺귯��
#include <GLM/ext.hpp> // glm ���� ���̺귯��
#include <GLM/gtc/matrix_transform.hpp> // glm ���� ���̺귯��
#elif DEBUG == 2
#include <GL/glm/glm/glm.hpp> // glm ���� ���̺귯��
#include <GL/glm/glm/ext.hpp> // glm ���� ���̺귯��
#include <GL/glm/glm/gtc/matrix_transform.hpp> // glm ���� ���̺귯��
#endif

#define STB_IMAGE_IMPLEMENTATION
#define WINDOW_WIDTH			800
#define WINDOW_HEIGHT			600
#define WINDOW_DEPTH			300
#define PORTAL_SIZE				0.5
#define PLAYER_COLOR			glm::vec3(0.3, 0.3, 1.0)
#define PLAYER_BARRAGE_COLOR	glm::vec3(0.8, 0.8, 1.0)
#define LIGHT_POS				glm::vec3(0.0, 50.0, 30.0)
#define LIGHT_COLOR				glm::vec3(1.0, 1.0, 1.0)

#include <GL/glew.h> // �ʿ��� ������� include
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#include <iostream>
#include <list>
#include <time.h>

#include "ReadFileToBuffer.h"
#include "Stage.h"
#include "Camera.h"
#include "Player.h"
#include "Monster.h"
#include "Mouse.h"
#include "GameTimer.h"
#include "Potal.h"
#include "box.h"
#include "UI_MainMenu.h"
#include "apple.h"
#pragma comment(lib, "glew32.lib")

using namespace std;

GLchar *vertexSource, *fragmentSource; // �ҽ��ڵ� ���� ����
GLuint vertexShader, fragmentShader; // ���̴� ��ü
GLuint shaderProgram;

GLvoid DrawScene(GLvoid);
GLvoid ReShape(int w, int h);

void KeyBoard(unsigned char key, int x, int y);
void KeyBoardUp(unsigned char key, int x, int y);
void SpecialKeyboard(int key, int x, int y);
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);
void Passive(int x, int y);
void TimerFunction(int value);

bool make_vertexShader();
bool make_fragmentShader();
void InitTexture();
void InitBuffer();
void InitShader();
void ConvertDeviceXYOpenGLXY(int x, int y, float* ox, float* oy); // Windows ��ǥ�� -> GLSL ��ǥ�� [�ں�ȯ]

void DrawProject(int now_stage_state);	// ���� ��ȯ

enum class STAGESTATE{STARTMENU, ONE, TWO, THREE, RESTART, END};

UI_MAIN_MENU gameMenu[3]; // 0: StartMenu, 1: RestartMenu, 2: EndMenu
STAGE gameStage[3]; // �������� 3��
POTAL stagePotal[3]; // �������� ��Ż 3��
BOX item_Box; // 3�������� ������ �ڽ�
APPLE item_Apple; // 3�������� ���
PLAYER player{MOVETYPE::UP};	// �÷��̾� ��ü
GLfloat p_X, p_Y, p_Z; // �÷��̾� ���� ��ǥ
GLfloat m_X, m_Y, m_Z; // ���� ��ǥ
int save_stage_state{ (int)STAGESTATE::ONE };
int now_stage_state{(int)STAGESTATE::STARTMENU };
bool gameLoop{ true };

// list<����>
list<MONSTER> lt_monster;
list<MONSTER>::iterator iter_monster;

int main(int argc, char* argv[])
{
	//--- ����
	srand((unsigned int)time(NULL));

	//--- ������ �����ϱ�
	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ����
	glutInitWindowPosition(500, 150); // �������� ��ġ ����
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT); // �������� ũ�� ����
	glutCreateWindow("AndrIos"); // ������ ���� // (������ �̸�)

	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew �ʱ�ȭ
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized" << std::endl;
	
	player.InitData();

	InitTexture();
	InitBuffer();
	InitShader();
	CurTime = clock();
	cout<< tm.tm_year + 1900 <<"."<< tm.tm_mon + 1 << "." << tm.tm_mday << "." << tm.tm_hour << "." << tm.tm_min << "." << tm.tm_sec<<endl;

	glutDisplayFunc(DrawScene); // ��� �Լ��� ����
	glutReshapeFunc(ReShape); // �ٽ� �׸��� �Լ� ����
	glutKeyboardFunc(KeyBoard); // Ű���� �Է� �ݹ� �Լ�
	glutKeyboardUpFunc(KeyBoardUp);
	glutSpecialFunc(SpecialKeyboard);
	glutMouseFunc(Mouse);
	// glutMotionFunc(Motion);
	glutPassiveMotionFunc(Passive);
	glutTimerFunc(1, TimerFunction, 1);
	glutMainLoop(); // �̺�Ʈ ó�� ����
}

// ������ ��� �Լ�
GLvoid DrawScene(GLvoid)
{
	//--- ����� ���� ����
	GLfloat rColor{ 0.0f }, gColor{ 0.0f }, bColor{ 0.0f };
	glClearColor(rColor, gColor, bColor, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//--------------------------------------------------------------------------------------------------------------------------- [�� ��ȯ ����]
	// ������ ���������ο� ���̴� �ҷ�����

	// [1. �� ��ȯ]
	// �⺻ ���� ��ǥ��
	unsigned int modelLocation = glGetUniformLocation(shaderProgram, "modelTransform");
	glm::mat4 WorldPosition = glm::mat4(1.0f); //--- transformation matrix
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(WorldPosition)); //--- modelTransform ������ ��ȯ �� �����ϱ�
																	   
	// ����
	// ������ ��ǥ
	glm::vec4 lightPos = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(LIGHT_POS, 1.0f);
	GLuint lightPosLocation = glGetUniformLocation(shaderProgram, "lightPos");
	glUniform3fv(lightPosLocation, 1, (float*)&lightPos);
	// ������ ��
	glm::vec3 lightColor = glm::vec3(1.0f);
	lightColor = LIGHT_COLOR;
	GLuint lightColorLocation = glGetUniformLocation(shaderProgram, "lightColor");
	glUniform3fv(lightColorLocation, 1, (float*)&lightColor);
	// ��ü�� ��
	glm::vec3 objColor = glm::vec3(1.0f);
	GLuint objColorLocation = glGetUniformLocation(shaderProgram, "objectColor");

	int shapeType = glGetUniformLocation(shaderProgram, "shapeType");
	glUniform1i(shapeType, 0);

	if (now_stage_state == (int)STAGESTATE::STARTMENU) { // ���� �޴�
		gameMenu[0].DrawScene(&shaderProgram, &modelLocation, &WorldPosition, &objColorLocation, &objColor, &clickPointX, &clickPointY,0);

		DrawMenuCamera(&shaderProgram);	// ī�޶� ��ȯ
	}
	else if (now_stage_state == (int)STAGESTATE::RESTART){
		gameMenu[1].DrawScene(&shaderProgram, &modelLocation, &WorldPosition, &objColorLocation, &objColor, &clickPointX, &clickPointY,1);

		DrawMenuCamera(&shaderProgram);	// ī�޶� ��ȯ
	}
	else if (now_stage_state == (int)STAGESTATE::END) {
		gameMenu[2].DrawScene(&shaderProgram, &modelLocation, &WorldPosition, &objColorLocation, &objColor, &clickPointX, &clickPointY,2);

		DrawMenuCamera(&shaderProgram);	// ī�޶� ��ȯ
	}
	else{ // ���� �÷��� ��������
		glEnable(GL_DEPTH_TEST); // ���� �˻�: 3���� ����� 2���� ��鿡 ������Ű�� ��ü���� ��ø�� �� �ִµ�, �������� �������� ����� ���� ���̰� ���̰� ū ���� ������ ���̵��� ���� ���Ÿ� �Ѵ�.
		glEnable(GL_CULL_FACE); // �ø� �˻�: �ĸ��� ����(backface culling)�Ͽ� �޸��� ��� ������ �� �ִ�.

		if (now_stage_state == (int)STAGESTATE::ONE) {
			gameStage[0].DrawScene(&shaderProgram, &modelLocation, &WorldPosition, &objColorLocation, &objColor);
			stagePotal[0].DrawScene(&shaderProgram, &modelLocation, &WorldPosition, &objColorLocation, &objColor);
		}
		else if (now_stage_state == (int)STAGESTATE::TWO) {
			gameStage[1].DrawScene(&shaderProgram, &modelLocation, &WorldPosition, &objColorLocation, &objColor);
			stagePotal[1].DrawScene(&shaderProgram, &modelLocation, &WorldPosition, &objColorLocation, &objColor);
		}
		else if (now_stage_state == (int)STAGESTATE::THREE) {
			gameStage[2].DrawScene(&shaderProgram, &modelLocation, &WorldPosition, &objColorLocation, &objColor);
			stagePotal[2].DrawScene(&shaderProgram, &modelLocation, &WorldPosition, &objColorLocation, &objColor);

			if (lt_monster.empty()) {
				item_Box.DrawScene(&shaderProgram, &modelLocation, &WorldPosition, &objColorLocation, &objColor);
				item_Apple.DrawScene(&shaderProgram, &modelLocation, &WorldPosition, &objColorLocation, &objColor, p_X, p_Z);
				}
		}

		glUniform1i(shapeType, 1);

		objColor = PLAYER_COLOR;
		glUniform3fv(objColorLocation, 1, (float*)&objColor);
		player.DrawScene(&modelLocation, &WorldPosition);

		objColor = PLAYER_BARRAGE_COLOR;
		glUniform3fv(objColorLocation, 1, (float*)&objColor);
		player.DrawScene_Barrage(&modelLocation, &WorldPosition);
		player.GetPlayerPosition(&p_X, &p_Y, &p_Z);

		iter_monster = lt_monster.begin();
		for (int i = 0; i <lt_monster.size(); ++i) {
			(*iter_monster).DrawScene(&modelLocation, &WorldPosition, &objColorLocation,p_X,p_Y,p_Z);
			(*iter_monster).DrawScene_Barrage(&modelLocation, &WorldPosition);
			++iter_monster;
		}

		DrawStageCamera(&shaderProgram, p_X, p_Y, p_Z);	// ī�޶� ��ȯ

		glDisable(GL_DEPTH_TEST); // ���� �˻�: 3���� ����� 2���� ��鿡 ������Ű�� ��ü���� ��ø�� �� �ִµ�, �������� �������� ����� ���� ���̰� ���̰� ū ���� ������ ���̵��� ���� ���Ÿ� �Ѵ�.
		glDisable(GL_CULL_FACE); // �ø� �˻�: �ĸ��� ����(backface culling)�Ͽ� �޸��� ��� ������ �� �ִ�.
	}			
	DrawProject(now_stage_state);	// ���� ��ȯ	if (now_stage_state == (int)STAGESTATE::STARTMENU)

	glutSwapBuffers();
}

GLvoid ReShape(int w, int h)
{
	glViewport(0, 0, w, h); // ����Ʈ ��ȯ
	glMatrixMode(GL_PROJECTION);
	glOrtho(0.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 0.0f, 0.0f, WINDOW_DEPTH);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void KeyBoard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w' :
	case 'W' :
		cameraRotateType = 1;
		player.Move(MOVETYPE::UP);
		break;

	case 'a' :
	case 'A' :
		cameraRotateType = 3;
		player.Move(MOVETYPE::LEFT);
		break;

	case 's' :
	case 'S' :
		cameraRotateType = 0;
		player.Move(MOVETYPE::DOWN);
		break;

	case 'd' :
	case 'D' :
		cameraRotateType = 2;
		player.Move(MOVETYPE::RIGHT);
		break;

	case '1':
		cameraMode = 0;
		break;
	case '2':
		cameraMode = 1;
		break;
	case '3':
		cameraMode = 2;
		break;
	case '4':
		cameraMode = 3;
		break;
	case 'z' : // ��ǥ ���� ��
	case 'Z' :
		cout << "x : " << p_X << ", z : " << p_Z << endl;
		break;

	case 'q':
		std::cerr << "[���α׷� ����]" << std::endl;
		exit(0);
		break;
	}

	if (now_stage_state == (int)STAGESTATE::STARTMENU || now_stage_state == (int)STAGESTATE::RESTART || now_stage_state == (int)STAGESTATE::END)
	{
		DrawMenuCamera(&shaderProgram);	// ī�޶� ��ȯ
	}
	else
	{
		player.GetPlayerPosition(&p_X, &p_Y, &p_Z);
		DrawStageCamera(&shaderProgram, p_X, p_Y, p_Z);	// ī�޶� ��ȯ
	}

	glutPostRedisplay(); //--- ������ �ٲ𶧸��� ��� �ݹ��Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
}

void SpecialKeyboard(int key, int x, int y)
{
	if (key == GLUT_KEY_UP)
	{
		cameraMoveZ += 1.0f;
		// cout << "up" << endl;
	}
	else if (key == GLUT_KEY_DOWN)
	{
		cameraMoveZ -= 1.0f;
		// cout << "down" << endl;
	}

	if (key == GLUT_KEY_RIGHT)
	{
		cameraMoveX -= 1.0f;
		// cout << "right" << endl;
	}
	else if (key == GLUT_KEY_LEFT)
	{
		cameraMoveX += 1.0f;
		// cout << "left" << endl;
	}

	if (key == GLUT_KEY_INSERT) {
		player.Create_Barrage();
	}
	glutPostRedisplay();
}

void Mouse(int button, int state, int x, int y)
{
	ConvertDeviceXYOpenGLXY(x, y, &clickPointX, &clickPointY); // Windows ��ǥ�� -> GLSL ��ǥ�� [�ں�ȯ]

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		cout << "���콺 ������(DOWN): " << clickPointX << ", " << clickPointY << endl;
		bMouse_Left_Button = true;

		if (now_stage_state == (int)STAGESTATE::STARTMENU && (clickPointX > -0.4f && clickPointX < 0.4f) && (clickPointY > -0.03f && clickPointY < 0.33f))
		{
			bClickMenu = true;
		}
		else if (now_stage_state == (int)STAGESTATE::RESTART && (clickPointX > -0.4f && clickPointX < 0.4f) && (clickPointY > -0.03f && clickPointY < 0.33f))
		{
			bClickMenu = true;
		}
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		cout << "���콺 ������(UP): " << clickPointX << ", " << clickPointY << endl;
		bMouse_Left_Button = false;
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		cout << "���콺 ������(DOWN): " << clickPointX << ", " << clickPointY << endl;
		bMouse_Right_Button = true;
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
	{
		cout << "���콺 ������(UP): " << clickPointX << ", " << clickPointY << endl;
		bMouse_Right_Button = false;
	}

	glutPostRedisplay();
}

void Motion(int x, int y)
{
	ConvertDeviceXYOpenGLXY(x, y, &clickPointX, &clickPointY); // Windows ��ǥ�� -> GLSL ��ǥ�� [�ں�ȯ]

	//if (bMouse_Left_Button == true)
	//{		
	//
	//}

	glutPostRedisplay();
} 

void Passive(int x, int y)
{
	ConvertDeviceXYOpenGLXY(x, y, &clickPointX, &clickPointY); // Windows ��ǥ�� -> GLSL ��ǥ�� [�ں�ȯ]

	// cout << clickPointX << ", " << clickPointY << endl;

	glutPostRedisplay();
}

void TimerFunction(int value)
{
	while (1)
	{
		CurTime = clock();
		if (CurTime - OldTime > 10) // CurTime - OldTime > 33 // 33pfs
		{
			OldTime = CurTime;
			break;
		}
	}

	GameTimer++;
	if (gameLoop == true)
	{

		// ���� �޴����� ���� ���� ��ư�� ������ 1���������� �̵��Ѵ�.
		if (now_stage_state == (int)STAGESTATE::STARTMENU && bClickMenu == true)
		{
			now_stage_state = (int)STAGESTATE::ONE;
			bClickMenu = false;
			InitBuffer();
		}
		else if (now_stage_state == (int)STAGESTATE::RESTART && bClickMenu == true)
		{
			player.Player_Hp_Full();
			now_stage_state = save_stage_state;
			bClickMenu = false;
			InitBuffer();
		}

		player.PlayerTimerFunction();

		iter_monster = lt_monster.begin();
		for (int i = 0; i < lt_monster.size(); ++i) {
			if (GameTimer % 100 == 0)
			{
				(*iter_monster).Create_Barrage();
			}
			if (GameTimer % 200 == 0)
			{
				(*iter_monster).Boss_Pattern();
			}
			++iter_monster;
		}

		if (PORTAL_SIZE >= p_X && -PORTAL_SIZE <= p_X && -10.0f + PORTAL_SIZE >= p_Z && -10.0 - PORTAL_SIZE <= p_Z && lt_monster.empty()) {
			if (now_stage_state == (int)STAGESTATE::ONE) {
				stagePotal[0].ReturnSetData();
				save_stage_state = (int)STAGESTATE::TWO;
				now_stage_state = (int)STAGESTATE::TWO;
				InitBuffer();
			}
			else if (now_stage_state == (int)STAGESTATE::TWO) {
				stagePotal[1].ReturnSetData();
				save_stage_state = (int)STAGESTATE::THREE;
				now_stage_state = (int)STAGESTATE::THREE;
				InitBuffer();
			}
			else if (now_stage_state == (int)STAGESTATE::THREE)
			{
				stagePotal[2].ReturnSetData();
				item_Box.ReturnSetData();
				item_Apple.ReturnSetData();
				save_stage_state = (int)STAGESTATE::ONE;
				now_stage_state = (int)STAGESTATE::END;
				InitBuffer();

				gameLoop = false;
			}
		}

		if (now_stage_state == (int)STAGESTATE::ONE && lt_monster.empty())
		{
			stagePotal[0].PotalTimerFunction();
		}
		else if (now_stage_state == (int)STAGESTATE::TWO && lt_monster.empty())
		{
			stagePotal[1].PotalTimerFunction();
		}
		else if (now_stage_state == (int)STAGESTATE::THREE && lt_monster.empty())
		{
			stagePotal[2].PotalTimerFunction();
			item_Box.BoxTimerFunction();
			item_Apple.BoxTimerFunction(p_X, p_Z);
		}


		if (bMouse_Left_Button == true && GameTimer % 10 == 0)
		{
			player.Create_Barrage();
		}
		else if (bMouse_Right_Button == true && GameTimer % 5 == 0)
		{
			player.Create_Barrage();
		}

		player.GetPlayerPosition(&p_X, &p_Y, &p_Z);
		iter_monster = lt_monster.begin();
		for (int i = 0; i < lt_monster.size(); ++i) {
			(*iter_monster).Set_Player_Pos(p_X, p_Y, p_Z);
			(*iter_monster).Return_Set_Barrage();
			(*iter_monster).Move_Barrage();
			(*iter_monster).GetMonsterPosition(&m_X, &m_Y, &m_Z);
			if (player.Attack(m_X, m_Y, m_Z, (*iter_monster).Get_Monster_Type())) {
				if ((*iter_monster).beAttacked(player.Get_Damage())) {
					lt_monster.erase(iter_monster);
					iter_monster = lt_monster.begin();
					if (lt_monster.empty()) {
						break;
					}
				}
			}

			if ((*iter_monster).Attack(p_X, p_Y, p_Z) || (*iter_monster).Attack_Boss_Pattern(p_X, p_Y, p_Z)) {
				if (player.beAttacked((*iter_monster).Get_Damage())) {
					cout << "���� ����" << endl;
					now_stage_state = (int)STAGESTATE::RESTART;
					InitBuffer();
				}
			}
			if (lt_monster.size() > 0) {
				++iter_monster;
			}
		}
	}


	glutPostRedisplay(); // ȭ�� �� ���
	glutTimerFunc(1, TimerFunction, 1); // Ÿ�̸��Լ� �缳��
}

void KeyBoardUp(unsigned char key, int x, int y)
{

}

bool make_vertexShader()
{
	vertexSource = filetobuf("vertexShader.glvs");

	//--- ���ؽ� ���̴� ��ü �����
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//--- ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, 0);
	//--- ���ؽ� ���̴� �������ϱ�
	glCompileShader(vertexShader);
	//--- �������� ����� ���� ���� ���: ���� üũ
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);

	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader ������ ����\n" << errorLog << std::endl;
		return false;
	}

	return true;
}

bool make_fragmentShader()
{
	fragmentSource = filetobuf("fragment.glfs");

	//--- �����׸�Ʈ ���̴� ��ü �����
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//--- ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, 0);
	//--- �����׸�Ʈ ���̴� ������
	glCompileShader(fragmentShader);
	//--- �������� ����� ���� ���� ���: ������ ���� üũ
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);

	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR: fragment shader ������ ����\n" << errorLog << std::endl;
		return false;
	}

	return true;
}

void InitTexture()
{
	for (int i = 0; i < 3; ++i)
	{
		gameMenu[i].InitTexture(&shaderProgram, i);
		stagePotal[i].InitTexture(&shaderProgram);
		gameStage[i].InitTexture(&shaderProgram);
	}

	item_Box.InitTexture(&shaderProgram);
	item_Apple.InitTexture(&shaderProgram);
}

void InitBuffer()
{
	if (now_stage_state == (int)STAGESTATE::STARTMENU)
	{
		gameMenu[0].InitBuffer();
	}
	else if (now_stage_state == (int)STAGESTATE::RESTART)
	{
		gameMenu[1].InitBuffer();
	}
	else if (now_stage_state == (int)STAGESTATE::END)
	{
		gameMenu[2].InitBuffer();
	}

	//-------------------------------------------------------------------------------------------------
	lt_monster.clear();
	player.InitBuffer();
	if (now_stage_state == (int)STAGESTATE::ONE) {
		player.Next_Stage_Portal();
		player.Player_Hp_Full();
		gameStage[0].InitBuffer();
		stagePotal[0].InitBuffer();

		MONSTER monster_1((int)MONSTERTYPE::NOMAL,3,-3);
		lt_monster.push_back(monster_1);
		MONSTER monster_2((int)MONSTERTYPE::NOMAL, -3, -3);
		lt_monster.push_back(monster_2);
		MONSTER monster_3((int)MONSTERTYPE::NOMAL, 6, -6);
		lt_monster.push_back(monster_3);
		MONSTER monster_4((int)MONSTERTYPE::NOMAL, -6, -6);
		lt_monster.push_back(monster_4);
		MONSTER monster_5((int)MONSTERTYPE::NOMAL, 0, -6);
		lt_monster.push_back(monster_5);
		MONSTER monster_6((int)MONSTERTYPE::NOMAL, 0, 0);
		lt_monster.push_back(monster_6);
		MONSTER monster_7((int)MONSTERTYPE::NOMAL, 6, 0);
		lt_monster.push_back(monster_7);
		MONSTER monster_8((int)MONSTERTYPE::NOMAL, -6, 0);
		lt_monster.push_back(monster_8);
		MONSTER monster_9((int)MONSTERTYPE::NOMAL, 3, 3);
		lt_monster.push_back(monster_9);
		MONSTER monster_10((int)MONSTERTYPE::NOMAL, -3, 3);
		lt_monster.push_back(monster_10);
	}
	else if (now_stage_state == (int)STAGESTATE::TWO) {
		player.Next_Stage_Portal();
		gameStage[1].InitBuffer();
		stagePotal[1].InitBuffer();

		MONSTER monster_1((int)MONSTERTYPE::ELITE, 3, -3);
		lt_monster.push_back(monster_1);
		MONSTER monster_2((int)MONSTERTYPE::ELITE, -3, -3);
		lt_monster.push_back(monster_2);
		MONSTER monster_3((int)MONSTERTYPE::NOMAL, 6, -3);
		lt_monster.push_back(monster_3);
		MONSTER monster_4((int)MONSTERTYPE::NOMAL, -6, -3);
		lt_monster.push_back(monster_4);
		MONSTER monster_5((int)MONSTERTYPE::NOMAL, 4.5, 0);
		lt_monster.push_back(monster_5);
		MONSTER monster_6((int)MONSTERTYPE::NOMAL, -4.5, 0);
		lt_monster.push_back(monster_6);
		MONSTER monster_7((int)MONSTERTYPE::NOMAL, 1.5, 0);
		lt_monster.push_back(monster_7);
		MONSTER monster_8((int)MONSTERTYPE::NOMAL, -1.5, 0);
		lt_monster.push_back(monster_8);
	}
	else if (now_stage_state == (int)STAGESTATE::THREE) {
		player.Next_Stage_Portal();
		gameStage[2].InitBuffer();
		stagePotal[2].InitBuffer();

		item_Box.InitBuffer();
		item_Apple.InitBuffer();
		MONSTER monster((int)MONSTERTYPE::BOSS, 0, -3);
		lt_monster.push_back(monster);

	}

	//-------------------------------------------------------------------------------------------------
}

void InitShader()
{
	make_vertexShader();
	make_fragmentShader();
	//-- shader Program
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// checkCompileErrors(s_program, "PROGRAM"); // ���� �޽��� ����
	//--- ���̴� �����ϱ�
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//--- Shader Program ����ϱ�
	glUseProgram(shaderProgram);
}

void ConvertDeviceXYOpenGLXY(int x, int y, float* ox, float* oy)
{
	int w = WINDOW_WIDTH; // 800
	int h = WINDOW_HEIGHT; // 600

	*ox = (float)(x - (float)w / 2.0) * (float)(1.0 / (float)(w / 2.0));
	*oy = -(float)(y - (float)h / 2.0) * (float)(1.0 / (float)(h / 2.0));
}

void DrawProject(int now_stage_state)
{
	//--------------------------------------------------------------------------------------------------------------------------- [���� ��ȯ ����]
	// [3. ���� ��ȯ]
	glm::mat4 projection = glm::mat4(1.0f);

	if (now_stage_state == (int)STAGESTATE::STARTMENU || now_stage_state == (int)STAGESTATE::RESTART || now_stage_state == (int)STAGESTATE::END)
		projection = glm::perspective(glm::radians(90.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 50.0f); // ���� ��ȯ // projection = glm::ortho(0, 800, 600, 0);
	else
		projection = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 50.0f); // ���� ��ȯ

	unsigned int projectionLocation = glGetUniformLocation(shaderProgram, "projectionTransform"); //--- ���ؽ� ���̴����� projectionTransform ������ġ
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]); //--- projectionTransform ������ ��ȯ�� �����ϱ�
	//--------------------------------------------------------------------------------------------------------------------------- [���� ��ȯ ��]
}