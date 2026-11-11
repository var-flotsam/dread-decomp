// Unit 1442 (tools/tu_map.py, no name hint): rebuilds the object's entry list (fn_710096cdec without
// its flag), runs the entry pass over the list at +0xb48 (fn_710096d788, with a cleared u64 and a
// scratch u32, flag set) and finishes with fn_710096b8fc. Names are placeholders.
#include "game/UnkTU1442.h"
#include "types.h"

void fn_710096cdec(char* self, bool flag);
void fn_710096d788(char* self, __attribute__((noescape)) u64* a, char* list, __attribute__((noescape)) u32* out, bool flag);

static inline void RunPass(char* self) {
    u32 out;
    u64 a = 0;
    fn_710096d788(self, &a, self + 0xb48, &out, true);
}

void fn_710096daa8(char* self) {
    fn_710096cdec(self, false);
    RunPass(self);
    fn_710096b8fc(self);
}
