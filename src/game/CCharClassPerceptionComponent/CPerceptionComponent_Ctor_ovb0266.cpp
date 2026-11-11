// CPerceptionComponent's constructor: the base constructor fn_71011acd10, the vtable, the list of owned
// perception objects at +0x58 (CPerceptionComponent_Dtor_b52.cpp's layout) with one entry added: the
// "Hearing" perception (a 0x10-byte object: its vtable and the name). Names and layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/CCharClassComponent.h"
#include "game/CCharClassPerceptionComponent.h"
#include "types.h"

using base::global::CStrId;

struct UnkStruct_71008bca30;

extern void* g_unk_7101a88cb0[];  // the perception's base vtable
extern void* g_unk_7101caa358[];  // the perception's vtable


struct UnkPerceptionRaw_b0266 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    CStrId name;
};

struct UnkPerceptionPtr_b0266 {
    // A union member: the list's count is reloaded after the element is stored.
    union {
        UnkPerceptionRaw_b0266* ptr;
    };
    UnkPerceptionPtr_b0266() : ptr(nullptr) {}
    UnkPerceptionPtr_b0266(UnkPerceptionPtr_b0266&& other) {
        UnkPerceptionRaw_b0266* p = other.ptr;
        other.ptr = nullptr;
        ptr = p;
    }
    ~UnkPerceptionPtr_b0266() {
        UnkPerceptionRaw_b0266* old = ptr;
        ptr = nullptr;
        if (old)
            old->vf_0x8();
    }
};

struct UnkPerceptionComponent_b0266 {
    void** vtable;
    u8 pad_0x8[0x10];
    u64 field_0x18;
    u8 pad_0x20[0x38];
    base::UnkVector<UnkPerceptionPtr_b0266> list;  // 0x58
};
static_assert(__builtin_offsetof(UnkPerceptionComponent_b0266, list) == 0x58, "layout");

void fn_71009c7a48(UnkPerceptionComponent_b0266* self) {
    fn_71011acd10(reinterpret_cast<UnkStruct_71008bca30*>(self));
    self->vtable = g_unk_7101a88c60 + 2;
    new (&self->list) base::UnkVector<UnkPerceptionPtr_b0266>(0xf);
    self->field_0x18 = 5000;
    static CStrId s_hearing("Hearing", true);
    auto* perception = static_cast<UnkPerceptionRaw_b0266*>(fn_71000839a0(0x10, 5));
    *reinterpret_cast<void***>(perception) = g_unk_7101a88cb0 + 2;
    new (&perception->name) CStrId(s_hearing);
    *reinterpret_cast<void***>(perception) = g_unk_7101caa358 + 2;
    UnkPerceptionPtr_b0266 owned;
    owned.ptr = perception;
    self->list.EmplaceBack(static_cast<UnkPerceptionPtr_b0266&&>(owned));
}
