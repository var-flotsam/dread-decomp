#include "types.h"

struct UnkRequest;
void fn_71008c1538(UnkRequest* request, f32 value);
struct CBaseDamageTriggerComponent;
void fn_7100a47030(CBaseDamageTriggerComponent* self);

struct CBaseDamageTriggerComponent {
    u8 unk_0x0[0xac0];
    u8 request[0x2c];       // 0xac0
    u32 field_0xaec;
    u8 unk_0xaf0[0xbc4 - 0xaf0];
    f32 field_0xbc4;
    u32 field_0xbc8;
    u32 field_0xbcc;
    u8 unk_0xbd0[0x8];
    u32 field_0xbd8;
    u32 field_0xbdc;
};

// Restarts the trigger from its configured values.
void fn_7100a45cac(CBaseDamageTriggerComponent* self) {
    self->field_0xbdc = 0;
    self->field_0xbd8 = self->field_0xbcc;
    fn_71008c1538(reinterpret_cast<UnkRequest*>(self->request), self->field_0xbc4);
    self->field_0xaec = self->field_0xbc8;
    fn_7100a47030(self);
}
