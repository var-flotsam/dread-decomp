// The CCharClassSclawkAIComponent unit. fn_710067b8b8: with no animation component (+0x220) the answer is 4;
// otherwise the id held under the usual track kind (fn_7101177ae8) is copied, and when its hash is
// 0x1cdf12aa84de383c the answer is 4 if bit 2 of the byte at +0xb6 of the fourth argument is clear and 5 if
// it is set; any other hash gives 0. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

// A copied string id whose destructor does not test for a null id.
struct UnkSclawkId_b68a4 {
    UnkStringData* data;
    UnkSclawkId_b68a4(const UnkSclawkId_b68a4& other) : data(other.data) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    ~UnkSclawkId_b68a4() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data != &pool->empty && --data->ref_count == 0)
                fn_71000803a4(pool, data);
        }
    }
};

base::global::UnkStringData** fn_7101177ae8(UnkAnimationComponent* self, u64 kind);

struct UnkSclawkOwner_b68a4 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* field_0x220;
};

struct UnkSclawkState_b68a4 {
    u8 pad_0x0[0xb6];
    u8 field_0xb6;
};

u32 fn_710067b8b8(void* self, UnkSclawkOwner_b68a4* owner, void* arg, UnkSclawkState_b68a4* state) {
    UnkAnimationComponent* anim = owner->field_0x220;
    u32 result;
    bool four;
    if (!anim)
        goto ret4;
    {
        UnkSclawkId_b68a4 id(*reinterpret_cast<UnkSclawkId_b68a4*>(fn_7101177ae8(anim, 0xf4061ff25a2163d6)));
        if (*reinterpret_cast<u64*>(reinterpret_cast<char*>(id.data) + 0x30) == 0x1cdf12aa84de383c) {
            four = !(state->field_0xb6 & 4);
            result = 5;
        } else {
            result = 0;
            four = false;
        }
    }
    if (four)
        goto ret4;
    return result;
ret4:
    return 4;
}
