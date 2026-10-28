// When the owning actor's AI component (a CCharClassAIComponent) has a flag set, hands the
// actor to a lazily created manager; then pokes two other actor components.
#include "base/UnkVector.h"
#include "game/UnkAttack.h"
#include "game/UnkManager_71726af468.h"
#include "game/UnkTimelineComponent.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflStatics.h"
#include "types.h"
#include "base/reflection/UnkClassGetters.h"

using base::reflection::CClass;
using base::reflection::UnkReflObject;

struct UnkStruct_71003609b8_Ai : UnkReflObject {
    u8 pad_0x8[0x169];
    bool field_0x171;
    u8 pad_0x172[0x365];
    bool field_0x4d7;
};

struct UnkStruct_71003609b8_Holder {
    u8 pad_0x0[0x10];
    UnkReflObject* field_0x10;
};

struct UnkStruct_71003609b8_Target {
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
};

void fn_7100daaee4(UnkManager_71726af468* self, UnkActor* actor);

UnkStruct_71003609b8_Target* fn_71010cc3bc(void* obj);

static inline UnkManager_71726af468* GetManager() {
    if (!g_unk_71726af468) {
        UnkManager_71726af468* m = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
        fn_7100da7e2c(m);
        g_unk_71726af468 = m;
    }
    return g_unk_71726af468;
}

// A small flag value whose constructors are not constexpr, so its statics are guarded.
struct UnkFlags_71003609b8 {
    u32 value;
    UnkFlags_71003609b8() { value = 0; }
};

struct UnkMask_71003609b8 {
    u32 value;
    explicit UnkMask_71003609b8(const UnkFlags_71003609b8& f) { value = (u8)~f.value; }
};

inline const UnkFlags_71003609b8& UnkNone_71003609b8() {
    static UnkFlags_71003609b8 s_value;
    return s_value;
}

inline const UnkMask_71003609b8& UnkAll_71003609b8() {
    static UnkMask_71003609b8 s_value(UnkNone_71003609b8());
    return s_value;
}

void fn_71003609b8(UnkAttackBase* self) {
    UnkStruct_71003609b8_Ai* ai = nullptr;
    if (auto* h = static_cast<UnkStruct_71003609b8_Holder*>(self->field_0x8->field_0x240)) {
        UnkReflObject* c = h->field_0x10;
        if (c && fn_710009d50c(c->vf_0x0(), fn_7100408b2c()))
            ai = static_cast<UnkStruct_71003609b8_Ai*>(c);
    }
    if (ai && ai->field_0x171)
        fn_7100daaee4(GetManager(), self->field_0x8);
    if (void* p = self->field_0x8->field_0x218) {
        if (fn_71010cc3bc(p) && ai->field_0x4d7)
            fn_71010cc3bc(p)->vf_0xa8();
    }
    if (auto* q = static_cast<UnkTimelineComponent*>(self->field_0x8->field_0x2e8)) {
        const u64* key = reinterpret_cast<const u64*>(self->field_0x18);
        if (fn_7100a2c13c(q, key)) {
            base::reflection::UnkReflTypeIdArg mask;
            mask.id = UnkAll_71003609b8().value;
            fn_7100a2cb7c(q, key, &mask.id, false, -1.0f);
        }
    }
}
