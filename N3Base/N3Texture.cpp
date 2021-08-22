// N3Texture.cpp: implementation of the CN3Texture class.
//
//////////////////////////////////////////////////////////////////////
#include "pch.h"
#include "N3Texture.h"
#include <iostream>
#include <filesystem>

#include "N3Base.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif
#include <fstream>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CN3Texture::CN3Texture()
{
	memset(&m_Header, 0, sizeof(m_Header));
	m_lpTexture = NULL;
}

CN3Texture::~CN3Texture()
{
	if(m_lpTexture)
	{
		int nRefCount = m_lpTexture->Release();
		if(nRefCount == 0) m_lpTexture = NULL;
	}
}

void CN3Texture::Release()
{
	if (32 == m_Header.nWidth && 32 == m_Header.nHeight) CN3Base::s_ResrcInfo.nTexture_Loaded_32X32--;
	else if(64 == m_Header.nWidth && 64 == m_Header.nHeight) CN3Base::s_ResrcInfo.nTexture_Loaded_64X64--;
	else if(128 == m_Header.nWidth && 128 == m_Header.nHeight) CN3Base::s_ResrcInfo.nTexture_Loaded_128X128--;
	else if(256 == m_Header.nWidth && 256 == m_Header.nHeight) CN3Base::s_ResrcInfo.nTexture_Loaded_256X256--;
	else if(512 == m_Header.nWidth && 512 == m_Header.nHeight) CN3Base::s_ResrcInfo.nTexture_Loaded_512X512--;
	else if(512 < m_Header.nWidth && 512 < m_Header.nHeight) CN3Base::s_ResrcInfo.nTexture_Loaded_Huge--;
	else CN3Base::s_ResrcInfo.nTexture_Loaded_OtherSize--;

	memset(&m_Header, 0, sizeof(m_Header));
	if(m_lpTexture && m_lpTexture->Release() == 0) m_lpTexture = NULL;

	//CN3BaseFileAccess::Release();
}

bool CN3Texture::Create(int nWidth, int nHeight, D3DFORMAT Format, BOOL bGenerateMipMap)
{
	if(nWidth != nHeight)

	if(nWidth <= 1 || nHeight <= 1 || D3DFMT_UNKNOWN == Format) return false;
	if(m_lpTexture != NULL) this->Release();

	if(CN3Base::s_dwTextureCaps & TEX_CAPS_POW2) // 2 의 승수만 된다면..
	{
		int nW, nH;
		for(nW = 1; nW <= nWidth; nW *= 2); nW /= 2;
		for(nH = 1; nH <= nHeight; nH *= 2); nH /= 2;

		nWidth = nW;
		nHeight = nH;
	}

	if((CN3Base::s_dwTextureCaps & TEX_CAPS_SQUAREONLY) && nWidth != nHeight) // 정사각형 텍스처만 되면..
	{
		if(nWidth > nHeight) nWidth = nHeight;
		else nHeight = nWidth;
	}

	// 비디오 카드가 256 이상의 텍스처를 지원 하지 못하면..
	if(nWidth > 256 && CN3Base::s_DevCaps.MaxTextureWidth <= 256) nWidth = CN3Base::s_DevCaps.MaxTextureWidth;
	if(nHeight > 256 && CN3Base::s_DevCaps.MaxTextureHeight <= 256) nHeight = CN3Base::s_DevCaps.MaxTextureHeight;

	// 헤더 세팅..
	memset(&m_Header, 0, sizeof(m_Header));

	// MipMap 단계 결정..
	// 4 X 4 픽셀까지만 MipMap 을 만든다..
	int nMMC = 1;
	if(bGenerateMipMap)
	{
		nMMC = 0;
		for(int nW = nWidth, nH = nHeight; nW >=4 && nH >= 4; nW /=2, nH /= 2) nMMC++;
	}

	HRESULT rval = CN3Base::s_lpD3DDev->CreateTexture(nWidth, nHeight, nMMC, 0, Format, D3DPOOL_MANAGED, &m_lpTexture, NULL);

#ifdef _N3GAME
	if(rval == D3DERR_INVALIDCALL)
	{
		CLogWriter::Write("N3Texture: createtexture err D3DERR_INVALIDCALL(%s)", m_szFileName.c_str());
		return false;
	}
	if(rval == D3DERR_OUTOFVIDEOMEMORY)
	{
		CLogWriter::Write("N3Texture: createtexture err D3DERR_OUTOFVIDEOMEMORY(%s)", m_szFileName.c_str());
		return false;
	}
	if(rval == E_OUTOFMEMORY)
	{
		CLogWriter::Write("N3Texture: createtexture err E_OUTOFMEMORY(%s)", m_szFileName.c_str());
		return false;
	}
#endif
	if(NULL == m_lpTexture)
	{
		__ASSERT(m_lpTexture, "Texture pointer is NULL!");
		return false;
	}


	m_Header.nWidth = nWidth;
	m_Header.nHeight = nHeight;
	m_Header.Format = Format;
	m_Header.bMipMap = bGenerateMipMap;

	if(32 == m_Header.nWidth && 32 == m_Header.nHeight) CN3Base::s_ResrcInfo.nTexture_Loaded_32X32++;
	else if(64 == m_Header.nWidth && 64 == m_Header.nHeight) CN3Base::s_ResrcInfo.nTexture_Loaded_64X64++;
	else if(128 == m_Header.nWidth && 128 == m_Header.nHeight) CN3Base::s_ResrcInfo.nTexture_Loaded_128X128++;
	else if(256 == m_Header.nWidth && 256 == m_Header.nHeight) CN3Base::s_ResrcInfo.nTexture_Loaded_256X256++;
	else if(512 == m_Header.nWidth && 512 == m_Header.nHeight) CN3Base::s_ResrcInfo.nTexture_Loaded_512X512++;
	else if(512 < m_Header.nWidth && 512 < m_Header.nHeight) CN3Base::s_ResrcInfo.nTexture_Loaded_Huge++;
	else CN3Base::s_ResrcInfo.nTexture_Loaded_OtherSize++;

	return true;
}

bool CN3Texture::LoadFromFile(const std::string& szFileName, uint32_t iVer)
{
	m_iFileFormatVersion = iVer;

	if(m_lpTexture != NULL) this->Release();

	m_szFileName = szFileName;

	int nFNL = m_szFileName.size();
	if(lstrcmpi(&m_szFileName[nFNL-3], "DXT") == 0)
	{
		HANDLE hFile = ::CreateFile(m_szFileName.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if(hFile == INVALID_HANDLE_VALUE)
		{
#ifdef _N3GAME
			// NOTE: temp correction for fukd up uifs
			//       - 63 is used to jump past the old texture locations
			//       - also note that the #ifdef is messed up here now...
			if (nFNL > 63) {
				OutputDebugString("WR: this is a temp fix for file -> ");
				OutputDebugString(&m_szFileName[63]);
				OutputDebugString("\n");
				hFile = ::CreateFile(&m_szFileName[63], GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			}

			if (hFile == INVALID_HANDLE_VALUE) {
				// NOTE: another temp fix
				m_szFileName = "ui/ui_ka_statebar.dxt";
				hFile = ::CreateFile(m_szFileName.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

				if (hFile == INVALID_HANDLE_VALUE) {
					CLogWriter::Write("invalid file handle(%d) - Can't open texture file(%s)", (int)hFile, m_szFileName.c_str());
					return false;
				}
			}
#endif
		}
		this->Load(hFile);
		CloseHandle(hFile);
	}
	else
	{
		D3DXIMAGE_INFO ImgInfo;
		HRESULT rval = D3DXCreateTextureFromFileEx(CN3Base::s_lpD3DDev,
			m_szFileName.c_str(),
													D3DX_DEFAULT, 
													D3DX_DEFAULT, 
													D3DX_DEFAULT, 
													0,
													D3DFMT_UNKNOWN,
													D3DPOOL_MANAGED,
													D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR,
													D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR,
													0,
													&ImgInfo,
													NULL,
													&m_lpTexture );
		if(rval == D3D_OK)
		{
			D3DSURFACE_DESC sd;
			m_lpTexture->GetLevelDesc(0, &sd);

			m_Header.nWidth = sd.Width;
			m_Header.nHeight = sd.Height;
			m_Header.Format = sd.Format;
		}
		else
		{
#ifdef _N3GAME
			// NOTE: strange error with "ka_hotkey.uif" containing image references to other UIFs...
			//       but loading them like textures...
			CLogWriter::Write("N3Texture - Failed to load texture(%s)", m_szFileName.c_str());
#endif
		}

		if(32 == m_Header.nWidth && 32 == m_Header.nHeight) CN3Base::s_ResrcInfo.nTexture_Loaded_32X32++;
		else if(64 == m_Header.nWidth && 64 == m_Header.nHeight) CN3Base::s_ResrcInfo.nTexture_Loaded_64X64++;
		else if(128 == m_Header.nWidth && 128 == m_Header.nHeight) CN3Base::s_ResrcInfo.nTexture_Loaded_128X128++;
		else if(256 == m_Header.nWidth && 256 == m_Header.nHeight) CN3Base::s_ResrcInfo.nTexture_Loaded_256X256++;
		else if(512 == m_Header.nWidth && 512 == m_Header.nHeight) CN3Base::s_ResrcInfo.nTexture_Loaded_512X512++;
		else if(512 < m_Header.nWidth && 512 < m_Header.nHeight) CN3Base::s_ResrcInfo.nTexture_Loaded_Huge++;
		else CN3Base::s_ResrcInfo.nTexture_Loaded_OtherSize++;
	}

	if(NULL == m_lpTexture)
	{
		this->Release();
		return false;
	}
	return true;
}

bool CN3Texture::Load(HANDLE hFile)
{
	DWORD dwRWC = 0;
	int nL = 0;
	ReadFile(hFile, &nL, 4, &dwRWC, NULL);
	if (nL > 0)
	{
		std::vector<char> buffer(nL + 1, NULL);
		ReadFile(hFile, &buffer[0], nL, &dwRWC, NULL);
	}

	__DXT_HEADER HeaderOrg; // 헤더를 저장해 놓고..
	ReadFile(hFile, &HeaderOrg, sizeof(HeaderOrg), &dwRWC, NULL); // 헤더를 읽는다..
	if(	'N' != HeaderOrg.szID[0] || 'T' != HeaderOrg.szID[1] || 'F' != HeaderOrg.szID[2] || 3 != HeaderOrg.szID[3] ) // "NTF"3 - Noah Texture File Ver. 3.0
	{
#ifdef _N3GAME
		CLogWriter::Write("N3Texture Warning - Old format DXT file (%s)", m_szFileName.c_str());
#endif
	}

	// DXT Format 을 읽어야 하는데 지원이 되는지 안되는지 보고 지원안되면 대체 포맷을 정한다.
	bool bDXTSupport = FALSE;
	D3DFORMAT fmtNew = HeaderOrg.Format;
	if(D3DFMT_DXT1 == HeaderOrg.Format) 
	{
		if(CN3Base::s_dwTextureCaps & TEX_CAPS_DXT1) bDXTSupport = true;
		else fmtNew = D3DFMT_A1R5G5B5;
	}
	else if(D3DFMT_DXT2 == HeaderOrg.Format)
	{
		if(CN3Base::s_dwTextureCaps & TEX_CAPS_DXT2) bDXTSupport = true;
		else fmtNew = D3DFMT_A4R4G4B4;
	}
	else if(D3DFMT_DXT3 == HeaderOrg.Format)
	{
		if(CN3Base::s_dwTextureCaps & TEX_CAPS_DXT3) bDXTSupport = true;
		else fmtNew = D3DFMT_A4R4G4B4;
	}
	else if(D3DFMT_DXT4 == HeaderOrg.Format)
	{
		if(CN3Base::s_dwTextureCaps & TEX_CAPS_DXT4) bDXTSupport = true;
		else fmtNew = D3DFMT_A4R4G4B4;
	}
	else if(D3DFMT_DXT5 == HeaderOrg.Format)
	{
		if(CN3Base::s_dwTextureCaps & TEX_CAPS_DXT5) bDXTSupport = true;
		else fmtNew = D3DFMT_A4R4G4B4;
	}

	int iWCreate = HeaderOrg.nWidth, iHCreate = HeaderOrg.nHeight;
	if(fmtNew != HeaderOrg.Format) { iWCreate /= 2; iHCreate /= 2; }// DXT 지원이 안되면 너비 높이를 줄인다.
	this->Create(iWCreate, iHCreate, fmtNew, HeaderOrg.bMipMap); // 서피스 만들고..

	if(m_lpTexture == NULL)
	{
#ifdef _N3GAME
		CLogWriter::Write("N3Texture error - Can't create texture (%s)", m_szFileName.c_str());
#endif
		return false;
	}

	D3DSURFACE_DESC sd;
	D3DLOCKED_RECT LR;
	int iMMC = m_lpTexture->GetLevelCount();

	if(	D3DFMT_DXT1 == HeaderOrg.Format || 
		D3DFMT_DXT2 == HeaderOrg.Format || 
		D3DFMT_DXT3 == HeaderOrg.Format || 
		D3DFMT_DXT4 == HeaderOrg.Format || 
		D3DFMT_DXT5 == HeaderOrg.Format )
	{
		if(TRUE == bDXTSupport)
		{
			if(iMMC > 1)
			{
				for(int i = 0; i < iMMC; i++)
				{
					m_lpTexture->GetLevelDesc(i, &sd);
					m_lpTexture->LockRect(i, &LR, NULL, NULL);

					int iTexSize = 0;
					switch(HeaderOrg.Format) {
						case D3DFMT_DXT1: {
							iTexSize = (sd.Width*sd.Height/2);
						} break;
						case D3DFMT_DXT2: {
							printf("ERROR: D3DFMT_DXT2\n");
							system("pause");
							exit(-1);
						} break;
						case D3DFMT_DXT3: {
							iTexSize = (sd.Width*sd.Height);
						} break;
						case D3DFMT_DXT4: {
							printf("ERROR: D3DFMT_DXT4\n");
							system("pause");
							exit(-1);
						} break;
						case D3DFMT_DXT5: {
							iTexSize = (sd.Width*sd.Height*2);
							printf("ERROR: D3DFMT_DXT5\n");
							system("pause");
							exit(-1);
						} break;
					}

					ReadFile(hFile, (uint8_t*)LR.pBits, iTexSize, &dwRWC, NULL); // 일렬로 된 데이터를 쓰고..
					m_lpTexture->UnlockRect(i);
				}

				// 텍스처 압축안되는 비디오 카드를 위한 여분의 데이터 건너뛰기.. 
				size_t iWTmp = HeaderOrg.nWidth / 2, iHTmp = HeaderOrg.nHeight / 2;
				for(; iWTmp >= 4 && iHTmp >= 4; iWTmp /= 2, iHTmp /= 2) // 한픽셀에 두바이트가 들어가는 A1R5G5B5 혹은 A4R4G4B4 포맷으로 되어 있다..
					::SetFilePointer(hFile, iWTmp * iHTmp * 2, 0, FILE_CURRENT); // 건너뛰고.
			}
			else // pair of if(iMMC > 1)
			{
				m_lpTexture->GetLevelDesc(0, &sd);
				m_lpTexture->LockRect(0, &LR, NULL, NULL);

				int iTexSize = 0;
				switch(HeaderOrg.Format) {
					case D3DFMT_DXT1: {
						iTexSize = (sd.Width*sd.Height/2);
					} break;
					case D3DFMT_DXT2: {
						printf("ERROR: D3DFMT_DXT2\n");
						system("pause");
						exit(-1);
					} break;
					case D3DFMT_DXT3: {
						iTexSize = (sd.Width*sd.Height);
					} break;
					case D3DFMT_DXT4: {
						printf("ERROR: D3DFMT_DXT4\n");
						system("pause");
						exit(-1);
					} break;
					case D3DFMT_DXT5: {
						iTexSize = (sd.Width*sd.Height*2);
						printf("ERROR: D3DFMT_DXT5\n");
						system("pause");
						exit(-1);
					} break;
				}

				ReadFile(hFile, (uint8_t*)LR.pBits, iTexSize, &dwRWC, NULL); // 일렬로 된 데이터를 쓰고..
				m_lpTexture->UnlockRect(0);

				// 텍스처 압축안되는 비디오 카드를 위한 여분의 데이터 건너뛰기.. 
				::SetFilePointer(hFile, HeaderOrg.nWidth * HeaderOrg.nHeight / 4, 0, FILE_CURRENT); // 건너뛰고.
				if(HeaderOrg.nWidth >= 1024) SetFilePointer(hFile, 256 * 256 * 2, 0, FILE_CURRENT); // 사이즈가 512 보다 클경우 부두용 데이터 건너뛰기..
			}
		}
		else // DXT 지원이 안되면..
		{
			if(iMMC > 1) // LOD 만큼 건너뛰기...
			{
				// 압축 데이터 건너뛰기..
				size_t iWTmp = HeaderOrg.nWidth, iHTmp = HeaderOrg.nHeight, iSkipSize = 0;
				for(; iWTmp >= 4 && iHTmp >= 4; iWTmp /= 2, iHTmp /= 2)
				{
					if(D3DFMT_DXT1 == HeaderOrg.Format) iSkipSize += iWTmp * iHTmp / 2; // DXT1 형식은 16비트 포맷에 비해 1/4 로 압축..
					else iSkipSize += iWTmp * iHTmp; // DXT2 ~ DXT5 형식은 16비트 포맷에 비해 1/2 로 압축..
				}
				::SetFilePointer(hFile, iSkipSize, 0, FILE_CURRENT); // 건너뛰고.

				// LOD 만큼 건너뛰기..
				iWTmp = HeaderOrg.nWidth / 2; iHTmp = HeaderOrg.nHeight / 2; iSkipSize = 0;

				// 비디오 카드 지원 텍스처 크기가 작을경우 건너뛰기..
				for(; iWTmp > CN3Base::s_DevCaps.MaxTextureWidth || iHTmp > CN3Base::s_DevCaps.MaxTextureHeight; iWTmp /= 2, iHTmp /= 2)
					iSkipSize += iWTmp * iHTmp * 2;
				if(iSkipSize) ::SetFilePointer(hFile, iSkipSize, 0, FILE_CURRENT); // 건너뛰고.

				for(int i = 0; i < iMMC; i++)
				{
					m_lpTexture->GetLevelDesc(i, &sd);
					m_lpTexture->LockRect(i, &LR, NULL, NULL);
					int nH = sd.Height;
					for(int y = 0; y < nH; y++)
						ReadFile(hFile, (uint8_t*)LR.pBits + y * LR.Pitch, 2 * sd.Width, &dwRWC, NULL);
					m_lpTexture->UnlockRect(i);
				}
			}
			else // pair of if(iMMC > 1)
			{
				// 압축 데이터 건너뛰기..
				int iWTmp = HeaderOrg.nWidth, iHTmp = HeaderOrg.nHeight, iSkipSize = 0;
				if(D3DFMT_DXT1 == HeaderOrg.Format) iSkipSize = iWTmp * iHTmp / 2; // DXT1 형식은 16비트 포맷에 비해 1/4 로 압축..
				else iSkipSize = iWTmp * iHTmp; // DXT2 ~ DXT5 형식은 16비트 포맷에 비해 1/2 로 압축..

			}
		}
	}
	else
	{
		int iPixelSize = 0;
		if(	fmtNew == D3DFMT_A1R5G5B5 || fmtNew == D3DFMT_A4R4G4B4) iPixelSize = 2;
		else if(fmtNew == D3DFMT_R8G8B8) iPixelSize = 3;
		else if(fmtNew == D3DFMT_A8R8G8B8 || fmtNew == D3DFMT_X8R8G8B8) iPixelSize = 4;
		else 
		{
			__ASSERT(0, "Not supported texture format");
		}

		if(iMMC > 1)
		{
			// 비디오 카드 지원 텍스처 크기가 작을경우 건너뛰기..
			size_t iWTmp = HeaderOrg.nWidth, iHTmp = HeaderOrg.nHeight, iSkipSize = 0;
			for(; iWTmp > CN3Base::s_DevCaps.MaxTextureWidth || iHTmp > CN3Base::s_DevCaps.MaxTextureHeight; iWTmp /= 2, iHTmp /= 2)
				iSkipSize += iWTmp * iHTmp * iPixelSize;
			if(iSkipSize) ::SetFilePointer(hFile, iSkipSize, 0, FILE_CURRENT); // 건너뛰고.

			// 데이터 읽기..
			for(int i = 0; i < iMMC; i++)
			{
				m_lpTexture->GetLevelDesc(i, &sd);
				m_lpTexture->LockRect(i, &LR, NULL, NULL);
				for(int y = 0; y < (int)sd.Height; y++)
					ReadFile(hFile, (uint8_t*)LR.pBits + y * LR.Pitch, iPixelSize * sd.Width, &dwRWC, NULL);
				m_lpTexture->UnlockRect(i);
			}
		}
		else // pair of if(iMMC > 1)
		{
			// 비디오 카드 지원 텍스처 크기가 작을경우 건너뛰기..
			if(HeaderOrg.nWidth >= 512 && m_Header.nWidth <= 256)
				::SetFilePointer(hFile, HeaderOrg.nWidth * HeaderOrg.nHeight * iPixelSize, 0, FILE_CURRENT); // 건너뛰고.

			m_lpTexture->GetLevelDesc(0, &sd);
			m_lpTexture->LockRect(0, &LR, NULL, NULL);
			for(int y = 0; y < (int)sd.Height; y++)
				ReadFile(hFile, (uint8_t*)LR.pBits + y * LR.Pitch, iPixelSize * sd.Width, &dwRWC, NULL);
			m_lpTexture->UnlockRect(0);

			if(m_Header.nWidth >= 512 && m_Header.nHeight >= 512)
				SetFilePointer(hFile, 256 * 256 * 2, 0, FILE_CURRENT); // 사이즈가 512 보다 클경우 부두용 데이터 건너뛰기..
		}
	}
//	this->GenerateMipMap(); // Mip Map 을 만든다..
	return true;
}

bool CN3Texture::SkipFileHandle(HANDLE hFile)
{
	__DXT_HEADER HeaderOrg; // 헤더를 저장해 놓고..
	DWORD dwRWC = 0;
	ReadFile(hFile, &HeaderOrg, sizeof(HeaderOrg), &dwRWC, NULL); // 헤더를 읽는다..
	if(	'N' != HeaderOrg.szID[0] || 'T' != HeaderOrg.szID[1] || 'F' != HeaderOrg.szID[2] || 3 != HeaderOrg.szID[3] ) // "NTF"3 - Noah Texture File Ver. 3.0
	{
#ifdef _N3GAME
		CLogWriter::Write("N3Texture Warning - Old format DXT file (%s)", m_szFileName.c_str());
#endif
	}

	// 압축 포맷이면..
	if(	D3DFMT_DXT1 == HeaderOrg.Format || 
		D3DFMT_DXT2 == HeaderOrg.Format || 
		D3DFMT_DXT3 == HeaderOrg.Format || 
		D3DFMT_DXT4 == HeaderOrg.Format || 
		D3DFMT_DXT5 == HeaderOrg.Format )
	{
		int iWTmp = HeaderOrg.nWidth, iHTmp = HeaderOrg.nHeight, iSkipSize = 0;
		if(HeaderOrg.bMipMap)
		{
			// 압축 데이터 건너뛰기..
			for(; iWTmp >= 4 && iHTmp >= 4; iWTmp/=2, iHTmp/=2)
			{
				if(D3DFMT_DXT1 == HeaderOrg.Format) iSkipSize += iWTmp * iHTmp / 2;
				else iSkipSize += iWTmp * iHTmp;
			}
			// 텍스처 압축안되는 비디오 카드를 위한 여분의 데이터 건너뛰기.. 
			iWTmp = HeaderOrg.nWidth / 2; iHTmp = HeaderOrg.nHeight / 2;
			for(; iWTmp >= 4 && iHTmp >= 4; iWTmp /= 2, iHTmp /= 2) // 한픽셀에 두바이트가 들어가는 A1R5G5B5 혹은 A4R4G4B4 포맷으로 되어 있다..
				iSkipSize += iWTmp * iHTmp * 2; // 건너뛰고.
		}
		else // pair of if(HeaderOrg.bMipMap)
		{
			// 압축 데이터 건너뛰기..
			if(D3DFMT_DXT1 == HeaderOrg.Format) iSkipSize += HeaderOrg.nWidth * HeaderOrg.nHeight / 2;
			else iSkipSize += iSkipSize += HeaderOrg.nWidth * HeaderOrg.nHeight;

			// 텍스처 압축안되는 비디오 카드를 위한 여분의 데이터 건너뛰기.. 
			iSkipSize += HeaderOrg.nWidth * HeaderOrg.nHeight * 2;
			if(HeaderOrg.nWidth >= 1024) iSkipSize += 256 * 256 * 2; // 사이즈가 1024 보다 클경우 부두용 데이터 건너뛰기..
		}

		::SetFilePointer(hFile, iSkipSize, 0, FILE_CURRENT); // 건너뛰고.
	}
	else
	{
		int iPixelSize = 0;
		if(	HeaderOrg.Format == D3DFMT_A1R5G5B5 || HeaderOrg.Format == D3DFMT_A4R4G4B4) iPixelSize = 2;
		else if(HeaderOrg.Format == D3DFMT_R8G8B8) iPixelSize = 3;
		else if(HeaderOrg.Format == D3DFMT_A8R8G8B8 || HeaderOrg.Format == D3DFMT_X8R8G8B8) iPixelSize = 4;
		else 
		{
			__ASSERT(0, "Not supported texture format");
		}

		int iWTmp = HeaderOrg.nWidth, iHTmp = HeaderOrg.nHeight, iSkipSize = 0;
		if(HeaderOrg.bMipMap)
		{
			for(; iWTmp >= 4 && iHTmp >= 4; iWTmp/=2, iHTmp/=2)
				iSkipSize += iWTmp * iHTmp * iPixelSize;
		}
		else
		{
			iSkipSize += iWTmp * iHTmp * iPixelSize;
			if(HeaderOrg.nWidth >= 512) iSkipSize += 256 * 256 * 2; // 사이즈가 512 보다 클경우 부두용 데이터 건너뛰기..
		}
		
		::SetFilePointer(hFile, iSkipSize, 0, FILE_CURRENT); // 건너뛰고.
	}
	return true;
}

void CN3Texture::UpdateRenderInfo()
{
}
