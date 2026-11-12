// CKraidAIComponent: fn_7100600b94 fills a damage request after the base attack's (fn_7101105c90): it
// sets the flag at +0x49 and the direction at +0x4c to (-1, 0, 0), keeps whether the target's helper
// (fn_71009e6434) or, while it was already set, the target's record says so in the flag at +0x14a14 (a
// set flag makes the request's factor at +0xd0 one), and clears the request's amount when the actor's
// character class allows the ghost-dash grab track. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU1370.h"
#include "game/UnkTU1501.h"
#include "game/UnkTU2702.h"
#include "types.h"

struct UnkRequest;
struct UnkHelper;

struct UnkKraidRequestVec3 {
    u32 a;
    u32 b;
    u32 c;
};

struct UnkKraidRequest {
    u8 pad_0x0[0x49];
    bool field_0x49;
    u8 pad_0x4a[0x2];
    u8 field_0x4c[0xc];
    u8 pad_0x58[0x78];
    f32 field_0xd0;
};

struct UnkKraidTargetRecord {
    u8 pad_0x0[0xf9];
    bool field_0xf9;
};

struct UnkKraidAI2 {
    u8 pad_0x0[0x30];
    UnkActor* actor;  // 0x30
    u8 pad_0x38[0x14a14 - 0x38];
    bool field_0x14a14;
};

struct UnkVec3;
void fn_710007b444(void* dst, const UnkVec3* value);

void fn_7100600b94(UnkKraidAI2* self, void* arg, UnkKraidRequest* request) {
    fn_7101105c90(self, arg, request);
    request->field_0x49 = true;
    UnkKraidRequestVec3 dir;
    dir.a = 0xbf800000;
    dir.b = 0;
    dir.c = 0;
    fn_710007b444(request->field_0x4c, reinterpret_cast<const UnkVec3*>(&dir));
    UnkActor* target = fn_7100d7c990(self->actor);
    if (target && target->field_0x208 && fn_71009e6434(static_cast<UnkHelper*>(target->field_0x208))) {
        self->field_0x14a14 = true;
        request->field_0xd0 = 1.0f;
    } else if (target && self->field_0x14a14) {
        UnkKraidTargetRecord* record = *reinterpret_cast<UnkKraidTargetRecord**>(reinterpret_cast<char*>(target) + 0x1f0);
        self->field_0x14a14 = record->field_0xf9;
        if (self->field_0x14a14)
            request->field_0xd0 = 1.0f;
    } else {
        self->field_0x14a14 = false;
    }
    UnkAnimationComponent* anim = static_cast<UnkAnimationComponent*>(self->actor->field_0x220);
    if (anim && fn_710117b348(anim, fn_7100c38228()))
        fn_71008c1538(reinterpret_cast<UnkRequest*>(request), 0.0f);
}
