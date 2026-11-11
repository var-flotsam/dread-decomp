// Sets of 8-byte values that CAIManager keeps as members (its +0x4f0 and +0x2b8, built by
// fn_710057a010 and fn_71005ac6e0; tools/tu_map.py units 822 and 867, and unit 695 for a third
// one): a vtable, then a pooled list of {value, next} nodes (base/UnkNodeOwner.h's layout, with
// the node list vtables g_unk_7101a32660/g_unk_7101a32680). Each unit has the destructors and an
// add that skips values already present (the removes, fn_710057a2c4 and fn_71005ac994, are not
// matched yet). Names and layouts are placeholders.
#include "base/UnkNodeOwner.h"
#include "base/UnkSList.h"
#include "base/UnkVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkTU604.h"
#include "types.h"

extern void* g_unk_7101a49720[];  // unit 822's set
extern void* g_unk_7101a501b8[];  // unit 867's set

template <u32 Next>
__attribute__((always_inline)) inline void UnkKeepNode_b43a1(UnkListNode<Next>*) {}

struct UnkNodeSetTag_b43a1;
using UnkNodeSetList_b43a1 = UnkNodeOwner<UnkListNode<8>, UnkKeepNode_b43a1<8>>;

struct UnkNodeSet_b43a1 {
    void** vtable;
    UnkNodeSetList_b43a1 list;

    // The list's count, head and tail as base/UnkSList.h has them (the count reloaded after links
    // are stored).
    base::UnkSList<UnkNodeSetTag_b43a1>* Values() {
        return reinterpret_cast<base::UnkSList<UnkNodeSetTag_b43a1>*>(&list);
    }

    void Destroy(void** set_vtable) {
        vtable = set_vtable + 2;
        list.Destroy(g_unk_7101a32660, g_unk_7101a32680);
    }

    // The value's position, or -1.
    __attribute__((always_inline)) s32 IndexOf(u64 v) {
        u32 i = 0;
        for (auto* n = Values()->head; n; n = n->next, ++i) {
            if (n->value == v)
                return (s32)i;
        }
        return -1;
    }

    __attribute__((always_inline)) void Add(u64 v) {
        if (IndexOf(v) < 0)
            Values()->PushBack(v);
    }
};

// The deleting destructors: the set destroyed, then freed by the replaceable operator delete (as a
// builtin, so the stores into it that nothing reads afterwards are dropped).
template <void** SetVtable>
struct UnkNodeSetDeleted_b43a1 : UnkNodeSet_b43a1 {
    ~UnkNodeSetDeleted_b43a1() { Destroy(SetVtable); }
};

void fn_710057a034(UnkNodeSet_b43a1* self) { self->Destroy(g_unk_7101a49720); }
void fn_710057a138(UnkNodeSetDeleted_b43a1<g_unk_7101a49720>* self) {
    self->~UnkNodeSetDeleted_b43a1();
    __builtin_operator_delete(self);
}
void fn_710057a224(UnkNodeSet_b43a1* self, u64 v) { self->Add(v); }

void fn_71005ac704(UnkNodeSet_b43a1* self) { self->Destroy(g_unk_7101a501b8); }
void fn_71005ac808(UnkNodeSetDeleted_b43a1<g_unk_7101a501b8>* self) {
    self->~UnkNodeSetDeleted_b43a1();
    __builtin_operator_delete(self);
}
void fn_71005ac8f4(UnkNodeSet_b43a1* self, u64 v) { self->Add(v); }

void fn_7100475394(UnkNodeSet_b43a1* self, u64 v) { self->Add(v); }

// Unit 695's set has three more members after the list; its constructor builds the list with
// room for 3 nodes per block (fn_71003fa9fc: block size, flags 0, allocation flags 0x13, the
// default allocator) and clears them.
void fn_71003fa9fc(void* list, u32 block, u32 flags, u32 alloc_flags, base::UnkAllocator* allocator);
extern void* g_unk_7101a394b0[];

// A reference-counted object (count at +8; the virtual at 0x10 disposes of it at zero).
struct UnkRefCounted_b43a1 {
    void** vtable;
    s32 ref_count;

    void Release() {
        if (--ref_count == 0)
            reinterpret_cast<void (*)(UnkRefCounted_b43a1*)>(vtable[2])(this);
    }
};

struct UnkNodeSet695_b43a1 : UnkNodeSet_b43a1 {
    u64 field_0x78;
    u32 field_0x80;
    UnkRefCounted_b43a1* field_0x88;
};

void fn_71004751f8(UnkNodeSet695_b43a1* self) {
    self->vtable = g_unk_7101a394b0 + 2;
    fn_71003fa9fc(&self->list, 3, 0, 0x13, nullptr);
    self->field_0x78 = 0;
    self->field_0x80 = 0;
    self->field_0x88 = nullptr;
}

// Its destructor also drops the reference at +0x88.
void fn_7100475244(UnkNodeSet695_b43a1* self) {
    self->vtable = g_unk_7101a394b0 + 2;
    if (UnkRefCounted_b43a1* p = self->field_0x88)
        p->Release();
    self->list.Destroy(g_unk_7101a32660, g_unk_7101a32680);
}

// Whether the value is in unit 695's set and every other entry is it or null.
bool fn_7100475aac(UnkNodeSet695_b43a1* self, u64 v) {
    bool only = self->IndexOf(v) >= 0;
    for (auto* n = self->Values()->head; n; n = n->next)
        only &= n->value == 0 || n->value == v;
    return only;
}

// Unit 695's set holds actors: the CBossAIComponent (fn_71003ab524's class) of the last one whose
// state at +0x1b8 (+0x70) isn't 2, or null when that actor has none or of another class.
struct UnkActorState_b43a1 {
    u8 pad_0x0[0x70];
    s32 field_0x70;
};
struct UnkActor_b43a1 {
    u8 pad_0x0[0x1b8];
    UnkActorState_b43a1* field_0x1b8;
    u8 pad_0x1c0[0x80];
    base::reflection::UnkReflObject* field_0x240;
};

base::reflection::UnkReflObject* fn_7100475564(UnkNodeSet695_b43a1* self) {
    base::reflection::UnkReflObject* found = nullptr;
    for (auto* n = self->Values()->head; n; n = n->next) {
        auto* actor = reinterpret_cast<UnkActor_b43a1*>(n->value);
        if (actor->field_0x1b8->field_0x70 == 2)
            continue;
        base::reflection::UnkReflObject* c = actor->field_0x240;
        found = c && fn_710009d50c(c->vf_0x0(), fn_71003ab524()) ? c : nullptr;
    }
    return found;
}
