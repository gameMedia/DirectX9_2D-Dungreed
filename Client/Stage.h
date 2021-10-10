#pragma once
#include "Scene.h"
class CActMgr;
class CTerrain;

class CStage :
	public CScene
{
private:
	CStage();
public:
	~CStage();

public:
	// CScene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;
	virtual void InitKey();

public:
	

public:
	static CStage* Create();

private:
	CTerrain* m_pTerrain;
	MAP_INFO RenderMap;
	bool InvenOnOff;
	ONOFFUIID::ID eID;
	

};

