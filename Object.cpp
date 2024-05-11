#include "Object.h"

CObject::CObject()
{
	DirectX::XMStoreFloat4x4(&m_xmf4x4_World, DirectX::XMMatrixIdentity());
}


CObject::~CObject()
{

}

DirectX::XMFLOAT3 CObject::GetPosition() 
{
	return DirectX::XMFLOAT3(m_xmf4x4_World._41, m_xmf4x4_World._42, m_xmf4x4_World._43);
}

void CObject::SetPosition(float x, float y, float z) {
	m_xmf4x4_World._41 = x;
	m_xmf4x4_World._42 = y;
	m_xmf4x4_World._43 = z;
}

void CObject::SetPosition(DirectX::XMFLOAT3 xmf3_Position) 
{
	SetPosition(xmf3_Position.x, xmf3_Position.y, xmf3_Position.z);
}