#pragma once

// UnkTU2744: functions of this translation unit that other files call (tools/tu_map.py unit 2744,
// 0x71011b020c-0x71011be0d0). One declaration per symbol: include this instead of declaring one.
// Names and layouts are placeholders.
#include "game/UnkGuiDisplayObject.h"
#include "base/global/CStrId.h"
#include "base/global/CRntString.h"
#include "base/reflection/CClass.h"
#include "types.h"

struct UnkWeakTarget;
struct UnkButtonTemplate_ovb0108;
struct UnkGuiRoot;
struct UnkLabelTextObject;
void fn_71011b6b5c(UnkGuiRoot* root, u32 value);
void fn_71011bab80(UnkGuiRoot* root, void* object, s32 frame);
void fn_71011b3768(UnkGuiRoot* root);
void fn_71011bab10(UnkGuiRoot* root, UnkLabelTextObject** out, s32 frame);
void fn_71011baba0(UnkGuiRoot* root, void* object, s32 frame);
void fn_71011b5598(UnkGuiRoot* root, UnkGuiDisplayObject* object);
void* fn_71011b882c(UnkGuiRoot*, const base::global::CStrId*);
UnkWeakTarget* fn_71011b5f5c(u32 script, const char* const* name, const base::global::CRntString* source, void* bag);
UnkGuiDisplayObject* fn_71011b63c4(UnkGuiRoot* root, UnkButtonTemplate_ovb0108* def);
const base::global::CStrId& fn_71011b8920(UnkGuiRoot* root);
const base::reflection::CType* fn_71011bcf1c();
