// CThermalDeviceComponent's unit: reflected collection slot 0x128, erase the 0x48-byte entry an index
// iterator names through fn_7100a95ff0. The index goes through a 64-bit byte offset from the collection and a pointer
// difference. Names are placeholders.
#include "types.h"

struct UnkCallbackSetVector;

struct UnkEntry48_A6_b67 {
    u8 unk_0x0[0x48];
};

void fn_7100a95ff0(UnkCallbackSetVector* v, u32 index);

void fn_7100a950f8(UnkCallbackSetVector* self, const u32* it) {
    UnkEntry48_A6_b67* base = reinterpret_cast<UnkEntry48_A6_b67*>(self);
    u64 offset = (u64)*it * sizeof(UnkEntry48_A6_b67);
    UnkEntry48_A6_b67* p = reinterpret_cast<UnkEntry48_A6_b67*>(reinterpret_cast<char*>(base) + offset);
    fn_7100a95ff0(self, (u32)(p - base));
}
