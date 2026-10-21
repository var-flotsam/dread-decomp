// Sets the reflected value stored under a key: when the key is present and its current value
// accepts the new one in place, nothing else happens; otherwise the value is stored through the
// map's insert, the owner is marked dirty and its listener is notified.
#include "types.h"

struct UnkTypedRef_7100332c80 {
    const char* type;
    void* ptr;
};

struct UnkReflValue_7100332c80 {
    void* field_0x0;
    const char* type;  // its size is at +0x18
    u32 kind;
    u32 field_0x14;  // 1: stored through field_0x0
};

struct UnkDirtyFlag_7100332c80 {
    u8 pad[0x60];
    u8 field_0x60;
};

class UnkListener_7100332c80 {
public:
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38(void* owner, const u64* key, UnkTypedRef_7100332c80* value);
};

struct UnkValueMap_7100332c80 {
    u64 field_0x0;
};

struct UnkKeyedValues_7100332c80 {
    void* vtable;
    UnkListener_7100332c80* listener;
    UnkDirtyFlag_7100332c80* field_0x10;
    UnkValueMap_7100332c80 map;  // +0x18
    UnkReflValue_7100332c80* values;
    u64* keys;
    u32 count;
    u8 pad_0x34[0x50 - 0x34];
    u8 dirty;

    UnkReflValue_7100332c80* End() { return &values[count]; }
    UnkReflValue_7100332c80* Find(const u64* key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == *key)
                return &values[i];
        return End();
    }
};

void fn_71000a16e0(UnkTypedRef_7100332c80* out, const char* type, void* data);
void fn_71000a1760(UnkTypedRef_7100332c80* out, const UnkTypedRef_7100332c80* in);
bool fn_710009d5d4(const char* type, UnkTypedRef_7100332c80* dst, const UnkTypedRef_7100332c80* src);
void fn_7100332dc4(UnkValueMap_7100332c80* map, const u64* key, const UnkTypedRef_7100332c80* value);

void fn_7100332c80(UnkKeyedValues_7100332c80* self, const u64* key, const UnkTypedRef_7100332c80* value) {
    UnkTypedRef_7100332c80 dst;
    UnkTypedRef_7100332c80 copy;
    UnkReflValue_7100332c80* v = self->Find(key);
    if (v != self->End()) {
        const char* type = v->type;
        if (type) {
            void* p = v->field_0x14 == 1 || *reinterpret_cast<const u64*>(type + 0x18) > 8 ? v->field_0x0 : v;
            if (p) {
                fn_71000a16e0(&dst, type, p);
                if (fn_710009d5d4(type, &dst, value))
                    return;
            }
        }
    }
    fn_7100332dc4(&self->map, key, value);
    self->dirty = 1;
    if (self->field_0x10)
        self->field_0x10->field_0x60 = 1;
    UnkListener_7100332c80* l = self->listener;
    fn_71000a1760(&copy, value);
    l->vf_0x38(self, key, &copy);
}
