// tools/tu_map.py unit 127 (no class hint): binds the receiver's link at +0x140 to what a source's
// virtual returns, once (nothing when the link is already bound or there is no source). Names and
// layouts are placeholders.
#include "types.h"

struct UnkLinkTarget127_b66a1;
bool fn_710002589c(void* link, UnkLinkTarget127_b66a1* target, u32 mode, void* extra);

struct UnkLinkSource127_b66a1 {
    void** vtable;
};

struct UnkLinkOwner127_b66a1 {
    u8 pad_0x0[0x140];
    UnkLinkTarget127_b66a1* bound;  // 0x140, the link's first word
};

void fn_7100143790(UnkLinkOwner127_b66a1* self, UnkLinkSource127_b66a1* source, u32 mode, void* extra) {
    if (source == nullptr) {
        return;
    }
    if (__builtin_expect(self->bound == nullptr, 1)) {
        typedef UnkLinkTarget127_b66a1* (*Fn)(UnkLinkSource127_b66a1*);
        UnkLinkTarget127_b66a1* target = reinterpret_cast<Fn>(source->vtable[0x20 / 8])(source);
        fn_710002589c(&self->bound, target, mode, extra);
    }
}
