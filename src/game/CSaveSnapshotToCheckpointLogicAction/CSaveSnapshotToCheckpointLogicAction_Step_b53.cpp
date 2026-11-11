// CSaveSnapshotToCheckpointLogicAction unit (tools/tu_map.py, the name from reflection): the action's
// step. After the base step (fn_7100a55ac8) it reads the texts of its two string ids (+0x10, +0x18; each
// read bumps the entry's use count at +0x44), resolves its lazily resolved actor reference (source
// +0x28, state +0x50, handle +0x58) and hands the game manager the two texts, the actor's name (its +0x30
// id; empty without an actor) as a string and the flag at +0x60 (fn_7100dfc344). Names and layouts are
// placeholders.
#include "base/UnkRefHandle.h"
#include "base/global/CRntString.h"
#include "game/CTriggerLogicAction.h"
#include "game/UnkGameManager.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkTutoExitAction_b47;

// A string-id entry as read here: the text pointer at +0x10, a use count at +0x44 (a union member:
// the original reloads the id after the increment).
struct UnkSnapshotStrEntry_b53 {
    u8 pad_0x0[0x10];
    const char* text;
    u8 pad_0x18[0x2c];
    union {
        u32 uses;
    };
};

static inline __attribute__((always_inline)) const char* const& Text(UnkSnapshotStrEntry_b53* const& id) {
    ++id->uses;
    return id->text;
}

bool fn_7100dfc344(UnkGameManager* manager, const char* const& a, const char* const& b,
                   const base::global::CRntString& name, bool flag);

struct UnkSnapshotActor_b53 {
    u8 pad_0x0[0x30];
    UnkSnapshotStrEntry_b53* name;
};

struct UnkSaveSnapshotAction_b53 {
    void* vtable;
    u8 pad_0x8[0x8];
    UnkSnapshotStrEntry_b53* first;   // 0x10
    UnkSnapshotStrEntry_b53* second;  // 0x18
    void* list_vtable;                // 0x20
    u8 source[0x28];                  // 0x28
    u32 state;                        // 0x50
    u8 pad_0x54[4];
    base::UnkRefHandle handle;  // 0x58
    bool flag;                  // 0x60
};

void fn_7100a42b48(UnkSaveSnapshotAction_b53* self) {
    fn_7100a55ac8(reinterpret_cast<UnkTutoExitAction_b47*>(self));
    UnkGameManager* manager = g_unk_7101d47310;
    const char* const& a = Text(self->first);
    const char* const& b = Text(self->second);
    if (self->state == 1) {
        {
            UnkRefPtr fresh = fn_710023b580(self->source);
            fn_7100234f78(&self->handle, reinterpret_cast<const base::UnkRefHandle*>(&fresh));
        }
        self->state = 2;
    }
    const char* text = "";
    if (base::UnkRefCounted* target = self->handle.ptr) {
        auto* actor = *reinterpret_cast<UnkSnapshotActor_b53**>(reinterpret_cast<char*>(target) + 0x10);
        if (actor) {
            const char* t = Text(actor->name);
            text = t ? t : "";
        }
    }
    base::global::CRntString name(text);
    fn_7100dfc344(manager, a, b, name, self->flag);
}
