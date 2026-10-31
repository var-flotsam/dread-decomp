// CShockWaveComponent's destructors (complete and deleting), the same shape as fn_71010cf1e8 and
// fn_71010cf260 (src/game/CFakePhysicsMovement/fn_71010cf260.cpp): restore the vtables, free the
// node list at 0x250, destroy the member at 0x40 (fn_71003cc474) and run the component's base
// destructor (fn_71010c4194). Names and the layout are placeholders.
#include "base/UnkVector.h"
#include "game/UnkComponent.h"
#include "types.h"

struct UnkShockWaveMember;
void fn_71003cc474(UnkShockWaveMember* self);  // its destructor

struct UnkShockWaveMember {
    u8 unk_0x0[0x210];
    ~UnkShockWaveMember() { fn_71003cc474(this); }
};

struct UnkShockWaveNode {
    u64 field_0x0;
    UnkShockWaveNode* next;
};

extern void* g_unk_7101a8a820[];  // the component's vtable
extern void* g_unk_7101a32680[];  // the node list's vtable

struct UnkShockWaveList {
    void** vtable;
    u32 count;
    UnkShockWaveNode* head;
    UnkShockWaveNode* tail;
    base::UnkAllocator* allocator;

    ~UnkShockWaveList() {
        vtable = &g_unk_7101a32680[2];
        for (UnkShockWaveNode* n = head; n;) {
            UnkShockWaveNode* next = n->next;
            allocator->vf_0x48(n);
            n = next;
        }
        count = 0;
        head = nullptr;
        tail = nullptr;
    }
};

struct UnkShockWaveBase : UnkBaseComponent {
    ~UnkShockWaveBase() { fn_71010c4194(this); }
};

struct CShockWaveComponent : UnkShockWaveBase {
    UnkShockWaveMember field_0x40;
    UnkShockWaveList field_0x250;

    ~CShockWaveComponent() { vtable = &g_unk_7101a8a820[2]; }
};

void fn_71009f3a60(CShockWaveComponent& self) {
    self.~CShockWaveComponent();
}

void fn_71009f3ae0(CShockWaveComponent& self) {
    delete &self;
}
