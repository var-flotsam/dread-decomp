// CRumbleComponent's unit (tools/tu_map.py unit 1500): starts a rumble at a place when the manager is enabled:
// the point the owner's component at +0x228 gives for `name` (when the name is set and is not one of two ids of
// the game's table), or else the owner's position; hands the rumble the component's field at +0x40, moves it
// there and records it with the name and two flags. Returns its handle (the null handle when disabled).
// Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/input/UnkRumbleManager.h"
#include "base/math/UnkZeroVector3.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU50.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;
using base::input::UnkRumbleManager;

struct CRumbleComponent;
struct UnkHyperBeamEffectDesc;
struct UnkHyperBeamEffectRef;
struct UnkHyperBeamEffectHandle {
    UnkHyperBeamEffectRef* ref;
    s64 serial;
};

// A handle to a playing rumble: current while the target's serial is the handle's.
struct UnkHandleTarget {
    void* field_0x0;
    u64 field_0x8;
};

struct UnkHandle {
    UnkHandleTarget* target;
    u64 serial;
};

// The rumble while the handle is current.
static inline void* CurrentRumble(const UnkHandle& handle) {
    if (!handle.target || handle.serial == ~0ull || handle.target->field_0x8 != handle.serial)
        return nullptr;
    return handle.target->field_0x0;
}

// The rumble manager, created on first use.
static inline UnkRumbleManager* RumbleManager() {
    if (!g_unk_7101d47348) {
        UnkRumbleManager* m = static_cast<UnkRumbleManager*>(fn_71000839a0(sizeof(UnkRumbleManager), 0x3f));
        fn_71000733e4(m);
        g_unk_7101d47348 = m;
    }
    return g_unk_7101d47348;
}

// The rumble's file: a CFilePathStrId assigned as CStrId is (on the file-path pool).
struct UnkRumblePath_l9da8f4 : CFilePathStrId {
    UnkRumblePath_l9da8f4& operator=(const UnkRumblePath_l9da8f4& other) {
        if (data != other.data) {
            UnkStringData* d = other.data;
            UnkStringPool* pool = g_unk_7101d47200;
            if (d != &pool->empty)
                ++d->ref_count;
            if (data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0)
                    StringPool_Release(pool, data);
            }
            data = d;
        }
        return *this;
    }
};

struct UnkRumbleWords3_l9da8f4 {
    u32 x, y, z;
    UnkRumbleWords3_l9da8f4& operator=(const UnkRumbleWords3_l9da8f4& o) {
        x = o.x;
        y = o.y;
        z = o.z;
        return *this;
    }
};

struct UnkRumbleWord_l9da8f4 {
    u64 value;
    UnkRumbleWord_l9da8f4& operator=(const UnkRumbleWord_l9da8f4& o) {
        value = o.value;
        return *this;
    }
};

// What a rumble is played from (0x48 bytes): its file and settings.
struct UnkRumbleParams_l9da8f4 {
    UnkRumblePath_l9da8f4 path;
    u64 field_0x8;
    u32 field_0x10;
    UnkRumbleWords3_l9da8f4 field_0x14;
    UnkRumbleWord_l9da8f4 field_0x20;
    u64 field_0x28;
    u64 field_0x30;
    u64 field_0x38;
    u16 field_0x40;
};

bool fn_710007453c(UnkRumbleManager* self);
struct UnkRumbleParams;
UnkHandle fn_7100074354(UnkRumbleManager* self, const UnkRumbleParams* params);
void fn_7100072030(void* rumble, void* arg);
void fn_7100072484(char* p0, char* p1);

// A rumble the component started (0x68 bytes): its handle, what it was played from, the name and flags.
struct UnkRumbleRecord_l9da8f4 {
    UnkHandle handle;
    UnkRumbleParams_l9da8f4 params;
    CStrId name;  // 0x58
    bool field_0x60;
    bool field_0x61;
    bool field_0x62;
};
static_assert(sizeof(UnkRumbleRecord_l9da8f4) == 0x68, "UnkRumbleRecord_l9da8f4 layout");

struct UnkRumbleRecords_l9da8f4 : base::global::CRntVector<UnkRumbleRecord_l9da8f4> {
    UnkRumbleRecord_l9da8f4& Back() {
        u32 n = count;
        UnkRumbleRecord_l9da8f4* end = data + n;
        return n ? end[-1] : *end;
    }
};

struct UnkRumbleRecords;
void CRumbleComponent_ResizeRumbleEntries(UnkRumbleRecords* self, u32 count);  // resizes

struct UnkRumbleOwner_l9da8f4 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x228 - 0x68];
    void* field_0x228;
};

struct UnkRumbleComponent_l9da8f4 {
    u8 pad_0x0[0x30];
    UnkRumbleOwner_l9da8f4* owner;  // 0x30
    u8 pad_0x38[0x40 - 0x38];
    u8 field_0x40[8];
    UnkRumbleRecords_l9da8f4 records;  // 0x48
};

// The id table's slot at +0x2128 (UnkGameManagerIdTable has no member for it yet).
struct UnkIdTableSlots_l9da8f4 {
    u8 pad_0x0[0x2128];
    CStrId field_0x2128;
};

static inline const CStrId& IdTableSlot2128() {
    return reinterpret_cast<const UnkIdTableSlots_l9da8f4&>(GameManagerIdTable()).field_0x2128;
}

// Whether two string ids are the same.
static inline bool SameId(const CStrId& a, const CStrId& b) {
    return a.data == b.data;
}

UnkHyperBeamEffectHandle fn_71009da8f4(CRumbleComponent* component, const UnkHyperBeamEffectDesc* desc,
                                       const CStrId* name, u32 a, u32 b, u32 c) {
    UnkRumbleComponent_l9da8f4* self = reinterpret_cast<UnkRumbleComponent_l9da8f4*>(component);
    const UnkRumbleParams_l9da8f4* params = reinterpret_cast<const UnkRumbleParams_l9da8f4*>(desc);
    if (!fn_710007453c(RumbleManager()))
        return reinterpret_cast<const UnkHyperBeamEffectHandle&>(g_unk_7101d586c0);

    UnkVec3f point(0.0f, 0.0f, 0.0f);
    bool found = false;
    // The name gives a point of its own when it is set and is neither of two ids of the table.
    if (name->data != &g_unk_7101d47108->empty) {
        void* target = self->owner->field_0x228;
        if (!SameId(*name, IdTableSlot2128()) && !SameId(*name, GameManagerIdTable().field_0xd40) && target) {
            u64 key = name->data->Key();
            found = fn_710117364c(target, &key, reinterpret_cast<UnkGrappleAnswer_b50*>(&point));
        }
    }
    if (!found) {
        UnkRumbleOwner_l9da8f4* owner = self->owner;
        point = UnkVec3f(owner->x, owner->y, owner->z);
    }

    UnkHandle handle = fn_7100074354(RumbleManager(), reinterpret_cast<const UnkRumbleParams*>(params));
    if (void* rumble = CurrentRumble(handle)) {
        fn_7100072030(rumble, self->field_0x40);
        fn_7100072484(static_cast<char*>(CurrentRumble(handle)), reinterpret_cast<char*>(&point));
    }
    CRumbleComponent_ResizeRumbleEntries(reinterpret_cast<UnkRumbleRecords*>(&self->records), self->records.count + 1);
    UnkRumbleRecord_l9da8f4& r = self->records.Back();
    r.handle = handle;
    r.params = *params;
    r.name = *name;
    r.field_0x60 = a & 1;
    r.field_0x61 = b & 1;
    r.field_0x62 = true;
    return reinterpret_cast<const UnkHyperBeamEffectHandle&>(handle);
}
