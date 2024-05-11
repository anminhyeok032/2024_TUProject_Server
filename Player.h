#pragma once
#include "global.h"
#include "Object.h"

class ServerPlayer : public CObject
{
protected:
	XMFLOAT3					xmf3_Position_;
	XMFLOAT3					xmf3_Right_;
	XMFLOAT3					xmf3_Up_;
	XMFLOAT3					xmf3_Look_;

	float						player_id_;

	float						speed_; // ¥‹¿ß: m/s

	XMFLOAT3					xmf3Velocity_;
	XMFLOAT3     				xmf3Gravity_;

	float           			fMaxVelocityXZ_;
	float           			fMaxVelocityY_;

	float           			fFriction_;

	// RotationComponent
	short						pitch_ = 0.f;
	short						yaw_ = 0.f;
	short						roll_ = 0.f;

	

	// MovementComponent
	const XMFLOAT3 m_cxmf3GravityDirection = XMFLOAT3(0.f, -1.0f, 0.f);

	XMFLOAT3 direction_vector_ = XMFLOAT3(0.f, 0.f, 0.f);
	float acceleration_ = 2000.f;
	float friction_ = 3000.f;
	bool is_friction_ = true;

	XMFLOAT3 velocity_vector_ = XMFLOAT3(0.f, 0.f, 0.f);
	float max_speed_ = 1000.f;

	float gravity_acceleration_ = 980.f;
	float gravity_velocity_ = 0.f;

public:
	void InputActionMove(const DWORD& dwDirection, short yaw);
	XMFLOAT3 Update(const float& elapsed_time, XMFLOAT3& owner);
	void SetPlayerId(float id) { player_id_ = id; }
	void Rotate(const float& pitch, const float& yaw, const float& roll);
	void UpdateRotate(const float& elapsed_time);

	void OrientRotationToMove(float elapsed_time);

	short GetYaw() { return yaw_; }
	short GetPitch() { return pitch_; }
	short GetRoll() { return roll_; }

	XMFLOAT3 GetLookVector() const { return(Vector3::Normalize(XMFLOAT3(yaw_, pitch_, roll_))); }

	XMFLOAT3 GetDirectionVector() const { return(velocity_vector_); }


};