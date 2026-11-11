// Unit 90 of tools/tu_map.py (no class hint): the version writer's (UnkVersionWriter_A1_b71.cpp)
// write of one reflected value: a byte block (length and bytes), a string (as text with its end),
// or the raw bytes of any other type. Names and layouts are placeholders.
#include "../../../src/game_aca57c/UnkBufferString.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntFile.h"
#include "base/global/CRntString.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflValue;
using base::reflection::UnkReflValueRef;

const CType* fn_710000ffd0();
s64 fn_710007cb4c(void* stream, const char* format, ...);  // write
UnkReflValue fn_71000a1774(const UnkReflValueRef* value, const CType* type);

extern void* g_unk_7101a1a5a8[];
using UnkString200 = UnkBufferString<0x200, g_unk_7101a1a5a8>;

struct UnkWriter_b0202 {
    u8 pad_0x0[0xc88];
    base::global::CRntFile* file;  // 0xc88
};

// Destroys the reflected value.
struct UnkValueGuard_b0202 {
    UnkReflValue* value;
    ~UnkValueGuard_b0202() { fn_71000a1c60(value); }
};

// A length and its bytes.
struct UnkBlob_b0202 {
    u32 length;
    u8* data;  // 0x8
};

void fn_71000ed480(UnkWriter_b0202* self, const UnkReflValueRef* ref) {
    if (fn_710009d50c(ref->type, fn_710000ffd0())) {
        const UnkBlob_b0202* blob = static_cast<const UnkBlob_b0202*>(ref->value);
        if (blob) {
            fn_710007cb4c(self->file, "i", blob->length);
            fn_710007c3f0(self->file, blob->data, blob->length);
            return;
        }
    }
    if (!fn_710009d52c(ref->type, fn_7100007f84())) {
        fn_710007c3f0(self->file, ref->value, *reinterpret_cast<const u64*>(reinterpret_cast<const u8*>(ref->type) + 0x18));
        return;
    }
    UnkReflValue value = fn_71000a1774(ref, fn_7100007f84());
    UnkValueGuard_b0202 guard = {&value};
    UnkString200 text;
    const CType* value_type = reinterpret_cast<const CType*>(value.type);
    if (value_type && fn_710009d50c(value_type, fn_7100007f84())) {
        const base::global::CRntString* src = nullptr;
        if (value.type)
            src = value.field_0x14 == 1 || *reinterpret_cast<const u64*>(value.type + 0x18) > 8
                      ? static_cast<const base::global::CRntString*>(value.field_0x0)
                      : reinterpret_cast<const base::global::CRntString*>(&value);
        fn_710007eaa0(text.Str(), src);
        const char* chars = text.Str()->data;
        base::global::CRntFile* file = self->file;
        if (!chars)
            chars = g_unk_710162e2ed;
        fn_710007c3f0(file, chars, __builtin_strlen(chars) + 1);
    }
}
