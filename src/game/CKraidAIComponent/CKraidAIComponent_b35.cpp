// A function in the CKraidAIComponent unit: when fn_71003ad418 holds for the object, returns
// fn_71005fd134 on the last two arguments (its first parameter is unused: the original leaves x0 as the
// test left it), else false. Names and layouts are placeholders.
#include "types.h"

struct UnkKraidAI;
bool fn_71003ad418(UnkKraidAI* self);
bool fn_71005fd134(void* unused, void* a, void* b);

bool fn_71005fd258(UnkKraidAI* self, void* unused, void* a, void* b) {
    if (!fn_71003ad418(self))
        return false;
    void* dead;  // the callee ignores it
    return fn_71005fd134(dead, a, b);
}
