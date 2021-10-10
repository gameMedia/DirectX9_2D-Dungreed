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
	// 사운드 리소스 정보를 갖는 객체 
	map<TCHAR*, FMOD_SOUND*> m_mapSound;
	// 재생하고 있는 사운드를 관리할 객체 
	FMOD_CHANNEL* m_pChannel[MAXCHANNEL];
	// 사운드 채널 객체 및 장치를 관리하는 객체. 
	FMOD_SYSTEM* m_pSystem;

};
