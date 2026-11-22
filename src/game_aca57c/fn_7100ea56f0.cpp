// Sets a record's number and builds its two ids from a base string: the string with the number
// appended, and then with "_border" appended as well.
#include "UnkBufferString.h"
#include "base/UnkPooledStrId.h"
#include "base/global/CStrId.h"
#include "base/global/UnkTwoStepStrId.h"
#include "game/UnkBufferStringPool.h"
#include "game/UnkIdWord.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::UnkStrIdBase;
using base::global::UnkStringData;
using base::global::UnkStringPool;

using UnkString20 = UnkBufferString<0x20, g_unk_7101a1b5a0>;

// A string id taking over another (CStrId's move-assignment).
struct UnkAssignedStrId {
    UnkStringData* data;

    __attribute__((always_inline)) void operator=(base::UnkPooledString&& other) {
        UnkStringPool* pool = g_unk_7101d47108;
        if (pool) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
        }
        data = other.data;
        other.data = &pool->empty;
    }
};

extern u8 g_unk_71729cdad8[] __attribute__((visibility("hidden")));

struct UnkNumberedIds {
    u32 field_0x0;
    u32 field_0x4;
    UnkAssignedStrId field_0x8;
    UnkAssignedStrId field_0x10;
};

struct UnkTempStrIdT : base::UnkPooledString {
    __attribute__((always_inline)) UnkTempStrIdT(const UnkString20& str) {
        fn_7100000850(reinterpret_cast<UnkStrIdBase*>(this), *str.Str(), true);
    }
};

// NON_MATCHING(minor): the original keeps the moved-from temporary's empty-string stores (it
// outlives the string here), which ours drops as dead.
void fn_7100ea56f0(UnkNumberedIds* self, u32 number) {
    self->field_0x4 = number;
    UnkString20 str;
    fn_710007eb6c(&str, g_unk_71729cdad8);
    fn_710007f634(&str, self->field_0x4);
    self->field_0x8 = UnkTempStrIdT(str);
    fn_710007f564(&str, "_border");
    self->field_0x10 = UnkTempStrIdT(str);
}

// Builds a record's id from a string, with "[suffix]" appended when the suffix isn't empty, and
// takes the word after the string.
using UnkString80 = UnkBufferString<0x80, g_unk_7101a1b750>;

struct UnkSuffix {
    const char* data;
};

struct UnkStringWord {
    u8 field_0x0[0xc8];
    u64 field_0xc8;
};

void fn_7100ae9bb0(UnkIdWord* self, const UnkStringWord* source, const UnkSuffix* suffix) {
    self->field_0x8 = source->field_0xc8;
    UnkString80 str;
    fn_710007eaa0(str.Str(), reinterpret_cast<const base::global::CRntString*>(source));
    if (suffix->data && suffix->data[0]) {
        fn_710007f5ac(str.Str(), '[');
        fn_710007f3b4(str.Str(), reinterpret_cast<const base::global::CRntString*>(suffix));
        fn_710007f5ac(str.Str(), ']');
    }
    base::UnkPooledString id;
    fn_7100000850(reinterpret_cast<UnkStrIdBase*>(&id), *str.Str(), true);
    self->id.~UnkStrIdTwoStep();
    self->id.data = id.data;
}
