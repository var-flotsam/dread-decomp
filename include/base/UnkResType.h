#pragma once

// A registered resource type (0x80 bytes): what a resource is named by in the registry
// (base/UnkResKey.h) and what a registry record is built from (base/UnkResDesc.h). Each is a
// static, built at start-up by fn_7100004698 from its name and a list of formats (as in
// src/base/animation/CAnimationResource/fn_7100005700.cpp) and destroyed at exit by fn_7100004b94
// (src/base/animation/CAnimationResource/CAnimationResource_Registry_ovb0033.cpp). Names are
// placeholders; the layout is the one fn_7100004698 fills in.

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

// A format a resource type loads (0x38 bytes): the class that loads it, its file extension and
// the extensions of its sources. fn_7100004578 builds one on the stack and fn_7100004a44 destroys
// it, both out of line, so the members are spelled without the string id's and the vector's
// constructors: two string ids (base::global::CStrId's layout) and an array of them
// (base::global::CRntVector's).
struct UnkResFormat {
    base::global::UnkStringData* class_name;
    base::global::UnkStringData* extension;  // 0x8
    base::global::UnkStringData** sources;   // 0x10
    u32 source_count;                        // 0x18
    u32 source_capacity;                     // 0x1c
    u32 source_step;                         // 0x20
    bool field_0x24;
    u32 source_flags;                      // 0x28
    base::UnkAllocator* source_allocator;  // 0x30
};
static_assert(sizeof(UnkResFormat) == 0x38, "UnkResFormat layout");

struct UnkResType {
    base::global::CStrId name;
    base::global::CRntVector<UnkResFormat> formats;             // 0x8
    base::global::CRntVector<base::global::CStrId> extensions;  // 0x30, each format's file extension
    base::global::CRntVector<base::global::CStrId> sources;     // 0x58, every format's source extensions
};
static_assert(sizeof(UnkResType) == 0x80, "UnkResType layout");

// The format's constructor.
void fn_7100004578(UnkResFormat* self, const char* class_name, const char* extension, const char* const* sources,
                   u32 count);
// Builds the resource type at `self` (a UnkResType) named `name` from `count` formats.
void fn_7100004698(void* self, const char* name, UnkResFormat* formats, u32 count);
void fn_7100004a44(UnkResFormat* self);  // the format's destructor
void fn_7100004b94(void* self);          // the resource type's destructor (an exit handler)
