// N3Texture.cpp: implementation of the CN3Texture class.
//
//////////////////////////////////////////////////////////////////////
#include "N3Texture.h"

#ifdef _N3TOOL
//#include "BitmapFile.h"
#endif // #ifdef _N3TOOL

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CN3Texture::CN3Texture()
{
	m_dwType |= OBJ_TEXTURE;

	memset(&m_Header, 0, sizeof(m_Header));
	m_lpTexture = NULL;
	m_iLOD = 0;
}

CN3Texture::~CN3Texture()
{
	if (m_lpTexture)
	{
		int nRefCount = m_lpTexture->Release();
		if (nRefCount == 0) m_lpTexture = NULL;
	}
}

void CN3Texture::Release()
{
	if (32 == m_Header.nWidth && 32 == m_Header.nHeight) s_ResrcInfo.nTexture_Loaded_32X32--;
	else if (64 == m_Header.nWidth && 64 == m_Header.nHeight) s_ResrcInfo.nTexture_Loaded_64X64--;
	else if (128 == m_Header.nWidth && 128 == m_Header.nHeight) s_ResrcInfo.nTexture_Loaded_128X128--;
	else if (256 == m_Header.nWidth && 256 == m_Header.nHeight) s_ResrcInfo.nTexture_Loaded_256X256--;
	else if (512 == m_Header.nWidth && 512 == m_Header.nHeight) s_ResrcInfo.nTexture_Loaded_512X512--;
	else if (512 < m_Header.nWidth && 512 < m_Header.nHeight) s_ResrcInfo.nTexture_Loaded_Huge--;
	else s_ResrcInfo.nTexture_Loaded_OtherSize--;

	memset(&m_Header, 0, sizeof(m_Header));
	if (m_lpTexture && m_lpTexture->Release() == 0) m_lpTexture = NULL;
	m_iLOD = 0;

	CN3BaseFileAccess::Release();
}

bool CN3Texture::Create(int nWidth, int nHeight, D3DFORMAT Format, BOOL bGenerateMipMap)
{
	if (nWidth != nHeight)

		if (nWidth <= 1 || nHeight <= 1 || D3DFMT_UNKNOWN == Format) return false;
	if (m_lpTexture != NULL) this->Release();

	if (s_dwTextureCaps & TEX_CAPS_POW2) // 2 �� �¼��� �ȴٸ�..
	{
		int nW, nH;
		for (nW = 1; nW <= nWidth; nW *= 2); nW /= 2;
		for (nH = 1; nH <= nHeight; nH *= 2); nH /= 2;

		nWidth = nW;
		nHeight = nH;
	}

	if ((s_dwTextureCaps & TEX_CAPS_SQUAREONLY) && nWidth != nHeight) // ���簢�� �ؽ�ó�� �Ǹ�..
	{
		if (nWidth > nHeight) nWidth = nHeight;
		else nHeight = nWidth;
	}

	// ���� ī�尡 256 �̻��� �ؽ�ó�� ���� ���� ���ϸ�..
	if (nWidth > 256 && CN3Base::s_DevCaps.MaxTextureWidth <= 256) nWidth = CN3Base::s_DevCaps.MaxTextureWidth;
	if (nHeight > 256 && CN3Base::s_DevCaps.MaxTextureHeight <= 256) nHeight = CN3Base::s_DevCaps.MaxTextureHeight;

	// ��� ����..
	memset(&m_Header, 0, sizeof(m_Header));

	// MipMap �ܰ� ����..
	// 4 X 4 �ȼ������� MipMap �� �����..
	int nMMC = 1;
	if (bGenerateMipMap)
	{
		nMMC = 0;
		for (int nW = nWidth, nH = nHeight; nW >= 4 && nH >= 4; nW /= 2, nH /= 2) nMMC++;
	}

	HRESULT rval = s_lpD3DDev->CreateTexture(nWidth, nHeight, nMMC, 0, Format, D3DPOOL_MANAGED, &m_lpTexture, NULL);

#ifdef _N3GAME
	if (rval == D3DERR_INVALIDCALL)
	{
		CLogWriter::Write("N3Texture: createtexture err D3DERR_INVALIDCALL(%s)", m_szFileName.c_str());
		return false;
	}
	if (rval == D3DERR_OUTOFVIDEOMEMORY)
	{
		CLogWriter::Write("N3Texture: createtexture err D3DERR_OUTOFVIDEOMEMORY(%s)", m_szFileName.c_str());
		return false;
	}
	if (rval == E_OUTOFMEMORY)
	{
		CLogWriter::Write("N3Texture: createtexture err E_OUTOFMEMORY(%s)", m_szFileName.c_str());
		return false;
	}
#endif
	if (NULL == m_lpTexture)
	{
		__ASSERT(m_lpTexture, "Texture pointer is NULL!");
		return false;
	}


	m_Header.nWidth = nWidth;
	m_Header.nHeight = nHeight;
	m_Header.Format = Format;
	m_Header.bMipMap = bGenerateMipMap;

	if (32 == m_Header.nWidth && 32 == m_Header.nHeight) s_ResrcInfo.nTexture_Loaded_32X32++;
	else if (64 == m_Header.nWidth && 64 == m_Header.nHeight) s_ResrcInfo.nTexture_Loaded_64X64++;
	else if (128 == m_Header.nWidth && 128 == m_Header.nHeight) s_ResrcInfo.nTexture_Loaded_128X128++;
	else if (256 == m_Header.nWidth && 256 == m_Header.nHeight) s_ResrcInfo.nTexture_Loaded_256X256++;
	else if (512 == m_Header.nWidth && 512 == m_Header.nHeight) s_ResrcInfo.nTexture_Loaded_512X512++;
	else if (512 < m_Header.nWidth && 512 < m_Header.nHeight) s_ResrcInfo.nTexture_Loaded_Huge++;
	else s_ResrcInfo.nTexture_Loaded_OtherSize++;

	return true;
}

#ifdef _N3TOOL
bool CN3Texture::CreateFromSurface(LPDIRECT3DSURFACE9 lpSurf, D3DFORMAT Format, BOOL bGenerateMipMap)
{
	if (lpSurf == NULL) return false;

	D3DSURFACE_DESC sd;
	lpSurf->GetDesc(&sd);

	if (this->Create(sd.Width, sd.Height, Format, bGenerateMipMap) == false) return false;
	if (bGenerateMipMap)
	{
		this->GenerateMipMap(lpSurf);
	}

	return true;
}
#endif // end of _N3TOOL

bool CN3Texture::LoadFromFile(const std::string& szFileName, uint32_t iVer)
{
	m_iFileFormatVersion = iVer;

	if (m_lpTexture != NULL) this->Release();

	this->FileNameSet(szFileName); // ���� �̸��� �����ϰ�..
	std::string szFullPath;
	if (-1 != m_szFileName.find(':') || -1 != m_szFileName.find("\\\\") || -1 != m_szFileName.find("//")) // ���ڿ��� ':', '\\', '//' �� ��� ������ ��ü ����̴�..
	{
		szFullPath = m_szFileName;
	}
	else
	{
		if (NULL != s_szPath[0]) szFullPath = s_szPath;
		szFullPath += m_szFileName;
	}

	int nFNL = szFullPath.size();
	if (lstrcmpi(&szFullPath[nFNL - 3], "DXT") == 0)
	{
		HANDLE hFile = ::CreateFile(szFullPath.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			auto lastError = GetLastError();

			OutputDebugString("texture load error");
#ifdef _N3GAME
			// NOTE: temp correction for fukd up uifs
			//       - 63 is used to jump past the old texture locations
			//       - also note that the #ifdef is messed up here now...
			if (nFNL > 63) {
				OutputDebugString("WR: this is a temp fix for file -> ");
				OutputDebugString(&szFullPath[63]);
				OutputDebugString("\n");
				hFile = ::CreateFile(&szFullPath[63], GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			}

			if (hFile == INVALID_HANDLE_VALUE) {
				// NOTE: another temp fix
				szFullPath = "ui/ui_ka_statebar.dxt";
				hFile = ::CreateFile(szFullPath.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

				if (hFile == INVALID_HANDLE_VALUE) {
					CLogWriter::Write("invalid file handle(%d) - Can't open texture file(%s)", (int)hFile, szFullPath.c_str());
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
		HRESULT rval = D3DXCreateTextureFromFileEx(s_lpD3DDev,
			szFullPath.c_str(),
			D3DX_DEFAULT,
			D3DX_DEFAULT,
			D3DX_DEFAULT,
			0,
			D3DFMT_UNKNOWN,
			D3DPOOL_MANAGED,
			D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
			D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
			0,
			&ImgInfo,
			NULL,
			&m_lpTexture);
		if (rval == D3D_OK)
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
			CLogWriter::Write("N3Texture - Failed to load texture(%s)", szFullPath.c_str());
#endif
		}

		if (32 == m_Header.nWidth && 32 == m_Header.nHeight) s_ResrcInfo.nTexture_Loaded_32X32++;
		else if (64 == m_Header.nWidth && 64 == m_Header.nHeight) s_ResrcInfo.nTexture_Loaded_64X64++;
		else if (128 == m_Header.nWidth && 128 == m_Header.nHeight) s_ResrcInfo.nTexture_Loaded_128X128++;
		else if (256 == m_Header.nWidth && 256 == m_Header.nHeight) s_ResrcInfo.nTexture_Loaded_256X256++;
		else if (512 == m_Header.nWidth && 512 == m_Header.nHeight) s_ResrcInfo.nTexture_Loaded_512X512++;
		else if (512 < m_Header.nWidth && 512 < m_Header.nHeight) s_ResrcInfo.nTexture_Loaded_Huge++;
		else s_ResrcInfo.nTexture_Loaded_OtherSize++;
	}

	if (NULL == m_lpTexture)
	{
		this->Release();
		return false;
	}
	return true;
}

bool CN3Texture::Load(HANDLE hFile)
{
	CN3BaseFileAccess::Load(hFile);

	DWORD dwRWC = 0;

	__DXT_HEADER HeaderOrg; // ����� ������ ����..
	ReadFile(hFile, &HeaderOrg, sizeof(HeaderOrg), &dwRWC, NULL); // ����� �д´�..
	if ('N' != HeaderOrg.szID[0] || 'T' != HeaderOrg.szID[1] || 'F' != HeaderOrg.szID[2] || 3 != HeaderOrg.szID[3]) // "NTF"3 - Noah Texture File Ver. 3.0
	{
#ifdef _N3GAME
		CLogWriter::Write("N3Texture Warning - Old format DXT file (%s)", m_szFileName.c_str());
#endif
	}

	// DXT Format �� �о�� �ϴµ� ������ �Ǵ��� �ȵǴ��� ���� �����ȵǸ� ��ü ������ ���Ѵ�.
	bool bDXTSupport = FALSE;
	D3DFORMAT fmtNew = HeaderOrg.Format;
	if (D3DFMT_DXT1 == HeaderOrg.Format)
	{
		if (s_dwTextureCaps & TEX_CAPS_DXT1) bDXTSupport = true;
		else fmtNew = D3DFMT_A1R5G5B5;
	}
	else if (D3DFMT_DXT2 == HeaderOrg.Format)
	{
		if (s_dwTextureCaps & TEX_CAPS_DXT2) bDXTSupport = true;
		else fmtNew = D3DFMT_A4R4G4B4;
	}
	else if (D3DFMT_DXT3 == HeaderOrg.Format)
	{
		if (s_dwTextureCaps & TEX_CAPS_DXT3) bDXTSupport = true;
		else fmtNew = D3DFMT_A4R4G4B4;
	}
	else if (D3DFMT_DXT4 == HeaderOrg.Format)
	{
		if (s_dwTextureCaps & TEX_CAPS_DXT4) bDXTSupport = true;
		else fmtNew = D3DFMT_A4R4G4B4;
	}
	else if (D3DFMT_DXT5 == HeaderOrg.Format)
	{
		if (s_dwTextureCaps & TEX_CAPS_DXT5) bDXTSupport = true;
		else fmtNew = D3DFMT_A4R4G4B4;
	}

	int iWCreate = HeaderOrg.nWidth, iHCreate = HeaderOrg.nHeight;
	if (fmtNew != HeaderOrg.Format) { iWCreate /= 2; iHCreate /= 2; }// DXT ������ �ȵǸ� �ʺ� ���̸� ���δ�.
	if (m_iLOD > 0 && m_iLOD <= 2 && HeaderOrg.nWidth >= 16 && HeaderOrg.nHeight >= 16) // LOD ��ŭ �۰� ����..
	{
		for (int i = 0; i < m_iLOD; i++)
		{
			iWCreate /= 2;
			iHCreate /= 2;
		}
	}
	else m_iLOD = 0; // LOD ������ �ƴϸ�..

	int iLODPrev = m_iLOD;
	this->Create(iWCreate, iHCreate, fmtNew, HeaderOrg.bMipMap); // ���ǽ� �����..
	m_iLOD = iLODPrev;

	if (m_lpTexture == NULL)
	{
#ifdef _N3GAME
		CLogWriter::Write("N3Texture error - Can't create texture (%s)", m_szFileName.c_str());
#endif
		return false;
	}

	D3DSURFACE_DESC sd;
	D3DLOCKED_RECT LR;
	int iMMC = m_lpTexture->GetLevelCount(); // ������ MipMap ��

	// ���� �����̸�..
	if (D3DFMT_DXT1 == HeaderOrg.Format ||
		D3DFMT_DXT2 == HeaderOrg.Format ||
		D3DFMT_DXT3 == HeaderOrg.Format ||
		D3DFMT_DXT4 == HeaderOrg.Format ||
		D3DFMT_DXT5 == HeaderOrg.Format)
	{
		if (TRUE == bDXTSupport) // ���� �ؽ�ó �����̸�..
		{
			if (iMMC > 1)
			{
				if (m_iLOD > 0) // LOD ��ŭ �ǳʶٱ�...
				{
					size_t iWTmp = HeaderOrg.nWidth, iHTmp = HeaderOrg.nHeight, iSkipSize = 0;
					for (int i = 0; i < m_iLOD; i++, iWTmp /= 2, iHTmp /= 2)
					{
						if (D3DFMT_DXT1 == HeaderOrg.Format) iSkipSize += iWTmp * iHTmp / 2; // DXT1 ������ 16��Ʈ ���˿� ���� 1/4 �� ����..
						else iSkipSize += iWTmp * iHTmp; // DXT2 ~ DXT5 ������ 16��Ʈ ���˿� ���� 1/2 �� ����..
					}
					::SetFilePointer(hFile, iSkipSize, 0, FILE_CURRENT); // �ǳʶٰ�.
				}

				for (int i = 0; i < iMMC; i++)
				{
					m_lpTexture->GetLevelDesc(i, &sd);
					m_lpTexture->LockRect(i, &LR, NULL, NULL);

					int iTexSize = 0;
					switch (HeaderOrg.Format) {
					case D3DFMT_DXT1: {
						iTexSize = (sd.Width * sd.Height / 2);
					} break;
					case D3DFMT_DXT2: {
						printf("ERROR: D3DFMT_DXT2\n");
						system("pause");
						exit(-1);
					} break;
					case D3DFMT_DXT3: {
						iTexSize = (sd.Width * sd.Height);
					} break;
					case D3DFMT_DXT4: {
						printf("ERROR: D3DFMT_DXT4\n");
						system("pause");
						exit(-1);
					} break;
					case D3DFMT_DXT5: {
						iTexSize = (sd.Width * sd.Height * 2);
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
				for (; iWTmp >= 4 && iHTmp >= 4; iWTmp /= 2, iHTmp /= 2) // ���ȼ��� �ι���Ʈ�� ���� A1R5G5B5 Ȥ�� A4R4G4B4 �������� �Ǿ� �ִ�..
					::SetFilePointer(hFile, iWTmp * iHTmp * 2, 0, FILE_CURRENT); // �ǳʶٰ�.
			}
			else // pair of if(iMMC > 1)
			{
				m_lpTexture->GetLevelDesc(0, &sd);
				m_lpTexture->LockRect(0, &LR, NULL, NULL);

				int iTexSize = 0;
				switch (HeaderOrg.Format) {
				case D3DFMT_DXT1: {
					iTexSize = (sd.Width * sd.Height / 2);
				} break;
				case D3DFMT_DXT2: {
					printf("ERROR: D3DFMT_DXT2\n");
					system("pause");
					exit(-1);
				} break;
				case D3DFMT_DXT3: {
					iTexSize = (sd.Width * sd.Height);
				} break;
				case D3DFMT_DXT4: {
					printf("ERROR: D3DFMT_DXT4\n");
					system("pause");
					exit(-1);
				} break;
				case D3DFMT_DXT5: {
					iTexSize = (sd.Width * sd.Height * 2);
					printf("ERROR: D3DFMT_DXT5\n");
					system("pause");
					exit(-1);
				} break;
				}

				ReadFile(hFile, (uint8_t*)LR.pBits, iTexSize, &dwRWC, NULL); // �Ϸķ� �� �����͸� ����..
				m_lpTexture->UnlockRect(0);

				// �ؽ�ó ����ȵǴ� ���� ī�带 ���� ������ ������ �ǳʶٱ�.. 
				::SetFilePointer(hFile, HeaderOrg.nWidth * HeaderOrg.nHeight / 4, 0, FILE_CURRENT); // �ǳʶٰ�.
				if (HeaderOrg.nWidth >= 1024) SetFilePointer(hFile, 256 * 256 * 2, 0, FILE_CURRENT); // ����� 512 ���� Ŭ��� �εο� ������ �ǳʶٱ�..
			}
		}
		else // DXT ������ �ȵǸ�..
		{
			if (iMMC > 1) // LOD ��ŭ �ǳʶٱ�...
			{
				// ���� ������ �ǳʶٱ�..
				size_t iWTmp = HeaderOrg.nWidth, iHTmp = HeaderOrg.nHeight, iSkipSize = 0;
				for (; iWTmp >= 4 && iHTmp >= 4; iWTmp /= 2, iHTmp /= 2)
				{
					if (D3DFMT_DXT1 == HeaderOrg.Format) iSkipSize += iWTmp * iHTmp / 2; // DXT1 ������ 16��Ʈ ���˿� ���� 1/4 �� ����..
					else iSkipSize += iWTmp * iHTmp; // DXT2 ~ DXT5 ������ 16��Ʈ ���˿� ���� 1/2 �� ����..
				}
				::SetFilePointer(hFile, iSkipSize, 0, FILE_CURRENT); // �ǳʶٰ�.

				// LOD ��ŭ �ǳʶٱ�..
				iWTmp = HeaderOrg.nWidth / 2; iHTmp = HeaderOrg.nHeight / 2; iSkipSize = 0;
				if (m_iLOD > 0)
				{
					for (int i = 0; i < m_iLOD; i++, iWTmp /= 2, iHTmp /= 2)
						iSkipSize += iWTmp * iHTmp * 2; // ��ġ�� �ʺ� ������ �ȼ��� ũ��� �����Ѵ�...
				}

				// ���� ī�� ���� �ؽ�ó ũ�Ⱑ ������� �ǳʶٱ�..
				for (; iWTmp > s_DevCaps.MaxTextureWidth || iHTmp > s_DevCaps.MaxTextureHeight; iWTmp /= 2, iHTmp /= 2)
					iSkipSize += iWTmp * iHTmp * 2;
				if (iSkipSize) ::SetFilePointer(hFile, iSkipSize, 0, FILE_CURRENT); // �ǳʶٰ�.

				for (int i = 0; i < iMMC; i++)
				{
					m_lpTexture->GetLevelDesc(i, &sd);
					m_lpTexture->LockRect(i, &LR, NULL, NULL);
					int nH = sd.Height;
					for (int y = 0; y < nH; y++)
						ReadFile(hFile, (uint8_t*)LR.pBits + y * LR.Pitch, 2 * sd.Width, &dwRWC, NULL);
					m_lpTexture->UnlockRect(i);
				}
			}
			else // pair of if(iMMC > 1)
			{
				// ���� ������ �ǳʶٱ�..
				int iWTmp = HeaderOrg.nWidth, iHTmp = HeaderOrg.nHeight, iSkipSize = 0;
				if (D3DFMT_DXT1 == HeaderOrg.Format) iSkipSize = iWTmp * iHTmp / 2; // DXT1 ������ 16��Ʈ ���˿� ���� 1/4 �� ����..
				else iSkipSize = iWTmp * iHTmp; // DXT2 ~ DXT5 ������ 16��Ʈ ���˿� ���� 1/2 �� ����..

			}
		}
	}
	else
	{
		int iPixelSize = 0;
		if (fmtNew == D3DFMT_A1R5G5B5 || fmtNew == D3DFMT_A4R4G4B4) iPixelSize = 2;
		else if (fmtNew == D3DFMT_R8G8B8) iPixelSize = 3;
		else if (fmtNew == D3DFMT_A8R8G8B8 || fmtNew == D3DFMT_X8R8G8B8) iPixelSize = 4;
		else
		{
			__ASSERT(0, "Not supported texture format");
		}

		if (iMMC > 1)
		{
			if (m_iLOD > 0) // LOD ��ŭ �ǳʶٱ�...
			{
				int iWTmp = HeaderOrg.nWidth, iHTmp = HeaderOrg.nHeight, iSkipSize = 0;
				for (int i = 0; i < m_iLOD; i++, iWTmp /= 2, iHTmp /= 2) iSkipSize += iWTmp * iHTmp * iPixelSize; // ��ġ�� �ʺ� ������ �ȼ��� ũ��� �����Ѵ�...
				::SetFilePointer(hFile, iSkipSize, 0, FILE_CURRENT); // �ǳʶٰ�.
			}

			// ���� ī�� ���� �ؽ�ó ũ�Ⱑ ������� �ǳʶٱ�..
			size_t iWTmp = HeaderOrg.nWidth, iHTmp = HeaderOrg.nHeight, iSkipSize = 0;
			for (; iWTmp > s_DevCaps.MaxTextureWidth || iHTmp > s_DevCaps.MaxTextureHeight; iWTmp /= 2, iHTmp /= 2)
				iSkipSize += iWTmp * iHTmp * iPixelSize;
			if (iSkipSize) ::SetFilePointer(hFile, iSkipSize, 0, FILE_CURRENT); // �ǳʶٰ�.

			// ������ �б�..
			for (int i = 0; i < iMMC; i++)
			{
				m_lpTexture->GetLevelDesc(i, &sd);
				m_lpTexture->LockRect(i, &LR, NULL, NULL);
				for (int y = 0; y < (int)sd.Height; y++)
					ReadFile(hFile, (uint8_t*)LR.pBits + y * LR.Pitch, iPixelSize * sd.Width, &dwRWC, NULL);
				m_lpTexture->UnlockRect(i);
			}
		}
		else // pair of if(iMMC > 1)
		{
			// ���� ī�� ���� �ؽ�ó ũ�Ⱑ ������� �ǳʶٱ�..
			if (HeaderOrg.nWidth >= 512 && m_Header.nWidth <= 256)
				::SetFilePointer(hFile, HeaderOrg.nWidth * HeaderOrg.nHeight * iPixelSize, 0, FILE_CURRENT); // �ǳʶٰ�.

			m_lpTexture->GetLevelDesc(0, &sd);
			m_lpTexture->LockRect(0, &LR, NULL, NULL);
			for (int y = 0; y < (int)sd.Height; y++)
				ReadFile(hFile, (uint8_t*)LR.pBits + y * LR.Pitch, iPixelSize * sd.Width, &dwRWC, NULL);
			m_lpTexture->UnlockRect(0);

			if (m_Header.nWidth >= 512 && m_Header.nHeight >= 512)
				SetFilePointer(hFile, 256 * 256 * 2, 0, FILE_CURRENT); // ����� 512 ���� Ŭ��� �εο� ������ �ǳʶٱ�..
		}
	}
	//	this->GenerateMipMap(); // Mip Map �� �����..
	return true;
}

bool CN3Texture::SkipFileHandle(HANDLE hFile)
{
	CN3BaseFileAccess::Load(hFile);

	__DXT_HEADER HeaderOrg; // ����� ������ ����..
	DWORD dwRWC = 0;
	ReadFile(hFile, &HeaderOrg, sizeof(HeaderOrg), &dwRWC, NULL); // ����� �д´�..
	if ('N' != HeaderOrg.szID[0] || 'T' != HeaderOrg.szID[1] || 'F' != HeaderOrg.szID[2] || 3 != HeaderOrg.szID[3]) // "NTF"3 - Noah Texture File Ver. 3.0
	{
#ifdef _N3GAME
		CLogWriter::Write("N3Texture Warning - Old format DXT file (%s)", m_szFileName.c_str());
#endif
	}

	// ���� �����̸�..
	if (D3DFMT_DXT1 == HeaderOrg.Format ||
		D3DFMT_DXT2 == HeaderOrg.Format ||
		D3DFMT_DXT3 == HeaderOrg.Format ||
		D3DFMT_DXT4 == HeaderOrg.Format ||
		D3DFMT_DXT5 == HeaderOrg.Format)
	{
		int iWTmp = HeaderOrg.nWidth, iHTmp = HeaderOrg.nHeight, iSkipSize = 0;
		if (HeaderOrg.bMipMap)
		{
			// ���� ������ �ǳʶٱ�..
			for (; iWTmp >= 4 && iHTmp >= 4; iWTmp /= 2, iHTmp /= 2)
			{
				if (D3DFMT_DXT1 == HeaderOrg.Format) iSkipSize += iWTmp * iHTmp / 2;
				else iSkipSize += iWTmp * iHTmp;
			}
			// �ؽ�ó ����ȵǴ� ���� ī�带 ���� ������ ������ �ǳʶٱ�.. 
			iWTmp = HeaderOrg.nWidth / 2; iHTmp = HeaderOrg.nHeight / 2;
			for (; iWTmp >= 4 && iHTmp >= 4; iWTmp /= 2, iHTmp /= 2) // ���ȼ��� �ι���Ʈ�� ���� A1R5G5B5 Ȥ�� A4R4G4B4 �������� �Ǿ� �ִ�..
				iSkipSize += iWTmp * iHTmp * 2; // �ǳʶٰ�.
		}
		else // pair of if(HeaderOrg.bMipMap)
		{
			// ���� ������ �ǳʶٱ�..
			if (D3DFMT_DXT1 == HeaderOrg.Format) iSkipSize += HeaderOrg.nWidth * HeaderOrg.nHeight / 2;
			else iSkipSize += iSkipSize += HeaderOrg.nWidth * HeaderOrg.nHeight;

			// �ؽ�ó ����ȵǴ� ���� ī�带 ���� ������ ������ �ǳʶٱ�.. 
			iSkipSize += HeaderOrg.nWidth * HeaderOrg.nHeight * 2;
			if (HeaderOrg.nWidth >= 1024) iSkipSize += 256 * 256 * 2; // ����� 1024 ���� Ŭ��� �εο� ������ �ǳʶٱ�..
		}

		::SetFilePointer(hFile, iSkipSize, 0, FILE_CURRENT); // �ǳʶٰ�.
	}
	else
	{
		int iPixelSize = 0;
		if (HeaderOrg.Format == D3DFMT_A1R5G5B5 || HeaderOrg.Format == D3DFMT_A4R4G4B4) iPixelSize = 2;
		else if (HeaderOrg.Format == D3DFMT_R8G8B8) iPixelSize = 3;
		else if (HeaderOrg.Format == D3DFMT_A8R8G8B8 || HeaderOrg.Format == D3DFMT_X8R8G8B8) iPixelSize = 4;
		else
		{
			__ASSERT(0, "Not supported texture format");
		}

		int iWTmp = HeaderOrg.nWidth, iHTmp = HeaderOrg.nHeight, iSkipSize = 0;
		if (HeaderOrg.bMipMap)
		{
			for (; iWTmp >= 4 && iHTmp >= 4; iWTmp /= 2, iHTmp /= 2)
				iSkipSize += iWTmp * iHTmp * iPixelSize;
		}
		else
		{
			iSkipSize += iWTmp * iHTmp * iPixelSize;
			if (HeaderOrg.nWidth >= 512) iSkipSize += 256 * 256 * 2; // ����� 512 ���� Ŭ��� �εο� ������ �ǳʶٱ�..
		}

		::SetFilePointer(hFile, iSkipSize, 0, FILE_CURRENT); // �ǳʶٰ�.
	}
	return true;
}


#ifdef _N3TOOL
bool CN3Texture::SaveToFile()
{
	char szExt[_MAX_EXT];
	_splitpath(m_szFileName.c_str(), NULL, NULL, NULL, szExt);
	if (lstrcmpi(szExt, ".dxt") != 0) return false;

	return CN3BaseFileAccess::SaveToFile();
}
#endif // end of _N3TOOL


#ifdef _N3TOOL
bool CN3Texture::SaveToFile(const std::string& szFileName)
{
	this->FileNameSet(szFileName);
	return this->SaveToFile();
}
#endif // end of _N3TOOL


#ifdef _N3TOOL
bool CN3Texture::Save(HANDLE hFile)
{
	// TODO(srmeier): implement this
	return false;

	/*
	if(NULL == m_lpTexture) return false;

	CN3BaseFileAccess::Save(hFile);

	DWORD dwRWC = 0;

	D3DSURFACE_DESC sd;
	m_lpTexture->GetLevelDesc(0, &sd);

	int nMMC = m_lpTexture->GetLevelCount();
	(nMMC > 1) ? m_Header.bMipMap = TRUE : m_Header.bMipMap = FALSE;
	if(TRUE == m_Header.bMipMap) // MipMap ������ �´��� Ȯ��..
	{
		int nMMC2 = 0;
		for(int nW = sd.Width, nH = sd.Height; nW >=4 && nH >= 4; nW /=2, nH /= 2) nMMC2++;
		if(nMMC < nMMC2)
		{
#ifdef _N3GAME
			CLogWriter::Write("N3Texture save warning - Invalid MipMap Count (%s)", m_szFileName.c_str());
#endif
			m_Header.bMipMap = FALSE;
			nMMC = 1;
		}
		else
		{
			nMMC = nMMC2;
		}
	}

	m_Header.szID[0] = 'N';
	m_Header.szID[1] = 'T';
	m_Header.szID[2] = 'F';
	m_Header.szID[3] = 3; // Noah Texture File Ver '3'
	m_Header.nWidth = sd.Width;
	m_Header.nHeight = sd.Height;
	m_Header.bMipMap = (nMMC > 1) ? TRUE : FALSE;

	WriteFile(hFile, &m_Header, sizeof(m_Header), &dwRWC, NULL); // ����� ����

	if(m_lpTexture == NULL) return false;

	if(	D3DFMT_DXT1 == sd.Format ||
		D3DFMT_DXT2 == sd.Format ||
		D3DFMT_DXT3 == sd.Format ||
		D3DFMT_DXT4 == sd.Format ||
		D3DFMT_DXT5 == sd.Format )
	{
		D3DLOCKED_RECT LR;

		for(int i = 0; i < nMMC; i++)
		{
			m_lpTexture->GetLevelDesc(i, &sd);

			m_lpTexture->LockRect(i, &LR, NULL, NULL);
			WriteFile(hFile, (uint8_t*)LR.pBits, sd.Size, &dwRWC, NULL); // �Ϸķ� �� �����͸� ����..
			m_lpTexture->UnlockRect(i);
		}

		// �߰��� ������� ���� ������ ���ش�.. ���� ũ���̴�.
		// ������� ���� ������ �ػ󵵸� �Ѵܰ� ���߾ ����.
		LPDIRECT3DSURFACE8 lpSurfSrc = NULL, lpSurfDest = NULL;
		D3DFORMAT fmtExtra = D3DFMT_UNKNOWN;
		if(D3DFMT_DXT1 == sd.Format) fmtExtra = D3DFMT_A1R5G5B5;
		else fmtExtra = D3DFMT_A4R4G4B4;

		int nMMC2 = nMMC - 1;
		if(nMMC == 1) nMMC2 = nMMC;
		for(i = 0; i < nMMC2; i++)
		{
			m_lpTexture->GetLevelDesc(i, &sd);
			m_lpTexture->GetSurfaceLevel(i, &lpSurfSrc);
			int nW = sd.Width / 2, nH = sd.Height / 2;
			s_lpD3DDev->CreateImageSurface(nW, nH, fmtExtra, &lpSurfDest);
			D3DXLoadSurfaceFromSurface(lpSurfDest, NULL, NULL, lpSurfSrc, NULL, NULL, D3DX_FILTER_TRIANGLE, 0); // ���ǽ� ����.
			int nPixelSize = 2;
			lpSurfDest->LockRect(&LR, NULL, NULL);
			for(int y = 0; y < nH; y++)
			{
				WriteFile(hFile, (uint8_t*)LR.pBits + y * LR.Pitch, nW * 2, &dwRWC, NULL);
			}
			lpSurfDest->UnlockRect();
			lpSurfDest->Release(); lpSurfDest = NULL;
			lpSurfSrc->Release(); lpSurfSrc = NULL;
		}

		if(nMMC == 1 && m_Header.nWidth >= 1024) // �εθ� ���� 256 * 256 ¥�� �ϳ� �� �������ش�..
		{
			m_lpTexture->GetLevelDesc(0, &sd);
			m_lpTexture->GetSurfaceLevel(0, &lpSurfSrc);
			int nW = 256, nH = 256;
			s_lpD3DDev->CreateImageSurface(nW, nH, fmtExtra, &lpSurfDest);
			D3DXLoadSurfaceFromSurface(lpSurfDest, NULL, NULL, lpSurfSrc, NULL, NULL, D3DX_FILTER_TRIANGLE, 0); // ���ǽ� ����.
			int nPixelSize = 2;
			lpSurfDest->LockRect(&LR, NULL, NULL);
			for(int y = 0; y < nH; y++)
			{
				WriteFile(hFile, (uint8_t*)LR.pBits + y * LR.Pitch, nW * 2, &dwRWC, NULL);
			}
			lpSurfDest->UnlockRect();
			lpSurfDest->Release(); lpSurfDest = NULL;
			lpSurfSrc->Release(); lpSurfSrc = NULL;
		}
	}
	else // �Ϲ����� �����̸�.
	{
		int nPixelSize = 0;
		if(	D3DFMT_A1R5G5B5 == sd.Format ||
			D3DFMT_A4R4G4B4 == sd.Format) nPixelSize = 2;
		else if(D3DFMT_R8G8B8 == sd.Format) nPixelSize = 3;
		else if(D3DFMT_A8R8G8B8 == sd.Format || D3DFMT_X8R8G8B8 == sd.Format) nPixelSize = 4;
		else
		{
			__ASSERT(0, "this Texture Format Not Supported");
		}

		D3DLOCKED_RECT LR;
		for(int i = 0; i < nMMC; i++)
		{
			m_lpTexture->GetLevelDesc(i, &sd);
			m_lpTexture->LockRect(i, &LR, NULL, 0); // �� ���� Lock
			int nH = sd.Height;
			for(int y = 0; y < nH; y++) // �׳� �ȼ� ����..
				WriteFile(hFile, (uint8_t*)LR.pBits + y * LR.Pitch, sd.Width * nPixelSize, &dwRWC, NULL);
			m_lpTexture->UnlockRect(i);
		}

		if(nMMC == 1 && m_Header.nWidth >= 512) // �εθ� ���� 256 * 256 ¥�� �ϳ� �� �������ش�..
		{
			LPDIRECT3DSURFACE8 lpSurfSrc = NULL, lpSurfDest = NULL;

			m_lpTexture->GetLevelDesc(0, &sd);
			m_lpTexture->GetSurfaceLevel(0, &lpSurfSrc);
			int nW = 256, nH = 256;
			s_lpD3DDev->CreateImageSurface(nW, nH, sd.Format, &lpSurfDest);
			HRESULT rval = D3DXLoadSurfaceFromSurface(lpSurfDest, NULL, NULL, lpSurfSrc, NULL, NULL, D3DX_FILTER_TRIANGLE, 0); // ���ǽ� ����.
			lpSurfDest->LockRect(&LR, NULL, NULL);
			for(int y = 0; y < nH; y++)
			{
				WriteFile(hFile, (uint8_t*)LR.pBits + y * LR.Pitch, nW * 2, &dwRWC, NULL);
			}
			lpSurfDest->UnlockRect();
			lpSurfDest->Release(); lpSurfDest = NULL;
			lpSurfSrc->Release(); lpSurfSrc = NULL;
		}
	}

	return true;
	*/
}
#endif // end of _N3TOOL

#ifdef _N3TOOL
bool CN3Texture::Convert(D3DFORMAT Format, int nWidth, int nHeight, BOOL bGenerateMipMap)
{
	if (m_lpTexture == NULL) return false;

	D3DSURFACE_DESC dsd;
	m_lpTexture->GetLevelDesc(0, &dsd);
	if (0 >= nWidth || 0 >= nHeight)
	{
		nWidth = dsd.Width;
		nHeight = dsd.Height;
	}

	LPDIRECT3DTEXTURE9 lpTexOld = m_lpTexture;

	m_lpTexture = NULL;
	if (this->Create(nWidth, nHeight, Format, bGenerateMipMap) == false) return false;
	if (bGenerateMipMap)
	{
		LPDIRECT3DSURFACE9 lpTSOld;
		lpTexOld->GetSurfaceLevel(0, &lpTSOld);
		this->GenerateMipMap(lpTSOld); // MipMap ����
		lpTSOld->Release();
	}
	else
	{
		LPDIRECT3DSURFACE9 lpTSNew;
		LPDIRECT3DSURFACE9 lpTSOld;
		m_lpTexture->GetSurfaceLevel(0, &lpTSNew);
		lpTexOld->GetSurfaceLevel(0, &lpTSOld);
		D3DXLoadSurfaceFromSurface(lpTSNew, NULL, NULL, lpTSOld, NULL, NULL, D3DX_FILTER_NONE, 0); // ù���� ���� ���ǽ� ����.
		lpTSOld->Release();
		lpTSNew->Release();
	}

	lpTexOld->Release(); lpTexOld = NULL;

	return true;
}
#endif // end of _N3TOOL

#ifdef _N3TOOL
bool CN3Texture::GenerateMipMap(LPDIRECT3DSURFACE9 lpSurfSrc)
{
	if (m_lpTexture == NULL) return false;

	// MipMap �� � �ʿ����� ���..
	int nMMC = m_lpTexture->GetLevelCount();
	int nMMC2 = 0;
	for (int nW = m_Header.nWidth, nH = m_Header.nHeight; nW >= 4 && nH >= 4; nW /= 2, nH /= 2) nMMC2++;

	bool bNeedReleaseSurf = false;
	if (NULL == lpSurfSrc)
	{
		bNeedReleaseSurf = true;
		if (D3D_OK != m_lpTexture->GetSurfaceLevel(0, &lpSurfSrc)) return false;
	}

	HRESULT rval = D3D_OK;
	if (nMMC < nMMC2) // ������ ���� ����..
	{
		LPDIRECT3DTEXTURE9 lpTexOld = m_lpTexture;
		m_lpTexture = NULL;
		rval = this->CreateFromSurface(lpSurfSrc, m_Header.Format, TRUE);
		if (bNeedReleaseSurf) { lpSurfSrc->Release(); lpSurfSrc = NULL; }
		lpTexOld->Release(); lpTexOld = NULL;

		if (D3D_OK == rval)
		{
			m_Header.bMipMap = TRUE;
			return true;
		}
		else
		{
			m_Header.bMipMap = FALSE;
			return FALSE;
		}
	}
	else // MipMap �� ������ �׳� ǥ�鸸 ����
	{
		if (false == bNeedReleaseSurf) // �ٸ� ���ǽ����� �����ؾ� �Ǵ� �Ÿ� 0 ������ ����..
		{
			LPDIRECT3DSURFACE9 lpSurfDest;
			m_lpTexture->GetSurfaceLevel(0, &lpSurfDest);
			uint32_t dwFilter = D3DX_FILTER_TRIANGLE; // �⺻ ���ʹ� ����..
			HRESULT rval = D3DXLoadSurfaceFromSurface(lpSurfDest, NULL, NULL, lpSurfSrc, NULL, NULL, dwFilter, 0); // ���� �� ü�ο� ���ǽ� �̹��� ��� ���� 
			lpSurfDest->Release(); lpSurfDest = NULL;
		}

		for (int i = 1; i < nMMC2; i++)
		{
			LPDIRECT3DSURFACE9 lpSurfDest, lpSurfUp;
			m_lpTexture->GetSurfaceLevel(i - 1, &lpSurfUp);
			m_lpTexture->GetSurfaceLevel(i, &lpSurfDest);
			uint32_t dwFilter = D3DX_FILTER_TRIANGLE; // �⺻ ���ʹ� ����..
			HRESULT rval = D3DXLoadSurfaceFromSurface(lpSurfDest, NULL, NULL, lpSurfUp, NULL, NULL, dwFilter, 0); // ���� �� ü�ο� ���ǽ� �̹��� ��� ���� 
			lpSurfDest->Release();
			lpSurfUp->Release();
		}

		if (bNeedReleaseSurf) { lpSurfSrc->Release(); lpSurfSrc = NULL; }
		if (D3D_OK == rval)
		{
			m_Header.bMipMap = TRUE;
			return true;
		}
		else
		{
			m_Header.bMipMap = FALSE;
			return FALSE;
		}
	}
}
#endif // end of _N3TOOL

void CN3Texture::UpdateRenderInfo()
{
}

#ifdef _N3TOOL
bool CN3Texture::SaveToBitmapFile(const std::string& szFN)
{
	// TODO(srmeier): implement this
	return false;

	/*
	if(szFN.empty()) return false;
	if(NULL == m_lpTexture) return false;

	LPDIRECT3DSURFACE9 lpSurfSrc = NULL;
	m_lpTexture->GetSurfaceLevel(0, &lpSurfSrc);

	if(NULL == lpSurfSrc) return false;

	LPDIRECT3DSURFACE9 lpSurfDest = NULL;
	s_lpD3DDev->CreateImageSurface(m_Header.nWidth, m_Header.nHeight, D3DFMT_A8R8G8B8, &lpSurfDest);

	if(NULL == lpSurfDest) return false;
	if(D3D_OK != D3DXLoadSurfaceFromSurface(lpSurfDest, NULL, NULL, lpSurfSrc, NULL, NULL, D3DX_FILTER_TRIANGLE, 0)) // ���ǽ� ����.
	{
		lpSurfDest->Release(); lpSurfDest = NULL;
		lpSurfSrc->Release(); lpSurfSrc = NULL;
	}

	CBitMapFile bmpf;
	bmpf.Create(m_Header.nWidth, m_Header.nHeight);

	D3DLOCKED_RECT LR;
	lpSurfDest->LockRect(&LR, NULL, 0);
	for(int y = 0; y < m_Header.nHeight; y++)
	{
		uint8_t* pPixelsSrc = ((uint8_t*)LR.pBits) + y * LR.Pitch;
		uint8_t* pPixelsDest = (uint8_t*)(bmpf.Pixels(0, y));
		for(int x = 0; x < m_Header.nWidth; x++)
		{
			pPixelsDest[0] = pPixelsSrc[0];
			pPixelsDest[1] = pPixelsSrc[1];
			pPixelsDest[2] = pPixelsSrc[2];

			pPixelsSrc += 4;
			pPixelsDest += 3;
		}
	}
	lpSurfDest->UnlockRect();

	lpSurfDest->Release(); lpSurfDest = NULL;
	lpSurfSrc->Release(); lpSurfSrc = NULL;

	return bmpf.SaveToFile(szFN.c_str());
	*/
}
#endif // end of _N3TOOL