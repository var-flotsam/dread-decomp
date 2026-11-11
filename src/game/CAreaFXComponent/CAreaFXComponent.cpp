// A frozen-state component in the CAreaFXComponent unit: fn_71007ff914 adjusts an incoming damage
// request after the base check (fn_71007fb280). A frozen (fn_71007f9f20) component that isn't
// breaking (fn_71007f9fd0) either unfreezes (fn_71007fa130) or moves to state 2 (fn_71007f9428).
// Otherwise the request takes the actor's whole life when the AI component (virtual slot 152) says so,
// else at most the life minus one; when it didn't kill, the breaking flag goes to +0x3f4 and the state
// to 2. Names and layouts are placeholders.
#include "game/CFrozenComponent.h"
#include "types.h"

struct UnkRequest;

struct UnkFrostbiteRequest {
    u8 pad_0x0[0xf8];
    f32 amount;  // 0xf8
};

struct UnkFrostbiteLife {
    u8 pad_0x0[0x54];
    f32 life;  // 0x54
};

struct UnkFrostbiteAI {
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
    virtual void vf_0x188();
    virtual void vf_0x190();
    virtual void vf_0x198();
    virtual void vf_0x1a0();
    virtual void vf_0x1a8();
    virtual void vf_0x1b0();
    virtual void vf_0x1b8();
    virtual void vf_0x1c0();
    virtual void vf_0x1c8();
    virtual void vf_0x1d0();
    virtual void vf_0x1d8();
    virtual void vf_0x1e0();
    virtual void vf_0x1e8();
    virtual void vf_0x1f0();
    virtual void vf_0x1f8();
    virtual void vf_0x200();
    virtual void vf_0x208();
    virtual void vf_0x210();
    virtual void vf_0x218();
    virtual void vf_0x220();
    virtual void vf_0x228();
    virtual void vf_0x230();
    virtual void vf_0x238();
    virtual void vf_0x240();
    virtual void vf_0x248();
    virtual void vf_0x250();
    virtual void vf_0x258();
    virtual void vf_0x260();
    virtual void vf_0x268();
    virtual void vf_0x270();
    virtual void vf_0x278();
    virtual void vf_0x280();
    virtual void vf_0x288();
    virtual void vf_0x290();
    virtual void vf_0x298();
    virtual void vf_0x2a0();
    virtual void vf_0x2a8();
    virtual void vf_0x2b0();
    virtual void vf_0x2b8();
    virtual void vf_0x2c0();
    virtual void vf_0x2c8();
    virtual void vf_0x2d0();
    virtual void vf_0x2d8();
    virtual void vf_0x2e0();
    virtual void vf_0x2e8();
    virtual void vf_0x2f0();
    virtual void vf_0x2f8();
    virtual void vf_0x300();
    virtual void vf_0x308();
    virtual void vf_0x310();
    virtual void vf_0x318();
    virtual void vf_0x320();
    virtual void vf_0x328();
    virtual void vf_0x330();
    virtual void vf_0x338();
    virtual void vf_0x340();
    virtual void vf_0x348();
    virtual void vf_0x350();
    virtual void vf_0x358();
    virtual void vf_0x360();
    virtual void vf_0x368();
    virtual void vf_0x370();
    virtual void vf_0x378();
    virtual void vf_0x380();
    virtual void vf_0x388();
    virtual void vf_0x390();
    virtual void vf_0x398();
    virtual void vf_0x3a0();
    virtual void vf_0x3a8();
    virtual void vf_0x3b0();
    virtual void vf_0x3b8();
    virtual void vf_0x3c0();
    virtual void vf_0x3c8();
    virtual void vf_0x3d0();
    virtual void vf_0x3d8();
    virtual void vf_0x3e0();
    virtual void vf_0x3e8();
    virtual void vf_0x3f0();
    virtual void vf_0x3f8();
    virtual void vf_0x400();
    virtual void vf_0x408();
    virtual void vf_0x410();
    virtual void vf_0x418();
    virtual void vf_0x420();
    virtual void vf_0x428();
    virtual void vf_0x430();
    virtual void vf_0x438();
    virtual void vf_0x440();
    virtual void vf_0x448();
    virtual void vf_0x450();
    virtual void vf_0x458();
    virtual void vf_0x460();
    virtual void vf_0x468();
    virtual void vf_0x470();
    virtual void vf_0x478();
    virtual void vf_0x480();
    virtual void vf_0x488();
    virtual void vf_0x490();
    virtual void vf_0x498();
    virtual void vf_0x4a0();
    virtual void vf_0x4a8();
    virtual void vf_0x4b0();
    virtual void vf_0x4b8();
    virtual bool vf_0x4c0();
};

struct UnkFrostbiteActor {
    u8 pad_0x0[0x1b8];
    UnkFrostbiteLife* life;  // 0x1b8
    u8 pad_0x1c0[0x80];
    UnkFrostbiteAI* ai;  // 0x240
};

struct UnkFrostbiteComponent {
    u8 pad_0x0[0x30];
    UnkFrostbiteActor* actor;  // 0x30
    u8 pad_0x38[0x3f4 - 0x38];
    bool field_0x3f4;
};

void fn_71008c1538(UnkRequest* request, f32 value);

bool fn_71007ff914(UnkFrostbiteComponent* self, UnkFrostbiteRequest* request) {
    UnkFrostbiteLife* life = self->actor->life;
    UnkFrostbiteAI* ai = self->actor->ai;
    if (!fn_71007fb280(self, request))
        return false;
    if (fn_71007f9f20(self) && !fn_71007f9fd0(self)) {
        if (!fn_71007f9fb0(self)) {
            fn_71007fa130(self);
            return false;
        }
    } else {
        bool kill = ai ? ai->vf_0x4c0() : false;
        if (life && kill) {
            fn_71008c1538(reinterpret_cast<UnkRequest*>(request), life->life);
            return false;
        }
        if (life) {
            f32 most = life->life + -1.0f;
            f32 amount = request->amount;
            fn_71008c1538(reinterpret_cast<UnkRequest*>(request),
                          amount <= 0.0f ? 0.0f : (amount >= most ? most : amount));
        }
        if (kill)
            return false;
        self->field_0x3f4 = fn_71007f9fd0(self);
    }
    fn_71007f9428(self, 2);
    return false;
}
