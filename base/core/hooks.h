#pragma once
// used: winapi, directx, fmt includes
#include "../common.h"
// used: hook setup/destroy
#include "../utilities/vmthook.h"
// used: recvprop hook setup/destroy, recvproxydata
#include "netvar.h"
// used: baseclasses
#include "interfaces.h"

#ifndef FASTCALL
#define FASTCALL __fastcall
#endif

/*
* VTABLE INDEXES
* functions indexes in their virtual tables
*/
namespace VTABLE
{
	// work with namespace cuz if use enum class need additional convert to int
	enum
	{
		/* directx table */
		RESET = 16,
		ENDSCENE = 42,
		RESETEX = 132,

		/* client table */
		FRAMESTAGENOTIFY = 37,
		DISPATCHUSERMESSAGE = 38,

		/* panel table */
		PAINTTRAVERSE = 41,

		/* clientmode table */
		OVERRIDEVIEW = 18,
		OVERRIDEMOUSEINPUT = 23,
		CREATEMOVE = 24,
		GETVIEWMODELFOV = 35,
		DOPOSTSCREENEFFECTS = 44,

		/* modelrender table */
		DRAWMODELEXECUTE = 21,

		/* engine table */
		ISCONNECTED = 27,

		/* bsp query table */
		LISTLEAVESINBOX = 6,

		/* prediction table */
		RUNCOMMAND = 19,

		/* steamgamecoordinator table */
		SENDMESSAGE = 0,
		RETRIEVEMESSAGE = 2,

		/* sound table */
		EMITSOUND = 5,

		/* materialsystem table */
		OVERRIDECONFIG = 21,

		/* renderview table */
		SCENEEND = 9,

		/* surface table */
		LOCKCURSOR = 67,
		PLAYSOUND = 82,

		/* gameevent table */
		FIREEVENT = 9,

		/* sv_cheats table */
		SVCHEATS_GETBOOL = 13,

		/* netchannel table */
		SENDNETMSG = 40,
		SENDDATAGRAM = 46
	};
}

/*
 * VMT MANAGERS
 * virtual method table hook managers
 */
namespace VMT
{
	inline CVMTHook	Panel;
	inline CVMTHook	Direct;
	inline CVMTHook	Reset;
	inline CVMTHook	Client;
	inline CVMTHook	ClientMode;
	inline CVMTHook	Engine;
	inline CVMTHook	BspQuery;
	inline CVMTHook	NetChannel;
	inline CVMTHook	Prediction;
	inline CVMTHook	SteamGameCoordinator;
	inline CVMTHook	Sound;
	inline CVMTHook	ModelRender;
	inline CVMTHook	Surface;
	inline CVMTHook	GameEvent;
	inline CVMTHook	SvCheats;
}

/*
 * HOOKS
 * swap functions with given pointers
 */
namespace H
{
	// Get
	bool	Setup();
	void	Restore();

	// Handlers
	/* [type][call]		hk[name] (args...) */
	long	D3DAPI		hkReset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
	long	D3DAPI		hkEndScene(IDirect3DDevice9* pDevice);
	bool	FASTCALL	hkCreateMove(IClientModeShared* thisptr, int edx, float flInputSampleTime, CUserCmd* pCmd);
	void	FASTCALL	hkPaintTraverse(ISurface* thisptr, int edx, unsigned int iPanel, bool bForceRepaint, bool bForce);
	void	FASTCALL	hkPlaySound(ISurface* thisptr, int edx, const char* szFileName);
	void	FASTCALL	hkLockCursor(ISurface* thisptr, int edx);
	void	FASTCALL	hkFrameStageNotify(IBaseClientDll* thisptr, int edx, EClientFrameStage stage);
	bool	FASTCALL	hkDispatchUserMessage(IBaseClientDll* thisptr, int edx, int iMessageType, unsigned int a3, unsigned int uBytes, const void* bfMessageData);
	void	FASTCALL	hkDrawModelExecute(IVModelRender* thisptr, int edx, IMatRenderContext* pContext, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld);
	int		FASTCALL	hkListLeavesInBox(void* thisptr, int edx, Vector& vecMins, Vector& vecMaxs, unsigned short* puList, int iListMax);
	bool	FASTCALL	hkIsConnected(IEngineClient* thisptr, int edx);
	bool	FASTCALL	hkSendNetMsg(INetChannel* thisptr, int edx, INetMessage* pMessage, bool bForceReliable, bool bVoice);
	int		FASTCALL	hkSendDatagram(INetChannel* thisptr, int edx, bf_write* pDatagram);
	void	FASTCALL	hkOverrideView(IClientModeShared* thisptr, int edx, CViewSetup* pSetup);
	void	FASTCALL	hkOverrideMouseInput(IClientModeShared* thisptr, int edx, float* x, float* y);
	float	FASTCALL	hkGetViewModelFOV(IClientModeShared* thisptr, int edx);
	int		FASTCALL	hkDoPostScreenEffects(IClientModeShared* thisptr, int edx, CViewSetup* pSetup);
	void	FASTCALL	hkRunCommand(IPrediction* thisptr, int edx, CBaseEntity* pEntity, CUserCmd* pCmd, IMoveHelper* pMoveHelper);
	int		FASTCALL	hkSendMessage(ISteamGameCoordinator* thisptr, int edx, std::uint32_t uMsgType, const void* pData, std::uint32_t uData);
	int		FASTCALL	hkRetrieveMessage(ISteamGameCoordinator* thisptr, int edx, std::uint32_t* puMsgType, void* pDest, std::uint32_t uDest, std::uint32_t* puMsgSize);
	void	FASTCALL	hkEmitSound(IEngineSound* thisptr, int edx, IRecipientFilter& filter, int nEntityIndex, int iChannel, const char* szSoundEntry, unsigned int uSoundEntryHash, const char* szSample, float flVolume, float flAttenuation, int nSeed, int iFlags, int iPitch, const Vector* vecOrigin, const Vector* vecDirection, CUtlVector<Vector>* pUtlVecOrigins, bool bUpdatePositions, int flSoundTime, int nSpeakerEntity, StartSoundParams_t& parameters);
	bool	FASTCALL	hkSvCheatsGetBool(CConVar* thisptr, int edx);
	long	CALLBACK	hkWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
}

/*
 * RECV VAR PROXY MANAGERS
 * proxy property hook managers
 */
namespace RVP
{
	inline std::shared_ptr<CRecvPropHook> SmokeEffectTickBegin;
}

/*
 * PROXIES
 * networkable property proxy swap functions
 */
namespace P
{
	// Get
	bool	Setup();
	void	Restore();

	// Handlers
	void	SmokeEffectTickBegin(const CRecvProxyData* pData, void* pStruct, void* pOut);
}
