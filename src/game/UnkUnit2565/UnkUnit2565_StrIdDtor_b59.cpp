// Unit 2565 (tools/tu_map.py): the destructor of a secondary base (vtable g_unk_7101aa9d38 at +0x18)
// that holds a string id at +0 and calls no base destructor. Names are placeholders.
#include "base/UnkDestructors.h"

extern void* g_unk_7101aa9d38[];

void fn_7100e7c4fc(char* self) {
    *reinterpret_cast<char**>(self + 0x18) = reinterpret_cast<char*>(g_unk_7101aa9d38) + 0x10;
    base::UnkStrIdAt<0>::Release(self);
}
