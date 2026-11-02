// The reflected End of an array of 0x30-byte records: an iterator at the element count. Near copy
// of fn_71000d4c68 (src/game_s7/fn_71000d2e54.cpp).
#include "base/UnkCollectionIter.h"
#include "base/UnkVector.h"
#include "types.h"

struct UnkRecord_7100671864 {
    u64 field_0x0[6];
};

using UnkRecordIter_7100671864 = base::UnkCollectionIter<UnkRecord_7100671864>;

void fn_7100671864(const base::UnkVector<UnkRecord_7100671864>* v, UnkRecordIter_7100671864* out) {
    *out = UnkRecordIter_7100671864(v->count, v->data + v->count);
}

// The constructor (fn_71006707e8): the base constructor fn_7100416db8, the vtable, the array at
// +0x518 (capacity 10, growth 15, default allocator), the member at +0x540 (fn_7100669dec), then
// the post-construction virtual call (slot 3). Names and layouts are placeholders.

// Declared as src/game/CCharClassYamplotXAIComponent/CCharClassYamplotXAIComponent.cpp does.
struct UnkObject_7100416db8;
void fn_7100416db8(UnkObject_7100416db8* self);
void fn_7100669dec(void* self);
extern void* g_unk_7101a5db38[];

struct UnkRodotukDef {
    void** vtable;
    u8 pad_0x8[0x510];
    u64 field_0x518;
    u64 field_0x520;
    u32 field_0x528;
    bool field_0x52c;
    u32 field_0x530;
    base::UnkAllocator* field_0x538;
    u8 field_0x540[0x10];
};

void fn_71006707e8(UnkRodotukDef* self) {
    fn_7100416db8(reinterpret_cast<UnkObject_7100416db8*>(self));
    self->vtable = g_unk_7101a5db38 + 2;
    self->field_0x518 = 0;
    self->field_0x520 = 0;
    self->field_0x528 = 10;
    self->field_0x52c = true;
    self->field_0x530 = 15;
    self->field_0x538 = fn_7100083990();
    fn_7100669dec(self->field_0x540);
    reinterpret_cast<void (*)(UnkRodotukDef*)>(self->vtable[3])(self);
}
