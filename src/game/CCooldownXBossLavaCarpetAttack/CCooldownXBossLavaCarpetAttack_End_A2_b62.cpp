// CCooldownXBossLavaCarpetAttack's unit (tools/tu_map.py): after fn_71004c196c, unlinks the link at +0x70 from the
// target of the weak handle at +0x88 and drops the handle; runs fn_71004c1e08; drops every render item of the
// 0x30-byte entries at +0x1dc (count at +0x1d8); when the owner's (+0x8) AI component (+0x240) is a
// CCooldownXBossAIComponent, sets the id 0x20b5d12f42ca490a on the timeline of the entity its +0x156d0 handle
// (fn_71003b0c4c) leads to, when that timeline has a resource entry for it, then calls fn_71004d4a28 on it; finally,
// when +0x785 is set, hands the owner's +0x30 to fn_710116a170 on the +0x1c0 of fn_7100d7c990's actor and clears
// +0x780 and +0x785. Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CBossAIComponent.h"
#include "game/CCooldownXBossAttack.h"
#include "game/UnkActor.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkTimelineComponent.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::reflection::CClass;

struct UnkCooldownXBossAttack_b62a2;
struct UnkLaserBite_b59;
// Typed as CHydrogigaZiplineRailComponent_b49.cpp declares it (one signature per symbol).
struct UnkZiplineLink_b49;
struct UnkZiplineLinkTarget_b49;
struct UnkCarpetAI_b62a2;
void fn_71004d4a28(UnkCarpetAI_b62a2* ai);
void fn_710116a170(void* self, void* item);

struct UnkCarpetAI_b62a2 {
    virtual const CClass* vf_0x0();
};

struct UnkCarpetEntity_b62a2 {
    u8 pad_0x0[0x2e8];
    void* field_0x2e8;
};
struct UnkCarpetRef_b62a2 {
    u8 pad_0x0[0x10];
    UnkCarpetEntity_b62a2* entity;
};
struct UnkCarpetHandle_b62a2 {
    UnkCarpetRef_b62a2* ref;
};

struct UnkCarpetItem_b62a2 {
    u8 pad_0x0[0x8];
    u8 item[0x8];
};
typedef UnkCarpetItem_b62a2* __attribute__((aligned(4))) UnkCarpetItemPtr_b62a2;
struct UnkCarpetEntry_b62a2 {
    u8 pad_0x0[0x28];
    UnkCarpetItemPtr_b62a2 item;
};
static_assert(sizeof(UnkCarpetEntry_b62a2) == 0x30, "layout");

struct UnkLavaCarpet_b62a2 {
    u8 pad_0x0[0x8];
    UnkActor* owner;
    u8 pad_0x10[0x70 - 0x10];
    u8 link[0x18];  // 0x70
    UnkWeakHandle* field_0x88;
    u8 pad_0x90[0x1d8 - 0x90];
    u32 count;
    UnkCarpetEntry_b62a2 entries[30];
    u8 pad_end[0x780 - 0x1dc - 30 * 0x30];
    u32 field_0x780;
    u8 pad_0x784;
    bool field_0x785;
};
static_assert(__builtin_offsetof(UnkLavaCarpet_b62a2, field_0x785) == 0x785, "layout");

void fn_71004c4fac(UnkLavaCarpet_b62a2* self) {
    fn_71004c196c(reinterpret_cast<UnkCooldownXBossAttack_b62a2*>(self));
    if (UnkWeakHandle* handle = self->field_0x88) {
        if (UnkWeakTarget* target = handle->target)
            fn_710117b3f0(target, self->link);
        ResetRef(&self->field_0x88);
    }
    fn_71004c1e08(reinterpret_cast<UnkLaserBite_b59*>(self));
    u64 n = (u64)self->count * sizeof(UnkCarpetEntry_b62a2);
    asm("" : "+r"(n));
    for (UnkCarpetEntry_b62a2* it = self->entries; n != 0; ++it, n -= sizeof(UnkCarpetEntry_b62a2)) {
        if (UnkCarpetItem_b62a2* item = it->item) {
            fn_71002199d8(g_unk_7101d47300, reinterpret_cast<UnkItemBase_71011a91c4*>(item->item), 0);
            it->item = nullptr;
        }
    }
    auto* ai = static_cast<UnkCarpetAI_b62a2*>(self->owner->field_0x240);
    if (ai && fn_710009d50c(ai->vf_0x0(), fn_71004cf318())) {
        UnkCarpetRef_b62a2* ref = reinterpret_cast<UnkCarpetHandle_b62a2*>(
            fn_71003b0c4c(reinterpret_cast<UnkZiplineLink_b49*>(reinterpret_cast<u8*>(ai) + 0x156d0)))->ref;
        if (ref) {
            if (UnkCarpetEntity_b62a2* entity = ref->entity) {
                base::reflection::UnkReflTypeIdArg mask;
                u64 id = 0x20b5d12f42ca490aull;
                auto* timeline = static_cast<UnkTimelineComponent*>(entity->field_0x2e8);
                if (timeline && fn_7100a2c13c(timeline, &id)) {
                    mask.id = UnkAll_SpitSend().value;
                    fn_7100a2cb7c(timeline, &id, &mask.id, false, -1.0f);
                }
            }
        }
        fn_71004d4a28(ai);
    }
    if (self->field_0x785) {
        UnkActor* actor = fn_7100d7c990(self->owner);
        fn_710116a170(*reinterpret_cast<void**>(reinterpret_cast<char*>(actor) + 0x1c0),
                      reinterpret_cast<char*>(self->owner) + 0x30);
        self->field_0x780 = 0;
        self->field_0x785 = false;
    }
}
