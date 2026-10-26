#pragma once

// A GUI display object as the hand-written menu and screen sources (game_aca57c, game_s7) and the
// flag setters of its own unit see it (tools/tu_map.py puts fn_71011cdce8 and fn_71011cf2a4 in the
// unit it names after GUI::CDisplayObject): the class getter in vtable slot 0, the flags the
// setters keep at +0x74..+0x76 and +0x181..+0x184, and the name the game_s7 screens pass on at
// +0x190. Only the members the callers share are spelled out; the names and the layout are
// placeholders.

#include "types.h"

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
    u8 pad_0x185[0xb];
    const char* field_0x190;  // a name the game_s7 screens look children up by (fn_71011d9508)
};
static_assert(__builtin_offsetof(UnkGuiDisplayObject, field_0x74) == 0x74, "UnkGuiDisplayObject placeholder layout");
static_assert(__builtin_offsetof(UnkGuiDisplayObject, field_0xb4) == 0xb4, "UnkGuiDisplayObject placeholder layout");
static_assert(__builtin_offsetof(UnkGuiDisplayObject, field_0x144) == 0x144, "UnkGuiDisplayObject placeholder layout");
static_assert(__builtin_offsetof(UnkGuiDisplayObject, field_0x181) == 0x181, "UnkGuiDisplayObject placeholder layout");
static_assert(__builtin_offsetof(UnkGuiDisplayObject, field_0x190) == 0x190, "UnkGuiDisplayObject placeholder layout");

// Sets the flag at +0x181 (the menus' focus handling, fn_7100b021fc, gives it to the newly focused
// object and clears it on the old one). When it changes while the object is shown, after stale
// derived flags are brought up to date, and the object is active, the GUI root g_unk_717271e8f8
// is told that the object gained (fn_71011b7514) or lost (fn_71011b5d94) it. Defined in
// src/game_hi/fn_71011cdce8.cpp, the unit's file.
void fn_71011cf2a4(UnkGuiDisplayObject* self, bool value);

// Sets the flag at +0xb4: marks the object stale and, if its derived visibility changed, sends a
// change event to the GUI root. Defined in src/game_hi/fn_71011cdce8.cpp.
void fn_71011cdce8(UnkGuiDisplayObject* self, bool value);
