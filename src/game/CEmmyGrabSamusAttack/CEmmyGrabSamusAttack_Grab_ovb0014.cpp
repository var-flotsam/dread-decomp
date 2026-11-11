// CEmmyGrabSamusAttack's unit (tools/tu_map.py, unit 800), batch b0014:
// - fn_7100510fd8 resets the grab timers (+0x6b4..+0x6c0) and sends the id of the current kind
//   (fn_71005077f8, fn_710050a500); fn_710050c914, fn_710050e384, fn_710050ef1c and fn_710050d510 do the
//   same, then respectively: start the grab effect (fn_710050afd0) and take the AI's value
//   (fn_7100552ee4); start the other effect and take the character class's tunable at +0x120; hand the
//   owner's position and facing to the E.M.M.I. registry and finish as fn_710050d07c; pick the AI
//   value (fn_7100552e3c) and the countdown scale at +0x6c8 as fn_710050ccc8.
// - fn_710050c490 runs the struggle effects (fn_710050f26c) while +0x6b8 is positive, else switches
//   both effects off.
// - fn_710050af84 starts the grab effect for the kinds that have one.
// - fn_710050a8f8 is the animation id of each grab kind.
// - fn_710050bd10 returns the grab answer and the 2D direction blended from the target's direction
//   and the owner's facing.
// - fn_710050a1b0 places both effects at a point of the screen, facing the camera.
// - fn_710050c06c sets the grab up (or, when the player can't be grabbed, gives control back).
// - fn_71005091d4 says whether a navmesh side can be climbed for the grab.
// Names and layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAIManager/CTunableAIManager.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CBreakableTileGroup.h"
#include "game/CEmmyGrabSamusAttack.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkNavMeshCell.h"
#include "game/UnkRenderManager.h"
#include "game/UnkSceneObject.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2702.h"
#include "game/UnkTU280.h"
#include "game/UnkTU811.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

// fn_71005077f8's id type (src/game/CEmmyGrabSamusAttack/fn_71005077f8.cpp): a data pointer with a
// user copy constructor, returned through x8.
struct UnkStrId_71005077f8 {
    UnkStringData* data;
    UnkStrId_71005077f8(const base::global::UnkStrIdBase& other) : data(other.data) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    UnkStrId_71005077f8(const UnkStrId_71005077f8& other);
};
struct UnkStruct_71005077f8;
struct UnkGrabAttack_b59a3;
struct UnkGrabId_b59a3;

struct UnkKindSource_ovb0014 {
    u8 pad_0x0[0x10];
    u32 field_0x10;
};

struct UnkGrabAttack_ovb0014 {
    u8 pad_0x0[0x38];
    UnkKindSource_ovb0014* field_0x38;
    u8 pad_0x40[0x6b4 - 0x40];
    f32 field_0x6b4;
    f32 field_0x6b8;
    f32 field_0x6bc;
    f32 field_0x6c0;
};

// Resets the grab timers and sends the id of the current kind (14 without a kind source) unless it is
// the empty id, then releases it.
static inline void ResetAndSend(UnkGrabAttack_ovb0014* self) {
    self->field_0x6b4 = 0.0f;
    self->field_0x6b8 = -1.0f;
    self->field_0x6bc = -1.0f;
    self->field_0x6c0 = -1.0f;
    u32 kind = self->field_0x38 ? self->field_0x38->field_0x10 : 14;
    UnkStrId_71005077f8 id = fn_71005077f8(reinterpret_cast<UnkStruct_71005077f8*>(self), kind);
    UnkStringData* data = id.data;
    if (data != &g_unk_7101d47108->empty)
        fn_710050a500(reinterpret_cast<UnkGrabAttack_b59a3*>(self), reinterpret_cast<const UnkGrabId_b59a3*>(&id));
    if (UnkStringPool* pool = g_unk_7101d47108) {
        if (data && data != &pool->empty) {
            if (--data->ref_count == 0) {
                fn_71000803a4(pool, data);
                pool = g_unk_7101d47108;
            }
        }
        id.data = &pool->empty;
    }
}

void fn_7100510fd8(UnkGrabAttack_ovb0014* self) {
    ResetAndSend(self);
}

struct UnkGrabFx4_b61;
struct UnkGrabArg_b61;
struct UnkEmmyAI_b61;
extern UnkGrabArg_b61 g_unk_71726de598 __attribute__((visibility("hidden")));

struct UnkGrabAttack2_ovb0014 {
    u8 pad_0x0[0x8];
    UnkActor* owner;
    u8 pad_0x10[0x6b1 - 0x10];
    bool field_0x6b1;
    u8 pad_0x6b2[0x6b8 - 0x6b2];
    f32 field_0x6b8;
    f32 field_0x6bc;
};

void fn_710050c914(UnkGrabAttack2_ovb0014* self) {
    ResetAndSend(reinterpret_cast<UnkGrabAttack_ovb0014*>(self));
    self->field_0x6b1 = false;
    fn_710050afd0(reinterpret_cast<UnkGrabFx4_b61*>(self), &g_unk_71726de598);
    auto* ai = static_cast<base::reflection::UnkReflObject*>(self->owner->field_0x240);
    auto* emmy = ai && fn_710009d50c(ai->vf_0x0(), fn_710053b934()) ? reinterpret_cast<UnkEmmyAI_b61*>(ai) : nullptr;
    f32 v = fn_7100552ee4(emmy);
    self->field_0x6b8 = v;
    self->field_0x6bc = v;
}

struct UnkEmmyRegistry_b55;
struct UnkGrabVec_b58a3 {
    f32 x, y, z;
};
struct UnkGrabAttack_A3_b68;

struct UnkGrabAttack3_ovb0014 {
    u8 pad_0x0[0x8];
    UnkActor* owner;
    u8 pad_0x10[0x6b0 - 0x10];
    bool field_0x6b0;
};

static inline __attribute__((always_inline)) const UnkGrabVec_b58a3* Addr(const UnkGrabVec_b58a3& v) {
    return &v;
}

// The owner's position and the third column of its matrix.
static inline __attribute__((always_inline)) UnkGrabVec_b58a3 Pos(const UnkActor* a) {
    return {a->field_0x44, a->field_0x54, a->field_0x64};
}

static inline __attribute__((always_inline)) UnkGrabVec_b58a3 Dir(const UnkMatrix44& m) {
    return {m.m[2], m.m[6], m.m[10]};
}

void fn_710050ef1c(UnkGrabAttack3_ovb0014* self) {
    ResetAndSend(reinterpret_cast<UnkGrabAttack_ovb0014*>(self));
    UnkActor* owner = self->owner;
    fn_7100daeb50(g_unk_71726af0a8, 1, Addr(Pos(owner)),
                  Addr(Dir(fn_710002614c(reinterpret_cast<const UnkMatrix44*>(owner->field_0x38)))));
    fn_710050d150(reinterpret_cast<UnkGrabAttack_A3_b68*>(self));
    self->field_0x6b0 = true;
}

struct UnkCharClassTunables_ovb0014 {
    u8 pad_0x0[0x120];
    f32 field_0x120;
};

struct UnkReflObjectD8_ovb0014 {
    void** vtable;
    const base::reflection::CType* vf_0x0() {
        return reinterpret_cast<const base::reflection::CType* (*)(UnkReflObjectD8_ovb0014*)>(vtable[0])(this);
    }
    UnkCharClassTunables_ovb0014* vf_0xd8() {
        return reinterpret_cast<UnkCharClassTunables_ovb0014* (*)(UnkReflObjectD8_ovb0014*)>(vtable[0xd8 / 8])(this);
    }
};

struct UnkEmmyAI_ovb0014 : UnkReflObjectD8_ovb0014 {
    u8 pad_0x8[8];
    UnkReflObjectD8_ovb0014* char_class;  // 0x10
};

void fn_710050e384(UnkGrabAttack2_ovb0014* self) {
    ResetAndSend(reinterpret_cast<UnkGrabAttack_ovb0014*>(self));
    self->field_0x6b1 = false;
    fn_710050afd0(reinterpret_cast<UnkGrabFx4_b61*>(self), &g_unk_71726de5a0);
    auto* ai = static_cast<UnkEmmyAI_ovb0014*>(self->owner->field_0x240);
    if (!ai || !fn_710009d50c(ai->vf_0x0(), fn_710053b934()))
        return;
    UnkReflObjectD8_ovb0014* cc = ai->char_class;
    if (!cc || !fn_710009d50c(cc->vf_0x0(), fn_710051e010()))
        return;
    f32 v = cc->vf_0xd8()->field_0x120;
    self->field_0x6b8 = v;
    self->field_0x6bc = v;
}

struct UnkEmmyAI;

struct UnkGrabAttack7_ovb0014 {
    u8 pad_0x0[0x8];
    UnkActor* owner;
    u8 pad_0x10[0x6b8 - 0x10];
    f32 field_0x6b8;
    f32 field_0x6bc;
    u8 pad_0x6c0[8];
    f32 field_0x6c8;
};

void fn_710050d510(UnkGrabAttack7_ovb0014* self) {
    ResetAndSend(reinterpret_cast<UnkGrabAttack_ovb0014*>(self));
    auto* object = static_cast<UnkEmmyAI_ovb0014*>(self->owner->field_0x240);
    UnkEmmyAI_ovb0014* ai = nullptr;
    if (object && fn_710009d50c(object->vf_0x0(), fn_710053b934()))
        ai = object;
    f32 picked = fn_7100552e3c(reinterpret_cast<UnkEmmyAI*>(ai));
    self->field_0x6b8 = picked;
    self->field_0x6bc = picked;
    if (ai) {
        if (UnkReflObjectD8_ovb0014* inner = ai->char_class) {
            if (fn_710009d50c(inner->vf_0x0(), fn_710051e010())) {
                f32 scale = inner->vf_0xd8()->field_0x120;
                self->field_0x6c8 = 2.0f / scale * self->field_0x6b8 + 2.0f;
            }
        }
    }
}

struct UnkGrabSm_b61;
struct UnkStruct_710050f26c;
struct UnkColorIn_710050f26c;
struct UnkColor_710050f26c;

struct UnkColor_ovb0014 {
    f32 r, g, b, a;
    UnkColor_ovb0014(f32 r_, f32 g_, f32 b_, f32 a_) : r(r_), g(g_), b(b_), a(a_) {}
};

struct UnkGrabAttack_ovb0014c {
    u8 pad_0x0[0x6b8];
    f32 field_0x6b8;
    f32 field_0x6bc;
    u8 pad_0x6c0[8];
    f32 field_0x6c8;
    f32 field_0x6cc;
    f32 field_0x6d0;
    f32 field_0x6d4;
    u8 pad_0x6d8[0x18];
    UnkSceneObject* field_0x6f0;
    UnkSceneObject* field_0x6f8;
};

// Switches an effect's two modes off and disables it, reading the slot afresh for each call.
static inline void SwitchOff(UnkGrabAttack_ovb0014c* self, UnkSceneObject* UnkGrabAttack_ovb0014c::*slot) {
    if (self->*slot) {
        fn_71001be430((self->*slot)->field_0x130, 0, false);
        fn_71001be430((self->*slot)->field_0x130, 1, false);
        fn_7100149490(self->*slot, false);
    }
}

void fn_710050c490(UnkGrabSm_b61* sm) {
    auto* self = reinterpret_cast<UnkGrabAttack_ovb0014c*>(sm);
    if (self->field_0x6b8 > 0.0f) {
        f32 t = (self->field_0x6c8 + -2.0f) * (self->field_0x6bc / self->field_0x6b8) + 2.0f;
        self->field_0x6d0 = 2.0f;
        self->field_0x6d4 = 0.9f;
        self->field_0x6cc = t;
        UnkColor_ovb0014 color(0.3f, 0.3f, 0.0f, 0.9f);
        UnkColor_ovb0014 color2(0.5f, 0.5f, 0.0f, 0.9f);
        fn_710050f26c(reinterpret_cast<UnkStruct_710050f26c*>(self), 2.0f,
                      reinterpret_cast<const UnkColorIn_710050f26c*>(&color), t,
                      reinterpret_cast<const UnkColor_710050f26c*>(&color2), 0.2f);
    } else {
        SwitchOff(self, &UnkGrabAttack_ovb0014c::field_0x6f8);
        SwitchOff(self, &UnkGrabAttack_ovb0014c::field_0x6f0);
    }
}

struct UnkGrabKindAttack_ovb0014 {
    u8 pad_0x0[0x38];
    UnkKindSource_ovb0014* field_0x38;
};

void fn_710050af84(UnkGrabKindAttack_ovb0014* self) {
    if (!self->field_0x38)
        return;
    switch (self->field_0x38->field_0x10) {
    case 1:
        fn_710050afd0(reinterpret_cast<UnkGrabFx4_b61*>(self), &g_unk_71726de598);
        break;
    case 3:
        fn_710050afd0(reinterpret_cast<UnkGrabFx4_b61*>(self), &g_unk_71726de598);
        break;
    case 4:
        fn_710050afd0(reinterpret_cast<UnkGrabFx4_b61*>(self), &g_unk_71726de598);
        break;
    case 5:
        fn_710050afd0(reinterpret_cast<UnkGrabFx4_b61*>(self), &g_unk_71726de598);
        break;
    case 6:
        fn_710050afd0(reinterpret_cast<UnkGrabFx4_b61*>(self), &g_unk_71726de598);
        break;
    case 7:
        fn_710050afd0(reinterpret_cast<UnkGrabFx4_b61*>(self), &g_unk_71726de598);
        break;
    case 8:
        fn_710050afd0(reinterpret_cast<UnkGrabFx4_b61*>(self), &g_unk_71726de598);
        break;
    case 9:
        fn_710050afd0(reinterpret_cast<UnkGrabFx4_b61*>(self), &g_unk_71726de5a0);
        break;
    case 10:
        fn_710050afd0(reinterpret_cast<UnkGrabFx4_b61*>(self), &g_unk_71726de5a0);
        break;
    case 11:
        fn_710050afd0(reinterpret_cast<UnkGrabFx4_b61*>(self), &g_unk_71726de5a0);
        break;
    case 12:
        fn_710050afd0(reinterpret_cast<UnkGrabFx4_b61*>(self), &g_unk_71726de5a0);
        break;
    case 13:
        fn_710050afd0(reinterpret_cast<UnkGrabFx4_b61*>(self), &g_unk_71726de5a0);
        break;
    }
}

using base::global::CStrId;
using base::global::UnkStrIdBase;

// An animation id of the table below: copied like CStrId, released in two steps.
struct UnkGrabAnimId_ovb0014 : UnkStrIdBase {
    UnkGrabAnimId_ovb0014(const char* str) : UnkStrIdBase(str, true) {}
    UnkGrabAnimId_ovb0014(const UnkStrIdBase& other) : UnkStrIdBase(other) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    ~UnkGrabAnimId_ovb0014() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

// The animation of each grab kind (the first two have none).
UnkStrId_71005077f8 fn_710050a8f8(u32 kind) {
    UnkGrabAnimId_ovb0014 ids[] = {
        g_unk_7101d470e0,
        g_unk_7101d470e0,
        "impactemmy",
        "grabemmysamusnobarrierfar2",
        "grabemmysamusprep1",
        "grabemmysamusqte1",
        "grabemmysamusqte1earlyfail",
        "grabemmysamusqte1win",
        "grabemmysamusqte1notime",
        "grabemmysamusprep2",
        "grabemmysamusqte2",
        "grabemmysamusqte2earlyfail",
        "grabemmysamusqte2fail",
        "grabemmysamusqte2win",
    };
    return ids[kind];
}

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkGrappleAnswer_b50;

struct UnkKraidCreatureVec {
    f32 x;
    f32 y;
    f32 z;
    UnkKraidCreatureVec(const UnkKraidCreatureVec& other) : x(other.x), y(other.y), z(other.z) {}
};

struct UnkGrabOwner_ovb0014 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
    u8 pad_0x78[0x228 - 0x78];
    void* field_0x228;
};

struct UnkGrabAttack6_ovb0014 {
    u8 pad_0x0[0x8];
    UnkGrabOwner_ovb0014* owner;
};

// The answer of the owner's component at +0x228 and a 2D direction.
struct UnkGrabAnswer_ovb0014 {
    f32 x, y, z;
    UnkGrabAnswer_ovb0014() : x(0.0f), y(0.0f), z(0.0f) {}
    UnkGrabAnswer_ovb0014(const UnkGrabAnswer_ovb0014& o) : x(o.x), y(o.y), z(o.z) {}
};

struct UnkGrabAim_ovb0014 {
    UnkGrabAnswer_ovb0014 answer;
    CVector2D dir;
};

static inline __attribute__((always_inline)) const CVector3D* Addr(const CVector3D& v) {
    return &v;
}

// The third column of a transform's matrix.
static inline __attribute__((always_inline)) CVector3D Forward(const UnkMatrix44& m) {
    return {m.m[2], m.m[6], m.m[10]};
}

UnkGrabAim_ovb0014 fn_710050bd10(UnkGrabAttack6_ovb0014* self) {
    void* answerer = self->owner->field_0x228;
    u64 id = 0x6d776e7ab7ab37ebUL;
    UnkGrabAnswer_ovb0014 answer;
    fn_710117364c(answerer, &id, reinterpret_cast<UnkGrappleAnswer_b50*>(&answer));
    CVector2D flat;
    {
        UnkKraidCreatureVec target = fn_7100d7be08(self->owner);
        fn_710007b430(&flat, reinterpret_cast<const CVector3D*>(&target));
    }
    f32 length = sqrtf(flat.x * flat.x + flat.y * flat.y);
    f32 t = length / 200.0f;
    t = t <= 0.0f ? 0.0f : (t > 1.0f ? 1.0f : t);
    CVector2D dir;
    if (length > 1.1920929e-07f) {
        f32 inv = 1.0f / length;
        dir = {inv * flat.x, inv * flat.y};
    } else {
        dir = {0.0f, 0.0f};
    }
    CVector2D a = {t * dir.x, t * dir.y};
    f32 ax = a.x;
    f32 ay = a.y;
    f32 rest = 1.0f - t;
    CVector2D forward2;
    fn_710007b430(&forward2, Addr(Forward(fn_710002614c(&self->owner->transform))));
    CVector2D b = {rest * forward2.x, rest * forward2.y};
    return {answer, {ax + b.x, ay + b.y}};
}

struct UnkStruct_7100219588_View;
UnkVec3f fn_7100076acc(const UnkXform* x);

// The screen as fn_7100219bcc describes it: its size in pixels at +0x8.
struct UnkScreenInfo_ovb0014 {
    u64 field_0x0;
    u32 width;
    u32 height;
    u32 field_0x10;
    f32 field_0x14;
    UnkScreenInfo_ovb0014() : field_0x0(0), width(0), height(0), field_0x10(0), field_0x14(1.0f) {}
};

struct UnkCameraTarget_ovb0014 {
    u8 pad_0x0[0x20];
    UnkMatrix44 transform;  // 0x20
};

struct UnkCamera_ovb0014 {
    u8 pad_0x0[0x84];
    f32 field_0x84;
    u8 pad_0x88[0x170 - 0x88];
    UnkCameraTarget_ovb0014* field_0x170;
};

struct UnkView_ovb0014 {
    u8 pad_0x0[0x108];
    UnkCamera_ovb0014* camera;
};

void fn_7100219bcc(UnkRenderManager* self, UnkScreenInfo_ovb0014* info, u32 a, u32 b);
void fn_71001bad24(UnkCamera_ovb0014* camera, const UnkVec3f* screen, const UnkScreenInfo_ovb0014* info,
                   UnkVec3f* out);
UnkMatrix44 fn_7100078314(const UnkMatrix44* m);
void fn_7100149474(UnkSceneObject* object, const UnkVec3f* angle);
void fn_7100149460(UnkSceneObject* object, const UnkVec3f* position);
void fn_7100149488(UnkSceneObject* object, const UnkVec3f* scale);

struct UnkGrabAttack8_ovb0014 {
    u8 pad_0x0[0x6f0];
    UnkSceneObject* field_0x6f0;
    UnkSceneObject* field_0x6f8;
};

static inline __attribute__((always_inline)) const UnkVec3f* Addr(const UnkVec3f& v) {
    return &v;
}

void fn_710050a1b0(UnkStruct_710050f26c* attack) {
    auto* self = reinterpret_cast<UnkGrabAttack8_ovb0014*>(attack);
    UnkScreenInfo_ovb0014 info;
    fn_7100219bcc(g_unk_7101d47300, &info, 0, 0);
    auto* view = reinterpret_cast<UnkView_ovb0014*>(fn_71001fce38(g_unk_7101d47300->field_0x220, 0, 0));
    UnkCamera_ovb0014* camera = view->camera;
    UnkVec3f screen(info.width * 0.5f, info.height * 0.8f, 0.0f);
    UnkVec3f position(0.0f, 0.0f, 0.0f);
    fn_71001bad24(camera, &screen, &info, &position);
    // Both effects at that point, facing the camera, at the camera's scale.
    if (UnkSceneObject* effect = self->field_0x6f0) {
        UnkVec3f angle = fn_7100076acc(reinterpret_cast<const UnkXform*>(
            &static_cast<const UnkMatrix44&>(fn_7100078314(&camera->field_0x170->transform))));
        fn_7100149474(effect, &angle);
        fn_7100149460(self->field_0x6f0, &position);
        fn_7100149488(self->field_0x6f0, Addr(UnkVec3f(camera->field_0x84, camera->field_0x84, camera->field_0x84)));
    }
    if (UnkSceneObject* effect = self->field_0x6f8) {
        UnkVec3f angle = fn_7100076acc(reinterpret_cast<const UnkXform*>(
            &static_cast<const UnkMatrix44&>(fn_7100078314(&camera->field_0x170->transform))));
        fn_7100149474(effect, &angle);
        fn_7100149460(self->field_0x6f8, &position);
        fn_7100149488(self->field_0x6f8, Addr(UnkVec3f(camera->field_0x84, camera->field_0x84, camera->field_0x84)));
    }
}

struct UnkSelf;
struct UnkEmmyGrabAttack_A3_b67;
struct UnkPlayer;
UnkPlayer* fn_7100dec788(UnkGameManager*);
struct UnkOwner;
UnkOwner* fn_7100defcd4(UnkGameManager*);
bool fn_7100df34c8(UnkGameManager* self, void* a, void* b, void* c, void* d, bool e, bool f);
void fn_7100df31b8(UnkGameManager* self, u32 a, bool b, bool c, CStrId id, u32 d);
struct UnkGrabAttack_b58a3;

struct UnkGrabPart_ovb0014 {
    void** vtable;
    u8 pad_0x8[0x54];
    bool field_0x5c;

    void vf_0x180(u32 a, u32 b) {
        reinterpret_cast<void (*)(UnkGrabPart_ovb0014*, u32, u32)>(vtable[0x180 / 8])(this, a, b);
    }
};

struct UnkGrabPlayer_ovb0014 {
    u8 pad_0x0[0x1c0];
    UnkGrabPart_ovb0014* field_0x1c0;
};

struct UnkGrabAttack10_ovb0014 {
    u8 pad_0x0[0x8];
    UnkActor* owner;  // 0x8
    u8 pad_0x10[0x6b8 - 0x10];
    f32 field_0x6b8;
    f32 field_0x6bc;
    u8 pad_0x6c0[8];
    f32 field_0x6c8;
};

void fn_710050c06c(UnkGrabAttack10_ovb0014* self) {
    void* player = fn_7100dec788(g_unk_7101d47310);
    void* target = fn_7100defcd4(g_unk_7101d47310);
    if (target != player &&
        !fn_7100df34c8(g_unk_7101d47310, nullptr, nullptr, nullptr, nullptr, true, false)) {
        UnkGrabPart_ovb0014* part = reinterpret_cast<UnkGrabPlayer_ovb0014*>(fn_7100dec788(g_unk_7101d47310))->field_0x1c0;
        if (!part->field_0x5c)
            part->vf_0x180(0, 0);
        fn_7100507d24(reinterpret_cast<UnkGrabAttack_b58a3*>(self));
        return;
    }
    fn_7100df31b8(g_unk_7101d47310, 1, false, true, g_unk_7101d470e0, 1);
    fn_71005068c8(reinterpret_cast<UnkEmmyGrabAttack_A3_b67*>(self), 50.0f, 350.0f, 242.0f, false, true);
    auto* ai = static_cast<base::reflection::UnkReflObject*>(self->owner->field_0x240);
    auto* emmy = ai && fn_710009d50c(ai->vf_0x0(), fn_710053b934()) ? reinterpret_cast<UnkEmmyAI_b61*>(ai) : nullptr;
    f32 v = fn_7100552ee4(emmy);
    if (v > 0.0f) {
        f32 r = fn_7100552d94(reinterpret_cast<UnkSelf*>(emmy));
        self->field_0x6b8 = r;
        self->field_0x6bc = r;
        self->field_0x6c8 = (2.0f / v) * r + 2.0f;
    } else {
        self->field_0x6b8 = 0.0f;
        self->field_0x6bc = 0.0f;
    }
}

struct UnkComponent_7100286798;
struct UnkQueryObj;

bool fn_71002882d0(void* map, u16 cell, u8 side);
bool fn_71002b007c(UnkNavMeshCell* cell);
UnkQueryObj* fn_71002b37d8(void* tile);
bool fn_7100d65fb0(void* area, const base::math::CVector2D* point);

// The parts of a cell read here: its id and the tile it lies on (+0x68).
struct UnkNavCellView_ovb0014 {
    u16 id;
    u8 pad_0x2[0x66];
    struct {
        u8 pad_0x0[0x1e9];
        bool field_0x1e9;
    }* tile;  // 0x68
};

struct UnkAreaObject_ovb0014 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual s32 Kind();  // 0x18
    u8 pad_0x8[0xa0];
    u8 area[1];  // 0xa8
};

struct UnkScenarioLink_ovb0014 {
    u8 pad_0x0[0x158];
    UnkAreaObject_ovb0014* field_0x158;
};

struct UnkScenario_ovb0014 {
    u8 pad_0x0[0x10];
    UnkScenarioLink_ovb0014* field_0x10;
};

struct UnkScenarioHolder_ovb0014 {
    u8 pad_0x0[0xc38];
    UnkScenario_ovb0014* field_0xc38;
};

struct UnkNavQuery_ovb0014 {
    UnkComponent_7100286798* component;
    UnkNavMesh* mesh;
};

// The side's normal crossed with straight up.
static inline f32 Upness(const UnkNavMeshEdge* side) {
    return side->field_0xc * 1.0f - side->field_0x10 * 0.0f;
}

// Tile kinds 5, 6 and 9.
static inline bool IsClimbableKind(u32 kind) {
    switch (kind) {
    case 5:
    case 6:
    case 9:
        return true;
    default:
        return false;
    }
}

static inline bool IsClimbableTile(UnkNavMeshCell* cell) {
    UnkQueryObj* q = fn_71002b37d8(reinterpret_cast<UnkNavCellView_ovb0014*>(cell)->tile);
    if (!q)
        return true;
    u32 kind = fn_7100c9e960(q);
    return IsClimbableKind(kind);
}

bool fn_71005091d4(UnkNavQuery_ovb0014* query, UnkNavMeshCell* cell, const UnkNavMeshEdge* side) {
    auto* view = reinterpret_cast<UnkNavCellView_ovb0014*>(cell);
    bool blocked = fn_71002882d0(fn_71010fb398(query->component), view->id, side->index);
    bool flagged = view->tile ? view->tile->field_0x1e9 : false;
    bool rejected = fn_71002b005c(cell);
    if (blocked || flagged || rejected)
        return false;
    if (!(Upness(side) >= 0.99f))
        return false;
    if (auto* holder = static_cast<UnkScenarioHolder_ovb0014*>(fn_7100defcec(g_unk_7101d47310))) {
        if (UnkScenario_ovb0014* scenario = holder->field_0xc38) {
            if (UnkScenarioLink_ovb0014* link = scenario->field_0x10) {
                UnkAreaObject_ovb0014* object = link->field_0x158;
                UnkAreaObject_ovb0014* area = object->Kind() == 16 ? object : nullptr;
                if (area) {
                    UnkNavMeshSideSegment segment;
                    segment.field_0x10 = -1.0f;
                    segment.start.x = 0.0f;
                    segment.start.y = 0.0f;
                    segment.end.x = 0.0f;
                    segment.end.y = 0.0f;
                    fn_71002afea8(cell, &side->index, &segment);
                    if (!fn_7100d65fb0(area->area, &segment.start) && !fn_7100d65fb0(area->area, &segment.end))
                        return false;
                }
            }
        }
    }
    if (fn_71002b007c(cell) && !IsClimbableTile(cell))
        return false;
    u16 next = side->field_0x2;
    if (next == 0xffff || !(Upness(side) > 0.98f))
        return true;
    UnkNavMeshCell* other = fn_710027a328(query->mesh, next);
    return !fn_71002b007c(other) || IsClimbableTile(other);
}
