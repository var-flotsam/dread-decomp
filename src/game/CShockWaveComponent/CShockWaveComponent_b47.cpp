// CShockWaveComponent unit (tools/tu_map.py): the reset after the component base's (fn_71010c4228):
// the member at 0x40 is assigned a freshly constructed one (fn_71009f35b0, fn_71009f3bf8), the node list at
// 0x250 is freed and emptied, and the fields after it are cleared. The layout follows
// CShockWaveComponent_2.cpp. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/CAutsharpAIComponent.h"
#include "game/CComponent.h"
#include "game/CShockWaveComponent.h"
#include "types.h"

struct UnkParent;
struct UnkShockWaveMember;

struct UnkShockWaveMember {
    u8 unk_0x0[0x210];
    ~UnkShockWaveMember() { fn_71003cc474(this); }
};

// A freshly constructed member (fn_71009f35b0).
struct UnkShockWaveFreshB47 : UnkShockWaveMember {
    UnkShockWaveFreshB47() { fn_71009f35b0(reinterpret_cast<char*>(this)); }
};

struct UnkShockWaveNodeB47 {
    u64 field_0x0;
    UnkShockWaveNodeB47* next;
};

struct UnkShockWaveResetB47 {
    void** vtable;
    u8 pad_0x8[0x38];
    UnkShockWaveMember field_0x40;
    void** field_0x250;
    u32 field_0x258;
    UnkShockWaveNodeB47* field_0x260;
    UnkShockWaveNodeB47* field_0x268;
    base::UnkAllocator* field_0x270;
    u32 field_0x278;
    void* field_0x280;
    void* field_0x288;
    bool field_0x290;
    bool field_0x291;
    bool field_0x292;
    u8 pad_0x293[0x5];
    u32 field_0x298;
};

void fn_71009f3b68(UnkShockWaveResetB47* self) {
    fn_71010c4228(reinterpret_cast<UnkParent*>(self));
    {
        UnkShockWaveFreshB47 fresh;
        fn_71009f3bf8(&self->field_0x40, &fresh);
    }
    for (UnkShockWaveNodeB47* n = self->field_0x260; n;) {
        UnkShockWaveNodeB47* next = n->next;
        self->field_0x270->vf_0x48(n);
        n = next;
    }
    self->field_0x258 = 0;
    __builtin_memset(&self->field_0x260, 0, 0x10);
    self->field_0x298 = 0;
    __builtin_memset(&self->field_0x280, 0, 0x13);
}
