#pragma once
#define DEBUG 1
#if DEBUG == 1
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

// 좌표 값과 노멀 값 데이터 정보 값
typedef struct SHAPE {
	GLfloat position[3];
}SHAPE;

class PLAYER {
	// 플레이어 객체의 주요 구성
	GLuint mVao_Head;
	GLuint mVbo_Head[3];
	GLuint mEbo_Head;
	GLuint mVao_Body;
	GLuint mVbo_Body[2];
	GLuint mEbo_Body;
	GLuint mVao_ArmLeg;
	GLuint mVbo_ArmLeg[2];
	GLuint mEbo_ArmLeg;
	
	// 현재 좌표
	double m_x{ 0.0f};
	double m_y{ 0.0f };
	double m_z{ 9.0f };

	// 현재 보는 방향
	int mMove_Dir;
	// 이동 좌표 저장
	glm::vec3 mMove_Player = glm::vec3(m_x, m_y, m_z);

	// 머리를 담당하는 원
	float* mVPosData;
	float* mVNormalData;
	float* mVTextureCoordinateData;
	int* mIndexData;
	int mVertexCount;
	int mIndexCount;

	// 플레이어가 쏜 탄막 객체 저장 리스트
	list<BARRAGE> lt_Bar;
	list<BARRAGE>::iterator iter_Bar;
	GLfloat m_Rotate;

	// 몸을 담당하는 육면체
	GLfloat body[8][3]{};
	GLfloat armLeg[8][3]{};


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
	int mHp;
	double mSpeed;
	unsigned int Atk;
public:
	PLAYER(int movetype) { //생성자
		this->mMove_Dir = movetype;
		this->mHp = 100;
		this->Atk = 5;
		this->m_Rotate = 180;
	}
	~PLAYER() { // 소멸자

	}
	PLAYER(const PLAYER& player) { // 복사 생성자 얆은 복사

	}
	void InitBuffer();
	void DrawScene(unsigned int* modelLocation, glm::mat4* WorldPosition);
	void Move(int moveType);
	void Move_Barrage();
	void Create_Barrage();															// 총알 생성
	void DrawScene_Barrage(unsigned int* modelLocation, glm::mat4* WorldPosition);	// 생성된 총알들 그리기 
	bool Attack(GLfloat x, GLfloat y, GLfloat z, int monstertype);
	void Next_Stage_Portal();
	void Return_Set_Barrage();
	void Player_Hp_Full();
	bool beAttacked(int damage);														// 좌표 설정
	void InitData();																// 설정된 좌표들 배치
	void GetPlayerPosition(GLfloat* get_x, GLfloat* get_y, GLfloat* get_z);	// 플레이어 좌표를 받아온다
	void PlayerTimerFunction();
	int Get_Damage();
};