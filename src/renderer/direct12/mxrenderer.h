/****************************************************************************
** Copyright (C) 2021-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXRENDERER_H
#define MXRENDERER_H

#include "mxvector.h"
#include "mxgpuarray.h"
#include "mxlist.h"


#include<d3d12.h>
#include<dxgi1_6.h>


class MxTexture;
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


    


    MxRenderer();
    ~MxRenderer();

    // used by the OpenGL renderer
    void initialize();

    // \TODO rename to setupDirectX to not confuse with initialize
    void setupDirectX(HWND hwnd, unsigned int windowWidth, unsigned int windowHeight);

  

  
    //! Clears the whole window with the given color, for 1 frame only. Used for debug purposes only (to check if display fb is preserved)
    //void clear(float r, float g, float b);
    //! sets \a viewport
    void setViewport(int x, int y, unsigned int width, unsigned int height);
    void setScissor( const MxVector2I &pos, const MxVector2I &size );
    void setBlending( Blending blend);
    // \TODO rename to setDepthTest(bool) ??
    void enableDepthTest(bool enable);
    MxGpuArray * getBuffer( UINT64 length );
    void setProgram( MxGpuProgram* program );
    //void setTexturesParameters( unsigned int flags );

    /*!The parameters are the Filterand Wrap bits alloed.Note that multiple options are allowed,
     thats because the renderer tries to minimize state changes.
     Whenever setting data to a texture (MxTexture::setPixelData()), if calling bindTexture() call it afterwards
    */
    void bindTexture( MxTexture *texture, unsigned char parameters, int inputIndex );
    void checkGLError( const char *fileName, int line );
    void renderBegin();
    //! triggered at the end of each render (buffer swap)
    virtual void renderEnd();
    void resetBoundTextures();
    


  
    /// @private
    void prepareRootSignature();
    /// @private
    void prepareToDraw();
    /// @private
    void allocResource(MxGpuArray* newArray, UINT64 length);
    void releaseGpuArrays();
    bool setupPipeline();
    void connectInputTextures( int count );
  
    enum PipelineFlag
    {
        DepthTestEnabled = 0x01,
        StencilEnabled = 0x02,
    };
    unsigned int pPipelineEnabled;
    Blending pBlendOption;
    bool pEnableSRGB;
    bool pFirstTime;
    bool pPipelineChanged;
    MxGpuProgram* pCurrProgram;
    bool pRootSignatureChanged;
    unsigned int pCurrInputTextureFlags; // up to 4 inputs
    struct StoredRootSignature
    {
        unsigned int inputFlags;
        ID3D12RootSignature* rootSignature;
    };
    MxList<StoredRootSignature> pRootInputList;


    // \TODO is there a better way to reuse buffers ??
   MxList<MxGpuArray> pBufferViews;
   
   enum Limits
   {
       MaxBoundTextures = 2,
   };

   struct Texture_p
   {
       MxTexture* texture;
       unsigned char parameters;
    };
    // \TODO change to MxStack
   Texture_p pBoundTextures[MaxBoundTextures]; // bound textures in its slots
   
   D3D12_STATIC_SAMPLER_DESC pSamplerDesc[MaxBoundTextures];


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


    ID3D12Resource* pBackBuffers[2];
    ID3D12PipelineState* pPipelinestate;
};

#endif

