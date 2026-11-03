// CShockWaveComponent's constructors, without (fn_71009f39e0) and with (fn_71009f55e0) the
// post-construction virtual call (slot 3): the component base
// constructor fn_71010c4148, the vtable, the member at 0x40 (fn_71009f35b0), the empty node list at
// 0x250 (vtable, default allocator, 0x13) and cleared fields after it. The layout follows
// CShockWaveComponent.cpp. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

struct UnkStateObject;
void fn_71010c4148(UnkStateObject* self);
void fn_71009f35b0(char* self);
extern void* g_unk_7101a8a820[];  // the component's vtable
extern void* g_unk_7101a32680[];  // the node list's vtable

struct UnkShockWaveComponentCtor {
    void** vtable;
    u8 pad_0x8[0x38];
    u8 field_0x40[0x210];
    void** field_0x250;
    u32 field_0x258;
    void* field_0x260;
    void* field_0x268;
    base::UnkAllocator* field_0x270;
    u32 field_0x278;
    void* field_0x280;
    void* field_0x288;
    bool field_0x290;
    bool field_0x291;
    bool field_0x292;
    u8 pad_0x293[0x5];
    u32 field_0x298;
    bool field_0x29c;
    void* field_0x2a0;
};

static inline __attribute__((always_inline)) void InitShockWave(UnkShockWaveComponentCtor* self) {
    fn_71010c4148(reinterpret_cast<UnkStateObject*>(self));
    self->vtable = g_unk_7101a8a820 + 2;
    fn_71009f35b0(reinterpret_cast<char*>(self->field_0x40));
    self->field_0x250 = g_unk_7101a32680 + 2;
    self->field_0x258 = 0;
    self->field_0x260 = nullptr;
    self->field_0x268 = nullptr;
    self->field_0x270 = fn_7100083990();
    self->field_0x278 = 0x13;
    self->field_0x280 = nullptr;
    self->field_0x288 = nullptr;
    self->field_0x290 = false;
    self->field_0x291 = false;
    self->field_0x292 = false;
    self->field_0x298 = 0;
    self->field_0x29c = false;
    self->field_0x2a0 = nullptr;
}

void fn_71009f39e0(UnkShockWaveComponentCtor* self) {
    InitShockWave(self);
}

void fn_71009f55e0(UnkShockWaveComponentCtor* self) {
    InitShockWave(self);
    reinterpret_cast<void (*)(UnkShockWaveComponentCtor*)>(self->vtable[3])(self);
}
