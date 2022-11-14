#pragma once
#include "Monster.h"

void MONSTER::DrawScene(unsigned int* modelLocation, glm::mat4* WorldPosition, unsigned int* objectColorLocation,GLfloat x, GLfloat y,GLfloat z) {
	glFrontFace(GL_CCW);	// 반시계 방향을 앞면으로 인식

	glm::mat4 Trans_move = glm::translate(glm::mat4(1.0f), mMove_Monster);


	glm::vec3 objColor = glm::vec3(1.0f);
	glBindVertexArray(mVao_Monster);
	glm::mat4 scaleMatrix = glm::mat4(1.0f);
	glm::mat4 rotateMatrix = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 Matrix = glm::mat4(1.0f);
	glm::vec3 playerPos = glm::vec3(x, y, z);
	glm::mat4 trans_monster = glm::mat4(1.0f);
	if (this->mMonsterType == (int)MONSTERTYPE::NOMAL) {
		scaleMatrix = glm::scale(glm::mat4(1.0f), NOMAL_SCALE);
		trans_monster = glm::translate(glm::mat4(1.0f), NOMAL_YPOS);
		objColor = NOMAL_COLOR;
	}
	else if (this->mMonsterType == (int)MONSTERTYPE::ELITE) {
		scaleMatrix = glm::scale(glm::mat4(1.0f), EILTE_SCALE);
		trans_monster = glm::translate(glm::mat4(1.0f), EILTE_YPOS);
		if (this->m_x > 0.0f) {
			Trans_move = Trans_move * glm::translate(glm::mat4(1.0f), glm::vec3(-1.75f, 0.0f, 1.75f));
		}
		else if (this->m_x < 0.0f) {
			Trans_move = Trans_move * glm::translate(glm::mat4(1.0f), glm::vec3(1.75f, 0.0f, 1.75f));
		}
		objColor = EILTE_COLOR;
	}
	else if (this->mMonsterType == (int)MONSTERTYPE::BOSS) {
		Trans_move = Trans_move * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 2.75f));
		scaleMatrix = glm::scale(glm::mat4(1.0f), BOSS_SCALE);
		trans_monster = glm::translate(glm::mat4(1.0f), BOSS_YPOS);
		objColor = BOSS_COLOR;
	}
	view = glm::inverse(glm::lookAt(this->mMove_Monster, playerPos, glm::vec3(0.0f, 1.0f, 0.0f))) * glm::rotate(glm::mat4(1.0f),glm::radians(180.0f),glm::vec3(0.0f,1.0f,0.0f));
	*WorldPosition = scaleMatrix * Trans_move  * trans_monster  * view;
	glUniformMatrix4fv(*modelLocation, 1, GL_FALSE, glm::value_ptr(*WorldPosition)); //--- modelTransform 변수에 변환 값 적용하기
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glUniform3fv(*objectColorLocation, 1, (float*)&objColor);
	glDrawElements(GL_TRIANGLES, mIndexCount, GL_UNSIGNED_INT, 0);
}

void MONSTER::DrawScene_Barrage(unsigned int* modelLocation, glm::mat4* WorldPosition) {
	this->iter_Bar = this->lt_Bar.begin();
	for (int i = 0; i < this->lt_Bar.size(); ++i) {
		(*iter_Bar).DrawScene(modelLocation, WorldPosition);
		++this->iter_Bar;
	}
	this->iter_Boss_Bar = this->lt_Boss_Bar.begin();
	for (int i = 0; i < this->lt_Boss_Bar.size(); ++i) {
		(*iter_Boss_Bar).DrawScene(modelLocation, WorldPosition);
		++this->iter_Boss_Bar;
	}
}

bool MONSTER::Attack(GLfloat x, GLfloat y, GLfloat z) {
	GLfloat b_x, b_y, b_z;

	this->iter_Bar = this->lt_Bar.begin();
	for (int i = 0; i < this->lt_Bar.size(); ++i) {
		(*iter_Bar).GetBarragePosition(&b_x, &b_y, &b_z);
		if (b_x >= x - PLAYER_SIZE && b_x <= x + PLAYER_SIZE &&
			b_z >= z - PLAYER_SIZE && b_z <= z + PLAYER_SIZE) {
			(*iter_Bar).Atk_Return_Set_Barrage();
			return true;
			break;
		}
		++this->iter_Bar;
	}
	return false;
}

bool MONSTER::beAttacked(int damage) {
	this->mHp -= damage;
	cout << this->mHp << endl;
	if (this->mHp <= 0) {
		return true;
	}
	return false;
}

int MONSTER::Get_Damage() {
	return this->mAtk;
}


void MONSTER::GetMonsterPosition(GLfloat* get_x, GLfloat* get_y, GLfloat* get_z) {
	*get_x = m_x;
	*get_y = m_y;
	*get_z = m_z;
}

void MONSTER::Create_Barrage() {
	++this->barrage_count;
	if (this->mMonsterType == (int)MONSTERTYPE::NOMAL) {
		if (this->barrage_count == 30) {
			this->barrage_count = 0;
		}
	}
	else if (this->mMonsterType == (int)MONSTERTYPE::ELITE) {
		if (this->barrage_count == 6) {
			this->barrage_count = 0;
		}
	}
	else if (this->mMonsterType == (int)MONSTERTYPE::BOSS) {
		if (this->barrage_count == 15) {
			this->barrage_count = 0;
		}
	}
	iter_Bar = lt_Bar.begin();
	for (int i = 1; i < this->barrage_count; ++i) {
		++iter_Bar;
	}
	(*iter_Bar).Set_Monster_type(this->mMonsterType);
	(*iter_Bar).Set_Monster_Shot(true, this->m_x, this->m_y, this->m_z,this->p_x,this->p_y,this->p_z);
}

int MONSTER::Get_Monster_Type() {
	return this->mMonsterType;
}

void MONSTER::Set_Player_Pos(GLfloat x, GLfloat y, GLfloat z) {
	this->p_x = x;
	this->p_y = y;
	this->p_z = z;
}

void MONSTER::Return_Set_Barrage() {
	this->iter_Bar = this->lt_Bar.begin();

	for (int i = 0; i < this->lt_Bar.size(); ++i) {
		(*iter_Bar).Return_Set_Barrage();
		++this->iter_Bar;
	}
	this->iter_Boss_Bar = this->lt_Boss_Bar.begin();
	for (int i = 0; i < this->lt_Boss_Bar.size(); ++i) {
		(*iter_Boss_Bar).Return_Set_Barrage();
		++this->iter_Boss_Bar;
	}
}

void MONSTER::Move_Barrage() {
	this->iter_Bar = this->lt_Bar.begin();
	for (int i = 0; i < this->lt_Bar.size(); ++i) {
		(*iter_Bar).Move();
		++iter_Bar;
	}
	this->iter_Boss_Bar = this->lt_Boss_Bar.begin();
	for (int i = 0; i < this->lt_Boss_Bar.size(); ++i) {
		(*iter_Boss_Bar).Move();
		++iter_Boss_Bar;
	}
}

bool MONSTER::Attack_Elite_Pattern(GLfloat x, GLfloat y, GLfloat z) {
	return false;
}

void MONSTER::Elite_Pattern() {

}

bool MONSTER::Attack_Boss_Pattern(GLfloat x, GLfloat y, GLfloat z) {
	GLfloat b_x, b_y, b_z;

	this->iter_Boss_Bar = this->lt_Boss_Bar.begin();
	for (int i = 0; i < this->lt_Boss_Bar.size(); ++i) {
		(*iter_Boss_Bar).GetBarragePosition(&b_x, &b_y, &b_z);
		if (b_x >= x - PLAYER_SIZE && b_x <= x + PLAYER_SIZE &&
			b_z >= z - PLAYER_SIZE && b_z <= z + PLAYER_SIZE) {
			(*iter_Boss_Bar).Atk_Return_Set_Barrage();
			return true;
			break;
		}
		++this->iter_Boss_Bar;
	}
	return false;
}

void MONSTER::Boss_Pattern() {
	if (this->mMonsterType == (int)MONSTERTYPE::BOSS) {
		this->iter_Boss_Bar = this->lt_Boss_Bar.begin();
		if (start_count == 10) {
			start_count = 0;
		}
		this->end_count = start_count + 5;
		for (int i = 0; i < this->end_count; ++i) {
			if (i <= end_count && i >= start_count) {
				if (i == start_count)(*iter_Boss_Bar).Set_Monster_Pattern_Shot(true, this->m_x, this->m_y, this->m_z, this->p_x, this->p_y, this->p_z);
				else if (i == start_count + 1)(*iter_Boss_Bar).Set_Monster_Pattern_Shot(true, this->m_x - 0.3, this->m_y, this->m_z, this->p_x, this->p_y, this->p_z);
				else if (i == start_count + 2)(*iter_Boss_Bar).Set_Monster_Pattern_Shot(true, this->m_x + 0.3, this->m_y, this->m_z, this->p_x, this->p_y, this->p_z);
				else if (i == start_count + 3)(*iter_Boss_Bar).Set_Monster_Pattern_Shot(true, this->m_x - 0.6, this->m_y, this->m_z, this->p_x, this->p_y, this->p_z);
				else if (i == start_count + 4)(*iter_Boss_Bar).Set_Monster_Pattern_Shot(true, this->m_x + 0.6, this->m_y, this->m_z, this->p_x, this->p_y, this->p_z);
				(*iter_Boss_Bar).Set_Monster_type((int)MONSTERTYPE::BOSS);
			}
			++iter_Boss_Bar;
		}
		start_count += 5;
	}
}