#include "Barrage.h"

void BARRAGE::DrawScene(unsigned int* modelLocation, glm::mat4* WorldPosition) {
	glFrontFace(GL_CCW);	// 반시계 방향을 앞면으로 인식


	glm::mat4 Trans_move = glm::translate(glm::mat4(1.0f), glm::vec3(this->m_x,this->m_y,this->m_z));
	glm::mat4 Rotate_move = glm::rotate(glm::mat4(1.0f), glm::radians(this->m_rotate), glm::vec3(0.0f, 1.0f, 0.0f));

	glBindVertexArray(mVao);
	glm::mat4 scaleMatrix = glm::mat4(1.0f);
	glm::mat4 trans_Barrrage = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::vec3 barragePos = glm::vec3(this->m_x, this->m_y, this->m_z);
	glm::vec3 playerPos = glm::vec3(this->t_x, this->t_y, this->t_z);
	view = glm::inverse(glm::lookAt(barragePos, playerPos, glm::vec3(0.0f, 1.0f, 0.0f)));
	scaleMatrix = glm::scale(glm::mat4(1.0f), BARRAGE_SCALE);
	trans_Barrrage = glm::translate(glm::mat4(1.0f), BARRAGE_POS);
	if (this->mCharacter_Type == (int)CHARACTERTYPE::PLAYER || this->mCharacter_Type == (int)CHARACTERTYPE::NON) {
		*WorldPosition = Trans_move * scaleMatrix * trans_Barrrage * Rotate_move;
	}
	else if (this->mCharacter_Type == (int)CHARACTERTYPE::MONSTER) {
		if (this->mBarrage_Type == (int)BARRAGETYPE::SHOT) {
			*WorldPosition = Trans_move * scaleMatrix * trans_Barrrage * Rotate_move;
			//*WorldPosition = scaleMatrix * Trans_move * trans_Barrrage * view;
		}
		else if (this->mBarrage_Type == (int)BARRAGETYPE::PATTERN) {
			*WorldPosition = Trans_move * scaleMatrix * trans_Barrrage * Rotate_move;
		}
	}
	glUniformMatrix4fv(*modelLocation, 1, GL_FALSE, glm::value_ptr(*WorldPosition)); //--- modelTransform 변수에 변환 값 적용하기
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawElements(GL_TRIANGLES, mIndexCount, GL_UNSIGNED_INT, 0);

}

void BARRAGE::GetBarragePosition(GLfloat* get_x, GLfloat* get_y, GLfloat* get_z) {
	*get_x = m_x;
	*get_y = m_y;
	*get_z = m_z;
}

bool BARRAGE::Get_Shotcheck() {
	if (this->shot_check) {
		return true;
	}
	else {
		return false;
	}
}

void BARRAGE::Move() {
	if (this->shot_check) {
		if (this->mCharacter_Type == (int)CHARACTERTYPE::MONSTER) {
			GLfloat size{};
			if (this->mMonster_Type == (int)MONSTERTYPE::NOMAL) {
				size = 0.2;
			}
			else if (this->mMonster_Type == (int)MONSTERTYPE::ELITE) {
				size = 0.5;
			}
			else if (this->mMonster_Type == (int)MONSTERTYPE::BOSS) {
				size = 0.8;
			}
			if (t_x >= st_x && t_z >= st_z) {
				if ( (t_x - size <= st_x && t_x + size >= st_x) && t_z != st_z) {
					this->m_z += BARRAGE_SPEED;
					this->m_rotate = 180.0f;
				}
				else if (t_x != m_x && (t_z - size <= st_z && t_z + size >= st_z)) {
					this->m_x += BARRAGE_SPEED;
					this->m_rotate = -90.0f;
				}
				else if (t_x != m_x && t_z != st_z) {
					this->m_x += BARRAGE_SPEED / 2.0f;
					this->m_z += BARRAGE_SPEED / 2.0f;
					this->m_rotate = -135.0f;
				}
			}
			else if(t_x >= st_x && t_z <= st_z){
				if ((t_x - size <= st_x && t_x + size >= st_x) && t_z != st_z) {
					this->m_z -= BARRAGE_SPEED;
					this->m_rotate = 0;
				}
				else if (t_x != st_x && (t_z - size <= st_z && t_z + size >= st_z)) {
					this->m_x += BARRAGE_SPEED;
					this->m_rotate = -90.0f;
				}
				else if (t_x != st_x && t_z != st_z) {
					this->m_x += BARRAGE_SPEED / 2.0f;
					this->m_z -= BARRAGE_SPEED / 2.0f;
					this->m_rotate = -405.0f;
				}
			}
			else if (t_x <= st_x && t_z >= st_z) {
				if ((t_x - size <= st_x && t_x + size >= st_x) && t_z != st_z) {
					this->m_z += BARRAGE_SPEED;
					this->m_rotate = 180.0f;
				}
				else if (t_x != st_x && (t_z - size <= st_z && t_z + size >= st_z)) {
					this->m_x -= BARRAGE_SPEED;
					this->m_rotate = 90;
				}
				else if (t_x != st_x && t_z != st_z) {
					this->m_x -= BARRAGE_SPEED / 2.0f;
					this->m_z += BARRAGE_SPEED / 2.0f;
					this->m_rotate = -225.0f;
				}
			}
			else if(t_x <= st_x && t_z <= st_z){
				if ((t_x - size <= st_x && t_x + size >= st_x) && t_z != st_z) {
					this->m_z -= BARRAGE_SPEED;
					this->m_rotate = 0;
				}
				else if (t_x != st_x && (t_z - size <= st_z && t_z + size >= st_z)) {
					this->m_x -= BARRAGE_SPEED;
					this->m_rotate = 90;
				}
				else if (t_x != st_x && t_z != st_z) {
					this->m_x -= BARRAGE_SPEED / 2.0f;
					this->m_z -= BARRAGE_SPEED / 2.0f;
					this->m_rotate = -315.0f;
				}
			}
		}
		else {
			if (this->mMove_Dir == MOVETYPE::UP) {
				this->m_z -= BARRAGE_SPEED;
				this->m_rotate = 0;
			}
		else if (this->mMove_Dir == MOVETYPE::DOWN) {
			this->m_z += BARRAGE_SPEED;
			this->m_rotate = 180;
			}
		else if (this->mMove_Dir == MOVETYPE::LEFT) {
			this->m_x -= BARRAGE_SPEED;
			this->m_rotate = 90;
			}
		else if (this->mMove_Dir == MOVETYPE::RIGHT) {
			this->m_x += BARRAGE_SPEED;
			this->m_rotate = -90;
			}
		}
	}
	this->mCreate_Position = glm::vec3(this->m_x, this->m_y, this->m_z);
}

GLfloat BARRAGE::GetBarrageSpeed() {
	return BARRAGE_SPEED;
}

void BARRAGE::Set_Player_Shot(bool shot_check, int move_dir, GLfloat m_x, GLfloat m_y,GLfloat m_z) {
	this->shot_check = shot_check;
	this->mMove_Dir = move_dir;
	this->m_x = m_x;
	this->m_z = m_z;
	this->m_y = 0.55f;
	this->mCreate_Position = glm::vec3(this->m_x, 0.55f, this->m_z);
	this->mCharacter_Type = (int)CHARACTERTYPE::PLAYER;
	this->mBarrage_Type = (int)BARRAGETYPE::SHOT;
}

void BARRAGE::Set_Monster_Shot(bool shot_check, GLfloat m_x, GLfloat m_y, GLfloat m_z, GLfloat p_x, GLfloat p_y, GLfloat p_z) {
	this->shot_check = shot_check;
	this->m_x = this->st_x = m_x;
	this->m_z = this->st_z = m_z;
	this->m_y = 0.55f;
	this->t_x = p_x;
	this->t_y = p_y;
	this->t_z = p_z;
	this->mCreate_Position = glm::vec3(this->m_x, 0.55f, this->m_z);
	this->mCharacter_Type = (int)CHARACTERTYPE::MONSTER;
	this->mBarrage_Type = (int)BARRAGETYPE::SHOT;
}

void BARRAGE::Set_Monster_Pattern_Shot(bool shot_check, GLfloat m_x, GLfloat m_y, GLfloat m_z, GLfloat p_x, GLfloat p_y, GLfloat p_z) {
	this->shot_check = shot_check;
	this->m_x = this->st_x = m_x;
	this->m_z = this->st_z = m_z;
	this->m_y = 0.55f;
	this->t_x = p_x;
	this->t_y = p_y;
	this->t_z = p_z;
	this->mCreate_Position = glm::vec3(this->m_x, 0.55f, this->m_z);
	this->mCharacter_Type = (int)CHARACTERTYPE::MONSTER;
	this->mBarrage_Type = (int)BARRAGETYPE::PATTERN;
}

void BARRAGE::Set_Monster_type(int monstertype) {
	this->mMonster_Type = monstertype;
}

void BARRAGE::Atk_Return_Set_Barrage() {
	this->m_x = -100;
	this->m_z = -100;
	this->shot_check = false;
}

bool BARRAGE::Return_Set_Barrage() {
	if ((this->m_x >= 10.0f || this->m_x <= -10.0f ||
		this->m_z >= 10.0f || this->m_z <= -10.0f) &&
		this->shot_check) {
		this->m_x = -100;
		this->m_z = -100;
		this->shot_check = false;
		return true;
	}
	return false;
}