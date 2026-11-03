#include "types.h"

struct UnkUnit510Data {
    u8 unk_0x0[0x121e];
    bool field_0x121e;
    u8 unk_0x121f;
    bool field_0x1220;
};

struct UnkUnit510Source {
    u8 unk_0x0[0x240];
    UnkUnit510Data* data;
};

struct UnkUnit510Node {
    u8 unk_0x0[0x18];
    UnkUnit510Source* source;
    u8 unk_0x20[0x18];
    UnkUnit510Data* data;
    bool useAlt;
};

bool fn_7100399d74(UnkUnit510Node* self) {
    if (!self->source)
        return false;
    self->data = self->source->data;
    if (!self->data)
        return false;
    if (self->useAlt)
        return self->data->field_0x1220;
    return self->data->field_0x121e;
}
