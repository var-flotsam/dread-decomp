// CActionSwitcherOnPullGrapplePointComponent's pull: after the base step (fn_710082ccec), sends the owner's
// animation component (+0x220 of +0x30) a mode-0 request under the id of the pulled action (+0xa0) when
// `pulled`, marking the pull done (+0x98), running fn_710082cee4/fn_710082cf08 and calling the component's
// vf_0x1a8 with the game manager's blackboard (+0x968); under the id behind +0x90's +0x98 otherwise. Names
// and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkGameManager.h"
#include "types.h"

struct UnkPullIdHolder_b0266 {
    u8 pad_0x0[0x30];
    u64 hash;  // 0x30
};

struct UnkPullIdRef_b0266 {
    u8 pad_0x0[0x98];
    UnkPullIdHolder_b0266* holder;  // 0x98
};

struct UnkPullOwner_b0266 {
    u8 pad_0x0[0x220];
    void* animation;  // 0x220
};

struct UnkPull_b0266 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkPullOwner_b0266* owner;  // 0x30
    u8 pad_0x38[0x58];
    UnkPullIdRef_b0266* field_0x90;
        bool done;  // 0x98
    u8 pad_0x99[7];
    UnkPullIdHolder_b0266* field_0xa0;

    void vf_0x1a8(void* blackboard, bool flag) {
        reinterpret_cast<void (*)(UnkPull_b0266*, void*, bool)>(vtable[0x1a8 / 8])(this, blackboard, flag);
    }
};

struct UnkDoorGrapplePoint_A5_b66;
struct UnkPullableGrapplePoint_b54;
void fn_710082ccec(UnkDoorGrapplePoint_A5_b66* self, void*, bool);
void fn_710082cee4(UnkPullableGrapplePoint_b54* self, bool);
void fn_710082cf08(UnkPullableGrapplePoint_b54* self, bool);

void fn_71008263e4(UnkPull_b0266* self, void* arg, bool pulled) {
    fn_710082ccec(reinterpret_cast<UnkDoorGrapplePoint_A5_b66*>(self), arg, pulled);
    void* animation = self->owner->animation;
    switch (static_cast<int>(pulled)) {
    default:
        base::reflection::UnkReflTypeIdArg mode_b;
        base::reflection::UnkReflTypeIdArg mask_b;
        u64 hash_b;
        hash_b = self->field_0x90->holder->hash;
        mode_b.id = 0;
        mask_b.id = UnkAll_SpitSend().value;
        fn_7101176690(animation, hash_b, 0xf4061ff25a2163d6ull, &mode_b, &g_unk_710164b958, &g_unk_7172769740,
                      &mask_b);
        break;
    case 1:
        base::reflection::UnkReflTypeIdArg mode_a;
        base::reflection::UnkReflTypeIdArg mask_a;
        u64 hash_a;
        hash_a = self->field_0xa0->hash;
        mode_a.id = 0;
        mask_a.id = UnkAll_SpitSend().value;
        fn_7101176690(animation, hash_a, 0xf4061ff25a2163d6ull, &mode_a, &g_unk_710164b958, &g_unk_7172769740,
                      &mask_a);
        self->done = true;
        fn_710082cee4(reinterpret_cast<UnkPullableGrapplePoint_b54*>(self), false);
        fn_710082cf08(reinterpret_cast<UnkPullableGrapplePoint_b54*>(self), true);
        self->vf_0x1a8(&g_unk_7101d47310->field_0x968, true);
        break;
    }
}
