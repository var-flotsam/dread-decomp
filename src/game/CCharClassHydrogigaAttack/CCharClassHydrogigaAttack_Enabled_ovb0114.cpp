// CCharClassHydrogigaAttack's unit (tools/tu_map.py): a bool tunable of Hydrogiga,
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

extern base::reflection::UnkTunable g_unk_71726eb108;

// A reflected field (0xb0 bytes).
struct UnkHydrogigaAttackField_ovb0114 {
    u8 pad_0x0[0xb0];
};

// A class as the lookup sees it: its fields and their name hashes.
struct UnkHydrogigaAttackClass_ovb0114 {
    u8 pad_0x0[0xe8];
    UnkHydrogigaAttackField_ovb0114* fields;  // 0xe8
    u64* keys;                            // 0xf0
    u32 count;                            // 0xf8

    UnkHydrogigaAttackField_ovb0114* End() { return fields + count; }
    UnkHydrogigaAttackField_ovb0114* Find(u64 key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &fields[i];
        return End();
    }
    // The field with that name hash, null when there is none.
    UnkHydrogigaAttackField_ovb0114* FindField(u64 key) {
        UnkHydrogigaAttackField_ovb0114* it = Find(key);
        return it != End() ? it : nullptr;
    }
};

// A reflected object: its class is vtable slot 0.
struct UnkHydrogigaAttackObject_ovb0114 {
    virtual UnkHydrogigaAttackClass_ovb0114* GetClass();
};

struct UnkHydrogigaAttackValue_ovb0114 {
    const CType* type;
    void* value;
};

static inline bool Get(UnkHydrogigaAttackObject_ovb0114* obj, u64 key, bool* out) {
    UnkHydrogigaAttackClass_ovb0114* cls = obj->GetClass();
    UnkReflTypedPtr typed;
    typed.type = reinterpret_cast<char*>(obj->GetClass());
    typed.ptr = obj;
    UnkHydrogigaAttackValue_ovb0114 value = {fn_710000d228(), out};
    if (UnkHydrogigaAttackField_ovb0114* field = cls->FindField(key)) {
        fn_71000e78e0(reinterpret_cast<UnkReflField*>(field), reinterpret_cast<const UnkReflFieldObject*>(&typed),
                      reinterpret_cast<UnkReflValueArg*>(&value));
        return true;
    }
    return false;
}

// A string's data: its key (CRC-64) at +0x30.
struct UnkHydrogigaAttackStrData_ovb0114 {
    u8 pad_0x0[0x30];
    u64 key;
};

struct UnkHydrogigaAttack_ovb0114 {
    u8 pad_0x0[0x38];
    UnkHydrogigaAttackStrData_ovb0114* name;  // 0x38
};

f32 fn_71005acf10(UnkHydrogigaAttack_ovb0114* self) {
    bool enabled = true;
    UnkHydrogigaAttackObject_ovb0114* tunable = reinterpret_cast<UnkHydrogigaAttackObject_ovb0114*>(&g_unk_71726eb108);
    Get(tunable, self->name->key, &enabled);
    return enabled;
}
