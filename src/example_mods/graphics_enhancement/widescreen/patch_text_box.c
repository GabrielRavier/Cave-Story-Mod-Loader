// Graphics enhancement mod for Freeware Cave Story
// Copyright © 2017 Clownacy

#include "patch_text_box.h"

#include <windows.h>

#include "cave_story.h"
#include "mod_loader.h"

#include "../common.h"

void DrawHUD_Air_hijack(int x, int y)
{
	CS_DrawHUD_Air(((SCREEN_WIDTH - 320) / 2) + x, y);
}

void TextBoxDrawSpriteHijack(RECT* clip, int x, int y, RECT* src, int slot)
{
	CS_DrawSprite_WithTransparency(clip, ((SCREEN_WIDTH - 244) / 2) + (x - 38), y, src, slot);
}

void ClearScreenHijack(RECT* rect, int flags)
{
	rect->left += (SCREEN_WIDTH - 320) / 2;
	rect->right += (SCREEN_WIDTH - 320) / 2;

	CS_DrawColourFill(rect, flags);
}

void PatchTextBox(void)
{
//	ModLoader_WriteByte((void*)0x41A350, 0xC3);
	ModLoader_WriteRelativeAddress((void*)0x42201A + 1, TextBoxDrawSpriteHijack);
	ModLoader_WriteRelativeAddress((void*)0x422058 + 1, TextBoxDrawSpriteHijack);
	ModLoader_WriteRelativeAddress((void*)0x42207F + 1, TextBoxDrawSpriteHijack);
	ModLoader_WriteRelativeAddress((void*)0x422118 + 1, TextBoxDrawSpriteHijack);
	ModLoader_WriteRelativeAddress((void*)0x422180 + 1, TextBoxDrawSpriteHijack);
	ModLoader_WriteRelativeAddress((void*)0x4222C6 + 1, TextBoxDrawSpriteHijack);
	ModLoader_WriteRelativeAddress((void*)0x4222E0 + 1, TextBoxDrawSpriteHijack);
	ModLoader_WriteRelativeAddress((void*)0x422300 + 1, TextBoxDrawSpriteHijack);
	ModLoader_WriteRelativeAddress((void*)0x42231D + 1, TextBoxDrawSpriteHijack);
	ModLoader_WriteRelativeAddress((void*)0x42233A + 1, TextBoxDrawSpriteHijack);
	ModLoader_WriteRelativeAddress((void*)0x422357 + 1, TextBoxDrawSpriteHijack);
	ModLoader_WriteRelativeAddress((void*)0x4223DB + 1, TextBoxDrawSpriteHijack);
	ModLoader_WriteRelativeAddress((void*)0x422446 + 1, TextBoxDrawSpriteHijack);
	ModLoader_WriteRelativeAddress((void*)0x4224CD + 1, TextBoxDrawSpriteHijack);
	ModLoader_WriteRelativeAddress((void*)0x4224FE + 1, TextBoxDrawSpriteHijack);
	ModLoader_WriteRelativeAddress((void*)0x42220D + 1, ClearScreenHijack);
	ModLoader_WriteRelativeAddress((void*)0x41084E + 1, DrawHUD_Air_hijack);

	ModLoader_WriteLong((void*)0x4219F9 + 6, ((SCREEN_WIDTH - 244) / 2) + 14);
	ModLoader_WriteLong((void*)0x421A0D + 6, SCREEN_WIDTH - (((SCREEN_WIDTH - 244) / 2) + 14));
}
