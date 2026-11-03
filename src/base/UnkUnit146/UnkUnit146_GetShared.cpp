// tools/tu_map.py unit 146 (no class hint): returns a copy of the shared pointer held at +0x18 (one
// more owner). Names are placeholders.
#include <memory>

#include "types.h"

struct UnkSharedHolder146 {
    u8 pad_0x0[0x18];
    std::shared_ptr<void> shared;  // 0x18
};

std::shared_ptr<void> fn_710014af28(const UnkSharedHolder146* self) {
    return self->shared;
}
