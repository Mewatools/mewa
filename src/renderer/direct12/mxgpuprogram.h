/****************************************************************************
** Copyright (C) 2021-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXGPUPROGRAM_H
#define MXGPUPROGRAM_H

class MxRenderer;
struct D3D12_GRAPHICS_PIPELINE_STATE_DESC;

/*! 
Interface class to interact with MxRenderer
*/
class MxGpuProgram
{
public:
	MxGpuProgram();

	bool isInitialized() const;

	virtual void setToPipeline(D3D12_GRAPHICS_PIPELINE_STATE_DESC* pipeline) = 0;
	void init(MxRenderer* renderer);



	MxRenderer* pRenderer;

};


#endif

