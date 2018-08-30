#pragma region INITIALIZATION
#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include "d3dx9.h"
#include <iostream>
#include <string>

using namespace std;

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

struct Monkey {
	float x, y, z,
		velocity;
	byte bananas,
		lives;
	unsigned short curstg;
	int score,
		ctn;
};

struct ctrl {
	double ctrlstick[4],
		    camstick[4];
	byte dpad;
	bool buttons[8]; // 0=A,1=B,2=Start,3=X,4=Y,5=Z,6=L,7=R
};

#pragma region OBJECTS
LPDIRECT3D9 d3d;
LPDIRECT3DDEVICE9 d3ddev;
LPDIRECT3DTEXTURE9 textures[1048576];
LPD3DXSPRITE sprites[1048576];
RECT rects[1048576];
LPRECT lprects[1048576];
D3DXVECTOR2 pos[1048576];
LPDIRECTXFILE objects[1048576];
LPD3DXFONT fonts[1048576];
RAWINPUT* rawinput[4];
HFONT hfonts[1048576];
#pragma endregion

#pragma region DEFINES
#define TEX_LOADING 0
#define TEX_ADV_AMUSEVIS 0x00001
#define TEX_ADV_PRESENTBY 0x00002
#define TEX_ADV_SEGALOGO 0x00003
#define TEX_ADV_PRESSSTART 0x00004
#define TEX_ADV_GREENBALL 0x00005
#define TEX_ADV_THINKBBL 0x00006
#define TEX_ADV_THINKBBLSML 0x00007
#define TEX_ADV_CREATEDBY 0x00008
#define TEX_ADV_BLUEBALL 0x00009 //<-- wtf
#define TEX_ADV_LOGOA 0x0000A
#define TEX_ADV_LOGOL 0x0000B
#define TEX_ADV_LOGOB 0x0000C
#define TEX_ADV_LOGOM 0x0000D
#define TEX_ADV_LOGOO 0x0000E
#define TEX_ADV_LOGON 0x0000F
#define TEX_ADV_LOGOK 0x00010
#define TEX_ADV_LOGOE 0x00011
#define TEX_ADV_LOGOY 0x00012
#define TEX_ADV_GRBALLSHD 0x00013
#define TEX_ADV_LOGOS 0x00014
#define TEX_ADV_LOGOU 0x00015
#define TEX_ADV_LOGOP 0x00016
#define TEX_ADV_LOGOE_RED 0x00017
#define TEX_ADV_LOGOR 0x00018
#define TEX_ADV_BLBALLSHD 0x00019
#define TEX_ADV_TRDMRK 0x00020
#define TEX_COM_SYSFONT 0x10001
#define TEX_COM_CURSOR 0x10002
#define TEX_COM_CTRLS 0x10003
#define TEX_COM_FACE_GG_IDLE 0x10004
#define TEX_COM_FACE_GG_DNGR1 0x10005
#define TEX_COM_SYSFONT2 0x10007
#define TEX_COM_FACE_GG_DNGR2 0x10008
#define TEX_COM_FACE_GG_DNGR3 0x10009
#define TEX_COM_FACE_GG_DNGR4 0x1000A
#define TEX_COM_FACE_GG_DNGR5 0x1000B
#define TEX_COM_FACE_GG_WAIT5 0x1000C
#define TEX_COM_BANANA_2D 0x1000D
#define TEX_COM_COPYRIGHT 0x1000E
#define TEX_COM_FACE_GG_ANGRY 0x1000D
#define TEX_COM_FACE_AA_IDLE 0x1000E 
#define TEX_COM_FACE_AA_DNGR1 0x1000F
#define TEX_COM_FACE_AA_DNGR2 0x10010
#define TEX_COM_FACE_AA_DNGR3 0x10011
#define TEX_COM_FACE_AA_SAFE1 0x10012
#define TEX_COM_FACE_AA_SAFE2 0x10013
#define TEX_COM_FACE_AA_SAFE3 0x10014
#define TEX_COM_FACE_AA_WAIT5 0x10015
#define TEX_COM_FACE_AA_ANGRY 0x10016
#define TEX_COM_FACE_AA_BLINK1 0x10017
#define TEX_COM_FACE_AA_BLINK2 0x10018
#define TEX_COM_FACE_AA_WAIT1 0x10019
#define TEX_COM_FACE_AA_WAIT2 0x1001A
#define TEX_COM_FACE_AA_WAIT3 0x1001B
#define TEX_COM_FACE_AA_WAIT4 0x1001C
#define TEX_COM_RING_ORANGE 0x1001D
#define TEX_COM_TEST_MODE_FNT 0x1001E
#define TEX_COM_FACE_MM_IDLE 0x1001F
#define TEX_COM_FACE_MM_DNGR1 0x10020
#define TEX_COM_FACE_MM_DNGR2 0x10021
#define TEX_COM_FACE_MM_DNGR3 0x10022
#define TEX_COM_FACE_MM_DNGR4 0x10023
#define TEX_COM_FACE_MM_WAIT5 0x10024
#define TEX_COM_FACE_MM_ANGRY 0x10025
#define TEX_COM_FACE_MM_ANGRY2 0x10026
#define TEX_COM_FACE_MM_BLINK1 0x10027
#define TEX_COM_FACE_MM_BLINK2 0x10028
#define TEX_COM_FACE_MM_WAIT1 0x10029
#define TEX_COM_FACE_MM_WAIT2 0x1002A
#define TEX_COM_FACE_MM_WAIT3 0x1002B
#define TEX_COM_FACE_MM_WAIT4 0x1002C
#define TEX_COM_FACE_MM_WIN1 0x1002D
#define TEX_COM_FACE_MM_WIN2 0x1002E
#define TEX_COM_FACE_MM_WIN3 0x1002F
#define TEX_COM_FACE_MM_WIN4 0x10030
#define TEX_COM_RING_PURPLE 0x10031
#define TEX_COM_FACE_BB_IDLE 0x10032
#define TEX_COM_FACE_BB_DNGR1 0x10033
#define TEX_COM_FACE_BB_DNGR2 0x10034
#define TEX_COM_FACE_BB_DNGR3 0x10035
#define TEX_COM_FACE_BB_DNGR4 0x10036
#define TEX_COM_FACE_BB_WAIT5 0x10037
#define TEX_COM_FACE_BB_ANGRY 0x10038
#define TEX_COM_FACE_BB_ANGRY2 0x10039
#define TEX_COM_FACE_BB_BLINK1 0x1003A
#define TEX_COM_FACE_BB_BLINK2 0x1003B
#define TEX_COM_FACE_BB_WAIT1 0x1003C
#define TEX_COM_FACE_BB_WAIT2 0x1003D
#define TEX_COM_FACE_BB_WAIT3 0x1003E
#define TEX_COM_FACE_BB_WAIT4 0x1003F
#define TEX_COM_FACE_BB_WIN1 0x10040
#define TEX_COM_FACE_BB_WIN2 0x10041
#define TEX_COM_RING_BLUE 0x10042
#define TEX_COM_UNIT_KILO 0x10043
#define TEX_COM_PANEL_13_4 0x10044
#define TEX_COM_FACE_AA_WIN1 0x10045
#define TEX_COM_FACE_AA_WIN2 0x10046
#define TEX_COM_FACE_AA_WIN3 0x10047
#define TEX_COM_FACE_AA_WIN4 0x10048
#define TEX_COM_DEMO_DIALOG_SPIKE 0x10049
#define TEX_COM_DEMO_DIALOG 0x1004A
#define TEX_COM_8PX_WHITE 0x1004B
#define TEX_COM_UNKNOWN1 0x1004C
#define TEX_COM_FACE_GG_ANGRY2 0x1004D
#define TEX_COM_FACE_GG_BLINK1 0x1004E
#define TEX_COM_FACE_GG_BLINK2 0x1004F
#define TEX_COM_HUD_FNT 0x10050
#define TEX_COM_FACE_GG_WAIT1 0x10051
#define TEX_COM_SEGA_AV_LOGO 0x10052
#define TEX_COM_FACE_GG_WAIT2 0x10053
#define TEX_COM_FACE_GG_WAIT3 0x10054
#define TEX_COM_BANANA_10_2D 0x10055
#define TEX_COM_FACE_GG_WAIT4 0x10056
#define TEX_COM_FACE_GG_WIN1 0x1002D
#define TEX_COM_UNIT_MPH 0x1002E
#define TEX_COM_PANEL_26_11 0x1002F
#define TEX_COM_DIALOG_JPEN_FNT 0x10030
#define TEX_COM_FACE_GG_WIN2 0x1002E
#define TEX_COM_FACE_GG_WIN3 0x1002F
#define TEX_COM_FACE_GG_WIN4 0x10030
#define TEX_COM_RING_PINK 0x10031
#define TEX_COM_PANEL_2_1 0x10032
#define TEX_END_SEGA 0x20001
#define TEX_END_AVCOOP 0x20002
#define TEX_END_AV 0x20003
#define TEX_END_TMUSIC 0x20006
#define TEX_END_DOLE 0x20007
#define TEX_HOW_PANEL_52_7 0x30001
#define TEX_HOW_CIRCLRECT 0x30002
#define TEX_HOW_CIRCLE 0x30003
#define TEX_HOW_FGT_ITM_BIG 0x30004
#define TEX_HOW_FGT_BOX 0x30005
#define TEX_HOW_FGT_ITM_LNG 0x30006
#define TEX_HOW_FGT_ITM_VTX 0x30007
#define TEX_HOW_RAC_ITM_BNP 0x30008
#define TEX_HOW_RAC_ITM_BMB 0x30009
#define TEX_HOW_RAC_BOX 0x3000A
#define TEX_HOW_RAC_ITM_ICE 0x3000B
#define TEX_HOW_RAC_ITM_HXG 0x3000C
#define TEX_HOW_RAC_ITM_SPD 0x3000D
#define TEX_HOW_TGT_SCRSH_BOMB 0x3000E
#define TEX_HOW_TGT_BOMB 0x3000F
#define TEX_HOW_TGT_2X 0x30010
#define TEX_HOW_TGT_BRAKE2 0x30011
#define TEX_HOW_TGT_SCRSH_FOG 0x30012
#define TEX_HOW_TGT_FOG 0x30013
#define TEX_HOW_TGT_BLANK 0x30014
#define TEX_HOW_TGT_BRAKE 0x30015
#define TEX_HOW_TGT_SCRSH_SPIKE 0x30016
#define TEX_HOW_TGT_SPIKE 0x30017
#define TEX_HOW_TGT_3X 0x30018
#define TEX_HOW_TGT_NOWIND 0x30019
#define TEX_HOW_BOX_GREEN_LINES 0x3002A
#define TEX_HOW_BOX_RED_LINES 0x3002B
#define TEX_HOW_BOX_GREEN 0x3002C
#define TEX_HOW_BOX_GRAY 0x3002D
#define TEX_HOW_BOX_ORANGE 0x3002E
#define TEX_HOW_BOX_RED 0x3002F
#define TEX_HOW_BOX_BLUE_LINES 0x30030
#define TEX_HOW_BOX_GREEN_LINES2 0x30031
#define TEX_HOW_BOX_BLUE_LINES2 0x30032
#define TEX_HOW_BOX_BLUE2 0x30033
#define TEX_HOW_BOX_GREEN2 0x30034
#define TEX_HOW_BOX_GRAY2 0x30035
#define TEX_HOW_BOX_ORANGE2 0x30036
#define TEX_HOW_BOX_RED2 0x30037
#define TEX_HOW_BOX_SANNKAKU_L 0x30038
#define TEX_HOW_BOX_GCN_CTRL 0x30039
#define TEX_HOW_BOX_BLUE 0x3003A
#define TEX_HOW_SANNKAKU_U 0x3003B
#define TEX_HOW_8PX_WHITE 0x3003C
#define TEX_NML_TIMER 0x3003D
#define TEX_NML_DIFF_ICON 0x3003E
#define TEX_NML_1ST2ND3RD 0x3003F
#define TEX_NML_RESULT 0x30040
#define TEX_NML_GOAL 0x30041
#define TEX_NML_NUM 0x30042
#define TEX_NML_FNT1 0x30043
#define TEX_NML_FNT2 0x30044
#define TEX_NML_FNT3 0x30045
#define TEX_NML_FNT4 0x30046
#define TEX_NML_BBL 0x30047
#define TEX_NML_PLRDISP 0x30048
#define TEX_NML_TIMER_NUM_BIG 0x30049
#define TEX_NML_TIMER_NUM_SML 0x3004A
#define TEX_NML_TIMER_CRACK 0x3004D
#define TEX_NML_TIMER_PIECE1 0x3004E
#define TEX_NML_TIMER_PIECE2 0x3004F
#define TEX_NML_TIMER_PIECE3 0x30050
#define TEX_NML_TIMER_PIECE4 0x30051
#define TEX_NML_TIMER_PIECE5 0x30052
#define TEX_NML_TIMER_PIECE6 0x30053
#define TEX_NML_TIMER_PIECE7 0x30054
#define TEX_NML_TIMER_PIECE8 0x30055
#define TEX_NML_TIMER_PIECE9 0x30056
#define TEX_NML_TIMER_PIECE10 0x30057
#define TEX_RNK_DGT 0x30058
#define TEX_RNK_FNT 0x30059
#define TEX_RNK_A 0x3005A
#define TEX_RNK_G 0x3005B
#define TEX_RNK_I 0x3005C
#define TEX_RNK_K 0x3005D
#define TEX_RNK_N 0x3005E
#define TEX_RNK_R 0x3005F
#define TEX_RNK_TITLE 0x3005A
#define TEX_RNK_BEST_MST 0x3005B
#define TEX_RNK_BEST_EXT 0x3005C
#define TEX_RNK_CIRCLE 0x3005D
#define TEX_RNK_SMBLOGO 0x3005E
#define TEX_RNK_BLUE 0x3005F
#define TEX_RNK_GREEN 0x30060
#define TEX_RNK_RED 0x30061
#define TEX_RNK_ORANGE 0x30062
#define TEX_RNK_MODES 0x30063
#define TEX_RNK_12345TH 0x30064
#define TEX_RNK_DLG_FNT 0x30065
#define TEX_RNK_UNKNOWN1 0x30066
#define TEX_RNK_BG_MAIN 0x30067
#define TEX_RNK_8PX_WHITE 0x30068
#define TEX_RNK_BESTLAP 0x30069
#define TEX_RNK_STAR 0x3006A
#define TEX_RNK_PANEL_26_3 0x3006B
#define TEX_RNK_PANEL_61_6 0x3006C
#define TEX_RNK_REDCIRCL 0x3006D
#define TEX_RNK_R_CIRCL 0x3006E
#define TEX_RNK_T_CIRCL 0x3006F
#define TEX_RNK_RAC_DGT 0x30070
#define TEX_RNK_PTN 0x30071
#define TEX_RNK_PTS 0x30072
#define TEX_RNK_PLS_MNS 0x30073
#define TEX_RNK_12345TH2 0x30074
#define TEX_RNK_DGT2 0x30075
#define TEX_RNK_SCORE 0x30076
#define TEX_RNK_PLRNUM 0x30077
#define TEX_RNK_UNKNOWN2 0x30078
#define TEX_RNK_BANANA 0x30079
#define TEX_RNK_BANANA_RED 0x30080
#define TEX_SEL_PRCT_BEG_EX_EN 0x40001
#define TEX_SEL_PRCT_BEG_EX 0x40002
#define TEX_SEL_PRCT_ADV_EX_EN 0x40003
#define TEX_SEL_PRCT_ADV_EX 0x40004
#define TEX_SEL_BG 0x40005
#define TEX_SEL_FLOWER_GG 0x40006
#define TEX_SEL_RACE_SEL 0x40007
#define TEX_SEL_PANEL 0x40008
#define TEX_SEL_UNKNOWN1 0x40009
#define TEX_SEL_FIGHT_SEL 0x4000A
#define TEX_SEL_UNKNOWN2 0x4000B
#define TEX_SEL_STAR 0x4000C
#define TEX_SEL_PANEL_13_4 0x4000D
#define TEX_SEL_PANEL_13_2 0x4000E
#define TEX_SEL_BUTTON 0x4000F
#define TEX_SEL_PLAYERS 0x40010
#define TEX_SEL_DISABLED 0x40011
#define TEX_SEL_BTN_TITLES 0x40012
#define TEX_SEL_PLAYER_SEL 0x40013
#define TEX_SEL_FLOWER_AA 0x40014
#define TEX_SEL_FLOWER_MM 0x40015
#define TEX_SEL_FLOWER_BB 0x40016
#define TEX_SEL_PANEL_68_11 0x40017
#define TEX_SEL_PANEL_21_11 0x40018
#define TEX_SEL_PANEL_14_5 0x40019
#define TEX_SEL_PANEL_TAB1 0x4001A
#define TEX_SEL_PANEL_TAB2 0x4001B
#define TEX_SEL_LEVEL_DGT 0x4001C
#define TEX_SEL_PRCT_BEG_EN 0x4001D
#define TEX_SEL_PRCT_BEG 0x4001F
#define TEX_SEL_PRCT_ADV_EN 0x4002A
#define TEX_SEL_PRCT_ADV 0x4002B
#define TEX_SEL_PRCT_EXP_EN 0x4002C
#define TEX_SEL_PRCT_EXP 0x4002D
#define TEX_SEL_PRCT_MST_EN 0x4002E
#define TEX_SEL_PRCT_MST 0x4002F
#define TEX_SEL_PANEL_TAB3 0x4002A
#define TEX_SEL_PANEL_TAB4 0x4002B
#define TEX_SEL_PRCT_EXP_EN 0x4002C
#define TEX_SEL_PRCT_EXP 0x4002D
#define TEX_SEL_STATIC 0x4002E
#define TEX_SEL_STATIC_SML 0x4002F
#define TEX_SEL_TV_FRAME 0x40030
#define TEX_SEL_UNKNOWN3 0x40031
#define TEX_SEL_ADVANCED 0x40032
#define TEX_SEL_BEGINNER 0x40033
#define TEX_SEL_EXPERT 0x40034
#define TEX_SEL_DGT 0x40035
#define TEX_SEL_DGTE 0x40036
#define TEX_SEL_DIFF_ICON_SML 0x40037
#define TEX_SEL_STATIC2 0x40038
#define TEX_SEL_BOX 0x40039
#define TEX_SEL_GAMESETTINGS 0x4003A
#define TEX_SEL_ADV_BTN_EN 0x4003B
#define TEX_SEL_ADV_BTN 0x4003C
#define TEX_SEL_BEG_BTN_EN 0x4003D
#define TEX_SEL_BEG_BTN 0x4003E
#define TEX_SEL_EXP_BTN_EN 0x4003F
#define TEX_SEL_EXP_BTN 0x40040
#define TEX_SEL_BTN_GOLF 0x40041

#define OBJ_NULL 0
#define OBJ_BOX 0x00001
#define OBJ_AVBALL 0x00002
#define OBJ_STAGE 0xFFFFF
#pragma endregion

#pragma region VARIABLES
uintptr_t frame, subframe, score;

unsigned short time = 0xE10, stage;

int overridden_version = 0, $;
byte fade = 0, maxctn;

double pi = 3.1415926535897932384626433832795028841971693993751058209749445923;

_D3DFORMAT overridden_format = D3DFMT_X8R8G8B8;

_D3DDEVTYPE overridden_devtype = D3DDEVTYPE_HAL;

D3DSWAPEFFECT overridden_swapfx = D3DSWAPEFFECT_DISCARD;

struct CUSTOMVERTEX { FLOAT X, Y, Z, RHW; DWORD COLOR; };
#define CUSTOMFVF (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

byte screen = 0,

p1binds[] =
#pragma region DEFAULT CONTROLS
{
	//PLAYER 1
	'W', //Control Stick
	'A',
	'S',
	'D',
	VK_LEFT,
	VK_UP,
	VK_RIGHT,
	VK_DOWN,
	'I', //C-Stick
	'J',
	'K',
	'L',
	VK_SPACE, //A
	VK_LSHIFT, //B
	VK_RETURN, //START
	'Z', //X
	'X', //Y
	'C', //Z
	'Q', //L
	'E', //R
};
#pragma endregion

ctrl controllers[4];

signed int color;
static bool freeze, infctn;

void initD3D(HWND hWnd);
void render_frame(void);
void cleanD3D(void);
void init_game(void);
void command(LPWSTR exec, LPWSTR args);
#pragma endregion

void command(LPWSTR exec, LPWSTR args) {
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	ShellExecute(NULL,
		L"open",
		exec,
		args,
		L"",
		SW_HIDE);
	CoUninitialize();
}

void NewTexture(LPCWSTR fname, D3DFORMAT fmt, LPDIRECT3DTEXTURE9 *tex)
{
	D3DXCreateTextureFromFileEx(d3ddev, fname, D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2,
		D3DX_DEFAULT, 0, fmt, D3DPOOL_DEFAULT, D3DX_DEFAULT,
		D3DX_DEFAULT, 0, NULL, NULL, tex);
}

#pragma region CHALLENGE MODE THINGS
struct challengeModeStage {
	unsigned short id;
	int modifiers; // NULL / 0 = No changes, 1 = Final stage, 2 = Custom time
	short time;
};

byte challengeModeCreateStage(challengeModeStage stg)
{
	byte newstage[] = {
		0x02,0x0,0x0,0x0,0x0,stg.id
	};
	return (byte)newstage;
}

byte challengeModeEndStgList[] = {
	0x03,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

// BEGINNER STAGES

challengeModeStage b01 = { 1 };
challengeModeStage b02 = { 2 };
challengeModeStage b03 = { 3 };
challengeModeStage b04 = { 4 };
challengeModeStage b05 = { 91 };
challengeModeStage b06 = { 5 };
challengeModeStage b07 = { 6 };
challengeModeStage b08 = { 7 };
challengeModeStage b09 = { 8 };
challengeModeStage b10 = { 9 };

// ADVANCED STAGES

challengeModeStage a01 = { 11 };
challengeModeStage a02 = { 12 };
challengeModeStage a03 = { 13 };
challengeModeStage a04 = { 14 };
challengeModeStage a05 = { 91, 2, 1800 };
challengeModeStage a06 = { 15 };
challengeModeStage a07 = { 16 };
challengeModeStage a08 = { 17, 2, 1800 };
challengeModeStage a09 = { 18 };
challengeModeStage a10 = { 92, 2, 1800 };
challengeModeStage a11 = { 21, 2, 1800 };
challengeModeStage a12 = { 22, 2, 1800 };
challengeModeStage a13 = { 23, 2, 1800 };
challengeModeStage a14 = { 24 };
challengeModeStage a15 = { 25 };
challengeModeStage a16 = { 26, 2, 1800 };
challengeModeStage a17 = { 27 };
challengeModeStage a18 = { 28 };
challengeModeStage a19 = { 29 };
challengeModeStage a20 = { 93 };
challengeModeStage a21 = { 31 };
challengeModeStage a22 = { 32 };
challengeModeStage a23 = { 33 };
challengeModeStage a24 = { 34 };
challengeModeStage a25 = { 35 };
challengeModeStage a26 = { 36 };
challengeModeStage a27 = { 37 };
challengeModeStage a28 = { 38 };
challengeModeStage a29 = { 39 };
challengeModeStage a30 = { 40 };

// EXPERT STAGES

challengeModeStage e01 = { 41 };
challengeModeStage e02 = { 42 };
challengeModeStage e03 = { 43 };
challengeModeStage e04 = { 44 };
challengeModeStage e05 = { 91 };
challengeModeStage e06 = { 45 };
challengeModeStage e07 = { 46 };
challengeModeStage e08 = { 47 };
challengeModeStage e09 = { 48 };
challengeModeStage e10 = { 92 };
challengeModeStage e11 = { 51 };
challengeModeStage e12 = { 52 };
challengeModeStage e13 = { 53 };
challengeModeStage e14 = { 54 };
challengeModeStage e15 = { 55 };
challengeModeStage e16 = { 56 };
challengeModeStage e17 = { 57 };
challengeModeStage e18 = { 58 };
challengeModeStage e19 = { 59 };
challengeModeStage e20 = { 93 };
challengeModeStage e21 = { 61 };
challengeModeStage e22 = { 62 };
challengeModeStage e23 = { 63 };
challengeModeStage e24 = { 64 };
challengeModeStage e25 = { 65 };
challengeModeStage e26 = { 66 };
challengeModeStage e27 = { 67 };
challengeModeStage e28 = { 68 };
challengeModeStage e29 = { 69 };
challengeModeStage e30 = { 94 };
challengeModeStage e31 = { 71 };
challengeModeStage e32 = { 72 };
challengeModeStage e33 = { 73 };
challengeModeStage e34 = { 74 };
challengeModeStage e35 = { 75 };
challengeModeStage e36 = { 76 };
challengeModeStage e37 = { 77 };
challengeModeStage e38 = { 78 };
challengeModeStage e39 = { 79 };
challengeModeStage e40 = { 95 };
challengeModeStage e41 = { 81 };
challengeModeStage e42 = { 82 };
challengeModeStage e43 = { 83 };
challengeModeStage e44 = { 84 };
challengeModeStage e45 = { 85 };
challengeModeStage e46 = { 86 };
challengeModeStage e47 = { 87 };
challengeModeStage e48 = { 88 };
challengeModeStage e49 = { 89 };
challengeModeStage e50 = { 90 };

// BEGINNER EXPERT STAGES

challengeModeStage bx01 = { 101 };
challengeModeStage bx02 = { 102 };
challengeModeStage bx03 = { 103 };

// ADVANCED EXPERT STAGES

challengeModeStage ax01 = { 101 };
challengeModeStage ax02 = { 104 };
challengeModeStage ax03 = { 103 };

byte challenge[] = {
	0
};
#pragma endregion

string debugBoxText[] = {
	"GAME START",
	"STAGE SELECT",
	"MINI MODE",
	"OPTION",
	"TEST MODE"
};

string englishText[] = {
	""
};

string floorText[] = {
	"saru mark.pic"
	"[Lib No.%d]",
	"FINAL FLOOR",
	"BONUS FLOOR",
	"FLOOR %d",
	"EXTRA %d",
	"MASTER %d",
	"ROUND %d"
};

string pressStartText[] = {
	"Options",
	"p/SANNKAKU_R/ a/Options p/SANNKAKU_L/",
	"Game Start",
	"p/SANNKAKU_R/ a/Game Start p/SANNKAKU_L/",
	""
};

string bananaStr[] = {
	"banana.pic",
	"BANANA(S)"
	"%2d BANANA%s LEFT",

};

string eventText[] = {
	"JUMP TO FLOOR %d",
	"FALL OUT",
	"TIME OVER",
	"BONUS FINISH",
	"h/NOKORI",
	"a/Continue(s):%d",
	"HURRY UP!"
};

string legalText[] = {
	"spr_banana_work",
	"AMUSEMENT VISION, LTD./SEGA,2001",
	"spr_c_work",
	"spr_logo2_work",
	"MONKEY BALL"
};

byte introLevels[] = {
	0x63, 0x47, 0x3D, 0x65, 0x0D, 0x09, 0x15, 0x5F
};

byte levelThemes[201] = { 0x01,
0x0D,0x0D,0x0D,0x0D,0x10,0x10,0x10,0x10,0x10,0x01,
0x0D,0x0D,0x0D,0x0D,0x10,0x10,0x10,0x10,0x01,0x01,
0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x01,
0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,
0x0D,0x0D,0x0D,0x0D,0x0F,0x0F,0x0F,0x0F,0x01,0x01,
0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x01,
0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x01,
0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x01,
0x14,0x14,0x14,0x14,0x14,0x14,0x14,0x14,0x14,0x14,
0x15,0x15,0x15,0x15,0x15,0x01,0x01,0x01,0x0D,0x01,
0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,
0x11,0x11,0x11,0x01,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,
0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,
0x11,0x12,0x0D,0x13,0x0F,0x0E,0x0F,0x12,0x10,0x11,
0x11,0x0E,0x0D,0x13,0x11,0x11,0x11,0x11,0x11,0x0D,
0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,
0x0D,0x0D,0x0D,0x0D,0x0D,0x0D,0x0D,0x0D,0x0D,0x0D,
0x0D,0x0D,0x0D,0x0D,0x0D,0x0D,0x0D,0x0D,0x19,0x17,
0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,
0x01,0x01,0x01,0x01,0x01,0x01,0x15,0x1B,0x0D,0x01 };
#pragma endregion

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	HWND hWnd;
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	//wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = L"GMBJ8P";

	ShowCursor(false);

	RegisterClassEx(&wc);

	hWnd = CreateWindowEx(NULL,
		L"GMBJ8P",
		L"Super Monkey Ball",
		WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX,
		GetSystemMetrics(SM_CXSCREEN) / 2 - SCREEN_WIDTH / 2,
		GetSystemMetrics(SM_CYSCREEN) / 2 - SCREEN_HEIGHT / 2,
		SCREEN_WIDTH + 6, SCREEN_HEIGHT + 34,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hWnd, nCmdShow);

	initD3D(hWnd);

	MSG msg;

	while (TRUE)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
			break;

		render_frame();
	}

	cleanD3D();

	return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		cleanD3D();
		exit(0);
		return 0;
	} break;
	case WM_KEYDOWN:

		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

void initD3D(HWND hWnd)
{
	int argc;

	LPWSTR *argv = CommandLineToArgvW(GetCommandLine(), &argc);

	try
	{
		if (argv[1][0] == *"-" && argv[1][1] == *"d" && argv[1][2] == *"3" && argv[1][3] == *"d" && argv[1][4] == *":")
		{
			if (argv[1][5] == *"9" && argv[1][6] == *"b")
			{
				overridden_version = D3D9b_SDK_VERSION;
				if (argv[1][7] == *",")
				{
					if (argv[1][8] == *"a")
					{
						if (argv[1][9] == *"1")
						{
							overridden_format = D3DFMT_A1;
							if (argv[1][10] == *"6" && argv[1][11] == *"b" && argv[1][12] == *"1" && argv[1][13] == *"6" && argv[1][14] == *"g" && argv[1][15] == *"1" && argv[1][16] == *"6" && argv[1][17] == *"r" && argv[1][18] == *"1" && argv[1][19] == *"6")
							{
								overridden_format = D3DFMT_A16B16G16R16;
								if (argv[1][20] == *"f")
								{
									overridden_format = D3DFMT_A16B16G16R16F;
								}
							}
						}
						if (argv[1][9] == *"2")
						{
							if ((argv[1][11] == *"1" && argv[1][12] == *"0" && argv[1][14] == *"1" && argv[1][15] == *"0" && argv[1][17] == *"1" && argv[1][18] == *"0"))
							{
								if (argv[1][10] == *"b" &&argv[1][13] == *"g" && argv[1][16] == *"r")
								{
									overridden_format = D3DFMT_A2B10G10R10;
								}
								if (argv[1][10] == *"r" &&argv[1][13] == *"g" && argv[1][16] == *"b")
								{
									overridden_format = D3DFMT_A2R10G10B10;
								}
								if (argv[1][10] == *"w" &&argv[1][13] == *"v" && argv[1][16] == *"u")
								{
									overridden_format = D3DFMT_A2W10V10U10;
								}
							}
						}
						if (argv[1][9] == *"3" && argv[1][10] == *"2" && argv[1][11] == *"b" && argv[1][12] == *"3" && argv[1][13] == *"2" && argv[1][14] == *"g" && argv[1][15] == *"3" && argv[1][16] == *"2" && argv[1][17] == *"r" && argv[1][18] == *"3" && argv[1][19] == *"2" && argv[1][20] == *"f")
						{
							overridden_format = D3DFMT_A32B32G32R32F;
						}
						if (argv[1][9] == *"4" || argv[1][11] == *"8")
						{
							overridden_format = D3DFMT_A4L4;
							if (argv[1][11] == *"4" || argv[1][11] == *"8")
							{
								if (argv[1][10] == *"l" && argv[1][11] == *"4")
								{
									overridden_format = D3DFMT_A4L4;
								}
								if (argv[1][11] == *"8")
								{
									if (argv[1][10] == *"l")
									{
										overridden_format = D3DFMT_A8L8;
									}
									if (argv[1][10] == *"p")
									{
										overridden_format = D3DFMT_A8P8;
									}
								}
								if (argv[1][10] == *"r" && argv[1][11] == *"4" && argv[1][12] == *"g" && argv[1][13] == *"4" && argv[1][14] == *"b" && argv[1][15] == *"4")
								{
									overridden_format = D3DFMT_A4R4G4B4;
								}
							}
							if (argv[1][9] == *"8")
							{
								overridden_format = D3DFMT_A8;
								if (argv[1][10] == *"b" && argv[1][11] == *"8" && argv[1][12] == *"g" && argv[1][13] == *"8" && argv[1][14] == *"r" && argv[1][15] == *"8")
								{
									overridden_format = D3DFMT_A8B8G8R8;
								}
								if (argv[1][10] == *"r" && argv[1][11] == *"3" && argv[1][12] == *"g" && argv[1][13] == *"3" && argv[1][14] == *"b" && argv[1][15] == *"2")
								{
									overridden_format = D3DFMT_A8R3G3B2;
								}
								if (argv[1][10] == *"r" && argv[1][11] == *"8" && argv[1][12] == *"g" && argv[1][13] == *"8" && argv[1][14] == *"b" && argv[1][15] == *"8")
								{
									overridden_format = D3DFMT_A8R8G8B8;
								}
							}
						}
					}
					if (argv[1][8] == *"b" && argv[1][9] == *"i" && argv[1][10] == *"n" && argv[1][11] == *"a" && argv[1][12] == *"r" && argv[1][13] == *"y" && argv[1][14] == *"b" && argv[1][15] == *"u" && argv[1][16] == *"f" && argv[1][17] == *"f" && argv[1][18] == *"e" && argv[1][19] == *"r")
					{
						overridden_format = D3DFMT_BINARYBUFFER;
					}
					if (argv[1][8] == *"c" && argv[1][9] == *"x" && argv[1][10] == *"v" && argv[1][11] == *"8" && argv[1][12] == *"u" && argv[1][13] == *"8")
					{
						overridden_format = D3DFMT_CxV8U8;
					}
					if (argv[1][8] == *"d")
					{
						if (argv[1][9] == *"1" && argv[1][10] == *"5" && argv[1][11] == *"s" && argv[1][12] == *"1")
						{
							overridden_format = D3DFMT_D15S1;
						}
						if (argv[1][9] == *"1" && argv[1][10] == *"6")
						{
							overridden_format = D3DFMT_D16;
							if (argv[1][11] == *"l")
							{
								overridden_format = D3DFMT_D16_LOCKABLE;
							}
						}
						if (argv[1][9] == *"2" && argv[1][10] == *"4")
						{
							if (argv[1][11] == *"f" && argv[1][12] == *"s" && argv[1][13] == *"8")
							{
								overridden_format = D3DFMT_D24FS8;
							}
							if (argv[1][11] == *"s" && argv[1][12] == *"8")
							{
								overridden_format = D3DFMT_D24S8;
							}
							if (argv[1][11] == *"x" && argv[1][12] == *"4" && argv[1][13] == *"s" && argv[1][14] == *"4")
							{
								overridden_format = D3DFMT_D24X4S4;
							}
							if (argv[1][11] == *"x" && argv[1][12] == *"8")
							{
								overridden_format = D3DFMT_D24X8;
							}
						}
						if (argv[1][9] == *"3" && argv[1][10] == *"2")
						{
							overridden_format = D3DFMT_D32;
							if (argv[1][11] == *"f" && argv[1][12] == *"l")
							{
								overridden_format = D3DFMT_D32F_LOCKABLE;
							}
							if (argv[1][11] == *"l")
							{
								overridden_format = D3DFMT_D32_LOCKABLE;
							}
						}
						if (argv[1][9] == *"x" && argv[1][10] == *"t")
						{
							if (argv[1][11] = *"1") overridden_format = D3DFMT_DXT1;
							if (argv[1][11] = *"2") overridden_format = D3DFMT_DXT2;
							if (argv[1][11] = *"3") overridden_format = D3DFMT_DXT3;
							if (argv[1][11] = *"4") overridden_format = D3DFMT_DXT4;
							if (argv[1][11] = *"5") overridden_format = D3DFMT_DXT5;
						}
						if (argv[1][9] == *"x" && argv[1][10] == *"t")
						{

						}
					}
				}
			}
			else if (argv[1][5] == *"x")
			{
				overridden_version = D3DX_VERSION;
			}
			else if (argv[1][5] == *"s" && argv[1][6] == *"d" && argv[1][7] == *"k")
			{
				overridden_version = D3D_SDK_VERSION;
			}
			else if (argv[1][5] == *"d" && argv[1][6] == *"e" && argv[1][7] == *"f")
			{
				overridden_version = DIRECT3D_VERSION;
			}
		}
	}

	catch (exception ex) {
		overridden_version = D3D_SDK_VERSION;
		overridden_format = D3DFMT_X8R8G8B8;
		overridden_devtype = D3DDEVTYPE_HAL;
		overridden_swapfx = D3DSWAPEFFECT_DISCARD;
	};

	d3d = Direct3DCreate9(overridden_version);

	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = overridden_swapfx;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.BackBufferFormat = overridden_format;
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

	d3d->CreateDevice(D3DADAPTER_DEFAULT,
		overridden_devtype,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING|
		D3DCREATE_MULTITHREADED,
		&d3dpp,
		&d3ddev);

	d3ddev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);

	d3ddev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

	d3ddev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

	d3ddev->SetRenderState(D3DRS_LIGHTING, FALSE);

	d3ddev->SetRenderState(D3DRS_ZENABLE, FALSE);

	NewTexture(L"data\\loading.gct", D3DFMT_DXT1, &textures[TEX_LOADING]);

	D3DXCreateSprite(d3ddev, &sprites[0]);

	hfonts[0] = CreateFontA(64,64,0,0,400,false,false,false,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_DONTCARE,"Arial Narrow");

	D3DXCreateFont(d3ddev, hfonts[0], &fonts[0]);
}

void render_frame(void)
{
	if (!freeze)
	{
		d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, color, 0.0f, 0);

		d3ddev->BeginScene();

		switch (screen)
		{
		case 0:
			switch (frame)
			{
			case 0:
				color = 0;
				sprites[0]->Begin();
				sprites[0]->Draw(textures[TEX_LOADING], NULL, NULL, 0, 0, NULL, 0xFFFFFFFF);
				sprites[0]->End();
				break;
			case 1:
				init_game();
				screen = 1;
				subframe = 0;
				break;
			}
			break;
		case 1:
			if (fade < 255 && subframe < 51) {
				color = D3DCOLOR_ARGB(255, fade, fade, fade);
				pos[TEX_ADV_SEGALOGO].x = 64.0f;
				pos[TEX_ADV_SEGALOGO].y = -16.0f;
				pos[TEX_ADV_PRESENTBY].x = 208.0f;
				pos[TEX_ADV_PRESENTBY].y = 96.0f;
				sprites[0]->Begin();
				sprites[0]->Draw(textures[TEX_ADV_SEGALOGO], NULL, NULL, NULL, 0, &pos[TEX_ADV_SEGALOGO], color);
				sprites[0]->Draw(textures[TEX_ADV_PRESENTBY], NULL, NULL, NULL, 0, &pos[TEX_ADV_PRESENTBY], color);
				sprites[0]->End();
				fade += 5;
			}
			else if (subframe > 50 && subframe < 291)
			{
				color = D3DCOLOR_ARGB(255, 255, 255, 255);

				sprites[0]->Begin();
				sprites[0]->Draw(textures[TEX_ADV_SEGALOGO], NULL, NULL, NULL, 0, &pos[TEX_ADV_SEGALOGO], color);
				sprites[0]->Draw(textures[TEX_ADV_PRESENTBY], NULL, NULL, NULL, 0, &pos[TEX_ADV_PRESENTBY], color);
				sprites[0]->End();
			}
			else if (subframe > 290 && subframe < 340) {
				color = D3DCOLOR_ARGB(255, fade, fade, fade);
				sprites[0]->Begin();
				sprites[0]->Draw(textures[TEX_ADV_SEGALOGO], NULL, NULL, NULL, 0, &pos[TEX_ADV_SEGALOGO], color);
				sprites[0]->Draw(textures[TEX_ADV_PRESENTBY], NULL, NULL, NULL, 0, &pos[TEX_ADV_PRESENTBY], color);
				sprites[0]->End();
				fade -= 5;
			}
			if (subframe > 340 && subframe < 390) {
				color = D3DCOLOR_ARGB(255, fade, (int)(fade / 1.328125), 0);
				pos[TEX_ADV_CREATEDBY].x = 224.0f;
				pos[TEX_ADV_CREATEDBY].y = 48.0f;
				pos[TEX_ADV_AMUSEVIS].x = 192.0f;
				pos[TEX_ADV_AMUSEVIS].y = 128.0f;
				sprites[0]->Begin();
				sprites[0]->Draw(textures[TEX_ADV_CREATEDBY], &rects[TEX_ADV_CREATEDBY], NULL, NULL, 0, &pos[TEX_ADV_CREATEDBY], D3DCOLOR_ARGB(255, fade, fade, fade));
				sprites[0]->Draw(textures[TEX_ADV_AMUSEVIS], NULL, NULL, NULL, 0, &pos[TEX_ADV_AMUSEVIS], D3DCOLOR_ARGB(255, fade, fade, fade));
				sprites[0]->End();
				fade += 5;
			}
			else if (subframe > 389 && subframe < 530) {
				sprites[0]->Begin();
				sprites[0]->Draw(textures[TEX_ADV_CREATEDBY], &rects[TEX_ADV_CREATEDBY], NULL, NULL, 0, &pos[TEX_ADV_CREATEDBY], D3DCOLOR_ARGB(255, fade, fade, fade));
				sprites[0]->Draw(textures[TEX_ADV_AMUSEVIS], NULL, NULL, NULL, 0, &pos[TEX_ADV_AMUSEVIS], D3DCOLOR_ARGB(255, fade, fade, fade));
				sprites[0]->End();
			}
			else if (subframe > 529 && subframe < 580) {
				color = D3DCOLOR_ARGB(255, fade, (int)(fade / 1.328125), 0);
				sprites[0]->Begin();
				sprites[0]->Draw(textures[TEX_ADV_CREATEDBY], &rects[TEX_ADV_CREATEDBY], NULL, NULL, 0, &pos[TEX_ADV_CREATEDBY], D3DCOLOR_ARGB(255, fade, fade, fade));
				sprites[0]->Draw(textures[TEX_ADV_AMUSEVIS], NULL, NULL, NULL, 0, &pos[TEX_ADV_AMUSEVIS], D3DCOLOR_ARGB(255, fade, fade, fade));
				sprites[0]->End();
				fade -= 5;
			}
			else if (subframe > 579 && fade == 0 && subframe != 0)
			{
				command(L"gx.exe", L"data\\bmp\\bmp_nml.tpl tpl 0 data\\temp\\0x00000004");
				NewTexture(L"data\\temp\\0x00000004", D3DFMT_DXT2, &textures[TEX_NML_TIMER]);
				command(L"gx.exe", L"data\\bmp\\bmp_nml.tpl tpl 12 data\\temp\\0x00000005");
				NewTexture(L"data\\temp\\0x00000005", D3DFMT_DXT2, &textures[TEX_NML_TIMER_NUM_BIG]);
				CreateDirectory(L"data\\temp\\0x99999999", NULL);
				command(L"gx.exe", L"data\\st001\\st001.gma gma data\\temp\\0x99999999");
				//DirectXFileCreate(&objects[0]);
				//NewTexture(L"data\\temp\\0x99999999", D3DFMT_DXT2, &textures[TEX_NML_TIMER_NUM_BIG]);
				subframe = 0;
				screen = 2;
			}
			break;
		case 2:
			pos[TEX_NML_TIMER].x = 256;
			pos[TEX_NML_TIMER].y = 0;
			color = 0xFFFFFFFF;
			sprites[0]->Begin();
			sprites[0]->Draw(textures[TEX_NML_TIMER], NULL, NULL, NULL, 0, &pos[TEX_NML_TIMER], D3DCOLOR_ARGB(255, 255, 255, 255));
			/*switch (time)
			{
			case 3600:
				sprites[0]->Draw(textures[TEX_NML_TIMER_NUM_BIG], NULL, NULL, NULL, 0, &pos[TEX_NML_TIMER], D3DCOLOR_ARGB(255, 255, 255, 255));
				break;
			case 1:
				break;
			}*/
			fonts[0]->Begin();
			fonts[0]->DrawTextA("Test",-1,&rects[TEX_ADV_CREATEDBY],0,D3DCOLOR_ARGB(255,255,0,0));
			fonts[0]->End();
			sprites[0]->End();
			time -= 1;
			break;
		}

		d3ddev->EndScene();

		d3ddev->Present(NULL, NULL, NULL, NULL);

		frame += 1;
		subframe += 1;
	}
}

void init_game(void)
{
	command(L"gx.exe",L"data\\bmp\\bmp_adv.tpl tpl 2 data\\temp\\0x00000000");
	NewTexture(L"data\\temp\\0x00000000", D3DFMT_DXT1, &textures[TEX_ADV_SEGALOGO]);
	command(L"gx.exe", L"data\\bmp\\bmp_adv.tpl tpl 1 data\\temp\\0x00000001");
	NewTexture(L"data\\temp\\0x00000001", D3DFMT_DXT1, &textures[TEX_ADV_PRESENTBY]);
	command(L"gx.exe", L"data\\bmp\\bmp_adv.tpl tpl 0 data\\temp\\0x00000002");
	NewTexture(L"data\\temp\\0x00000002", D3DFMT_DXT2, &textures[TEX_ADV_AMUSEVIS]);
	command(L"gx.exe", L"data\\bmp\\bmp_adv.tpl tpl 7 data\\temp\\0x00000003");
	NewTexture(L"data\\temp\\0x00000003", D3DFMT_DXT2, &textures[TEX_ADV_CREATEDBY]);
	rects[TEX_ADV_CREATEDBY].left = 32;
	rects[TEX_ADV_CREATEDBY].right = 256-32;
	rects[TEX_ADV_CREATEDBY].bottom = 256;
	rects[TEX_ADV_CREATEDBY].top = 200;
	textures[TEX_LOADING]->Release();
}

void cleanD3D(void)
{
	d3ddev->Release();
	d3d->Release();
}