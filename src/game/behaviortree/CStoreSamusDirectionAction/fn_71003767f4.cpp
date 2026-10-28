// When fn_7100394e2c fills an offset for the record at +0x30 and the owner's object at +0x240
// answers a negative value from its vtable slot 0x328, hands the negated offset to
// fn_71003954fc. Returns 5 when the object exists (6 otherwise).
#include "types.h"

struct UnkObject {
    void** vtable;
    f32 vf_0x328(u32 a, u32 b) { return reinterpret_cast<f32 (*)(UnkObject*, u32, u32)>(vtable[0x65])(this, a, b); }
};

struct UnkOwner {
    u8 pad_0x0[0x240];
    UnkObject* field_0x240;
};

struct UnkSelf {
    u8 pad_0x0[0x18];
    UnkOwner* field_0x18;
    u8 pad_0x20[0x10];
    u8 field_0x30[1];
};

bool fn_7100394e2c(UnkSelf* self, u8* record, f32* offset);
void fn_71003954fc(UnkSelf* self, u8* record, f32 offset);

u32 fn_71003767f4(UnkSelf* self) {
    f32 out;
    u8* record = self->field_0x30;
    if (fn_7100394e2c(self, record, &out)) {
        if (UnkOwner* owner = self->field_0x18) {
            if (UnkObject* object = owner->field_0x240) {
                if (object->vf_0x328(0, 1) < 0.0f)
                    fn_71003954fc(self, record, -out);
                return 5;
            }
        }
    }
    return 6;
}
