#pragma once
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

#include <iostream>
#include <list>
#include <GL/glew.h> // �ʿ��� ������� include
#include "ReadObj.h"
#include "Barrage.h"

#define PLAYER_SPEED		0.21f
#define PLAYER_ATK			5
#define BARRAGE_CREATE_YPOS	0.5f

#define HEAD_SCALE			glm::vec3(0.25f, 0.25f, 0.25f)

#define HEAD_POS			glm::vec3(0.0f, 0.75f, 0.0f)
#define BODY_POS			glm::vec3(0.0f, 0.5f, 0.0f)

#define PLAYER_BODY_WIDTH	200
#define PLAYER_BODY_HEIGHT	150
#define PLAYER_BODY_Z		0.5f

#define PLAYER_FILE_NAME		"player_example.obj"

using namespace std;

// ��ǥ ���� ��� �� ������ ���� ��
typedef struct SHAPE {
	GLfloat position[3];
}SHAPE;

class PLAYER {
	// �÷��̾� ��ü�� �ֿ� ����
	GLuint mVao_Head;
	GLuint mVbo_Head[3];
	GLuint mEbo_Head;
	GLuint mVao_Body;
	GLuint mVbo_Body[2];
	GLuint mEbo_Body;
	GLuint mVao_ArmLeg;
	GLuint mVbo_ArmLeg[2];
	GLuint mEbo_ArmLeg;
	
	// ���� ��ǥ
	double m_x{ 0.0f};
	double m_y{ 0.0f };
	double m_z{ 9.0f };

	// ���� ���� ����
	int mMove_Dir;
	// �̵� ��ǥ ����
	glm::vec3 mMove_Player = glm::vec3(m_x, m_y, m_z);

	// �Ӹ��� ����ϴ� ��
	float* mVPosData;
	float* mVNormalData;
	float* mVTextureCoordinateData;
	int* mIndexData;
	int mVertexCount;
	int mIndexCount;

	// �÷��̾ �� ź�� ��ü ���� ����Ʈ
	list<BARRAGE> lt_Bar;
	list<BARRAGE>::iterator iter_Bar;
	GLfloat m_Rotate;

	// ���� ����ϴ� ����ü
	GLfloat body[8][3]{};
	GLfloat armLeg[8][3]{};


	// EBO ����ü�� ����
	unsigned int Rectindex[12][3]{
		// �ո� �ﰢ�� 1 2
		0, 2, 1,
		2, 3, 1,
		// ���� �ﰢ�� 3 4
		2, 6, 3,
		3, 6, 7,
		// �޸� �ﰢ�� 5 6
		6, 4, 7,
		7, 4, 5,
		// ������ �ﰢ�� 7 8
		0, 1, 5,
		4, 0, 5,
		// ���� �ﰢ�� 9 10
		1, 3, 5,
		3, 7, 5,
		// �Ʒ��� �ﰢ�� 11 12
		0, 4, 6,
		0, 6, 2
	};
	// ����ü�� ��� ��
	GLfloat Normalindex[36][3]{
		// �ո� �ﰢ�� 1 2
		0.0f, 0.0f, 1.0f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f, 1.0f,
		// ���� �ﰢ�� 3 4
		1.0f, 0.0f, 0.0f,	1.0f, 0.0f, 0.0f,	1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,	1.0f, 0.0f, 0.0f,	1.0f, 0.0f, 0.0f,
		// �޸� �ﰢ�� 5 6
		0.0f, 0.0f, -1.0f,	0.0f, 0.0f, -1.0f,	0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,	0.0f, 0.0f, -1.0f,	0.0f, 0.0f, -1.0f,
		// ������ �ﰢ�� 7 8
		-1.0f, 0.0f, 0.0f,	-1.0f, 0.0f, 0.0f,	-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,	-1.0f, 0.0f, 0.0f,	-1.0f, 0.0f, 0.0f,
		// ���� �ﰢ�� 9 10
		0.0f, 1.0f, 0.0f,	0.0f, 1.0f, 0.0f,	0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,	0.0f, 1.0f, 0.0f,	0.0f, 1.0f, 0.0f,
		// �Ʒ��� �ﰢ�� 11 12
		0.0f, -1.0f, 0.0f,	0.0f, -1.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,	0.0f, -1.0f, 0.0f,	0.0f, -1.0f, 0.0f
	};
	int barrage_count{};
	// �÷��̾��� ���� ��
	int mHp;
	double mSpeed;
	unsigned int Atk;
public:
	PLAYER(int movetype) { //������
		this->mMove_Dir = movetype;
		this->mHp = 100;
		this->Atk = 5;
		this->m_Rotate = 180;
	}
	~PLAYER() { // �Ҹ���

	}
	PLAYER(const PLAYER& player) { // ���� ������ ���� ����

	}
	void InitBuffer();
	void DrawScene(unsigned int* modelLocation, glm::mat4* WorldPosition);
	void Move(int moveType);
	void Move_Barrage();
	void Create_Barrage();															// �Ѿ� ����
	void DrawScene_Barrage(unsigned int* modelLocation, glm::mat4* WorldPosition);	// ������ �Ѿ˵� �׸��� 
	bool Attack(GLfloat x, GLfloat y, GLfloat z, int monstertype);
	void Next_Stage_Portal();
	void Return_Set_Barrage();
	void Player_Hp_Full();
	bool beAttacked(int damage);														// ��ǥ ����
	void InitData();																// ������ ��ǥ�� ��ġ
	void GetPlayerPosition(GLfloat* get_x, GLfloat* get_y, GLfloat* get_z);	// �÷��̾� ��ǥ�� �޾ƿ´�
	void PlayerTimerFunction();
	int Get_Damage();
};