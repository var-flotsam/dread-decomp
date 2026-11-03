// CFlockingSwarmControllerComponent unit (tools/tu_map.py unit 1158): fills in the value at +0x14 of the
// list's entry `index` (0x20-byte entries) when it is not positive yet: the entry's position (vtable slot
// 0x18) projected to 2D is measured by fn_71007399c4. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "types.h"

struct UnkSwarmEntry_fsc {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual const base::math::CVector3D* vf_0x18();
    u8 pad_0x8[0xc];
    f32 value;  // 0x14
    u8 pad_0x18[8];
};

struct UnkSwarmList_fsc {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual u32 vf_0x28();
    UnkSwarmEntry_fsc* entries;  // 0x8
};

struct UnkFlockingSwarm_fsc;
f32 fn_71007399c4(UnkFlockingSwarm_fsc* self, const base::math::CVector2D* pos);

void fn_7100739bdc(UnkFlockingSwarm_fsc* self, u32 index, UnkSwarmList_fsc* list) {
    if (index < list->vf_0x28()) {
        UnkSwarmEntry_fsc* entry = &list->entries[index];
        if (!(entry->value > 0.0f)) {
            base::math::CVector2D pos;
            fn_710007b430(&pos, entry->vf_0x18());
            entry->value = fn_71007399c4(self, &pos);
        }
    }
}
