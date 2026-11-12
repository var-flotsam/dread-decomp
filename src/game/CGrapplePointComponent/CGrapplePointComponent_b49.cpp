// CGrapplePointComponent unit (tools/tu_map.py): fn_7100822888 restores the point from the given
// blackboard's section of the current scenario: the flag (g_unk_71727235d0) goes to vtable slot 0x168
// and the "Position" (a base::math::CVector3D) to slot 0x170, each only when it was read (keys looked up
// as in CGrapplePointComponent_b48.cpp). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "game/CBlackboard.h"
#include "game/CGrapplePointComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CStrId;

CStrId fn_71011a2fbc(UnkGameManagerSub_0x608*);

// Reads the vector stored under `name` into `out`; false when there is none.
bool fn_7100395d3c(CBlackboard::CSection* self, const CStrId* name, base::math::CVector3D* out);

struct UnkGrapplePointValue_b49 {
    CStrId id;
    u64 field_0x8;
};

struct UnkGrapplePoint_b49;
struct UnkGrapplePointVtable_b49 {
    u8 pad_0x0[0x168];
    void (*vf_0x168)(UnkGrapplePoint_b49* self, bool flag);
    void (*vf_0x170)(UnkGrapplePoint_b49* self, base::math::CVector3D* position);
};

struct UnkGrapplePoint_b49 {
    UnkGrapplePointVtable_b49* vtable;
    u8 pad_0x8[0x50];
    UnkGrapplePointValue_b49* values;
    u64* keys;
    u32 count;

    __attribute__((always_inline)) const CStrId* Find(const UnkProperty& property) {
        u64 index = 0;
        u32 n = count;
        if (n) {
            u64* k = keys;
            u64 key = (u64)property.name.data;
            for (; index != n; ++index) {
                if (k[index] == key)
                    break;
            }
        }
        return &values[index].id;
    }
};

void fn_7100822888(UnkGrapplePoint_b49* self, CBlackboard* blackboard) {
    CBlackboard::CSection* section = fn_71010472e0(blackboard, fn_71011a2fbc(g_unk_7101d47310->field_0x608));
    if (!section)
        return;
    bool flag = false;
    if (fn_710006d11c(section, self->Find(g_unk_71727235d0), &flag))
        self->vtable->vf_0x168(self, flag);
    base::math::CVector3D position;
    *(u64*)&position = 0;  // x and y as one word, then z
    position.z = 0.0f;
    if (fn_7100395d3c(section, self->Find(g_unk_71727235f8), &position)) {
        base::math::CVector3D copy = position;
        self->vtable->vf_0x170(self, &copy);
    }
}
