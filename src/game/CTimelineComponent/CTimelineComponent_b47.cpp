// CTimelineComponent unit (tools/tu_map.py): calls the base method, then caches the owner object at +0x10
// at +0x1a8 when it is a CCharClassTimelineComponent and copies four of its settings; clears the cache
// otherwise. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"
#include "game/UnkComponent.h"

using base::reflection::UnkReflObject;


struct UnkTimelineDefLink {
    u8 pad_0x0[0x30];
    u64 field_0x30;
};

struct UnkCharClassTimeline : UnkReflObject {
    u8 pad_0x8[0x58 - 8];
    u32 field_0x58;
    u8 pad_0x5c[4];
    UnkTimelineDefLink* field_0x60;
    u32 field_0x68;
    u32 field_0x6c;
};

struct UnkTimelineComponentB47 {
    u8 pad_0x0[0x10];
    UnkCharClassTimeline* object;
    u8 pad_0x18[0x48 - 0x18];
    u32 field_0x48;
    u8 pad_0x4c[4];
    u64 field_0x50;
    u8 pad_0x58[4];
    u32 field_0x5c;
    u32 field_0x60;
    u8 pad_0x64[0x1a8 - 0x64];
    UnkCharClassTimeline* def;
};

bool fn_7100a2babc(UnkTimelineComponentB47* self, void* arg) {
    bool result = fn_71010c41bc(reinterpret_cast<UnkBaseComponent*>(self), arg);
    UnkCharClassTimeline* object = self->object;
    if (object && fn_710009d50c(object->vf_0x0(), fn_7100796bc4())) {
        self->def = object;
        self->field_0x50 = object->field_0x60->field_0x30;
        self->field_0x48 = object->field_0x58;
        self->field_0x60 = object->field_0x6c;
        self->field_0x5c = object->field_0x68;
    } else {
        self->def = nullptr;
    }
    return result;
}
