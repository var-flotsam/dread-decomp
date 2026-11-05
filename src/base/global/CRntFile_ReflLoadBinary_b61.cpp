// base::global::CRntFile's unit (tools/tu_map.py unit 56): a reflected function's invoker
// (fn_710007e4e8) taking a size-prefixed blob (a 32-bit length, then the bytes) and the blob's total
// size. When the length is positive and fits, the bytes are written at the file's position (room made
// by fn_710007c514) and the position moved on; it answers the bytes consumed (length + 4), or 0, as
// an 8-byte value (fn_7100010e58). Names are placeholders.
#include <cstring>

#include "base/global/CRntFile.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::global::CRntFile;
using base::reflection::CType;
using base::reflection::UnkReflValue;

const CType* fn_7100010e58();  // an 8-byte type's getter

static inline __attribute__((always_inline)) void* ArgPtr_b61(UnkReflValue* arg) {
    if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        return arg->field_0x0;
    return arg;
}

UnkReflValue fn_710007e4e8(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* a = args[0];
    CRntFile* file = *reinterpret_cast<CRntFile**>(obj + 8);
    const s32* blob = *static_cast<const s32* const*>(ArgPtr_b61(&a[0]));
    u64 size = *static_cast<const u64*>(ArgPtr_b61(&a[1]));
    u64 read;
    s32 length;
    if (size < 4 || (length = *blob) < 1 || size - 4 < (u64)length) {
        read = 0;
    } else {
        u64 n = length;
        const s32* src = blob + 1;
        fn_710007c514(file, n, 1);
        std::memcpy(file->data + file->position, src, n);
        file->position += n;
        read = n + 4;
    }
    const CType* type = fn_7100010e58();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &read, false);
    return r;
}
