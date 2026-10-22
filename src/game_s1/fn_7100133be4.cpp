// Destructor of a record of three string ids and a string (base::global::CRntString's layout,
// as UnkString30 in src/game_aca57c/fn_7100b1ae74.cpp), which is cleared (fn_710007ed40) before
// the members are destroyed: fn_7100133be4.
#include "base/global/CStrId.h"
#include "types.h"

using namespace base::global;

struct UnkString30;
void fn_710007ed40(UnkString30* self);
void fn_710007ecd4(UnkString30* self);

struct UnkString30 {
    u64 field_0x0[6];

    __attribute__((always_inline)) ~UnkString30() { fn_710007ecd4(this); }
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
    UnkReleasedStrId field_0x8;
    UnkReleasedStrId field_0x10;
    UnkString30 field_0x18;

    __attribute__((always_inline)) ~UnkIdsString() { fn_710007ed40(&field_0x18); }
};

void fn_7100133be4(UnkIdsString* self) { self->~UnkIdsString(); }
