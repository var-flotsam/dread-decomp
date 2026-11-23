// CSclawkAIComponent's unit (tools/tu_map.py unit 1005): the hit filter (fn_710067d4a4: unless the
// track's state id is 0x1cdf12aa84de383c, hits without the flags 0x8011020 give 1 and are taken) and the
// state update (fn_710067c8e0: wait, check, two countdowns, then at the track's end the part
// 0xe6de93263e1369e9 is switched on once). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CAnimationComponent.h"
#include "game/CFXComponent.h"
#include "game/CSclawkAIComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkSwitchObject.h"
#include "game/UnkSclawkAIComponent.h"
#include "game/UnkTU2702.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkGoliathAI_b55;
struct UnkGoliathHitInfo_b55;
struct UnkGoliathHitTarget_b55;

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
                StringPool_Release(pool, data);
        }
    }
    u64 Hash() const { return *reinterpret_cast<const u64*>(reinterpret_cast<const char*>(data) + 0x30); }
};

struct UnkSclawkHitInfo_ovb0125 {
    u8 pad_0x0[0xb4];
    u32 flags;
};

u32 fn_710067d4a4(UnkSclawkAIComponent* self, u32* out, void* arg2, UnkSclawkHitInfo_ovb0125* info,
                  UnkGoliathHitTarget_b55** hit) {
    if (auto* anim = static_cast<UnkAnimationComponent*>(self->owner->field_0x220)) {
        bool handled = false;
        {
            UnkSclawkId_ovb0125 id(fn_7101177ae8(anim, 0xf4061ff25a2163d6)->data);
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

u32 fn_710067ca84(UnkSclawkAIComponent* self);

void fn_710067c8e0(UnkSclawkAIComponent* self, f32 dt) {
    switch (self->state) {
    case 0:
        self->wait -= dt;
        if (self->wait <= 0.0f)
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
        if (fn_710117ae38(static_cast<UnkAnimationComponent*>(self->owner->field_0x220), 0x6596032ae6e891b5, false)) {
            if (__builtin_expect(!fn_710067ca84(self), 1)) {
                void* parts = self->owner->field_0x280;
                if (parts && !self->part_shown) {
                    ShowSclawkPart(parts, true);
                    self->part_shown = true;
                }
            }
            fn_710067c478(self, 0);
        }
        break;
    }
}
