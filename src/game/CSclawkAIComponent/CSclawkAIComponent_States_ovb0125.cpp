// CSclawkAIComponent's unit (tools/tu_map.py unit 1005): the hit filter (fn_710067d4a4: unless the
// track's state id is 0x1cdf12aa84de383c, hits without the flags 0x8011020 give 1 and are taken) and the
// state update (fn_710067c8e0: wait, check, two countdowns, then at the track's end the part
// 0xe6de93263e1369e9 is switched on once). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkSwitchObject.h"
#include "game/UnkTU2702.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkGoliathAI_b55;
struct UnkGoliathHitInfo_b55;
struct UnkGoliathHitTarget_b55;
base::global::UnkStringData** fn_7101177ae8(UnkAnimationComponent* self, u64 kind);

// A copied string id whose destructor does not test for a null id.
struct UnkSclawkId_ovb0125 {
    UnkStringData* data;
    UnkSclawkId_ovb0125(UnkStringData* d) : data(d) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    ~UnkSclawkId_ovb0125() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data != &pool->empty && --data->ref_count == 0)
                fn_71000803a4(pool, data);
        }
    }
    u64 Hash() const { return *reinterpret_cast<const u64*>(reinterpret_cast<const char*>(data) + 0x30); }
};

struct UnkSclawkOwner_ovb0125 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* field_0x220;
};

struct UnkSclawkAI_ovb0125 {
    u8 pad_0x0[0x30];
    UnkSclawkOwner_ovb0125* owner;
};

struct UnkSclawkHitInfo_ovb0125 {
    u8 pad_0x0[0xb4];
    u32 flags;
};

u32 fn_710067d4a4(UnkSclawkAI_ovb0125* self, u32* out, void* arg2, UnkSclawkHitInfo_ovb0125* info,
                  UnkGoliathHitTarget_b55** hit) {
    if (UnkAnimationComponent* anim = self->owner->field_0x220) {
        bool handled = false;
        {
            UnkSclawkId_ovb0125 id(*fn_7101177ae8(anim, 0xf4061ff25a2163d6));
            if (id.Hash() != 0x1cdf12aa84de383c && !(info->flags & 0x8011020)) {
                // An untyped store: the original reloads the string pool after it.
                u32 one = 1;
                __builtin_memcpy(out, &one, sizeof(one));
                handled = true;
            }
        }
        if (handled)
            return 1;
    }
    return fn_71011061ac(reinterpret_cast<UnkGoliathAI_b55*>(self), out, arg2,
                         reinterpret_cast<UnkGoliathHitInfo_b55*>(info), hit);
}

struct UnkObject;
struct UnkComponent;
UnkComponent* fn_7101152b90(UnkObject*, u64);
bool fn_710114e668(UnkObject*, u8);

struct UnkSclawkPart_ovb0125 {
    u8 pad_0x0[0xb];
    u8 kind;  // 0xb
};

struct UnkSclawkOwner2_ovb0125 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* field_0x220;
    u8 pad_0x228[0x58];
    UnkObject* field_0x280;
};

struct UnkSclawkAI2_ovb0125 {
    u8 pad_0x0[0x30];
    UnkSclawkOwner2_ovb0125* owner;
    u8 pad_0x38[0x14698];
    f32 field_0x146d0;
    f32 field_0x146d4;
    f32 field_0x146d8;
    u32 field_0x146dc;
    u32 state;  // 0x146e0
    u8 pad_0x146e4[0xb];
    bool field_0x146ef;
};
static_assert(__builtin_offsetof(UnkSclawkAI2_ovb0125, field_0x146ef) == 0x146ef, "layout");

u32 fn_710067ca84(UnkSclawkAI2_ovb0125* self);
void fn_710067c478(UnkSclawkAI2_ovb0125* self, u32 state);

void fn_710067c8e0(UnkSclawkAI2_ovb0125* self, f32 dt) {
    switch (self->state) {
    case 0:
        self->field_0x146d0 -= dt;
        if (self->field_0x146d0 <= 0.0f)
            self->state = 1;
        break;
    case 1: {
        u32 r = fn_710067ca84(self);
        self->field_0x146dc = r;
        if (!r)
            fn_710067c478(self, 2);
        break;
    }
    case 2:
        if (self->field_0x146d4 > 0.0f) {
            self->field_0x146d4 -= dt;
            if (self->field_0x146d4 <= 0.0f)
                fn_710067c478(self, 3);
        } else if (self->field_0x146d8 > 0.0f) {
            self->field_0x146d8 -= dt;
            if (self->field_0x146d8 <= 0.0f)
                fn_710067c478(self, 3);
        }
        break;
    case 3:
        if (fn_710117ae38(self->owner->field_0x220, 0x6596032ae6e891b5, false)) {
            if (__builtin_expect(!fn_710067ca84(self), 1)) {
                UnkObject* parts = self->owner->field_0x280;
                if (parts && !self->field_0x146ef) {
                    auto* part = reinterpret_cast<UnkSclawkPart_ovb0125*>(fn_7101152b90(parts, 0xe6de93263e1369e9));
                    if (part && !fn_710114e668(parts, part->kind))
                        part = nullptr;
                    fn_7100146e60(reinterpret_cast<UnkSwitchObject*>(part), true);
                    self->field_0x146ef = true;
                }
            }
            fn_710067c478(self, 0);
        }
        break;
    }
}
