// CAutsniperShootAttack's unit (tools/tu_map.py unit 576): sends the owner's (+0x8, fn_7100d7c990)
// component at +0x1c0 a damage request (fn_71008c0c74, virtual 0x188) with the unit's static string id at
// +0xb8, the weak handle of the owner (fn_7100d78378) at +0x8, the tunable's value (+0x54 of
// g_unk_7172716160, fn_71008c1538), flags 0x400000 (+0xb4), the attack's point (+0x70) at +0x6c and bit 3
// of +0x94 set. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/UnkActor.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector3D;

extern base::reflection::UnkTunable g_unk_7172716160;
extern CStrId g_unk_71726bf858 __attribute__((visibility("hidden")));

UnkWeakHandle* fn_7100d78378(UnkWeakTarget* self);

struct UnkRequest;
struct UnkStruct_71008c0c74;
struct UnkBase_71003cc474;
void fn_71008c0c74(UnkStruct_71008c0c74* self);
void fn_71008c1558(UnkBase_71003cc474* self);
void fn_71008c1538(UnkRequest* request, f32 value);

struct UnkSniperRequest_A2_b71 {
    u8 pad_0x0[0x8];
    UnkWeakHandle* source;  // 0x8
    u8 pad_0x10[0x11];
    bool field_0x21;
    u8 pad_0x22[0x8];
    bool field_0x2a;
    u8 pad_0x2b[0x41];
    CVector3D point;  // 0x6c
    u8 pad_0x78[0x1c];
    u32 field_0x94;
    u8 pad_0x98[0x1c];
    u32 flags;  // 0xb4
    u8 id_raw[0x8];  // 0xb8: a CStrId
    u8 pad_0xc0[0x40];

    CStrId& Id() { return *reinterpret_cast<CStrId*>(id_raw); }

    UnkSniperRequest_A2_b71() { fn_71008c0c74(reinterpret_cast<UnkStruct_71008c0c74*>(this)); }
    ~UnkSniperRequest_A2_b71() { fn_71008c1558(reinterpret_cast<UnkBase_71003cc474*>(this)); }
};
static_assert(sizeof(UnkSniperRequest_A2_b71) == 0x100, "request layout");

struct UnkSniperTarget_A2_b71 {
    void** vtable;
    void vf_0x188(UnkSniperRequest_A2_b71* request) {
        reinterpret_cast<void (*)(UnkSniperTarget_A2_b71*, UnkSniperRequest_A2_b71*)>(vtable[0x188 / 8])(this,
                                                                                                    request);
    }
};

struct UnkSniperShoot_A2_b71 {
    u8 pad_0x0[0x8];
    UnkActor* owner;  // 0x8
    u8 pad_0x10[0x60];
    CVector3D point;  // 0x70
};

void fn_71003d8100(UnkSniperShoot_A2_b71* self) {
    UnkSniperRequest_A2_b71 request;
    request.Id() = g_unk_71726bf858;
    AssignRef(&request.source, fn_7100d78378(reinterpret_cast<UnkWeakTarget*>(self->owner)));
    fn_71008c1538(reinterpret_cast<UnkRequest*>(&request),
                  *reinterpret_cast<const f32*>(reinterpret_cast<const char*>(&g_unk_7172716160) + 0x54));
    request.flags = 0x400000;
    request.field_0x21 = false;
    request.field_0x2a = false;
    request.point = self->point;
    request.field_0x94 |= 8;
    UnkActor* actor = fn_7100d7c990(self->owner);
    (*reinterpret_cast<UnkSniperTarget_A2_b71**>(reinterpret_cast<char*>(actor) + 0x1c0))->vf_0x188(&request);
}
