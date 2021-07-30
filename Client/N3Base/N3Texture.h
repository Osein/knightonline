#pragma once

class CN3Texture
{
public:
	typedef struct __DXT_HEADER
	{
		char szID[4]; // "NTF"숫자 - Noah Texture File Ver. ?.0
		int nWidth;
		int nHeight;
		D3DFORMAT Format; // 0 - 압축 안함 1 ~ 5 : D3DFMT_DXT1 ~ D3DFMT_DXT5
		BOOL bMipMap; // Mip Map ??
	} __DxtHeader;

	int m_iLOD;
	uint32_t m_iFileFormatVersion;

protected:
	__DXT_HEADER m_Header;
	LPDIRECT3DTEXTURE9 m_lpTexture;
	std::string		m_szFileName;

public:
	void				UpdateRenderInfo();
	bool				LoadFromFile(const std::string& szFileName, uint32_t iVer = N3FORMAT_VER_DEFAULT);
	bool				Load(HANDLE hFile);
	bool				SkipFileHandle(HANDLE hFile);

	std::string FileName() { return m_szFileName; }

	uint32_t				Width() { return m_Header.nWidth; }
	uint32_t				Height() { return m_Header.nHeight; }
	D3DFORMAT			PixelFormat() { return m_Header.Format; }
	int					MipMapCount() { if(NULL == m_lpTexture) return 0; else return m_lpTexture->GetLevelCount(); }
	
	bool				Create(int nWidth, int nHeight, D3DFORMAT Format, BOOL bGenerateMipMap); // 장치에 맞게 생성
	LPDIRECT3DTEXTURE9	Get() { return m_lpTexture; }
	operator LPDIRECT3DTEXTURE9 () { return m_lpTexture; }
	
	void Release();
	CN3Texture();
	virtual ~CN3Texture();
};
