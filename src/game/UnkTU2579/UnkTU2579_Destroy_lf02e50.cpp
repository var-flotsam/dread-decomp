// Unit 2579 (tools/tu_map.py): the sensor owner's destructor body (fn_7100f02e50): its string-id dictionary
// is cleared and its buckets freed, then the array of shared objects and the sensor array are freed.
// Names and layouts are placeholders.
#include "base/global/CRntDictionary.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

extern char g_unk_7101caa278[];
extern char g_unk_7101caa2d8[];

// A dictionary value: its own vtable and a second id.
struct UnkSensorEntry_lf02e50 {
    char* vtable;
    CStrId id;  // 0x8
    u64 field_0x10;

    ~UnkSensorEntry_lf02e50() { vtable = g_unk_7101caa2d8 + 0x10; }
};

// A reference-counted object; the last release destroys it through its second virtual.
struct UnkShared_lf02e50 {
    virtual void vf_0x0();
    virtual void Destroy();  // 0x8
    u8 pad_0x8[0x28];
    u32 refs;  // 0x30
};

struct UnkSharedPtr_lf02e50 {
    UnkShared_lf02e50* ptr;

    ~UnkSharedPtr_lf02e50() {
        if (UnkShared_lf02e50* held = ptr; held && --held->refs == 0)
            held->Destroy();
    }
};

struct UnkSensorOwner_lf02e50 {
    char* vtable;
    base::global::CRntVector<UnkReflObject*> sensors;  // 0x8
    base::global::CRntVector<UnkSharedPtr_lf02e50> shared;  // 0x30
    base::global::CRntDictionary<CStrId, UnkSensorEntry_lf02e50> entries;  // 0x58
};
static_assert(__builtin_offsetof(UnkSensorOwner_lf02e50, entries) == 0x58, "layout");

void fn_7100f02e50(UnkSensorOwner_lf02e50* self) {
    self->vtable = g_unk_7101caa278 + 0x10;
    self->entries.Clear();
    self->entries.buckets.Free();
    self->shared.Free();
    self->sensors.Free();
}
