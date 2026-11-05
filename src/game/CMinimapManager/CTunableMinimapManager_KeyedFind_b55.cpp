// Units 2565 and 2566 (tools/tu_map.py): lookups by a u32 key in arrays with a key array alongside
// (values at +0, keys at +8, count at +0x10), the shape of fn_71000dc0c8 (value or null) and
// fn_71000ddafc (erase when present) with u32 keys and 0x248- or 0x118-byte values. Names and
// layouts are placeholders.
#include "types.h"

struct UnkString200Array;  // src/game_aca57c/fn_7100e7b084.cpp
struct UnkString200;
UnkString200* fn_7100e7c0f0(UnkString200Array* self, UnkString200* pos);
void fn_7100ea5d7c(char*, void*);

template <u64 Size>
struct UnkU32KeyedArray_b55 {
    struct Value {
        u8 bytes[Size];
    };
    Value* values;
    u32* keys;
    u32 count;

    Value* End() { return &values[count]; }
    Value* Find(const u32& key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &values[i];
        return End();
    }
};

template <u64 Size>
static inline void* KeyedFind_b55(char* self, const u32* key) {
    auto* a = reinterpret_cast<UnkU32KeyedArray_b55<Size>*>(self);
    auto* v = a->Find(*key);
    return v == a->End() ? nullptr : v;
}

void* fn_7100e7b814(char* self, const u32* key) { return KeyedFind_b55<0x248>(self, key); }
void* fn_7100eadd88(char* self, const u32* key) { return KeyedFind_b55<0x118>(self, key); }

void fn_7100e7b784(char* self, const u32* key) {
    auto* a = reinterpret_cast<UnkU32KeyedArray_b55<0x248>*>(self);
    auto* v = a->Find(*key);
    if (v != a->End())
        fn_7100e7c0f0(reinterpret_cast<UnkString200Array*>(self), reinterpret_cast<UnkString200*>(v));
}
void fn_7100eadcf8(char* self, const u32* key) {
    auto* a = reinterpret_cast<UnkU32KeyedArray_b55<0x118>*>(self);
    auto* v = a->Find(*key);
    if (v != a->End())
        fn_7100ea5d7c(self, v);
}
