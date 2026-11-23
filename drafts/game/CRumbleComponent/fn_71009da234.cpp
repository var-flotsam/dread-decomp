// CRumbleComponent's unit (tools/tu_map.py unit 1500): the component's update: when the rumble
// manager (created on first use) is active, the stopped records whose rumble has ended are erased,
// then the marked ones are placed and the rest updated. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/input/CRumbleManager/CTunableRumbleManager.h"
#include "base/input/UnkRumbleManager.h"
#include "base/math/UnkZeroVector3.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;
using base::input::UnkRumbleManager;

// A string id on `Pool`: copies retain, moves leave the empty id behind, and the release is done in
// two steps (the id and the count are reloaded after the decrement).
template <UnkStringPool*& Pool>
struct UnkRumbleStrId_ovb0083 {
    UnkStringData* data;

    explicit UnkRumbleStrId_ovb0083(UnkStringData* d) : data(d) {
        if (&Pool->empty != data)
            ++data->ref_count;
    }
    UnkRumbleStrId_ovb0083(UnkRumbleStrId_ovb0083&& other) : data(other.data) {
        other.data = &Pool->empty;
    }
    ~UnkRumbleStrId_ovb0083() {
        if (UnkStringPool* pool = Pool) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    StringPool_Release(pool, data);
                    pool = Pool;
                }
            }
            data = &pool->empty;
        }
    }
};

struct UnkRumbleTarget_ovb0083 {
    void* rumble;
    s64 id;
};

// A rumble the component started (0x68 bytes).
struct UnkRumbleEntry_ovb0083 {
    UnkRumbleTarget_ovb0083* target = nullptr;  // the rumble's handle, current while target->id == id
    s64 id = -1;
    UnkRumbleStrId_ovb0083<g_unk_7101d47200> path{g_unk_7101d470e8.data};  // 0x10
    u64 field_0x18 = 0;
    s32 field_0x20 = 0x7fffffff;
    UnkVec3f field_0x24 = reinterpret_cast<const UnkVec3f&>(g_unk_710164779c);
    f32 field_0x30 = 0.0f;
    f32 field_0x34 = 0.0f;
    f32 field_0x38 = 1.0f;
    f32 field_0x3c = 1.0f;
    f32 field_0x40 = 0.5f;
    f32 field_0x44 = 0.0f;
    f32 volume = 0.0f;  // 0x48
    f32 field_0x4c = 1.0f;
    bool field_0x50 = false;
    bool field_0x51 = false;
    UnkRumbleStrId_ovb0083<g_unk_7101d47108> name{g_unk_7101d470e0.data};  // 0x58
    bool field_0x60 = true;
    bool field_0x61 = true;
    bool restart = false;  // 0x62
    bool stopped = false;  // 0x63

    // The rumble while the handle is current.
    void* Get() const {
        if (!target || target->id != id || id == -1)
            return nullptr;
        return target->rumble;
    }
};
static_assert(sizeof(UnkRumbleEntry_ovb0083) == 0x68, "UnkRumbleEntry_ovb0083 layout");

using UnkRumbleEntries_ovb0083 = base::global::CRntVector<UnkRumbleEntry_ovb0083>;

struct UnkRumbleOwner_l9da364;

struct UnkRumbleComponent_l9da364 {
    u8 pad_0x0[0x30];
    UnkRumbleOwner_l9da364* owner;  // 0x30
    u8 pad_0x38[0x48 - 0x38];
    UnkRumbleEntries_ovb0083 entries;  // 0x48
};

void fn_71009dc0cc(UnkRumbleEntries_ovb0083* self, u32 index);
void fn_71009da364(UnkRumbleComponent_l9da364* self);
void fn_71009da588(UnkRumbleComponent_l9da364* self);

void fn_71009da234(UnkRumbleComponent_l9da364* self) {
    UnkRumbleManager* manager = g_unk_7101d47348;
    if (!manager) {
        manager = static_cast<UnkRumbleManager*>(fn_71000839a0(0x2210, 0x3f));
        fn_71000733e4(manager);
        g_unk_7101d47348 = manager;
    }
    if (!fn_710007453c(manager))
        return;
    UnkRumbleEntries_ovb0083* entries = &self->entries;
    UnkRumbleEntry_ovb0083* data = entries->data;
    u32 count = entries->count;
    UnkRumbleEntry_ovb0083* it = data;
    while (count != 0 && it != data + count) {
        if (!it->Get() && it->stopped) {
            u32 index = it - data;
            fn_71009dc0cc(entries, index);
            data = entries->data;
            it = &data[index];
            count = entries->count;
        } else {
            ++it;
        }
    }
    fn_71009da364(self);
    fn_71009da588(self);
}
