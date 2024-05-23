#include "CHeightMap.h"

CHeightMap::~CHeightMap()
{
	if (p_heightmap_pixels_) delete[] p_heightmap_pixels_;
}

float CHeightMap::GetHeight(float fx, float fz)
{
	fx = fx / xmf3_scale_.x;
	fz = fz / xmf3_scale_.z;

	if ((fx < 0.0f) || (fz < 0.0f) || (fx >= width_) || (fz >= length_)) return(0.0f);

	int x = (int)fx;
	int z = (int)fz;
	float fxPercent = fx - x;
	float fzPercent = fz - z;

	float fBottomLeft = (float)p_heightmap_pixels_[x + (z * width_)];
	float fBottomRight = (float)p_heightmap_pixels_[(x + 1) + (z * width_)];
	float fTopLeft = (float)p_heightmap_pixels_[x + ((z + 1) * width_)];
	float fTopRight = (float)p_heightmap_pixels_[(x + 1) + ((z + 1) * width_)];


	if (fzPercent < (1.0f - fxPercent))
	{
		fTopRight = fTopLeft + (fBottomRight - fBottomLeft);
	}
	else
	{
		fBottomLeft = fTopLeft + (fBottomRight - fTopRight);
	}
	
	float fTopHeight = fTopLeft * (1 - fxPercent) + fTopRight * fxPercent;
	float fBottomHeight = fBottomLeft * (1 - fxPercent) + fBottomRight * fxPercent;
	float fHeight = fBottomHeight * (1 - fzPercent) + fTopHeight * fzPercent;

	return (fHeight * xmf3_scale_.y);
}

void CHeightMap::initialize(LPCTSTR pFileName, int nWidth, int nLength, XMFLOAT3 xmf3Scale)
{
	width_ = nWidth;
	length_ = nLength;
	xmf3_scale_ = xmf3Scale;

	BYTE* heightmap_pixels = new BYTE[width_ * length_];

	HANDLE file = ::CreateFile(pFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_READONLY, NULL);

	DWORD byte_read;
	::ReadFile(file, heightmap_pixels, width_ * length_, &byte_read, NULL);
	::CloseHandle(file);

	p_heightmap_pixels_ = new BYTE[width_ * length_];

	for (int z = 0; z < length_; z++)
	{
		for (int x = 0; x < width_; x++)
		{
			p_heightmap_pixels_[x + ((length_ - 1 - z) * width_)] = heightmap_pixels[x + (z * width_)];
		}
	}
}
