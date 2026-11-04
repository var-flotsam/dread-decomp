// CWarLotusAIComponent unit (tools/tu_map.py, the name from reflection): forwards its two arguments to
// the owner's virtual at slot 0x2c0. Names are placeholders.
#include "types.h"

struct UnkWarLotusOwner_b64a4 {
    void** vtable;
};

void fn_71006e0e14(UnkWarLotusOwner_b64a4* self, u64 a, u64 b) {
    typedef void (*Fn)(UnkWarLotusOwner_b64a4*, u64, u64);
    reinterpret_cast<Fn>(self->vtable[0x2c0 / 8])(self, a, b);
}
