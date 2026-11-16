// Unit 2506: builds a CTakumakuAIComponent request from a source's settings with its two position triples
// exchanged where the flags say so (each exchange sets the other triple's flag). Names and layouts are
// placeholders.

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

    // The out-of-line constructor, fn_71006d4d4c.
    UnkTakumakuRequest(UnkWeakTarget* owner_, const UnkTakumakuSettings* src, UnkWeakTarget* other_)
        asm("fn_71006d4d4c");
};

struct UnkTakumakuHolder_d4a138 {
    u8 pad_0x0[0x10];
    UnkWeakTarget* target;  // 0x10
};

struct UnkTakumakuSource_d4a138 {
    UnkTakumakuHolder_d4a138* other;  // 0x0
    UnkTakumakuHolder_d4a138* owner;  // 0x8
    UnkTakumakuSettings settings;     // 0x10
};

UnkTakumakuRequest fn_7100d4a138(const UnkTakumakuSource_d4a138* src) {
    const UnkTakumakuSettings& in = src->settings;
    bool swap_first = in.flags & 0x10;
    UnkTakumakuSettings settings;
    settings.field_0x0 = 0xffffffffu;
    settings.field_0x4 = in.field_0x4;
    settings.field_0x8 = in.field_0x8;
    settings.field_0xc = in.field_0xc;
    settings.field_0x10 = swap_first ? in.field_0x1c : in.field_0x10;
    settings.field_0x14 = swap_first ? in.field_0x20 : in.field_0x14;
    settings.field_0x18 = swap_first ? in.field_0x24 : in.field_0x18;
    settings.field_0x1c = in.field_0x1c;
    settings.field_0x20 = in.field_0x20;
    settings.field_0x24 = in.field_0x24;
    settings.field_0x28 = in.field_0x28;
    settings.flags = swap_first ? in.flags | 0x20 : in.flags;
    if (in.flags & 0x20) {
        settings.field_0x1c = in.field_0x10;
        settings.field_0x20 = in.field_0x14;
        settings.field_0x24 = in.field_0x18;
        settings.flags |= 0x10;
    }
    settings.flags |= in.flags & 0x200;
    if (in.flags & 1) {
        settings.flags |= 1;
        settings.field_0x0 = in.field_0x0;
    }
    UnkWeakTarget* owner = src->owner ? src->owner->target : nullptr;
    UnkWeakTarget* other = src->other ? src->other->target : nullptr;
    return UnkTakumakuRequest(owner, &settings, other);
}
