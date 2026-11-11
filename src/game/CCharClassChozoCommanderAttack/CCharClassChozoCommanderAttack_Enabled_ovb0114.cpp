// CCharClassChozoCommanderAttack's unit (tools/tu_map.py): a bool tunable of the Chozo Commander,
// read by the attack's name through reflection (the field whose name hash matches), as 1.0 or 0.0;
// 1.0 when there is no such field. Names and layouts are placeholders.
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "game/UnkTU85.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflTypedPtr;

struct UnkReflField;
struct UnkReflFieldObject;
struct UnkReflValueArg;

extern base::reflection::UnkTunable g_unk_71726c91d0;

// A reflected field (0xb0 bytes).
struct UnkChozoAttackField_ovb0114 {
    u8 pad_0x0[0xb0];
};

// A class as the lookup sees it: its fields and their name hashes.
struct UnkChozoAttackClass_ovb0114 {
    u8 pad_0x0[0xe8];
    UnkChozoAttackField_ovb0114* fields;  // 0xe8
    u64* keys;                            // 0xf0
    u32 count;                            // 0xf8

    UnkChozoAttackField_ovb0114* End() { return fields + count; }
    UnkChozoAttackField_ovb0114* Find(u64 key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &fields[i];
        return End();
    }
    // The field with that name hash, null when there is none.
    UnkChozoAttackField_ovb0114* FindField(u64 key) {
        UnkChozoAttackField_ovb0114* it = Find(key);
        return it != End() ? it : nullptr;
    }
};

// A reflected object: its class is vtable slot 0.
struct UnkChozoAttackObject_ovb0114 {
    virtual UnkChozoAttackClass_ovb0114* GetClass();
};

struct UnkChozoAttackValue_ovb0114 {
    const CType* type;
    void* value;
};

static inline bool Get(UnkChozoAttackObject_ovb0114* obj, u64 key, bool* out) {
    UnkChozoAttackClass_ovb0114* cls = obj->GetClass();
    UnkReflTypedPtr typed;
    typed.type = reinterpret_cast<char*>(obj->GetClass());
    typed.ptr = obj;
    UnkChozoAttackValue_ovb0114 value = {fn_710000d228(), out};
    if (UnkChozoAttackField_ovb0114* field = cls->FindField(key)) {
        fn_71000e78e0(reinterpret_cast<UnkReflField*>(field), reinterpret_cast<const UnkReflFieldObject*>(&typed),
                      reinterpret_cast<UnkReflValueArg*>(&value));
        return true;
    }
    return false;
}

// A string's data: its key (CRC-64) at +0x30.
struct UnkChozoAttackStrData_ovb0114 {
    u8 pad_0x0[0x30];
    u64 key;
};

struct UnkChozoAttack_ovb0114 {
    u8 pad_0x0[0x38];
    UnkChozoAttackStrData_ovb0114* name;  // 0x38
};

f32 fn_710041cbe4(UnkChozoAttack_ovb0114* self) {
    bool enabled = true;
    UnkChozoAttackObject_ovb0114* tunable = reinterpret_cast<UnkChozoAttackObject_ovb0114*>(&g_unk_71726c91d0);
    Get(tunable, self->name->key, &enabled);
    return enabled;
}
