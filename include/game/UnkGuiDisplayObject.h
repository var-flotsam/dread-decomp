#pragma once

// A GUI display object as the hand-written menu and screen sources (game_aca57c, game_s7) and the
// flag setters of its own unit see it (tools/tu_map.py puts fn_71011cdce8 and fn_71011cf2a4 in the
// unit it names after GUI::CDisplayObject): the class getter in vtable slot 0, the flags the
// setters keep at +0x74..+0x76 and +0x181..+0x184, and the name the game_s7 screens pass on at
// +0x190. Only the members the callers share are spelled out; the names and the layout are
// placeholders.

#include "types.h"

namespace base::math {
struct CVector2D;  // base/math/CVector.h
}
namespace base::reflection {
class CClass;
}

struct UnkGuiDisplayObject {
    virtual const base::reflection::CClass* GetClass();  // vf_0x0: the reflected class
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();  // fn_71011cdce8 calls it the first time its flag changes

    u8 pad_0x8[0x6c];
    bool field_0x74;  // active: the flags below take effect only while it is set
    u8 pad_0x75;
    bool field_0x76;  // shown
    u8 pad_0x77[0x3d];
    bool field_0xb4;  // the flag fn_71011cdce8 sets
    u8 pad_0xb5[0x8f];
    u32 field_0x144;  // change bits (fn_71011cdce8 sets 0x40)
    u8 pad_0x148[0x39];
    bool field_0x181;  // the flag fn_71011cf2a4 sets
    u8 pad_0x182;
    bool field_0x183;  // the derived flags are stale: fn_71011cee40 brings them up to date
    bool field_0x184;  // vf_0xa0 has run
    bool field_0x185;  // the flag fn_71011d0088 sets
    u8 pad_0x186[0xa];
    const char* field_0x190;  // a name the game_s7 screens look children up by (fn_71011d9508)
};
static_assert(__builtin_offsetof(UnkGuiDisplayObject, field_0x74) == 0x74, "UnkGuiDisplayObject placeholder layout");
static_assert(__builtin_offsetof(UnkGuiDisplayObject, field_0xb4) == 0xb4, "UnkGuiDisplayObject placeholder layout");
static_assert(__builtin_offsetof(UnkGuiDisplayObject, field_0x144) == 0x144, "UnkGuiDisplayObject placeholder layout");
static_assert(__builtin_offsetof(UnkGuiDisplayObject, field_0x181) == 0x181, "UnkGuiDisplayObject placeholder layout");
static_assert(__builtin_offsetof(UnkGuiDisplayObject, field_0x190) == 0x190, "UnkGuiDisplayObject placeholder layout");

// The object's derived state, 0x38 bytes into it, as fn_71011cde14 returns it; the offsets are from
// there, so +0x3c and +0x3e are the object's +0x74 (active) and +0x76 (shown) above.
struct UnkGuiObjectState {
    u8 unk_0x0[0x18];
    f32 field_0x18;  // with +0x20, the two factors of a list's scroll step (fn_71011ff97c)
    u8 unk_0x1c[0x4];
    f32 field_0x20;
    u8 unk_0x24[0x14];
    s32 frame;        // the frame the label's refresh works in (0x7fffffff when none)
    bool field_0x3c;  // tested by the root's update and the screens
    u8 unk_0x3d;
    bool field_0x3e;  // the object can take the focus
};
static_assert(__builtin_offsetof(UnkGuiObjectState, field_0x18) == 0x18, "UnkGuiObjectState placeholder layout");
static_assert(__builtin_offsetof(UnkGuiObjectState, frame) == 0x38, "UnkGuiObjectState placeholder layout");
static_assert(__builtin_offsetof(UnkGuiObjectState, field_0x3e) == 0x3e, "UnkGuiObjectState placeholder layout");

// The object's derived state (its +0x38), first brought up to date (fn_71011cee40) when +0x183 says
// it is stale. Defined in the unit's generated code.
UnkGuiObjectState* fn_71011cde14(UnkGuiDisplayObject* self);

// Sets the flag at +0xb5 (the callers pass whether the object is to be seen). On a change, the first
// change runs vf_0xa0 and sets +0x184, the derived flags go stale (+0x183) and change bit 0x80 is
// set (+0x144). Defined in the unit's generated code.
void fn_71011cdc30(UnkGuiDisplayObject* self, bool value);

// Sets the flag at +0x185, as fn_71011cdc30 (change bit 4; src/game/GUI/CDisplayObject.cpp).
void fn_71011d0088(UnkGuiDisplayObject* self, bool value);

// Places the object: its position (+0x80) and the two modes it is placed by (+0x78 and +0x7c: the map
// markers pass the side each coordinate is measured from). On a change the derived flags go stale
// (src/game/GUI/CDisplayObject.cpp).
void fn_71011cde64(UnkGuiDisplayObject* self, const base::math::CVector2D* position, u32 h_mode, u32 v_mode);

// Sets the flag at +0x181 (the menus' focus handling, fn_7100b021fc, gives it to the newly focused
// object and clears it on the old one). When it changes while the object is shown, after stale
// derived flags are brought up to date, and the object is active, the GUI root g_unk_717271e8f8
// is told that the object gained (fn_71011b7514) or lost (fn_71011b5d94) it. Defined in
// src/game_hi/fn_71011cdce8.cpp, the unit's file.
void fn_71011cf2a4(UnkGuiDisplayObject* self, bool value);

// Sets the flag at +0xb4: marks the object stale and, if its derived visibility changed, sends a
// change event to the GUI root. Defined in src/game_hi/fn_71011cdce8.cpp.
void fn_71011cdce8(UnkGuiDisplayObject* self, bool value);

// Sets a 32-bit value on the object (the menus' setup passes 2 before adding it to its menu). Defined
// in the unit's generated code.
void fn_71011cde44(UnkGuiDisplayObject* self, s32 value);

// The destructor of the display-object container (the unit named after GUI::CDisplayObjectContainer).
// Not decompiled yet.
void fn_71011d8e90(UnkGuiDisplayObject* self);

// Adds `child` to the container's list at +0x1c8; the first time, vf_0xa0 runs and +0x184 is set,
// as for the flags above. Returns whether it was added. Not decompiled yet.
bool fn_71011d968c(UnkGuiDisplayObject* container, UnkGuiDisplayObject* child);

// The child of the container whose name hash is `id`, from the list at +0x1d0 (searching the child
// containers too), or null (src/game/GUI/CDisplayObjectContainer/fn_71011d9508.cpp).
UnkGuiDisplayObject* fn_71011d98ec(UnkGuiDisplayObject* container, u64 id);

// GUI::CDisplayObject's constructor: the base's, the vtable, two empty ids, the display state and its
// derived copy (unit sizes and scale, white, active and shown), the render slots at +0xb8, an empty
// id at +0x130 and the name at +0x150 (src/game/GUI/CDisplayObject.cpp). The other display objects'
// constructors start with it.
void fn_71011cce1c(UnkGuiDisplayObject* self);

namespace base::global {
class CStrId;
}
// The container's direct child named `name`, from the list at +0x1d0, or null
// (src/game/GUI/CDisplayObjectContainer/fn_71011d9508.cpp).
UnkGuiDisplayObject* fn_71011d94d4(UnkGuiDisplayObject* container, const base::global::CStrId& name);
