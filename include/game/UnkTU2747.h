#pragma once

// UnkTU2747: functions and globals of this translation unit that other files use (tools/tu_map.py unit 2747,
// 0x71011c198c-0x71011d6a04). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "game/UnkGuiDisplayObject.h"
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "types.h"

struct UnkSprite_spr;
struct UnkChangeMenuStateButton_b48a3;
struct UnkGallery_A5_b75;
struct UnkGuiButtonFocus_ovb0000;
struct UnkGuiEvent_b48a3;
void fn_71011cf364(void* composition, s32 value);
void fn_71011cd4b4(void* self);  // The display object's set-up (derived classes call it from their constructors).
void fn_71011ccfb4(char*);  // The display object's destructor.
void fn_71011cef74(void* composition, void* owner);
void fn_71011cf39c(UnkChangeMenuStateButton_b48a3* self, UnkGuiEvent_b48a3* event);
void fn_71011cf630(UnkGuiButtonFocus_ovb0000* self);
void fn_71011cf70c(UnkGallery_A5_b75* self);
void fn_71011cff5c(void* a, void* b, void* c, bool d, bool e, bool f);
extern char g_unk_7101cb5b90[];  // GUI::CDisplayObject's vtable.
bool fn_71011ceac0(UnkGuiDisplayObject* self);
bool fn_71011ceb34(UnkGuiDisplayObject* self);
void fn_71011cf8d0(UnkSprite_spr* self, const base::math::CVector2D* size);  // Sets the sprite's size.
void fn_71011cee40(UnkGuiDisplayObject* object);
void fn_71011cd424(UnkGuiDisplayObject* self, UnkGuiDisplayObject* parent);
void fn_71011cc940(UnkGuiDisplayObject* self, f32 value);
void fn_71011cca3c(UnkGuiDisplayObject* self, f32 value);
void fn_71011ccd20(UnkGuiDisplayObject* self, f32 value);
void fn_71011cdfa8(UnkGuiDisplayObject* self, f32 dt);
void fn_71011ce208(UnkGuiDisplayObject* self, f32 value);
void fn_71011cec68(UnkGuiDisplayObject* self, f32 value);
void fn_71011cee10(UnkGuiDisplayObject* self);
UnkVec2Result fn_71011cf12c(UnkGuiDisplayObject* self, const UnkVec2Result& point);
