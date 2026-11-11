// CFootstepPlatformComponent: fn_71007e8744 marks the flag at +0xc8 when the player's movement
// (+0x208 of the player) stands on something: its current mode (+0x2d08) is the one of the kind
// fn_71009a60fc reports and fn_71009ae798 accepts it, or else fn_71009e12d0 says so. Then it runs
// the object's vf_0x168. Names and layouts are placeholders.
#include "game/CSamusMagnetGloveMovementState/CParams.h"
#include "game/UnkGameManager.h"
#include "types.h"

struct UnkPlayer;
struct UnkFootstepArg;

struct UnkFootstepMode {
    u8 pad_0x0[0x10];
    u32 kind;
};

struct UnkFootstepMovement {
    u8 pad_0x0[0x2d08];
    UnkFootstepMode* mode;
    u8 pad_0x2d10[0x8];
    u32 count;
    UnkFootstepMode* modes[1];
} __attribute__((packed));

struct UnkFootstepPlayer {
    u8 pad_0x0[0x208];
    UnkFootstepMovement* movement;
};

UnkPlayer* fn_7100dec788(UnkGameManager* manager);
u32 fn_71009a60fc();
bool fn_71009e12d0(void* obj);

struct UnkFootstepPlatform {
    virtual void vf_0x0();
    void Dispatch(UnkFootstepArg* arg) {
        reinterpret_cast<void (*)(UnkFootstepPlatform*, UnkFootstepArg*)>((*reinterpret_cast<void***>(this))[0x168 / 8])(this, arg);
    }
    u8 pad_0x8[0xc0];
    bool field_0xc8;
};

static inline UnkFootstepMode* FindMode(UnkFootstepMovement* movement, u32 kind) {
    UnkFootstepMode** it = movement->modes;
    u64 size = (u64)movement->count << 3;
    asm("" : "+r"(size));
    if (size != 0) {
        do {
            UnkFootstepMode* mode = *it;
            if (mode->kind == kind)
                return mode;
            ++it;
            size -= 8;
        } while (size != 0);
    }
    return nullptr;
}

void fn_71007e8744(UnkFootstepPlatform* self, UnkFootstepArg* arg) {
    UnkFootstepPlayer* player = reinterpret_cast<UnkFootstepPlayer*>(fn_7100dec788(g_unk_7101d47310));
    if (UnkFootstepMovement* movement = player->movement) {
        u32 kind = fn_71009a60fc();
        bool on = false;
        if (kind <= 14) {
            UnkFootstepMode* mode = FindMode(movement, kind);
            if (mode && movement->mode == mode && fn_71009ae798(mode))
                on = true;
        }
        if (on || fn_71009e12d0(movement))
            self->field_0xc8 = true;
    }
    self->Dispatch(arg);
}
