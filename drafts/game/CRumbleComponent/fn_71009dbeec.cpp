// CRumbleComponent's unit (tools/tu_map.py unit 1500): growing the component's record list (+0x48,
// a base::global::CRntVector of 0x68-byte records), moving the records over and releasing the old
// ones' string ids. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/math/UnkZeroVector3.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

// Three floats copied one by one.
struct UnkRumbleOffset_l9dbeec {
    f32 x, y, z;
    UnkRumbleOffset_l9dbeec(const base::math::CVector3D& v) : x(v.x), y(v.y), z(v.z) {}
    UnkRumbleOffset_l9dbeec(const UnkRumbleOffset_l9dbeec& o) : x(o.x), y(o.y), z(o.z) {}
};

// A string id on `Pool`: copies retain, moves leave the empty id behind, and the release is done in
// two steps (the id and the count are reloaded after the decrement).
template <UnkStringPool*& Pool>
struct UnkRumbleStrId_l9dbeec {
    UnkStringData* data;

    explicit UnkRumbleStrId_l9dbeec(UnkStringData* d) : data(d) {
        if (&Pool->empty != data)
            ++data->ref_count;
    }
    UnkRumbleStrId_l9dbeec(UnkRumbleStrId_l9dbeec&& other) : data(other.data) {
        other.data = &Pool->empty;
    }
    ~UnkRumbleStrId_l9dbeec() {
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

struct UnkRumbleTarget_l9dbeec {
    void* rumble;
    s64 id;
};

// A rumble the component started (0x68 bytes).
struct UnkRumbleEntry_l9dbeec {
    UnkRumbleTarget_l9dbeec* target = nullptr;  // the rumble's handle, current while target->id == id
    s64 id = -1;
    UnkRumbleStrId_l9dbeec<g_unk_7101d47200> path{g_unk_7101d470e8.data};  // 0x10
    u64 field_0x18 = 0;
    s32 field_0x20 = 0x7fffffff;
    UnkRumbleOffset_l9dbeec field_0x24{g_unk_710164779c};
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
    UnkRumbleStrId_l9dbeec<g_unk_7101d47108> name{g_unk_7101d470e0.data};  // 0x58
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
static_assert(sizeof(UnkRumbleEntry_l9dbeec) == 0x68, "UnkRumbleEntry_l9dbeec layout");

using UnkRumbleEntries_l9dbeec = base::global::CRntVector<UnkRumbleEntry_l9dbeec>;

// Grows the array to n entries: the entries are moved to the new storage and the old ones destroyed.
void fn_71009dbeec(UnkRumbleEntries_l9dbeec* self, u32 n) {
    if (self->capacity < n) {
        UnkRumbleEntry_l9dbeec* p = static_cast<UnkRumbleEntry_l9dbeec*>(
            self->allocator->vf_0x40((u64)n * sizeof(UnkRumbleEntry_l9dbeec), 8, self->flags));
        if (UnkRumbleEntry_l9dbeec* src = self->data) {
            UnkRumbleEntry_l9dbeec* dst = p;
            for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
                new (dst) UnkRumbleEntry_l9dbeec(static_cast<UnkRumbleEntry_l9dbeec&&>(*src));
                src->~UnkRumbleEntry_l9dbeec();
            }
            self->allocator->vf_0x48(self->data);
        }
        self->data = p;
        self->capacity = n;
    }
}
