// CChozoWarriorXSpitAttack (unit 717): aims the spit. On a new aim (+0x48) the start and direction are taken
// from the owner towards its target (or from the mouth's matrix when the model has it) and cast through the
// collision tree, keeping the hit point, its distance and normal. Otherwise the spit's collider is created
// (when the owner has none yet), turned along the aim and placed at the start in the owner's space.
// Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "game/CAbilityShinespark.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkActor.h"
#include "game/UnkBroadphaseObject.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU365.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

#include <math.h>

using base::global::CStrId;
using base::math::CVector2D;

struct UnkEntity;
struct UnkMat4f;
extern const CStrId g_unk_71726cf1e0[] __attribute__((visibility("hidden")));
// The name of the spit's origin in the model.
extern char g_unk_7101649880[];

struct UnkSpitConfig_l484b10 {
    u8 pad_0x0[0x3c8];
    u32 mask;  // 0x3c8
};

struct UnkSpitOwner_l484b10 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x218 - 0x94];
    UnkSpitConfig_l484b10* config;  // 0x218
    u8 pad_0x220[0x8];
    UnkEntity* model;  // 0x228
    u8 pad_0x230[0x2a0 - 0x230];
    UnkCollisionContainer* collision;  // 0x2a0
};

struct UnkSpitAttack_l484b10 {
    u8 pad_0x0[0x8];
    UnkSpitOwner_l484b10* owner;  // 0x8
    u8 pad_0x10[0x38];
    bool aim;  // 0x48
    u8 pad_0x49[0xf];
    f32 field_0x58;
    f32 field_0x5c;
    f32 distance;  // 0x60
    f32 range;     // 0x64
    UnkVec3f dir;     // 0x68
    UnkVec3f start;   // 0x74
    UnkVec3f hit;     // 0x80
    UnkVec3f normal;  // 0x8c
};
static_assert(__builtin_offsetof(UnkSpitAttack_l484b10, normal) == 0x8c, "layout");

// A collision hit (0x48 bytes): the object hit and a second reference, an id, the point, a vector, the
// normal and flags.
struct UnkSpitHit_l484b10 {
    base::UnkRefPtrMember object;
    base::UnkRefPtrMember field_0x8;
    s32 id;
    UnkVec3f point;
    UnkVec3f field_0x20;
    UnkVec3f normal;
    u32 field_0x38;
    u32 flags;
    u32 field_0x40;

    UnkSpitHit_l484b10()
        : object{nullptr}, field_0x8{nullptr}, id(-1), point(0.0f, 0.0f, 0.0f), field_0x20(0.0f, 0.0f, 0.0f),
          normal(0.0f, 0.0f, 0.0f), field_0x38(0), flags(0), field_0x40(0) {}
};
static_assert(sizeof(UnkSpitHit_l484b10) == 0x48, "UnkSpitHit_l484b10 layout");

// The cast's callback object: the closest hit.
struct UnkSpitQuery_l484b10 {
    void** vtable;
    UnkSpitHit_l484b10 hit;
    bool field_0x50;

    UnkSpitQuery_l484b10() {
        vtable = g_unk_7101a23590 + 2;
        field_0x50 = false;
    }
    ~UnkSpitQuery_l484b10() { vtable = g_unk_7101a20ed8 + 2; }
};

// The segment fn_7100d5c518 casts: its two ends and the vector between them.
struct UnkSpitSegment_l484b10 {
    CVector2D start;
    CVector2D end;
    CVector2D delta;

    UnkSpitSegment_l484b10(const CVector2D& s, const CVector2D& e) {
        start.x = s.x;
        start.y = s.y;
        end.x = e.x;
        end.y = e.y;
        delta.x = e.x - s.x;
        delta.y = e.y - s.y;
    }
};

// The oriented box of the spit's collider.
struct UnkSpitBody_l484b10 {
    void** vtable;
    u8 pad_0x8[0xa0];
    f32 position[2];  // 0xa8
    f32 axis_x[2];    // 0xb0
    f32 axis_y[2];    // 0xb8
    u32 field_0xc0;
    f32 size;   // 0xc4
    f32 angle;  // 0xc8
    u32 field_0xcc;

    s32 Kind() { return reinterpret_cast<s32 (*)(UnkSpitBody_l484b10*)>(vtable[0x18 / 8])(this); }
    void SetPosition(const UnkVec3f* pos) {
        reinterpret_cast<void (*)(UnkSpitBody_l484b10*, const UnkVec3f*)>(vtable[0x70 / 8])(this, pos);
    }
    void Update() { reinterpret_cast<void (*)(UnkSpitBody_l484b10*)>(vtable[0x80 / 8])(this); }
};

struct UnkSpitCollider_l484b10 {
    void** vtable;
    u8 pad_0x8[0x150];
    UnkSpitBody_l484b10* body;  // 0x158

    void vf_0x48() { reinterpret_cast<void (*)(UnkSpitCollider_l484b10*)>(vtable[0x48 / 8])(this); }
};

using UnkSpitCreate_l484b10 = UnkCollisionObject* (*)(UnkCollisionContainer*, const CStrId*, const CStrId*,
                                                       const CStrId*, const CStrId*, u32, u32, u32, u64, s32, u64,
                                                       s32, u64, s32, bool, bool, bool);

static inline UnkSpitBody_l484b10* AsBox(UnkSpitBody_l484b10* body) {
    return body->Kind() == 0x40 ? body : nullptr;
}

static inline UnkMatrix44 Identity() {
    UnkMatrix44 m;
    for (u32 i = 0; i < 16; ++i)
        m.m[i] = 0.0f;
    m.m[15] = 1.0f;
    m.m[10] = 1.0f;
    m.m[5] = 1.0f;
    m.m[0] = 1.0f;
    return m;
}

void fn_7100484b10(UnkSpitAttack_l484b10* self) {
    UnkTree* tree;
    if (self->aim) {
        self->aim = false;
        UnkActor* target = fn_7100d7c990(reinterpret_cast<UnkActor*>(self->owner));
        UnkGameManagerSub_0x608* sub = g_unk_7101d47310->field_0x608;
        tree = sub ? static_cast<UnkTree*>(sub->field_0x820) : nullptr;
        if (target && tree) {
            UnkXform& xform = self->owner->xform;
            self->start = UnkVec3f(xform.m[0][3], xform.m[1][3], xform.m[2][3]);
            f32 x = target->field_0x44 - self->start.x;
            f32 y = target->field_0x54 - self->start.y;
            f32 z = target->field_0x64 - self->start.z;
            f32 scale = 1.0f / (sqrtf(x * x + y * y + z * z) + 0.0001f);
            self->dir = UnkVec3f(x * scale, y * scale, z * scale);
            UnkMatrix44 mouth = Identity();
            if (fn_71011735e8(self->owner->model, g_unk_7101649880, &mouth, 0)) {
                self->start = UnkVec3f(mouth.m[3], mouth.m[7], mouth.m[11]);
                self->dir = UnkVec3f(mouth.m[0], mouth.m[4], mouth.m[8]);
            }
        }
    } else {
        if (fn_71008640a4(self->owner->collision, &g_unk_71726cf1e0[0]))
            return;
        UnkCollisionContainer* collision = self->owner->collision;
        UnkCollisionObject* object;
        {
            CStrId none(base::global::UnkStrIdInline(), "", false);
            const CStrId* key = reinterpret_cast<const CStrId*>(GameManagerIdTable().pad_0x0 + 0x19d0);
            object = reinterpret_cast<UnkSpitCreate_l484b10>((*reinterpret_cast<void***>(collision))[0x168 / 8])(
                collision, &g_unk_71726cf1e0[0], &g_unk_71726cf1e0[0], &none, key, 0x100, 0, 0, 0, -1, 0, -1, 0, -1,
                false, false, true);
        }
        if (!object)
            return;
        f32 angle = atan2f(self->hit.y - self->start.y, self->hit.x - self->start.x);
        f32 c = cosf(angle);
        f32 s = sinf(angle);
        f32 ns = -sinf(angle);
        f32 c2 = cosf(angle);
        UnkSpitCollider_l484b10* collider = reinterpret_cast<UnkSpitCollider_l484b10*>(object);
        UnkSpitBody_l484b10* box = AsBox(collider->body);
        box->position[0] = 0.0f;
        box->position[1] = 0.0f;
        box->axis_x[0] = c;
        box->axis_x[1] = s;
        box->axis_y[0] = ns;
        box->axis_y[1] = c2;
        box->field_0xc0 = 0;
        box->size = 50.0f;
        box->angle = angle;
        box->field_0xcc = 0;
        box->Update();
        UnkSpitBody_l484b10* body = collider->body;
        if (body)
            body = AsBox(body);
        UnkMatrix44 inverse = fn_7100078314(&self->owner->xform.GetMatrix());
        UnkVec3f local = fn_7100078170(reinterpret_cast<const UnkMat4f*>(&inverse), &self->start);
        body->SetPosition(&local);
        collider->vf_0x48();
        fn_7100d35a98(reinterpret_cast<UnkEntry*>(object), true);
        self->field_0x58 = 0.0f;
        self->range = 50000.0f;
        return;
    }
    UnkSpitQuery_l484b10 query;
    CVector2D from = {self->start.x, self->start.y};
    CVector2D to = {self->dir.x * 5000.0f + from.x, self->dir.y * 5000.0f + from.y};
    UnkSpitSegment_l484b10 segment(from, to);
    if (fn_7100d5c518(tree, self->owner->config->mask, reinterpret_cast<const UnkBox*>(&segment), &query, 2)) {
        self->hit = query.hit.point;
        f32 dx = self->start.x - self->hit.x;
        f32 dy = self->start.y - self->hit.y;
        f32 dz = self->start.z - self->hit.z;
        self->distance = sqrtf(dx * dx + dy * dy + dz * dz);
        self->normal = query.hit.normal;
        self->field_0x5c = 0.0f;
    }
}
