// Destructor of a record of a string id, 8 bytes, two string ids and a string, as fn_7100133be4
// (src/game_s1/fn_7100133be4.cpp): fn_7100e6c960.
#include "base/global/CStrId.h"
#include "base/global/UnkStringPool.h"
#include "types.h"

using namespace base::global;

struct UnkString30;
void fn_710007ed40(UnkString30* self);

struct UnkString30 {
    u64 field_0x0[6];

    __attribute__((always_inline)) ~UnkString30() { fn_710007ecd4(reinterpret_cast<CRntString*>(this)); }
};

// A string id whose destructor decrements and tests in two steps (the original reloads the id
// after the decrement).
struct UnkReleasedStrId {
    UnkStringData* data;

    __attribute__((always_inline)) ~UnkReleasedStrId() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

struct UnkIdsString {
    UnkReleasedStrId field_0x0;
    u64 field_0x8;
    UnkReleasedStrId field_0x10;
    UnkReleasedStrId field_0x18;
    UnkString30 field_0x20;

    __attribute__((always_inline)) ~UnkIdsString() { fn_710007ed40(&field_0x20); }
};

void fn_7100e6c960(UnkIdsString* self) { self->~UnkIdsString(); }
