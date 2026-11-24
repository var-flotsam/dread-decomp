// CCharClassPlayVibrationEvent's unit (tools/tu_map.py): the event's run, creating the effect (fn_7100d7c614) from a description as src/game/CHyperBeamGun.cpp builds one.
// Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/math/UnkZeroVector3.h"
#include "game/UnkActor.h"
#include "game/UnkTU2523.h"
#include "game/UnkXformNode.h"
#include "types.h"
#include "game/UnkHyperBeamEffectHandle.h"

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
                    StringPool_Release(pool, data);
            }
            data = d;
        }
        return *this;
    }
    ~UnkVibrationPathId_ovb0137() {
        if (UnkStringPool* pool = g_unk_7101d47200) {
            if (data && data != &pool->empty) {
                if (--data->ref_count == 0) {
                    StringPool_Release(pool, data);
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
struct UnkHyperBeamEffectDesc;

struct UnkPlayVibrationEvent_ovb0137 {
    u8 pad_0x0[0x10];
    UnkVibrationPathId_ovb0137 path;  // 0x10
    CStrId name;                      // 0x18
    u32 eType;
    f32 field_0x24[4];
    f32 fPan;
    f32 fFadeInTime;
    f32 fFadeOutTime;
    f32 fSpeed;
    bool b3D;
    bool bLoop;
    bool bStopOnEntityDead;
    bool bStopOnChangeAnim;
};

struct UnkVibrationContext_ovb0137 {
    UnkActor* owner;
};

// Starts the event's vibration effect (its file path at +0x10) on the owner.
void CCharClassPlayVibrationEvent_PlayVibrationOnOwner(UnkPlayVibrationEvent_ovb0137* self, UnkVibrationContext_ovb0137* ctx) {
    if (self->path.data == &g_unk_7101d47200->empty || !ctx->owner)
        return;
    UnkVibrationDesc_ovb0137 desc;
    desc.path = self->path;
    desc.field_0x8 = 0;
    desc.field_0x10 = self->eType;
    desc.field_0x14 = reinterpret_cast<const UnkVec3f&>(g_unk_710164779c);
    desc.field_0x20[0] = self->field_0x24[0];
    desc.field_0x20[1] = self->field_0x24[1];
    desc.field_0x20[2] = self->field_0x24[2];
    desc.field_0x20[3] = self->field_0x24[3];
    desc.field_0x30 = self->fPan;
    desc.field_0x34 = self->fFadeInTime;
    desc.field_0x38 = self->fFadeOutTime;
    desc.field_0x3c = self->fSpeed;
    desc.field_0x40 = self->b3D;
    desc.field_0x41 = self->bLoop;
    fn_7100d7c614(ctx->owner, reinterpret_cast<const UnkHyperBeamEffectDesc*>(&desc), &self->name, self->bStopOnChangeAnim,
                  self->bStopOnEntityDead, 1);
}
