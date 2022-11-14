#pragma once
#include "Player.h"

#define STAGE_SIZE 10.0f

void PLAYER::DrawScene(unsigned int* modelLocation, glm::mat4* WorldPosition) {
	glFrontFace(GL_CCW);	// 반시계 방향을 앞면으로 인식

	glm::mat4 Trans_move = glm::translate(glm::mat4(1.0f), mMove_Player);
	glm::mat4 Rotate_move = glm::rotate(glm::mat4(1.0f), glm::radians(this->m_Rotate), glm::vec3(0.0f, 1.0f, 0.0f));


	//몸 그리기
	glBindVertexArray(mVao_Head);
	glm::mat4 scaleMatrix = glm::mat4(1.0f);
	glm::mat4 trans_Head = glm::mat4(1.0f);
	scaleMatrix = glm::scale(glm::mat4(1.0f), HEAD_SCALE);
	trans_Head = glm::translate(glm::mat4(1.0f), HEAD_POS);
	*WorldPosition = Trans_move * scaleMatrix * trans_Head * Rotate_move;
	glUniformMatrix4fv(*modelLocation, 1, GL_FALSE, glm::value_ptr(*WorldPosition)); //--- modelTransform 변수에 변환 값 적용하기
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawElements(GL_TRIANGLES, mIndexCount, GL_UNSIGNED_INT, 0);

}

void PLAYER::InitData() {
	ReadObj(PLAYER_FILE_NAME, mVPosData, mVNormalData, mVTextureCoordinateData, mIndexData, mVertexCount, mIndexCount);

	for (int i = 0; i < 31; ++i) {
		BARRAGE bar{ -100.0f, BARRAGE_CREATE_YPOS , -100.0f, this->mMove_Dir };
		lt_Bar.push_back(bar);
	}

}

void PLAYER::InitBuffer() {
	// 몸 객체
	glGenVertexArrays(1, &mVao_Head);
	glBindVertexArray(mVao_Head);

	glGenBuffers(3, mVbo_Head);

	glBindBuffer(GL_ARRAY_BUFFER, mVbo_Head[0]);
	glBufferData(GL_ARRAY_BUFFER, mVertexCount * sizeof(float) * 3, mVPosData, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, mVbo_Head[1]);
	glBufferData(GL_ARRAY_BUFFER, mVertexCount * sizeof(float) * 3, mVNormalData, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, NULL);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, mVbo_Head[2]);
	glBufferData(GL_ARRAY_BUFFER, mVertexCount * sizeof(float) * 2, mVTextureCoordinateData, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, NULL);
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &mEbo_Head);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo_Head);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndexCount * sizeof(int), mIndexData, GL_STATIC_DRAW);


}

void PLAYER::Move_Barrage() {
	this->iter_Bar = this->lt_Bar.begin();
	for (int i = 0; i < this->lt_Bar.size(); ++i) {
		(*iter_Bar).Move();
		++iter_Bar;
	}
}

void PLAYER::Move(int movetype) {
	if (movetype == MOVETYPE::UP && this->m_z > -10.0f + PLAYER_SIZE + PLAYER_SIZE) {
		this->m_z -= PLAYER_SPEED;
		this->m_Rotate = 180;
	}
	else if (movetype == MOVETYPE::DOWN && this->m_z < 10.0f - PLAYER_SIZE - PLAYER_SIZE) {
		this->m_z += PLAYER_SPEED;
		this->m_Rotate = 0;
	}
	else if (movetype == MOVETYPE::LEFT && this->m_x > -10.0f + PLAYER_SIZE + PLAYER_SIZE) {
		this->m_x -= PLAYER_SPEED;
		this->m_Rotate = -90;
	}
	else if (movetype == MOVETYPE::RIGHT && this->m_x < 10.0f - PLAYER_SIZE - PLAYER_SIZE) {
		this->m_x += PLAYER_SPEED;
		this->m_Rotate = 90;
	}
	this->mMove_Dir = movetype;
	this->mMove_Player = glm::vec3(this->m_x, this->m_y, this->m_z);
}

void PLAYER::DrawScene_Barrage(unsigned int* modelLocation, glm::mat4* WorldPosition) {
	this->iter_Bar = this->lt_Bar.begin();
	for (int i = 0; i < this->lt_Bar.size(); ++i) {
		(*iter_Bar).DrawScene(modelLocation, WorldPosition);
		++this->iter_Bar;
	}
}

void PLAYER::Return_Set_Barrage() {
	iter_Bar = lt_Bar.begin();
	for (int i = 0; i < this->lt_Bar.size(); ++i) {
		(*iter_Bar).Return_Set_Barrage();
		++this->iter_Bar;
	}
}

int PLAYER::Get_Damage() {
	return this->Atk;
}

void PLAYER::Create_Barrage() {
	++this->barrage_count;
	if (this->barrage_count == 31) {
		this->barrage_count = 0;
	}
	iter_Bar = lt_Bar.begin();
	for (int i = 1; i < this->barrage_count; ++i) {
		++iter_Bar;
	}
	(*iter_Bar).Set_Player_Shot(true, this->mMove_Dir, this->m_x, this->m_y, this->m_z);
}

bool PLAYER::Attack(GLfloat x, GLfloat y, GLfloat z, int monstertype) {
	// 내가 쏜 탄막이 적과 충돌하였는가 
	GLfloat b_x, b_y, b_z;
	GLfloat monster_Size{};
	if (monstertype == (int)MONSTERTYPE::NOMAL) {
		monster_Size = NOMAL_SIZE;
	}
	else if (monstertype == (int)MONSTERTYPE::ELITE) {
		monster_Size = EILTE_SIZE;
	}
	else if (monstertype == (int)MONSTERTYPE::BOSS) {
		monster_Size = BOSS_SIZE;
	}
	this->iter_Bar = this->lt_Bar.begin();
	for (int i = 0; i < this->lt_Bar.size(); ++i) {
		(*iter_Bar).GetBarragePosition(&b_x, &b_y, &b_z);
		if (b_x >= x - monster_Size && b_x <= x + monster_Size &&
			b_z >= z - monster_Size && b_z <= z + monster_Size)
			{
				(*iter_Bar).Atk_Return_Set_Barrage();
				return true;
				break;
		}
		++this->iter_Bar;
	}

	return false;
}
bool PLAYER::beAttacked(int damage) {	//플레이어의 피격
	this->mHp -= damage;
	cout << "현재 HP" << this->mHp << endl;
	if (this->mHp <= 0) {
		return true;
	}
	return false;
}

void PLAYER::GetPlayerPosition(GLfloat* get_x, GLfloat* get_y, GLfloat* get_z)	// 플레이어 좌표를 받아온다
{
	*get_x = m_x;
	*get_y = m_y;
	*get_z = m_z;
}

void PLAYER::PlayerTimerFunction()
{
	this->Move_Barrage();
	this->Return_Set_Barrage();
}

void PLAYER::Next_Stage_Portal() {
	this->m_x = 0.0f;
	this->m_z = 9.0f;
	this->mMove_Player = glm::vec3(m_x, 0.0f, m_z);
}

void PLAYER::Player_Hp_Full() {
	this->mHp = 100;
}