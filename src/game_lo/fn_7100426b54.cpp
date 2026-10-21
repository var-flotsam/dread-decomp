// When the owner has the sub-object at +0x220, forwards a request to it under a fixed 64-bit
// id, with a mode (2 when `flag` is set, else 0), two global descriptors and an all-bits mask.
#include "types.h"

// A small flag value whose constructors are not constexpr, so its statics are guarded.
struct UnkFlags_7100426b54 {
    u32 value;
    UnkFlags_7100426b54() { value = 0; }
};

struct UnkMask_7100426b54 {
    u32 value;
    explicit UnkMask_7100426b54(const UnkFlags_7100426b54& f) { value = (u8)~f.value; }
};

inline const UnkFlags_7100426b54& UnkNone_7100426b54() {
    static UnkFlags_7100426b54 s_value;
    return s_value;
}

inline const UnkMask_7100426b54& UnkAll_7100426b54() {
    static UnkMask_7100426b54 s_value(UnkNone_7100426b54());
    return s_value;
}

// An 8-byte argument of which only the first word is set here.
struct UnkMode_7100426b54 {
    u32 value;
    u32 field_0x4;
    explicit UnkMode_7100426b54(u32 v) : value(v) {}
};

struct UnkDesc_7100426b54;
extern UnkDesc_7100426b54 unk_7101cd0898;  // reached through the GOT
extern UnkDesc_7100426b54 unk_7101cd08a0;  // reached through the GOT

void fn_7101176690(void* target, void* arg, u64 id, const UnkMode_7100426b54* mode, const UnkDesc_7100426b54* a,
                   const UnkDesc_7100426b54* b, const UnkMask_7100426b54* mask);

struct UnkOwner_7100426b54 {
    u8 unk_0x0[0x220];
    void* field_0x220;
};

struct UnkStruct_7100426b54 {
    u8 unk_0x0[0x8];
    UnkOwner_7100426b54* owner;
};

void fn_7100426b54(UnkStruct_7100426b54* self, void* arg, bool flag) {
    void* target = self->owner->field_0x220;
    if (!target)
        return;
    UnkMode_7100426b54 mode(flag ? 2 : 0);
    UnkMask_7100426b54 mask = UnkAll_7100426b54();
    fn_7101176690(target, arg, 0x71b15e29dc542a11, &mode, &unk_7101cd0898, &unk_7101cd08a0, &mask);
}
