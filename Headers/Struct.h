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
	// IDirect3DTexture9: 이미지 한장을 제어하는 Com객체
	LPDIRECT3DTEXTURE9	pTexture;

	// 이미지 한장의 정보.
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
	D3DXVECTOR3 vPos;	// 위치벡터
	D3DXVECTOR3 vDir;	// 방향벡터
	D3DXVECTOR3 vLook;	// 기준벡터 ??
	D3DXVECTOR3 vSize;	// 크기 벡터
	D3DXVECTOR3 vLength; // 길이 벡터
	D3DXVECTOR3 vCollPos; // 충돌체크할 위치
	D3DXVECTOR3 vCollLength; // 충돌체크할 길이
	D3DXMATRIX	matScale;// 스케일
	D3DXMATRIX	matRotZ;// 자전
	D3DXMATRIX	matTrans;// 이동
	D3DXMATRIX matPlanetZ; // 공전
	D3DXMATRIX matParent; // 부모
	D3DXMATRIX	matWorld;// 월드
	
	UNIT_TEX_INFO Texture;
	
}UNIT_INFO;


namespace ITEMTYPE
{
	enum TYPE
	{
		ATTACKWEAPON, // 주무기
		SUBWEAPON, // 보조무기 (공격력만 올려줄거임)
		ARTIFACT, // 특수장비
		END
	};
}

namespace ITEMINTER // 아이템 사거리
{
	enum TYPE
	{
		NONE,
		MELEE, // 근접
		LONG, // 원거리공격
		END
	};
}

typedef struct tagItemData
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vSize;
	D3DXVECTOR3 vLength;
	D3DXMATRIX	matScale;// 스케일
	D3DXMATRIX	matTrans;// 이동
	D3DXMATRIX	matWorld;// 월드
	UNIT_TEX_INFO Texture;

	float iAtt;// 공격력
	int iPlusAtt;
	int iBlock;
	int Regen;
	int iGold;// 돈
	wstring szName; // 아이템 이름
	bool eatItem;
	bool Griped;
	ITEMTYPE::TYPE eType;
	ITEMINTER::TYPE eLength; // 사거리
}ITEM_DATA;

typedef struct tagUnitSave
{

	wstring strName;
	wstring ObjKey;
	wstring StateKey;

	D3DXVECTOR3 vPos; // 위치 벡터
	D3DXVECTOR3 vSize; // 크기 벡터
	int iDrawiD;
	int iOption; // 몬스터의 옵션 (1. 기본 생성 2. 플레이어 다가갈 시 생성)
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


// API때 곧 버릴거임
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
	D3DXVECTOR3 vPos;	// 위치벡터
	D3DXVECTOR3 vDir;	// 방향벡터
	D3DXVECTOR3 vLook;	// 기준벡터 ??
	D3DXVECTOR3 vSize;	// 크기 벡터
	D3DXVECTOR3 vLength; // 길이 벡터
	
	D3DXMATRIX	matScale;// 스케일
	D3DXMATRIX	matRotZ;// 자전
	D3DXMATRIX	matTrans;// 이동
	D3DXMATRIX matPlanetZ; // 공전
	D3DXMATRIX matParent; // 부모
	D3DXMATRIX	matWorld;// 월드

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