// CQuetzoaXAIComponent unit (tools/tu_map.py unit 962): the constructor (fn_7100640918). Base constructor
// (fn_7100649b44), vtable, the member at 0x14b28 (fn_710063d620), the container at 0x14b88 (base vtable,
// empty array with step 10 and flags 0xf, final vtable, allocator), two cleared runs and the object with
// vtable g_unk_7101a22d48 at 0x14c08, then the post-construction virtual call (slot 3). The same container
// as fn_7100a1482c (src/game/CAutsharpSpawnPointComponent/CAutsharpSpawnPointComponent.cpp). Names and
// layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

void fn_7100649b44(void* self);
void fn_710063d620(void* member);
extern void* g_unk_7101a58f80[];
extern void* g_unk_7101a20998[];
extern void* g_unk_7101a20950[];
extern void* g_unk_7101a22d48[];

struct UnkQuetzoaXAIComponent {
    void** vtable;
    u8 pad_0x8[0x14b28 - 8];
    u8 field_0x14b28[0x60];
    void** field_0x14b88;
    u64 field_0x14b90;
    u64 field_0x14b98;
    u32 field_0x14ba0;
    bool field_0x14ba4;
    u32 field_0x14ba8;
    base::UnkAllocator* field_0x14bb0;
    u32 field_0x14bb8;
    u64 field_0x14bc0;
    u8 pad_0x14bc8[8];
    u64 field_0x14bd0[6];
    u8 pad_0x14c00[8];
    void** field_0x14c08;
    u64 field_0x14c10[3];
};

void fn_7100640918(UnkQuetzoaXAIComponent* self) {
    fn_7100649b44(self);
    self->vtable = g_unk_7101a58f80 + 2;
    fn_710063d620(self->field_0x14b28);
    self->field_0x14b88 = g_unk_7101a20998 + 2;
    self->field_0x14b90 = 0;
    self->field_0x14b98 = 0;
    self->field_0x14ba0 = 10;
    self->field_0x14ba4 = true;
    self->field_0x14ba8 = 0xf;
    self->field_0x14bb0 = fn_7100083990();
    self->field_0x14bb8 = 0;
    self->field_0x14b88 = g_unk_7101a20950 + 2;
    self->field_0x14bc0 = 0;
    __builtin_memset(self->field_0x14bd0, 0, sizeof(self->field_0x14bd0));
    self->field_0x14c08 = g_unk_7101a22d48 + 2;
    __builtin_memset(self->field_0x14c10, 0, sizeof(self->field_0x14c10));
    reinterpret_cast<void (*)(UnkQuetzoaXAIComponent*)>(self->vtable[3])(self);
}
