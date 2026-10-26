// Runs the base's part (fn_7100499a88) and, when that has nothing to report, checks whether the
// sub-object at +0x220 of the owner (+0x30) has the kind of thing the object from fn_710049b3cc
// describes: if so the request's value is cleared, its flag at +0x20 is set and fn_7100499a60 is
// told. Returns what the base returned.
#include "game/UnkAnimationComponent.h"
#include "types.h"

struct UnkDescribed {
    void** vtable;
};

struct UnkRequest {
    u8 pad_0x0[0x20];
    u8 field_0x20;
};

struct UnkOwnerObject {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* field_0x220;
};

struct UnkChecker {
    u8 pad_0x0[0x30];
    UnkOwnerObject* field_0x30;
};

u32 fn_7100499a88(UnkChecker* self, UnkRequest* request);
UnkDescribed* fn_710049b3cc(UnkChecker* self);
void fn_71008c1538(UnkRequest* request, f32 value);
void fn_7100499a60(UnkChecker* self);

u32 fn_710048f258(UnkChecker* self, UnkRequest* request) {
    u32 result = fn_7100499a88(self, request);
    if (result)
        return result;
    if (UnkAnimationComponent* target = self->field_0x30->field_0x220) {
        if (fn_710117ae38(target, reinterpret_cast<u64>(fn_710049b3cc(self)->vtable[6]), false)) {
            fn_71008c1538(request, 0.0f);
            request->field_0x20 = 1;
            fn_7100499a60(self);
        }
    }
    return result;
}
