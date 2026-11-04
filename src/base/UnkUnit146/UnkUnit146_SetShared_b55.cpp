// tools/tu_map.py units 146 and 148 (no class hint; next to fn_710014af28 and fn_710014b350): pass a
// copy of a shared pointer to the object at +0x10 through a function of the unit, with 1 and -1.
// Names are placeholders.
#include <memory>

#include "types.h"

void fn_7100005b9c(void* obj, std::shared_ptr<void> value, bool flag, s64 index);
void fn_71000038f8(void* obj, std::shared_ptr<void> value, bool flag, s64 index);

struct UnkSharedTarget {
    u8 pad_0x0[0x10];
    u8 field_0x10[1];
};

void fn_710014af54(UnkSharedTarget* self, const std::shared_ptr<void>& value) {
    fn_7100005b9c(self->field_0x10, value, true, -1);
}

void fn_710014b37c(UnkSharedTarget* self, const std::shared_ptr<void>& value) {
    fn_71000038f8(self->field_0x10, value, true, -1);
}
