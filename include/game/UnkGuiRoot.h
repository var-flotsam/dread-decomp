#pragma once

// The GUI root, the global g_unk_717271e8f8: the registry the menus look compositions up in
// (game/UnkGuiHandle.h), the menu container at +0x68 the screens add their compositions to, the
// owner of the focus (a reference to the focused object at +0x230), and a list of keys at +0xca8
// the menus register with. tools/tu_map.py puts its functions in the unit of
// gui/scripts/guimanager.lua. The name and the layout are placeholders.

#include "game/UnkGuiDisplayObject.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

// A key the menus register with the root (fn_710002ffb4 adds one, fn_710002fd10 sets its value to
// -1.0f). Only the pointer to it is passed around; the keys are globals (g_unk_7172762fd8, ...).
struct UnkGuiKey;

// The root's list of keys at +0xca8 (a linked list of nodes: key, value, next), as far as the two
// functions below use it: the count at +8, the head at +0x10 and tail at +0x18, and the allocator
// at +0x20. A callback set follows at +0x30 (notified when the list becomes empty or not).
struct UnkGuiKeyList {
    u8 unk_0x0[8];
    s32 count;
    u8 unk_0xc[4];
    void* head;
    void* tail;
    void* allocator;
    u8 unk_0x28[8];
};

struct UnkGuiRoot {
    u8 unk_0x0[0x68];
    UnkGuiDisplayObject* menu;
    u8 unk_0x70[0x1c0];
    UnkWeakHandle* field_0x230;  // the focus: the weak handle of the focused object (its target is at +0x10)
    UnkWeakHandle* field_0x238;  // the handle of the object fn_71011b6bbc checks the new focus against
    u8 unk_0x240[0xa68];
    UnkGuiKeyList field_0xca8;
};
static_assert(__builtin_offsetof(UnkGuiRoot, menu) == 0x68, "UnkGuiRoot placeholder layout");
static_assert(__builtin_offsetof(UnkGuiRoot, field_0x230) == 0x230, "UnkGuiRoot placeholder layout");
static_assert(__builtin_offsetof(UnkGuiRoot, field_0x238) == 0x238, "UnkGuiRoot placeholder layout");
static_assert(__builtin_offsetof(UnkGuiRoot, field_0xca8) == 0xca8, "UnkGuiRoot placeholder layout");

extern UnkGuiRoot* g_unk_717271e8f8;

// Makes `object` the root's focused object (the reference at +0x230). The previous one gets a
// focus-lost event (message kind 3); the new one a focus-gained event (kind 2), and `value` is the
// bool that event carries as a property (the menus pass false). Defined in the unit's generated code.
void fn_71011b6bbc(UnkGuiRoot* root, UnkGuiDisplayObject* object, bool value);

// Sends a message (a 0x28-byte event built by fn_710120235c) to the root. Defined in the unit's
// generated code; callers pass their own message type.
void fn_71011b6e0c(UnkGuiRoot* root, void* message);

// Tells the root that `object` gained (fn_71011b7514) or lost (fn_71011b5d94) the property
// game/UnkGuiDisplayObject.h describes. Defined in the unit's generated code.
void fn_71011b7514(UnkGuiRoot* root, UnkGuiDisplayObject* object);
void fn_71011b5d94(UnkGuiRoot* root, UnkGuiDisplayObject* object);

// Tells the root about an object (the render manager's menu path). Defined in the unit's generated code.
void fn_71011b57c4(UnkGuiRoot* root, void* object);

// Registers `key` with the root's list (fn_710002ffb4, which also notifies the list's callback set
// when it becomes non-empty) and sets a registered key's value to -1.0f (fn_710002fd10). Not
// decompiled yet.
void fn_710002ffb4(UnkGuiKeyList* list, UnkGuiKey* key);
void fn_710002fd10(UnkGuiKeyList* list, UnkGuiKey* key);
