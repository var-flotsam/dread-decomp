// CCharClassTweakPlayRateTrack's unit (tools/tu_map.py unit 2163): a float field of the track read by
// name through reflection (the field of its class whose name hash matches). Names and layouts are
// placeholders.
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflTypedPtr;

struct UnkReflField;
struct UnkReflFieldObject;
struct UnkReflValueArg;
bool fn_71000e78e0(UnkReflField* field, const UnkReflFieldObject* obj, UnkReflValueArg* value);  // gets

// A reflected field (0xb0 bytes).
struct UnkPlayRateField_ovb0106 {
    u8 pad_0x0[0xb0];
};

// A class as the lookup sees it: its fields and their name hashes.
struct UnkPlayRateClass_ovb0106 {
    u8 pad_0x0[0xe8];
    UnkPlayRateField_ovb0106* fields;  // 0xe8
    u64* keys;                        // 0xf0
    u32 count;                        // 0xf8

    UnkPlayRateField_ovb0106* End() { return fields + count; }
    UnkPlayRateField_ovb0106* Find(u64 key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &fields[i];
        return End();
    }
    // The field with that name hash, null when there is none.
    UnkPlayRateField_ovb0106* FindField(u64 key) {
        UnkPlayRateField_ovb0106* it = Find(key);
        return it != End() ? it : nullptr;
    }
};

// A reflected object: its class is vtable slot 0.
struct UnkPlayRateObject_ovb0106 {
    virtual UnkPlayRateClass_ovb0106* GetClass();
};

// A string's data: its key (CRC-64) at +0x30.
struct UnkPlayRateStrData_ovb0106 {
    u8 pad_0x0[0x30];
    u64 key;
};

struct UnkPlayRateValue_ovb0106 {
    const CType* type;
    void* value;
};

static inline bool Get(UnkPlayRateObject_ovb0106* obj, u64 key, f32* out) {
    UnkPlayRateClass_ovb0106* cls = obj->GetClass();
    UnkReflTypedPtr typed;
    typed.type = reinterpret_cast<char*>(obj->GetClass());
    typed.ptr = obj;
    UnkPlayRateValue_ovb0106 value = {fn_710000abbc(), out};
    if (UnkPlayRateField_ovb0106* field = cls->FindField(key)) {
        fn_71000e78e0(reinterpret_cast<UnkReflField*>(field), reinterpret_cast<const UnkReflFieldObject*>(&typed),
                      reinterpret_cast<UnkReflValueArg*>(&value));
        return true;
    }
    return false;
}

struct UnkPlayRateTrack_ovb0106 : UnkPlayRateObject_ovb0106 {
    u8 pad_0x8[0x1c];
    f32 field_0x24;
};

struct UnkPlayRateName_ovb0106 {
    UnkPlayRateStrData_ovb0106* data;
};

// The value of the track's field the name names; the value at +0x24 when there is no such field.
f32 fn_7100c61474(UnkPlayRateTrack_ovb0106* self, const UnkPlayRateName_ovb0106* name) {
    f32 v = self->field_0x24;
    Get(self, name->data->key, &v);
    return v;
}
