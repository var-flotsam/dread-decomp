// CShockWavePoolComponent unit (tools/tu_map.py): near copy of fn_71009f3b68 (CShockWaveComponent_b47.cpp),
// the reset after the component base's (fn_71010c4228): the member at 0x40 is assigned a freshly constructed
// one (fn_71009f5730, fn_71009f5d50), the node list at 0x278 is freed and emptied, and the fields after it are
// cleared. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/CComponent.h"
#include "game/CSpitclawkAIComponent.h"
#include "types.h"

struct UnkParent;
void fn_71009f5730(char* self);
struct UnkShockWavePoolMember;

struct UnkShockWavePoolMember {
    u8 unk_0x0[0x228];
    ~UnkShockWavePoolMember() { fn_71006c9404(this); }
};

// A freshly constructed member (fn_71009f5730).
struct UnkShockWavePoolFreshB48 : UnkShockWavePoolMember {
    UnkShockWavePoolFreshB48() { fn_71009f5730(reinterpret_cast<char*>(this)); }
};
void fn_71009f5d50(UnkShockWavePoolMember* self, const UnkShockWavePoolMember* other);  // assignment

struct UnkShockWavePoolNodeB48 {
    u64 field_0x0;
    UnkShockWavePoolNodeB48* next;
};

struct UnkShockWavePoolResetB48 {
    void** vtable;
    u8 pad_0x8[0x38];
    UnkShockWavePoolMember field_0x40;
    void** field_0x268;
    u32 field_0x270;
    UnkShockWavePoolNodeB48* field_0x278;
    UnkShockWavePoolNodeB48* field_0x280;
    base::UnkAllocator* field_0x288;
    u32 field_0x290;
    u32 pad_0x294;
    u8 field_0x298[0xf];
    u8 pad_0x2a7[0x71];
    void* field_0x318;
    u32 field_0x320;
};

void fn_71009f5cc0(UnkShockWavePoolResetB48* self) {
    fn_71010c4228(reinterpret_cast<UnkParent*>(self));
    {
        UnkShockWavePoolFreshB48 fresh;
        fn_71009f5d50(&self->field_0x40, &fresh);
    }
    for (UnkShockWavePoolNodeB48* n = self->field_0x278; n;) {
        UnkShockWavePoolNodeB48* next = n->next;
        self->field_0x288->vf_0x48(n);
        n = next;
    }
    self->field_0x270 = 0;
    __builtin_memset(&self->field_0x278, 0, 0x10);
    self->field_0x318 = 0;
    self->field_0x320 = 0;
    __builtin_memset(&self->field_0x298, 0, 0xf);
}
