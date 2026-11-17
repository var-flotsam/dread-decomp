#pragma once

// The record a resource type is registered with (0x90 bytes): base::res's registry keeps an array of
// them (src/base/UnkResRegistry.cpp builds and copies one, fn_71000e9a7c and fn_71000ec89c);
// src/game/msapi/res/CRenderRes/fn_71001b1dd0.cpp fills them in, and src/base/res/CRes.cpp is the
// destructor's unit (fn_71000e8dd0). Names and layout are placeholders inferred from the code.

#include "base/UnkResPathVec.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

struct UnkResType;
struct UnkResFactory;

// The record's type and registry, set together (and copied as one block).
struct UnkResDescHead {
    const UnkResType* resource_type;
    void* registry;  // the registry's second base (+8), or null

    UnkResDescHead(const UnkResType* type, void* registry) : resource_type(type), registry(registry) {}
};

// A callback: a context and a function, copied member by member.
struct UnkResCallback {
    void* context;
    void (*function)();

    UnkResCallback(void* context, void (*function)()) : context(context), function(function) {}
    UnkResCallback(const UnkResCallback& other) : context(other.context), function(other.function) {}
};

// The record's list of file-path ids; destroying it releases the ids and frees the array.
struct UnkResDescPaths : UnkResPathVec {
    explicit UnkResDescPaths(u32 flags) : UnkResPathVec(flags) {}

    ~UnkResDescPaths() {
        if (data) {
            for (u32 i = 0; i < count; ++i)
                data[i].~CFilePathStrId();
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

struct UnkResDesc : UnkResDescHead {
    base::global::CStrId name;                 // 0x10
    base::global::CFilePathStrId field_0x18;
    base::global::CFilePathStrId field_0x20;
    UnkResCallback field_0x28;
    UnkResCallback field_0x38;
    UnkResFactory* factory;                    // 0x48
    u32 type;                                  // 0x50
    u64 field_0x58;
    UnkResDescPaths paths;                     // 0x60
    base::global::CFilePathStrId field_0x88;

    UnkResDesc(const UnkResType* type, UnkResFactory* factory) asm("fn_71000e9a7c");  // src/base/UnkResRegistry.cpp
    ~UnkResDesc() asm("fn_71000e8dd0");                                               // src/base/res/CRes.cpp
};
static_assert(sizeof(UnkResDesc) == 0x90, "UnkResDesc layout");
