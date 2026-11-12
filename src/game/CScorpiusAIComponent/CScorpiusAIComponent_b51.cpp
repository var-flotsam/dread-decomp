// CScorpiusAIComponent: the position of the hit object when the owner's collision container maps
// it to the id at 0x7172704230 (its matrix's translation column). Names are placeholders.
#include "base/global/CStrId.h"
#include "game/CCollisionComponent.h"
#include "game/UnkCollisionRecord.h"
#include "types.h"

using base::global::CStrId;

extern CStrId g_unk_7172704230 __attribute__((visibility("hidden")));

struct UnkScorpiusOwner_b51 {
    u8 pad_0x0[0x2a0];
    UnkCollisionContainer* field_0x2a0;
};

struct UnkScorpiusAI_b51 {
    u8 pad_0x0[0x30];
    UnkScorpiusOwner_b51* owner;
};

struct UnkMatrix34_b51 {
    f32 m[3][4];
};

struct UnkHitObject_b51 {
    virtual void vf_0x0(); virtual void vf_0x8(); virtual void vf_0x10(); virtual void vf_0x18();
    virtual void vf_0x20(); virtual void vf_0x28(); virtual void vf_0x30(); virtual void vf_0x38();
    virtual void vf_0x40(); virtual void vf_0x48(); virtual void vf_0x50(); virtual void vf_0x58();
    virtual void vf_0x60(); virtual void vf_0x68();
    virtual const UnkMatrix34_b51* vf_0x70();
};

struct UnkHitEntry_b51 {
    u8 pad_0x0[0x10];
    UnkHitObject_b51* object;
};

struct UnkVec3_b51s {
    f32 x, y, z;
    UnkVec3_b51s(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
};

void fn_7100698070(UnkScorpiusAI_b51* self, UnkHitEntry_b51* const* hit, UnkVec3_b51s* out) {
    UnkCollisionContainer* collision = self->owner->field_0x2a0;
    if (!collision)
        return;
    UnkHitEntry_b51* entry = *hit;
    const UnkCollisionObject* object =
        entry ? reinterpret_cast<const UnkCollisionObject*>(entry->object) : nullptr;
    if (fn_7100864018(collision, const_cast<UnkCollisionObject*>(object))->data == g_unk_7172704230.data) {
        const UnkMatrix34_b51* m = (*hit)->object->vf_0x70();
        *out = UnkVec3_b51s(m->m[0][3], m->m[1][3], m->m[2][3]);
    }
}
