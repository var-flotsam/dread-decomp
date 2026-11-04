// Sets an item's flag at 0xf8; when it changes, reports it (gated by 0xf9) to the owner, and when
// the flag ends up clear, notifies the object at the description's +0x78 (fn_71001933cc).
#include "engine/scene/UnkToggle.h"

void fn_71001933cc(void*);

void fn_7100208ca8(char* p, bool on) {
    UnkToggle* self = reinterpret_cast<UnkToggle*>(p);
    if (self->flag_0xf8 != on) {
        self->flag_0xf8 = on;
        if (self->listener_0x8)
            fn_71001eef58(self->listener_0x8, self, on && self->flag_0xf9);
        if (!self->flag_0xf8)
            fn_71001933cc(*reinterpret_cast<void**>(reinterpret_cast<char*>(self->src_0x108) + 0x78));
    }
}
