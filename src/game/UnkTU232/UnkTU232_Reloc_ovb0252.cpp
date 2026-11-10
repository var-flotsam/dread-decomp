// Unit 232 (data/tu_map.csv, no reliable name): the release of a resource block with its two counted
// resources: the header's pointers turn into offsets from it. Names and layouts are placeholders.
#include "base/UnkCountedResource.h"
#include "types.h"

typedef u64 __attribute__((may_alias)) UnkAliasU64_b0252;

struct UnkRelocHeader_b0252 {
    u8 pad_0x0[8];
    UnkAliasU64_b0252 field_0x8;
    u8 pad_0x10[0x18];
    UnkAliasU64_b0252 field_0x28;
    UnkAliasU64_b0252 field_0x30;
};

struct UnkResourceRef_b0252 {
    UnkCountedResource* p;
    ~UnkResourceRef_b0252() {
        UnkCountedResource* r = p;
        if (r && --r->ref_count == 0)
            fn_710001737c(r);
    }
    void Reset() {
        UnkCountedResource* r = p;
        if (r) {
            if (--r->ref_count == 0)
                fn_710001737c(r);
            p = nullptr;
        }
    }
};

struct UnkRelocOwner_b0252 {
    bool linked;  // 0x0
    u8 pad_0x1[7];
    u8 pad_0x8[8];
    UnkRelocHeader_b0252* header;  // 0x10
    UnkResourceRef_b0252 first;    // 0x18
    UnkResourceRef_b0252 second;   // 0x20

    // Turns the header's pointers into offsets from the header and drops the resources.
    __attribute__((always_inline)) void Unlink() {
        if (header && linked) {
            u64 base = reinterpret_cast<u64>(header);
            if (u64 v = header->field_0x8) {
                v -= base;
                __builtin_memcpy(&header->field_0x8, &v, sizeof(v));
            }
            if (u64 v = header->field_0x30) {
                v -= base;
                __builtin_memcpy(&header->field_0x30, &v, sizeof(v));
            }
            if (u64 v = header->field_0x28) {
                v -= base;
                __builtin_memcpy(&header->field_0x28, &v, sizeof(v));
            }
            second.Reset();
            first.Reset();
            linked = false;
        }
    }
};

// Declared with this type in src/game/msapi/res/CRenderRes/CRenderRes_Data_ovb0076.cpp.
struct UnkRenderResData_ovb0076;

void fn_71001aae7c(UnkRenderResData_ovb0076* data) {
    UnkRelocOwner_b0252* self = reinterpret_cast<UnkRelocOwner_b0252*>(data);
    self->Unlink();
    self->second.~UnkResourceRef_b0252();
    self->first.~UnkResourceRef_b0252();
}
