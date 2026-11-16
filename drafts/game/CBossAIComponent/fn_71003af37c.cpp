// CBossAIComponent's unit (tools/tu_map.py): works out which boss (or boss variant) the owner's AI
// component (+0x30, +0x240) is, from its reflected class and the given id; -1 when none.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkManager_71726af468.h"
#include "game/UnkTU2704.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::CClass;

__attribute__((visibility("hidden"))) extern CStrId g_unk_71726bc5c0;
__attribute__((visibility("hidden"))) extern CStrId g_unk_71726bc5d8;
__attribute__((visibility("hidden"))) extern CStrId g_unk_71726bc5e0;
__attribute__((visibility("hidden"))) extern CStrId g_unk_71726bc5e8;
__attribute__((visibility("hidden"))) extern CStrId g_unk_71726bc5f0;
__attribute__((visibility("hidden"))) extern CStrId g_unk_71726bc5f8;
__attribute__((visibility("hidden"))) extern CStrId g_unk_71726bc608;

bool fn_7100475530(void* registry);

struct UnkBossActor_l3af37c {
    void** vtable;
    const CClass* vf_0x0() { return reinterpret_cast<const CClass* (*)(UnkBossActor_l3af37c*)>(vtable[0])(this); }
};

struct UnkBossLink_l3af37c {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

struct UnkBossOwner_l3af37c {
    u8 pad_0x0[0x240];
    UnkBossActor_l3af37c* field_0x240;
    u8 pad_0x248[0x398 - 0x248];
    UnkBossLink_l3af37c* field_0x398;
};

struct UnkBossAI_l3af37c {
    u8 pad_0x0[0x30];
    UnkBossOwner_l3af37c* owner;
};

template <const CClass* (*GetClass)()>
inline bool IsA_l3af37c(UnkBossActor_l3af37c* actor) {
    return actor && fn_710009d50c(actor->vf_0x0(), GetClass());
}

template <const CClass* (*GetClass)()>
inline UnkBossActor_l3af37c* As_l3af37c(UnkBossActor_l3af37c* actor) {
    return IsA_l3af37c<GetClass>(actor) ? actor : nullptr;
}

static inline UnkManager_71726af468* Manager_l3af37c() {
    if (!g_unk_71726af468) {
        UnkManager_71726af468* m = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
        fn_7100da7e2c(m);
        g_unk_71726af468 = m;
    }
    return g_unk_71726af468;
}

// The manager's member at +0x228 (UnkManager_71726af468 does not name it yet).
static inline bool ManagerCheck_l3af37c() {
    return fn_7100475530(reinterpret_cast<u8*>(Manager_l3af37c()) + 0x228);
}

s64 fn_71003af37c(UnkBossAI_l3af37c* self, const CStrId& id, bool any) {
    s32 kind = -1;
    if (IsA_l3af37c<fn_7100690c74>(self->owner->field_0x240))  // CScorpiusAIComponent
        kind = 0;
    if (IsA_l3af37c<fn_71005fac74>(self->owner->field_0x240))  // CKraidAIComponent
        kind = 1;
    if (IsA_l3af37c<fn_71005ba098>(self->owner->field_0x240))  // CHydrogigaAIComponent
        kind = 2;
    if (IsA_l3af37c<fn_71004cf318>(self->owner->field_0x240))  // CCooldownXBossAIComponent
        kind = 3;
    if (IsA_l3af37c<fn_710044e708>(self->owner->field_0x240))  // CChozoCommanderAIComponent
        kind = 4;

    UnkBossActor_l3af37c* warriorX = As_l3af37c<fn_710048db44>(self->owner->field_0x240);
    UnkBossActor_l3af37c* warrior = As_l3af37c<fn_7100497f2c>(self->owner->field_0x240);
    if (any ? warriorX || warrior : warriorX != nullptr) {
        if (id == g_unk_71726bc5e0)
            kind = 6;
        else if (id == g_unk_71726bc5e8)
            kind = 7;
        else if (id == g_unk_71726bc5c0)
            kind = 8;
        else if (id == g_unk_71726bc5f8)
            kind = 9;
    }

    UnkBossActor_l3af37c* eliteX = As_l3af37c<fn_710047aa94>(self->owner->field_0x240);
    UnkBossActor_l3af37c* elite = As_l3af37c<fn_710047a224>(self->owner->field_0x240);
    if (any ? eliteX || elite : eliteX != nullptr)
        kind = 5;

    if (IsA_l3af37c<fn_710046ea2c>(self->owner->field_0x240)) {  // CChozoRobotSoldierAIComponent
        if (id == g_unk_71726bc5e0) {
            kind = 10;
        } else if (id == g_unk_71726bc5f0) {
            UnkBossLink_l3af37c* link = self->owner->field_0x398;
            if (!(*fn_710112bf20(link ? link->field_0x10 : nullptr) == g_unk_71726bc608))
                kind = 11;
            else if (any || !ManagerCheck_l3af37c())
                kind = 12;
        } else if (id == g_unk_71726bc5d8) {
            if (any || !ManagerCheck_l3af37c())
                kind = 13;
        }
    }

    if (IsA_l3af37c<fn_710063efe4>(self->owner->field_0x240))  // CQuetzoaXAIComponent
        kind = 14;
    if (IsA_l3af37c<fn_7100587ee4>(self->owner->field_0x240))  // CGoliathXAIComponent
        kind = 15;
    return kind;
}
