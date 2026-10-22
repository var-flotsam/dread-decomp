// Two copies of a record's move constructor: two words, a string id that is copied, a string id
// that is taken over (the source is left holding the empty id), seventeen words and three flags;
// a fourth flag is cleared.
#include <new>

#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;

struct UnkMovedStrId {
    UnkStringData* data;

    __attribute__((always_inline)) UnkMovedStrId(UnkMovedStrId&& other) : data(other.data) {
        other.data = &g_unk_7101d47108->empty;
    }
};

// Copied word by word.
struct UnkWords17 {
    u32 values[17];

    __attribute__((always_inline)) UnkWords17(const UnkWords17& other) {
        for (int i = 0; i < 17; ++i)
            values[i] = other.values[i];
    }
};

struct UnkIdRecord60 {
    u32 field_0x0;
    u32 field_0x4;
    CStrId id;
    UnkMovedStrId moved_id;
    UnkWords17 words;
    bool field_0x5c;
    bool field_0x5d;
    bool field_0x5e;
    bool field_0x5f;

    __attribute__((always_inline)) UnkIdRecord60(UnkIdRecord60&& other)
        : field_0x0(other.field_0x0), field_0x4(other.field_0x4), id(other.id),
          moved_id(static_cast<UnkMovedStrId&&>(other.moved_id)), words(other.words), field_0x5c(other.field_0x5c),
          field_0x5d(other.field_0x5d), field_0x5e(other.field_0x5e), field_0x5f(false){}
};
static_assert(sizeof(UnkIdRecord60) == 0x60, "UnkIdRecord60 layout");

void fn_7100d9b18c(UnkIdRecord60* self, UnkIdRecord60* other) {
    new (self) UnkIdRecord60(static_cast<UnkIdRecord60&&>(*other));
}
void fn_7100d9be3c(UnkIdRecord60* self, UnkIdRecord60* other) {
    new (self) UnkIdRecord60(static_cast<UnkIdRecord60&&>(*other));
}
