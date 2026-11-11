// CAreaFXComponent unit (tools/tu_map.py unit 1263). fn_71007fe39c: after the base step
// (fn_71007f900c) succeeds, caches the object at +0x10 at +0x3f8 when it is a
// CCharClassFrozenAsPlatformComponent, and resets the state after it (+0x400, the id at +0x408 to
// the empty id, -1 at +0x410, +0x414, +0x3f4). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CFrozenComponent.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkAreaFX_b50 {
    u8 pad_0x0[0x10];
    UnkReflObject* field_0x10;
    u8 pad_0x18[0x3dc];
    bool field_0x3f4;
    u8 pad_0x3f5[0x3];
    UnkReflObject* field_0x3f8;
    bool field_0x400;
    u8 pad_0x401[0x7];
    CStrId field_0x408;
    f32 field_0x410;
    bool field_0x414;
};

bool fn_71007fe39c(UnkAreaFX_b50* self, void* arg) {
    if (!fn_71007f900c(self, arg))
        return false;
    UnkReflObject* object = self->field_0x10;
    self->field_0x3f8 = object && fn_710009d50c(object->vf_0x0(), fn_71007fbc5c()) ? object : nullptr;
    self->field_0x400 = false;
    self->field_0x408 = g_unk_7101d470e0;
    self->field_0x410 = -1.0f;
    self->field_0x414 = false;
    self->field_0x3f4 = false;
    return true;
}

// fn_71007fe318: after the base step (fn_71007f954c), drops the references in the list at +0x438
// (count +0x440) and empties it.
struct UnkRefObj_b50fx {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    union {
        u32 ref_count;
    };
};

struct UnkAreaFXRefs_b50 {
    u8 pad_0x0[0x438];
    UnkRefObj_b50fx** refs;  // 0x438
    u32 ref_count;           // 0x440
};

void fn_71007f954c(UnkAreaFXRefs_b50* self);

void fn_71007fe318(UnkAreaFXRefs_b50* self) {
    fn_71007f954c(self);
    for (u64 i = 0; i < self->ref_count; ++i) {
        if (UnkRefObj_b50fx* ref = self->refs[i]) {
            if (--ref->ref_count == 0)
                ref->vf_0x10();
        }
    }
    self->ref_count = 0;
}
