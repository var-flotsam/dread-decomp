// Unit 2702 (CAIComponent's neighbourhood): adds a string id to (or removes it from) an AI's
// small list of reasons to be ignored, then recomputes the ignored flag (the animation's ignore
// track, a timed override, any listed reason) and notifies the AI when it changed.
// Names and layouts are placeholders.
#include <algorithm>
#include <new>

#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

using base::global::CStrId;

// As src/game/CCharClassEventScenarioComponent/CCharClassEventScenarioComponent_Scenario_ovb0097.cpp
// defines them.
struct __attribute__((packed)) UnkScenarioIdList_ovb0097 {
    u32 count;
    CStrId ids[4];  // 0x4
};
static_assert(sizeof(UnkScenarioIdList_ovb0097) == 0x24, "UnkScenarioIdList_ovb0097 layout");
void CCharClassEventScenarioComponent_ResizeStrIdList(UnkScenarioIdList_ovb0097* self, u32 n);

struct UnkIgnoreOwner_l0ff850 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;
};

struct UnkIgnoreAI_l0ff850;
struct UnkIgnoreAIVtbl_l0ff850 {
    void* pad_0x0[230];
    void (*vf_0x730)(UnkIgnoreAI_l0ff850* self);
};

struct UnkIgnoreAI_l0ff850 {
    UnkIgnoreAIVtbl_l0ff850* vtable;
    u8 pad_0x8[0x28];
    UnkIgnoreOwner_l0ff850* owner;  // 0x30
    u8 pad_0x38[0x11e9];
    bool ignored;  // 0x1221
    u8 pad_0x1222[2];
    UnkScenarioIdList_ovb0097 reasons;  // 0x1224
    bool forced;                        // 0x1248
    u8 pad_0x1249[3];
    f32 forced_time;  // 0x124c
};
static_assert(__builtin_offsetof(UnkIgnoreAI_l0ff850, forced_time) == 0x124c, "layout");

void fn_71010ff850(void* target, bool flag, void* arg) {
    auto* self = static_cast<UnkIgnoreAI_l0ff850*>(target);
    const CStrId* reason = static_cast<const CStrId*>(arg);
    UnkScenarioIdList_ovb0097* list = &self->reasons;

    CStrId* begin = list->ids;
    CStrId* end = begin + list->count;
    CStrId* it = std::find_if(begin, end, [reason](const CStrId& id) { return id.data == reason->data; });
    if (flag) {
        if (it == end) {
            new (end) CStrId(*reason);
            ++list->count;
        }
    } else if (it != end) {
        CStrId* last = end - 1;
        if (last != it)
            it->ReleaseAndTake(*last);
        CCharClassEventScenarioComponent_ResizeStrIdList(list, list->count - 1);
    }

    bool was_ignored = self->ignored;
    self->ignored = false;
    UnkAnimationComponent* animation = self->owner->animation;
    self->ignored = animation ? fn_710117b348(animation, fn_7100d05168()) : false;
    if (self->forced) {
        self->ignored = true;
        if (!(self->forced_time > 0.0f)) {
            self->ignored = false;
            self->forced = false;
        }
    }
    if (list->count != 0)
        self->ignored = true;
    if (was_ignored != self->ignored)
        self->vtable->vf_0x730(self);
}
