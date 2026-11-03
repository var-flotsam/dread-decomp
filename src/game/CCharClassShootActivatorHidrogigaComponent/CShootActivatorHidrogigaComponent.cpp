// fn_7100897298, in the CCharClassShootActivatorHidrogigaComponent unit: runs the base step
// (fn_7100890ae4), looks the owner's object up (fn_71005c1008); on the first pass (state 1) takes
// the handle fn_7100353be8 builds from +0x588 into the slot at +0x5b8 and moves to state 2, then
// gives the object the slot's target refers to a back pointer (+0xa0). Clears the flag at +0x601.
// Names and layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "types.h"

struct UnkHidrogigaShootActivator {
    u8 pad_0x0[0x30];
    u64 field_0x30;
    u8 pad_0x38[0x588 - 0x38];
    u8 field_0x588[0x28];
    u32 field_0x5b0;
    u8 pad_0x5b4[4];
    base::UnkRefHandle field_0x5b8;
    u8 pad_0x5c0[0x601 - 0x5c0];
    bool field_0x601;
};

void fn_7100890ae4(UnkHidrogigaShootActivator* self);
char* fn_71005c1008(u64 id);
using UnkRef = base::UnkRefPtrMember;
UnkRef fn_7100353be8(void* source);

void fn_7100897298(UnkHidrogigaShootActivator* self) {
    fn_7100890ae4(self);
    char* found = fn_71005c1008(self->field_0x30);
    if (found) {
        if (self->field_0x5b0 == 1) {
            {
                base::UnkRefPtrMember handle = fn_7100353be8(self->field_0x588);
                fn_710031d208(&self->field_0x5b8, reinterpret_cast<base::UnkRefHandle*>(&handle));
            }
            self->field_0x5b0 = 2;
        }
        if (base::UnkRefCounted* target = self->field_0x5b8.ptr) {
            if (*reinterpret_cast<void**>(reinterpret_cast<char*>(target) + 0x10)) {
                asm("" : "+r"(target));
                if (char* other = fn_71005c1008(*reinterpret_cast<u64*>(reinterpret_cast<char*>(target) + 0x10)))
                    *reinterpret_cast<char**>(other + 0xa0) = found;
            }
        }
    }
    self->field_0x601 = false;
}
