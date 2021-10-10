#pragma once
#include "EffectImpl.h"
class CDamageEffectImpl :
	public CEffectImpl
{
private:
	explicit CDamageEffectImpl(
		const wstring&  wstrObjectKey,
		const wstring&  wstrStateKey,
		int _iDamage);
public:
	virtual ~CDamageEffectImpl();

	// CEffectImpl��(��) ���� ��ӵ�
	virtual int Update() override;
	virtual void Render(const D3DXMATRIX * pWorldMatrix) override;

private:
	void	Set_SearchNumber();
	int	Coll_Num(int _number);

public:
	static CDamageEffectImpl* Create(
		const wstring&  wstrObjectKey,
		const wstring&  wstrStateKey,
		int _iDamage);

private:
	wstring m_wstrStateKey;
	int iDamage;
	int Number; // n��


	vector<int>	m_vecDamageNum;
};