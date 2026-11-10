// CCharClassCooldownXBossAttack's unit (tools/tu_map.py): a bool tunable of the Cooldown X boss,
// read by the attack's name through reflection (the field whose name hash matches), as 1.0 or 0.0;
// 1.0 when there is no such field. Names and layouts are placeholders.
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflTypedPtr;

struct UnkReflField;
struct UnkReflFieldObject;
struct UnkReflValueArg;
bool fn_71000e78e0(UnkReflField* field, const UnkReflFieldObject* obj, UnkReflValueArg* value);  // gets

extern base::reflection::UnkTunable g_unk_71726d8260;

// A reflected field (0xb0 bytes).
struct UnkCooldownXAttackField_ovb0114 {
    u8 pad_0x0[0xb0];
};

// A class as the lookup sees it: its fields and their name hashes.
struct UnkCooldownXAttackClass_ovb0114 {
    u8 pad_0x0[0xe8];
    UnkCooldownXAttackField_ovb0114* fields;  // 0xe8
    u64* keys;                            // 0xf0
    u32 count;                            // 0xf8

    UnkCooldownXAttackField_ovb0114* End() { return fields + count; }
    UnkCooldownXAttackField_ovb0114* Find(u64 key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &fields[i];
        return End();
    }
    // The field with that name hash, null when there is none.
    UnkCooldownXAttackField_ovb0114* FindField(u64 key) {
        UnkCooldownXAttackField_ovb0114* it = Find(key);
        return it != End() ? it : nullptr;
    }
};

// A reflected object: its class is vtable slot 0.
struct UnkCooldownXAttackObject_ovb0114 {
    virtual UnkCooldownXAttackClass_ovb0114* GetClass();
};

struct UnkCooldownXAttackValue_ovb0114 {
    const CType* type;
    void* value;
};

static inline bool Get(UnkCooldownXAttackObject_ovb0114* obj, u64 key, bool* out) {
    UnkCooldownXAttackClass_ovb0114* cls = obj->GetClass();
    UnkReflTypedPtr typed;
    typed.type = reinterpret_cast<char*>(obj->GetClass());
    typed.ptr = obj;
    UnkCooldownXAttackValue_ovb0114 value = {fn_710000d228(), out};
    if (UnkCooldownXAttackField_ovb0114* field = cls->FindField(key)) {
        fn_71000e78e0(reinterpret_cast<UnkReflField*>(field), reinterpret_cast<const UnkReflFieldObject*>(&typed),
                      reinterpret_cast<UnkReflValueArg*>(&value));
        return true;
    }
    return false;
}

// A string's data: its key (CRC-64) at +0x30.
struct UnkCooldownXAttackStrData_ovb0114 {
    u8 pad_0x0[0x30];
    u64 key;
};

struct UnkCooldownXAttack_ovb0114 {
    u8 pad_0x0[0x38];
    UnkCooldownXAttackStrData_ovb0114* name;  // 0x38
};

f32 fn_71004a0af8(UnkCooldownXAttack_ovb0114* self) {
    bool enabled = true;
    UnkCooldownXAttackObject_ovb0114* tunable = reinterpret_cast<UnkCooldownXAttackObject_ovb0114*>(&g_unk_71726d8260);
    Get(tunable, self->name->key, &enabled);
    return enabled;
}
