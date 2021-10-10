#include "stdafx.h"
#include "CollisionMgr.h"
#include "Obj.h"
#include "Terrain.h"
#include "AnimationImpl.h"
#include "NormalEffect.h"
#include "Dungeon.h"
#include "SkullDog.h"
#include "Boss.h"

CCollisionMgr::CCollisionMgr()
{
}


CCollisionMgr::~CCollisionMgr()
{
}

bool CCollisionMgr::CheckRect(CObj * pDst, TILE_INFO * pSour, float * pMoveX, float * pMoveY)
{

	float fRadiusSumX = (pSour->vLength.x * 0.5f) + (pDst->Get_Info().vLength.x *0.2f);
	float fRadiusSumY = (pSour->vLength.y * 0.5f) + (pDst->Get_Info().vLength.y * 0.5f);

	float fDistX = fabs(pSour->vPos.x - pDst->Get_Info().vPos.x);
	float fDistY = fabs(pSour->vPos.y - pDst->Get_Info().vPos.y);

	if (fRadiusSumX > fDistX && fRadiusSumY > fDistY)
	{
		*pMoveX = fRadiusSumX - fDistX;
		*pMoveY = fRadiusSumY - fDistY;

		return true;
	}

	return false;

}

void CCollisionMgr::CollisionCheck_Tile(OBJLIST & rDestList, vector<TILE_INFO*>* rSourList)
{
	bool Collise;
	float MoveX = 0.f;
	float MoveY = 0.f;
	D3DXVECTOR3 UnitPos;
	D3DXVECTOR3 Dir;
	float Length;
	RECT rc = {};
	for (auto& rDst : rDestList)
	{
		UnitPos = rDst->Get_Info().vPos;
		if ((*rSourList).empty() == true)
		{
			return;
		}

		for (auto pTile : (*rSourList))
		{
			Dir = {
				UnitPos.x - pTile->vPos.x,
				UnitPos.y - pTile->vPos.y,
				0.f };
			Length = D3DXVec3Length(&Dir);
			if (Length > 200.f)
			{
				continue;
			}
			Collise = CheckRect(rDst, pTile, &MoveX, &MoveY);
			if (Collise == true)
			{
				if (pTile->byOption == 1)
				{
					float x = rDst->Get_Info().vPos.x;
					float y = rDst->Get_Info().vPos.y;
					if (MoveX > MoveY)
					{
						if (y < pTile->vPos.y)
							MoveY *= -1.f;

						rDst->Set_Pos(x, y + MoveY);
						rDst->Set_CollisionRect(true);
					}
					else
					{
						if (x < pTile->vPos.x)
							MoveX *= -1.f;

						rDst->Set_Pos(x + MoveX, y);
						rDst->Set_CollisionRect(true);


					}

				}
				else if (pTile->byOption == FIELDID::DUNGEON1) // 2
				{

					float x = rDst->Get_Info().vPos.x;
					float y = rDst->Get_Info().vPos.y;
					if (MoveX > MoveY)
					{
						if (y < pTile->vPos.y)
							MoveY *= -1.f;

						rDst->Set_Pos(x, y + MoveY);
						rDst->Set_CollisionRect(true);
					}
					else
					{
						if (x < pTile->vPos.x)
							MoveX *= -1.f;

						rDst->Set_Pos(x + MoveX, y);
						rDst->Set_CollisionRect(true);
					}
					dynamic_cast<CDungeon*>(CSceneMgr::Get_Instance()->Get_CurScene())->Set_Field(FIELDID::DUNGEON1);
					return;

				}
				else if (pTile->byOption == FIELDID::DUNGEON2) // 3
				{

					float x = rDst->Get_Info().vPos.x;
					float y = rDst->Get_Info().vPos.y;
					if (MoveX > MoveY)
					{
						if (y < pTile->vPos.y)
							MoveY *= -1.f;

						rDst->Set_Pos(x, y + MoveY);
						rDst->Set_CollisionRect(true);
					}
					else
					{
						if (x < pTile->vPos.x)
							MoveX *= -1.f;

						rDst->Set_Pos(x + MoveX, y);
						rDst->Set_CollisionRect(true);
					}
					dynamic_cast<CDungeon*>(CSceneMgr::Get_Instance()->Get_CurScene())->Set_Field(FIELDID::DUNGEON2);
					return;

				}
				else if (pTile->byOption == FIELDID::WEAPON_SHOP) //4
				{

					float x = rDst->Get_Info().vPos.x;
					float y = rDst->Get_Info().vPos.y;
					if (MoveX > MoveY)
					{
						if (y < pTile->vPos.y)
							MoveY *= -1.f;

						rDst->Set_Pos(x, y + MoveY);
						rDst->Set_CollisionRect(true);
					}
					else
					{
						if (x < pTile->vPos.x)
							MoveX *= -1.f;

						rDst->Set_Pos(x + MoveX, y);
						rDst->Set_CollisionRect(true);
					}
					dynamic_cast<CDungeon*>(CSceneMgr::Get_Instance()->Get_CurScene())->Set_Field(FIELDID::WEAPON_SHOP);
					return;

				}
				else if (pTile->byOption == FIELDID::DUNGEON3) //5
				{

					float x = rDst->Get_Info().vPos.x;
					float y = rDst->Get_Info().vPos.y;
					if (MoveX > MoveY)
					{
						if (y < pTile->vPos.y)
							MoveY *= -1.f;

						rDst->Set_Pos(x, y + MoveY);
						rDst->Set_CollisionRect(true);
					}
					else
					{
						if (x < pTile->vPos.x)
							MoveX *= -1.f;

						rDst->Set_Pos(x + MoveX, y);
						rDst->Set_CollisionRect(true);
					}
					dynamic_cast<CDungeon*>(CSceneMgr::Get_Instance()->Get_CurScene())->Set_Field(FIELDID::DUNGEON3);
					return;

				}
				else if (pTile->byOption == FIELDID::RESTAURANT) //6
				{

					float x = rDst->Get_Info().vPos.x;
					float y = rDst->Get_Info().vPos.y;
					if (MoveX > MoveY)
					{
						if (y < pTile->vPos.y)
							MoveY *= -1.f;

						rDst->Set_Pos(x, y + MoveY);
						rDst->Set_CollisionRect(true);
					}
					else
					{
						if (x < pTile->vPos.x)
							MoveX *= -1.f;

						rDst->Set_Pos(x + MoveX, y);
						rDst->Set_CollisionRect(true);
					}
					dynamic_cast<CDungeon*>(CSceneMgr::Get_Instance()->Get_CurScene())->Set_Field(FIELDID::RESTAURANT);
					return;

				}
				else if (pTile->byOption == FIELDID::DUNGEON4)//7
				{

					float x = rDst->Get_Info().vPos.x;
					float y = rDst->Get_Info().vPos.y;
					if (MoveX > MoveY)
					{
						if (y < pTile->vPos.y)
							MoveY *= -1.f;

						rDst->Set_Pos(x, y + MoveY);
						rDst->Set_CollisionRect(true);
					}
					else
					{
						if (x < pTile->vPos.x)
							MoveX *= -1.f;

						rDst->Set_Pos(x + MoveX, y);
						rDst->Set_CollisionRect(true);
					}
					dynamic_cast<CDungeon*>(CSceneMgr::Get_Instance()->Get_CurScene())->Set_Field(FIELDID::DUNGEON4);
					return;

				}
				else if (pTile->byOption == FIELDID::REINFORCE)// 8
				{

					float x = rDst->Get_Info().vPos.x;
					float y = rDst->Get_Info().vPos.y;
					if (MoveX > MoveY)
					{
						if (y < pTile->vPos.y)
							MoveY *= -1.f;

						rDst->Set_Pos(x, y + MoveY);
						rDst->Set_CollisionRect(true);
					}
					else
					{
						if (x < pTile->vPos.x)
							MoveX *= -1.f;

						rDst->Set_Pos(x + MoveX, y);
						rDst->Set_CollisionRect(true);
					}
					dynamic_cast<CDungeon*>(CSceneMgr::Get_Instance()->Get_CurScene())->Set_Field(FIELDID::REINFORCE);
					return;

				}
				else if (pTile->byOption == FIELDID::BOSSMAP) // 9
				{

					float x = rDst->Get_Info().vPos.x;
					float y = rDst->Get_Info().vPos.y;
					if (MoveX > MoveY)
					{
						if (y < pTile->vPos.y)
							MoveY *= -1.f;

						rDst->Set_Pos(x, y + MoveY);
						rDst->Set_CollisionRect(true);
					}
					else
					{
						if (x < pTile->vPos.x)
							MoveX *= -1.f;

						rDst->Set_Pos(x + MoveX, y);
						rDst->Set_CollisionRect(true);
					}
					dynamic_cast<CDungeon*>(CSceneMgr::Get_Instance()->Get_CurScene())->Set_Field(FIELDID::BOSSMAP);
					return;

				}

			}

		}
	}


}

void CCollisionMgr::CollisionMonsterCheck_Tile(OBJLIST & rDestList, vector<TILE_INFO*>* rSourList)
{
	bool Collise;
	float MoveX = 0.f;
	float MoveY = 0.f;
	D3DXVECTOR3 UnitPos;
	D3DXVECTOR3 Dir;
	float Length;
	RECT rc = {};
	for (auto& rDst : rDestList)
	{
		UnitPos = rDst->Get_Info().vPos;
		if ((*rSourList).empty() == true)
		{
			return;
		}

		for (auto pTile : (*rSourList))
		{
			Dir = {
				UnitPos.x - pTile->vPos.x,
				UnitPos.y - pTile->vPos.y,
				0.f };
			Length = D3DXVec3Length(&Dir);
			if (Length > 200.f)
			{
				continue;
			}
			Collise = CheckRect(rDst, pTile, &MoveX, &MoveY);
			if (Collise == true)
			{

					float x = rDst->Get_Info().vPos.x;
					float y = rDst->Get_Info().vPos.y;
					if (MoveX > MoveY)
					{
						if (y < pTile->vPos.y)
							MoveY *= -1.f;

						rDst->Set_Pos(x, y + MoveY);
						rDst->Set_CollisionRect(true);
					}
					else
					{
						if (x < pTile->vPos.x)
							MoveX *= -1.f;

						rDst->Set_Pos(x + MoveX, y);
						rDst->Set_CollisionRect(true);


					}

				
			}
		}
	}
}

void CCollisionMgr::CollisionBulletCheck_Tile(OBJLIST & rDestList, vector<TILE_INFO*>* rSourList)
{
	bool Collise;
	float MoveX = 0.f;
	float MoveY = 0.f;
	D3DXVECTOR3 UnitPos;
	D3DXVECTOR3 Dir;
	float Length;
	RECT rc = {};
	for (auto& rDst : rDestList)
	{
		UnitPos = rDst->Get_Info().vPos;
		if ((*rSourList).empty() == true)
		{
			return;
		}

		for (auto pTile : (*rSourList))
		{
			Dir = {
				UnitPos.x - pTile->vPos.x,
				UnitPos.y - pTile->vPos.y,
				0.f };
			Length = D3DXVec3Length(&Dir);
			if (Length > 40.f)
			{
				continue;
			}
			Collise = CheckRect(rDst, pTile, &MoveX, &MoveY);
			if (Collise == true)
			{
				if (pTile->byOption == 1)
				{
					rDst->RectColl();
				}
			}
		}
	}
}

void CCollisionMgr::CollisionRect(OBJLIST & rDestList, OBJLIST & rSourList)
{
	for (auto& rDst : rDestList)
	{
		for (auto& rSrc : rSourList)
		{

			RECT rc = {};
			if (IntersectRect(&rc, &rDst->Get_Rect(), &rSrc->Get_Rect()))
			{
				rSrc->RectColl();
			}
		}
	}
}

void CCollisionMgr::CollisionRectangle(OBJLIST & rDestList, OBJLIST & rSourList)
{
	RECT rc = {};
	for (auto& rDst : rDestList)
	{
		for (auto& rSrc : rSourList)
		{
			if (IntersectRect(&rc, &rDst->Get_Rect(), &rSrc->Get_Rect()))
			{

				rSrc->RectColl();
			}
		}
	}
}

void CCollisionMgr::CollisionRectSkullDog(OBJLIST & rDestList, OBJLIST & rSourList)
{// rDst player rSour Skulldog
	int CurState = 0;
	RECT rc = {};
	for (auto& rDst : rDestList)
	{
		for (auto& rSrc : rSourList)
		{

			
			if (IntersectRect(&rc, &rDst->Get_Rect(), &rSrc->Get_Rect()))
			{
				CurState = dynamic_cast<CSkullDog*>(rSrc)->Get_eCurState();
				if (CurState == 5) // JUMP이다
				{
					rDst->RectColl(rSrc);
				}

			}
		}
	}
}

void CCollisionMgr::CollisionRectMeleeAttack(OBJLIST & rDestList, OBJLIST & rSourList)
{
	RECT rc = {};
	for (auto& rDst : rDestList)
	{

		for (auto& rSrc : rSourList)
		{

			if (IntersectRect(&rc, &rDst->Get_Rect(), &rSrc->Get_Rect()))
			{

				rSrc->RectColl(rDst);

			}
		}
	}
}

void CCollisionMgr::CollisionRectBullet(OBJLIST & rDestList, OBJLIST & rSourList)
{
	D3DXVECTOR3 UnitPos;
	D3DXVECTOR3 Dir;
	RECT rc = {};
	float Length;
	for (auto& rDst : rDestList)
	{
		UnitPos = rDst->Get_Info().vPos;
		for (auto& rSrc : rSourList)
		{
			Dir = {
				UnitPos.x - rSrc->Get_Info().vPos.x,
				UnitPos.y - rSrc->Get_Info().vPos.y,
				0.f };
			Length = D3DXVec3Length(&Dir);
			if (Length > 40.f)
			{
				continue;
			}

			if (IntersectRect(&rc, &rDst->Get_Rect(), &rSrc->Get_Rect()))
			{
				rDst->RectColl(rSrc);
				rSrc->RectColl(rDst);

			}
		}
	}
}

void CCollisionMgr::CollisionRectBoss(OBJLIST & rDestList, OBJLIST & rSourList)
{
	RECT rc = {};
	int CurState = 0;

	for (auto& rDst : rDestList)
	{

		for (auto& rSrc : rSourList)
		{

			if (IntersectRect(&rc, &rDst->Get_Rect(), &rSrc->Get_Rect()))
			{
				CurState = dynamic_cast<CBoss*>(rSrc)->Get_eCurState();
				if (CurState == 6) // JUMP이다
				{
					rSrc->RectColl(rDst);
					rDst->RectColl();
				}
			}
		}
	}
}

void CCollisionMgr::CollisionRevolver(OBJLIST & rDestList, OBJLIST & rSourList)
{
	RECT rc = {};
	for (auto& rDst : rDestList)
	{

		for (auto& rSrc : rSourList)
		{

			if (IntersectRect(&rc, &rDst->Get_Rect(), &rSrc->Get_Rect()))
			{

				rSrc->RectColl(rDst);
				rDst->RectColl();
			}
		}
	}
}

