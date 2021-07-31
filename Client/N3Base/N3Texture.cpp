// N3Texture.cpp: implementation of the CN3Texture class.
//
//////////////////////////////////////////////////////////////////////
#include "pch.h"
#include "N3Texture.h"

#ifdef _N3TOOL
#include "BitmapFile.h"
#endif // #ifdef _N3TOOL
#include "N3Base.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif

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

	if(CN3Base::s_dwTextureCaps & TEX_CAPS_POW2) // 2 �� �¼��� �ȴٸ�..
	{
		int nW, nH;
		for(nW = 1; nW <= nWidth; nW *= 2); nW /= 2;
		for(nH = 1; nH <= nHeight; nH *= 2); nH /= 2;

		nWidth = nW;
		nHeight = nH;
	}

	if((CN3Base::s_dwTextureCaps & TEX_CAPS_SQUAREONLY) && nWidth != nHeight) // ���簢�� �ؽ�ó�� �Ǹ�..
	{
		if(nWidth > nHeight) nWidth = nHeight;
		else nHeight = nWidth;
	}

	// ���� ī�尡 256 �̻��� �ؽ�ó�� ���� ���� ���ϸ�..
	if(nWidth > 256 && CN3Base::s_DevCaps.MaxTextureWidth <= 256) nWidth = CN3Base::s_DevCaps.MaxTextureWidth;
	if(nHeight > 256 && CN3Base::s_DevCaps.MaxTextureHeight <= 256) nHeight = CN3Base::s_DevCaps.MaxTextureHeight;

	// ��� ����..
	memset(&m_Header, 0, sizeof(m_Header));

	// MipMap �ܰ� ����..
	// 4 X 4 �ȼ������� MipMap �� �����..
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

	__DXT_HEADER HeaderOrg; // ����� ������ ����..
	ReadFile(hFile, &HeaderOrg, sizeof(HeaderOrg), &dwRWC, NULL); // ����� �д´�..
	if(	'N' != HeaderOrg.szID[0] || 'T' != HeaderOrg.szID[1] || 'F' != HeaderOrg.szID[2] || 3 != HeaderOrg.szID[3] ) // "NTF"3 - Noah Texture File Ver. 3.0
	{
#ifdef _N3GAME
		CLogWriter::Write("N3Texture Warning - Old format DXT file (%s)", m_szFileName.c_str());
#endif
	}

	// DXT Format �� �о�� �ϴµ� ������ �Ǵ��� �ȵǴ��� ���� �����ȵǸ� ��ü ������ ���Ѵ�.
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
	if(fmtNew != HeaderOrg.Format) { iWCreate /= 2; iHCreate /= 2; }// DXT ������ �ȵǸ� �ʺ� ���̸� ���δ�.
	this->Create(iWCreate, iHCreate, fmtNew, HeaderOrg.bMipMap); // ���ǽ� �����..

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

					ReadFile(hFile, (uint8_t*)LR.pBits, iTexSize, &dwRWC, NULL); // �Ϸķ� �� �����͸� ����..
					m_lpTexture->UnlockRect(i);
				}

				// �ؽ�ó ����ȵǴ� ���� ī�带 ���� ������ ������ �ǳʶٱ�.. 
				size_t iWTmp = HeaderOrg.nWidth / 2, iHTmp = HeaderOrg.nHeight / 2;
				for(; iWTmp >= 4 && iHTmp >= 4; iWTmp /= 2, iHTmp /= 2) // ���ȼ��� �ι���Ʈ�� ���� A1R5G5B5 Ȥ�� A4R4G4B4 �������� �Ǿ� �ִ�..
					::SetFilePointer(hFile, iWTmp * iHTmp * 2, 0, FILE_CURRENT); // �ǳʶٰ�.
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

				ReadFile(hFile, (uint8_t*)LR.pBits, iTexSize, &dwRWC, NULL); // �Ϸķ� �� �����͸� ����..
				m_lpTexture->UnlockRect(0);

				// �ؽ�ó ����ȵǴ� ���� ī�带 ���� ������ ������ �ǳʶٱ�.. 
				::SetFilePointer(hFile, HeaderOrg.nWidth * HeaderOrg.nHeight / 4, 0, FILE_CURRENT); // �ǳʶٰ�.
				if(HeaderOrg.nWidth >= 1024) SetFilePointer(hFile, 256 * 256 * 2, 0, FILE_CURRENT); // ����� 512 ���� Ŭ��� �εο� ������ �ǳʶٱ�..
			}
		}
		else // DXT ������ �ȵǸ�..
		{
			if(iMMC > 1) // LOD ��ŭ �ǳʶٱ�...
			{
				// ���� ������ �ǳʶٱ�..
				size_t iWTmp = HeaderOrg.nWidth, iHTmp = HeaderOrg.nHeight, iSkipSize = 0;
				for(; iWTmp >= 4 && iHTmp >= 4; iWTmp /= 2, iHTmp /= 2)
				{
					if(D3DFMT_DXT1 == HeaderOrg.Format) iSkipSize += iWTmp * iHTmp / 2; // DXT1 ������ 16��Ʈ ���˿� ���� 1/4 �� ����..
					else iSkipSize += iWTmp * iHTmp; // DXT2 ~ DXT5 ������ 16��Ʈ ���˿� ���� 1/2 �� ����..
				}
				::SetFilePointer(hFile, iSkipSize, 0, FILE_CURRENT); // �ǳʶٰ�.

				// LOD ��ŭ �ǳʶٱ�..
				iWTmp = HeaderOrg.nWidth / 2; iHTmp = HeaderOrg.nHeight / 2; iSkipSize = 0;

				// ���� ī�� ���� �ؽ�ó ũ�Ⱑ ������� �ǳʶٱ�..
				for(; iWTmp > CN3Base::s_DevCaps.MaxTextureWidth || iHTmp > CN3Base::s_DevCaps.MaxTextureHeight; iWTmp /= 2, iHTmp /= 2)
					iSkipSize += iWTmp * iHTmp * 2;
				if(iSkipSize) ::SetFilePointer(hFile, iSkipSize, 0, FILE_CURRENT); // �ǳʶٰ�.

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
				// ���� ������ �ǳʶٱ�..
				int iWTmp = HeaderOrg.nWidth, iHTmp = HeaderOrg.nHeight, iSkipSize = 0;
				if(D3DFMT_DXT1 == HeaderOrg.Format) iSkipSize = iWTmp * iHTmp / 2; // DXT1 ������ 16��Ʈ ���˿� ���� 1/4 �� ����..
				else iSkipSize = iWTmp * iHTmp; // DXT2 ~ DXT5 ������ 16��Ʈ ���˿� ���� 1/2 �� ����..

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
			// ���� ī�� ���� �ؽ�ó ũ�Ⱑ ������� �ǳʶٱ�..
			size_t iWTmp = HeaderOrg.nWidth, iHTmp = HeaderOrg.nHeight, iSkipSize = 0;
			for(; iWTmp > CN3Base::s_DevCaps.MaxTextureWidth || iHTmp > CN3Base::s_DevCaps.MaxTextureHeight; iWTmp /= 2, iHTmp /= 2)
				iSkipSize += iWTmp * iHTmp * iPixelSize;
			if(iSkipSize) ::SetFilePointer(hFile, iSkipSize, 0, FILE_CURRENT); // �ǳʶٰ�.

			// ������ �б�..
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
			// ���� ī�� ���� �ؽ�ó ũ�Ⱑ ������� �ǳʶٱ�..
			if(HeaderOrg.nWidth >= 512 && m_Header.nWidth <= 256)
				::SetFilePointer(hFile, HeaderOrg.nWidth * HeaderOrg.nHeight * iPixelSize, 0, FILE_CURRENT); // �ǳʶٰ�.

			m_lpTexture->GetLevelDesc(0, &sd);
			m_lpTexture->LockRect(0, &LR, NULL, NULL);
			for(int y = 0; y < (int)sd.Height; y++)
				ReadFile(hFile, (uint8_t*)LR.pBits + y * LR.Pitch, iPixelSize * sd.Width, &dwRWC, NULL);
			m_lpTexture->UnlockRect(0);

			if(m_Header.nWidth >= 512 && m_Header.nHeight >= 512)
				SetFilePointer(hFile, 256 * 256 * 2, 0, FILE_CURRENT); // ����� 512 ���� Ŭ��� �εο� ������ �ǳʶٱ�..
		}
	}
//	this->GenerateMipMap(); // Mip Map �� �����..
	return true;
}

bool CN3Texture::SkipFileHandle(HANDLE hFile)
{
	__DXT_HEADER HeaderOrg; // ����� ������ ����..
	DWORD dwRWC = 0;
	ReadFile(hFile, &HeaderOrg, sizeof(HeaderOrg), &dwRWC, NULL); // ����� �д´�..
	if(	'N' != HeaderOrg.szID[0] || 'T' != HeaderOrg.szID[1] || 'F' != HeaderOrg.szID[2] || 3 != HeaderOrg.szID[3] ) // "NTF"3 - Noah Texture File Ver. 3.0
	{
#ifdef _N3GAME
		CLogWriter::Write("N3Texture Warning - Old format DXT file (%s)", m_szFileName.c_str());
#endif
	}

	// ���� �����̸�..
	if(	D3DFMT_DXT1 == HeaderOrg.Format || 
		D3DFMT_DXT2 == HeaderOrg.Format || 
		D3DFMT_DXT3 == HeaderOrg.Format || 
		D3DFMT_DXT4 == HeaderOrg.Format || 
		D3DFMT_DXT5 == HeaderOrg.Format )
	{
		int iWTmp = HeaderOrg.nWidth, iHTmp = HeaderOrg.nHeight, iSkipSize = 0;
		if(HeaderOrg.bMipMap)
		{
			// ���� ������ �ǳʶٱ�..
			for(; iWTmp >= 4 && iHTmp >= 4; iWTmp/=2, iHTmp/=2)
			{
				if(D3DFMT_DXT1 == HeaderOrg.Format) iSkipSize += iWTmp * iHTmp / 2;
				else iSkipSize += iWTmp * iHTmp;
			}
			// �ؽ�ó ����ȵǴ� ���� ī�带 ���� ������ ������ �ǳʶٱ�.. 
			iWTmp = HeaderOrg.nWidth / 2; iHTmp = HeaderOrg.nHeight / 2;
			for(; iWTmp >= 4 && iHTmp >= 4; iWTmp /= 2, iHTmp /= 2) // ���ȼ��� �ι���Ʈ�� ���� A1R5G5B5 Ȥ�� A4R4G4B4 �������� �Ǿ� �ִ�..
				iSkipSize += iWTmp * iHTmp * 2; // �ǳʶٰ�.
		}
		else // pair of if(HeaderOrg.bMipMap)
		{
			// ���� ������ �ǳʶٱ�..
			if(D3DFMT_DXT1 == HeaderOrg.Format) iSkipSize += HeaderOrg.nWidth * HeaderOrg.nHeight / 2;
			else iSkipSize += iSkipSize += HeaderOrg.nWidth * HeaderOrg.nHeight;

			// �ؽ�ó ����ȵǴ� ���� ī�带 ���� ������ ������ �ǳʶٱ�.. 
			iSkipSize += HeaderOrg.nWidth * HeaderOrg.nHeight * 2;
			if(HeaderOrg.nWidth >= 1024) iSkipSize += 256 * 256 * 2; // ����� 1024 ���� Ŭ��� �εο� ������ �ǳʶٱ�..
		}

		::SetFilePointer(hFile, iSkipSize, 0, FILE_CURRENT); // �ǳʶٰ�.
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
			if(HeaderOrg.nWidth >= 512) iSkipSize += 256 * 256 * 2; // ����� 512 ���� Ŭ��� �εο� ������ �ǳʶٱ�..
		}
		
		::SetFilePointer(hFile, iSkipSize, 0, FILE_CURRENT); // �ǳʶٰ�.
	}
	return true;
}

void CN3Texture::UpdateRenderInfo()
{
}
