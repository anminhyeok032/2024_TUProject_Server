#pragma once
#include "global.h"

// �̱��� ���� ���
class CHeightMap
{
private:
	BYTE* p_heightmap_pixels_;
	int	width_, length_;
	XMFLOAT3 xmf3_scale_;	// �ȼ��� ũ��

private:
	CHeightMap() { }

public:
	~CHeightMap();
	void initialize(LPCTSTR pFileName, int nWidth, int nLength, XMFLOAT3 xmf3Scale);

	float GetHeight(float fx, float fz);

	static CHeightMap& GetInstance()
	{
		static CHeightMap instance;
		return instance;
	}	
};

