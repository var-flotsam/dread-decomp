// CHecathonAIComponent's unit (tools/tu_map.py unit 865): after the base step (fn_71010ff2b8) the owner is
// removed from the AI manager's set at +0x2b8 (fn_71005ac994), the flag at +0x147cc is cleared and the two
// delays are drawn again from the tunable g_unk_71726e8d00's ranges (as fn_71005a9eac in
// CHecathonAIComponent_Timeline_b60.cpp); the HECATHONPLANKTONFX child gets the tunable's value at +0x28
// (fn_710080c5d4), then the type id is sent (fn_71005aa108, tail call). Names are placeholders.
#include <stdlib.h>

#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CHecathonPlanktonFXComponent.h"
#include "game/UnkManager_71726af468.h"
#include "game/UnkTU2702.h"
#include "types.h"

using base::reflection::UnkTunable;
// As src/game/CHecathonAIComponent/CHecathonAIComponent_Timeline_b60.cpp declares it.
extern UnkTunable g_unk_71726e8d00;

void fn_71005ac994(void* set, void* value);  // removes a value from the set

struct UnkRefl_fn_71005aad7c;
template <>
base::reflection::UnkReflObject* base::reflection::refl_FindTyped<UnkRefl_fn_71005aad7c>(void*);
struct UnkRefl_fn_71005aa108;
template <>
void base::reflection::refl_SendTypeId<UnkRefl_fn_71005aa108>(void*);

struct UnkPlanktonFX_A3_b71;

struct UnkHecathonAI_A3_b71 {
    u8 pad_0x0[0x18];
    void* field_0x18;
    u8 pad_0x20[0x10];
    void* owner;
    u8 pad_0x38[0x14768 - 0x38];
    u32 field_0x14768;
    u8 pad_0x1476c[4];
    u32 field_0x14770;
    f32 field_0x14774;
    f32 field_0x14778;
    u8 pad_0x1477c[0x147cc - 0x1477c];
    bool field_0x147cc;
};

static inline f32 Random_A3_b71() {
    return (f32)((rand() & 0x7fff) * (1.0 / 32767.0));
}

static inline f32 RandomRange_A3_b71(f32 lo, f32 hi) {
    f32 r = Random_A3_b71();
    return lo + (hi - lo) * r;
}

template <class T>
static inline T Tunable_A3_b71(u32 offset) {
    return *reinterpret_cast<T*>(reinterpret_cast<char*>(&g_unk_71726e8d00) + offset);
}

void fn_71005a9fd8(UnkHecathonAI_A3_b71* self) {
    fn_71010ff2b8(reinterpret_cast<char*>(self));
    UnkManager_71726af468* m = g_unk_71726af468;
    if (!m) {
        m = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
        fn_7100da7e2c(m);
        g_unk_71726af468 = m;
    }
    fn_71005ac994(reinterpret_cast<char*>(m) + 0x2b8, self->owner);
    self->field_0x147cc = false;
    self->field_0x14774 = RandomRange_A3_b71(Tunable_A3_b71<f32>(0x34), Tunable_A3_b71<f32>(0x38));
    self->field_0x14768 = Tunable_A3_b71<u32>(0x28);
    self->field_0x14778 = RandomRange_A3_b71(Tunable_A3_b71<f32>(0x3c), Tunable_A3_b71<f32>(0x40));
    self->field_0x14770 = Tunable_A3_b71<u32>(0x30);
    fn_710080c5d4(reinterpret_cast<UnkPlanktonFX_A3_b71*>(
                      base::reflection::refl_FindTyped<UnkRefl_fn_71005aad7c>(self->field_0x18)),
                  Tunable_A3_b71<f32>(0x28));
    base::reflection::refl_SendTypeId<UnkRefl_fn_71005aa108>(self);
}
