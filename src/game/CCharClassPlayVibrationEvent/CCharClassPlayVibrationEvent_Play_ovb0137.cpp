// CCharClassPlayVibrationEvent's unit (tools/tu_map.py): the event's run, creating the effect (fn_7100d7c614) from a description as src/game/CHyperBeamGun.cpp builds one.
// Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/math/UnkZeroVector3.h"
#include "game/UnkActor.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;
using base::math::CVector3D;

// A file-path id (pool g_unk_7101d47200) whose count is a plain int.
struct UnkVibrationPathId_ovb0137 {
    UnkStringData* data;

    explicit UnkVibrationPathId_ovb0137(const base::global::UnkFilePathStrIdBase& other) : data(other.data) {
        if (&g_unk_7101d47200->empty != data)
            ++data->ref_count;
    }
    UnkVibrationPathId_ovb0137& operator=(const UnkVibrationPathId_ovb0137& other) {
        if (other.data != data) {
            UnkStringData* d = other.data;
            UnkStringPool* pool = g_unk_7101d47200;
            if (d != &pool->empty)
                ++d->ref_count;
            if (data != &pool->empty) {
                if (--data->ref_count == 0)
                    fn_71000803a4(pool, data);
            }
            data = d;
        }
        return *this;
    }
    ~UnkVibrationPathId_ovb0137() {
        if (UnkStringPool* pool = g_unk_7101d47200) {
            if (data && data != &pool->empty) {
                if (--data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47200;
                }
            }
            data = &pool->empty;
        }
    }
};

// What the vibration effect is created from (the effect description of fn_7100d7c614, as
// src/game/CHyperBeamGun.cpp builds it).
struct UnkVibrationDesc_ovb0137 {
    UnkVibrationPathId_ovb0137 path;
    u64 field_0x8;
    u32 field_0x10;
    UnkVec3f field_0x14;
    f32 field_0x20[4];
    f32 field_0x30;
    f32 field_0x34;
    f32 field_0x38;
    f32 field_0x3c;
    bool field_0x40;
    bool field_0x41;

    UnkVibrationDesc_ovb0137()
        : path(g_unk_7101d470e8), field_0x8(0), field_0x10(0x7fffffff), field_0x14(reinterpret_cast<const UnkVec3f&>(g_unk_710164779c)),
          field_0x20{0.0f, 0.0f, 1.0f, 1.0f}, field_0x30(0.5f), field_0x34(0.0f), field_0x38(0.0f),
          field_0x3c(1.0f), field_0x40(false), field_0x41(false) {}
};

struct UnkHyperBeamEffectRef;
struct UnkHyperBeamEffectHandle {
    UnkHyperBeamEffectRef* ref;
    s64 serial;
};
struct UnkHyperBeamEffectDesc;
UnkHyperBeamEffectHandle fn_7100d7c614(UnkActor* owner, const UnkHyperBeamEffectDesc* desc, const CStrId* name,
                                       u32 a, u32 b, u32 c);

struct UnkPlayVibrationEvent_ovb0137 {
    u8 pad_0x0[0x10];
    UnkVibrationPathId_ovb0137 path;  // 0x10
    CStrId name;                      // 0x18
    u32 field_0x20;
    f32 field_0x24[4];
    f32 field_0x34;
    f32 field_0x38;
    f32 field_0x3c;
    f32 field_0x40;
    bool field_0x44;
    bool field_0x45;
    bool field_0x46;
    bool field_0x47;
};

struct UnkVibrationContext_ovb0137 {
    UnkActor* owner;
};

// Starts the event's vibration effect (its file path at +0x10) on the owner.
void fn_7100ccc95c(UnkPlayVibrationEvent_ovb0137* self, UnkVibrationContext_ovb0137* ctx) {
    if (self->path.data == &g_unk_7101d47200->empty || !ctx->owner)
        return;
    UnkVibrationDesc_ovb0137 desc;
    desc.path = self->path;
    desc.field_0x8 = 0;
    desc.field_0x10 = self->field_0x20;
    desc.field_0x14 = reinterpret_cast<const UnkVec3f&>(g_unk_710164779c);
    desc.field_0x20[0] = self->field_0x24[0];
    desc.field_0x20[1] = self->field_0x24[1];
    desc.field_0x20[2] = self->field_0x24[2];
    desc.field_0x20[3] = self->field_0x24[3];
    desc.field_0x30 = self->field_0x34;
    desc.field_0x34 = self->field_0x38;
    desc.field_0x38 = self->field_0x3c;
    desc.field_0x3c = self->field_0x40;
    desc.field_0x40 = self->field_0x44;
    desc.field_0x41 = self->field_0x45;
    fn_7100d7c614(ctx->owner, reinterpret_cast<const UnkHyperBeamEffectDesc*>(&desc), &self->name, self->field_0x47,
                  self->field_0x46, 1);
}
