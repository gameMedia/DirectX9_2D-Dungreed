#include "stdafx.h"
#include "EffectImpl.h"


CEffectImpl::CEffectImpl(const wstring& wstrObjectKey)
	:m_wstrObjectKey(wstrObjectKey)
	,playSpeed(1.f)
{
}


CEffectImpl::~CEffectImpl()
{
}
