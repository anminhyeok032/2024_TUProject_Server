#pragma once
#include "global.h"

// Objects data



class CObject {

protected:
	DirectX::XMFLOAT4X4 m_xmf4x4_World;

	// 물리 옵션 적용관련 변수
	bool is_fall_ = false; //중력의 적용을 받는지

public:
	CObject();
	virtual ~CObject();

	DirectX::XMFLOAT3 GetPosition();

	//
	DirectX::BoundingOrientedBox Get_OBB(int nType);

	void SetPosition(float x, float y, float z);
	void SetPosition(DirectX::XMFLOAT3 xmf3_Position);

	//
	virtual DirectX::XMFLOAT4X4 GetWorldMatrix() { return m_xmf4x4_World; };

	bool is_fall() const { return is_fall_; }
};