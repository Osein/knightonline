#pragma once

#include "N3Base.h"

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
