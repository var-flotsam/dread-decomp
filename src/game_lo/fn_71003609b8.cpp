// When the owning actor's AI component (a CCharClassAIComponent) has a flag set, hands the
// actor to a lazily created manager; then pokes two other actor components.
#include "base/reflection/CClass.h"
#include "types.h"

using base::reflection::CClass;
using base::reflection::UnkReflObject;

bool fn_710009d50c(const CClass* cls, const CClass* base);
const CClass* fn_7100408b2c();  // CCharClassAIComponent::refl_GetClass
void* fn_71000839a0(u64 size, u32 flags);

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

struct UnkStruct_71003609b8_Actor {
    u8 pad_0x0[0x218];
    void* field_0x218;
    u8 pad_0x220[0x20];
    UnkStruct_71003609b8_Holder* field_0x240;
    u8 pad_0x248[0xa0];
    void* field_0x2e8;
};

struct UnkManager_71003609b8 {
    u8 pad_0x0[0x2610];
};
void fn_7100da7e2c(UnkManager_71003609b8* self);
void fn_7100daaee4(UnkManager_71003609b8* self, UnkStruct_71003609b8_Actor* actor);
extern UnkManager_71003609b8* unk_7101cd0900;  // the manager instance (reached through the GOT)

UnkStruct_71003609b8_Target* fn_71010cc3bc(void* obj);
void* fn_7100a2c13c(void* obj, void* key);
struct UnkMask_71003609b8;
void fn_7100a2cb7c(void* obj, void* key, UnkMask_71003609b8* mask, f32 value, u32 arg);

struct UnkStruct_71003609b8 {
    u8 pad_0x0[0x8];
    UnkStruct_71003609b8_Actor* field_0x8;
    u8 pad_0x10[0x8];
    u8 field_0x18[0x8];
};

static inline UnkManager_71003609b8* GetManager() {
    if (!unk_7101cd0900) {
        UnkManager_71003609b8* m = static_cast<UnkManager_71003609b8*>(fn_71000839a0(0x2610, 0x3f));
        fn_7100da7e2c(m);
        unk_7101cd0900 = m;
    }
    return unk_7101cd0900;
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

void fn_71003609b8(UnkStruct_71003609b8* self) {
    UnkStruct_71003609b8_Ai* ai = nullptr;
    if (UnkStruct_71003609b8_Holder* h = self->field_0x8->field_0x240) {
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
    if (void* q = self->field_0x8->field_0x2e8) {
        if (fn_7100a2c13c(q, self->field_0x18)) {
            UnkMask_71003609b8 mask = UnkAll_71003609b8();
            fn_7100a2cb7c(q, self->field_0x18, &mask, -1.0f, 0);
        }
    }
}
