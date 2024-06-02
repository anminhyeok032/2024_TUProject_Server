#pragma once
#include "global.h"
#include "Object.h"

// Sword 1 Slash 시간
const float Sword_1_slash_move_end_time_ = 1.f / 60.f * 28.f;
// Sphere 4 Slash 시간
const float Sphere_41_slash_move_end_time_ = 1.f / 30.f * 5.f;
const float Sphere_42_slash_move_end_time_ = 1.f / 30.f * 10.f;

class ServerPlayer : public CObject
{
protected:
	XMFLOAT3					xmf3_Position_;
	XMFLOAT3					xmf3_Right_;
	XMFLOAT3					xmf3_Up_;
	XMFLOAT3					xmf3_Look_ = { 0, 0, 1 };

	int						player_id_;

	float						speed_; // 단위: m/s

	XMFLOAT3					xmf3Velocity_;
	XMFLOAT3     				xmf3Gravity_;

	float           			fMaxVelocityXZ_;
	float           			fMaxVelocityY_;

	float           			fFriction_;

	// RotationComponent
	float						pitch_ = 0.f;
	float						yaw_ = 0.f;
	float						roll_ = 0.f;

	// KeyInput
	std::unordered_map<char, bool> keyboard_input_;

	// Animation
	bool anim_playing_ = false;
	WeaponType weapon_;
	PlayerAttackType attack_;
	PlayerAnimationState player_animation_state_;

	// MovementComponent
	const XMFLOAT3 m_cxmf3GravityDirection = XMFLOAT3(0.f, -1.0f, 0.f);

	XMFLOAT3 direction_vector_ = XMFLOAT3(0.f, 0.f, 0.f);
	float acceleration_ = 8000.f;
	float friction_ = 3000.f;
	bool is_friction_ = true;

	XMFLOAT3 velocity_vector_ = XMFLOAT3(0.f, 0.f, 0.f);
	float max_speed_ = 1000.f;

	float gravity_acceleration_ = 980.f;
	float gravity_velocity_ = 0.f;

	float slash_time_1_ = 0.f;
	float slash_time_2_ = 0.f;


public:
	void InputActionMove(const uint8_t Direction, short camera_yaw);
	bool SkillMove(const float& elapsed_time);
	XMFLOAT3 UpdateSkillMove(const float& elapsed_time, const XMFLOAT3& owner);
	void ResetAnimTime();

	XMFLOAT3 Update(const float& elapsed_time, const XMFLOAT3& owner);
	void Rotate(const float& pitch, const float& yaw, const float& roll);
	void UpdateRotate(const float& elapsed_time);
	void OrientRotationToMove(float elapsed_time);

	bool MoveCheckSword(const float& elapsed_time);
	bool MoveCheckSphere(const float& elapsed_time);


	
	float GetYaw() { return yaw_; }
	float GetPitch() { return pitch_; }
	float GetRoll() { return roll_; }
	bool GetFriction() { return is_friction_; }
	bool GetAnimPlaying() { return anim_playing_; }
	WeaponType GetPlayerWeapon() { return weapon_; }
	PlayerAttackType GetPlayerAttack() { return attack_; }
	PlayerAnimationState GetPlayerAnimationState() { return player_animation_state_; }

	void SetYaw(float yaw) { yaw_ = yaw; }
	void SetPitch(float pitch) { pitch_ = pitch; }
	void SetRoll(float roll) { roll_ = roll; }

	void SetFriction(bool is_friction) { is_friction_ = is_friction; }
	void SetPlayerId(int id) { player_id_ = id; }
	void SetLook(float pitch, float yaw, float roll) { yaw_ = yaw; pitch_ = pitch; roll_ = roll; }
	void SetMaxSpeed(float speed) { max_speed_ = speed;  }

	void SetAnimationStatus(bool curr_animation) { anim_playing_ = curr_animation; }

	void SetCurrentAnimation(WeaponType weapon, PlayerAttackType attack, PlayerAnimationState player_animation_state);

	XMFLOAT3 GetLookVector() const { return(Vector3::Normalize(XMFLOAT3(pitch_, yaw_, roll_))); }
	//XMFLOAT3 GetLookVector() const { return(XMFLOAT3(pitch_, yaw_, roll_)); }

	XMFLOAT3 GetDirectionVector() const { return(direction_vector_); }
	void SetDirectionVector(XMFLOAT3 direction) { direction_vector_ = direction; }

	XMFLOAT3 GetVelocityVector() const { return(velocity_vector_); }
	void SetVelocityVector() { (velocity_vector_ = XMFLOAT3(0.f, 0.f, 0.f)); }

};