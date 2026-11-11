// The CEmmyAIComponent unit. fn_71005521a0 forwards a non-empty id from an event (its CStrId at
// +0x10) to the owner's sub-object (+0x220 of the owner at +0x30) as a request with mode 0, when
// the flag at +0x27720 is clear, the state at +0x29ef0 is 1 and fn_710054c504(self, 1) holds;
// descriptors as UnkSpitSend (src/game_s2/UnkSpitSend.h). Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "game/CAnimationComponent.h"
#include "types.h"

using base::global::CStrId;

struct UnkEmmyOwner_b57a3 {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

struct UnkEmmyAI_b57a3 {
    u8 pad_0x0[0x30];
    UnkEmmyOwner_b57a3* owner;
    u8 pad_0x38[0x27720 - 0x38];
    bool field_0x27720;
    u8 pad_0x27721[0x29ef0 - 0x27721];
    s32 field_0x29ef0;
};

struct UnkEmmyIdEvent_b57a3 {
    u8 pad_0x0[0x10];
    CStrId id;
};

bool fn_710054c504(UnkEmmyAI_b57a3* self, bool flag);

void fn_71005521a0(UnkEmmyAI_b57a3* self, UnkEmmyIdEvent_b57a3* event) {
    if (!event || event->id.data == &g_unk_7101d47108->empty)
        return;
    if (!self->owner->field_0x220)
        return;
    if (self->field_0x27720 || self->field_0x29ef0 != 1)
        return;
    if (!fn_710054c504(self, true))
        return;
    void* target = self->owner->field_0x220;
    u64 hash = *reinterpret_cast<u64*>(reinterpret_cast<char*>(event->id.data) + 0x30);
    UnkSpitSend(target, hash, 0xf4061ff25a2163d6);
}

// fn_7100552f10: when the owner's animation (+0x220 of the owner at +0x30) plays
// "grabemmysamus-stagger_loop" or "grabemmysamus-stagger-recovery_init" (fn_710117af10), sends it
// the hash of "grabemmysamus-stagger-recovery" with mode 0; then adds the key g_unk_71726e1fc0 to the
// key list at +0x29418 (fn_710002ffb4).
#include "game/UnkGuiRoot.h"

struct UnkStruct_710117af10;
struct UnkCls_710117af10;
extern CStrId g_unk_71726e1fc0 __attribute__((visibility("hidden")));

struct UnkEmmyAIStagger_b57a3 {
    u8 pad_0x0[0x30];
    UnkEmmyOwner_b57a3* owner;
    u8 pad_0x38[0x29418 - 0x38];
    UnkGuiKeyList field_0x29418;
};

static inline UnkCls_710117af10** AsName_b57a3(const CStrId& id) {
    return reinterpret_cast<UnkCls_710117af10**>(const_cast<CStrId*>(&id));
}

void fn_7100552f10(UnkEmmyAIStagger_b57a3* self) {
    static const CStrId s_loop("grabemmysamus-stagger_loop", true);
    static const CStrId s_init("grabemmysamus-stagger-recovery_init", true);
    if (fn_710117af10(static_cast<UnkStruct_710117af10*>(self->owner->field_0x220), AsName_b57a3(s_loop), false) ||
        fn_710117af10(static_cast<UnkStruct_710117af10*>(self->owner->field_0x220), AsName_b57a3(s_init), false)) {
        void* target = self->owner->field_0x220;
        UnkSpitSend(target, fn_7100001570("grabemmysamus-stagger-recovery", 30), 0xf4061ff25a2163d6);
    }
    fn_710002ffb4(&self->field_0x29418, &g_unk_71726e1fc0);
}
