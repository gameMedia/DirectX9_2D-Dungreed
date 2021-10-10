#pragma once
class CCollisionMgr
{
public:
	CCollisionMgr();
	~CCollisionMgr();

public:
	static void CollisionCheck_Tile(OBJLIST& rDestList, vector<TILE_INFO*>* rSourList);
	static void CollisionMonsterCheck_Tile(OBJLIST& rDestList, vector<TILE_INFO*>* rSourList);
	static void CollisionBulletCheck_Tile(OBJLIST& rDestList, vector<TILE_INFO*>* rSourList);
	static void CollisionRect(OBJLIST& rDestList, OBJLIST& rSourList);
	static void CollisionRectangle(OBJLIST& rDestList, OBJLIST& rSourList);
	
	static void CollisionRectSkullDog(OBJLIST& rDestList, OBJLIST& rSourList);
	
	static void CollisionRectMeleeAttack(OBJLIST& rDestList, OBJLIST& rSourList);

	static void CollisionRectBullet(OBJLIST& rDestList, OBJLIST& rSourList);

	static void CollisionRectBoss(OBJLIST& rDestList, OBJLIST& rSourList);
	

	static void CollisionRevolver(OBJLIST& rDestList, OBJLIST& rSourList);

private:
	static bool CheckRect(CObj* pDst, TILE_INFO* pSour, float*pMoveX, float* pMoveY);
};

