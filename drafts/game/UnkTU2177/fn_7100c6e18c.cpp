// UnkTU2177: from a start node, finds a later node in the same world slot (kind 0x10) whose position meets
// the start node's position moved by its collider's translation, and marks both nodes. Names and layouts are
// placeholders.
#include <new>

#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "game/UnkTU109.h"
#include "game/logic/collision/CCollider.h"
#include "types.h"

struct UnkStruct_0x48 {
    u8 unk_0x0[0x48];
};

struct UnkPool_0x48 {
    UnkStruct_0x48* slots;
    u32 count;
};

struct UnkTU2177World {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual s32 vf_0x18();
    u8 pad_0x8[0xa0];
    UnkPool_0x48 pool;  // 0xa8
};

struct UnkTU2177Owner {
    u8 pad_0x0[0x158];
    UnkTU2177World* world;
};

struct UnkTU2177Entity {
    u8 pad_0x0[0x10];
    UnkTU2177Owner* owner;
};

#pragma pack(push, 4)
struct UnkTU2177Node {
    UnkTU2177Node()
        : entity(nullptr), field_0x8(0), handle(-1), pos{0.0f, 0.0f, 0.0f}, field_0x20{0.0f, 0.0f, 0.0f},
          field_0x2c{0.0f, 0.0f, 0.0f}, field_0x38(0.0f), flags(0), field_0x40(0.0f),
          field_0x48(false) {}

    UnkTU2177Entity* entity;
    u64 field_0x8;
    s32 handle;
    base::math::CVector3D pos;
    base::math::CVector3D field_0x20;
    base::math::CVector3D field_0x2c;
    f32 field_0x38;
    u32 flags;  // 0x3c
    f32 field_0x40;
    u32 field_0x44;  // not set by the constructor
    bool field_0x48;
    u32 field_0x4c;  // not set by the constructor
};
static_assert(sizeof(UnkTU2177Node) == 0x50, "");

struct UnkTU2177NodeList {
    // Grows the list to hold index i, constructing the new nodes.
    UnkTU2177Node& At(u32 i) {
        u32 size = i + 1;
        if (size > count) {
            for (u32 j = count; j < size; ++j)
                new (&items[j]) UnkTU2177Node();
            count = size;
        }
        return items[i];
    }

    u32 count;
    UnkTU2177Node items[16];
};
#pragma pack(pop)

bool fn_7100c6e18c(void* self, UnkStruct_0x48* slot, u32 start, UnkTU2177NodeList* nodes) {
    u32 count = nodes->count;
    base::math::CVector2D point;
    fn_710007b430(&point, &nodes->At(start).pos);
    UnkTU2177Entity* entity = nodes->At(start).entity;
    UnkTU2177Owner* owner = entity != nullptr ? entity->owner : nullptr;
    UnkMatrix44 transform = fn_7100d36a2c(reinterpret_cast<UnkShape*>(owner));
    base::math::CVector3D translation;
    translation.x = transform.m[3];
    translation.y = transform.m[7];
    translation.z = transform.m[11];
    base::math::CVector2D offset;
    fn_710007b430(&offset, &translation);
    point.x = offset.x + point.x;
    point.y = offset.y + point.y;

    for (u32 i = start + 1; i < count; ++i) {
        if (nodes->At(i).field_0x48)
            continue;
        UnkTU2177World* world = nodes->At(i).entity->owner->world;
        if ((world != nullptr) & (world->vf_0x18() == 0x10)) {
            if (!(nodes->At(i).flags & 1))
                continue;
            UnkStruct_0x48* nodeSlot = fn_710012431c(&world->pool, nodes->At(i).handle);
            base::math::CVector2D other;
            fn_710007b430(&other, &nodes->At(i).pos);
            if (point.x + 1e-5f >= other.x && point.x - 1e-5f <= other.x && nodeSlot == slot &&
                point.y + 1e-5f >= other.y && point.y - 1e-5f <= other.y) {
                nodes->At(start).field_0x48 = true;
                nodes->At(i).field_0x48 = true;
                return true;
            }
        }
    }
    return false;
}
