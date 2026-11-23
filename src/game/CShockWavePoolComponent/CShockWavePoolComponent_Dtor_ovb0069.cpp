// CShockWavePoolComponent's destructors (complete and deleting), the shape of CShockWaveComponent's
// (src/game/CShockWaveComponent/CShockWaveComponent.cpp): restore the vtables, free the array at 0x2a8
// and the node list at 0x268, destroy the member at 0x40 (fn_71006c9404) and run the component's base
// destructor (fn_71010c4194). Names and the layout are placeholders.
#include "base/global/CRntVector.h"
#include "game/CSpitclawkAIComponent.h"
#include "game/UnkComponent.h"
#include "game/UnkShockWavePoolMember.h"
#include "game/UnkTU604.h"
#include "types.h"
#include "base/UnkVtableList.h"

struct UnkShockWavePoolMember;

struct UnkShockWavePoolNode_ovb0069 {
    u64 field_0x0;
    UnkShockWavePoolNode_ovb0069* next;
};

extern void* g_unk_7101a8a9b0[];  // the component's vtable

struct UnkShockWavePoolList_ovb0069 : base::UnkVtableList<UnkShockWavePoolNode_ovb0069> {
    ~UnkShockWavePoolList_ovb0069() {
        vtable = &g_unk_7101a32680[2];
        Clear();
    }
};

struct UnkShockWavePoolArray_ovb0069 {
    void* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    ~UnkShockWavePoolArray_ovb0069() {
        if (data) {
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

struct UnkShockWavePoolBase_ovb0069 : UnkBaseComponent {
    ~UnkShockWavePoolBase_ovb0069() { fn_71010c4194(this); }
};

struct CShockWavePoolComponent_ovb0069 : UnkShockWavePoolBase_ovb0069 {
    UnkShockWavePoolMember field_0x40;
    UnkShockWavePoolList_ovb0069 field_0x268;
    u8 field_0x298[0x10];
    UnkShockWavePoolArray_ovb0069 field_0x2a8;

    ~CShockWavePoolComponent_ovb0069() { vtable = &g_unk_7101a8a9b0[2]; }
};
static_assert(__builtin_offsetof(CShockWavePoolComponent_ovb0069, field_0x2a8) == 0x2a8, "layout");

void fn_71009f5b78(CShockWavePoolComponent_ovb0069& self) {
    self.~CShockWavePoolComponent_ovb0069();
}

void fn_71009f5c18(CShockWavePoolComponent_ovb0069& self) {
    delete &self;
}
