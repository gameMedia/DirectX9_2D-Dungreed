#pragma once

typedef struct tagPathInfo
{
	wstring wstrObjectKey = L"";
	wstring wstrStateKey = L"";
	wstring wstrRelativePath = L"";
	int		iImgCnt = 0;
}PATH_INFO;


typedef struct tagFrame
{
	tagFrame() {}
	tagFrame(float _fCurFrame, float _fFrameCnt)
		: fCurFrame(_fCurFrame), fFrameCnt(_fFrameCnt)
	{

	}


	float fCurFrame;
	float fFrameCnt;
}FRAME;

typedef struct tagTile
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vSize;
	D3DXVECTOR3 vLength;
	D3DXMATRIX	matWorld;
	BYTE		byDrawID;
	BYTE		byOption;
	int			TILEX1;
	int			TILEY1;
	float		TILECX;
	float		TILECY;
}TILE_INFO;

typedef struct tapMap
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vSize;
	D3DXMATRIX	matScale;
	D3DXMATRIX  matTrans;
	D3DXMATRIX  matWorld;
	BYTE		byDrawID;
	float		MAP_SIZEX;
	float		MAP_SIZEY;
}MAP_INFO;

typedef struct tagTexture
{
	// IDirect3DTexture9: �̹��� ������ �����ϴ� Com��ü
	LPDIRECT3DTEXTURE9	pTexture;

	// �̹��� ������ ����.
	D3DXIMAGE_INFO	tImgInfo;
}TEX_INFO;
typedef struct tagUnitTexture
{
	wstring wstrObjectKey;
	wstring wstrStateKey;
	int iIndex;
}UNIT_TEX_INFO;

typedef struct tagUnit
{
	D3DXVECTOR3 vPos;	// ��ġ����
	D3DXVECTOR3 vDir;	// ���⺤��
	D3DXVECTOR3 vLook;	// ���غ��� ??
	D3DXVECTOR3 vSize;	// ũ�� ����
	D3DXVECTOR3 vLength; // ���� ����
	D3DXVECTOR3 vCollPos; // �浹üũ�� ��ġ
	D3DXVECTOR3 vCollLength; // �浹üũ�� ����
	D3DXMATRIX	matScale;// ������
	D3DXMATRIX	matRotZ;// ����
	D3DXMATRIX	matTrans;// �̵�
	D3DXMATRIX matPlanetZ; // ����
	D3DXMATRIX matParent; // �θ�
	D3DXMATRIX	matWorld;// ����
	
	UNIT_TEX_INFO Texture;
	
}UNIT_INFO;


namespace ITEMTYPE
{
	enum TYPE
	{
		ATTACKWEAPON, // �ֹ���
		SUBWEAPON, // �������� (���ݷ¸� �÷��ٰ���)
		ARTIFACT, // Ư�����
		END
	};
}

namespace ITEMINTER // ������ ��Ÿ�
{
	enum TYPE
	{
		NONE,
		MELEE, // ����
		LONG, // ���Ÿ�����
		END
	};
}

typedef struct tagItemData
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vSize;
	D3DXVECTOR3 vLength;
	D3DXMATRIX	matScale;// ������
	D3DXMATRIX	matTrans;// �̵�
	D3DXMATRIX	matWorld;// ����
	UNIT_TEX_INFO Texture;

	float iAtt;// ���ݷ�
	int iPlusAtt;
	int iBlock;
	int Regen;
	int iGold;// ��
	wstring szName; // ������ �̸�
	bool eatItem;
	bool Griped;
	ITEMTYPE::TYPE eType;
	ITEMINTER::TYPE eLength; // ��Ÿ�
}ITEM_DATA;

typedef struct tagUnitSave
{

	wstring strName;
	wstring ObjKey;
	wstring StateKey;

	D3DXVECTOR3 vPos; // ��ġ ����
	D3DXVECTOR3 vSize; // ũ�� ����
	int iDrawiD;
	int iOption; // ������ �ɼ� (1. �⺻ ���� 2. �÷��̾� �ٰ��� �� ����)
	int X;
	int Y;
}UNIT_SAVE;




typedef struct tagUnitTool
{
#ifdef TOOL
	CString strName;
#else
	wstring strName;
#endif
	int		iAttack;
	int		iDefence;
	BYTE	byJobIndex;
	BYTE	byItem = 0x00;
}UNIT_DATA;


// API�� �� ��������
typedef struct tagInfo
{
	int iLevel;
	int iHp;
	float iMaxHp;
	int iAtt;
	int iGold;
	int iBlock;
	int iPlusHp;
}INFO;

typedef struct tagInvenInfo
{
	D3DXVECTOR3 vPos;	// ��ġ����
	D3DXVECTOR3 vDir;	// ���⺤��
	D3DXVECTOR3 vLook;	// ���غ��� ??
	D3DXVECTOR3 vSize;	// ũ�� ����
	D3DXVECTOR3 vLength; // ���� ����
	
	D3DXMATRIX	matScale;// ������
	D3DXMATRIX	matRotZ;// ����
	D3DXMATRIX	matTrans;// �̵�
	D3DXMATRIX matPlanetZ; // ����
	D3DXMATRIX matParent; // �θ�
	D3DXMATRIX	matWorld;// ����

	UNIT_TEX_INFO Texture;

	bool HoldingItem;

	ITEM_DATA ItemData;
}INVENINFO;

typedef struct DrawInfo
{
	float fX;
	float fY;
	float fCX;
	float fCY;
}DRAWINFO;


//typedef struct tileInfo
//{
//	float fX;
//	float fY;
//	float fCX;
//	float fCY;
//}TILEINFO;

typedef struct tagLinePos
{
	tagLinePos()
	{
		ZeroMemory(this, sizeof(tagLinePos));
	}
	tagLinePos(float _x, float _y)
	{
		Linepos.x = _x;
		Linepos.y = _y;
	}
	D3DXVECTOR2 Linepos;
}LINEPOS;

typedef struct tagLine
{
	tagLine()
	{
		ZeroMemory(this, sizeof(tagLine));
	}
	tagLine(LINEPOS& Left, LINEPOS& Right,
		int& _Alpha,int& _Red,
		int& _Green,int& _Blue,
		int& _iOption)
	{
		LPoint = Left;
		RPoint = Right;
		Alpha = _Alpha;
		Red = _Red;
		Green = _Green;
		Blue = _Blue;
		iOption = _iOption;
	}
	LINEPOS LPoint;
	LINEPOS RPoint;
	int Alpha;
	int Red;
	int Green;
	int Blue;
	int iOption;
}LINEINFO;