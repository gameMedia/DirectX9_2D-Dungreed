#pragma once

namespace Link
{
	enum Move{LEFT, RIGHT, UP, DOWN};
}

namespace OBJID
{
	enum ID
	{
		MAP,
		TERRAIN,
		PLAYER,
		MAINHAND,
		SUBHAND,
		DUNGEONGATE,
		REVOLVERBULLET,
		

		//OBJ
		MONEY,

		// Monster
		MONSTER,
		SKULLDOG,
		BANSH,
		SKULLSOLIDER,
		BAT,

		BOSS,
		ICEPILLAR,
		ICEBULLET,
		ICESPEAR,

		BANSHBULLET,
		BATBULLET,


		TRAP,
		//Main_Item
		NORMALSWORD,
		FIRESWORD,
		NORAMLREVOLVER,
		RAREREVOLVER,
		BOMERANG,
		// Artifact
		REGENRING,
		MAGICSHIELD,
		BATITEM,
		BATITEMBULLET,
		CHEST,
		WINGBOOT,

		UI,
		MELEEATTACK,
		ACTBOMERANG,
		EFFECT,
		MOUSE,

		END
	};
	
}

namespace COMMANDID
{
	enum ID
	{
		JUMP,
		END
	};
}

namespace UIID
{
	enum ID
	{
		StartButton,
		SetButton,
		ExitButton,
		HUNGRYGAUGE,
		ENEMYLIFEBAR,
		LIFEBAR,
		DASHGAUGE,
		BOSSHPBAR,
		INVENTORYICON,
		END
	};
}
namespace ONOFFUIID
{
	enum ID
	{
		ONINVENTORY,
		OFFINVENTORY,
		END
	};
}

namespace FIELDID
{
	enum ID
	{
		TOWN,
		NOFIELD,
		DUNGEON1, // 던전 입장시
		DUNGEON2, // 대각선과 해골개
		WEAPON_SHOP, // 무기 상점 포탈
		DUNGEON3, // 점프맵 박쥐
		RESTAURANT, // 음식 판매
		DUNGEON4, // 스켈레톤과 음표 유령 포탈
		REINFORCE, // 무기 강화
		BOSSMAP, // 보스방
		END
	};
}

namespace FIELDTYPE
{
	enum TYPE
	{
		MTOWN,
		INDUNGEON,
		END
	};
}

