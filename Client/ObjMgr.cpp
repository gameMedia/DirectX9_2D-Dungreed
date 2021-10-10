#include "stdafx.h"
#include "ObjMgr.h"
#include "Obj.h"
#include "Terrain.h"
CObjMgr* CObjMgr::m_pInstance = nullptr;

CObjMgr::CObjMgr()
{
}


CObjMgr::~CObjMgr()
{
	Release();
}



void CObjMgr::Initialize()
{
}

int CObjMgr::Update()
{
	for (int i = 0; i < OBJID::END; ++i)
	{
		for (auto& iter = m_listObj[i].begin(); iter != m_listObj[i].end();)
		{
			int iEvent = (*iter)->Update();
			if (OBJ_DEAD == iEvent)
			{
				if (*iter)
				{
					delete *iter;
					*iter = nullptr;
				}

				iter = m_listObj[i].erase(iter);
			}
			else
				++iter;
		}
	}
	return 0;
	
}

void CObjMgr::LateUpdate()
{
	for (int i = 0; i < OBJID::END; ++i)
	{
		for (auto& pObj : m_listObj[i])
		{
			pObj->LateUpdate();
		}
	}
	// 타일과 플레이어
	CCollisionMgr::CollisionCheck_Tile(m_listObj[OBJID::PLAYER], m_vecOptionTile);
	// 타일과 몬스터
	CCollisionMgr::CollisionMonsterCheck_Tile(m_listObj[OBJID::SKULLDOG], m_vecOptionTile);
	CCollisionMgr::CollisionMonsterCheck_Tile(m_listObj[OBJID::BANSH], m_vecOptionTile);
	CCollisionMgr::CollisionMonsterCheck_Tile(m_listObj[OBJID::BAT], m_vecOptionTile);
	// 타일과 총알
	CCollisionMgr::CollisionBulletCheck_Tile(m_listObj[OBJID::BANSHBULLET], m_vecOptionTile);
	CCollisionMgr::CollisionBulletCheck_Tile(m_listObj[OBJID::BATBULLET], m_vecOptionTile);
	CCollisionMgr::CollisionBulletCheck_Tile(m_listObj[OBJID::ICEBULLET], m_vecOptionTile);
	CCollisionMgr::CollisionBulletCheck_Tile(m_listObj[OBJID::REVOLVERBULLET], m_vecOptionTile);
	CCollisionMgr::CollisionBulletCheck_Tile(m_listObj[OBJID::BATITEMBULLET], m_vecOptionTile);

	CCollisionMgr::CollisionRect(m_listObj[OBJID::PLAYER], m_listObj[OBJID::NORMALSWORD]);
	CCollisionMgr::CollisionRect(m_listObj[OBJID::PLAYER], m_listObj[OBJID::FIRESWORD]);
	CCollisionMgr::CollisionRect(m_listObj[OBJID::PLAYER], m_listObj[OBJID::REGENRING]);
	CCollisionMgr::CollisionRect(m_listObj[OBJID::PLAYER], m_listObj[OBJID::MAGICSHIELD]);
	CCollisionMgr::CollisionRect(m_listObj[OBJID::PLAYER], m_listObj[OBJID::BOMERANG]);
	CCollisionMgr::CollisionRect(m_listObj[OBJID::PLAYER], m_listObj[OBJID::BATITEM]);
	CCollisionMgr::CollisionRect(m_listObj[OBJID::PLAYER], m_listObj[OBJID::WINGBOOT]);

	CCollisionMgr::CollisionRectangle(m_listObj[OBJID::PLAYER], m_listObj[OBJID::DUNGEONGATE]);
	

	// 플레이어와 몬스터 <- RectColl 왼쪽 애만 RectColl을 부른다.
	CCollisionMgr::CollisionRectSkullDog(m_listObj[OBJID::PLAYER], m_listObj[OBJID::SKULLDOG]);
	
	// 근접 공격 이펙트와 몬스터 ->	RectColl 오른쪽 애만 RectColl을 부른다.
 	CCollisionMgr::CollisionRectMeleeAttack(m_listObj[OBJID::MELEEATTACK], m_listObj[OBJID::SKULLDOG]);
	CCollisionMgr::CollisionRectMeleeAttack(m_listObj[OBJID::MELEEATTACK], m_listObj[OBJID::BANSH]);
	CCollisionMgr::CollisionRectMeleeAttack(m_listObj[OBJID::MELEEATTACK], m_listObj[OBJID::BAT]);
	CCollisionMgr::CollisionRectMeleeAttack(m_listObj[OBJID::MELEEATTACK], m_listObj[OBJID::ICEBULLET]);
	CCollisionMgr::CollisionRectMeleeAttack(m_listObj[OBJID::PLAYER], m_listObj[OBJID::ACTBOMERANG]);
	CCollisionMgr::CollisionRectMeleeAttack(m_listObj[OBJID::ACTBOMERANG], m_listObj[OBJID::SKULLDOG]);
	CCollisionMgr::CollisionRectMeleeAttack(m_listObj[OBJID::ACTBOMERANG], m_listObj[OBJID::BANSH]);
	CCollisionMgr::CollisionRectMeleeAttack(m_listObj[OBJID::ACTBOMERANG], m_listObj[OBJID::BAT]);
	CCollisionMgr::CollisionRectMeleeAttack(m_listObj[OBJID::MELEEATTACK], m_listObj[OBJID::ICEPILLAR]);
	CCollisionMgr::CollisionRectMeleeAttack(m_listObj[OBJID::ACTBOMERANG], m_listObj[OBJID::ICEPILLAR]);
	CCollisionMgr::CollisionRectMeleeAttack(m_listObj[OBJID::PLAYER], m_listObj[OBJID::MONEY]);
	
	
	
	CCollisionMgr::CollisionRevolver(m_listObj[OBJID::REVOLVERBULLET], m_listObj[OBJID::ICEPILLAR]);
	CCollisionMgr::CollisionRevolver(m_listObj[OBJID::BATITEMBULLET], m_listObj[OBJID::ICEPILLAR]);
	
	// 총알과 플레이어 or 총알과 몬스터 양쪽 RectColl을 둘다 부른다PLAYER
	CCollisionMgr::CollisionRectBullet(m_listObj[OBJID::PLAYER], m_listObj[OBJID::BANSHBULLET]);
	CCollisionMgr::CollisionRectBullet(m_listObj[OBJID::PLAYER], m_listObj[OBJID::BATBULLET]);
	CCollisionMgr::CollisionRectBullet(m_listObj[OBJID::PLAYER], m_listObj[OBJID::ICEBULLET]);
	CCollisionMgr::CollisionRectBullet(m_listObj[OBJID::PLAYER], m_listObj[OBJID::ICESPEAR]);

	CCollisionMgr::CollisionRectBullet(m_listObj[OBJID::REVOLVERBULLET], m_listObj[OBJID::BAT]);
	CCollisionMgr::CollisionRectBullet(m_listObj[OBJID::REVOLVERBULLET], m_listObj[OBJID::BANSH]);
	CCollisionMgr::CollisionRectBullet(m_listObj[OBJID::REVOLVERBULLET], m_listObj[OBJID::SKULLDOG]);

	CCollisionMgr::CollisionRevolver(m_listObj[OBJID::BATITEMBULLET], m_listObj[OBJID::BAT]);
	CCollisionMgr::CollisionRevolver(m_listObj[OBJID::BATITEMBULLET], m_listObj[OBJID::BANSH]);
	CCollisionMgr::CollisionRevolver(m_listObj[OBJID::BATITEMBULLET], m_listObj[OBJID::SKULLDOG]);


	// 보스 맞기 스턴일떄만 맞는다.
	CCollisionMgr::CollisionRectBoss(m_listObj[OBJID::MELEEATTACK], m_listObj[OBJID::BOSS]);
	CCollisionMgr::CollisionRectBoss(m_listObj[OBJID::ACTBOMERANG], m_listObj[OBJID::BOSS]);
	CCollisionMgr::CollisionRectBoss(m_listObj[OBJID::REVOLVERBULLET], m_listObj[OBJID::BOSS]);
	CCollisionMgr::CollisionRectBoss(m_listObj[OBJID::BATITEMBULLET], m_listObj[OBJID::BOSS]);

}

void CObjMgr::Render()
{
	for (int i = 0; i < OBJID::END; ++i)
	{
		for (auto& pObj : m_listObj[i])
		{
			pObj->Render();
		}
	}
}

void CObjMgr::Release()
{
	for (int i = 0; i < OBJID::END; ++i)
	{
		for (auto& pObj : m_listObj[i])
		{
			if (pObj)
			{
				delete pObj;
				pObj = nullptr;
			}

		}
		m_listObj[i].clear();
	}
}

HRESULT CObjMgr::Add_Object(OBJID::ID eID, CObj * pObj)
{
	NULL_CHECK_RETURN(pObj, E_FAIL);
	m_listObj[eID].emplace_back(pObj);
	return S_OK;
}

HRESULT CObjMgr::Delete_Object(OBJID::ID eID)
{
	for (auto& pObj : m_listObj[eID])
	{
		if (pObj)
		{
			delete pObj;
			pObj = nullptr;
		}
	}
	m_listObj[eID].clear();

	return S_OK;
}
