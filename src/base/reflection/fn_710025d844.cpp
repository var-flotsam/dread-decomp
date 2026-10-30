// Reflection construct functions (class slot 0x28) of two array types with UnkVector's layout
// and allocation flags 0xf: fn_710025d844 and fn_71000e20d0 value-initialise one in place, so the
// object is zero-filled before the member initialisers run (the second zero store at 0x10 and the
// one at 0x20 in the original).

#include <new>

#include "base/UnkVector.h"
#include "types.h"

struct UnkVector_0xf {
    void* data = nullptr;
    u32 count = 0;
    u32 capacity = 0;
    u32 step = 10;
    bool field_0x14 = true;
    u32 flags = 0xf;
    base::UnkAllocator* allocator = fn_7100083990();
};
static_assert(sizeof(UnkVector_0xf) == 0x28, "UnkVector_0xf layout");

void fn_710025d844(void* p) { new (p) UnkVector_0xf(); }

// Identical copies: fn_7100ca96d8, fn_710104eca8.
void fn_7100ca96d8(void* p) { new (p) UnkVector_0xf(); }
void fn_710104eca8(void* p) { new (p) UnkVector_0xf(); }

void fn_71000e20d0(void* p) { new (p) UnkVector_0xf(); }
