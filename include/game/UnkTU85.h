#pragma once

// UnkTU85: functions of this translation unit that other files call (tools/tu_map.py unit 85,
// 0x71000e6ce0-0x71000e8728). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkClassEntry;
struct UnkClassEntryContext;
struct UnkElem_0xb0;
struct UnkRecordB0_b44a1;
struct UnkReflField;
struct UnkReflFieldObject;
struct UnkReflValueArg;
struct UnkVersionEntry;

// Gets the field's value.
bool fn_71000e78e0(UnkReflField* field, const UnkReflFieldObject* obj, UnkReflValueArg* value);

// Sets the field's value.
bool fn_71000e7a80(UnkReflField* field, const UnkReflFieldObject* obj, UnkReflValueArg* value);

void fn_71000e70c4(UnkRecordB0_b44a1* self);
bool fn_71000e6ce0(UnkVersionEntry* self, u16* major, u16* minor);
void fn_71000e6fd8(void* self, void* other);
void fn_71000e7b5c(UnkClassEntry* entry, UnkClassEntryContext* context);
void fn_71000e82a8(UnkElem_0xb0*, void*);
