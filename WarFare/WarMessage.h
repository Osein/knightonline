#pragma once

class CDFont;

class CWarMessage  
{
private:
	float	m_fTime;
	CDFont*	m_pMessageFont;
	POINT	m_ptMessage;
public:
	void Tick();
	void	RenderMessage();
	void	SetMessage(const std::string& szText, uint32_t dwFlags = 0x0001,uint32_t dwColor = 0xffffffff);
	void	Release();
	void	InitFont();
	CWarMessage();
	virtual ~CWarMessage();

};
