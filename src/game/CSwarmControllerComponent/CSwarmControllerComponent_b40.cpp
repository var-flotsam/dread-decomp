// CSwarmControllerComponent unit (tools/tu_map.py unit 1163): fn_7100747f74 releases the pending
// callback object at +0x260 (fn_71005c4828), then fades out (0.2, fn_7100105a80) every sound still
// alive among the weak references of the array at +0x230 (fn_7100748064 gives reference i, as in
// src/game/CShotManager.cpp). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/snd/UnkSound.h"
#include "types.h"

struct UnkWeakTarget_sw {
    UnkSound* sound;
    s64 generation;
};

// The layout of UnkShotWeakRef in src/game/CShotManager.cpp.
struct UnkShotWeakRef {
    UnkWeakTarget_sw* target;
    s64 generation;
};

UnkShotWeakRef* fn_7100748064(base::UnkVector<UnkShotWeakRef>* self, u32 i);
void fn_71005c4828(void*, void**);

struct UnkSwarmController_sw {
    u8 pad_0x0[0x230];
    base::UnkVector<UnkShotWeakRef> sounds;
    u8 pad_0x258[0x8];
    void* field_0x260;
};

static inline __attribute__((always_inline)) UnkSound* Get_sw(UnkShotWeakRef* ref) {
    UnkWeakTarget_sw* t = ref->target;
    if (t && t->generation == ref->generation && ref->generation != -1)
        return t->sound;
    return nullptr;
}

void fn_7100747f74(UnkSwarmController_sw* self) {
    if (self->field_0x260) {
        fn_71005c4828(self->field_0x260, &self->field_0x260);
        self->field_0x260 = nullptr;
    }
    for (u32 i = 0; i < self->sounds.count; ++i) {
        if (Get_sw(fn_7100748064(&self->sounds, i)))
            fn_7100105a80(Get_sw(fn_7100748064(&self->sounds, i)), 0.2f, 0.0f);
    }
}
