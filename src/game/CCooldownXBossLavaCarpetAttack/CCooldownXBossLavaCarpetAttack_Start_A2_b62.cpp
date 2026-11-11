// CCooldownXBossLavaCarpetAttack's unit (tools/tu_map.py): the attack's start. After fn_71004c1944, hooks the
// callback fn_71004c5db0 (link at +0x70, handle at +0x88) up to the object the owner (+0x8) tracks at +0x220, as
// fn_71004c98dc (src/game/CChozoWarriorXGlaiveSpinAttack/UnkAttackTrackLink_b54.cpp); clears +0x58; when the
// owner's AI component (+0x240) is a CCooldownXBossAIComponent, picks the pattern at +0x1c0 (the tunable's +0x24
// unless it is -1; else 0 or 3 at random while the AI's +0x1581c is positive, or a random one of the +0x108 count of
// the definition at +0x68), resets the pattern state, sets +0x80 of g_unk_7172790a08 to 1, switches on the entity
// the AI's +0x156d0 handle (fn_71003b0c4c) leads to and sets the id 0xe582b5412ec250e3 on its timeline when the
// timeline has a resource entry for it; always clears +0x785 and +0x780. Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CBossAIComponent.h"
#include "game/CCooldownXBossAttack.h"
#include "game/UnkTU2772.h"
#include "game/UnkTUProperty.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkTimelineComponent.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::reflection::CClass;

struct UnkSelf;
void fn_7100316018(UnkWeakTarget* self, UnkWeakHandle** out);
void fn_710117b3e8(UnkWeakTarget* target, void* link);
void fn_71004c5db0();
struct UnkEntityState;
void fn_7100232864(UnkEntityState* entity, bool value);
// Typed as CHydrogigaZiplineRailComponent_b49.cpp declares it (one signature per symbol).
struct UnkZiplineLink_b49;
struct UnkZiplineLinkTarget_b49;
extern UnkTunable g_unk_71726d1f48;
struct UnkGlobal2;
extern UnkGlobal2 g_unk_7172790a08;

struct UnkCarpetStartAI_b62a2 {
    virtual const CClass* vf_0x0();
};

struct UnkCarpetStartEntity_b62a2 {
    u8 pad_0x0[0x2e8];
    void* field_0x2e8;
};
struct UnkCarpetStartRef_b62a2 {
    u8 pad_0x0[0x10];
    UnkCarpetStartEntity_b62a2* entity;
};

struct UnkCarpetStartOwner_b62a2 {
    u8 pad_0x0[0x220];
    UnkWeakTarget* field_0x220;
    u8 pad_0x228[0x240 - 0x228];
    UnkCarpetStartAI_b62a2* field_0x240;
};

struct UnkCarpetStartDef_b62a2 {
    u8 pad_0x0[0x108];
    s32 count;
};

struct UnkCarpetTrackLink_b62a2 {
    u8 pad_0x0[0x8];
    void* context;
    void (*callback)();
};

struct UnkCarpetStart_b62a2 {
    u8 pad_0x0[0x8];
    UnkCarpetStartOwner_b62a2* owner;
    u8 pad_0x10[0x58 - 0x10];
    u64 field_0x58;
    u8 pad_0x60[0x8];
    UnkCarpetStartDef_b62a2* field_0x68;
    UnkCarpetTrackLink_b62a2 link;  // 0x70
    UnkWeakHandle* handle;          // 0x88
    u8 pad_0x90[0x1c0 - 0x90];
    s32 field_0x1c0;
    u32 field_0x1c4;
    u32 field_0x1c8;
    u8 pad_0x1cc[0x1d8 - 0x1cc];
    u32 field_0x1d8;
    u8 pad_0x1dc[0x780 - 0x1dc];
    u32 field_0x780;
    u8 pad_0x784;
    bool field_0x785;
    bool field_0x786;
};
static_assert(__builtin_offsetof(UnkCarpetStart_b62a2, field_0x786) == 0x786, "layout");

void fn_71004c3664(UnkCarpetStart_b62a2* self) {
    fn_71004c1944(reinterpret_cast<UnkSelf*>(self));
    UnkCarpetTrackLink_b62a2* link = &self->link;
    link->context = self;
    link->callback = fn_71004c5db0;
    UnkWeakTarget* track = self->owner->field_0x220;
    if (UnkWeakHandle* handle = self->handle) {
        if (UnkWeakTarget* target = handle->target)
            fn_710117b3f0(target, link);
        if (UnkWeakHandle* p = self->handle) {
            if (--p->ref_count == 0)
                p->vf_0x10();
            self->handle = nullptr;
        }
    }
    if (track) {
        asm("" : "+r"(self));
        fn_7100316018(track, &self->handle);
        fn_710117b3e8(self->handle ? self->handle->target : nullptr, link);
    }
    self->field_0x58 = 0;
    UnkCarpetStartAI_b62a2* ai = self->owner->field_0x240;
    if (ai && fn_710009d50c(ai->vf_0x0(), fn_71004cf318())) {
        s32 pattern = *reinterpret_cast<s32*>(reinterpret_cast<char*>(&g_unk_71726d1f48) + 0x24);
        if (pattern == -1) {
            if (*reinterpret_cast<f32*>(reinterpret_cast<char*>(ai) + 0x1581c) > 0.0f) {
                pattern = (fn_71012395a0() & 1) ? 3 : 0;
            } else {
                s32 count = self->field_0x68->count;
                pattern = count ? (fn_71012395a0() & 0x7fff) % count : 0;
            }
        }
        self->field_0x1c0 = pattern;
        self->field_0x1c4 = 0;
        self->field_0x1c8 = 1;
        self->field_0x786 = false;
        self->field_0x1d8 = 0;
        *reinterpret_cast<f32*>(reinterpret_cast<char*>(&g_unk_7172790a08) + 0x80) = 1.0f;
        UnkCarpetStartRef_b62a2* ref = *reinterpret_cast<UnkCarpetStartRef_b62a2**>(
            fn_71003b0c4c(reinterpret_cast<UnkZiplineLink_b49*>(reinterpret_cast<u8*>(ai) + 0x156d0)));
        if (ref) {
            if (UnkCarpetStartEntity_b62a2* entity = ref->entity) {
                fn_7100232864(reinterpret_cast<UnkEntityState*>(entity), true);
                base::reflection::UnkReflTypeIdArg mask;
                u64 id = 0xe582b5412ec250e3ull;
                auto* timeline = static_cast<UnkTimelineComponent*>(entity->field_0x2e8);
                if (timeline && fn_7100a2c13c(timeline, &id)) {
                    mask.id = UnkAll_SpitSend().value;
                    fn_7100a2cb7c(timeline, &id, &mask.id, false, -1.0f);
                }
            }
        }
    }
    self->field_0x785 = false;
    self->field_0x780 = 0;
}
