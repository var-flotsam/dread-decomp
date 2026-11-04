// The move-assignments of CHearingStimulusDef's and CStimulusDef's units (tools/tu_map.py units 2582
// and 2589): a record with a string id at +8 and a word at +0x10; the id is released as a
// destructor would, takes the other's (left empty), then the word is copied. Names and layouts are
// placeholders.
#include <new>

#include "base/UnkDestructors.h"
#include "types.h"

template <int Tag>
struct UnkIdWordRecord_b55 {
    u64 field_0x0;
    base::UnkStrIdMember id;
    u64 value;

    __attribute__((always_inline)) UnkIdWordRecord_b55& operator=(UnkIdWordRecord_b55&& other) {
        if (this != &other) {
            id.~UnkStrIdMemberOf();
            new (&id) base::UnkStrIdMember(static_cast<base::UnkStrIdMember&&>(other.id));
            value = other.value;
        }
        return *this;
    }
};

UnkIdWordRecord_b55<0>* fn_7100f041bc(UnkIdWordRecord_b55<0>* self, UnkIdWordRecord_b55<0>* other) {
    return &(*self = static_cast<UnkIdWordRecord_b55<0>&&>(*other));
}
UnkIdWordRecord_b55<1>* fn_7100f05410(UnkIdWordRecord_b55<1>* self, UnkIdWordRecord_b55<1>* other) {
    return &(*self = static_cast<UnkIdWordRecord_b55<1>&&>(*other));
}
