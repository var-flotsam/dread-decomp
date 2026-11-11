// CCharClassHideCutsceneActorEvent's unit (tools/tu_map.py): hides a cutscene actor. With an
// actor name (+0x10) the request goes to the cutscene manager (fn_71011986c8 on
// g_unk_7101d47330); without one the event's own entity is hidden (fn_71002334fc) and, when the
// flag at +0x18 is set, the entries of the object at +0x50 of its component (+0x228) are reset.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkTU253.h"
#include "game/UnkTU341.h"
#include "types.h"

using base::global::CStrId;

struct UnkStruct_71002334fc;
struct UnkResetOwner;

extern void* g_unk_7101d47330;
void fn_71011986c8(void* manager, const CStrId* actor, bool visible, bool flag);

struct UnkCutsceneActorState_ovb0062 {
    u8 unk_0x0[0x138];
    UnkResetOwner* entries;  // 0x138
};

struct UnkCutsceneActorComponent_ovb0062 {
    u8 unk_0x0[0x50];
    UnkCutsceneActorState_ovb0062* state;  // 0x50
};

struct UnkCutsceneActorEntity_ovb0062 {
    u8 unk_0x0[0x228];
    UnkCutsceneActorComponent_ovb0062* component;  // 0x228
};

struct UnkCutsceneActorEvent_ovb0062 {
    u8 unk_0x0[0x10];
    CStrId actor;  // 0x10
    bool flag;     // 0x18
};

void fn_7100cc2fc8(UnkCutsceneActorEvent_ovb0062* self, UnkCutsceneActorEntity_ovb0062** owner) {
    if (!*owner)
        return;
    if (self->actor.data != &g_unk_7101d47108->empty) {
        fn_71011986c8(g_unk_7101d47330, &self->actor, false, self->flag);
        return;
    }
    fn_71002334fc(reinterpret_cast<UnkStruct_71002334fc*>(*owner), false);
    if (self->flag) {
        if (UnkCutsceneActorState_ovb0062* state = (*owner)->component->state)
            fn_71001c3270(state->entries);
    }
}
