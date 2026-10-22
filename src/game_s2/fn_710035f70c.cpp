// Appends an element to a growable array whose elements have an out-of-line destructor
// (fn_71002874dc): the array grows to the next multiple of its step first, moving the elements over one
// by one.
#include "base/UnkVector.h"
#include "types.h"

struct UnkElem;
void fn_71002874dc(UnkElem* elem);

struct UnkElem {
    u32 field_0x0;

    ~UnkElem() { fn_71002874dc(this); }
};

void fn_710035f70c(base::UnkVector<UnkElem>* self, UnkElem* value) {
    self->EmplaceBack(static_cast<UnkElem&&>(*value));
}
