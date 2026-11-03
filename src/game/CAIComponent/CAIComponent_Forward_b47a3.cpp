// CAIComponent unit (tools/tu_map.py): forwards its arguments, without the second, to the virtual at
// slot 0x568 of the object. Names and types are placeholders.
#include "types.h"

struct UnkAIForwardTarget_b47a3;
typedef void (*UnkAIForwardFn_b47a3)(UnkAIForwardTarget_b47a3*, u32, u32, void*, void*, void*, u32, u32);

void fn_710110f3a0(UnkAIForwardTarget_b47a3* self, void*, u32 a, u32 b, void* c, void* d, void* e, u32 f, u32 g) {
    UnkAIForwardFn_b47a3 fn = *reinterpret_cast<UnkAIForwardFn_b47a3*>(*reinterpret_cast<char**>(self) + 0x568);
    fn(self, a, b, c, d, e, f, g);
}
