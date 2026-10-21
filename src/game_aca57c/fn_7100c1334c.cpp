// Setters taking a reflected value. Nine copies: the string id is read in place or through its
// pointer (by the value's kind and type size), assigned to the owner's id at Offset, and the
// id is handed with the field after it to an update function (a stateless functor passed by
// value, which takes an argument register). They return a status record {0, 0, 2, 1}.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkValueType {
    u8 unk_0x0[0x18];
    u64 size;
};

// The value is stored in place when its type fits in 8 bytes, unless kind is 1 (by reference).
struct UnkValueArg {
    void* storage;
    UnkValueType* type;
    u8 unk_0x10[0x4];
    s32 kind;

    template <class T>
    const T& Get() const {
        if (kind == 1 || type->size >= 9)
            return *static_cast<const T*>(storage);
        return *reinterpret_cast<const T*>(this);
    }
};

struct UnkValueHolder {
    u8 unk_0x0[0x8];
    char* owner;
};

struct UnkSetStatus {
    u64 field_0x0;
    u64 field_0x8;
    u32 field_0x10;
    u32 field_0x14;
};

struct UnkUpdateFn {};

// FromValue: the update function gets the new value instead of the field.
template <u32 Offset, void (*Update)(UnkUpdateFn, const CStrId*, void*), bool FromValue = false>
__attribute__((always_inline)) static inline UnkSetStatus SetStrId(void* self, UnkValueHolder* holder,
                                                                   UnkValueArg* const* arg) {
    char* owner = holder->owner;
    const CStrId& value = (*arg)->Get<CStrId>();
    CStrId* id = reinterpret_cast<CStrId*>(owner + Offset);
    *id = value;
    Update(UnkUpdateFn(), FromValue ? &value : id, owner + Offset + 8);
    UnkSetStatus status;
    status.field_0x0 = 0;
    status.field_0x8 = 0;
    status.field_0x10 = 2;
    status.field_0x14 = 1;
    return status;
}

void fn_7100c12c00(UnkUpdateFn, const CStrId* id, void* next);
void fn_7100d1f764(UnkUpdateFn, const CStrId* id, void* next);
void fn_7100cfca0c(UnkUpdateFn, const CStrId* id, void* next);

UnkSetStatus fn_7100c1334c(void* self, UnkValueHolder* holder, UnkValueArg* const* arg) {
    return SetStrId<0x18, fn_7100c12c00>(self, holder, arg);
}
UnkSetStatus fn_7100d20728(void* self, UnkValueHolder* holder, UnkValueArg* const* arg) {
    return SetStrId<0x18, fn_7100d1f764>(self, holder, arg);
}
UnkSetStatus fn_7100d208e0(void* self, UnkValueHolder* holder, UnkValueArg* const* arg) {
    return SetStrId<0x58, fn_7100d1f764>(self, holder, arg);
}
UnkSetStatus fn_7100d20a98(void* self, UnkValueHolder* holder, UnkValueArg* const* arg) {
    return SetStrId<0x98, fn_7100d1f764>(self, holder, arg);
}
UnkSetStatus fn_7100d20c50(void* self, UnkValueHolder* holder, UnkValueArg* const* arg) {
    return SetStrId<0xd8, fn_7100d1f764>(self, holder, arg);
}
UnkSetStatus fn_7100d20e08(void* self, UnkValueHolder* holder, UnkValueArg* const* arg) {
    return SetStrId<0x118, fn_7100d1f764>(self, holder, arg);
}
UnkSetStatus fn_7100d20fc0(void* self, UnkValueHolder* holder, UnkValueArg* const* arg) {
    return SetStrId<0x158, fn_7100d1f764>(self, holder, arg);
}
UnkSetStatus fn_7100cfd0c4(void* self, UnkValueHolder* holder, UnkValueArg* const* arg) {
    return SetStrId<0x18, fn_7100cfca0c, true>(self, holder, arg);
}
UnkSetStatus fn_7100cfd1d0(void* self, UnkValueHolder* holder, UnkValueArg* const* arg) {
    return SetStrId<0x48, fn_7100cfca0c, true>(self, holder, arg);
}

// A setter that only acts on a change: the new id clears the cached object after it and the
// owner is told through vf_0x88.
struct UnkStrIdTarget {
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
    u8 unk_0x8[0x1a0];
    CStrId field_0x1a8;
    void* field_0x1b0;
};

struct UnkStrIdTargetHolder {
    u8 unk_0x0[0x8];
    UnkStrIdTarget* owner;
};

UnkSetStatus fn_7100b870d0(void* self, UnkStrIdTargetHolder* holder, UnkValueArg* const* arg) {
    UnkStrIdTarget* owner = holder->owner;
    const CStrId& value = (*arg)->Get<CStrId>();
    if (value.data != owner->field_0x1a8.data) {
        owner->field_0x1a8 = value;
        owner->field_0x1b0 = nullptr;
        owner->vf_0x88();
    }
    UnkSetStatus status;
    status.field_0x0 = 0;
    status.field_0x8 = 0;
    status.field_0x10 = 2;
    status.field_0x14 = 1;
    return status;
}

// An angle setter: degrees from the value, converted to radians in double precision.
struct UnkAngleOwner {
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
    u8 unk_0x8[0xa0];
    u8 field_0xa8[0x10];
};

struct UnkAngleOwnerHolder {
    u8 unk_0x0[0x8];
    UnkAngleOwner* owner;
};

void fn_7100110664(void* angle, f32 radians);

UnkSetStatus fn_7100d69140(void* self, UnkAngleOwnerHolder* holder, UnkValueArg* const* arg) {
    UnkAngleOwner* owner = holder->owner;
    f32 degrees = (*arg)->Get<f32>();
    fn_7100110664(owner->field_0xa8, (f32)((f64)degrees * 3.141592653589793 / 180.0));
    owner->vf_0x80();
    UnkSetStatus status;
    status.field_0x0 = 0;
    status.field_0x8 = 0;
    status.field_0x10 = 2;
    status.field_0x14 = 1;
    return status;
}

// Copies two values of every item in a target's list into the target.
struct UnkListItem {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual s32 vf_0x20();
    virtual void vf_0x28();
    virtual s32 vf_0x30();
};

struct UnkItemList {
    u8 unk_0x0[0x8];
    u32 count;
};

struct UnkListTarget {
    u8 unk_0x0[0x8];
    UnkItemList* list;
};

struct UnkListTargetRef {
    UnkListTarget* target;
};

UnkListItem** fn_7100b081ac(UnkItemList* list, u32 index);
void fn_7100b04510(UnkListTarget* target, s32 value);
void fn_7100b04524(UnkListTarget* target, s32 value);

UnkSetStatus fn_7100b08704(void* self, void* holder, UnkValueArg* const* arg) {
    UnkListTarget* target = (*arg)->Get<UnkListTargetRef*>()->target;
    UnkItemList* list = target->list;
    for (u32 i = 0; i < list->count; ++i) {
        UnkListItem* item = *fn_7100b081ac(list, i);
        fn_7100b04510(target, item->vf_0x20());
        fn_7100b04524(target, item->vf_0x30());
    }
    UnkSetStatus status;
    status.field_0x0 = 0;
    status.field_0x8 = 0;
    status.field_0x10 = 2;
    status.field_0x14 = 1;
    return status;
}
