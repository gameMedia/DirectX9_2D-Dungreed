// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

#include <io.h>
#include "fmod.h"
#pragma comment(lib, "fmodex_vc.lib")

#include <Vfw.h>
#pragma comment(lib, "Vfw32.lib")

#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <map>
#include <time.h>
#include <algorithm>
#include <functional>
#include <string>
#include <process.h>

using namespace std;


// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#include "Include.h" // 내가 필요한 헤더들을 여기 넣자


// Mgr
#include "TextureMgr.h"
#include "DeviceMgr.h"
#include "TileMgr.h"
#include "SceneMgr.h"
#include "ObjMgr.h"
#include "KeyMgr.h"
#include "TimeMgr.h"
#include "ScrollMgr.h"
#include "UIMgr.h"
#include "FieldMgr.h"
#include "CollisionMgr.h"
#include "LineMgr.h"
#include "OnOffUIMgr.h"
#include "SoundMgr.h"