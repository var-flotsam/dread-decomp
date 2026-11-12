#include "game/CFrozenComponent.h"
#include "game/navmesh/SNavMeshPath.h"
#include "types.h"

struct UnkFrozenEvent {
    u64 owner;
    bool active;
};

struct UnkFrozenEvents;
void fn_71007fb7b4(UnkFrozenEvents* events, const UnkFrozenEvent* event);

struct CFrozenComponent {
    u8 unk_0x0[0x30];
    u64 field_0x30;
    u8 unk_0x38[0x2e0 - 0x38];
    u8 events[0x8];  // 0x2e0
};

// Raises the frozen event for every state other than 0 and 5.
void fn_71007fa080(CFrozenComponent* self, u32 state) {
    if (state == 0 || state == 5)
        return;
    UnkFrozenEvent event;
    event.owner = self->field_0x30;
    event.active = true;
    fn_71007fb7b4(reinterpret_cast<UnkFrozenEvents*>(self->events), &event);
}

struct UnkLinkList;
void fn_71002aeb9c(UnkLinkList* list, UnkLinkList** slot);

struct UnkFrozenTarget {
    u8 unk_0x0[0x198];
    u8 links[0x8];  // 0x198
};

struct UnkFrozenOwner {
    u8 unk_0x0[0x1b8];
    UnkFrozenTarget* target;
};

struct UnkVec3Zero {
    f32 x, y, z;
};

struct CFrozenComponent2 {
    u8 unk_0x0[0x30];
    UnkFrozenOwner* owner;
    u8 unk_0x38[0x360 - 0x38];
    UnkLinkList* link;  // 0x360
    u8 unk_0x368[0x39c - 0x368];
    UnkVec3Zero field_0x39c;
};

// Relinks the component into its owner's target list, clears 0x39c and restarts from 0.
void fn_71007f954c(CFrozenComponent2* self) {
    if (UnkFrozenTarget* target = self->owner->target) {
        UnkLinkList** slot = &self->link;
        if (*slot) {
            fn_71002aeb9c(*slot, slot);
            *slot = nullptr;
        }
        UnkLinkList* links = reinterpret_cast<UnkLinkList*>(target->links);
        *slot = links;
        fn_71002af070(links, slot);
    }
    UnkVec3Zero* v = &self->field_0x39c;
    v->z = 0.0f;
    v->x = 0.0f;
    v->y = 0.0f;
    fn_71007f95b8(self, 0.0f);
}
