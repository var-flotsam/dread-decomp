// Eight setters of an object's weak-handle slots (0x2240 to 0x2278): each stores the weak
// handle of the given target (created on first use; none for a null target) and releases the
// handle it replaces.
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkHandleOwner_0x2280 {
    u8 unk_0x0[0x2240];
    UnkWeakHandle* field_0x2240;
    UnkWeakHandle* field_0x2248;
    UnkWeakHandle* field_0x2250;
    UnkWeakHandle* field_0x2258;
    UnkWeakHandle* field_0x2260;
    UnkWeakHandle* field_0x2268;
    UnkWeakHandle* field_0x2270;
    UnkWeakHandle* field_0x2278;
};

#define SETTER(addr, field)                                                                    \
    void fn_##addr(UnkHandleOwner_0x2280* self, UnkWeakTarget* target) {                       \
        if (target)                                                                            \
            AssignRef(&self->field, GetWeakHandle(target));                                    \
        else                                                                                   \
            ResetRef(&self->field);                                                            \
    }

SETTER(7100e02988, field_0x2248)
SETTER(7100e02acc, field_0x2240)
SETTER(7100e02c10, field_0x2258)
SETTER(7100e02d54, field_0x2250)
SETTER(7100e02e98, field_0x2260)
SETTER(7100e02fdc, field_0x2268)
SETTER(7100e03120, field_0x2270)
SETTER(7100e03264, field_0x2278)
