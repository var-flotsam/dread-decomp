// base::reflection::CVariable's unit (tools/tu_map.py unit 86): fn_71000e88c0 builds the class descriptor
// g_unk_7101d67598 in place (named "base::reflection::CVariable", the name's constructor inlined, no parent,
// fn_71000e8728 as the member-field function; the descriptor constructor fn_71000e872c is instantiated in
// src/reflection/class_descriptors_00.cpp), registers its destructor and adds it to the unit's type list.
// include/base/reflection/UnkClassGetters.h lists it as the class getter returning the descriptor; the
// original returns nothing. Names and layouts are placeholders.
#include <new>

#include "base/UnkAtExit.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkReflTypeDtors.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::CClass;

DREAD_REFL_TAG(71000e872c);
extern template CClass::CClass(const CStrId&, const CClass*, base::reflection::UnkFn_0x70,
                               base::reflection::UnkFn_0x70, UnkRefl_fn_71000e872c*);

void fn_71000e8728(CClass*);
extern CClass g_unk_7101d67598;
extern u8 g_unk_7101d676d0[];

void fn_71000e88c0() {
    {
        CStrId name(base::global::UnkStrIdInline{}, "base::reflection::CVariable", true);
        new (&g_unk_7101d67598) CClass(name, nullptr, nullptr, &fn_71000e8728, static_cast<UnkRefl_fn_71000e872c*>(nullptr));
    }
    fn_7100000250(fn_7100096604, &g_unk_7101d67598, &g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d676d0, &g_unk_7101d67598);
}
