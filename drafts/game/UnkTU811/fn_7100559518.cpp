// UnkTU811 (CEmmyAIComponent's unit): clears the detection state, then marks the player detected when it
// stands within the range of the current mode, and finally refreshes the chase flag.
// Names and layouts are placeholders.
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU811.h"
#include "types.h"

extern "C" float sqrtf(float);

void fn_710054667c(char* self, u32 on);

struct UnkEmmyTuning_l559518 {
    u8 pad_0x0[0xb0];
    bool chase;  // 0xb0
    u8 pad_0xb1[0xbc - 0xb1];
    u32 detectTime;  // 0xbc
};

struct UnkEmmyContext_l559518 {
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
    virtual UnkEmmyTuning_l559518* vf_0xd8();  // the tuning block
};

struct UnkEmmyDetect_l559518 {
    f32 rangeView;  // 0x0
    u8 pad_0x4[0x70 - 0x4];
    f32 rangeB;  // 0x70
    u8 pad_0x74[0xe0 - 0x74];
    f32 rangeC;  // 0xe0
    u8 pad_0xe4[0x150 - 0xe4];
    f32 rangeA;  // 0x150
    u8 pad_0x154[0x29c - 0x154];
    u32 timeA;  // 0x29c
    u32 timeB;  // 0x2a0
    u8 pad_0x2a4[0x2aa - 0x2a4];
    bool detected;  // 0x2aa
};

struct UnkEmmyAI_l559518 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x149b0 - 0x38];
    UnkEmmyDetect_l559518 detect;  // 0x149b0
    u8 pad_0x14c5c[0x29ee0 - 0x14c5c];
    UnkEmmyContext_l559518* context;  // 0x29ee0
};
static_assert(__builtin_offsetof(UnkEmmyAI_l559518, detect) == 0x149b0, "UnkEmmyAI_l559518 layout");
static_assert(__builtin_offsetof(UnkEmmyAI_l559518, context) == 0x29ee0, "UnkEmmyAI_l559518 layout");

void fn_7100559518(UnkEmmyAI_l559518* self) {
    UnkEmmyDetect_l559518& detect = self->detect;
    detect.timeA = 0;
    detect.timeB = 0;
    detect.detected = false;

    UnkActor* player = reinterpret_cast<UnkActor*>(fn_7100dec788(g_unk_7101d47310));
    if (player) {
        u32 time = self->context->vf_0xd8()->detectTime;
        self->context->vf_0xd8();

        f32 range;
        if (fn_710054ad2c(reinterpret_cast<UnkEmmyAIView*>(self))) {
            range = detect.rangeView;
        } else if (fn_71005436e8(reinterpret_cast<UnkEmmyAIComponent_b46a1*>(self))) {
            range = detect.rangeA;
        } else if (fn_710054aecc(reinterpret_cast<UnkEmmyAI2_b62a3*>(self))) {
            range = detect.rangeB;
        } else if (fn_7100543b30(reinterpret_cast<UnkEmmyAIView*>(self))) {
            range = detect.rangeC;
        } else {
            range = 0.0f;
        }

        f32 rangeSq = range * range;
        UnkActor* owner = self->owner;
        f32 dx = player->field_0x44 - owner->field_0x44;
        f32 dy = player->field_0x54 - owner->field_0x54;
        f32 dz = player->field_0x64 - owner->field_0x64;
        f32 distSq = dx * dx + dy * dy + dz * dz;
        if (distSq < rangeSq) {
            f32 distance = sqrtf(distSq);
            (void)distance;
            detect.detected = true;
            detect.timeA = time;
            detect.timeB = time;
        }
    }

    if (!fn_71005436e8(reinterpret_cast<UnkEmmyAIComponent_b46a1*>(self))) {
        bool chase = self->context->vf_0xd8()->chase && fn_7100543b30(reinterpret_cast<UnkEmmyAIView*>(self));
        fn_710054667c(reinterpret_cast<char*>(self), chase);
    }
}
