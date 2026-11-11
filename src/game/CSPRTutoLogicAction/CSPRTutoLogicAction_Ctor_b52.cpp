// CSPRTutoLogicAction's unit (tools/tu_map.py): its two identical constructors, on the logic-action
// base (fn_7100a55aa4): the vtable, two callback sets at +0x10 and +0x50 (each built through its base:
// a vtable, an empty array with allocator flags 0xf, a cleared word; then its own vtable and a cleared
// owner), an empty array at +0x90, a block of flags and values, then a delegate at +0xc8 bound to the
// object and fn_7100a54184. Names and layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "game/CTriggerLogicAction.h"
#include "types.h"

// The base constructor, as src/game/CChangeSetupLogicAction/CChangeSetupLogicAction_b35.cpp declares it.
struct UnkChangeSetupLogicActionDef;
void fn_7100a54184();
extern char g_unk_7101a8f458[];
extern void* g_unk_7101a20998[];  // the callback set's base vtable
extern void* g_unk_7101a20950[];  // the callback set's vtable

struct UnkSPRTutoCallable_b52;

struct UnkSPRTutoCallbackSet_b52 {
    void* vtable;
    base::UnkVector<UnkSPRTutoCallable_b52*> items;
    u32 field_0x30;
    void* owner;

    __attribute__((always_inline)) void Init() {
        vtable = g_unk_7101a20998 + 2;
        new (&items) base::UnkVector<UnkSPRTutoCallable_b52*>(0xf);
        field_0x30 = 0;
        vtable = g_unk_7101a20950 + 2;
        owner = nullptr;
    }
};

struct UnkSPRTutoLogicAction_b52 {
    void* vtable;
    u8 unk_0x8[0x8];
    UnkSPRTutoCallbackSet_b52 first;   // 0x10
    UnkSPRTutoCallbackSet_b52 second;  // 0x50
    base::UnkVector<void*> items;      // 0x90
    bool field_0xb8;
    bool field_0xb9;
    bool field_0xba;
    s32 field_0xbc;
    bool field_0xc0;
    bool field_0xc1;
    bool field_0xc2;
    bool field_0xc3;
    u32 field_0xc4;
    void* field_0xc8;
    void* target;          // 0xd0
    void (*method)();      // 0xd8
    u64 adjust;            // 0xe0
    void* field_0xe8;
};
static_assert(__builtin_offsetof(UnkSPRTutoLogicAction_b52, field_0xe8) == 0xe8, "layout");

__attribute__((always_inline)) static inline void Construct_b52(UnkSPRTutoLogicAction_b52* self) {
    fn_7100a55aa4(reinterpret_cast<UnkChangeSetupLogicActionDef*>(self));
    self->vtable = g_unk_7101a8f458 + 0x10;
    self->first.Init();
    self->second.Init();
    new (&self->items) base::UnkVector<void*>(0xf);
    self->field_0xb8 = false;
    self->field_0xb9 = true;
    self->field_0xba = false;
    self->field_0xbc = -1;
    self->field_0xc0 = false;
    self->field_0xc1 = true;
    self->field_0xc2 = false;
    self->field_0xc3 = false;
    self->field_0xc4 = 0x800;
    self->field_0xc8 = nullptr;
    self->target = self;
    self->method = &fn_7100a54184;
    self->adjust = 0;
    self->field_0xe8 = nullptr;
}

void fn_7100a5340c(UnkSPRTutoLogicAction_b52* self) {
    Construct_b52(self);
}

void fn_7100a5407c(UnkSPRTutoLogicAction_b52* self) {
    Construct_b52(self);
}
