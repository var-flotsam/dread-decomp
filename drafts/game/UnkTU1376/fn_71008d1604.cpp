// CDoorLifeComponent's unit (tools/tu_map.py unit 1376; the name is a hint): fn_71008d1604 runs the item life
// setup, hands the game manager's blackboard to the door (vtable slot 0x140), attaches each of its two linked
// entities' shield or demolition block list to the door, then refreshes its state. Names and layouts are
// placeholders.
#include "base/UnkRefHandle.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU437.h"
#include "game/navmesh/SNavMeshPath.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkDoorCentral_A4_b76;
struct UnkItemLife_l8e0f30;
void fn_71008e0f30(UnkItemLife_l8e0f30* self);

struct UnkDoor_b0251;
void fn_71008d1944(UnkDoor_b0251* self, bool on);
struct UnkDoorLife_l8d1604;
void fn_71008d1b4c(UnkDoorLife_l8d1604* self, void* data);

// A shield or demolition block life component: each keeps a list that the door joins.
struct UnkDoorShieldLife_l8d1604 : UnkReflObject {
    u8 pad_0x8[0x408 - 0x8];
    UnkLinkList* list;  // 0x408 (its address is the list)
};
struct UnkDemolitionBlockLife_l8d1604 : UnkReflObject {
    u8 pad_0x8[0x3e8 - 0x8];
    UnkLinkList* list;  // 0x3e8 (its address is the list)
};

struct UnkLinkedEntity_l8d1604 {
    u8 pad_0x0[0x1b8];
    UnkReflObject* life;  // 0x1b8
};

struct UnkLinkHandle_l8d1604 {
    u8 pad_0x0[0x10];
    UnkLinkedEntity_l8d1604* entity;  // 0x10
};

// A lazily resolved link to another entity: the key is resolved to a handle on first use.
struct UnkEntityLink_l8d1604 {
    u8 pad_0x0[0x8];
    u8 key[0x28];               // 0x8
    u32 state;                  // 0x30
    u32 pad_0x34;
    base::UnkRefHandle handle;  // 0x38: an UnkLinkHandle_l8d1604

    void Resolve() {
        if (state == 1) {
            {
                base::UnkRefPtrMember ref = fn_7100353be8(key);
                fn_710031d208(&handle, reinterpret_cast<const base::UnkRefHandle*>(&ref));
            }
            state = 2;
        }
    }
    // The linked entity's life component, null when there is none.
    UnkReflObject* Life() {
        Resolve();
        UnkLinkHandle_l8d1604* h = reinterpret_cast<UnkLinkHandle_l8d1604*>(handle.ptr);
        if (!h || !h->entity)
            return nullptr;
        return h->entity->life;
    }
};
static_assert(sizeof(UnkEntityLink_l8d1604) == 0x40, "layout");

struct UnkDoorState_l8d1604 {
    u8 pad_0x0[0x10];
    u32 id;
};

// The door's membership in another entity's list.
struct UnkJoinedList_l8d1604 {
    UnkLinkList* list;
    u8 pad_0x8[0x18 - 0x8];

    // Leaves the list it is in, if any, and joins `other`.
    void Join(UnkLinkList* other) {
        if (list) {
            fn_71002aeb9c(list, &list);
            list = nullptr;
        }
        list = other;
        fn_71002af070(other, &list);
    }
};
static_assert(sizeof(UnkJoinedList_l8d1604) == 0x18, "layout");

struct UnkDoorLife_l8d1604 {
    void** vtable;
    u8 pad_0x8[0x388 - 0x8];
    UnkEntityLink_l8d1604 links[2];  // 0x388
    u8 pad_0x408[0x430 - 0x408];
    UnkDoorState_l8d1604* current;  // 0x430: the state machine's current state
    u8 pad_0x438[0xa14 - 0x438];
    u32 field_0xa14;
    u8 pad_0xa18[0xba8 - 0xa18];
    UnkJoinedList_l8d1604 joined[2];  // 0xba8: one per link

    // vtable slot 0x140: takes the blackboard.
    void vf_0x140(CBlackboard* blackboard) {
        reinterpret_cast<void (*)(UnkDoorLife_l8d1604*, CBlackboard*)>(vtable[0x140 / 8])(this, blackboard);
    }
};
static_assert(__builtin_offsetof(UnkDoorLife_l8d1604, current) == 0x430, "layout");
static_assert(__builtin_offsetof(UnkDoorLife_l8d1604, joined) == 0xba8, "layout");

void fn_71008d1604(UnkDoorCentral_A4_b76* door) {
    UnkDoorLife_l8d1604* self = reinterpret_cast<UnkDoorLife_l8d1604*>(door);
    fn_71008e0f30(reinterpret_cast<UnkItemLife_l8e0f30*>(self));
    self->vf_0x140(&g_unk_7101d47310->field_0x968);
    // Each link joins the linked entity's shield list, or else its demolition block list.
    for (u32 i = 0; i < 2; ++i) {
        if (UnkReflObject* life = self->links[i].Life()) {
            if (fn_710009d50c(life->vf_0x0(), fn_71008ddcf0())) {
                self->joined[i].Join(reinterpret_cast<UnkLinkList*>(&static_cast<UnkDoorShieldLife_l8d1604*>(life)->list));
                continue;
            }
        }
        if (UnkReflObject* life = self->links[i].Life()) {
            if (fn_710009d50c(life->vf_0x0(), fn_71008c4688()))
                self->joined[i].Join(reinterpret_cast<UnkLinkList*>(&static_cast<UnkDemolitionBlockLife_l8d1604*>(life)->list));
        }
    }
    fn_71008d1944(reinterpret_cast<UnkDoor_b0251*>(self), self->current && self->current->id == 1);
    fn_71008d1b4c(self, &self->field_0xa14);
}
