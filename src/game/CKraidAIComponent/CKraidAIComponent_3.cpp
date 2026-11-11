// CKraidAIComponent: fn_71005fcf9c builds a damage request on the stack (fn_71008c0c74), sets its
// amount (fn_71008c1538) to 500 or, when the word at +0x1140 is set, 250, copies a unit-static id
// into its id at +0xb8, hands it to the actor's component at +0x1b8 (virtual slot 49) and destroys
// it (fn_71008c1558). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkTU1370.h"
#include "types.h"

using base::global::CStrId;

struct UnkRequest;
struct UnkStruct_71008c0c74;
struct UnkBase_71003cc474;

struct UnkKraidDamageRequest {
    u8 pad_0x0[0xb8];
    union {
        CStrId id;  // 0xb8
    };
    u8 pad_0xc0[0x40];

    UnkKraidDamageRequest() {}
    ~UnkKraidDamageRequest() {}
};

struct UnkKraidDamageTarget {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual void vf_0xd8();
    virtual void vf_0xe0();
    virtual void vf_0xe8();
    virtual void vf_0xf0();
    virtual void vf_0xf8();
    virtual void vf_0x100();
    virtual void vf_0x108();
    virtual void vf_0x110();
    virtual void vf_0x118();
    virtual void vf_0x120();
    virtual void vf_0x128();
    virtual void vf_0x130();
    virtual void vf_0x138();
    virtual void vf_0x140();
    virtual void vf_0x148();
    virtual void vf_0x150();
    virtual void vf_0x158();
    virtual void vf_0x160();
    virtual void vf_0x168();
    virtual void vf_0x170();
    virtual void vf_0x178();
    virtual void vf_0x180();
    virtual void vf_0x188(UnkKraidDamageRequest* request);
};

struct UnkKraidDamageActor {
    u8 pad_0x0[0x1b8];
    UnkKraidDamageTarget* field_0x1b8;
};

struct UnkKraidAI3 {
    u8 pad_0x0[0x30];
    UnkKraidDamageActor* actor;  // 0x30
    u8 pad_0x38[0x1140 - 0x38];
    u32 field_0x1140;
};

extern CStrId g_unk_71726f3770 __attribute__((visibility("hidden")));
void fn_71008c1538(UnkRequest* request, f32 value);

bool fn_71005fcf9c(UnkKraidAI3* self) {
    if (self->actor->field_0x1b8) {
        UnkKraidDamageRequest request;
        fn_71008c0c74(reinterpret_cast<UnkStruct_71008c0c74*>(&request));
        fn_71008c1538(reinterpret_cast<UnkRequest*>(&request), self->field_0x1140 ? 250.0f : 500.0f);
        request.id = g_unk_71726f3770;
        self->actor->field_0x1b8->vf_0x188(&request);
        fn_71008c1558(reinterpret_cast<UnkBase_71003cc474*>(&request));
    }
    return true;
}
