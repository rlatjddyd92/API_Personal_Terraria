#pragma once

#define WINCX		800
#define WINCY		600

#define PURE		= 0

#define TILECX		16
#define TILECY		16
#define MAPROW		100
#define MAPCOL		1000

#define PICKLIMIT	TILECX*3

#define INVENC		35

#define INVENQ		10
#define INVENM		20
#define INVENE		10

#define	OBJ_NOEVENT	0
#define	OBJ_DEAD	1

#define	VK_MAX		0xff

#define NUKE_CX		1500

#define ENDPHASE    100

#define PI			3.141592
#define SURTIME		5000
#define SAFETIME	13000

#define MINEWAIT    100

#define HEALCOOL    300

template<typename T>
void Safe_Delete(T& Temp)
{
	if (Temp)
	{
		delete Temp;
		Temp = nullptr;
	}
}

typedef struct Vector2D
{
	float fX;
	float fY;

	Vector2D operator+ (Vector2D Dest) { fX += Dest.fX; fY += Dest.fY; return { fX ,fY }; }
	Vector2D operator* (float Scarla) { fX *= Scarla; fY *= Scarla; return { fX ,fY }; }
	Vector2D Normalliztion(Vector2D Dest) 
	{
		float fVecPower = sqrt((Dest.fX * Dest.fX) + (Dest.fY * Dest.fY));
		return { Dest.fX / fVecPower , Dest.fY / fVecPower };
	}
};

typedef struct tagInfo
{
	float	fX;
	float	fY;
	float	fCX;
	float	fCY;

}INFO;

typedef struct tagIndex
{
	int iRow;
	int iCol;
}INDEX;

struct tagFinder
{
public:
	tagFinder(const TCHAR* pString) : m_pString(pString) {}

public:
	template<typename T>
	bool operator()(T& Pair)
	{
		return !lstrcmp(m_pString, Pair.first);
	}

private:
	const TCHAR* m_pString;
};

extern HWND g_hWnd;

enum class OBJID
{
	PLAYER,
	MONSTER,
	BOSS,
	MISSILE,
	ITEM,
	SUMMON,
	CATCHER,
	MELEE,
	BOMB,
	EXPLOSION,
	FIRE,
	UI,
	INVEN,
	MESSAGE,
	EFEECT,
	ACHIEVEMENT,
	MOUSE,
	END
};

enum class SCENETYPE
{
	MAINMENU,
	EDITER,
	INGAME,
	EXIT,
	END
};

enum class TILETYPE
{
	EMPTY,
	DIRT,
	ROCK,
	COPPER,
	IORN,
	SILVER,
	SAND,
	GOLD,
	WOODEN,
	TREE,
	COAL,
	BEDROCK,
	PRISON,
	BURNTREE,
	FUNITURE,
	END
};

enum class BUTTONFUNCTION
{
	INGAME,
	EDIT,
	EXIT,
	MAINMENU,
	SAVE,
	LOAD,
	END
};

enum class OBJRESULT
{
	NONE,
	DEAD,
	ACTIVE,
	END
};

enum class OBJSPEC
{
	HEALTH,
	ATTACK,
	FUEL,
	END
};

enum class ACTION
{
	IDLE,
	WALKING,
	USING,
	JUMPING,
	END
};

enum class OBJSPECIAL
{
	PUSH,
	FIRE,
	FROZEN,
	POISON,
	SUPERJUMP,
	END
};

enum class OBJBUFF
{
	HEAL,
	POISON,
	FIRE,
	FROZEN,

};

enum class EQUIPPOSITION
{
	ARMOR,
	ACCESSORY,
	CHACTER,
	END
};

enum class ITEMTYPE
{
	POTION,
	TILE,
	FUNITURE,
	PICK,
	MELEE,
	GUN,
	BOW,
	THROW,
	ARMOR,
	SUPERJUMP,
	CHACTER,
	FIRETHROWER,
	AXE,
	BAR,
	END
};

enum class THROWTYPE
{
	BOMB,
	MINE_HIGH,
	MINE_FIRE,
	END
};

enum class DIRECTION
{
	EAST,
	SOUTH,
	WEST,
	NORTH,
	END
};

enum class ITEMMODE
{
	INVEN,
	PICKUP,
	GROUND,
	END
};

enum class INVENTYPE
{
	NORMAL, // <- 기본 상태, 유저가 보유한 인벤 
	FUNITURE_OUTPUT, // <- 가구에서 접근, 제작/구매로 얻는 아이템 
	FUNITURE_INPUT, // <- 가구에서 접근, 제작/구매 시 필요한 아이템 
	END
};

enum class STATS
{
	HEALTH_PLAYER,
	HEALTH_MON,
	HEALTH_BOSS,
	ATTACK,
	DEFENCE,
	FLYGAGE,
	END
};

enum class BACKGROUND
{
	NORMAL,
	SURVIVAL,
	DYNAMITE,
	ABYSS,
	RAVA,
	END
};

enum class CHANNELID
{
	SOUND_PLAYER, // <- 유저 행동 
	SOUND_BGM, // <- 배경음
	SOUND_MON_1, // 몬스터 채널 (몬스터 별 채널 랜덤 배정) 
	SOUND_MON_2,
	SOUND_MON_3,
	SOUND_MON_4,
	SOUND_MON_5,
	SOUND_MON_6,
	SOUND_EXP_1,
	SOUND_EXP_2,
	SOUND_EXP_3,
	SOUND_EXP_4,
	SOUND_EXP_5,
	SOUND_EFFECT,
	MAXCHANNEL
};

enum class FUNITURETYPE
{
	WORKDESK,
	FURNACE,
	WORKSTATION,
	DOOR,
	END
};

enum class ACHIEVMENT
{
	MAKE_FUNANCE,
	MAKE_WORKSTATION,
	SURVIVE,
	END
};