#pragma once
class CSoundMgr
{
	DEFINE_SINGLETONE(CSoundMgr)
private:
	CSoundMgr();
	~CSoundMgr();
public:
	enum CHANNELID { 
		BGM, 
		
		PLAYER,
		PLAYERDASH,
		PLAYERJUMP,
		PLAYERMOVE,
		PLAYERMELEEATTACK,
		PLAYERHIT,
		PLAYERSWING,
		REVOLVERBULLET,
		EATFOOD,
		SALESHOP,

		ONOFFINVENTORYSOUND,
		PICKINVENITEM,
		GETITEM,
		GETMONEY,
		
		MONSTERHIT,
		MONSTERDEAD,
		BOXOPEN,
		
		BANSHATTACK,
		BATATTACK,
		BOSSICESPEAR,

		ICEBULLETCREATE,
		ICEBULLETDEAD,

		MONSTER, 
		
		EFFECT, 
		
		UI, 
		
		MAXCHANNEL };
public:
	void Initialize();
	void Release();
public:
	void PlaySound(TCHAR* pSoundKey, CHANNELID eID);
	void PlayBGM(TCHAR* pSoundKey);
	void StopSound(CHANNELID eID);
	void StopAll();
private:
	void LoadSoundFile();

private:
	// ���� ���ҽ� ������ ���� ��ü 
	map<TCHAR*, FMOD_SOUND*> m_mapSound;
	// ����ϰ� �ִ� ���带 ������ ��ü 
	FMOD_CHANNEL* m_pChannel[MAXCHANNEL];
	// ���� ä�� ��ü �� ��ġ�� �����ϴ� ��ü. 
	FMOD_SYSTEM* m_pSystem;

};
