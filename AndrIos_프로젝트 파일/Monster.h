#pragma once
#define DEBUG 1
#if DEBUG  == 1
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

#define MONSTER_FILE_NAME		"enemy.obj"
// ���͸��� ũ��
#define NOMAL_SCALE			glm::vec3(0.5f, 0.5f, 0.5f)
#define EILTE_SCALE			glm::vec3(0.75f, 0.75f, 0.75f)
#define BOSS_SCALE			glm::vec3(1.0f, 1.0f, 1.0f)
// ���͸��� Y����ǥ
#define NOMAL_YPOS			glm::vec3(0.0f, 0.5f, 0.0f)
#define EILTE_YPOS			glm::vec3(.0f, 0.25f, 0.0f)
#define BOSS_YPOS			glm::vec3(0.0f, 0.0f, 0.0f)
// ���͸��� ��
#define NOMAL_COLOR			glm::vec3(0.3f, 0.6f, 1.0f)
#define EILTE_COLOR			glm::vec3(0.9f, 0.7f, 0.0f)
#define BOSS_COLOR			glm::vec3(1.0f, 0.0f, 0.0f)



#define BARRAGE_CREATE_YPOS	0.5

using namespace std;



class MONSTER {
	// �÷��̾� ��ü�� �ֿ� ����
	GLuint mVao_Monster;
	GLuint mVbo_Monster[3];
	GLuint mEbo_Monster;

	// ���� ��ǥ
	GLfloat m_x{};
	GLfloat m_y{};
	GLfloat m_z{};

	// ���� �÷��̾� ��ǥ
	GLfloat p_x{};
	GLfloat p_y{};
	GLfloat p_z{};

	// �⺻ ����
	list<BARRAGE> lt_Bar;
	list<BARRAGE>::iterator iter_Bar;
	// ���� 1�� ���� ź��
	list<BARRAGE> lt_Elite_Bar;
	list<BARRAGE>::iterator iter_Elite_Bar;
	// ���� 2�� ���� ź��
	list<BARRAGE> lt_Boss_Bar;
	list<BARRAGE>::iterator iter_Boss_Bar;

	GLfloat m_Rotate{};

	// ���� ���� ����
	int mMove_Dir{1};
	
	// �̵� ��ǥ ����
	glm::vec3 mMove_Monster = glm::vec3(m_x, m_y, m_z);

	// �Ӹ��� ����ϴ� ��
	float* mVPosData;
	float* mVNormalData;
	float* mVTextureCoordinateData;
	int* mIndexData;
	int mVertexCount;
	int mIndexCount;

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
	unsigned int mHp;
	GLfloat mSpeed;
	unsigned int mAtk;
	int mMonsterType;
	int start_count{};
	int end_count{};
public :
	MONSTER(int monstertype, GLfloat x, GLfloat z) { //������
		this->m_x = x;
		this->m_z = z;
		if (monstertype == (int)MONSTERTYPE::NOMAL) {
			this->mHp = 100;
			this->mAtk = 10;
			this->mMonsterType = (int)MONSTERTYPE::NOMAL;
		}
		else if (monstertype == (int)MONSTERTYPE::ELITE) {
			this->mHp = 200;
			this->mAtk = 15;
			this->mMonsterType = (int)MONSTERTYPE::ELITE;
		}
		else if (monstertype == (int)MONSTERTYPE::BOSS) {
			this->mHp = 400;
			this->mAtk = 20;
			this->mMonsterType = (int)MONSTERTYPE::BOSS;
		}
	}
	~MONSTER() { // �Ҹ���
		lt_Bar.clear();
		lt_Boss_Bar.clear();
		lt_Elite_Bar.clear();
	}
	MONSTER(const MONSTER& monster) { // ���� ������ ���� ����

		this->m_x = monster.m_x;
		this->m_z = monster.m_z;
		this->mHp = monster.mHp;
		this->mAtk = monster.mAtk;
		this->mMonsterType = monster.mMonsterType;
		this->mMove_Monster = glm::vec3(this->m_x, 0.0f, this->m_z);
		
		if (this->mMonsterType == (int)MONSTERTYPE::NOMAL) {
			for (int i = 0; i < 30; ++i) {
				BARRAGE bar{ -100, BARRAGE_CREATE_YPOS , -100, this->mMove_Dir };
				lt_Bar.push_back(bar);
			}
		}
		else if (this->mMonsterType == (int)MONSTERTYPE::ELITE) {
			for (int i = 0; i < 6; ++i) {
				BARRAGE bar{ -100, BARRAGE_CREATE_YPOS , -100, this->mMove_Dir };
				lt_Bar.push_back(bar);
			}
			for (int i = 0; i < 24; ++i) {
				BARRAGE bar{ -100, BARRAGE_CREATE_YPOS , -100, this->mMove_Dir };
				lt_Elite_Bar.push_back(bar);
			}
		}
		else if (this->mMonsterType == (int)MONSTERTYPE::BOSS) {
			for (int i = 0; i < 15; ++i) {
				BARRAGE bar{ -100, BARRAGE_CREATE_YPOS , -100, this->mMove_Dir };
				lt_Bar.push_back(bar);
			}
			for (int i = 0; i < 15; ++i) {
				BARRAGE bar{ -100, BARRAGE_CREATE_YPOS , -100, this->mMove_Dir };
				lt_Boss_Bar.push_back(bar);
			}
		}

		ReadObj(MONSTER_FILE_NAME, mVPosData, mVNormalData, mVTextureCoordinateData, mIndexData, mVertexCount, mIndexCount);
		glGenVertexArrays(1, &mVao_Monster);
		glBindVertexArray(mVao_Monster);

		glGenBuffers(3, mVbo_Monster);
		glBindBuffer(GL_ARRAY_BUFFER, mVbo_Monster[0]);
		glBufferData(GL_ARRAY_BUFFER, mVertexCount * sizeof(float) * 3, mVPosData, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, NULL);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, mVbo_Monster[1]);
		glBufferData(GL_ARRAY_BUFFER, mVertexCount * sizeof(float) * 3, mVNormalData, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, NULL);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, mVbo_Monster[2]);
		glBufferData(GL_ARRAY_BUFFER, mVertexCount * sizeof(float) * 2, mVTextureCoordinateData, GL_STATIC_DRAW);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, NULL);
		glEnableVertexAttribArray(2);

		glGenBuffers(1, &mEbo_Monster);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo_Monster);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndexCount * sizeof(int), mIndexData, GL_STATIC_DRAW);
	}
	void DrawScene(unsigned int* modelLocation, glm::mat4* WorldPosition, unsigned int* objectColorLocation, GLfloat get_x, GLfloat get_y, GLfloat get_z);
	void DrawScene_Barrage(unsigned int* modelLocation, glm::mat4* WorldPosition);	// ������ �Ѿ˵� �׸��� 
	void Move_Barrage();
	bool Attack(GLfloat x, GLfloat y, GLfloat z);
	bool Attack_Elite_Pattern(GLfloat x, GLfloat y, GLfloat z);
	bool Attack_Boss_Pattern(GLfloat x, GLfloat y, GLfloat z);
	bool beAttacked(int damage);
	void Create_Barrage();
	void Elite_Pattern();
	void Boss_Pattern();
	void Return_Set_Barrage();
	void Set_Player_Pos(GLfloat x,GLfloat y,GLfloat z);
	void GetMonsterPosition(GLfloat* get_x, GLfloat* get_y, GLfloat* get_z);
	int Get_Damage();
	int Get_Monster_Type();
};