// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
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

// C ��Ÿ�� ��� �����Դϴ�.
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


// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
#include "Include.h" // ���� �ʿ��� ������� ���� ����


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