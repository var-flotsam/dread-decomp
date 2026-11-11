// CCharClassAutclastAIComponent's unit (tools/tu_map.py): one of two parts of the large object built once
// (base::reflection::refl_GetSingleton), +0x23b0 when the owner's AI (+0x218) is in state 4 (fn_71010cc3a4),
// +0x23a8 otherwise. Names are placeholders.
#include "base/reflection/UnkReflStatics.h"
#include "game/CEnemyMovement.h"
#include "types.h"

struct UnkAutclastOwner_ovb0103 {
    u8 pad_0x0[0x218];
    void* ai;  // 0x218
};

static inline const void* SingletonPart_ovb0103(u32 offset) {
    return reinterpret_cast<const char*>(&base::reflection::refl_GetSingleton()) + offset;
}

const void* fn_71003ba3f4(void* self, UnkAutclastOwner_ovb0103* owner) {
    if (fn_71010cc3a4(owner->ai) == 4)
        return SingletonPart_ovb0103(0x23b0);
    return SingletonPart_ovb0103(0x23a8);
}
