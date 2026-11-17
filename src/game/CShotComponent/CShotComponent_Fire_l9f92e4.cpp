// CShotComponent's unit (tools/tu_map.py unit 1508): fires the first entry of the class definition
// that accepts the request (by key when keys are given), filling a launch state; when one fires and
// a callback is set, the callback decides whether the stored state is kept or replaced by the new one.
// Names and layouts are placeholders.
#include "game/UnkAssignedSlot.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkShotLaunchState;
void fn_71009f9d8c(UnkShotLaunchState* self);

struct UnkShotDef_ovb0066;
struct UnkShotEntry_ovb0066;
bool fn_71009f8d1c(UnkShotDef_ovb0066* self, const u64* key, UnkShotEntry_ovb0066** out);

struct UnkRefCounted_l9f92e4;

struct UnkRefCountedVtable_l9f92e4 {
    void* vf_0x0;
    void* vf_0x8;
    void (*vf_0x10)(UnkRefCounted_l9f92e4* self);
};

struct UnkRefCounted_l9f92e4 {
    const UnkRefCountedVtable_l9f92e4* vtable;
    s32 refs;
};

// Drops a reference taken by a slot.
static inline void Release(UnkAssignedSlot& slot) {
    UnkRefCounted_l9f92e4* obj = static_cast<UnkRefCounted_l9f92e4*>(slot.ptr);
    if (obj && --obj->refs == 0)
        obj->vtable->vf_0x10(obj);
}

struct UnkShotTarget_l9f92e4 {
    UnkAssignedSlot target;  // 0x50
    UnkAssignedSlot field_0x58;
    s32 index;  // 0x60
    UnkVec3f field_0x64;
    UnkVec3f field_0x70;
    UnkVec3f field_0x7c;
    UnkVec3f field_0x88;

    ~UnkShotTarget_l9f92e4() {
        Release(field_0x58);
        Release(target);
    }

    UnkShotTarget_l9f92e4& operator=(const UnkShotTarget_l9f92e4& o) {
        if (&o != this) {
            target = o.target;
            field_0x58 = o.field_0x58;
            index = o.index;
            field_0x64 = o.field_0x64;
            field_0x70 = o.field_0x70;
            field_0x7c = o.field_0x7c;
            field_0x88 = o.field_0x88;
        }
        return *this;
    }
};

struct UnkShotLaunch_l9f92e4 {
    UnkVec3f origin;     // 0x0
    UnkVec3f target;     // 0xc
    UnkVec3f direction;  // 0x18
    u8 field_0x24[0xc];
    bool field_0x30;
    bool field_0x31;
    u8 pad_0x32[2];
    f32 field_0x34;
    u32 field_0x38;
    u8 pad_0x3c[4];
    void* owner;  // 0x40
    bool primary;  // 0x48
    UnkShotTarget_l9f92e4 aim;  // 0x50

    UnkShotLaunch_l9f92e4() { fn_71009f9d8c(reinterpret_cast<UnkShotLaunchState*>(this)); }
};
static_assert(sizeof(UnkShotLaunch_l9f92e4) == 0x98, "size");

struct UnkShotEntry_l9f92e4;

struct UnkShotEntryVtable_l9f92e4 {
    void* vf_0x0;
    void* vf_0x8;
    void* vf_0x10;
    bool (*vf_0x18)(UnkShotEntry_l9f92e4* self, void* a, void* b, void* c, bool d, bool e, f32 g,
                    UnkShotLaunch_l9f92e4* out);
};

struct UnkShotEntry_l9f92e4 {
    const UnkShotEntryVtable_l9f92e4* vtable;

    bool Fire(void* a, void* b, void* c, bool d, bool e, UnkShotLaunch_l9f92e4* out, f32 g) {
        auto fire = vtable->vf_0x18;
        return fire(this, a, b, c, d, e, g, out);
    }
};

struct UnkShotDef_l9f92e4 {
    u8 unk_0x0[0x58];
    UnkShotEntry_l9f92e4** entries;  // 0x58
    u32 count;                       // 0x60
};

struct UnkShotKeys_l9f92e4 {
    u64* keys;
    u32 count;
};

struct UnkShotComponent_l9f92e4 {
    u8 pad_0x0[0x30];
    u64 field_0x30;
    u8 pad_0x38[8];
    UnkShotDef_l9f92e4* def;  // 0x40
    u64 field_0x48;
    bool (*callback)(u64 ctx, UnkShotLaunch_l9f92e4* current, UnkShotLaunch_l9f92e4* candidate, u64 arg);  // 0x50
    UnkShotLaunch_l9f92e4 launch;  // 0x58

    bool FireEntries(void* a, void* b, void* c, bool d, bool e, const UnkShotKeys_l9f92e4* keys, f32 g,
                     UnkShotLaunch_l9f92e4* out) {
        if (!def)
            return false;
        if (keys->count) {
            for (const u64 *key = keys->keys, *end = key + keys->count; key != end; ++key) {
                UnkShotEntry_l9f92e4* entry = nullptr;
                if (fn_71009f8d1c(reinterpret_cast<UnkShotDef_ovb0066*>(def), key,
                                  reinterpret_cast<UnkShotEntry_ovb0066**>(&entry)) &&
                    entry && entry->Fire(a, b, c, d, e, out, g))
                    return true;
            }
            return false;
        }
        UnkShotDef_l9f92e4* d0 = def;
        for (UnkShotEntry_l9f92e4 **it = d0->entries, **end = it + d0->count; it != end; ++it) {
            if (*it && (*it)->Fire(a, b, c, d, e, out, g))
                return true;
        }
        return false;
    }
};

void fn_71009f92e4(UnkShotComponent_l9f92e4* self, void* a, void* b, void* c, bool d, bool e,
                   const UnkShotKeys_l9f92e4* keys, f32 g) {
    UnkShotLaunch_l9f92e4 launch;
    if (self->FireEntries(a, b, c, d, e, keys, g, &launch) && self->callback) {
        u64 arg = self->field_0x30;
        u64 ctx = self->field_0x48;
        bool keep = self->callback(ctx, &self->launch, &launch, arg);
        self->launch = keep ? self->launch : launch;
    }
}
