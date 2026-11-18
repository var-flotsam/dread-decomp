// UnkTU811 (CEmmyAIComponent's unit): the E.M.M.I.'s damage handler. Answers 1 while the owner cannot take
// damage, else the base handler's answer or one worked out from the hit (its flags, whether it is the
// grab counter "grabemmysamusqte1win" and the grab mode at +0x29ef0); a hit from the player or a moving
// source then turns the E.M.M.I. towards the player (fn_7100546a30). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CAnimationComponent.h"
#include "game/CGrabComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1370.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2702.h"
#include "game/UnkTU794.h"
#include "game/UnkTU811.h"
#include "types.h"

using base::global::CStrId;

bool fn_710054782c(UnkEmmyRelaxAI_b59* self);
void fn_7100552324(UnkEmmyRelaxAI_b59* self);
u32 fn_710086bf60(UnkDoorLifePart_b56* part, void* event);

struct UnkRefl_fn_7100554660;
template <>
base::reflection::UnkReflObject*
base::reflection::refl_FindTyped<UnkRefl_fn_7100554660>(void*);

struct UnkDamageKind_l5542c0 {
    u8 pad_0x0[0x30];
    u64 id;  // 0x30
};
__attribute__((visibility("hidden"))) extern UnkDamageKind_l5542c0* g_unk_71726e1f98;

struct UnkDamageSourceRef_l5542c0 {
    u8 pad_0x0[0x10];
    void* object;  // 0x10
};

struct UnkDamageInfo_l5542c0 {
    u8 pad_0x0[0x18];
    u64 kind;  // 0x18
};

struct UnkDamageEvent_l5542c0 {
    u8 pad_0x0[0x8];
    UnkDamageSourceRef_l5542c0* source;  // 0x8
    u8 pad_0x10[0x50];
    UnkDamageInfo_l5542c0* info;          // 0x60
    u8 pad_0x68[0x4c];
    u32 flags;                            // 0xb4
};

struct UnkDamageParams_l5542c0 {
    u8 pad_0x0[0x50];
    f32 field_0x50;
};

struct UnkDamageOwner_l5542c0 {
    u8 pad_0x0[0x1b8];
    UnkDamageParams_l5542c0* params;      // 0x1b8
    u8 pad_0x1c0[0x60];
    UnkStruct_710117af10* animation;      // 0x220
    u8 pad_0x228[0x38];
    void* grab;                           // 0x260
};

struct UnkWeakRef_l5542c0 {
    u8 pad_0x0[0x10];
    void* target;  // 0x10
};

struct UnkEmmyRecord_l5542c0 {
    UnkWeakRef_l5542c0* handle;
    u32 state;      // 0x8
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    u8 pad_0x18[0x2];
    bool field_0x1a;
    u8 pad_0x1b[0x40 - 0x1b];
    u8 field_0x40;
    u8 pad_0x41[0x3];
    u32 field_0x44;
    u32 field_0x48;
    u32 field_0x4c;
    u32 field_0x50;
    u32 field_0x54;
    u32 field_0x58;
};

struct UnkEmmyAI_l5542c0 {
    u8 pad_0x0[0x30];
    UnkDamageOwner_l5542c0* owner;  // 0x30
    u8 pad_0x38[0x14b40 - 0x38];
    UnkEmmyRecord_l5542c0 record;   // 0x14b40
    u8 pad_0x14ba0[0x27720 - 0x14ba0];
    bool patrolPending;             // 0x27720
    u8 pad_0x27721[0x29ef0 - 0x27721];
    u32 grabMode;                   // 0x29ef0
    u8 pad_0x29ef4[0x4];
    u32 field_0x29ef8;              // 0x29ef8
};
static_assert(__builtin_offsetof(UnkEmmyAI_l5542c0, patrolPending) == 0x27720, "UnkEmmyAI_l5542c0 layout");
static_assert(__builtin_offsetof(UnkEmmyAI_l5542c0, field_0x29ef8) == 0x29ef8, "UnkEmmyAI_l5542c0 layout");

// Whether the hit is of the grab-counter kind.
static inline bool IsCounterHit(const UnkDamageEvent_l5542c0* event) {
    return g_unk_71726e1f98->id == event->info->kind;
}

// Whether the owner plays the animation of Samus winning the grab counter.
static inline bool PlaysCounterWin(UnkEmmyAI_l5542c0* self) {
    static const CStrId s_win("grabemmysamusqte1win", true);
    return fn_710117af10(self->owner->animation,
                         reinterpret_cast<UnkCls_710117af10**>(const_cast<CStrId*>(&s_win)), false);
}

u32 fn_71005542c0(UnkEmmyAI_l5542c0* self, UnkDamageEvent_l5542c0* event) {
    if (!fn_7100d7bea0(reinterpret_cast<UnkActor*>(self->owner), 1))
        return 1;

    UnkEmmyRecord_l5542c0* record = &self->record;
    u32 partResult = 0;
    if (UnkDoorLifePart_b56* part = fn_71005012bc(reinterpret_cast<UnkDoorLifeOwner_b56*>(self->owner)))
        partResult = fn_710086bf60(part, event);

    u32 result = fn_7101105e44(reinterpret_cast<UnkSluggerAI*>(self), reinterpret_cast<UnkSluggerEvent*>(event));
    if (!result) {
        if (self->patrolPending) {
            result = 2;
        } else if (event->flags & 0xe) {
            if ((event->flags & 0x80) && IsCounterHit(event) && !PlaysCounterWin(self)) {
                switch (self->grabMode) {
                case 1:
                    if (self->owner->grab && fn_710113d83c(self->owner->grab)) {
                        result = 6;
                        break;
                    }
                    if (event->flags & 0x400)
                        fn_71008c1538(reinterpret_cast<UnkRequest*>(event), self->owner->params->field_0x50);
                    result = 0;
                    break;
                case 0:
                    result = 1;
                    break;
                default:
                    result = 0;
                    break;
                }
            } else {
                result = 2;
            }
        } else if ((event->flags & 0x80) && self->grabMode == 0 && IsCounterHit(event) && !PlaysCounterWin(self)) {
            self->field_0x29ef8 = 0;
            result = 12;
        } else {
            result = partResult ? partResult : 2;
        }
    }

    if (record->state == 2)
        return result;
    if (!event->source || !event->source->object)
        return result;
    if (!fn_7100df5c38(g_unk_7101d47310, event->source->object) &&
        !base::reflection::refl_FindTyped<UnkRefl_fn_7100554660>(event->source ? event->source->object : nullptr))
        return result;

    UnkPlayer* player = fn_7100dec788(g_unk_7101d47310);
    if (fn_710054782c(reinterpret_cast<UnkEmmyRelaxAI_b59*>(self)))
        return result;
    fn_7100546a30(reinterpret_cast<UnkEmmyRelaxAI_b59*>(self), 1, reinterpret_cast<UnkActor*>(player), nullptr);
    if (record->handle && record->handle->target) {
        u32 a = record->field_0xc;
        record->field_0x44 = record->field_0x10;
        record->field_0x48 = record->field_0x14;
        record->field_0x4c = 0;
        record->field_0x50 = a;
        if (record->field_0x1a)
            record->field_0x58 = a;
        if (record->field_0x40 != 1) {
            record->field_0x40 = 1;
            fn_7100552324(reinterpret_cast<UnkEmmyRelaxAI_b59*>(self));
        }
    }
    fn_71005479a0(reinterpret_cast<UnkEmmyRelaxAI_b59*>(self));
    return result;
}
