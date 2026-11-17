// CNavMeshItemComponent unit (tools/tu_map.py): after the base step, registers the component with the game
// manager and, when it has no current source yet, selects one: the source id stored in the current
// scenario's blackboard section under the owner's name (the default id when none is stored), as the
// reflected setter src/game/CNavMeshItemComponent/fn_71009c2d74.cpp selects by id. Names and layouts are
// placeholders.
#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "game/CComponent.h"
#include "game/CNavMeshItemComponent.h"
#include "game/CScenario.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkSubId_7100fc4b34.h"
#include "types.h"

using base::global::CStrId;

extern u64 g_unk_717273ee08 __attribute__((visibility("hidden")));

struct UnkNavMeshNamedDef_b49 : UnkIdSource {
    u64 field_0x8;
    UnkQualifiedName name;
};

static inline UnkSubId_7100fc4b34 SubId(UnkNavMeshNamedDef_b49& def, const CStrId& name) {
    if (!def.name.str || !def.name.str[0])
        fn_7100ae9d7c(&def.name, &def, g_unk_7101d470e0);
    return fn_71003327a4(&def.name, &name);
}

struct UnkSourceDesc {
    u8 pad_0x0[0x30];
    u64 id;
};

struct UnkSourceList {
    u8 pad_0x0[0x8];
    u32 count;
};

// A listed source: its descriptor holds the id it is selected by.
struct UnkNavMeshSource_l9bfae8 {
    u8 pad_0x0[0x40];
    UnkSourceDesc* desc;
};

struct UnkNavMeshItemOwner_l9bfae8 {
    u8 pad_0x0[0x30];
    CStrId name;
};

// The component's definition: the default source id.
struct UnkNavMeshItemDef_l9bfae8 {
    u8 pad_0x0[0x58];
    CStrId source_id;
};

struct UnkNavMeshItem_l9bfae8 {
    u8 pad_0x0[0x18];
    UnkNavMeshItemOwner_l9bfae8* owner;
    u8 pad_0x20[0x128];
    UnkSourceList sources;
    u8 pad_0x154[0x2c];
    UnkNavMeshSource_l9bfae8* current;
    u8 pad_0x188[0x28];
    UnkNavMeshItemDef_l9bfae8* def;
};
static_assert(__builtin_offsetof(UnkNavMeshItem_l9bfae8, sources) == 0x148, "layout");
static_assert(__builtin_offsetof(UnkNavMeshItem_l9bfae8, current) == 0x180, "layout");
static_assert(__builtin_offsetof(UnkNavMeshItem_l9bfae8, def) == 0x1b0, "layout");

void fn_71009bfae8(UnkNavMeshItem_l9bfae8* self) {
    fn_71010c4228(reinterpret_cast<UnkParent*>(self));
    fn_7100de9540(g_unk_7101d47310, &g_unk_717273ee08, reinterpret_cast<UnkWeakTarget*>(self));
    if (self->current)
        return;
    CStrId source_id = self->def->source_id;
    UnkGameManager* manager = g_unk_7101d47310;
    if (UnkGameManagerSub_0x608* scenario = manager->field_0x608) {
        CBlackboard* blackboard = &manager->field_0x968;
        CStrId section = fn_71011a2fbc(scenario);
        UnkSubId_7100fc4b34 key = SubId(g_unk_717273e3e0, self->owner->name);
        if (CBlackboard::CSection* s = fn_7101047270(blackboard, section))
            fn_710051d464(s, &key, &source_id);
    }
    if (source_id.data != &g_unk_7101d47108->empty) {
        u64 id = source_id.data->Key();
        if (!self->current || self->current->desc->id != id) {
            UnkNavMeshSource_l9bfae8* found = nullptr;
            if (id != u64(-1)) {
                for (u32 i = 0; i < self->sources.count; ++i) {
                    auto* src = reinterpret_cast<UnkNavMeshSource_l9bfae8*>(fn_71009c0bf0(&self->sources, i));
                    if (src && src->desc->id == id) {
                        found = src;
                        break;
                    }
                }
            }
            fn_71009bfda8(reinterpret_cast<UnkOwner*>(self), reinterpret_cast<UnkSource*>(found));
        }
    }
}
