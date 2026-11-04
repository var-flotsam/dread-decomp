// CMultiModelUpdaterComponent's unit (tools/tu_map.py): a reflected function that reads a bool from the argument's
// stream (fn_710007cf30, "b", as fn_71009f0fc4 in CSamusMovement_ReflSetGunFlag_A6_b63.cpp) and, when it differs
// from the flag at +0x218, stores it and passes it to the bool setter of the object at +0x138 of the +0x50 object;
// it returns the int 1. fn_7100939440 selects the model named by the string id argument: when it differs from the
// current one (+0x210) and the model map (entries of 0x20 at +0x1e8, keys at +0x1f0, count at +0x1f8) has it, and
// fn_7100938fcc accepts the entry, it takes the name; it returns whether it did. Names are placeholders.
#include "../../game_s7/UnkPropS7.h"
#include "base/global/CStrId.h"
#include "base/UnkCloneCalls.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::CType;
using base::reflection::UnkReflValue;

struct UnkClone_fn_71001c3314;
extern template void base::refl_SetBool<UnkClone_fn_71001c3314, 0x360>(char*, bool);

static inline __attribute__((always_inline)) void* ArgPtr_A5_b67(UnkReflValue* arg) {
    if (!arg->type)
        return nullptr;
    if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        return arg->field_0x0;
    return arg;
}

UnkReflValue fn_71009395b8(void*, char* obj, UnkReflValue* const* args) {
    char* self = *reinterpret_cast<char**>(obj + 8);
    void* stream = ArgPtr_A5_b67(args[0]);
    u8 value = 0;
    s32 one;
    fn_710007cf30(stream, "b", &value);
    u8 flag = value;
    if (*reinterpret_cast<u8*>(self + 0x218) != flag) {
        bool on = flag != 0;
        *reinterpret_cast<u8*>(self + 0x218) = flag;
        char* model = *reinterpret_cast<char**>(*reinterpret_cast<char**>(self + 0x50) + 0x138);
        base::refl_SetBool<UnkClone_fn_71001c3314, 0x360>(model, on);
    }
    one = 1;
    const CType* type = fn_7100014768();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &one, false);
    return r;
}

struct UnkModelEntry_A5_b67 {
    u8 pad_0x0[0x20];
};

struct UnkMultiModel_A5_b67;
bool fn_7100938fcc(UnkMultiModel_A5_b67* self, UnkModelEntry_A5_b67* entry);

struct UnkMultiModel_A5_b67 {
    u8 pad_0x0[0x1e8];
    UnkModelEntry_A5_b67* values;
    const void** keys;
    u32 count;
    u8 pad_0x1fc[0x14];
    CStrId name;  // 0x210

    UnkModelEntry_A5_b67* Find(const void* const& key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &values[i];
        return values + count;
    }
};

static inline __attribute__((always_inline)) void* ArgPtrNull_A5_b67(UnkReflValue* arg) {
    if (!arg->type) {
        void* null = nullptr;
        asm("" : "+r"(null));
        return null;
    }
    if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        return arg->field_0x0;
    return arg;
}

static inline __attribute__((always_inline)) bool Select_A5_b67(UnkMultiModel_A5_b67* self, const CStrId& name) {
    if (self->name.data == name.data)
        return false;
    UnkModelEntry_A5_b67* it = self->Find(name.data);
    if (it == self->values + self->count)
        return false;
    if (!fn_7100938fcc(self, it))
        return false;
    self->name = name;
    return true;
}

UnkReflValue fn_7100939440(void*, char* obj, UnkReflValue* const* args) {
    UnkMultiModel_A5_b67* self = *reinterpret_cast<UnkMultiModel_A5_b67**>(obj + 8);
    const CStrId* value = static_cast<const CStrId*>(ArgPtrNull_A5_b67(args[0]));
    bool done = Select_A5_b67(self, *value);
    const CType* type = fn_710000d228();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &done, false);
    return r;
}
