// CFootstepPlatformComponent's unit (tools/tu_map.py unit 1255): an event handler. Nothing happens
// while the player's ghost aura (fn_7100308c74) is in states 3-6, its shinespark (fn_7100327d14) is
// charged (state 4), or its movement stands on something (as fn_71007e8744 tests it). Otherwise, when
// the sender is one of the two unit statics, sets the flag at +0xd0, else runs fn_71007e8374.
// Returns 0. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkPlayer;
struct UnkAbility;
struct UnkAbilitySet;
struct UnkFootstepMode;
UnkPlayer* fn_7100dec788(UnkGameManager* manager);
UnkAbility* fn_71003012f8(UnkAbilitySet* set, u64 id);
u64 fn_7100308c74();
u64 fn_7100327d14();
u32 fn_7100309568(UnkAbility* ability);
u32 fn_71009a60fc();
bool fn_71009ae798(UnkFootstepMode* mode);
bool fn_71009e12d0(void* obj);

struct UnkFootstepStatic_b45a2 {
    u8 pad_0x0[0x30];
    u64 id;
};
__attribute__((visibility("hidden"))) extern UnkFootstepStatic_b45a2* g_unk_717271fae0;
__attribute__((visibility("hidden"))) extern UnkFootstepStatic_b45a2* g_unk_717271fad8;

struct UnkStepMode_ovb0138 {
    u8 pad_0x0[0x10];
    u32 kind;
};

struct UnkStepMovement_ovb0138 {
    u8 pad_0x0[0x2d08];
    UnkStepMode_ovb0138* mode;
    u8 pad_0x2d10[0x8];
    u32 count;
    UnkStepMode_ovb0138* modes[1];
} __attribute__((packed));

struct UnkStepPlayer_ovb0138 {
    u8 pad_0x0[0x208];
    UnkStepMovement_ovb0138* movement;
    u8 pad_0x210[0x2e0 - 0x210];
    UnkAbilitySet* abilities;
};

struct UnkStepShinesparkState_ovb0138 {
    u8 pad_0x0[0x10];
    s32 state;
};

struct UnkStepShinespark_ovb0138 : UnkReflObject {
    u8 pad[0x6b0 - sizeof(UnkReflObject)];
    UnkStepShinesparkState_ovb0138* charge;
};

struct UnkStepSenderObject_ovb0138 {
    u8 pad_0x0[0x18];
    u64 id;
};

struct UnkStepSender_ovb0138 {
    u8 pad_0x0[0x10];
    UnkStepSenderObject_ovb0138* object;
};

struct UnkStepEvent_ovb0138 {
    u8 pad_0x0[0x18];
    UnkStepSender_ovb0138* sender;
};

struct UnkFootstepPlatform_ovb0138 {
    u8 pad_0x0[0xd0];
    bool stepped;
};

void fn_71007e8374(UnkFootstepPlatform_ovb0138* self);

static inline UnkStepPlayer_ovb0138* Player() {
    return reinterpret_cast<UnkStepPlayer_ovb0138*>(fn_7100dec788(g_unk_7101d47310));
}

static inline UnkReflObject* PlayerAbility(u64 (*id)()) {
    if (!Player()->abilities)
        return nullptr;
    UnkAbilitySet* abilities = Player()->abilities;
    return reinterpret_cast<UnkReflObject*>(fn_71003012f8(abilities, id()));
}

// The mode of the given kind (as fn_71007e8744 in CFootstepPlatformComponent.cpp finds it).
static inline UnkStepMode_ovb0138* FindMode(UnkStepMovement_ovb0138* movement, u32 kind) {
    UnkStepMode_ovb0138** it = movement->modes;
    u64 size = (u64)movement->count << 3;
    asm("" : "+r"(size));
    if (size != 0) {
        do {
            UnkStepMode_ovb0138* mode = *it;
            if (mode->kind == kind)
                return mode;
            ++it;
            size -= 8;
        } while (size != 0);
    }
    return nullptr;
}

u32 fn_71007e81a0(UnkFootstepPlatform_ovb0138* self, UnkStepEvent_ovb0138* event) {
    UnkReflObject* aura = PlayerAbility(fn_7100308c74);
    if (aura && fn_710009d50c(aura->vf_0x0(), fn_7100306914())) {
        if (fn_7100309568(reinterpret_cast<UnkAbility*>(aura)) - 3 < 4)
            return 0;
    }
    UnkReflObject* spark = PlayerAbility(fn_7100327d14);
    if (spark && fn_710009d50c(spark->vf_0x0(), fn_71003255a4())) {
        UnkStepShinesparkState_ovb0138* charge = static_cast<UnkStepShinespark_ovb0138*>(spark)->charge;
        if (charge && charge->state == 4)
            return 0;
    }
    if (UnkStepMovement_ovb0138* movement = Player()->movement) {
        u32 kind = fn_71009a60fc();
        if (kind <= 14) {
            UnkStepMode_ovb0138* mode = FindMode(movement, kind);
            if (mode && movement->mode == mode && fn_71009ae798(reinterpret_cast<UnkFootstepMode*>(mode)))
                return 0;
        }
        if (fn_71009e12d0(movement))
            return 0;
    }
    u64 id = event->sender->object->id;
    if (g_unk_717271fae0->id == id || g_unk_717271fad8->id == id)
        self->stepped = true;
    else
        fn_71007e8374(self);
    return 0;
}
