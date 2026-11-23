// behaviortree::CEmmySearchingAction's unit (tools/tu_map.py unit 805): when the CEmmyMovement is in mode 1,
// picks the second search animation (the id the AI's blackboard holds, cleared after use, else one from the movement),
// plays it and listens to the animation through a weak handle. Names and layouts are placeholders.
#include "../../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAbilityShinespark.h"
#include "game/CAnimationComponent.h"
#include "game/CBlackboard.h"
#include "game/CEmmyMovement.h"
#include "game/CEnemyMovement.h"
#include "game/UnkNavMeshCell.h"
#include "game/UnkTUProperty.h"
#include "game/UnkTypedValue.h"
#include "game/behaviortree/CBehavior.h"
#include "game/behaviortree/CEmmySearchingAction.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

extern UnkPropertyName g_unk_71726df1c0;
// The animation ids by the movement's direction.
extern const u64 g_unk_7101649da0[] __attribute__((visibility("hidden")));


// An empty string id, built in place (CStrId has no default constructor; the original's is inline).
struct UnkEmptyStrId_l518abc {
    alignas(8) u8 storage[sizeof(CStrId)];

    UnkEmptyStrId_l518abc() {
        Id().data = nullptr;
        if (!g_unk_7101d47108)
            base::global::UnkEnsureStringPool();
        Id().data = &g_unk_7101d47108->empty;
    }
    ~UnkEmptyStrId_l518abc() { Id().~CStrId(); }
    CStrId& Id() { return *reinterpret_cast<CStrId*>(storage); }
};

struct UnkEmmyAI_l518abc : base::reflection::UnkReflObject {
    u8 pad_0x8[0x1258];
    CBlackboard blackboard;  // 0x1260
};

struct UnkEmmyActor_l518abc {
    u8 pad_0x0[0x1f0];
    UnkEmmyMovement* movement;  // 0x1f0
    u8 pad_0x1f8[0x28];
    UnkWeakTarget* animation;  // 0x220
    u8 pad_0x228[0x18];
    UnkEmmyAI_l518abc* ai;  // 0x240
};


// A listener on an animation: the callback with its context, and the weak handle of what it listens to.
struct UnkEmmyAnimLink_l518abc {
    u8 pad_0x0[0x8];
    void* context;
    void (*callback)();
    UnkWeakRef ref;  // 0x18

    // Stops listening to the animation the handle points at, and drops the handle.
    void Detach() {
        if (UnkWeakHandle* h = ref.handle) {
            if (UnkWeakTarget* target = h->target)
                fn_710117b3f0(target, this);
            ResetRef(&ref.handle);
        }
    }
    // Listens to `animation` instead.
    void Attach(UnkWeakTarget* animation) {
        Detach();
        if (animation) {
            fn_7100316018(animation, &ref);
            fn_710117b3e8(ref.handle ? ref.handle->target : nullptr, this);
        }
    }
};

struct UnkEmmySearchingAction_l518abc {
    u8 pad_0x0[0x18];
    UnkEmmyActor_l518abc* actor;  // 0x18
    u8 pad_0x20[0x18];
    u64 animation;  // 0x38
    UnkEmmyAnimLink_l518abc link;  // 0x40
};
static_assert(__builtin_offsetof(UnkEmmySearchingAction_l518abc, link.ref) == 0x58, "layout");

void fn_7100518abc(UnkEmmySearchingAction_l518abc* self) {
    UnkEmmyMovement* movement = self->actor->movement;
    if (movement && !fn_710009d50c(movement->vf_0x0(), fn_710057134c()))
        movement = nullptr;
    if (fn_71010cc3a4(movement) != 1)
        return;

    UnkWeakTarget* animation = self->actor->animation;
    UnkEmmyAI_l518abc* ai = self->actor->ai;
    if (ai && !fn_710009d50c(ai->vf_0x0(), fn_710053b934()))
        ai = nullptr;

    UnkEmptyStrId_l518abc empty;
    CStrId& id = empty.Id();
    // The cleared id's typed value keeps a slot of its own for the whole function.
    UnkTypedValue value;
    CBlackboard* blackboard = &ai->blackboard;
    if (CBlackboard::CSection* section = fn_7101047270(blackboard, g_unk_71726b7cb8))
        fn_710051d464(section, &g_unk_71726df1c0, &id);
    if (id.data != &g_unk_7101d47108->empty) {
        self->animation = id.data->Key();
        if (CBlackboard::CSection* section = fn_71010472e0(blackboard, g_unk_71726b7cb8)) {
            if (!blackboard->field_0xe8) {
                value.type = fn_7100009658();
                value.value = &g_unk_7101d470e0;
                fn_7100332c80(section, &g_unk_71726df1c0, &value);
            }
        }
    } else {
        UnkNavMeshCell* cell = movement->vf_0x2d0();
        if (cell && fn_71002b005c(cell))
            self->animation = 0x971fcfb14e82490c;
        else
            self->animation = g_unk_7101649da0[fn_71005717d4(movement, movement->field_0x9e4)];
    }

    u64 hash = self->animation;
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = 0;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    if (!fn_7101176690(animation, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740,
                       &mask)) {
        self->animation = ~0ull;
        self->link.Detach();
        return;
    }
    self->link.context = self;
    self->link.callback = fn_710051d568;
    self->link.Attach(animation);
}
