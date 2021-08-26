#include "pch.h"
#include "N3UITooltip.h"
#include "N3UIString.h"
#include "N3UIStatic.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CN3UITooltip::CN3UITooltip()
{
	m_eType = UI_TYPE_TOOLTIP;

	m_fHoverTime = 0.0f;
	m_bVisible = false;
	m_bSetText = false;
	ZeroMemory(&m_ptCursor, sizeof(m_ptCursor));
}

CN3UITooltip::~CN3UITooltip()
{
}

void CN3UITooltip::Release()
{
	CN3UIBase::Release();
	m_fHoverTime = 0.0f;
	m_bVisible = false;
	m_bSetText = false;
	ZeroMemory(&m_ptCursor, sizeof(m_ptCursor));
}

void CN3UITooltip::Render()
{
	if(!m_bVisible || !m_bSetText) return;
	if (NULL == m_pImageBkGnd)
	{	// �̹����� ������ ����Ʈ�� �׷�����
		static __VertexTransformedColor	pVB[8];
		static const uint16_t	pIB[16]= {0,1,1,2,2,3,3,0,4,5,5,6,6,7,7,4};
		static const D3DCOLOR BkColor= 0x80000000;
		static const D3DCOLOR BorderColorOut= 0xff808080;
		static const D3DCOLOR BorderColorIn= 0xffc0c0c0;
		pVB[0].Set((float)m_rcRegion.left,		(float)m_rcRegion.top,		UI_DEFAULT_Z, UI_DEFAULT_RHW, BkColor);
		pVB[1].Set((float)m_rcRegion.right,		(float)m_rcRegion.top,		UI_DEFAULT_Z, UI_DEFAULT_RHW, BkColor);
		pVB[2].Set((float)m_rcRegion.right,		(float)m_rcRegion.bottom,	UI_DEFAULT_Z, UI_DEFAULT_RHW, BkColor);
		pVB[3].Set((float)m_rcRegion.left,		(float)m_rcRegion.bottom,	UI_DEFAULT_Z, UI_DEFAULT_RHW, BkColor);
		pVB[4].Set((float)m_rcRegion.left+1,	(float)m_rcRegion.top+1,	UI_DEFAULT_Z, UI_DEFAULT_RHW, BorderColorIn);
		pVB[5].Set((float)m_rcRegion.right-1,	(float)m_rcRegion.top+1,	UI_DEFAULT_Z, UI_DEFAULT_RHW, BorderColorIn);
		pVB[6].Set((float)m_rcRegion.right-1,	(float)m_rcRegion.bottom-1,UI_DEFAULT_Z, UI_DEFAULT_RHW, BorderColorIn);
		pVB[7].Set((float)m_rcRegion.left+1,	(float)m_rcRegion.bottom-1,UI_DEFAULT_Z, UI_DEFAULT_RHW, BorderColorIn);

		// set texture stage state
		s_lpD3DDev->SetTexture( 0, NULL);
		s_lpD3DDev->SetTextureStageState( 0, D3DTSS_COLOROP,    D3DTOP_SELECTARG1 );
		s_lpD3DDev->SetTextureStageState( 0, D3DTSS_COLORARG1,  D3DTA_DIFFUSE );

		// draw
		s_lpD3DDev->SetFVF(FVF_TRANSFORMEDCOLOR);
		HRESULT hr = s_lpD3DDev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, pVB, sizeof(__VertexTransformedColor));	// ���� ĥ�ϱ�

		__VertexTransformedColor* pTemp = pVB;
		int i;
		for (i=0; i<4; ++i) pTemp++->color = BorderColorOut;	// �ٱ� �׵θ� ���� �ٲ۴�.
		s_lpD3DDev->DrawIndexedPrimitiveUP(D3DPT_LINELIST, 0, 8, 8, 
			pIB, D3DFMT_INDEX16, pVB, sizeof(__VertexTransformedColor));	// �׵θ� ĥ�ϱ�

		// �۾� �׸���
		m_pBuffOutRef->Render();
	}
	else CN3UIStatic::Render();
}

void CN3UITooltip::SetText(const std::string& szText)
{
	if(!m_bVisible || m_bSetText) return;

	// �������� ũ�⸦ �����Ѵ�.
	int iStrLen = szText.size();
	if (0 == iStrLen || NULL == m_pBuffOutRef) return;

	m_pBuffOutRef->ClearOnlyStringBuffer();	//�켱 ������ �ִ� �۾��� �����.(�������� �ʰ�)
	SIZE size;
	if (m_pBuffOutRef->GetTextExtent(szText, iStrLen, &size))
	{
		if (std::string::npos != szText.find('\n'))
		{	// '\n'���ڿ��߿� \n�� �� �����Ƿ� multi line���� ����
			m_pBuffOutRef->SetStyle(UISTYLE_STRING_ALIGNLEFT|UISTYLE_STRING_ALIGNTOP);
		}
		else if (iStrLen<25)
		{	// 25byte �̸��̸� �׳� ���ٷ�.
			m_pBuffOutRef->SetStyle(UISTYLE_STRING_SINGLELINE|UISTYLE_STRING_ALIGNCENTER|UISTYLE_STRING_ALIGNVCENTER);
		}
		else
		{	// single line�̹Ƿ� ������ ũ�⸦ ����Ѵ�.
			SIZE CharSize = {0,0};
			if (FALSE == m_pBuffOutRef->GetTextExtent("��", 2, &CharSize)) return;

			int iDiv = 25;
			int iLineCount = (iStrLen + iDiv-1)/iDiv;
			while (iLineCount > (iDiv/2))
			{
				iDiv += 5;
				iLineCount = (iStrLen + iDiv-1)/iDiv;
			}
			size.cx = ((iDiv+1)*CharSize.cx + 1)/2;
			size.cy = iLineCount*CharSize.cy;
		}

		size.cx += 12;
		size.cy += 12;
		SetSize(size.cx, size.cy);
	}
	m_pBuffOutRef->SetString(szText);

	// ��ġ ����
	POINT	ptNew = m_ptCursor;
	ptNew.x -= (m_rcRegion.right - m_rcRegion.left) / 2;
	ptNew.y -= (m_rcRegion.bottom - m_rcRegion.top) + 10;

	D3DVIEWPORT9 &vp = s_CameraData.vp;
	int iRegionWidth = m_rcRegion.right - m_rcRegion.left;
	int iRegionHeight = m_rcRegion.bottom - m_rcRegion.top;

	if (ptNew.x + iRegionWidth > (int)(vp.X+vp.Width))	// ȭ�� �����U���� ��� ���
		ptNew.x = vp.X+vp.Width - iRegionWidth;
	if (ptNew.x < (int)vp.X)	ptNew.x = vp.X;	// ȭ�� �������� ��� ���

	if (ptNew.y + iRegionHeight >(int)(vp.Y+vp.Height))	// ȭ�� �ϴ����� ��� ���
		ptNew.y = vp.Y+vp.Height - iRegionHeight;
	if (ptNew.y < (int)vp.Y) ptNew.y = vp.Y;	// ȭ�� ������� ��� ���	

	SetPos(ptNew.x, ptNew.y);

	m_bSetText = true;
}

void CN3UITooltip::Tick()
{
	float fOldTime = m_fHoverTime;
	m_fHoverTime += s_fSecPerFrm;
	static const float fDisplayTime = 0.3f;
	if (fOldTime < fDisplayTime && m_fHoverTime >= fDisplayTime)
	{
		SetVisible(true);	// tool tip ǥ��
	}
}

uint32_t CN3UITooltip::MouseProc(uint32_t dwFlags, const POINT& ptCur, const POINT& ptOld)
{
	uint32_t dwRet = UI_MOUSEPROC_NONE;
	if (!m_bVisible) return dwRet;

	// ���콺�� �����̸� m_fHoverTime�� 0���� �����
	if (ptCur.x != ptOld.x || ptCur.y != ptOld.y)
	{
		m_fHoverTime = 0.0f;
		m_bSetText = false;
		SetVisible(false);// tool tip�� ���ش�.
	}
	else
	{	// �ȿ����̸� Ŀ�� ��ġ ����
		m_ptCursor = ptCur;
	}

	dwRet |= CN3UIBase::MouseProc(dwFlags, ptCur, ptOld);
	return dwRet;
}
