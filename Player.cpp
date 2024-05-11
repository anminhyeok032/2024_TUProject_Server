#include "Player.h"

void ServerPlayer::InputActionMove(const DWORD& dwDirection, short yaw)
{
	XMFLOAT3 direction_vector = XMFLOAT3(0.f, 0.f, 0.f);
	if (dwDirection)
	{

		// 카메라의 yaw 회전만 가져와서 사용
		XMMATRIX R = XMMatrixRotationRollPitchYaw(0.f, XMConvertToRadians(yaw), 0.f);
		XMFLOAT3 look = XMFLOAT3(0.f, 0.f, 1.f), up = XMFLOAT3(0.f, 1.f, 0.f);
		XMStoreFloat3(&look, XMVector3TransformCoord(XMLoadFloat3(&look), R));
		XMFLOAT3 right = Vector3::CrossProduct(up, look);

		if (dwDirection & DIR_FORWARD) direction_vector = Vector3::Add(direction_vector, look);
		if (dwDirection & DIR_BACKWARD) direction_vector = Vector3::Add(direction_vector, look, -1.f);
		if (dwDirection & DIR_LEFT) direction_vector = Vector3::Add(direction_vector, right, -1.f);
		if (dwDirection & DIR_RIGHT) direction_vector = Vector3::Add(direction_vector, right);
	}
	direction_vector_ = direction_vector;
	yaw_ = yaw;
}

XMFLOAT3 ServerPlayer::Update(const float& elapsed_time, XMFLOAT3& owner)
{
	if (Vector3::Length(direction_vector_))
		is_friction_ = false;
	else
		is_friction_ = true;

	direction_vector_ = Vector3::Normalize(direction_vector_);

	// v = v0 + a * t - f * t
	velocity_vector_ = velocity_vector_ + (direction_vector_ * (acceleration_ * elapsed_time));
	if (is_friction_)
	{
		if (friction_ * elapsed_time > Vector3::Length(velocity_vector_))
			velocity_vector_ = XMFLOAT3(0, 0, 0);
		else
		{
			XMFLOAT3 friction_vector = Vector3::Normalize(velocity_vector_ * -1) * (friction_ * elapsed_time);
			velocity_vector_ = velocity_vector_ + friction_vector;
		}
	}

	float speed = Vector3::Length(velocity_vector_);
	if (speed > max_speed_)
		velocity_vector_ = Vector3::Normalize(velocity_vector_) * max_speed_;
	XMFLOAT3 xmf3NewPosition = Vector3::Add(owner, velocity_vector_ * elapsed_time);
	
	return xmf3NewPosition;

	// 일단 중력 무시
	//if (owner->is_fall())
	//{
	//	gravity_velocity_ += gravity_acceleration_ * elapsed_time;

	//	if (gravity_velocity_ > 5300.f) // 종단 속도
	//		gravity_velocity_ = 5300.f;
	//	xmf3NewPosition = Vector3::Add(xmf3NewPosition, m_cxmf3GravityDirection, gravity_velocity_);
	//}
	//else
	//	gravity_velocity_ = 0.f;

	//owner_->set_position_vector(xmf3NewPosition);

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

	// 해당 값을 주고 받기 위해 각도로 계산
	/*float yaw = atan2(xmf3_Look_.z, xmf3_Look_.x);
	float pitch = atan2(xmf3_Look_.y, sqrt(xmf3_Look_.x * xmf3_Look_.x + xmf3_Look_.z * xmf3_Look_.z));*/
}

void ServerPlayer::OrientRotationToMove(float elapsed_time)
{
	XMFLOAT3 v = GetLookVector(), d = Vector3::Normalize(direction_vector_), u = XMFLOAT3(0.f, 1.f, 0.f);
	float result = Vector3::DotProduct(u, Vector3::CrossProduct(d, v));

	float yaw = Vector3::Angle(v, d);
	if (result > 0)
	{
		yaw *= -1;
	}
	if (!IsZero(yaw))
	{
		Rotate(0.f, yaw * 12.f * elapsed_time, 0.f);
	}
}