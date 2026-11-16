// CTunableAIManager unit: turns a link to the player's input on or off. When the state changes, the
// INPUT component is moved between the player and the owner's actor (fn_7100d7822c) and the actor's
// controller is told. Names and layouts are placeholders.
#include <initializer_list>

#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "types.h"

using base::global::CStrId;

namespace base { struct UnkAllocator; }

struct UnkActor_710023259c;
struct UnkStrIdArray {
    base::global::CStrId* data;
    u32 count;
};
void fn_7100d7822c(UnkActor_710023259c* from, UnkActor_710023259c* to, const UnkStrIdArray& names);

// A string-id vector built from a list of ids, a growth step, an allocator and allocation flags.
struct UnkStrIdList_daa2ac : UnkStrIdArray {
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkStrIdList_daa2ac(std::initializer_list<CStrId> ids, u32 step, base::UnkAllocator* allocator,
                        u32 flags);  // fn_710097bb7c
    ~UnkStrIdList_daa2ac();          // fn_71008600a8
};

inline const CStrId& Kind_INPUT() {
    static const CStrId s_id("INPUT", true);
    return s_id;
}

// The actor's input controller: told when the input is linked or unlinked.
struct UnkInputController_daa2ac {
    void** vtable;

    void OnLinked() { reinterpret_cast<void (*)(UnkInputController_daa2ac*)>(vtable[0x538 / 8])(this); }
    void OnUnlinked() { reinterpret_cast<void (*)(UnkInputController_daa2ac*)>(vtable[0x540 / 8])(this); }
};

struct UnkLinkActor_daa2ac {
    u8 unk_0x0[0x240];
    UnkInputController_daa2ac* controller;
};

struct UnkLinkOwner_daa2ac {
    u8 unk_0x0[0x10];
    UnkLinkActor_daa2ac* actor;
};

struct UnkInputLink_daa2ac {
    UnkLinkOwner_daa2ac* owner;
    bool active;

    UnkLinkActor_daa2ac* Actor() const { return owner ? owner->actor : nullptr; }
};

struct UnkLinkPlayer_daa2ac {
    u8 unk_0x0[0x1d8];
    void* input;
};

void fn_7100daa2ac(UnkInputLink_daa2ac* self, bool enable) {
    static const UnkStrIdList_daa2ac s_kinds({Kind_INPUT()}, 10, nullptr, 0xf);
    bool active = (self->Actor() != nullptr) & enable;
    if (self->active == active)
        return;
    self->active = active;
    auto* player = reinterpret_cast<UnkLinkPlayer_daa2ac*>(fn_7100dec788(g_unk_7101d47310));
    if (self->active) {
        if (!player || !player->input)
            return;
        UnkLinkActor_daa2ac* actor = self->Actor();
        fn_7100d7822c(reinterpret_cast<UnkActor_710023259c*>(player),
                      reinterpret_cast<UnkActor_710023259c*>(actor), s_kinds);
        if (UnkInputController_daa2ac* controller = self->owner->actor->controller)
            controller->OnLinked();
    } else {
        UnkLinkActor_daa2ac* actor = self->Actor();
        if (!player || !actor)
            return;
        fn_7100d7822c(reinterpret_cast<UnkActor_710023259c*>(actor),
                      reinterpret_cast<UnkActor_710023259c*>(player), s_kinds);
        if (UnkInputController_daa2ac* controller = self->owner->actor->controller)
            controller->OnUnlinked();
    }
}
