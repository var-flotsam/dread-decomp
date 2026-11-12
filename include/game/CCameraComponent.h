#pragma once

// CCameraComponent: functions of this translation unit that other files call (tools/tu_map.py unit 2691,
// 0x71010df6c0-0x71010e7f8c). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/reflection/CClass.h"
#include "base/global/CStrId.h"
#include "types.h"

struct UnkCamComp2_ovb5;
struct UnkCamEntry_ovb5;
struct UnkCameraCtrl;
UnkCameraCtrl* fn_71010e389c(void* holder, void* key);
void fn_71010e41e0(UnkCamComp2_ovb5* self, UnkCamEntry_ovb5* entry, bool flag);
base::reflection::UnkReflObject* fn_71010e37f4(void* camera);
void fn_71010e4168(void* holder, const base::global::CStrId* id, bool flag);
