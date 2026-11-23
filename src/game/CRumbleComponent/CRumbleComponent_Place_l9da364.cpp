// CRumbleComponent's unit (tools/tu_map.py unit 1500): placing the rumbles marked for it. Each one
// takes the point the owner's component at +0x228 gives for the rumble's name (when the name is set
// and is not one of two ids of the game's table), or else the owner's position, and its rumble is
// moved there (fn_7100072484). Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/global/UnkStringPoolBody.h"
#include "base/math/UnkZeroVector3.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkRumbleTarget_ovb0083.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringEntry;
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

void fn_7100072484(char* p0, char* p1);

struct UnkRumbleOwner_l9da364 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x228 - 0x68];
    void* field_0x228;
};

struct UnkRumbleComponent_l9da364 {
    u8 pad_0x0[0x30];
    UnkRumbleOwner_l9da364* owner;  // 0x30
    u8 pad_0x38[0x48 - 0x38];
    UnkRumbleEntries_ovb0083 entries;  // 0x48
};

// The id table's slot at +0x2128 (a string id; UnkGameManagerIdTable has no member for it yet).
static inline const CStrId& IdTableSlot2128() {
    return *reinterpret_cast<const CStrId*>(reinterpret_cast<const u8*>(&GameManagerIdTable()) + 0x2128);
}

// Whether two string ids are the same.
template <class A, class B>
static inline bool SameId(const A& a, const B& b) {
    return a.data == b.data;
}

void fn_71009da364(UnkRumbleComponent_l9da364* self) {
    for (UnkRumbleEntry_ovb0083 *it = self->entries.data, *end = it + self->entries.count; it != end; ++it) {
        if (!it->restart)
            continue;
        UnkVec3f point(0.0f, 0.0f, 0.0f);
        void* target = self->owner->field_0x228;
        bool found = false;
        // The name gives a point of its own when it is set and is neither of two ids of the table.
        if (target && !SameId(it->name, base::global::CStrId::Empty()) && !SameId(it->name, GameManagerIdTable().field_0xd40) &&
            !SameId(it->name, IdTableSlot2128())) {
            u64 hash = reinterpret_cast<const UnkStringEntry*>(it->name.data)->buffer.key;
            found = fn_710117364c(target, &hash, reinterpret_cast<UnkGrappleAnswer_b50*>(&point));
        }
        if (found) {
            it->field_0x24 = point;
        } else {
            UnkRumbleOwner_l9da364* owner = self->owner;
            it->field_0x24 = UnkVec3f(owner->x, owner->y, owner->z);
        }
        if (void* rumble = it->Get())
            fn_7100072484(static_cast<char*>(rumble), reinterpret_cast<char*>(&it->field_0x24));
    }
}
