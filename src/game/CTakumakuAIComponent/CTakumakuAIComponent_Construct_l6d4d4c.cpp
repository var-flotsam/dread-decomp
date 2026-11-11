// CTakumakuAIComponent unit: builds a request that weakly refers to two objects and copies twelve
// settings; the flags get bit 10 when the second object is given.
// Names and layouts are placeholders.

#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkTakumakuSettings {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;
    u32 field_0x1c;
    u32 field_0x20;
    u32 field_0x24;
    u32 field_0x28;
    u32 flags;
};

struct UnkWeakRef {
    UnkWeakHandle* handle;

    __attribute__((always_inline)) explicit UnkWeakRef(UnkWeakTarget* target) {
        UnkWeakHandle* found = target ? GetWeakHandle(target) : nullptr;
        handle = nullptr;
        AssignRef(&handle, found);
    }
};

struct UnkTakumakuRequest {
    UnkWeakRef owner;
    UnkWeakRef other;
    UnkTakumakuSettings settings;
    u32 field_0x40;

    UnkTakumakuRequest(UnkWeakTarget* owner_, const UnkTakumakuSettings* src, UnkWeakTarget* other_);
};

__attribute__((always_inline)) inline UnkTakumakuRequest::UnkTakumakuRequest(UnkWeakTarget* owner_, const UnkTakumakuSettings* src,
                                              UnkWeakTarget* other_)
    : owner(owner_), other(other_) {
    settings.field_0x0 = src->field_0x0;
    settings.field_0x4 = src->field_0x4;
    settings.field_0x8 = src->field_0x8;
    settings.field_0xc = src->field_0xc;
    settings.field_0x10 = src->field_0x10;
    settings.field_0x14 = src->field_0x14;
    settings.field_0x18 = src->field_0x18;
    settings.field_0x1c = src->field_0x1c;
    settings.field_0x20 = src->field_0x20;
    settings.field_0x24 = src->field_0x24;
    settings.field_0x28 = src->field_0x28;
    settings.flags = src->flags;
    field_0x40 = 0;
    if (other_)
        settings.flags |= 0x400;
}

void fn_71006d4d4c(UnkTakumakuRequest* self, UnkWeakTarget* owner, const UnkTakumakuSettings* src,
                   UnkWeakTarget* other) {
    new (self) UnkTakumakuRequest(owner, src, other);
}
