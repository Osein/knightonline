#pragma once

#include <string>
#include <vector>
#include "N3UIBase.h"

class CDFont;

class CN3UIString : public CN3UIBase  
{
	friend class CN3UIEdit;

public:
	CN3UIString();
	virtual ~CN3UIString();

protected:
	CDFont*			m_pDFont;
	POINT			m_ptDrawPos;
	std::string 	m_szString;
	D3DCOLOR		m_Color;
	int				m_iLineCount;
	std::vector<int>	m_NewLineIndices;
	int				m_iStartLine;

// Attributes
public:
	void				SetColor(D3DCOLOR color) { m_Color = color; }
	D3DCOLOR			GetColor() const { return m_Color; }
	const std::string&	GetString() { return m_szString; }
	int					GetLineCount() const {return m_iLineCount;}
	int					GetStartLine() const {return m_iStartLine;}
	int					GetStringRealWidth(int iNum);
	int					GetStringRealWidth(std::string& szText);

	virtual	uint32_t	MouseProc(uint32_t dwFlags, const POINT& ptCur, const POINT& ptOld);
	virtual void	Render();
	virtual void	Release();	
	virtual void	Init(CN3UIBase* pParent);
	virtual BOOL	MoveOffset(int iOffsetX, int iOffsetY);// 글씨찍는 위치도 바뀌어 준다.
	virtual bool	Load(HANDLE hFile);
	void			ClearOnlyStringBuffer() { m_szString = ""; }	// string 버퍼만 지운다.
	void			SetStartLine(int iLine);	// multiline일경우 시작하는 라인 변경하기

	virtual void	operator = (const CN3UIString& other);

	virtual void	SetRegion(const RECT& Rect);
	virtual void	SetStyle(uint32_t dwStyle);
	virtual void	SetStyle(uint32_t dwType, uint32_t dwStyleEx);

	virtual void	SetString(const std::string& szString);
	virtual void	SetStringAsInt(int iVal);
	void			SetString_NoWordWrap(const std::string& szString);	// 글자 정렬 하지 않는다.
	virtual void	SetFont(const std::string& szFontName, uint32_t dwHeight, BOOL bBold, BOOL bItalic); // dwHeight는 point size이다.
	BOOL			GetTextExtent(const std::string& szString, int iStrLen, SIZE* pSize);
	uint32_t		GetFontColor() const;
	const std::string& GetFontName() const;
	uint32_t		GetFontHeight() const;
	uint32_t		GetFontFlags() const;
protected:
	void			WordWrap();		// wordwrap
};
