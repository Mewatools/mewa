/****************************************************************************
** Copyright (C) 2021-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXRENDERER_H
#define MXRENDERER_H

#include "mxvector.h"
#include "mxtexture.h"
#include "mxgpuarray.h"
#include "mxlist.h"


#include<d3d12.h>
#include<dxgi1_6.h>



class MxGpuArray;
class MxGpuProgram;


/*! 
   \brief The MxRenderer keeps track of the rendering state and resources

 The purpose of MxRenderer is to separate the rendering code
 from the application code, in order to isolate all graphics API calls.
This way we can easily support multiple 
graphics APIs, such as Direct12, Metal, Vulkan and OpenGL.

\sa MxTexture, MxGpuArray
*/
class MxRenderer
{
public:
    
    enum Blending {
        NoBlending = 0, // default
        BlendingText,
        BlendingImages,
        CustomBlending // not used
    };

    enum Filter {
        NoFilter = 0x0, // default
        LinearFilter = 0x01,
        MipmapFilter = 0x02,
        MipmapLinearFilter = 0x04
    };

    enum Wrap {
        ClampWrap = 0x0,
        RepeatWrap = 0x8
    };

    


    MxRenderer();
    ~MxRenderer();

    void initialize();

    // \TODO rename to setup to not confuse with initialize
    void init(HWND hwnd, unsigned int windowWidth, unsigned int windowHeight);

  

  
    //! Clears the whole window with the given color, for 1 frame only. Used for debug purposes only (to check if display fb is preserved)
    //void clear(float r, float g, float b);
    //! sets \a viewport
    void setViewport(float width, float height);
    void setScissor( const MxVector2I &pos, const MxVector2I &size );
    void setBlending( Blending blend);
    // \TODO rename to setDepthTest(bool) ??
    void enableDepthTest(bool enable);
    MxTexture* newTexture( const MxVector2I& size, MxTexture::PixelFormat format);
    MxGpuArray * getBuffer( UINT64 length );
    void setProgram( MxGpuProgram* program );
    void setTexturesParameters( unsigned int flags );
    // \TODO chage input argument to MxTexture*
    void bindTextureGL(unsigned int textureId, unsigned int slot = 0);
    void checkGLError( const char *fileName, int line );
    //! triggered at the end of each render (buffer swap)
    virtual void renderEnd();
    



    /// @private
    void setupRoot();
    /// @private
    void prepareToDraw();
    /// @private
    void allocResource(MxGpuArray* newArray, UINT64 length);
    void releaseGpuArrays();
    bool setupPipeline();
  



    bool pEnableSRGB;
    bool pFirstTime;
    bool pPipelineChanged;
    MxGpuProgram* pCurrProgram;
    bool pRootSignatureChanged;
    unsigned int pCurrInputTextureFlags;

    // \TODO is there a better way to reuse buffers ??
   MxList<MxGpuArray> pBufferViews;
   

    // \TODO make it a list of textures
    MxTexture pTextures[1];

    ID3D12Device* pDevice;
    IDXGIFactory6* pDxgiFactory;
    ID3D12CommandAllocator* pCmdAllocator;
    ID3D12GraphicsCommandList* pCmdList;
    ID3D12CommandQueue* pCmdQueue;
    IDXGISwapChain4* pSwapchain;
    ID3D12DescriptorHeap* pRtvHeaps;

    ID3DBlob* pErrorBlob;
    D3D12_GRAPHICS_PIPELINE_STATE_DESC pPipeline;
    ID3D12RootSignature* pRootSignature;

    ID3D12DescriptorHeap* pTexDescHeap;
    ID3D12Resource* pBackBuffers[2];
    ID3D12PipelineState* pPipelinestate;
};

#endif

