// CItemLifeComponent's unit (tools/tu_map.py unit 1379): the hit override. Flags the owner's object
// (fn_71005c1008, fn_7100a68480) and gives its weak target the id of g_unk_7172732460; then, when the owner
// has its animation (+0x220), hooks the listener at +0x318 (callback fn_71008e2a74) up to it, keeps `info`
// at +0x338 and sends it a one under a fixed hash; without one it dies at once (fn_71008e1ea4). The base
// call fn_710116064c follows. Names and layouts are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game/CNavMeshItemComponent.h"
#include "game/UnkTU372.h"
#include "game/UnkTU883.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkLifeBase_A5_b63;
struct UnkLifeArg_A5_b63;
struct UnkHitInfo_A5_b63;
void fn_710116064c(UnkLifeBase_A5_b63* self, UnkLifeArg_A5_b63* arg, UnkHitInfo_A5_b63* info);

struct UnkItemLife_l8e1ea4;
void fn_71008e1ea4(UnkItemLife_l8e1ea4* self);

void fn_7100a68480(char* object, bool flag);
void fn_71008e2a74(void* self);

// The id the owner's weak target is given (+0x30).
struct UnkItemLifeIds_l8e1cbc {
    u8 pad_0x0[0x30];
    u64 id;
};
__attribute__((visibility("hidden"))) extern UnkItemLifeIds_l8e1cbc* g_unk_7172732460;

struct UnkItemLifeOwner_l8e1cbc {
    u8 pad_0x0[0x220];
    UnkWeakTarget* animation;  // 0x220
};

// A listener that calls back into its owner.
struct UnkItemLifeListener_l8e1cbc {
    void** vtable;
    void* owner;              // 0x8
    void (*callback)(void*);  // 0x10
};

struct UnkItemLife_l8e1cbc {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkItemLifeOwner_l8e1cbc* owner;  // 0x30
    u8 pad_0x38[0x318 - 0x38];
    UnkItemLifeListener_l8e1cbc listener;  // 0x318
    UnkWeakHandle* handle;                 // 0x330, what the listener is hooked up to
    UnkHitInfo_A5_b63* info;               // 0x338

    // Unhooks the listener from the target of its handle and drops the handle.
    void Unhook() {
        if (UnkWeakHandle* h = handle) {
            if (UnkWeakTarget* target = h->target)
                fn_710117b3f0(target, &listener);
            if (UnkWeakHandle* p = handle) {
                if (--p->ref_count == 0)
                    p->vf_0x10();
                handle = nullptr;
            }
        }
    }
};
static_assert(__builtin_offsetof(UnkItemLife_l8e1cbc, info) == 0x338, "layout");

void fn_71008e1cbc(UnkItemLife_l8e1cbc* self, u32 kind, UnkHitInfo_A5_b63* info) {
    if (char* object = fn_71005c1008(reinterpret_cast<u64>(self->owner)))
        fn_7100a68480(object, true);
    if (void* target = fn_71002b3b84(reinterpret_cast<UnkWeakTarget*>(self->owner)))
        fn_71009c0f1c(target, g_unk_7172732460->id);
    if (UnkWeakTarget* animation = self->owner->animation) {
        self->listener.owner = self;
        self->listener.callback = fn_71008e2a74;
        self->Unhook();
        CAbilityGhostAura_AssignWeakHandleToRef(animation, &self->handle);
        fn_710117b3e8(self->handle ? self->handle->target : nullptr, &self->listener);
        self->info = info;
        base::reflection::refl_EmitU32(animation, 0xc567047b7a4bf0cf, 0xf4061ff25a2163d6, 1, &g_unk_710164b958,
                                       &g_unk_7172769740);
    } else {
        fn_71008e1ea4(reinterpret_cast<UnkItemLife_l8e1ea4*>(self));
    }
    // The base takes the word `kind` (w1); its shared declaration still types it as a pointer.
    fn_710116064c(reinterpret_cast<UnkLifeBase_A5_b63*>(self), reinterpret_cast<UnkLifeArg_A5_b63*>(u64(kind)), info);
}
