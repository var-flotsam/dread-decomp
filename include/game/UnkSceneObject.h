#pragma once

// A scene object as the hand-written callers see it: what the render manager's pool creates as an
// effect (fn_7100149930, game/UnkRenderManager.h), the minimap's camera and its two draw-pass
// objects (src/game/CMinimapManager.cpp). The functions of its unit (tools/tu_map.py unit 140,
// 0x710014803c-0x7100149bec, no reflected class) take it first: fn_7100148160 and fn_7100148178
// hand it draw lists, fn_710014826c and fn_7100148600 restart it with a mode and colours, and
// fn_7100149490 switches it on or off. The member at +0x20 gets the on/off flag (fn_71001fb1b8)
// and, for the minimap's camera, the view (fn_7100205f98). The sub-object at +0x130 belongs to
// unit 252 (0x71001bdeac-0x71001c071c): fn_71001be020 and fn_71001be430 set its slots. Only the
// members the callers touch are spelled out; the names and the layout are placeholders.

#include "types.h"

// The object behind +0x170 of the sub-object: a dirty flag the minimap sets after it moves its
// camera (fn_7100e90e6c).
struct UnkSceneObjectState {
    u8 pad_0x0[0xb0];
    bool dirty;
};

// The sub-object at +0x130: two scales the minimap reads for its view (+0xc4 and +0xcc), a vector
// of slots at +0xf0 (its count at +0xf8; fn_71001be430) and the state at +0x170.
struct UnkSceneObjectSub {
    u8 pad_0x0[0xc4];
    f32 field_0xc4;
    u8 pad_0xc8[0x4];
    f32 field_0xcc;
    u8 pad_0xd0[0xa0];
    UnkSceneObjectState* flags;
};
static_assert(__builtin_offsetof(UnkSceneObjectSub, flags) == 0x170, "UnkSceneObjectSub placeholder layout");

// Switches the slot `index` of the sub-object on or off (the flag at +8 of its entry), when the
// index is below 5 and below the slot count. Not defined by hand.
void fn_71001be430(UnkSceneObjectSub* sub, u32 index, bool enabled);

struct UnkSceneObject {
    u8 pad_0x0[0x20];
    u8 field_0x20[0x110];
    UnkSceneObjectSub* field_0x130;
};
static_assert(__builtin_offsetof(UnkSceneObject, field_0x130) == 0x130, "UnkSceneObject placeholder layout");

// A draw list as the slots take it: a pointer and a count.
struct UnkSceneSpan {
    void* data;
    u64 count;
};

// Hands a draw list to slot `slot` of the sub-object; false for a slot past the fifth
// (src/game/UnkSceneObject.cpp).
bool fn_7100148178(UnkSceneObject* self, u32 slot, const UnkSceneSpan& list, u32 flag);

// Switches the object on or off when the flag at +0x118 differs from `enabled`: the member at
// +0x20 gets the flag, and switching on also runs fn_7100025a88 on +0x138. Defined in src/clones/
// (base::refl_SetFlagCall, base/UnkCloneCalls.h).
void fn_7100149490(UnkSceneObject* self, bool enabled);
