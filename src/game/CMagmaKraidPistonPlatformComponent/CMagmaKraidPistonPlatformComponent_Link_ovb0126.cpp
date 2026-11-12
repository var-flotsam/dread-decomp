#include "../../game_s2/UnkSpitSend.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

void fn_7100919a04();


struct UnkKraidStaticZero_ovb0126 {
    u32 value;
    UnkKraidStaticZero_ovb0126() : value(0) {}
};

inline const UnkKraidStaticZero_ovb0126& UnkKraidStaticZero() {
    static UnkKraidStaticZero_ovb0126 s_value asm("g_unk_7101d57cc8");
    return s_value;
}

inline u32 UnkKraidDefaultMask() {
    static u32 s_mask asm("g_unk_7101d57cb8") = ~UnkKraidStaticZero().value & 0xff;
    return s_mask;
}

struct UnkKraidLink_ovb0126 {
    void** vtable;
    void* context;
    void (*callback)();
};

struct UnkKraidOwner_ovb0126 {
    u8 pad_0x0[0x220];
    UnkWeakTarget* field_0x220;
};

struct UnkKraidLinked_ovb0126 {
    u8 pad_0x0[0x30];
    UnkKraidOwner_ovb0126* owner;
    u8 pad_0x38[0x2b8 - 0x38];
    UnkKraidLink_ovb0126 link;
    UnkWeakHandle* handle;  // 0x2d0
};

static inline __attribute__((always_inline)) void SendAndLink(UnkKraidLinked_ovb0126* self, u64 hash,
                                                                void (*callback)()) {
    {
        using namespace base::reflection;
        UnkWeakTarget* actions = self->owner->field_0x220;
        UnkReflTypeIdArg value;
        value.id = 0;
        UnkReflTypeIdArg arg;
        arg.id = UnkKraidDefaultMask();
        fn_7101176690(actions, hash, 0xf4061ff25a2163d6, &value, &g_unk_710164b958, &g_unk_7172769740, &arg);
    }
    UnkWeakTarget* track = self->owner->field_0x220;
    if (!track)
        return;
    UnkKraidLink_ovb0126* link = &self->link;
    link->context = self;
    link->callback = callback;
    if (UnkWeakHandle* handle = self->handle) {
        if (UnkWeakTarget* target = handle->target)
            fn_710117b3f0(target, link);
        if (UnkWeakHandle* p = self->handle) {
            if (--p->ref_count == 0)
                p->vf_0x10();
            self->handle = nullptr;
        }
    }
    fn_7100316018(track, reinterpret_cast<UnkWeakHandle**>(reinterpret_cast<u8*>(self) + 0x2d0));
    fn_710117b3e8(self->handle ? self->handle->target : nullptr, link);
}

void fn_7100918f1c(UnkKraidLinked_ovb0126* self) {
    SendAndLink(self, 0x318b46a4b09e64bd, fn_7100919a04);
}

void fn_71009190b8(UnkKraidLinked_ovb0126* self) {
    SendAndLink(self, 0x638b792fde3aff23, fn_7100919a04);
}
