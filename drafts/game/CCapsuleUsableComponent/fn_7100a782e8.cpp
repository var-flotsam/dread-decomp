// CCapsuleUsableComponent (unit 1584): the capsule is used by `user`. It keeps the user's weak handle (+0xe8)
// and the name, then, when the first list's owner has a character class, plays the capsule's effect, sends
// that class the definition's first animation, hooks fn_7100a78db4 to it and sends the second list's owner
// the second one (mode 2); otherwise hands the use on (fn_7100aa1b08). Last, the player's input is
// switched on. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/UnkDestructors.h"
#include "base/UnkRefHandle.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game/CInputComponent.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2656.h"
#include "game/UnkTU437.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

struct UnkTeleporterOwner_b0233;
struct UnkTeleporterObject_b0233;
void fn_7100aa1b08(UnkTeleporterOwner_b0233* self, UnkTeleporterObject_b0233* object, void* arg);
void fn_7100a78db4();
extern char g_unk_717274fc28[] __attribute__((visibility("hidden")));

// What a list's owner reference points at: the actor at +0x10.
struct UnkCapsuleOwner_la782e8 : base::UnkRefCounted {
    UnkActor* actor;
};

// A callback list whose owner is looked up on first use (state 1, then 2).
struct UnkCapsuleList_la782e8 {
    void** vtable;
    u8 source[0x28];  // 0x8: what the owner is looked up from
    u32 state;        // 0x30
    base::UnkRefHandle owner;  // 0x38

    UnkCapsuleOwner_la782e8* Owner() {
        if (state == 1) {
            {
                base::UnkRefPtrMember found = fn_7100353be8(source);
                fn_710031d208(&owner, reinterpret_cast<base::UnkRefHandle*>(&found));
            }
            state = 2;
        }
        return static_cast<UnkCapsuleOwner_la782e8*>(owner.ptr);
    }
};
static_assert(sizeof(UnkCapsuleList_la782e8) == 0x40, "UnkCapsuleList_la782e8 layout");

struct UnkCapsuleLink_la782e8 {
    void** vtable;
    void* context;
    void (*callback)();
};

struct UnkCapsuleDef_la782e8 {
    u8 pad_0x0[0x1ac];
    f32 field_0x1ac;
    f32 field_0x1b0;
    u8 pad_0x1b4[0x1e0 - 0x1b4];
    CStrId anim_a;  // 0x1e0
    CStrId anim_b;  // 0x1e8
};

struct UnkCapsulePlayer_la782e8 {
    u8 pad_0x0[0x1d8];
    UnkInputComponent* input;  // 0x1d8
};

struct UnkCapsuleUsableComponent_la782e8 {
    u8 pad_0x0[0xe8];
    UnkWeakHandle* user;  // 0xe8
    u8 pad_0xf0[0x578 - 0xf0];
    CRntString name;  // 0x578
    UnkCapsuleList_la782e8 list_a;  // 0x5a8
    UnkCapsuleList_la782e8 list_b;  // 0x5e8
    UnkCapsuleLink_la782e8 link;    // 0x628
    UnkWeakHandle* handle;          // 0x640
    UnkCapsuleDef_la782e8* def;     // 0x648
};
static_assert(__builtin_offsetof(UnkCapsuleUsableComponent_la782e8, list_a) == 0x5a8, "layout");
static_assert(__builtin_offsetof(UnkCapsuleUsableComponent_la782e8, def) == 0x648, "layout");

static inline void* Track_la782e8(UnkCapsuleList_la782e8* list) {
    UnkCapsuleOwner_la782e8* owner = list->Owner();
    if (!owner || !owner->actor)
        return nullptr;
    return owner->actor->field_0x220;
}

bool fn_7100a782e8(UnkCapsuleUsableComponent_la782e8* self, UnkWeakTarget* user, const CRntString* name) {
    base::reflection::UnkReflTypeIdArg mode_a;
    base::math::CVector3D color;
    base::reflection::UnkReflTypeIdArg mask_a;
    base::reflection::UnkReflTypeIdArg mode_b;
    base::reflection::UnkReflTypeIdArg mask_b;
    if (!user)
        return false;
    AssignRef(&self->user, fn_7100d78378(user));
    fn_710007eaa0(&self->name, name);
    if (Track_la782e8(&self->list_a)) {
        f32 a = self->def->field_0x1ac;
        f32 b = self->def->field_0x1b0;
        color.x = 0.0f;
        color.y = 0.0f;
        color.z = 0.0f;
        fn_7101053c14(g_unk_7101d47310->field_0x1040, &color, a, b, 1.0f);
        void* track_a = self->list_a.Owner()->actor->field_0x220;
        u64 hash_a = self->def->anim_a.data->Key();
        mode_a.id = 0;
        mask_a.id = UnkAll_SpitSend().value;
        fn_7101176690(track_a, hash_a, 0xf4061ff25a2163d6, &mode_a, &g_unk_710164b958, &g_unk_7172769740, &mask_a);

        UnkCapsuleLink_la782e8* link = &self->link;
        link->context = self;
        link->callback = fn_7100a78db4;
        UnkWeakTarget* track = static_cast<UnkWeakTarget*>(self->list_a.Owner()->actor->field_0x220);
        if (UnkWeakHandle* handle = self->handle) {
            if (UnkWeakTarget* old = handle->target)
                fn_710117b3f0(old, link);
            ResetRef(&self->handle);
        }
        if (track) {
            fn_7100316018(track, &self->handle);
            fn_710117b3e8(self->handle ? self->handle->target : nullptr, link);
        }

        if (void* other = Track_la782e8(&self->list_b)) {
            u64 hash = self->def->anim_b.data->Key();
            mode_b.id = 2;
            mask_b.id = UnkAll_SpitSend().value;
            fn_7101176690(other, hash, 0xf4061ff25a2163d6, &mode_b, &g_unk_710164b958, &g_unk_7172769740, &mask_b);
        }
    } else {
        fn_7100aa1b08(reinterpret_cast<UnkTeleporterOwner_b0233*>(self),
                      reinterpret_cast<UnkTeleporterObject_b0233*>(self->user ? self->user->target : nullptr),
                      &self->name);
    }
    UnkCapsulePlayer_la782e8* player = reinterpret_cast<UnkCapsulePlayer_la782e8*>(fn_7100dec788(g_unk_7101d47310));
    if (player) {
        if (UnkInputComponent* input = player->input)
            fn_7100872460(input, true, false, g_unk_717274fc28);
    }
    return true;
}
