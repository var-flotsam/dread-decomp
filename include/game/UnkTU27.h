#pragma once

// UnkTU27: functions and globals of this translation unit that other files use (tools/tu_map.py unit 27,
// 0x710001fa30-0x71000233dc). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

struct UnkAssetResRef;
struct UnkStrIdPathPair;
struct UnkEntry_7100d92158;
struct UnkFactoryEntry;
struct UnkRegistryKey;
struct UnkRegistry_7100d92158;

// One of a record's two default callbacks: each forwards its second argument (fn_710001a0ac, fn_710001a39c).
void fn_71000204c4(void* context, void* arg);

// One of a record's two default callbacks: each forwards its second argument (fn_710001a0ac, fn_710001a39c).
void fn_71000204cc(void* context, void* arg);

void fn_710002002c(UnkRegistry_7100d92158* registry, const UnkEntry_7100d92158& entry);
void fn_7100020164(char* self);  // The entry's destructor.
UnkFactoryEntry* fn_7100020320(UnkRegistry_7100d92158* registry, const UnkRegistryKey* key);

// The resource the ids name, looked up in the registry (the flag's and the last argument's meanings are unknown; the
// last is a 64-bit word).
UnkAssetResRef MakeResourceFromRegistryFactory(UnkRegistry_7100d92158* registry, const UnkStrIdPathPair* ids, bool a, void* b);
void ReserveCountedRefVector(base::global::CRntVector<u8>* vector, u32 capacity);
extern base::global::CStrId g_unk_7101d4e590;
