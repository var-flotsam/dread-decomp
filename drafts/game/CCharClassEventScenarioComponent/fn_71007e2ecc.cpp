// CCharClassEventScenarioComponent unit (tools/tu_map.py unit 1251): starts the scenario's timers in a
// mode. For each linked actor whose timer is not already in that mode (unless forced), it looks up the
// actor's animation for the mode in the entry's id lists (grown on demand) and gives the timer a random
// phase within that animation's length and a random delay. Names and layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CAnimationComponent.h"
#include "game/CCharClassEventScenarioComponent.h"
#include "game/UnkTU341.h"
#include "game/UnkTU67.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::UnkStringData;

extern "C" int rand();
extern "C" float fmodf(float x, float y);

struct UnkScenarioIdList_ovb0097;
void CCharClassEventScenarioComponent_ResizeStrIdList(UnkScenarioIdList_ovb0097* self, u32 n);

// Up to four animation ids, one for each mode.
struct __attribute__((packed)) UnkScenarioIdList_l7e2ecc {
    u32 count;
    UnkStringData* ids[4];  // 0x4, string ids

    UnkScenarioIdList_l7e2ecc() { count = 0; }
    UnkScenarioIdList_l7e2ecc(const UnkScenarioIdList_l7e2ecc& other) {
        for (u64 i = 0; i < other.count; ++i) {
            UnkStringData* id = other.ids[i];
            ids[i] = id;
            if (&g_unk_7101d47108->empty != id)
                ++id->ref_count;
        }
        count = other.count;
    }
    ~UnkScenarioIdList_l7e2ecc() { SetCount(0); }

    void SetCount(u32 n) { CCharClassEventScenarioComponent_ResizeStrIdList(reinterpret_cast<UnkScenarioIdList_ovb0097*>(this), n); }
};
static_assert(sizeof(UnkScenarioIdList_l7e2ecc) == 0x24, "UnkScenarioIdList_l7e2ecc layout");

using UnkScenarioIds_l7e2ecc = base::global::CRntVector<UnkScenarioIdList_l7e2ecc>;

struct UnkScenarioTimer_ovb0097;
UnkScenarioTimer_ovb0097& CCharClassEventScenarioComponent_GetOrGrowTimerAt(base::global::CRntVector<UnkScenarioTimer_ovb0097>* self, u32 index);
u32* fn_71007e47d4(base::global::CRntVector<u32>* self, u32 index);
void fn_71007e58a0(base::global::CRntVector<UnkScenarioIds_l7e2ecc>* self, u32 count);

// A timer of the scenario (0x18 bytes).
struct UnkScenarioTimer_l7e2ecc {
    u64 key;    // the animation's id
    f32 time;   // 0x8, -1 when stopped
    f32 phase;  // 0xc
    u32 mode;   // 0x10
    bool forced;  // 0x14
};

struct UnkScenarioTarget_l7e2ecc : base::UnkRefCounted {
    void* field_0x10;
};

// A linked actor: the property it is read from, resolved once into the handle.
struct UnkScenarioLink_l7e2ecc {
    u8 pad_0x0[0x8];
    u8 source[0x28];  // 0x8
    u32 state;        // 0x30
    base::UnkRefHandle handle;  // 0x38
};

struct UnkScenarioAnim_l7e2ecc {
    u8 pad_0x0[0x24];
    s32 length;  // 0x24
};

struct UnkEventScenario_l7e2ecc {
    u8 pad_0x0[0x108];
    base::global::CRntVector<void*> links;                  // 0x108
    base::global::CRntVector<UnkScenarioIds_l7e2ecc> entries;  // 0x130
    base::global::CRntVector<u32> randoms;                  // 0x158
    base::global::CRntVector<void*> timers;                 // 0x180
    base::global::CRntVector<u32> modes;                    // 0x1a8
};
static_assert(__builtin_offsetof(UnkEventScenario_l7e2ecc, modes) == 0x1a8, "layout");

static inline f32 Random01() {
    return (rand() & 0x7fff) * (1.0 / 32767.0);
}

static inline f32 RandomRange(f32 lo, f32 hi) {
    return Random01() * (hi - lo) + lo;
}

// Grows the entries to index + 1 when the vector grows on demand: the capacity first to the next
// multiple of the step, then the count.
static inline void GrowEntries(base::global::CRntVector<UnkScenarioIds_l7e2ecc>* entries, u64 index) {
    if (entries->field_0x14 && index >= entries->count) {
        if (index >= entries->capacity && entries->step != 0) {
            u32 grown = ((u32)index / entries->step + 1) * entries->step;
            if (entries->capacity < grown) {
                auto* p = static_cast<UnkScenarioIds_l7e2ecc*>(
                    entries->allocator->vf_0x40((u64)grown * sizeof(UnkScenarioIds_l7e2ecc), 8, entries->flags));
                if (entries->data) {
                    UnkScenarioIds_l7e2ecc* src = entries->data;
                    UnkScenarioIds_l7e2ecc* dst = p;
                    for (u32 i = 0; i < entries->count; ++i, ++dst, ++src)
                        new (dst) UnkScenarioIds_l7e2ecc(static_cast<UnkScenarioIds_l7e2ecc&&>(*src));
                    entries->allocator->vf_0x48(entries->data);
                }
                entries->data = p;
                entries->capacity = grown;
            }
        }
        fn_71007e58a0(entries, (u32)index + 1);
    }
}

void fn_71007e2ecc(UnkEventScenario_l7e2ecc* self, u32 mode, bool force, f32 phase, f32 phase_range,
                   f32 time_min, f32 time_max) {
    for (u64 i = 0; i < self->links.count; ++i) {
        auto& timer = reinterpret_cast<UnkScenarioTimer_l7e2ecc&>(CCharClassEventScenarioComponent_GetOrGrowTimerAt(
            reinterpret_cast<base::global::CRntVector<UnkScenarioTimer_ovb0097>*>(&self->timers), i));
        if (*fn_71007e47d4(&self->modes, i) == mode && !force) {
            timer.time = -1.0f;
            continue;
        }

        auto* link = reinterpret_cast<UnkScenarioLink_l7e2ecc*>(
            fn_7100247340(reinterpret_cast<UnkCaterzillaLinks_ovb0054*>(&self->links), i));
        if (link->state == 1) {
            {
                UnkRefPtr reference = fn_710023b580(link->source);
                fn_7100234f78(&link->handle, reinterpret_cast<const base::UnkRefHandle*>(&reference));
            }
            link->state = 2;
        }
        auto* holder = static_cast<UnkScenarioTarget_l7e2ecc*>(link->handle.ptr);
        if (!holder || !holder->field_0x10)
            continue;
        UnkWeakTarget* target = fn_71007e4924(holder->field_0x10);
        if (!target)
            continue;

        GrowEntries(&self->entries, i);
        UnkScenarioIds_l7e2ecc& entry = self->entries.data[i];
        u32 index = *fn_710008c504(&self->randoms, i);
        UnkScenarioIdList_l7e2ecc& list = entry.GrowAt(index);
        if (mode + 1 > list.count)
            list.SetCount(mode + 1);
        timer.key = list.ids[mode]->Key();
        timer.mode = mode;
        timer.forced = force;
        timer.phase = RandomRange(0.0f, phase_range) + phase;
        auto* anim = static_cast<UnkScenarioAnim_l7e2ecc*>(fn_7101176d34(
            reinterpret_cast<UnkStruct_7101176ea8*>(target), reinterpret_cast<void*>(timer.key)));
        if (anim) {
            timer.phase = fmodf(timer.phase, (f32)anim->length);
            timer.time = RandomRange(time_min, time_max);
        } else {
            timer.time = -1.0f;
        }
    }
}
