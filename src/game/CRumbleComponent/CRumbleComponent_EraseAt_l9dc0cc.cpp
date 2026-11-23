// CRumbleComponent's unit (tools/tu_map.py unit 1500): removing one record from the component's
// record list (+0x48, a base::global::CRntVector of 0x68-byte records): it is destroyed and the later
// ones move down. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/math/UnkZeroVector3.h"
#include "game/UnkRumbleTarget_ovb0083.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

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
                    fn_71000803a4(pool, data);
                    pool = Pool;
                }
            }
            data = &pool->empty;
        }
    }
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
    UnkRumbleStrId_ovb0083<g_unk_7101d47108> name{base::global::CStrId::Empty().data};  // 0x58
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

// Destroys the entry at `index` and moves the later ones down by one.
void fn_71009dc0cc(UnkRumbleEntries_ovb0083* self, u32 index) {
    if (index < self->count) {
        self->data[index].~UnkRumbleEntry_ovb0083();
        for (u64 i = index; i < self->count - 1; ++i) {
            UnkRumbleEntry_ovb0083* at = &self->data[i];
            new (at) UnkRumbleEntry_ovb0083(static_cast<UnkRumbleEntry_ovb0083&&>(at[1]));
            (&self->data[i])[1].~UnkRumbleEntry_ovb0083();
        }
        self->count = self->count - 1;
    }
}
