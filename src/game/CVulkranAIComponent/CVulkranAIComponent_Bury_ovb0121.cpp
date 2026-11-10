// CVulkranAIComponent's unit (tools/tu_map.py): fn_71006dbb78 (after fn_71010ff2b8) removes the entities of the
// two counted lists at +0x14738 and +0x14760 and clears them; fn_71006dc870 picks the animation id for kind 6;
// fn_71006dcdec sends the buried track's value once; fn_71006dcfa8 registers the owner with the global manager and
// sets up its animation. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkManager_71726af468.h"
#include "types.h"

void fn_71010ff2b8(char*);
void fn_7100232c68(void*);

// An intrusively counted object: count at +8, the entity it refers to at +0x10; vf_0x10 deletes it.
struct UnkVulkranRef_ovb0121 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 ref_count;  // 0x8
    void* entity;   // 0x10
};

struct UnkVulkranRefPtr_ovb0121 {
    UnkVulkranRef_ovb0121* p;

    __attribute__((always_inline)) ~UnkVulkranRefPtr_ovb0121() {
        if (p && --p->ref_count == 0)
            p->vf_0x10();
    }
};

struct UnkVulkranRefList_ovb0121 {
    UnkVulkranRefPtr_ovb0121* data;
    union {
        u32 count;
    };
    u8 unk_0xc[0x1c];

    __attribute__((always_inline)) void RemoveEntities() {
        for (UnkVulkranRefPtr_ovb0121* it = data, *end = data + count; it != end; ++it) {
            UnkVulkranRef_ovb0121* ref = it->p;
            if (!ref)
                continue;
            ++ref->ref_count;
            if (ref->entity)
                fn_7100232c68(ref->entity);
            if (--ref->ref_count == 0)
                ref->vf_0x10();
        }
    }

    __attribute__((always_inline)) void Clear() {
        for (u64 i = 0; i < count; ++i)
            data[i].~UnkVulkranRefPtr_ovb0121();
        count = 0;
    }
};

struct UnkVulkranAIEnd_ovb0121 {
    u8 pad_0x0[0x14738];
    UnkVulkranRefList_ovb0121 first;   // 0x14738
    UnkVulkranRefList_ovb0121 second;  // 0x14760
};

void fn_71006dbb78(UnkVulkranAIEnd_ovb0121* self) {
    fn_71010ff2b8(reinterpret_cast<char*>(self));
    self->first.RemoveEntities();
    self->first.Clear();
    self->second.RemoveEntities();
    self->second.Clear();
}

using base::global::CStrId;

struct UnkTakumakuComponent;
bool fn_7101178e6c(UnkTakumakuComponent*, u32);

struct UnkVulkranTuning_ovb0121;
const CStrId* fn_71006dac64(UnkVulkranTuning_ovb0121* tuning);
const CStrId* fn_71006dac58(UnkVulkranTuning_ovb0121* tuning);

struct UnkVulkranOwner_ovb0121b {
    u8 pad_0x0[0x220];
    UnkTakumakuComponent* animation;  // 0x220
};

struct UnkVulkranAI_ovb0121b {
    u8 pad_0x0[0x30];
    UnkVulkranOwner_ovb0121b* owner;  // 0x30
    u8 pad_0x38[0x146c8 - 0x38];
    UnkVulkranTuning_ovb0121* tuning;  // 0x146c8
};

void fn_71006dc870(UnkVulkranAI_ovb0121b* self, s32 kind, void* /*unused*/, CStrId* out) {
    *out = g_unk_7101d470e0;
    if (kind != 6)
        return;
    UnkTakumakuComponent* animation = self->owner->animation;
    if (!animation)
        return;
    if (fn_7101178e6c(animation, 0xb))
        *out = *fn_71006dac64(self->tuning);
    else if (fn_7101178e6c(self->owner->animation, 8))
        *out = *fn_71006dac58(self->tuning);
    else
        *out = g_unk_7101d470e0;
}

struct UnkVulkranHashed_ovb0121 {
    u8 pad_0x0[0x30];
    u64 hash;  // 0x30
};

struct UnkVulkranTuning_ovb0121 {
    u8 pad_0x0[0x510];
    u32 field_0x510;
};

UnkVulkranHashed_ovb0121* const* fn_71006dabb4(UnkVulkranTuning_ovb0121* tuning);

struct UnkVulkranOwner_ovb0121c {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkVulkranAIBury_ovb0121 {
    u8 pad_0x0[0x30];
    UnkVulkranOwner_ovb0121c* owner;  // 0x30
    u8 pad_0x38[0x121d - 0x38];
    bool field_0x121d;
    u8 pad_0x121e[0x146c8 - 0x121e];
    UnkVulkranTuning_ovb0121* tuning;  // 0x146c8
    bool buried;                       // 0x146d0
    u8 pad_0x146d1[3];
    u32 field_0x146d4;
};

bool fn_71006dcdec(UnkVulkranAIBury_ovb0121* self) {
    UnkAnimationComponent* animation = self->owner->animation;
    if (!animation)
        return false;
    if (!fn_710117b348(animation, fn_7100cee9c8())) {
        self->buried = false;
        return true;
    }
    if (self->buried)
        return false;
    animation = self->owner->animation;
    u64 hash = (*fn_71006dabb4(self->tuning))->hash;
    if (base::reflection::refl_EmitU32(animation, hash, 0xf4061ff25a2163d6, 2, &g_unk_710164b958,
                                       &g_unk_7172769740)) {
        self->buried = true;
        self->field_0x146d4 = self->tuning->field_0x510;
        self->field_0x121d = true;
    }
    return false;
}

struct UnkActorTarget;
void fn_7101178e08(UnkActorTarget*, u32, u32, u32, f32);
void fn_710117662c(UnkActorTarget*, u32, u32, u32, u32, f32);
u32 fn_71010cc3a4(void*);

struct UnkVulkranOwner_ovb0121 {
    u8 pad_0x0[0x218];
    void* movement;              // 0x218
    UnkActorTarget* animation;   // 0x220
};

struct UnkVulkranAI_ovb0121 {
    u8 pad_0x0[0x30];
    UnkVulkranOwner_ovb0121* owner;  // 0x30
    u8 pad_0x38[0x14731 - 0x38];
    bool registered;  // 0x14731
};

bool fn_71006de9f8(void* list, UnkVulkranOwner_ovb0121* owner);
void fn_71006dd0c0(UnkVulkranAI_ovb0121* self);

static inline UnkManager_71726af468* Manager() {
    UnkManager_71726af468* manager = g_unk_71726af468;
    if (!manager) {
        manager = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
        fn_7100da7e2c(manager);
        g_unk_71726af468 = manager;
    }
    return manager;
}

bool fn_71006dcfa8(UnkVulkranAI_ovb0121* self, bool quiet) {
    if (!fn_71006de9f8(reinterpret_cast<char*>(Manager()) + 0x368, self->owner))
        return false;
    self->registered = true;
    UnkVulkranOwner_ovb0121* owner = self->owner;
    if (UnkActorTarget* animation = owner->animation) {
        void* movement = owner->movement;
        fn_7101178e08(animation, 6, 0, 0, -1.0f);
        fn_7101178e08(animation, 8, 0, 0, -1.0f);
        u32 moving = movement ? fn_71010cc3a4(movement) != 4 : 0;
        fn_710117662c(animation, 0xb, 0, moving, 1, -1.0f);
    }
    if (!quiet)
        fn_71006dd0c0(self);
    return true;
}
