// UnkTU811 (CEmmyAIComponent's unit): whether the EMMI senses the player this frame (moving, making noise or
// busy) within `radius` of the owner; stamps the time it did, or clears the stamp when the player hid.
// Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/CMeleeComponent.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU1501.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU39.h"
#include "game/UnkVec3Bits.h"
#include "game/logic/collision/CCircleShape2D.h"
#include "types.h"

struct UnkPlayerMovementCache;
void* fn_71010d7a6c(UnkPlayerMovementCache* self);

struct UnkPlayerPart_b62a3 {
    u8 pad_0x0[0x178];
    bool field_0x178;
};
struct UnkPlayerPart2_b62a3;
bool fn_710083eb28(UnkPlayerPart_b62a3*);
bool fn_7100300cf4(UnkPlayerPart2_b62a3*);

struct UnkLavaDropsShape_A2_b71 {
    u8 pad_0x0[0x8];
    u8 body[0xa8];

    UnkLavaDropsShape_A2_b71(const void* source, f32 size) { fn_7100d6169c(this, source, size); }
    ~UnkLavaDropsShape_A2_b71() { fn_7100d61750(this); }
};

struct UnkFrameTime_l54a798 {
    u8 pad_0x0[0x2c];
    f32 now;  // 0x2c
};

struct UnkPlayerMovement_l54a798 {
    u8 pad_0x0[0xf9];
    bool alertsOnMove;  // 0xf9
};

struct UnkMorphState_l54a798 {
    u8 pad_0x0[0x58];
    f32 time;  // 0x58
};

struct UnkSensedPlayer_l54a798 {
    u8 pad_0x0[0x200];
    UnkPlayerMovement_l54a798* movement;  // 0x200
    void* field_0x208;
    u8 pad_0x210[0x10];
    UnkAnimationComponent* charClass;  // 0x220
    u8 pad_0x228[0x2a8 - 0x228];
    UnkPlayerPart_b62a3* field_0x2a8;
    u8 pad_0x2b0[0x30];
    UnkPlayerPart2_b62a3* field_0x2e0;
    u8 pad_0x2e8[0x10];
    UnkMeleeComponent* melee;  // 0x2f8
};

f32 fn_7100d7be34(UnkSensedPlayer_l54a798* player);
UnkMorphState_l54a798* fn_710054aae8(UnkSensedPlayer_l54a798* player);

struct UnkSenseContext_l54a798 {
    u8 pad_0x0[0x602];
    bool ignoreHiding;  // 0x602
};

struct UnkSenseState_l54a798 {
    bool shown;  // 0x0
    u8 pad_0x1[0xb];
    bool enabled;  // 0xc
    u8 pad_0xd[0x3];
    f32 lastSensed;  // 0x10
};

struct UnkEmmyAI_l54a798 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x14c14 - 0x38];
    UnkSenseState_l54a798 sense;  // 0x14c14
    u8 pad_0x14c28[0x29350 - 0x14c28];
    u8 cooldown[0x29ee0 - 0x29350];  // 0x29350, a timer
    UnkSenseContext_l54a798* context;  // 0x29ee0
};
static_assert(__builtin_offsetof(UnkEmmyAI_l54a798, sense) == 0x14c14, "UnkEmmyAI_l54a798 layout");
static_assert(__builtin_offsetof(UnkEmmyAI_l54a798, context) == 0x29ee0, "UnkEmmyAI_l54a798 layout");

// Whether the player is busy with something the EMMI notices.
static inline bool IsBusy(UnkSensedPlayer_l54a798* player) {
    if (player->melee && fn_710092b604(player->melee))
        return true;
    if (player == reinterpret_cast<UnkSensedPlayer_l54a798*>(fn_7100defcd4(g_unk_7101d47310))) {
        UnkMorphState_l54a798* morph = fn_710054aae8(player);
        if (morph && morph->time <= 0.0f)
            return true;
    }
    UnkPlayerPart_b62a3* part = player->field_0x2a8;
    if (part) {
        if (part->field_0x178 || fn_710083eb28(part) || fn_7100df59a8(g_unk_7101d47310))
            return true;
    }
    if (player->field_0x2e0 && fn_7100300cf4(player->field_0x2e0))
        return true;
    return false;
}

// Whether the player moves fast enough or makes noise.
static inline bool IsAudible(UnkSensedPlayer_l54a798* player, UnkPlayerMovement_l54a798* movement, void* current) {
    f32 threshold = player == current ? 0.1f : 50.0f;
    if (fn_7100d7be34(player) > threshold) {
        if (movement->alertsOnMove && player->field_0x208 &&
            !fn_71009e75e8(static_cast<UnkFlagged*>(player->field_0x208))) {
            if (player->field_0x208)
                fn_71009e6434(static_cast<UnkHelper*>(player->field_0x208));
        }
        return true;
    }
    UnkAnimationComponent* charClass = player->charClass;
    return charClass && fn_710117b348(charClass, fn_7100c3d378());
}

bool fn_710054a798(UnkEmmyAI_l54a798* self, f32 radius) {
    if (!self->sense.enabled || !self->sense.shown)
        return false;
    if (fn_710002fb70(self->cooldown))
        return false;
    UnkAnimationComponent* charClass = static_cast<UnkAnimationComponent*>(self->owner->field_0x220);
    if (!charClass || fn_710117b348(charClass, fn_7100c78d04()))
        return false;

    auto* player = reinterpret_cast<UnkSensedPlayer_l54a798*>(fn_7100dec788(g_unk_7101d47310));
    if (!player)
        return false;
    UnkPlayerMovement_l54a798* movement = player->movement;

    bool hiding;  // unset in the original while the context ignores hiding
    void* current;
    if (self->context->ignoreHiding) {
        current = fn_7100defcd4(g_unk_7101d47310);
    } else {
        hiding = fn_7100df5740(g_unk_7101d47310);
        current = fn_7100defcd4(g_unk_7101d47310);
    }
    bool sensed = (!hiding && IsAudible(player, movement, current)) || IsBusy(player);

    if (sensed) {
        void* body = nullptr;
        auto* target = reinterpret_cast<UnkSensedPlayer_l54a798*>(fn_7100d7c990(self->owner));
        if (target && target->movement)
            body = fn_71010d7a6c(reinterpret_cast<UnkPlayerMovementCache*>(target->movement));
        UnkActor* owner = self->owner;
        UnkVec3Bits center = UnkVec3Bits::CopyBits(owner->field_0x44, owner->field_0x54, owner->field_0x64);
        UnkLavaDropsShape_A2_b71 shape(&center, radius);
        if (fn_7100d57ca0(&shape, shape.body, body, nullptr, 0)) {
            self->sense.lastSensed = reinterpret_cast<UnkFrameTime_l54a798*>(g_unk_7101d47328)->now;
            return true;
        }
    }
    if (hiding)
        self->sense.lastSensed = -3.40282347e38f;
    return false;
}
