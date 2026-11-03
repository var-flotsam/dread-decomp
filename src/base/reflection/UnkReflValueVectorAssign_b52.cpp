// Unit 83 of tools/tu_map.py (base::reflection): the copy assignment of an array of reflected values
// (the vector of src/base/reflection/UnkReflValueVectorCopy.cpp, fn_7100095acc): the values destroyed,
// room made for the other's capacity, then each of the other's values appended (fn_7100095e98). Names
// are placeholders.
#include "base/UnkVector.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::UnkReflValue;

void fn_71000a179c(UnkReflValue* self, const UnkReflValue* other);  // copy constructor

// As src/base/reflection/UnkReflValueVectorCopy.cpp defines them.
struct UnkReflValueElem_7100095acc {
    UnkReflValue value;

    UnkReflValueElem_7100095acc(UnkReflValueElem_7100095acc&& other) { fn_71000a179c(&value, &other.value); }
    ~UnkReflValueElem_7100095acc() { fn_71000a1c60(&value); }
};

using UnkReflValueVector_7100095acc = base::UnkVector<UnkReflValueElem_7100095acc>;

void fn_7100095e98(UnkReflValueVector_7100095acc* self, const UnkReflValueElem_7100095acc* value);

UnkReflValueVector_7100095acc* fn_71000d592c(UnkReflValueVector_7100095acc* self,
                                             const UnkReflValueVector_7100095acc* other) {
    if (self != other) {
        self->Clear();
        self->Reserve(other->capacity);
        for (const UnkReflValueElem_7100095acc *p = other->data, *end = p + other->count; p != end; ++p)
            fn_7100095e98(self, p);
    }
    return self;
}
