// From unit 2216 of tools/tu_map.py (0x7100ca6844-0x7100cab490), named after the
// CBTObserverComponent reflection data it registers: the constructor of an observer object
// (fn_7100ca6844, called by fn_7100de4b98) with a pooled hash map, two pairs of parallel arrays and
// three callbacks on itself. Names and layouts are placeholders.
#include "base/UnkBlockFreeList.h"
#include "base/UnkChainedMap.h"
#include "base/UnkCloneHelpers.h"
#include "base/UnkPairArrays.h"
#include "base/UnkVector.h"
#include "types.h"

// A callback on the observer (fn_7100caacd0, fn_7100caad50, fn_7100caadb0).
struct UnkBTObserverCallback {
    u64 field_0x0;
    void* object;
    void (*function)();
};

struct UnkBTObserver {
    char* vtable;
    UnkChainedMap map;         // 0x8, its nodes from the pool below
    base::UnkBlockPool nodes;  // 0x50
    base::UnkPairArrays field_0x90;
    u32 field_0xb8;
    u64 field_0xc0;
    u32 field_0xc8;
    u64 field_0xd0;
    u32 field_0xd8;
    bool field_0xdc;
    UnkBTObserverCallback callbacks[3];  // 0xe0
    base::UnkPairArrays field_0x128;
};
static_assert(sizeof(UnkBTObserver) == 0x150, "UnkBTObserver placeholder layout");

extern char g_unk_7101aa52b0[];  // the observer's vtable
// Sizes the map: its buckets and its pool of nodes.
void fn_7100caab7c(UnkChainedMap* map, u32 buckets, u32 nodes);
// Reserves room for `capacity` pairs.
void fn_7100ca9158(base::UnkPairArrays* pairs, u32 capacity);
void fn_7100caacd0();
void fn_7100caad50();
void fn_7100caadb0();

void fn_7100ca6844(UnkBTObserver* self) {
    self->vtable = g_unk_7101aa52b0 + 0x10;
    new (&self->map) UnkChainedMap();
    self->nodes.Init();
    self->nodes.field_0x38 = 0;
    self->nodes.field_0x3c = false;
    fn_7100caab7c(&self->map, 0xb, 100);
    self->field_0x90.keys = nullptr;
    self->field_0x90.values = nullptr;
    self->field_0x90.count = 0;
    self->field_0x90.capacity = 0;
    self->field_0x90.step = 10;
    self->field_0x90.flags = 0xe;
    self->field_0x90.allocator = fn_7100083990();
    self->field_0xb8 = 1;
    self->field_0xc0 = 0;
    self->field_0xc8 = 0;
    self->field_0xd0 = 0;
    self->field_0xd8 = 1;
    self->field_0xdc = false;
    self->callbacks[0].field_0x0 = 0;
    self->callbacks[0].object = self;
    self->callbacks[0].function = fn_7100caacd0;
    self->callbacks[1].field_0x0 = 0;
    self->callbacks[1].object = self;
    self->callbacks[1].function = fn_7100caad50;
    self->callbacks[2].field_0x0 = 0;
    self->callbacks[2].object = self;
    self->callbacks[2].function = fn_7100caadb0;
    self->field_0x128.keys = nullptr;
    self->field_0x128.values = nullptr;
    self->field_0x128.count = 0;
    self->field_0x128.capacity = 0;
    self->field_0x128.step = 10;
    self->field_0x128.flags = 5;
    self->field_0x128.allocator = fn_7100083990();
    fn_7100ca9158(&self->field_0x128, 0x50);
}
