// Unit 376: the copy assignment of the link key array (base::UnkVector of 4-byte keys that are
// destroyed out of line; its copy constructor is fn_710027e258): the old keys are destroyed, the array
// grows to the other's capacity and the other's keys are appended one by one. Names are placeholders.
#include "base/UnkEmptyDtorElem.h"
#include "base/UnkVector.h"
#include "types.h"

// A 4-byte key, copied member-wise and destroyed out of line.
struct UnkLinkKey_ovb0262j {
    u32 value;
    UnkLinkKey_ovb0262j(const UnkLinkKey_ovb0262j& o) { __builtin_memcpy(&value, &o.value, sizeof(value)); }
    ~UnkLinkKey_ovb0262j() { fn_71002874dc(this); }
};

__attribute__((flatten)) base::UnkVector<UnkLinkKey_ovb0262j>* fn_71002b5e8c(
    base::UnkVector<UnkLinkKey_ovb0262j>* self, const base::UnkVector<UnkLinkKey_ovb0262j>* other) {
    if (self != other) {
        self->Clear();
        self->Reserve(other->capacity);
        for (const UnkLinkKey_ovb0262j *e = other->data, *end = other->data + other->count; e != end; ++e)
            self->PushBackCopy(*e);
    }
    return self;
}
