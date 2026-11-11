#pragma once

// UnkTU27: functions of this translation unit that other files call (tools/tu_map.py unit 27,
// 0x710001fa30-0x71000233dc). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkEntry_7100d92158;
struct UnkFactoryEntry;
struct UnkRegistryKey;
struct UnkRegistry_7100d92158;
void fn_71000204c4(void* context, void* arg);
void fn_71000204cc(void* context, void* arg);
void fn_710002002c(UnkRegistry_7100d92158* registry, const UnkEntry_7100d92158& entry);
void fn_7100020164(char* self);
UnkFactoryEntry* fn_7100020320(UnkRegistry_7100d92158* registry, const UnkRegistryKey* key);
