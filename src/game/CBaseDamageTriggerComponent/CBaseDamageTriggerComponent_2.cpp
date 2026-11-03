// CBaseDamageTriggerComponent: fn_7100a45e00 restarts the trigger like fn_7100a45cac
// (CBaseDamageTriggerComponent_b34.cpp) while its state at +0x9b8 is at most one and both the damage
// and the interval are positive, then spends `dt`: once the delay at +0xbd8 has run out it applies the
// growing damage (fn_71008c1538 with min(+0xbb8 + +0xbd0, +0xbd4)) once per interval, and keeps the
// rest of `dt` modulo the interval at +0xbdc. Names and layouts are placeholders.
#include "types.h"

struct UnkRequest;
void fn_71008c1538(UnkRequest* request, f32 value);

struct UnkBaseDamageTrigger2 {
    u8 unk_0x0[0x9b8];
    s32 state;  // 0x9b8
    u8 unk_0x9bc[0xac0 - 0x9bc];
    u8 request[0x21];  // 0xac0
    bool field_0xae1;
    u8 unk_0xae2[0xa];
    u32 field_0xaec;
    u8 unk_0xaf0[0xbb8 - 0xaf0];
    f32 field_0xbb8;
    u8 unk_0xbbc[0x8];
    f32 field_0xbc4;  // damage
    f32 field_0xbc8;  // interval
    f32 field_0xbcc;  // delay
    f32 field_0xbd0;
    f32 field_0xbd4;
    f32 field_0xbd8;
    f32 field_0xbdc;
    bool field_0xbe0;
};

void fn_7100a45e00(UnkBaseDamageTrigger2* self, f32 dt) {
    if (self->state > 1 || self->field_0xbc4 <= 0.0f || self->field_0xbc8 <= 0.0f)
        return;
    self->field_0xbdc = 0.0f;
    *reinterpret_cast<u32*>(&self->field_0xbd8) = *reinterpret_cast<u32*>(&self->field_0xbcc);
    fn_71008c1538(reinterpret_cast<UnkRequest*>(self->request), self->field_0xbc4);
    self->field_0xaec = *reinterpret_cast<u32*>(&self->field_0xbc8);
    if (!(dt > 0.0f))
        return;
    f32 interval = self->field_0xbc8;
    if (self->field_0xbd8 >= dt) {
        self->field_0xbd8 = self->field_0xbcc - dt;
    } else {
        f32 rest = dt - self->field_0xbd8;
        self->field_0xbd8 = 0.0f;
        if (rest > 0.0f) {
            self->field_0xae1 = self->field_0xbe0;
            do {
                f32 amount = self->field_0xbb8 + self->field_0xbd0;
                fn_71008c1538(reinterpret_cast<UnkRequest*>(self->request),
                              amount < self->field_0xbd4 ? amount : self->field_0xbd4);
                interval = self->field_0xbc8;
                rest = rest - interval < 0.0f ? 0.0f : rest - interval;
            } while (rest >= interval);
            self->field_0xbd8 = rest;
        }
    }
    if (interval <= dt) {
        do
            dt -= interval;
        while (dt >= interval);
    }
    self->field_0xbdc = dt;
}
