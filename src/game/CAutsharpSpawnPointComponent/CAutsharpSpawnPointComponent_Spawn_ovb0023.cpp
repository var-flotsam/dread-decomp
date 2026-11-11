// CAutsharpSpawnPointComponent's unit (tools/tu_map.py). fn_7100a14190: after the base step
// (fn_710112b7c4), sends the owner's (+0x30) shape target (+0x228) the flag named for the spawn kind
// at +0x3b8 (0 or 1) and clears +0x400. fn_7100a144c8: when the entity's AI component (+0x240 of
// the components at +0x10 of +0x40) is a CAutsharpAIComponent, hands it a copy of the string id
// g_unk_7172744430 (fn_71003c91cc). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CModelUpdaterComponent.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkFlagTarget;

struct UnkAutsharpOwner_ovb0023 {
    u8 pad_0x0[0x228];
    UnkFlagTarget* field_0x228;
};

struct UnkAutsharpSpawnPoint_ovb0023 {
    u8 pad_0x0[0x30];
    UnkAutsharpOwner_ovb0023* owner;
    u8 pad_0x38[0x3b8 - 0x38];
    s32 field_0x3b8;
    u8 pad_0x3bc[0x400 - 0x3bc];
    bool field_0x400;
};

extern char g_unk_710164b4f8[];
extern char g_unk_710164b500[];

void fn_710112b7c4(UnkAutsharpSpawnPoint_ovb0023* self);

void fn_7100a14190(UnkAutsharpSpawnPoint_ovb0023* self) {
    fn_710112b7c4(self);
    UnkFlagTarget* target = self->owner->field_0x228;
    if (target) {
        if (self->field_0x3b8 == 0)
            fn_7101172764(target, g_unk_710164b4f8);
        else if (self->field_0x3b8 == 1)
            fn_7101172764(target, g_unk_710164b500);
    }
    self->field_0x400 = false;
}

struct UnkAutsharpSend_b57;
struct UnkAutsharpHashed_b57;

struct UnkAutsharpComponents_ovb0023 {
    u8 pad_0x0[0x240];
    UnkReflObject* field_0x240;
};

struct UnkAutsharpEntity_ovb0023 {
    u8 pad_0x0[0x10];
    UnkAutsharpComponents_ovb0023* components;
};

struct UnkAutsharpSpawnPoint2_ovb0023 {
    u8 pad_0x0[0x40];
    UnkAutsharpEntity_ovb0023* entity;
};

extern CStrId g_unk_7172744430 __attribute__((visibility("hidden")));

void fn_71003c91cc(UnkAutsharpSend_b57* self, UnkAutsharpHashed_b57* const* item);

void fn_7100a144c8(UnkAutsharpSpawnPoint2_ovb0023* self) {
    UnkReflObject* ai = self->entity->components->field_0x240;
    if (ai && fn_710009d50c(ai->vf_0x0(), fn_71003c8a7c())) {
        CStrId id(g_unk_7172744430);
        fn_71003c91cc(reinterpret_cast<UnkAutsharpSend_b57*>(ai),
                      reinterpret_cast<UnkAutsharpHashed_b57* const*>(&id));
    }
}
