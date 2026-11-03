// tools/tu_map.py unit 148 (no class hint): returns a copy of the shared pointer held at +0x18 (one
// more owner). Names are placeholders.
#include <memory>

#include "types.h"

struct UnkSharedHolder148 {
    u8 pad_0x0[0x18];
    std::shared_ptr<void> shared;  // 0x18
};

std::shared_ptr<void> fn_710014b350(const UnkSharedHolder148* self) {
    return self->shared;
}
