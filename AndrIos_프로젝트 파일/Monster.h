#pragma once
#define DEBUG 1
#if DEBUG  == 1
#include <GLM/glm.hpp> // glm 수학 라이브러리
#include <GLM/ext.hpp> // glm 수학 라이브러리
#include <GLM/gtc/matrix_transform.hpp> // glm 수학 라이브러리
#elif DEBUG == 2
#include <GL/glm/glm/glm.hpp> // glm 수학 라이브러리
#include <GL/glm/glm/ext.hpp> // glm 수학 라이브러리
#include <GL/glm/glm/gtc/matrix_transform.hpp> // glm 수학 라이브러리
#endif

#include <iostream>
#include <list>
#include <GL/glew.h> // 필요한 헤더파일 include
#include "ReadObj.h"
#include "Barrage.h"

#define MONSTER_FILE_NAME		"enemy.obj"
// 몬스터마다 크기
#define NOMAL_SCALE			glm::vec3(0.5f, 0.5f, 0.5f)
#define EILTE_SCALE			glm::vec3(0.75f, 0.75f, 0.75f)
#define BOSS_SCALE			glm::vec3(1.0f, 1.0f, 1.0f)
// 몬스터마다 Y축좌표
#define NOMAL_YPOS			glm::vec3(0.0f, 0.5f, 0.0f)
#define EILTE_YPOS			glm::vec3(.0f, 0.25f, 0.0f)
#define BOSS_YPOS			glm::vec3(0.0f, 0.0f, 0.0f)
// 몬스터마다 색
#define NOMAL_COLOR			glm::vec3(0.3f, 0.6f, 1.0f)
#define EILTE_COLOR			glm::vec3(0.9f, 0.7f, 0.0f)
#define BOSS_COLOR			glm::vec3(1.0f, 0.0f, 0.0f)



#define BARRAGE_CREATE_YPOS	0.5

using namespace std;



class MONSTER {
	// 플레이어 객체의 주요 구성
	GLuint mVao_Monster;
	GLuint mVbo_Monster[3];
	GLuint mEbo_Monster;

	// 현재 좌표
	GLfloat m_x{};
	GLfloat m_y{};
	GLfloat m_z{};

	// 현재 플레이어 좌표
	GLfloat p_x{};
	GLfloat p_y{};
	GLfloat p_z{};

	// 기본 공격
	list<BARRAGE> lt_Bar;
	list<BARRAGE>::iterator iter_Bar;
	// 패턴 1에 사용될 탄막
	list<BARRAGE> lt_Elite_Bar;
	list<BARRAGE>::iterator iter_Elite_Bar;
	// 패턴 2에 사용될 탄막
	list<BARRAGE> lt_Boss_Bar;
	list<BARRAGE>::iterator iter_Boss_Bar;

	GLfloat m_Rotate{};

	// 현재 보는 방향
	int mMove_Dir{1};
	
	// 이동 좌표 저장
	glm::vec3 mMove_Monster = glm::vec3(m_x, m_y, m_z);

	// 머리를 담당하는 원
	float* mVPosData;
	float* mVNormalData;
	float* mVTextureCoordinateData;
	int* mIndexData;
	int mVertexCount;
	int mIndexCount;

	// EBO 육면체의 순서
	unsigned int Rectindex[12][3]{
		// 앞면 삼각형 1 2
		0, 2, 1,
		2, 3, 1,
		// 왼쪽 삼각형 3 4
		2, 6, 3,
		3, 6, 7,
		// 뒷면 삼각형 5 6
		6, 4, 7,
		7, 4, 5,
		// 오른쪽 삼각형 7 8
		0, 1, 5,
		4, 0, 5,
		// 위쪽 삼각형 9 10
		1, 3, 5,
		3, 7, 5,
		// 아래쪽 삼각형 11 12
		0, 4, 6,
		0, 6, 2
	};

	// 육면체의 노멀 값
	GLfloat Normalindex[36][3]{
		// 앞면 삼각형 1 2
		0.0f, 0.0f, 1.0f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f, 1.0f,
		// 왼쪽 삼각형 3 4
		1.0f, 0.0f, 0.0f,	1.0f, 0.0f, 0.0f,	1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,	1.0f, 0.0f, 0.0f,	1.0f, 0.0f, 0.0f,
		// 뒷면 삼각형 5 6
		0.0f, 0.0f, -1.0f,	0.0f, 0.0f, -1.0f,	0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,	0.0f, 0.0f, -1.0f,	0.0f, 0.0f, -1.0f,
		// 오른쪽 삼각형 7 8
		-1.0f, 0.0f, 0.0f,	-1.0f, 0.0f, 0.0f,	-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,	-1.0f, 0.0f, 0.0f,	-1.0f, 0.0f, 0.0f,
		// 위쪽 삼각형 9 10
		0.0f, 1.0f, 0.0f,	0.0f, 1.0f, 0.0f,	0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,	0.0f, 1.0f, 0.0f,	0.0f, 1.0f, 0.0f,
		// 아래쪽 삼각형 11 12
		0.0f, -1.0f, 0.0f,	0.0f, -1.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,	0.0f, -1.0f, 0.0f,	0.0f, -1.0f, 0.0f
	};

	int barrage_count{};
	// 플레이어의 정보 값
	unsigned int mHp;
	GLfloat mSpeed;
	unsigned int mAtk;
	int mMonsterType;
	int start_count{};
	int end_count{};
public :
	MONSTER(int monstertype, GLfloat x, GLfloat z) { //생성자
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
	~MONSTER() { // 소멸자
		lt_Bar.clear();
		lt_Boss_Bar.clear();
		lt_Elite_Bar.clear();
	}
	MONSTER(const MONSTER& monster) { // 복사 생성자 얆은 복사

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
	void DrawScene_Barrage(unsigned int* modelLocation, glm::mat4* WorldPosition);	// 생성된 총알들 그리기 
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