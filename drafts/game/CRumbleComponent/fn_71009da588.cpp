// CRumbleComponent's unit (tools/tu_map.py unit 1500): the rumbles that have ended. A one-shot one is
// marked stopped; a looping one with some strength is played again from a copy of its parameters,
// scaled by the component's scale for it, when it is in range of the rumble manager's listener.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/input/UnkRumbleManager.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;
using base::input::UnkRumbleManager;

// A handle to a playing rumble (as in src/game/CAbilityEnergyWave/fn_71003053c4.cpp): current while the target's
// serial is the handle's.
struct UnkHandleTarget {
    void* field_0x0;
    u64 field_0x8;
};

struct UnkHandle {
    UnkHandleTarget* target;
    u64 serial;
};

// The rumble manager, created on first use.

static inline UnkRumbleManager* RumbleManager() {
    if (!g_unk_7101d47348) {
        UnkRumbleManager* m = static_cast<UnkRumbleManager*>(fn_71000839a0(sizeof(UnkRumbleManager), 0x3f));
        fn_71000733e4(m);
        g_unk_7101d47348 = m;
    }
    return g_unk_7101d47348;
}

// A string id on `Pool`: copies retain, and the release is done in two steps (the id and the count
// are reloaded after the decrement).
template <UnkStringPool*& Pool>
struct UnkRumbleStrId_l9da588 {
    UnkStringData* data;

    UnkRumbleStrId_l9da588(const UnkRumbleStrId_l9da588& other) : data(other.data) {
        if (&Pool->empty != data)
            ++data->ref_count;
    }
    ~UnkRumbleStrId_l9da588() {
        if (UnkStringPool* pool = Pool) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = Pool;
                }
            }
            data = &pool->empty;
        }
    }
};

// What a rumble is played from (0x48 bytes): its file and settings.
struct UnkRumblePlayParams_l9da588 {
    UnkRumbleStrId_l9da588<g_unk_7101d47200> path;
    u64 field_0x8;
    s32 scale_index;  // 0x10: the component's scale it takes, 0x7fffffff for none
    UnkVec3f pos;     // 0x14
    f32 range;        // 0x20
    f32 field_0x24;   // a range below this one plays anywhere
    f32 strength;     // 0x28
    f32 field_0x2c;
    f32 field_0x30;
    f32 field_0x34;
    f32 field_0x38;
    f32 field_0x3c;
    bool field_0x40;
    bool loop;  // 0x41
};
static_assert(sizeof(UnkRumblePlayParams_l9da588) == 0x48, "UnkRumblePlayParams_l9da588 layout");

// A rumble the component started (0x68 bytes).
struct UnkRumbleEntry_l9da588 {
    UnkHandle handle;
    UnkRumblePlayParams_l9da588 params;  // 0x10
    u8 pad_0x58[8];
    bool field_0x60;
    bool field_0x61;
    bool field_0x62;
    bool stopped;  // 0x63

    // The rumble while the handle is current.
    void* Get() const {
        if (!handle.target || handle.target->field_0x8 != handle.serial || handle.serial == ~0ull)
            return nullptr;
        return handle.target->field_0x0;
    }
};
static_assert(sizeof(UnkRumbleEntry_l9da588) == 0x68, "UnkRumbleEntry_l9da588 layout");

struct UnkRumbleComponent_l9da588 {
    u8 pad_0x0[0x48];
    UnkRumbleEntry_l9da588* entries;  // 0x48
    u32 count;                        // 0x50
    u8 pad_0x54[0x1c];
    f32 scales[1];  // 0x70, by the parameters' scale index
};
static_assert(__builtin_offsetof(UnkRumbleComponent_l9da588, scales) == 0x70, "layout");

// The squared distance from the manager's listener (the three words at +0x21e8, typed u32 in the
// manager's header) to `pos`.
static inline f32 DistanceSqToListener(const UnkRumbleManager* m, const UnkVec3f& pos) {
    const f32* listener = reinterpret_cast<const f32*>(&m->field_0x21e8);
    f32 dx = listener[0] - pos.x;
    f32 dy = listener[1] - pos.y;
    f32 dz = listener[2] - pos.z;
    return dx * dx + dy * dy + dz * dz;
}

void fn_71009da588(UnkRumbleComponent_l9da588* self) {
    for (UnkRumbleEntry_l9da588 *it = self->entries, *end = it + self->count; it != end; ++it) {
        if (!it->Get() && !it->params.loop && !it->stopped) {
            it->stopped = true;
            continue;
        }
        if (it->Get() || !it->params.loop || it->stopped || !(it->params.strength > 0.0f))
            continue;
        UnkRumblePlayParams_l9da588 params = it->params;
        s32 index = params.scale_index;
        f32 scale = index != 0x7fffffff ? self->scales[index] : 1.0f;
        params.strength = scale * params.strength;
        if (params.range < params.field_0x24 ||
            DistanceSqToListener(RumbleManager(), params.pos) < params.range * params.range)
            it->handle = fn_7100074354(RumbleManager(), reinterpret_cast<const UnkRumbleParams*>(&params));
    }
}
