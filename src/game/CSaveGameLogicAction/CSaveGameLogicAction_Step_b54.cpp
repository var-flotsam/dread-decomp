// CSaveGameLogicAction unit (tools/tu_map.py, the name from reflection): the action's step
// (fn_7100a41680). After the base step (fn_7100a55ac8), unless the mode flag (+0xb8 of g_unk_71726bc570)
// is set, the action's string id (+0x18) is empty or the game manager refuses (fn_7100dfbe3c), it hands
// the game manager the id's text and the name of the actor its lazily resolved reference names (source
// +0x28, state +0x50, handle +0x58; empty without one), both as strings, and the flag at +0x60: in mode 1
// (+0x10) through fn_7100dfaf88, in mode 0 through fn_7100dfd174 with 1 and an empty pair. Same reads as
// src/game/CSaveSnapshotToCheckpointLogicAction/CSaveSnapshotToCheckpointLogicAction_Step_b53.cpp.
// Names and layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/CTriggerLogicAction.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CRntString;

struct UnkTutoExitAction_b47;

struct UnkModeFlags {
    u8 unk_0x0[0xb8];
    bool field_0xb8;
};
extern UnkModeFlags* g_unk_71726bc570;

bool fn_7100dfaf88(UnkGameManager* self, void* name, const CRntString& tag, bool flag);

struct UnkSaveGamePair_b54 {
    u64 first;
    u64 second;
};
void fn_7100dfd174(UnkGameManager* self, CRntString* name, CRntString* tag, bool flag, u32 kind,
                   const UnkSaveGamePair_b54& extra);

// A string-id entry as read here: the text pointer at +0x10, a use count at +0x44 (a union member:
// the original reloads the id after the increment).
struct UnkSaveGameStrEntry_b54 {
    u8 pad_0x0[0x10];
    const char* text;
    u8 pad_0x18[0x2c];
    union {
        u32 uses;
    };
};

static inline __attribute__((always_inline)) const char* Text(UnkSaveGameStrEntry_b54* const& id) {
    ++id->uses;
    const char* t = id->text;
    return t ? t : "";
}

struct UnkSaveGameActor_b54 {
    u8 pad_0x0[0x30];
    UnkSaveGameStrEntry_b54* name;
};

struct UnkSaveGameAction_b54 {
    void* vtable;
    u8 pad_0x8[0x8];
    u32 mode;                        // 0x10
    u8 pad_0x14[4];
    UnkSaveGameStrEntry_b54* id;     // 0x18
    void* list_vtable;               // 0x20
    u8 source[0x28];                 // 0x28
    u32 state;                       // 0x50
    u8 pad_0x54[4];
    base::UnkRefHandle handle;  // 0x58
    bool flag;                  // 0x60

    __attribute__((always_inline)) const char* TargetName() {
        if (state == 1) {
            {
                UnkRefPtr fresh = fn_710023b580(source);
                fn_7100234f78(&handle, reinterpret_cast<const base::UnkRefHandle*>(&fresh));
            }
            state = 2;
        }
        if (base::UnkRefCounted* target = handle.ptr) {
            auto* actor = *reinterpret_cast<UnkSaveGameActor_b54**>(reinterpret_cast<char*>(target) + 0x10);
            if (actor)
                return Text(actor->name);
        }
        return "";
    }
};

void fn_7100a41680(UnkSaveGameAction_b54* self) {
    fn_7100a55ac8(reinterpret_cast<UnkTutoExitAction_b47*>(self));
    if (g_unk_71726bc570->field_0xb8)
        return;
    if (reinterpret_cast<base::global::UnkStringData*>(self->id) == &g_unk_7101d47108->empty)
        return;
    if (!fn_7100dfbe3c(g_unk_7101d47310))
        return;
    if (self->mode == 1) {
        UnkGameManager* manager = g_unk_7101d47310;
        CRntString name(Text(self->id));
        CRntString target(self->TargetName());
        fn_7100dfaf88(manager, &name, target, self->flag);
    } else if (self->mode == 0) {
        UnkGameManager* manager = g_unk_7101d47310;
        CRntString name(Text(self->id));
        CRntString target(self->TargetName());
        fn_7100dfd174(manager, &name, &target, self->flag, 1, UnkSaveGamePair_b54{});
    }
}
