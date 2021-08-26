#pragma once

#include "N3Base.h"

#define N3FORMAT_VER_UNKN 0x00000000
#define N3FORMAT_VER_1068 0x00000001 // (1<<0)
#define N3FORMAT_VER_1264 0x00000002 // (1<<1)
#define N3FORMAT_VER_1298 0x00000004 // (1<<2)
#define N3FORMAT_VER_2062 0x00000008 // (1<<3)
#define N3FORMAT_VER_CURR 0x40000000 // NOTE: not even going to attempting this right now
#define N3FORMAT_VER_HERO 0x80000000 // NOTE: Hero Online formatting

static const int N3FORMAT_VER_DEFAULT = N3FORMAT_VER_1264;//N3FORMAT_VER_1068;

class CN3BaseFileAccess : public CN3Base
{
protected:
	std::string		m_szFileName; // Base Path �� ������ ���� ��� + ���� �̸�

public:
	uint32_t m_iFileFormatVersion;
	int m_iLOD; // �ε��Ҷ� �� LOD

public:
	const std::string& FileName() const { return m_szFileName; } // Full Path
	void FileNameSet(const std::string& szFileName);

	bool LoadFromFile(); // ���Ͽ��� �о����.
	virtual bool LoadFromFile(const std::string& szFileName, uint32_t iVer = N3FORMAT_VER_DEFAULT); // ���Ͽ��� �о����.
	virtual bool Load(HANDLE hFile); // �ڵ鿡�� �о����..

	virtual bool SaveToFile(); // ���� ���� �̸���� ����.
	virtual bool SaveToFile(const std::string& szFileName); // ���̸����� ����.
	virtual bool Save(HANDLE hFile); // �ڵ��� ���� ����..

public:
	void Release();

	CN3BaseFileAccess();
	virtual ~CN3BaseFileAccess();
};