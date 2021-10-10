#pragma once

#define WINCX 800
#define WINCY 600

#define OBJ_NOEVENT 0
#define OBJ_DEAD 1

#define PI 3.141592f
#define DegreeToRadian(x) x*PI/180.f

#define Player_Speed 4.f
#define SkullDog_Speed 5.f
#define Bansh_Speed 1.f
#define Boss_Speed 4.f

// มกวม
#define GRAVITY 3.8f
#define POWER 20.f

#define ENUM_STATE enum STATE{IDLE = 1,MOVE,ATT,HIT,STUN,JUMP,DASH,END}
#define ENUM_DIR enum DIR{LEFT,RIGHT,DOWN,UP,DIREND}
#define RUBY		0x01
#define SAPPHIRE	0x02
#define DIAMOND		0x04

#define TILEX 96
#define TILEY 24
//
#define TILECX1 50
#define TILECY1 50

#define MIN_STR 64
#define MID_STR 128
#define MAX_STR 256


#ifdef TOOL
#define MSG_BOX(msg)									\
	AfxMessageBox(msg)
#else
#define MSG_BOX(msg)									\
	MessageBox(nullptr, msg, L"System Error", MB_OK)
#endif

#define NULL_CHECK(ptr) if(nullptr == (ptr)) return;
#define NULL_CHECK_RETURN(ptr, val) if(nullptr == (ptr)) return val;
#define NULL_CHECK_MSG(ptr, msg) if(nullptr == (ptr)) { MSG_BOX(msg); return; }
#define NULL_CHECK_MSG_RETURN(ptr, msg, val) if(nullptr == (ptr)) { MSG_BOX(msg); return val; }

#define FAILED_CHECK(hr) if(FAILED(hr)) return;
#define FAILED_CHECK_RETURN(hr, val) if(FAILED(hr)) return val;
#define FAILED_CHECK_MSG(hr, msg) if(FAILED(hr)) { MSG_BOX(msg); return; }
#define FAILED_CHECK_MSG_RETURN(hr, msg, val) if(FAILED(hr)) { MSG_BOX(msg); return val; }

#define NO_COPY(classname)						\
private:										\
	className(const className& Obj);			\
	className& operator=(const className& Obj);



#define DEFINE_SINGLETONE(classname)			\
public:											\
	static classname* Get_Instance()			\
	{											\
		if (m_pInstance == nullptr)				\
		{										\
			m_pInstance = new classname;		\
		}										\
		return m_pInstance;						\
	}											\
	static void Destory_Instance()				\
	{											\
		if (m_pInstance)						\
		{										\
			delete m_pInstance;					\
			m_pInstance = nullptr;				\
		}										\
	}											\
private:										\
	static classname* m_pInstance;


#define INFLEMENT_SIGLETONE(classname)			\
classname* classname::m_pInstance = nullptr;