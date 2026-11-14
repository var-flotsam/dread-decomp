// CTrainWithPortalUsableComponent unit (tools/tu_map.py): resolves the handle at +0x5a8 (source +0x578,
// state +0x5a0) and gives the actor it names one of two timeline keys. Names and layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "game/UnkTU437.h"
#include "game/UnkTimelineComponent.h"
#include "base/UnkDestructors.h"
#include "types.h"

struct UnkCounted_TrainB47 : base::UnkRefCounted {
    union {
        void* field_0x10;
        u64 raw_0x10;
    };
};

using UnkRef = base::UnkRefPtrMember;

// A lazily resolved handle (the shape of CMagmaKraidScenarioControllerComponent_b45.cpp's fn_710091793c):
// the state is 1 until the handle takes its value from the source and 2 after.
struct UnkLazyHandle_TrainB47 {
    u8 source[0x28];
    u32 state;
    u32 pad;
    base::UnkRefHandle handle;

    __attribute__((always_inline)) bool Resolve() {
        if (state == 1) {
            {
                UnkRef fresh = fn_7100353be8(source);
                fn_710031d208(&handle, reinterpret_cast<const base::UnkRefHandle*>(&fresh));
            }
            state = 2;
        }
        auto* c = static_cast<UnkCounted_TrainB47*>(handle.ptr);
        return c && c->raw_0x10;
    }
    // The object (the original reloads it after the test).
    __attribute__((always_inline)) void* Object() {
        auto* c = static_cast<UnkCounted_TrainB47*>(handle.ptr);
        return *reinterpret_cast<void* volatile*>(&c->field_0x10);
    }
};


extern __attribute__((visibility("hidden"))) const u64 g_unk_710164b608;
extern __attribute__((visibility("hidden"))) const u64 g_unk_710164b610;

struct UnkTrainWithPortalUsable_b47 {
    u8 pad_0x0[0x578];
    UnkLazyHandle_TrainB47 link;  // 0x578
};

void fn_7100a9b9a4(UnkTrainWithPortalUsable_b47* self, bool on) {
    if (!self->link.Resolve())
        return;
    UnkActor* actor = static_cast<UnkActor*>(self->link.Object());
    if (on)
        fn_7100d7c828(actor, &g_unk_710164b608, false);
    else
        fn_7100d7c828(actor, &g_unk_710164b610, false);
}
