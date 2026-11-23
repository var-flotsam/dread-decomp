// CChozoCommanderBeamBurstAttack unit (tools/tu_map.py unit 646): hits a target's life component (+0x1b8)
// with a damage request built from the hit (its collider and four points), the attack owner's and its
// target's weak handles and one of two tunable damage values. Names and layouts are placeholders.
#include "base/reflection/ReflectedAttributes.h"
#include "game/UnkActor.h"
#include "game/UnkTU1370.h"
#include "game/UnkTU2523.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkRequest;
struct UnkStruct_71008c0c74;
struct UnkBase_71003cc474;

using base::reflection::UnkTunable;
extern UnkTunable g_unk_71726c56e8;

// The damage values of the attack's tunables.
struct UnkBeamBurstDamage_l429b58 {
    f32 normal;  // 0x50
    f32 alt;     // 0x54
};

// The members of the attack's tunables read here.
struct UnkBeamBurstTunables_l429b58 {
    u8 pad_0x0[0x50];
    UnkBeamBurstDamage_l429b58 damage;  // 0x50
};

struct UnkVec3_l429b58 {
    f32 x;
    f32 y;
    f32 z;

    UnkVec3_l429b58& operator=(const UnkVec3_l429b58& other) {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }
};

struct UnkCollider_l429b58;

struct UnkColliderRef_l429b58 {
    u8 pad_0x0[0x10];
    UnkCollider_l429b58* collider;
};

struct UnkHit_l429b58 {
    UnkColliderRef_l429b58* collider;  // 0x0
    u8 pad_0x8[0x8];
    UnkVec3_l429b58 points[4];  // 0x10
};

struct UnkBeamDamageRequest_l429b58 {
    u8 pad_0x0[0x8];
    UnkWeakHandle* source;  // 0x8
    UnkWeakHandle* owner;   // 0x10
    u8 pad_0x18[0x9];
    bool field_0x21;
    u8 pad_0x22[0x7];
    bool field_0x29;
    u8 pad_0x2a[0x36];
    UnkCollider_l429b58* collider;  // 0x60
    UnkVec3_l429b58 points[4];      // 0x68
    u8 pad_0x98[0x30];
    u32 kind;  // 0xc8
    u8 pad_0xcc[0x34];

    UnkBeamDamageRequest_l429b58() { fn_71008c0c74(reinterpret_cast<UnkStruct_71008c0c74*>(this)); }
    ~UnkBeamDamageRequest_l429b58() { fn_71008c1558(reinterpret_cast<UnkBase_71003cc474*>(this)); }
};
static_assert(sizeof(UnkBeamDamageRequest_l429b58) == 0x100, "request layout");

struct UnkBeamLife_l429b58 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual void vf_0xd8();
    virtual void vf_0xe0();
    virtual void vf_0xe8();
    virtual void vf_0xf0();
    virtual void vf_0xf8();
    virtual void vf_0x100();
    virtual void vf_0x108();
    virtual void vf_0x110();
    virtual void vf_0x118();
    virtual void vf_0x120();
    virtual void vf_0x128();
    virtual void vf_0x130();
    virtual void vf_0x138();
    virtual void vf_0x140();
    virtual void vf_0x148();
    virtual void vf_0x150();
    virtual void vf_0x158();
    virtual void vf_0x160();
    virtual void vf_0x168();
    virtual void vf_0x170();
    virtual void vf_0x178();
    virtual void vf_0x180();
    virtual void Hit(UnkBeamDamageRequest_l429b58* request);  // 0x188
};

struct UnkBeamTarget_l429b58 {
    u8 pad_0x0[0x1b8];
    UnkBeamLife_l429b58* life;  // 0x1b8
};

struct UnkBeamBurstAttack_l429b58 {
    u8 pad_0x0[0x8];
    UnkActor* owner;  // 0x8
};

void fn_7100429b58(UnkBeamBurstAttack_l429b58* self, UnkBeamTarget_l429b58* target, UnkHit_l429b58* hit,
                   bool alt) {
    UnkBeamLife_l429b58* life = target->life;
    if (!life)
        return;
    UnkBeamDamageRequest_l429b58 request;
    request.points[0] = hit->points[0];
    request.points[1] = hit->points[1];
    request.points[2] = hit->points[2];
    request.points[3] = hit->points[3];
    AssignRef(&request.source, fn_7100d78378(reinterpret_cast<UnkWeakTarget*>(self->owner)));
    AssignRef(&request.owner, fn_7100d78378(reinterpret_cast<UnkWeakTarget*>(fn_7100d7c990(self->owner))));
    request.collider = hit->collider ? hit->collider->collider : nullptr;
    request.kind = 4;
    const UnkBeamBurstTunables_l429b58* tunables =
        reinterpret_cast<const UnkBeamBurstTunables_l429b58*>(&g_unk_71726c56e8);
    if (alt)
        fn_71008c1538(reinterpret_cast<UnkRequest*>(&request), tunables->damage.alt);
    else
        fn_71008c1538(reinterpret_cast<UnkRequest*>(&request), tunables->damage.normal);
    request.field_0x21 = !alt;
    request.field_0x29 = !alt;
    life->Hit(&request);
}
