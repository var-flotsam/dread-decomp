// CCharClassEventScenarioComponent's unit (tools/tu_map.py unit 1251): starts the scenario. Unless the game
// manager says not to (fn_7100deabe4), the collider is built from the owner's shape when there is none yet,
// every entry with a random range draws its number when it has none, the timers and the per-actor values are
// reset, and the scenario is started (fn_71007e2ecc, then fn_71007e35fc). Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CRntVector.h"
#include "game/CBlackboard.h"
#include "game/CCharClassEventScenarioComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU365.h"
#include "game/UnkTU67.h"
#include "types.h"

extern "C" int rand();

struct UnkEventScenario_ovb0097;
void fn_71007e4328(UnkEventScenario_ovb0097* self, void* shape, const UnkMatrix44* transform);

struct UnkScenarioActor_l7e2a88 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
};

struct UnkScenarioTracker_l7e2a88 {
    u8 pad_0x0[0x30];
    void* shape;  // 0x30
};

struct UnkScenarioEntry_l7e2a88 {
    u8 pad_0x0[0x8];
    s32 max_random;  // 0x8
    u8 pad_0xc[0x28 - 0xc];
};

struct UnkEventScenario_l7e2a88 {
    void** vtable;
    u8 base_0x8[0x28];
    UnkScenarioActor_l7e2a88* owner;  // 0x30
    u8 pad_0x38[0xf8 - 0x38];
    void* collider;  // 0xf8
    u8 pad_0x100[0x8];
    base::global::CRntVector<void*> actors;                     // 0x108
    base::global::CRntVector<UnkScenarioEntry_l7e2a88> entries;  // 0x130
    base::global::CRntVector<u32> randoms;                      // 0x158
    u8 pad_0x180[0x28];
    base::global::CRntVector<u32> actor_values;  // 0x1a8
    u8 pad_0x1d0[0x10];
    f32 start_time;  // 0x1e0
    bool field_0x1e4;
    u8 pad_0x1e5[8];
    bool finished;  // 0x1ed
};
static_assert(__builtin_offsetof(UnkEventScenario_l7e2a88, actor_values) == 0x1a8, "layout");
static_assert(__builtin_offsetof(UnkEventScenario_l7e2a88, finished) == 0x1ed, "layout");

void fn_71007e1730(UnkEventScenario_l7e2a88* self);
void CCharClassEventScenarioComponent_RebuildTimerArray(UnkEventScenario_ovb0097* self);
void fn_71007e35fc(UnkEventScenario_l7e2a88* self, f32 time);

using UnkScenarioLoad_l7e2a88 = void (*)(UnkEventScenario_l7e2a88*, CBlackboard*);
using UnkScenarioStart_l7e2a88 = void (*)(UnkEventScenario_l7e2a88*, s32, bool, f32, f32, f32, f32);

void fn_71007e2a88(UnkEventScenario_l7e2a88* self) {
    if (fn_7100deabe4(g_unk_7101d47310))
        return;
    if (!self->collider) {
        auto* tracker = reinterpret_cast<UnkScenarioTracker_l7e2a88*>(fn_710028aca8(self->owner));
        if (tracker)
            fn_71007e4328(reinterpret_cast<UnkEventScenario_ovb0097*>(self), tracker->shape,
                          &self->owner->transform);
        fn_71007e1730(self);
        CBlackboard* blackboard = &g_unk_7101d47310->field_0x968;
        reinterpret_cast<UnkScenarioLoad_l7e2a88>(self->vtable[0x140 / 8])(self, blackboard);
    }
    for (u64 i = 0; i < self->entries.count; ++i) {
        UnkScenarioEntry_l7e2a88* entry = &self->entries.data[i];
        if (entry->max_random && (s32)*fn_710008c504(&self->randoms, (u32)i) < 0) {
            s32 max = entry->max_random;
            *fn_710008c504(&self->randoms, (u32)i) = max ? (rand() & 0x7fff) % max : 0;
        }
    }
    CCharClassEventScenarioComponent_RebuildTimerArray(reinterpret_cast<UnkEventScenario_ovb0097*>(self));
    for (u32 i = 0; i < self->actors.count; ++i)
        self->actor_values.PushBack(0);
    reinterpret_cast<UnkScenarioStart_l7e2a88>(fn_71007e2ecc)(
        self, self->finished ? 2 : 0, true, self->finished ? 0.0f : self->start_time,
        (self->field_0x1e4 || self->finished) ? 0.0f : 100.0f, 0.0f, 0.0f);
    fn_71007e35fc(self, 0.0f);
}
