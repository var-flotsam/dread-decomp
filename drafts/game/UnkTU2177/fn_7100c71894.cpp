// UnkTU2177: walks a growable list of path nodes after a start index and returns the first whose owner is of
// kind 4 and whose position lies on a 2D segment. Names and layouts are placeholders.
#include <new>

#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "types.h"

struct UnkTU2177Kinded {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual s32 vf_0x18();
};

struct UnkTU2177Owner {
    u8 pad_0x0[0x158];
    UnkTU2177Kinded* field_0x158;
};

struct UnkTU2177Entity {
    u8 pad_0x0[0x10];
    UnkTU2177Owner* field_0x10;
};

#pragma pack(push, 4)
struct UnkTU2177Node {
    UnkTU2177Node()
        : entity(nullptr), field_0x8(0), handle(-1), pos{0.0f, 0.0f, 0.0f}, field_0x20{0.0f, 0.0f, 0.0f},
          field_0x2c{0.0f, 0.0f, 0.0f}, field_0x38{0.0f, 0.0f, 0.0f}, field_0x48(false) {}

    UnkTU2177Entity* entity;
    u64 field_0x8;
    s32 handle;
    base::math::CVector3D pos;
    base::math::CVector3D field_0x20;
    base::math::CVector3D field_0x2c;
    base::math::CVector3D field_0x38;
    u32 field_0x44;  // not set by the constructor
    bool field_0x48;
    u32 field_0x4c;  // not set by the constructor
};
static_assert(sizeof(UnkTU2177Node) == 0x50, "");

struct UnkTU2177NodeList {
    // Grows the list up to and including i, constructing the new nodes.
    UnkTU2177Node& At(u32 i) {
        if (i >= count) {
            for (u32 j = count; j < i + 1; ++j)
                new (&items[j]) UnkTU2177Node();
            count = i + 1;
        }
        return items[i];
    }

    u32 count;
    UnkTU2177Node items[16];
};
#pragma pack(pop)

bool fn_7100c71894(void* self, const UnkSegment2D* segment, void* target, u32 start, UnkTU2177NodeList* nodes,
                   u32* found) {
    u32 count = nodes->count;
    for (u32 i = start + 1; i < count; ++i) {
        if (!nodes->At(i).field_0x48) {
            UnkTU2177Kinded* kinded = nodes->At(i).entity->field_0x10->field_0x158;
            s32 kind = kinded->vf_0x18();
            if (kinded != nullptr && kind == 4) {
                base::math::CVector2D point;
                fn_710007b430(&point, &nodes->At(i).pos);
                if (fn_7100125c64(segment, &point, true, 0.01f)) {
                    *found = i;
                    return true;
                }
            }
        }
    }
    return false;
}
