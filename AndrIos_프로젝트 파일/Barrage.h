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

#define BARRAGE_SPEED			0.2
#define BARRAGE_SCALE			glm::vec3(0.3f, 0.3f, 0.3f)
#define BARRAGE_POS				glm::vec3(0.0f, 0.55f, 0.0f)

#define NOMAL_SIZE			0.5f
#define EILTE_SIZE			0.5f
#define BOSS_SIZE			1.0f
#define BARRAGE_SIZE		0.3f

#define PLAYER_SIZE			0.25f

#define BULLET_FILE_NAME	"bullet.obj"

using namespace std;

enum MOVETYPE { UP, DOWN, LEFT, RIGHT };

enum class MONSTERTYPE { NOMAL, ELITE, BOSS };
enum class CHARACTERTYPE { PLAYER, MONSTER, NON};
enum class BARRAGETYPE { SHOT,PATTERN };

class BARRAGE {
	// vao, vbo, ebo ��
	GLuint mVao;
	GLuint mVbo[3];
	GLuint mEbo;

	GLfloat m_rotate;

	// ���� ��ǥ
	glm::vec3 mCreate_Position = glm::vec3(0.0f);
	double m_x;
	double m_z;
	double m_y;
	double st_x;
	double st_z;
	double st_y;
	// Ÿ�� ��ǥ
	double t_x;
	double t_z;
	double t_y;

	// �Ѿ� �𵨸�(��)
	float* mVPosData;
	float* mVNormalData;
	float* mVTextureCoordinateData;
	int* mIndexData;
	int mVertexCount;
	int mIndexCount;

	// �Ѿ˿� ���� ����(���ǵ�, �����, ���ư��� ����)
	int mMove_Dir;
	//
	bool shot_check{};
	int mCharacter_Type;
	int mMonster_Type;
	int mBarrage_Type;
	bool pattern_check{};
public :
	BARRAGE(GLfloat x, GLfloat y, GLfloat z, int moveDir) {
		//�÷��̾� ��ǥ�� ����
		this->mMove_Dir = moveDir;
		this->m_x = x;
		this->m_y = y;
		this->m_z = z;
		this->mCreate_Position = glm::vec3(x,y,z);	
		if (this->mMove_Dir == MOVETYPE::UP) {
			this->m_rotate = 0;
		}
		else if (this->mMove_Dir == MOVETYPE::DOWN) {
			this->m_rotate = 180;
		}
		else if (this->mMove_Dir == MOVETYPE::LEFT) {
			this->m_rotate = 90;
		}
		else if (this->mMove_Dir == MOVETYPE::RIGHT) {
			this->m_rotate = -90;
		}
	}
	~BARRAGE() {

	}
	BARRAGE(const BARRAGE& barr) {
		this->mMove_Dir = barr.mMove_Dir;
		this->m_x = barr.m_x;
		this->m_y = barr.m_y;
		this->m_z = barr.m_z;
		this->mCreate_Position = barr.mCreate_Position;
		this->m_rotate = barr.m_rotate;
		this->mCharacter_Type = (int)CHARACTERTYPE::NON;
		ReadObj(BULLET_FILE_NAME, mVPosData, mVNormalData, mVTextureCoordinateData, mIndexData, mVertexCount, mIndexCount);
		
		glGenVertexArrays(1, &mVao);
		glBindVertexArray(mVao);

		glGenBuffers(3, mVbo);

		glBindBuffer(GL_ARRAY_BUFFER, mVbo[0]);
		glBufferData(GL_ARRAY_BUFFER, mVertexCount * sizeof(float) * 3, mVPosData, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, NULL);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, mVbo[1]);
		glBufferData(GL_ARRAY_BUFFER, mVertexCount * sizeof(float) * 3, mVNormalData, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, NULL);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, mVbo[2]);
		glBufferData(GL_ARRAY_BUFFER, mVertexCount * sizeof(float) * 2, mVTextureCoordinateData, GL_STATIC_DRAW);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, NULL);
		glEnableVertexAttribArray(2);

		glGenBuffers(1, &mEbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndexCount * sizeof(int), mIndexData, GL_STATIC_DRAW);
	}
	void DrawScene(unsigned int* modelLocation, glm::mat4* WorldPosition);
	GLfloat GetBarrageSpeed();
	void Set_Player_Shot(bool shot_check, int move_dir, GLfloat m_x, GLfloat m_y, GLfloat m_z);
	void Set_Monster_Shot(bool shot_check, GLfloat m_x, GLfloat m_y, GLfloat m_z, GLfloat p_x, GLfloat p_y, GLfloat p_z);
	void Set_Monster_Pattern_Shot(bool shot_check, GLfloat m_x, GLfloat m_y, GLfloat m_z, GLfloat p_x, GLfloat p_y, GLfloat p_z);
	void Set_Monster_type(int monstertype);
	bool Return_Set_Barrage();
	void Atk_Return_Set_Barrage();
	void Move();
	bool Get_Shotcheck();
	void GetBarragePosition(GLfloat* get_x, GLfloat* get_y, GLfloat* get_z);
};