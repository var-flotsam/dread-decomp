#include "base/UnkHashMap.h"
char** fn_7100085afc(char* set, u32 index);
void fn_7100085478(char* set);
struct UnkTag_71000853b8;
template <> struct base::UnkChainedSetTraits<UnkTag_71000853b8> {
    static char** bucket(char* self, u32 i) { return fn_7100085afc(self, i); }
};
struct UnkTrivialKey {};
void fn_71000853b8(char* self) {
    base::refl_ChainedSetClear<UnkTag_71000853b8, UnkTrivialKey, 0x10>(self + 8);
    *reinterpret_cast<bool*>(self) = false;
    fn_7100085478(self + 8);
}
