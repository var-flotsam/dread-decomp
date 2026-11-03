// From unit 72 of tools/tu_map.py (base::reflection::CClass's unit): the copy constructor of an
// array of reflected values (base::UnkVector of UnkReflValue): the other's settings, room for its
// capacity (the values moved with fn_71000a179c and destroyed with fn_71000a1c60, as the array's
// reallocation does), then each value appended with fn_7100095e98 (fn_7100095acc). Names are
// placeholders.
#include "base/UnkVector.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::UnkReflValue;

void fn_71000a179c(UnkReflValue* self, const UnkReflValue* other);  // copy constructor

struct UnkReflValueElem_7100095acc {
    UnkReflValue value;

    UnkReflValueElem_7100095acc(UnkReflValueElem_7100095acc&& other) { fn_71000a179c(&value, &other.value); }
    ~UnkReflValueElem_7100095acc() { fn_71000a1c60(&value); }
};

using UnkReflValueVector_7100095acc = base::UnkVector<UnkReflValueElem_7100095acc>;

void fn_7100095e98(UnkReflValueVector_7100095acc* self, const UnkReflValueElem_7100095acc* value);

void fn_7100095acc(UnkReflValueVector_7100095acc* self, const UnkReflValueVector_7100095acc* other) {
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = other->step;
    self->field_0x14 = other->field_0x14;
    self->flags = other->flags;
    self->allocator = other->allocator;
    self->Reserve(other->capacity);
    for (const UnkReflValueElem_7100095acc *p = other->data, *end = p + other->count; p != end; ++p)
        fn_7100095e98(self, p);
}
