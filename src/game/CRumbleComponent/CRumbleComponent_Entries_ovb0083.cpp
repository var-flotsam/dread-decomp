// CRumbleComponent's unit (tools/tu_map.py unit 1500): the component's record list (+0x48, a
// base::UnkVector of 0x68-byte records; its shrink and resize) and the stop of the rumbles played
// from a given file. Names and layouts are placeholders; the records
// are the ones src/game/CRumbleComponent.cpp fills (UnkRumbleRecord there), seen here with their
// default values and the two string ids released in two steps.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/input/UnkRumbleManager.h"
#include "base/math/UnkZeroVector3.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;
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
                    fn_71000803a4(pool, data);
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

using UnkRumbleEntries_ovb0083 = base::UnkVector<UnkRumbleEntry_ovb0083>;

struct UnkRumbleComponent_ovb0083 {
    u8 unk_0x0[0x48];
    UnkRumbleEntries_ovb0083 entries;  // 0x48
};

struct UnkRumbleRecords;
void fn_7100072328(void*, f32, f32);

// Destroys the entries past `n` and keeps the first n.
void fn_71009dbbb4(UnkRumbleEntries_ovb0083* self, u32 n) {
    for (u64 i = n; i < self->count; ++i)
        self->data[i].~UnkRumbleEntry_ovb0083();
    self->count = n;
}

void fn_71009dbeec(UnkRumbleEntries_ovb0083* self, u32 n);  // grows the array to n entries

// Resizes to n entries, default-constructing the new ones.
void fn_71009dbdb8(UnkRumbleRecords* records, u32 n) {
    UnkRumbleEntries_ovb0083* self = reinterpret_cast<UnkRumbleEntries_ovb0083*>(records);
    fn_71009dbeec(self, n);
    for (u64 i = self->count; i < n; ++i)
        new (&self->data[i]) UnkRumbleEntry_ovb0083();
    self->count = n;
}

// Stops (fades out over 0.2) every rumble played from `path` and marks its entry.
void fn_71009da834(UnkRumbleComponent_ovb0083* self, const CFilePathStrId* path) {
    for (UnkRumbleEntry_ovb0083 *it = self->entries.data, *end = it + self->entries.count; it != end; ++it) {
        if (it->path.data != path->data)
            continue;
        if (void* rumble = it->Get())
            fn_7100072328(rumble, 0.2f, 0.0f);
        it->stopped = true;
    }
}
