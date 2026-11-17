// CObsydomithonAIComponent's unit: which side of the owner (+0x30) `other` is on. The owner's point is
// its translation, or the transform its model (+0x228) gives for the singleton's id at +0xd60; the side
// is one of two ids by the sign of the offset along the owner's forward axis. Names and layouts are
// placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkActor.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CStrId;

__attribute__((visibility("hidden"))) extern CStrId g_unk_71726f9458;
__attribute__((visibility("hidden"))) extern CStrId g_unk_71726f9450;

struct UnkObsydomithonIdHolder_l62e858 {
    u8 pad_0x0[0x30];
    u64 id;
};

struct UnkObsydomithonAI_l62e858 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
};

CStrId fn_710062e858(UnkObsydomithonAI_l62e858* self, UnkActor* other) {
    if (!other)
        return g_unk_7101d470e0;
    UnkActor* owner = self->owner;
    UnkVec3f pos(owner->field_0x44, owner->field_0x54, owner->field_0x64);
    UnkEntity* model = static_cast<UnkEntity*>(owner ? owner->field_0x228 : nullptr);
    UnkMatrix44 m = {};
    m.m[15] = 1.0f;
    m.m[10] = 1.0f;
    m.m[5] = 1.0f;
    m.m[0] = 1.0f;
    if (model) {
        u64 id = (*reinterpret_cast<UnkObsydomithonIdHolder_l62e858**>(
                      reinterpret_cast<char*>(&base::reflection::refl_GetSingleton()) + 0xd60))->id;
        if (fn_71011735e8(model, &id, &m, 0))
            pos = UnkVec3f(m.m[3], m.m[7], m.m[11]);
    }
    f32 dx = pos.x - other->field_0x44;
    f32 dy = pos.y - other->field_0x54;
    f32 dz = pos.z - other->field_0x64;
    const UnkMatrix44& xform = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->owner->field_0x38));
    if (dx * xform.m[2] + dy * xform.m[6] + dz * xform.m[10] > 0.0f)
        return g_unk_71726f9458;
    return g_unk_71726f9450;
}
