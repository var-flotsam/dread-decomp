// CCharClassLavaPoolComponent's unit (tools/tu_map.py; the name from reflection): passes the flag
// to fn_71009025d8 and its inverse to fn_710024fa0c; returns 0. Names are placeholders.
#include "types.h"

struct UnkStruct_710024fa0c;
void fn_710024fa0c(UnkStruct_710024fa0c*, bool);
struct UnkLavaPool_b45a2;
void fn_71009025d8(UnkLavaPool_b45a2* self, bool on);

u32 fn_71008fc70c(UnkLavaPool_b45a2* self, bool on) {
    fn_71009025d8(self, on);
    fn_710024fa0c(reinterpret_cast<UnkStruct_710024fa0c*>(self), !on);
    return 0;
}
