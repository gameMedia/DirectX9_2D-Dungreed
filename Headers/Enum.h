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
		DUNGEON1, // ���� �����
		DUNGEON2, // �밢���� �ذ�
		WEAPON_SHOP, // ���� ���� ��Ż
		DUNGEON3, // ������ ����
		RESTAURANT, // ���� �Ǹ�
		DUNGEON4, // ���̷���� ��ǥ ���� ��Ż
		REINFORCE, // ���� ��ȭ
		BOSSMAP, // ������
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

