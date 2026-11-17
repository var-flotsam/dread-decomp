// Unit 1176 (hint 'CCentralUnitComponent'): lets go of the Emmy the central unit holds. Drops the handle at
// +0x6d8 when it refers to `emmy` (and stops if it refers to another one), resets the render pass colour when
// the Emmy's character class has its flag set, notifies every door Emmy FX component and requests state 7 from
// the state machine at +0x30 ("none" 8). Names and layouts are placeholders.
#include "../UnkSmRequest_A5_b73.h"
#include "base/math/CVector4D.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CDoorEmmyFXComponent.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU1176.h"
#include "game/UnkTU315.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::reflection::UnkReflObject;

extern const base::math::CVector4D g_unk_71016477b4;

struct UnkEmmyCharClass_l773c88 : UnkReflObject {
    u8 pad_0x8[0x610 - 0x8];
    bool field_0x610;
};

struct UnkEmmyAI_l773c88 : UnkReflObject {
    u8 pad_0x8[0x10 - 0x8];
    UnkEmmyCharClass_l773c88* char_class;
};

struct UnkEmmy_l773c88 {
    u8 pad_0x0[0x240];
    UnkEmmyAI_l773c88* ai;
};

struct UnkDoorEntry_l773c88 {
    u8 pad_0x0[0x10];
    UnkReflObject* component;
};

struct UnkDoorEntries_l773c88 {
    UnkDoorEntry_l773c88** data;
    u32 count;
};

void fn_71007b6ca0(UnkReflObject* component);

struct UnkCentralUnit_l773c88 {
    u8 pad_0x0[0x30];
    UnkSmMachine_A5_b73 machine;
    u8 pad_0xe0[0x6d8 - 0xe0];
    UnkWeakHandle* emmy;
};
static_assert(__builtin_offsetof(UnkCentralUnit_l773c88, emmy) == 0x6d8, "layout");

void fn_7100773c88(UnkCentralUnit_l773c88* self, UnkEmmy_l773c88* emmy) {
    if (UnkWeakHandle* handle = self->emmy) {
        if (handle->target) {
            if (handle->target != reinterpret_cast<UnkWeakTarget*>(emmy))
                return;
            if (--handle->ref_count == 0)
                handle->vf_0x10();
            self->emmy = nullptr;
        }
    }
    if (emmy) {
        if (UnkEmmyAI_l773c88* ai = emmy->ai) {
            if (fn_710009d50c(ai->vf_0x0(), fn_710053b934())) {
                UnkEmmyCharClass_l773c88* char_class = ai->char_class;
                if (char_class && fn_710009d50c(char_class->vf_0x0(), fn_710051e010()) &&
                    char_class->field_0x610)
                    fn_710021a0b4(g_unk_7101d47300, &g_unk_710164aa90, &g_unk_71016477b4);
            }
        }
    }
    UnkDoorEntries_l773c88* doors = reinterpret_cast<UnkDoorEntries_l773c88*>(fn_71007b5d90());
    for (u64 i = 0; i < doors->count; ++i) {
        UnkDoorEntry_l773c88* entry = doors->data[i];
        if (!entry)
            continue;
        UnkReflObject* component = entry->component;
        if (!component)
            continue;
        if (!fn_710009d50c(component->vf_0x0(), fn_71007b55e4()))
            continue;
        fn_71007b6ca0(component);
    }
    RequestState_A5_b73<15, 8>(&self->machine, 7);
}
