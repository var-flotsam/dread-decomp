// CChozoCommanderHypersparkAttack's unit (tools/tu_map.py unit 650): the attack's damage set-up: the
// request's id at +0xb8 becomes the unit's static id, its value (fn_71008c1538) is what fn_7100789f5c gives
// for that id and the word at +0xc8 is set to 4; in mode 2 (+0x58) the request is also flagged (+0x49) and
// given the owner's (+0x8) transform (+0x38, fn_710002614c) column at +0x8/+0x18/+0x28 as its point (+0x4c,
// fn_710007b444) with weight 1 (+0x50). Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "game/UnkTU1370.h"
#include "types.h"

using base::global::CStrId;

extern CStrId g_unk_71726c79b8 __attribute__((visibility("hidden")));

struct UnkRequest;
f32 fn_7100789f5c(const CStrId* id);

struct UnkVec3 {
    u32 a;
    u32 b;
    u32 c;
};
void fn_710007b444(void* dst, const UnkVec3* value);

struct UnkHypersparkOwner_A2_b71 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
};

struct UnkHypersparkAttack_A2_b71 {
    u8 pad_0x0[0x8];
    UnkHypersparkOwner_A2_b71* owner;  // 0x8
    u8 pad_0x10[0x48];
    s32 mode;  // 0x58
};

struct UnkHypersparkRequest_A2_b71 {
    u8 pad_0x0[0x49];
    bool field_0x49;
    u8 pad_0x4a[0x2];
    u8 field_0x4c[0x4];
    f32 field_0x50;
    u8 pad_0x54[0x64];
    CStrId id;  // 0xb8
    u8 pad_0xc0[0x8];
    u32 field_0xc8;
};

void fn_710042cd08(UnkHypersparkAttack_A2_b71* self, void* unused, UnkHypersparkRequest_A2_b71* request) {
    request->id = g_unk_71726c79b8;
    fn_71008c1538(reinterpret_cast<UnkRequest*>(request), fn_7100789f5c(&g_unk_71726c79b8));
    request->field_0xc8 = 4;
    if (self->mode == 2) {
        request->field_0x49 = true;
        const u32* m = reinterpret_cast<const u32*>(fn_710002614c(&self->owner->transform).m);
        u32 a = m[2];
        u32 b = m[6];
        u32 c = m[10];
        UnkVec3 v = {a, b, c};
        fn_710007b444(request->field_0x4c, &v);
        request->field_0x50 = 1.0f;
    }
}
