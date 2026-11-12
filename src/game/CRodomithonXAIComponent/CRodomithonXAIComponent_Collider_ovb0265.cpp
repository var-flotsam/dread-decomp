// CRodomithonXAIComponent's unit (tools/tu_map.py unit 988): finds (or creates through the container's slot 0x168) the
// collider named by fn_7100666a7c in the owner's collision container (+0x2a0), places its body at the owner's
// projected direction scaled by the entry's range and activates it. Names and layouts are placeholders.
#include "base/UnkAtExit.h"
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "game/UnkBroadphaseObject.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkGameManagerIdTable.h"
#include "types.h"

using base::global::CStrId;

struct UnkRodoOwner {
    u8 pad_0x0[0x2a0];
    UnkCollisionContainer* container;  // 0x2a0
};

struct UnkRodoEntry {
    u32 field_0x0;
    u32 id;  // 0x4
    f32 field_0x8;
    f32 field_0xc;
};

struct UnkRodoList {
    u8 pad_0x0[0x638];
    UnkRodoEntry* data;  // 0x638
    u32 count;           // 0x640
};

struct UnkRodoRef {
    UnkRodoList* list;
    u32 pad_0x8;
    u32 index;  // 0xc
};

struct UnkRodoSelf {
    u8 pad_0x0[0x30];
    UnkRodoOwner* owner;  // 0x30
    u8 pad_0x38[0x148a8 - 0x38];
    UnkRodoRef ref;  // 0x148a8
};

const CStrId* fn_7100666a7c(UnkRodoList* list);

struct UnkRodoBody {
    void** vtable;
    u8 pad_0x8[0xa0];
    f32 position[2];  // 0xa8
    f32 field_0xb0;
    f32 field_0xb4;
    f32 field_0xb8;
    f32 field_0xbc;
    u32 field_0xc0;
    f32 field_0xc4;
    u64 field_0xc8;

    u32 vf_0x18() { return reinterpret_cast<u32 (*)(UnkRodoBody*)>(vtable[0x18 / 8])(this); }
    void vf_0x80() { reinterpret_cast<void (*)(UnkRodoBody*)>(vtable[0x80 / 8])(this); }
};

struct UnkRodoCollider {
    void** vtable;
    u8 pad_0x8[0x150];
    UnkRodoBody* body;  // 0x158

    void vf_0x48() { reinterpret_cast<void (*)(UnkRodoCollider*)>(vtable[0x48 / 8])(this); }
};

using UnkRodoCreate = UnkCollisionObject* (*)(UnkCollisionContainer*, const CStrId*, const CStrId*, const CStrId*,
                                              const CStrId*, u32, u32, u32, u64, s32, u64, s32, u64, s32, bool, bool,
                                              bool);

void fn_71006690c4(UnkRodoSelf* self) {
    UnkCollisionContainer* container = self->owner->container;
    if (!container)
        return;
    UnkRodoRef* ref = &self->ref;
    UnkRodoList* list = ref->list;
    if (!list)
        return;
    u32 index = ref->index;
    if (index >= list->count)
        return;
    UnkRodoEntry* entry = &list->data[index];
    u32 id = entry->id;
    f32 a = entry->field_0x8;
    f32 b = entry->field_0xc;
    const CStrId* name = fn_7100666a7c(list);
    UnkCollisionObject* object = fn_71008640a4(container, name);
    if (!object) {
        const CStrId* key = reinterpret_cast<const CStrId*>(GameManagerIdTable().pad_0x0 + 0x19d0);
        object = reinterpret_cast<UnkRodoCreate>((*reinterpret_cast<void***>(container))[0x168 / 8])(
            container, name, name, &g_unk_7101d470e0, key, 0x2000, 1, 0, 0, -1, 0, -1, 0, -1, false, false, true);
        if (!object)
            return;
    }
    const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(reinterpret_cast<char*>(self->owner) + 0x38));
    base::math::CVector2D v2;
    // the column is copied as words
    struct Words {
        u32 x, y, z;
    };
    const u32* words = reinterpret_cast<const u32*>(&m.m[0]);
    u32 wx = words[1];
    u32 wy = words[5];
    u32 wz = words[9];
    Words v;
    v.x = wx;
    v.y = wy;
    v.z = wz;
    fn_710007b430(&v2, reinterpret_cast<base::math::CVector3D*>(&v));
    UnkRodoCollider* collider = reinterpret_cast<UnkRodoCollider*>(object);
    f32 scale = a * 0.5f + b;
    f32 px = scale * v2.x;
    f32 py = scale * v2.y;
    UnkRodoBody* sub = collider->body;
    UnkRodoBody* body = sub->vf_0x18() == 0x40 ? sub : nullptr;
    body->position[0] = px;
    body->position[1] = py;
    body->field_0xb0 = 1.0f;
    body->field_0xb4 = 0.0f;
    body->field_0xb8 = -0.0f;
    body->field_0xbc = 1.0f;
    body->field_0xc0 = id;
    body->field_0xc4 = a;
    body->field_0xc8 = 0;
    body->vf_0x80();
    collider->vf_0x48();
    fn_7100d35a98(reinterpret_cast<UnkEntry*>(object), true);
}
