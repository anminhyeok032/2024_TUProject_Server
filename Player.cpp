#include "Player.h"

void ServerPlayer::InputActionMove(const uint8_t Direction, short camera_yaw)
{
	bool is_key_pressed = Direction & 0x01;
	uint8_t key_stroke = Direction >> 1;
	char key = static_cast<char>(key_stroke);

	//std::cout << "key input : " << key << " = " << is_key_pressed << std::endl;



	// keyboard 업데이트
	if (false == is_key_pressed)
	{
		keyboard_input_.erase(key);
		switch (key)
		{
		case ' ':
			anim_playing_ = false;
			break;
		default:
			break;
		}
	}
	else
	{
		keyboard_input_[key] = is_key_pressed;
	}
	
	XMFLOAT3 animation_vector = XMFLOAT3(0.f, 0.f, 0.f);
	XMFLOAT3 direction_vector = XMFLOAT3(0.f, 0.f, 0.f);
	// 카메라의 yaw 회전만 가져와서 사용
	XMMATRIX R = XMMatrixRotationRollPitchYaw(0.f, XMConvertToRadians(camera_yaw), 0.f);
	XMFLOAT3 look = XMFLOAT3(0.f, 0.f, 1.f), up = XMFLOAT3(0.f, 1.f, 0.f);
	XMStoreFloat3(&look, XMVector3TransformCoord(XMLoadFloat3(&look), R));
	XMFLOAT3 right = Vector3::CrossProduct(up, look);

	// Process keyboard input
	for (const auto& entry : keyboard_input_)
	{
		char key_char = entry.first;
		bool is_pressed = entry.second;

		if (is_pressed)
		{
			switch (key_char)
			{
			// Movement
			case 'W':
				direction_vector = Vector3::Add(direction_vector, look);
				break;
			case 'A':
				direction_vector = Vector3::Add(direction_vector, right, -1.f);
				break;
			case 'S':
				direction_vector = Vector3::Add(direction_vector, look, -1.f);
				break;
			case 'D':
				direction_vector = Vector3::Add(direction_vector, right);
				break;
			// 
			case ' ':
				anim_playing_ = true;
				animation_vector = XMFLOAT3(0.f, 0.f, 0.f);
				animation_vector = Vector3::Add(animation_vector, xmf3_Look_);
				break;
			default:
				break;
			}
		}

	}

	// TODO: 소드2번 공격시 움직임 가능하게 해줘야 함
	if (GetPlayerAnimationState() == 8)
	{
		anim_playing_ = false;
	}
	
	if (true == anim_playing_)
	{
		direction_vector = animation_vector;
	}
	
	if (IsZeroVector(direction_vector))
	{
		velocity_vector_ = XMFLOAT3(0.0f, 0.0f, 0.0f);
	}

	direction_vector_ = direction_vector;
}

bool ServerPlayer::SkillMove(const float& elapsed_time)
{
	XMFLOAT3 animation_vector = XMFLOAT3(0.f, 0.f, 0.f);

	switch (weapon_)
	{
	// None
	case 0:
		break;
	// Sword
	case 1:
		if (true == MoveCheckSword(elapsed_time))
		{
			animation_vector = Vector3::Add(animation_vector, xmf3_Look_);
		}
		break;
	// Sphere
	case 2:
		if (true == MoveCheckSphere(elapsed_time))
		{
			animation_vector = Vector3::Add(animation_vector, xmf3_Look_);
		}
		break;
	default:
		break;
	}

	if (IsZeroVector(animation_vector))
	{
		velocity_vector_ = XMFLOAT3(0.0f, 0.0f, 0.0f);
		return false;
	}
	direction_vector_ = animation_vector;
	return true;

}

XMFLOAT3 ServerPlayer::UpdateSkillMove(const float& elapsed_time, const XMFLOAT3& owner)
{
	XMFLOAT3 xmf3NewPosition = { 0.f, 0.f, 0.f };
	//if (Vector3::Length(direction_vector_))
	//{
	//	return owner;
	//}


	// v = v0 + a * t - f * t
	velocity_vector_ = (velocity_vector_)+(direction_vector_ * (acceleration_ * elapsed_time));

	// 최대 속도 넘을시 속도를 조정
	float speed = Vector3::Length(velocity_vector_);
	if (speed > max_speed_)
	{
		velocity_vector_ = Vector3::Normalize(velocity_vector_) * max_speed_;
	}

	xmf3NewPosition = Vector3::Add(owner, velocity_vector_ * elapsed_time * 2);

	if (xmf3NewPosition.x < 50)	xmf3NewPosition.x = 51;
	if (xmf3NewPosition.x > TERRAIN + 200)	xmf3NewPosition.x = TERRAIN + 199;
	if (xmf3NewPosition.z < 50)	xmf3NewPosition.z = 51;
	if (xmf3NewPosition.z > TERRAIN + 200)	xmf3NewPosition.z = TERRAIN + 199;

	return xmf3NewPosition;
}

void ServerPlayer::ResetAnimTime()
{
	slash_time_1_ = 0;
	slash_time_2_ = 0;
}


XMFLOAT3 ServerPlayer::Update(const float& elapsed_time, const XMFLOAT3& owner)
{
	if (Vector3::Length(direction_vector_))
	{
		is_friction_ = false;
	}
	else
	{
		is_friction_ = true;
	}
	direction_vector_ = Vector3::Normalize(direction_vector_);

	// v = v0 + a * t - f * t
	velocity_vector_ = (velocity_vector_) + (direction_vector_ * (acceleration_ * elapsed_time));
	if (is_friction_)
	{
		if (friction_ * elapsed_time > Vector3::Length(velocity_vector_))
		{
			velocity_vector_ = XMFLOAT3(0, 0, 0);
		}
		else
		{
			XMFLOAT3 friction_vector = Vector3::Normalize(velocity_vector_ * -1) * (friction_ * elapsed_time);
			velocity_vector_ = velocity_vector_ + friction_vector;
		}
	}

	// 최대 속도 넘을시 속도를 조정
	float speed = Vector3::Length(velocity_vector_);
	if (speed > max_speed_)
	{
		velocity_vector_ = Vector3::Normalize(velocity_vector_) * max_speed_;
	}

	XMFLOAT3 xmf3NewPosition = Vector3::Add(owner, velocity_vector_ * elapsed_time);
	//std::cout << "[" << 0 << "]'s position : x = " << xmf3NewPosition.x << ", y = " << xmf3NewPosition.y << ", z = " << xmf3NewPosition.z << std::endl;
	
	//// 일단 중력 무시
	//if (xmf3NewPosition.y > 100)
	//{
	//	gravity_velocity_ += gravity_acceleration_ * elapsed_time;

	//	if (gravity_velocity_ > 5300.f) // 종단 속도
	//		gravity_velocity_ = 5300.f;
	//	xmf3NewPosition = Vector3::Add(xmf3NewPosition, m_cxmf3GravityDirection, gravity_velocity_);
	//}

	//else
	//	gravity_velocity_ = 0.f;

	if (xmf3NewPosition.x < 50)	xmf3NewPosition.x = 51;
	if (xmf3NewPosition.x > TERRAIN + 200)	xmf3NewPosition.x = TERRAIN + 199;
	if (xmf3NewPosition.z < 50)	xmf3NewPosition.z = 51;
	if (xmf3NewPosition.z > TERRAIN + 200)	xmf3NewPosition.z = TERRAIN + 199;

	return xmf3NewPosition;
}

void ServerPlayer::Rotate(const float& pitch, const float& yaw, const float& roll)
{
	pitch_ += pitch;
	yaw_ += yaw;
	roll_ += roll;
}

void ServerPlayer::UpdateRotate(const float& elapsed_time)
{
	XMMATRIX P = XMMatrixIdentity(), Y = XMMatrixIdentity(), R = XMMatrixIdentity();
	XMFLOAT3 x_axis = XMFLOAT3(1.f, 0.f, 0.f);
	XMFLOAT3 y_axis = XMFLOAT3(0.f, 1.f, 0.f);
	XMFLOAT3 z_axis = XMFLOAT3(0.f, 0.f, 1.f);

	P = XMMatrixRotationAxis(XMLoadFloat3(&x_axis), XMConvertToRadians(pitch_));
	Y = XMMatrixRotationAxis(XMLoadFloat3(&y_axis), XMConvertToRadians(yaw_));
	R = XMMatrixRotationAxis(XMLoadFloat3(&z_axis), XMConvertToRadians(roll_));
	
	XMMATRIX rotation_matrix = XMMatrixMultiply(XMMatrixMultiply(P, Y), R);
	xmf3_Look_ = Vector3::TransformNormal(z_axis, rotation_matrix);

	pitch_ = xmf3_Look_.x;
	yaw_ = xmf3_Look_.y;
	roll_ = xmf3_Look_.z;
}

void ServerPlayer::OrientRotationToMove(float elapsed_time)
{
	XMFLOAT3 v = GetLookVector(), d = Vector3::Normalize(direction_vector_), u = XMFLOAT3(0.f, 1.f, 0.f);
	if (Vector3::Length(direction_vector_) == 0.0f) return;

	float result = Vector3::DotProduct(u, Vector3::CrossProduct(d, v));

	float yaw = Vector3::Angle(v, d);
	if (result > 0)
	{
		yaw *= -1;
	}
	if (!IsZero(yaw))
	{
		yaw_ = yaw * 12.f * elapsed_time;
	}
	
}

bool ServerPlayer::MoveCheckSword(const float& elapsed_time )
{
	bool do_move = false;
	switch (attack_)
	{
		// LeftAttack
	case 1:
		if (player_animation_state_ == 7)	// A13
		{
			if (slash_time_2_ > Sword_1_slash_move_end_time_)
			{
				return false;
			}
			else
			{
				slash_time_2_ += elapsed_time;
				max_speed_ = 1500.f;
				do_move = true;
			}
		}
		break;
		// RightAttack
	case 2:
		max_speed_ = 700.f;
		do_move = false;
		anim_playing_ = false;	// 회전공격동안 이동 가능
		break;
		// BothAttack
	case 3:
		// 2개인 이유 물어보기 1000 / 1500
		max_speed_ = 800.f;
		do_move = true;
		break;
		// ControlAttack
	case 4:
		// 2개인 이유 물어보기 240.f / 120
		max_speed_ = 500.f;
		do_move = true;
		break;
	default:
		break;
	}

	return do_move;
}

bool ServerPlayer::MoveCheckSphere(const float& elapsed_time )
{
	bool do_move = false;
	switch (attack_)
	{
	// LeftAttack
	case 1:
		max_speed_ = 1000.f;
		do_move = true;
		break;
	// RightAttack
	case 2:
		max_speed_ = 700.f;
		do_move = true;
		break;
	// BothAttack
	case 3:
		// 2개인 이유 물어보기 1000 / 1500
		max_speed_ = 800.f;
		do_move = true;
		break;
	// ControlAttack
	case 4:
		if (player_animation_state_ == 20)	// A13
		{
			if (slash_time_1_ < Sphere_41_slash_move_end_time_)
			{
				slash_time_1_ += elapsed_time;
				max_speed_ = 500.f;
				do_move = true;
				break;
			}
			if (slash_time_2_ < Sphere_42_slash_move_end_time_)
			{
				slash_time_2_ += elapsed_time;
				max_speed_ = 3000.f;
				do_move = true;
				break;
			}
		}
		break;
	default:
		break;
	}

	return do_move;
}

void ServerPlayer::SetCurrentAnimation(WeaponType weapon, PlayerAttackType attack, PlayerAnimationState player_animation_state)
{
	weapon_ = weapon;
	attack_ = attack;
	player_animation_state_ =  player_animation_state;
}
