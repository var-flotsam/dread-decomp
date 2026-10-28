// Copy of fn_71000853b8 (src/base/fn_71000853b8.cpp) for sets whose nodes link at 0x18, naming its own callees and data
// (the comments are adapted from the original's): fn_710008c85c. Clears a chained set of trivially destroyed keys
// (base::refl_ChainedSetClear), drops the flag before it and finishes with fn_710008c9d8.
#include "base/UnkHashMap.h"

char** fn_710008d0c4(char* set, u32 index);
void fn_710008c9d8(char* set);
struct UnkTag_710008c85c;
template <>
struct base::UnkChainedSetTraits<UnkTag_710008c85c> {
    static char** bucket(char* self, u32 i) { return fn_710008d0c4(self, i); }
};
struct UnkTrivialKey {};
void fn_710008c85c(char* self) {
    base::refl_ChainedSetClear<UnkTag_710008c85c, UnkTrivialKey, 0x18>(self + 8);
    *reinterpret_cast<bool*>(self) = false;
    fn_710008c9d8(self + 8);
}
