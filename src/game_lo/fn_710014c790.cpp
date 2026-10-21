// Destructor of an unidentified polymorphic object: unregisters it from a global registry
// (keyed by its string id) when registered, resets its sub-blocks, then releases the id.
#include "base/UnkDestructors.h"
#include "types.h"

extern void* unk_7101a1d2c0[];  // vtable (reached through the GOT)

struct UnkRegistry_710014c790;
extern UnkRegistry_710014c790* unk_710b372d68;  // the registry (reached through the GOT)

void fn_7100169f34(UnkRegistry_710014c790* registry, base::UnkStrIdMember* key);
void fn_710014d9e8(void* block, u32 arg);
void fn_710014d838(void* block, u32 arg);

struct UnkStruct_710014c790 {
    void** vtable;
    u8 pad_0x8[0x28];
    base::UnkStrIdMember id;
    void* handle;
    u32 field_0x40;
    u8 pad_0x44[0x100];
    u8 field_0x144[0x584];
    u32 field_0x6c8;
    u8 pad_0x6cc[0x100];
    u8 field_0x7cc[0x440];
    bool field_0xc0c;

    __attribute__((always_inline)) ~UnkStruct_710014c790() {
        vtable = &unk_7101a1d2c0[2];
        if (handle) {
            fn_7100169f34(unk_710b372d68, &id);
            handle = nullptr;
        }
        field_0x40 = 0;
        fn_710014d9e8(field_0x144, 0);
        field_0xc0c = false;
        fn_710014d838(field_0x7cc, 0);
        field_0x6c8 = 0;
        fn_710014d9e8(field_0x144, 0);
        field_0x40 = 0;
    }
};

void fn_710014c790(UnkStruct_710014c790* self) {
    self->~UnkStruct_710014c790();
}
