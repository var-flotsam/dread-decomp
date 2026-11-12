// CElectricGeneratorComponent's unit (tools/tu_map.py unit 1587): the destructor (fn_7100a7c7b0: the
// string id at +0x570, the array of string ids at +0x540, then the base destructor fn_7100a9e6bc) and
// the element-at-index of an array of 0x188-byte records (fn_7100a7cd10, UnkVector::GrowAt; the records
// are built, moved and destroyed out of line); and the update (fn_7100a7d594): after the base update
// (fn_7100aa3264) it raises the render parameter BLACKOUT_LEVEL's x or y (mode 0 or 1 at +0x578) to 1,
// then places the owner's scenario object (fn_7100774014) relative to the player: the player's position
// minus the owner's and the difference of their y angles (fn_710116c128). Names and layouts are
// placeholders.
#define UNK_VECTOR_GROW_AT_ATTR __attribute__((always_inline))
#include <new>

#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/math/CVector4D.h"
#include "game/CUsableComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU1176.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2611.h"
#include "game/UnkXformNode.h"
#include "types.h"

extern void* g_unk_7101a90bd0[];

struct UnkElectricGenerator_ovb0120 {
    char* vtable;
    u8 pad_0x8[0x538];
    base::UnkVector<base::UnkStrIdMember> ids;  // 0x540
    u8 pad_0x568[0x8];
    base::UnkStrIdMember id;  // 0x570
};
static_assert(__builtin_offsetof(UnkElectricGenerator_ovb0120, id) == 0x570, "UnkElectricGenerator_ovb0120 layout");

void fn_7100a7c7b0(UnkElectricGenerator_ovb0120* self) {
    self->vtable = reinterpret_cast<char*>(g_unk_7101a90bd0) + 0x10;
    self->id.~UnkStrIdMemberOf();
    self->ids.Free();
    fn_7100a9e6bc(reinterpret_cast<char*>(self));
}

struct UnkSubAreaRecord_0x188;
void fn_7100faa3d8(UnkSubAreaRecord_0x188* self);

// A 0x188-byte record built, moved and destroyed out of line.
struct UnkGeneratorRecord_ovb0120;
struct UnkGeneratorRecord_ovb0120 {
    u8 data[0x188];
    UnkGeneratorRecord_ovb0120() { fn_7100fa99bc(this); }
    UnkGeneratorRecord_ovb0120(UnkGeneratorRecord_ovb0120&& other) { fn_7100fab18c(this, &other); }
    ~UnkGeneratorRecord_ovb0120() { fn_7100faa3d8(reinterpret_cast<UnkSubAreaRecord_0x188*>(this)); }
};

UnkGeneratorRecord_ovb0120* fn_7100a7cd10(base::UnkVector<UnkGeneratorRecord_ovb0120>* self, u32 index) {
    return &self->GrowAt(index);
}

using base::math::CVector4D;

void fn_710021a0bc(UnkRenderManager* self, const u64* hash, CVector4D* value);
void fn_710021a0b4(UnkRenderManager* self, const u64* hash, const base::math::CVector4D* value);
struct UnkScenarioObject_ovb0032;
struct UnkPlayer;

struct UnkGeneratorActor_ovb0120 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
};

struct UnkGeneratorTarget_ovb0120;
void fn_710116c128(UnkGeneratorTarget_ovb0120* self, const UnkVec3f* offset, const UnkVec3f* angle, bool flag);

struct UnkGeneratorOwnerRef_ovb0120 {
    u8 pad_0x0[0x10];
    UnkGeneratorActor_ovb0120* actor;  // 0x10
};

struct UnkElectricGeneratorUpd_ovb0120 {
    u8 pad_0x0[0xe0];
    UnkGeneratorOwnerRef_ovb0120* owner;  // 0xe0
    u8 pad_0xe8[0x490];
    s32 mode;  // 0x578
    CVector4D blackout;  // 0x57c
};

void fn_7100aa3264(UnkElectricGeneratorUpd_ovb0120* self);

void fn_7100a7d594(UnkElectricGeneratorUpd_ovb0120* self) {
    fn_7100aa3264(self);
    {
        u64 hash = fn_7100001570("BLACKOUT_LEVEL", 14);
        fn_710021a0bc(g_unk_7101d47300, &hash, &self->blackout);
        switch (self->mode) {
        case 0:
            self->blackout.x = 1.0f;
            break;
        case 1:
            self->blackout.y = 1.0f;
            break;
        }
        fn_710021a0b4(g_unk_7101d47300, &hash, &self->blackout);
    }
    if (!self->owner || !self->owner->actor)
        return;
    UnkGeneratorTarget_ovb0120* target = static_cast<UnkGeneratorTarget_ovb0120*>(
        fn_7100774014(reinterpret_cast<UnkScenarioObject_ovb0032*>(self->owner->actor)));
    if (!target)
        return;
    UnkGeneratorActor_ovb0120* player = reinterpret_cast<UnkGeneratorActor_ovb0120*>(fn_7100dec788(g_unk_7101d47310));
    UnkGeneratorActor_ovb0120* actor = self->owner ? self->owner->actor : nullptr;
    if (!player || !actor)
        return;
    UnkVec3f from = player->xform.GetTranslation();
    UnkVec3f to = actor->xform.GetTranslation();
    UnkVec3f offset(from.x - to.x, from.y - to.y, from.z - to.z);
    f32 a = player->xform.GetAngle().y;
    f32 b = actor->xform.GetAngle().y;
    UnkVec3f angle(0.0f, a - b, 0.0f);
    fn_710116c128(target, &offset, &angle, true);
}
