// Copyright (c) 2012- PPSSPP Project.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 2.0 or later versions.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License 2.0 for more details.

// A copy of the GPL 2.0 should have been included with the program.
// If not, see http://www.gnu.org/licenses/

// Official git repository and contact information can be found at
// https://github.com/hrydgard/ppsspp and http://www.ppsspp.org/.

#pragma once

#include <d3d9.h>

#include "GPU/GPU.h"
#include "GPU/GPUInterface.h"
#include "GPU/Common/TextureCacheCommon.h"

struct VirtualFramebuffer;
class TextureShaderCache;

class FramebufferManagerDX9;
class ShaderManagerDX9;

class TextureCacheDX9 : public TextureCacheCommon {
public:
	TextureCacheDX9(Draw::DrawContext *draw, Draw2D *draw2D);
	~TextureCacheDX9();

	void StartFrame() override;

	void SetFramebufferManager(FramebufferManagerDX9 *fbManager);

	void ForgetLastTexture() override {
		InvalidateLastTexture();
	}
	void InvalidateLastTexture() override;

	bool GetCurrentTextureDebug(GPUDebugBuffer &buffer, int level, bool *isFramebuffer) override;

protected:
	void BindTexture(TexCacheEntry *entry) override;
	void Unbind() override;
	void ReleaseTexture(TexCacheEntry *entry, bool delete_them) override;
	void BindAsClutTexture(Draw::Texture *tex, bool smooth) override;
	void *GetNativeTextureView(const TexCacheEntry *entry) override;

private:
	void ApplySamplingParams(const SamplerCacheKey &key) override;

	D3DFORMAT GetDestFormat(GETextureFormat format, GEPaletteFormat clutFormat) const;
	void UpdateCurrentClut(GEPaletteFormat clutFormat, u32 clutBase, bool clutIndexIsSimple) override;

	void BuildTexture(TexCacheEntry *const entry) override;

	LPDIRECT3DBASETEXTURE9 &DxTex(const TexCacheEntry *entry) const {
		return *(LPDIRECT3DBASETEXTURE9 *)&entry->texturePtr;
	}

	LPDIRECT3DDEVICE9 device_;
	LPDIRECT3DDEVICE9EX deviceEx_;

	LPDIRECT3DVERTEXDECLARATION9 pFramebufferVertexDecl;

	LPDIRECT3DBASETEXTURE9 lastBoundTexture;
	float maxAnisotropyLevel;

	FramebufferManagerDX9 *framebufferManagerDX9_;
};

D3DFORMAT getClutDestFormat(GEPaletteFormat format);
