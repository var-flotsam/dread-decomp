// CCharClassWaterPoolComponent's unit: reflected collection slot 0x128, erase the 0x48-byte entry an index
// iterator names through fn_710090d944 (copy of fn_7100a950f8's shape). Names are placeholders.
#include "types.h"

struct UnkCallbackSetVector;

struct UnkEntry48_A5_b68 {
    u8 unk_0x0[0x48];
};

void fn_710090d944(UnkCallbackSetVector* v, u32 index);

void fn_710090ca38(UnkCallbackSetVector* self, const u32* it) {
    UnkEntry48_A5_b68* base = reinterpret_cast<UnkEntry48_A5_b68*>(self);
    u64 offset = (u64)*it * sizeof(UnkEntry48_A5_b68);
    UnkEntry48_A5_b68* p = reinterpret_cast<UnkEntry48_A5_b68*>(reinterpret_cast<char*>(base) + offset);
    fn_710090d944(self, (u32)(p - base));
}
