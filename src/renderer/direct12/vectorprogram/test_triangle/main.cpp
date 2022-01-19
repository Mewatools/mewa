/****************************************************************************
** Copyright (C) 2021-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include<Windows.h>
#include<tchar.h>
#include<d3d12.h>
#include<DirectXMath.h>



#ifdef _DEBUG
#include<iostream>
#endif


#include "triangleapp.h"



#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")

using namespace DirectX;







LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	if (msg == WM_DESTROY) {
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

const unsigned int window_width = 1280;
const unsigned int window_height = 720;





void EnableDebugLayer() {
	ID3D12Debug* debugLayer = nullptr;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugLayer)))) {
		debugLayer->EnableDebugLayer();
		debugLayer->Release();
	}
}

#ifdef _DEBUG
int main() {
#else
#include<Windows.h>
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
#endif

	

	qDebug("starting test app...");
	HINSTANCE hInst = GetModuleHandle(nullptr);

	WNDCLASSEX w = {};
	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WindowProcedure;
	w.lpszClassName = _T("Direct12Test");
	w.hInstance = GetModuleHandle(0);
	RegisterClassEx(&w);

	RECT wrc = { 0,0, window_width, window_height };
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	HWND hwnd = CreateWindow(w.lpszClassName,
		_T("MewaDX12Test"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wrc.right - wrc.left,
		wrc.bottom - wrc.top,
		nullptr,
		nullptr,
		w.hInstance,
		nullptr);

	

#ifdef _DEBUG
	EnableDebugLayer();
#endif

	
	HRESULT result;


	TriangleApp* app = new TriangleApp();




	// \TODO needs refactoring
	MxRenderer* renderer = &(app->pRenderer);
	renderer->init(hwnd, window_width, window_height );
	app->intialize();
	app->onResizeWindow(window_width, window_height);

	ID3D12Fence* _fence = nullptr;
	UINT64 _fenceVal = 0;
	result = renderer->pDevice->CreateFence(_fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence));

	ShowWindow(hwnd, SW_SHOW);



	unsigned int tmp = 0;

	MSG msg = {};
	unsigned int frame = 0;
	while (true) {

		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT) {
			break;
		}



		

		
		UINT bbIdx = renderer->pSwapchain->GetCurrentBackBufferIndex();


		// copy back to front
		if (frame >= 1) {
			UINT fbIndex = bbIdx ^ (0x01);
			renderer->pCmdList->CopyResource( renderer->pBackBuffers[bbIdx], renderer->pBackBuffers[fbIndex]);
		}

		// this barrier should be after CopyResource (GPU copy). No need for a barrier before CopyResource
		D3D12_RESOURCE_BARRIER BarrierDesc = {};
		BarrierDesc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		BarrierDesc.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		BarrierDesc.Transition.pResource = renderer->pBackBuffers[bbIdx];
		BarrierDesc.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		BarrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST; //D3D12_RESOURCE_STATE_PRESENT;
		BarrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET; // D3D12_RESOURCE_STATE_COPY_DEST

		renderer->pCmdList->ResourceBarrier(1, &BarrierDesc);

		

		auto rtvH = renderer->pRtvHeaps->GetCPUDescriptorHandleForHeapStart();
		rtvH.ptr += bbIdx * renderer->pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		renderer->pCmdList->OMSetRenderTargets(1, &rtvH, false, nullptr);

		
		

		
		// test if display buffer is preserved through fb swap
		if ((frame % 60) == 0) 
		{
			float r, g, b;
			if (tmp % 2 == 0)
			{
				r = 0.1f;
				g = 0.1f;
				b = 1.0f;
			}
			else {
				r = 1.0f;
				g = 0.2f;
				b = 0.2f;
			}
			tmp++;
			float clearColor[] = { r,g,b,1.0f };
			
			renderer->pCmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);
		}
		
		++frame;

		app->onRender();



		BarrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		BarrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
		renderer->pCmdList->ResourceBarrier(1, &BarrierDesc);

		renderer->pCmdList->Close();



		ID3D12CommandList* cmdlists[] = { renderer->pCmdList };
		renderer->pCmdQueue->ExecuteCommandLists(1, cmdlists);
		renderer->pCmdQueue->Signal(_fence, ++_fenceVal);

		if (_fence->GetCompletedValue() != _fenceVal) {
			auto event = CreateEvent(nullptr, false, false, nullptr);
			_fence->SetEventOnCompletion(_fenceVal, event);
			WaitForSingleObject(event, INFINITE);
			CloseHandle(event);
		}
		renderer->renderEnd();
		renderer->pCmdAllocator->Reset();
		renderer->pCmdList->Reset(renderer->pCmdAllocator, renderer->pPipelinestate);

		renderer->releaseGpuArrays();
	
		renderer->pSwapchain->Present(1, 0);

	}

	UnregisterClass(w.lpszClassName, w.hInstance);
	return 0;
}
