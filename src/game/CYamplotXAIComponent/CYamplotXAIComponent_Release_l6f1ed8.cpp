// CYamplotXAIComponent's unit: when the owner's animation component is playing the usual track kind, checks
// the id of the track and clears or acts on the pending flags, calling the movement routine with the
// target's own vector (x zeroed). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CAnimationComponent.h"
#include "game/CMovementComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkMovementVec_71010d08d4.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

// A copied string id whose destructor does not test for a null id.
struct UnkYamplotId_l6f1ed8 {
    UnkStringData* data;
    UnkYamplotId_l6f1ed8(const UnkYamplotId_l6f1ed8& other) : data(other.data) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    ~UnkYamplotId_l6f1ed8() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data != &pool->empty && --data->ref_count == 0)
                fn_71000803a4(pool, data);
        }
    }
};

struct UnkStringEntry_l6f1ed8 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkYamplotTarget_l6f1ed8 {
    u8 pad_0x0[0x1dc];
    f32 field_0x1dc;
    f32 field_0x1e0;
    u8 pad_0x1e4[0x24];
    u32 field_0x208;
};

struct UnkYamplotOwner_l6f1ed8 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* field_0x220;
};

struct UnkYamplot_l6f1ed8 {
    u8 pad_0x0[0x30];
    UnkYamplotOwner_l6f1ed8* owner;
    u8 pad_0x38[0x14758 - 0x38];
    u8 field_0x14758;
    u8 pad_0x14759[9];
    u8 field_0x14762;
};

static inline void Release(UnkYamplotTarget_l6f1ed8* target) {
    UnkMovementVec_71010d08d4 v = {0.0f, target->field_0x1dc, target->field_0x1e0};
    fn_71010d08d4(reinterpret_cast<UnkMovement_71010d08d4*>(target), &v, true);
    target->field_0x208 = 0;
}

void fn_71006f1ed8(UnkYamplot_l6f1ed8* self, UnkYamplotTarget_l6f1ed8* target) {
    UnkAnimationComponent* anim = self->owner->field_0x220;
    if ((anim != nullptr) & (self->field_0x14762 != 0)) {
        UnkYamplotId_l6f1ed8 id(*reinterpret_cast<const UnkYamplotId_l6f1ed8*>(fn_7101177ae8(anim, 0xf4061ff25a2163d6)));
        if (reinterpret_cast<const UnkStringEntry_l6f1ed8*>(id.data)->hash == 0x56b5846baf59688c) {
            if (self->field_0x14762)
                Release(target);
        } else {
            self->field_0x14762 = 0;
        }
    } else if (self->field_0x14758) {
        Release(target);
    }
}
