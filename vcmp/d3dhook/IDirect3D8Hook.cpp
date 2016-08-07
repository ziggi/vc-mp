// PUBLIC DOMAIN
/*
	file:
		IDirect3D8Hook.cpp
	desc:
		This is the code for wrapping the IDirect3D8 interface.
*/

// Includes
#include "../main.h"

// Externals
extern BOOL					bWindowedMode;
extern IDirect3D8			*pD3D;
extern IDirect3DDevice8		*pD3DDevice;
extern IDirect3DDevice8Hook *pD3DDeviceHook;
extern CGame				*pGame;
extern CChatWindow			*pChatWindow;
extern CCmdWindow			*pCmdWindow;

// Globals
D3DDISPLAYMODE D3DDisplayMode;

// Functions
HRESULT __stdcall IDirect3D8Hook::CreateDevice(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DDevice8** ppReturnedDeviceInterface)
{
	/*
	if (bWindowedMode)
	{
		pPresentationParameters->Windowed = 1;
		pPresentationParameters->Flags = 0;
		pPresentationParameters->FullScreen_RefreshRateInHz = 0;
		pPresentationParameters->FullScreen_PresentationInterval = 0;

		GetAdapterDisplayMode(Adapter, &D3DDisplayMode);
		pPresentationParameters->BackBufferFormat = D3DDisplayMode.Format;

		SetWindowPos(pPresentationParameters->hDeviceWindow, HWND_NOTOPMOST, 0, 0, pPresentationParameters->BackBufferWidth, pPresentationParameters->BackBufferHeight, SWP_SHOWWINDOW);
	}*/

	HRESULT hr = pD3D->CreateDevice(Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, ppReturnedDeviceInterface);

	return hr;
}

ULONG __stdcall IDirect3D8Hook::AddRef(void)
{
	return pD3D->AddRef();
}

ULONG __stdcall IDirect3D8Hook::Release(void)
{
	return pD3D->Release();
}

//################################################################################

HRESULT __stdcall IDirect3D8Hook::QueryInterface(REFIID iid, void ** ppvObject)
{
	return pD3D->QueryInterface(iid, ppvObject);
}

HRESULT __stdcall IDirect3D8Hook::RegisterSoftwareDevice(void *pInitializeFunction)
{
	return pD3D->RegisterSoftwareDevice(pInitializeFunction);
}

UINT __stdcall IDirect3D8Hook::GetAdapterCount(void)
{
	return pD3D->GetAdapterCount();
}

HRESULT __stdcall IDirect3D8Hook::GetAdapterIdentifier(UINT Adapter, DWORD Flags, D3DADAPTER_IDENTIFIER8* pIdentifier)
{
	return pD3D->GetAdapterIdentifier(Adapter, Flags , pIdentifier);
}

UINT __stdcall IDirect3D8Hook::GetAdapterModeCount(UINT Adapter)
{
	return pD3D->GetAdapterModeCount(Adapter);
}

HRESULT __stdcall IDirect3D8Hook::EnumAdapterModes(UINT Adapter, UINT Mode, D3DDISPLAYMODE* pMode)
{
	return pD3D->EnumAdapterModes(Adapter, Mode, pMode);
}

HRESULT __stdcall IDirect3D8Hook::GetAdapterDisplayMode(UINT Adapter,D3DDISPLAYMODE *pMode)
{
	return pD3D->GetAdapterDisplayMode(Adapter, pMode);
}

HRESULT __stdcall IDirect3D8Hook::CheckDeviceType(UINT Adapter, D3DDEVTYPE CheckType, D3DFORMAT DisplayFormat, D3DFORMAT BackBufferFormat, BOOL Windowed)
{
	return pD3D->CheckDeviceType(Adapter, CheckType, DisplayFormat, BackBufferFormat, Windowed);
}

HRESULT __stdcall IDirect3D8Hook::CheckDeviceFormat(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, DWORD Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat)
{
	return pD3D->CheckDeviceFormat(Adapter, DeviceType, AdapterFormat, Usage, RType, CheckFormat);
}

HRESULT __stdcall IDirect3D8Hook::CheckDeviceMultiSampleType(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SurfaceFormat, BOOL Windowed, D3DMULTISAMPLE_TYPE MultiSampleType)
{
	return pD3D->CheckDeviceMultiSampleType(Adapter, DeviceType, SurfaceFormat, Windowed, MultiSampleType);
}

HRESULT __stdcall IDirect3D8Hook::CheckDepthStencilMatch(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat)
{
	return pD3D->CheckDepthStencilMatch(Adapter, DeviceType, AdapterFormat, RenderTargetFormat, DepthStencilFormat);
}

HRESULT __stdcall IDirect3D8Hook::GetDeviceCaps(UINT Adapter, D3DDEVTYPE DeviceType, D3DCAPS8* pCaps)
{
	return pD3D->GetDeviceCaps(Adapter, DeviceType, pCaps);
}

HMONITOR __stdcall IDirect3D8Hook::GetAdapterMonitor(UINT Adapter)
{
	return pD3D->GetAdapterMonitor(Adapter);
}
