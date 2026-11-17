// CScorpiusAIComponent unit: resets the component: the base reset, both node lists emptied, the two state
// pairs restarted, and the owner's path list re-attached. Names and layouts are placeholders.
#include "game/navmesh/SNavMeshPath.h"
#include "types.h"

struct UnkObject_71003ad278;
void fn_71003ad278(UnkObject_71003ad278* self);

struct UnkNode_l691f88 {
    u8 pad_0x0[0x8];
    UnkNode_l691f88* next;  // 0x8
};

struct UnkNodeAllocator_l691f88 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void Free(UnkNode_l691f88* node);  // 0x48
};

struct UnkNodeList_l691f88 {
    u32 count;                          // 0x0
    UnkNode_l691f88* head;              // 0x8
    UnkNode_l691f88* tail;              // 0x10
    UnkNodeAllocator_l691f88* allocator;  // 0x18

    void Clear() {
        for (UnkNode_l691f88* node = head; node;) {
            UnkNode_l691f88* next = node->next;
            allocator->Free(node);
            node = next;
        }
        count = 0;
        head = nullptr;
        tail = nullptr;
    }
};

struct UnkOwner_l691f88 {
    u8 pad_0x0[0x58c];
    u32 field_0x58c;
};

// The attack state block at 0x149a0.
struct UnkAttackState_l691f88 {
    UnkOwner_l691f88* owner;  // 0x0
    u8 pad_0x8[0x28c - 0x8];
    f32 weight;     // 0x28c
    s32 state;      // 0x290
    u8 pad_0x294[0x2a8 - 0x294];
    u8 state_data[0x8];       // 0x2a8
    UnkNodeList_l691f88 nodes;  // 0x2b0
    u8 pad_0x2d0[0x318 - 0x2d0];
    u32 field_0x318;
    u8 pad_0x31c[0x330 - 0x31c];
    bool field_0x330;
    u8 pad_0x331[0x338 - 0x331];
};

// The path-following block at 0x14cd8.
struct UnkPathState_l691f88 {
    UnkLinkList* path_slot;  // 0x0
    u8 pad_0x8[0x20 - 0x8];
    u16 field_0x20;
    u8 pad_0x22[0x28 - 0x22];
    u32 field_0x28;
    u32 field_0x2c;
    s32 state;                  // 0x30
    u8 pad_0x34[0x38 - 0x34];
    u8 state_data[0x8];         // 0x38
    UnkNodeList_l691f88 nodes;  // 0x40

    // Moves the slot onto the given path list.
    void Attach(UnkLinkList* list) {
        if (path_slot) {
            fn_71002aeb9c(path_slot, &path_slot);
            path_slot = nullptr;
        }
        path_slot = list;
        fn_71002af070(list, &path_slot);
    }
};

struct UnkScorpiusAI_l691f88 {
    u8 pad_0x0[0x1828];
    UnkLinkList* path;  // 0x1828: an object, attached by address
    u8 pad_0x1830[0x149a0 - 0x1830];
    UnkAttackState_l691f88 attack;  // 0x149a0
    UnkPathState_l691f88 follow;    // 0x14cd8
};
static_assert(__builtin_offsetof(UnkScorpiusAI_l691f88, follow) == 0x14cd8, "");

void fn_71006920f8(UnkScorpiusAI_l691f88* self, s32 mode, s32* state, void* data, s32 flags);

void fn_7100691f88(UnkScorpiusAI_l691f88* self) {
    UnkPathState_l691f88* follow = &self->follow;
    UnkAttackState_l691f88& attack = self->attack;
    fn_71003ad278(reinterpret_cast<UnkObject_71003ad278*>(self));
    attack.nodes.Clear();
    follow->nodes.Clear();
    attack.field_0x318 = 0;
    fn_71006920f8(self, 1, &attack.state, attack.state_data, 0);
    f32 weight = attack.state == 3 ? 1.0f : attack.state == 2 ? 0.5f : 0.0f;
    attack.weight = weight > 0.0f ? weight : 0.0f;
    fn_71006920f8(self, 1, &follow->state, follow->state_data, 0);
    follow->field_0x20 = 0;
    attack.field_0x330 = false;
    follow->field_0x28 = attack.owner->field_0x58c;
    follow->field_0x2c = 0;
    follow->Attach(reinterpret_cast<UnkLinkList*>(&self->path));
}
