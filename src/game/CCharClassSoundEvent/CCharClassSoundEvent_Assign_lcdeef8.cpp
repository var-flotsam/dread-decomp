// CCharClassSoundEvent unit (tools/tu_map.py unit 2297): the event's copy-assignment: the base's word, the
// eight file-path ids, the string id, the block of settings and the two last string ids and word.
// Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

// The event base: polymorphic, with one word.
struct UnkSoundEventBase_lcdeef8 {
    virtual void vf_0x0();
    u32 field_0x8;
};

struct UnkSoundEvent_lcdeef8 : UnkSoundEventBase_lcdeef8 {
    CFilePathStrId field_0x10[8];
    CStrId field_0x50;
    f32 field_0x58;
    f32 field_0x5c;
    f32 field_0x60;
    f32 field_0x64;
    f32 field_0x68;
    bool field_0x6c;
    u32 field_0x70;
    u32 field_0x74;
    f32 field_0x78;
    f32 field_0x7c;
    bool field_0x80;
    bool field_0x81;
    CStrId field_0x88;
    CStrId field_0x90;
    u32 field_0x98;

    // The implicit copy-assignment, emitted out of line (fn_7100cdeef8).
    UnkSoundEvent_lcdeef8& operator=(const UnkSoundEvent_lcdeef8& other) asm("fn_7100cdeef8");
};
static_assert(__builtin_offsetof(UnkSoundEvent_lcdeef8, field_0x88) == 0x88, "layout");

UnkSoundEvent_lcdeef8& UnkSoundEvent_lcdeef8::operator=(const UnkSoundEvent_lcdeef8& other) = default;
