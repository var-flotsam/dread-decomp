// behaviortree::CEmmySearchingAction's unit (tools/tu_map.py unit 805): the search's step over its
// open list (a min-heap on cost + estimate) and its start, stop and movement-listener handling.
// The action keeps its search flags from +0x12af1 (stopped, searching, the pending state at +0x12b35)
// and a callback the movement component calls back (+0x12b40). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

struct UnkEmmyAI_b0068 {
    void** vtable;

    const base::reflection::CClass* vf_0x0() {
        return reinterpret_cast<const base::reflection::CClass* (*)(UnkEmmyAI_b0068*)>(vtable[0])(this);
    }
    bool vf_0x7d8() { return reinterpret_cast<bool (*)(UnkEmmyAI_b0068*)>(vtable[0x7d8 / 8])(this); }
    u8* Bytes() { return reinterpret_cast<u8*>(this); }
};

struct UnkEmmyMovement_b0068 {
    void** vtable;
    u8 pad_0x8[0x19d8];
    u64 field_0x19e0;
    u64 field_0x19e8;

    const base::reflection::CClass* vf_0x0() {
        return reinterpret_cast<const base::reflection::CClass* (*)(UnkEmmyMovement_b0068*)>(vtable[0])(this);
    }
};

struct UnkEmmyActor_b0068 {
    u8 pad_0x0[0x1f0];
    UnkEmmyMovement_b0068* movement;  // 0x1f0
    u8 pad_0x1f8[0x48];
    UnkEmmyAI_b0068* ai;  // 0x240
};

struct UnkEmmySearchingAction_b0068;

// A callback bound to the action: the object and the function called with it.
struct UnkEmmySearchCallback_b0068 {
    UnkEmmySearchingAction_b0068* self;
    void (*fn)(UnkEmmySearchingAction_b0068*);
};

struct UnkEmmySearchState_b0068 {
    bool active;  // 0x12af2
    u8 pad_0x1[5];
    u8 field_0x12af8[0x10];
    u32 field_0x12b08;
    u8 pad_0x1a[0x24];
    s32 result;  // 0x12b30 (unaligned)
    u8 pad_0x42;
    bool pending;  // 0x12b35
} __attribute__((packed));

struct UnkEmmySearchingAction_b0068 {
    u8 pad_0x0[0x18];
    UnkEmmyActor_b0068* actor;  // 0x18
    u8 pad_0x20[0x48];
    u8 field_0x68[0x12a89];
    bool field_0x12af1;
    UnkEmmySearchState_b0068 state;  // 0x12af2
};
static_assert(__builtin_offsetof(UnkEmmySearchingAction_b0068, state.pending) == 0x12b35, "layout");

static inline UnkEmmyAI_b0068* EmmyAI(UnkEmmySearchingAction_b0068* self) {
    UnkEmmyAI_b0068* ai = self->actor->ai;
    if (ai && fn_710009d50c(ai->vf_0x0(), fn_710053b934()))
        return ai;
    return nullptr;
}

bool fn_7100549ec8(void* self);
void fn_710051c018(UnkEmmySearchingAction_b0068* self, bool* a, bool* b);
void fn_71002adc8c(void* self);

// Stops the search: marks it stopped when the AI is busy (+0x29f24, or vf_0x7d8 says so), else
// finishes it (fn_710051c018 unless fn_7100549ec8 holds with +0x14b5a clear) and resets +0x68.
void fn_710051857c(UnkEmmySearchingAction_b0068* self) {
    bool* flags = &self->field_0x12af1;
    UnkEmmyAI_b0068* ai = self->actor->ai;
    if (ai && fn_710009d50c(ai->vf_0x0(), fn_710053b934())) {
        if (ai->Bytes()[0x29f24] || ((ai->Bytes()[0x29f25] || ai->vf_0x7d8()) && !ai->vf_0x7d8())) {
            flags[0] = true;
            return;
        }
        if (!fn_7100549ec8(ai->Bytes() + 0x14b40) || ai->Bytes()[0x14b5a]) {
            bool a = false;
            bool b = false;
            fn_710051c018(self, &a, &b);
        }
        fn_71002adc8c(self->field_0x68);
    }
    flags[1] = false;
}

bool fn_7100519cec(UnkEmmySearchingAction_b0068* self, bool flag);
bool fn_710051a00c(UnkEmmySearchingAction_b0068* self);
void fn_710051aaec(UnkEmmySearchingAction_b0068* self);
s32 fn_71005193a0(UnkEmmySearchingAction_b0068* self, bool a, bool b);
bool fn_71005472bc(UnkEmmyAI_b0068* ai);
void fn_710051d694(UnkEmmySearchingAction_b0068* self);
void fn_710051d698(UnkEmmySearchingAction_b0068* self);
void fn_710051d60c(UnkEmmySearchingAction_b0068* self);
void fn_7100556c00(UnkEmmyAI_b0068* ai, bool pending, const UnkEmmySearchCallback_b0068* a,
                   const UnkEmmySearchCallback_b0068* b, u32 c, void* data, const UnkEmmySearchCallback_b0068& d);

// Starts a search when one can run (fn_7100519cec, or the AI's fn_71005472bc and a second try),
// stopping it otherwise; a search that ends at once (result 0 or 3) is handed to the AI with the
// action's callbacks.
void fn_7100519218(UnkEmmySearchingAction_b0068* self, bool flag) {
    if (!fn_7100519cec(self, false)) {
        UnkEmmyAI_b0068* ai = EmmyAI(self);
        if (!ai || !fn_71005472bc(ai) || !fn_7100519cec(self, true)) {
            fn_710051857c(self);
            return;
        }
    }
    UnkEmmySearchState_b0068* state = &self->state;
    if (fn_710051a00c(self))
        fn_710051aaec(self);
    if (!state->field_0x12b08) {
        fn_710051857c(self);
        return;
    }
    state->active = true;
    s32 result = fn_71005193a0(self, !flag, false);
    state->result = result;
    state->pending = true;
    if (result == 3 || result == 0) {
        if (UnkEmmyAI_b0068* ai = EmmyAI(self)) {
            if (state->field_0x12b08) {
                UnkEmmySearchCallback_b0068 a = {self, fn_710051d694};
                UnkEmmySearchCallback_b0068 b = {self, fn_710051d698};
                fn_7100556c00(ai, state->pending, &a, &b, 0, state->field_0x12af8,
                              UnkEmmySearchCallback_b0068{self, fn_710051d60c});
                state->pending = false;
            }
        }
    }
}

// The movement component's state change (from, to): going from 4 to 1 restarts a stopped search
// and clears the movement's listener when it is a CEnemyMovement.
void fn_710051d56c(UnkEmmySearchingAction_b0068* self, s32 from, s32 to) {
    if (from == to)
        return;
    if (from != 4 || to != 1)
        return;
    if (self->field_0x12af1) {
        self->field_0x12af1 = false;
        fn_7100519218(self, true);
    }
    UnkEmmyMovement_b0068* movement = self->actor->movement;
    if (movement && fn_710009d50c(movement->vf_0x0(), fn_71010c74e0())) {
        movement->field_0x19e8 = 0;
        movement->field_0x19e0 = 0;
    }
}

// The search's nodes (the list fn_710051cdd8 fills, CEmmySearchingAction_Collect_A3_b70.cpp).
struct UnkSearchNode_b0068 {
    void* vtable;
    void* item;   // 0x8
    void* owner;  // 0x10
    f32 cost;      // 0x18
    f32 estimate;  // 0x1c
    UnkSearchNode_b0068* next;  // 0x20

    f32 Total() const { return cost + estimate; }
};

// A fixed list of up to 100 nodes; the pointers follow the count unaligned.
struct UnkSearchNodeList_b0068 {
    u32 count;
    UnkSearchNode_b0068* nodes[100];

    UnkSearchNode_b0068* Back() { return *(nodes + count - 1); }
    bool IsFull() const { return count == 100; }
    void PushBack(UnkSearchNode_b0068* node) {
        nodes[count] = node;
        ++count;
    }
} __attribute__((packed, aligned(4)));

struct UnkSearchNodeChain_b0068 {
    void* vtable;
    u32 count;
    u32 pad_0xc;
    UnkSearchNode_b0068* head;
    UnkSearchNode_b0068* tail;
    base::UnkAllocator* allocator;
    u32 tag;
};

struct UnkSearch_b0068 {
    UnkSearchNodeList_b0068 open;     // 0x0, a min-heap on the total
    UnkSearchNodeList_b0068 visited;  // 0x324
    u8 pad_0x648[0xc84];
    UnkSearchNodeList_b0068 path;  // 0x12cc
    u8 pad_0x15f0[0x15f0 - 0x12cc - sizeof(UnkSearchNodeList_b0068)];
    s32 state;  // 0x15f0: 2 while searching, 3 done, 4 failed
    u8 pad_0x15f4[0x14];
    UnkSearchNode_b0068* goal;  // 0x1608
    u8 pad_0x1610[0x18];
    bool flag;  // 0x1628
    u8 pad_0x1629[3];
    u32 mode;  // 0x162c
};
static_assert(__builtin_offsetof(UnkSearch_b0068, mode) == 0x162c, "layout");

extern void* g_unk_7101a47f88[];

struct UnkCollectOwner_A3_b70;
struct UnkCollectList_A3_b70;
void fn_710051cdd8(UnkCollectOwner_A3_b70* self, bool flag, u32 mode, UnkCollectList_A3_b70* list);
void fn_710051cc5c(UnkSearch_b0068* self);  // pops the open list's best node
void fn_710051cf50(UnkSearch_b0068* self, UnkSearchNode_b0068* node);

// One step of the search: takes the open list's best node, ending the search when it costs more than
// the path's last node, adding it to the path when it reaches the goal's item, or else visiting it and
// opening its neighbours (fn_710051cdd8, then fn_710051cf50 for each). True while still searching.
bool fn_710051c9b0(UnkSearch_b0068* self) {
    if (self->state == 2) {
        UnkSearchNode_b0068* top;
        if (self->open.count == 0 ||
            ((top = self->open.nodes[0]), self->path.count != 0 && top->Total() > self->path.Back()->Total())) {
            self->state = self->path.count == 0 ? 4 : 3;
        } else {
            UnkSearchNode_b0068* goal = self->goal;
            if (top->item == goal->item) {
                self->path.PushBack(top);
                fn_710051cc5c(self);
            } else {
                fn_710051cc5c(self);
                if (self->visited.IsFull()) {
                    self->state = 4;
                } else {
                    self->visited.PushBack(top);
                    UnkSearchNodeChain_b0068 list;
                    list.vtable = g_unk_7101a47f88 + 2;
                    list.count = 0;
                    list.head = nullptr;
                    list.tail = nullptr;
                    list.allocator = fn_7100083990();
                    list.tag = 0x13;
                    fn_710051cdd8(reinterpret_cast<UnkCollectOwner_A3_b70*>(top), self->flag, self->mode,
                                  reinterpret_cast<UnkCollectList_A3_b70*>(&list));
                    for (UnkSearchNode_b0068* node = list.head; node; node = node->next)
                        fn_710051cf50(self, node);
                    list.vtable = g_unk_7101a47f88 + 2;
                    for (UnkSearchNode_b0068* node = list.head; node;) {
                        UnkSearchNode_b0068* next = node->next;
                        list.allocator->vf_0x48(node);
                        node = next;
                    }
                }
            }
        }
    }
    return self->state == 2;
}
