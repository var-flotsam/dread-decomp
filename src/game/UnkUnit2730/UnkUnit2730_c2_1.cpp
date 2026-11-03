// Unit 2730: calls each listener (+0x30, +0x38 of them) with the object at +0x58 and the event 3,
// then fn_710021a114 on the render manager. Names and layouts are placeholders.
#include "game/UnkRenderManager.h"
#include "types.h"

struct UnkListener_7101197cbc {
    void* context;
    void (*callback)(void* context, void* object, u32 event);
};

struct UnkObj_7101197cbc {
    u8 unk_0x0[0x30];
    UnkListener_7101197cbc* listeners;
    u32 count;
    u8 unk_0x3c[0x1c];
    void* field_0x58;
};

void fn_710021a114(UnkRenderManager* self);

void fn_7101197cbc(UnkObj_7101197cbc* self) {
    if (self->count) {
        UnkListener_7101197cbc* it = self->listeners;
        UnkListener_7101197cbc* end = it + self->count;
        void* object = self->field_0x58;
        for (; it != end; ++it)
            it->callback(it->context, object, 3);
    }
    fn_710021a114(g_unk_7101d47300);
}
