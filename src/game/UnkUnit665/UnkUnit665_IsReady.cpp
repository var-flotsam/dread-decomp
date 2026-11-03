#include "types.h"

struct UnkUnit665Config {
    u8 unk_0x0[0x15a0];
    f32 threshold;
};

struct UnkUnit665Object {
    u8 unk_0x0[0x1140];
    u32 field_0x1140;
    u8 unk_0x1144[0x149a0 - 0x1144];
    UnkUnit665Config* config;
    u8 unk_0x149a8[0x15cf8 - 0x149a8];
    f32 value;
    u8 unk_0x15cfc[9];
    bool enabled;
};

bool fn_7100453d84(const UnkUnit665Object* self) {
    if (self->field_0x1140 != 0)
        return false;
    if (!self->enabled)
        return false;
    return self->value >= self->config->threshold;
}
