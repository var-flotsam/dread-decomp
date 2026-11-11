// CEmmyGrabSamusAttack unit (tools/tu_map.py): an update. After fn_7100507d24, either fn_710050802c (flag at
// +0x6b0) or, for the player's part at +0x1c0 whose flag at +0x5c is clear, its slot 0x180 with (0, 0).
// When the owner's AI component (+0x240) is a CEmmyAIComponent (fn_710053b934) and the owner has its
// animation (+0x220), sends a request with mode 0x81 under kind 0x7c5476647f60cdaa (descriptors as
// UnkSpitSend_974c, src/game/UnkNamedSend_b54.h) whose hash depends on the AI's state at +0x29ef0 (0 or
// 1). Then hands the owner's position and the third column of its matrix to fn_7100daeb50 of the
// E.M.M.I. registry. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAIManager/CTunableAIManager.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CEmmyGrabSamusAttack.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "../UnkNamedSend_b54.h"
#include "types.h"

struct UnkPlayer;
UnkPlayer* fn_7100dec788(UnkGameManager*);

struct UnkEmmyRegistry_b55;

struct UnkGrabVec_b58a3 {
    f32 x, y, z;
};

struct UnkGrabPart_b58a3 {
    void** vtable;
    u8 pad_0x8[0x54];
    bool field_0x5c;

    void vf_0x180(u32 a, u32 b) {
        reinterpret_cast<void (*)(UnkGrabPart_b58a3*, u32, u32)>(vtable[0x180 / 8])(this, a, b);
    }
};

struct UnkGrabPlayer_b58a3 {
    u8 pad_0x0[0x1c0];
    UnkGrabPart_b58a3* field_0x1c0;
};

struct UnkGrabAI_b58a3 {
    void** vtable;
    u8 pad_0x8[0x29ef0 - 0x8];
    s32 field_0x29ef0;

    const base::reflection::CType* vf_0x0() {
        return reinterpret_cast<const base::reflection::CType* (*)(UnkGrabAI_b58a3*)>(vtable[0])(this);
    }
};

struct UnkGrabAttack_b58a3 {
    u8 pad_0x0[0x8];
    UnkActor* owner;
    u8 pad_0x10[0x6a0];
    bool field_0x6b0;
};

void fn_710050802c(UnkGrabAttack_b58a3* self);

static inline __attribute__((always_inline)) const UnkGrabVec_b58a3* Addr_b58a3(const UnkGrabVec_b58a3& v) {
    return &v;
}

static inline __attribute__((always_inline)) UnkGrabVec_b58a3 Dir_b58a3(const UnkMatrix44& m) {
    return {m.m[2], m.m[6], m.m[10]};
}

static inline __attribute__((always_inline)) UnkGrabVec_b58a3 Pos_b58a3(const UnkActor* a) {
    return {a->field_0x44, a->field_0x54, a->field_0x64};
}

void fn_7100507a28(UnkGrabAttack_b58a3* self) {
    fn_7100507d24(self);
    if (self->field_0x6b0) {
        fn_710050802c(self);
    } else {
        UnkGrabPart_b58a3* part = reinterpret_cast<UnkGrabPlayer_b58a3*>(fn_7100dec788(g_unk_7101d47310))->field_0x1c0;
        if (!part->field_0x5c)
            part->vf_0x180(0, 0);
    }
    auto* ai = static_cast<UnkGrabAI_b58a3*>(self->owner->field_0x240);
    if (ai && fn_710009d50c(ai->vf_0x0(), fn_710053b934())) {
        if (void* target = self->owner->field_0x220) {
            // Declarations a case label jumps over get no lifetime markers: each send keeps its own
            // slots, as in the original.
            switch (ai->field_0x29ef0) {
            case 0:
                base::reflection::UnkReflTypeIdArg mode;
                mode.id = 0x81;
                base::reflection::UnkReflTypeIdArg mask;
                mask.id = UnkAll_SpitSend().value;
                fn_7101176690(target, 0xe27a4a9050c8ea64, 0x7c5476647f60cdaa, &mode, &g_unk_710164b958,
                              &g_unk_717276974c, &mask);
                break;
            case 1:
                base::reflection::UnkReflTypeIdArg mode2;
                mode2.id = 0x81;
                base::reflection::UnkReflTypeIdArg mask2;
                mask2.id = UnkAll_SpitSend().value;
                fn_7101176690(target, 0x76df0a29ce276dea, 0x7c5476647f60cdaa, &mode2, &g_unk_710164b958,
                              &g_unk_717276974c, &mask2);
                break;
            default:
                break;
            }
        }
    }
    UnkActor* owner = self->owner;
    fn_7100daeb50(g_unk_71726af0a8, 1, Addr_b58a3(Pos_b58a3(owner)),
                  Addr_b58a3(Dir_b58a3(fn_710002614c(reinterpret_cast<const UnkMatrix44*>(owner->field_0x38)))));
}
