// CActorLayer's reflection (from unit 346 of tools/tu_map.py, 0x7100257370-0x7100261c94, whose static
// initialiser src/base/reflection/fn_7100261530.cpp registers the class with this function): an
// actor layer is a dictionary of sublayers ("dctSublayers", keyed by their "sName") and one of actor
// groups ("dctActorGroups"), with the functions that create and delete a sublayer by name and the
// hooks run after each part is read. The keys' strings are their getters' (base/reflection/
// UnkReflAttributes.h names the shared ones); names and layouts are placeholders.
// Holds fn_71002584a0 (CActorLayer::refl_InitClass's body).
#include "../game_s7/UnkStaticInitS7.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/UnkTUInit.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::CClass;
using base::reflection::CType;
using base::reflection::UnkReflAttrKey;
using base::reflection::UnkReflField;
using base::reflection::UnkReflFunction;
using unk_static_init::UnkEnumType;
using unk_static_init::UnkNamedValueTmp;

// A reflected value of any type (its storage and its type's hooks, as UnkNamedValueTmp lays them
// out), set up from a string id by the type's +0x38 hook and torn down by its +0x40 hook.
struct UnkReflAnyValue {
    u64 storage;
    unk_static_init::UnkValueType* type;

    UnkReflAnyValue(const CStrId& from, const CType* t) {
        auto* hooks = reinterpret_cast<unk_static_init::UnkValueType*>(const_cast<CType*>(t));
        auto init = hooks->init;
        type = hooks;
        if (init)
            init(this, &from);
    }
    ~UnkReflAnyValue() {
        if (type) {
            type->destroy(this);
            type = nullptr;
        }
    }
};

// A list of such values (base::UnkVector's layout), built from one value by fn_7100ad3404.
struct UnkReflAnyList {
    UnkReflAnyValue* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    // The list of the one value `first` (fn_7100ad3404, which keeps the placeholder types
    // src/game_s7/fn_7100f7b200.cpp gives it).
    explicit UnkReflAnyList(UnkReflAnyValue&& first);
    ~UnkReflAnyList() {
        if (data) {
            for (u64 i = 0; i < count; ++i)
                data[i].~UnkReflAnyValue();
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

struct UnkStruct_71729e0ab0;
void fn_7100ad3404(UnkStruct_71729e0ab0* self, UnkNamedValueTmp* value, bool flag);
inline UnkReflAnyList::UnkReflAnyList(UnkReflAnyValue&& first) {
    fn_7100ad3404(reinterpret_cast<UnkStruct_71729e0ab0*>(this), reinterpret_cast<UnkNamedValueTmp*>(&first), true);
}

// The types: the sublayer dictionary (built in this unit), the actor group dictionary, and two
// whose values tag the fields' attributes (roles unknown).
extern UnkReflType g_unk_71726a0628;
extern UnkEnumType g_unk_717269c300;
extern UnkReflType g_unk_7172756f90;
extern UnkReflType g_unk_71727bc110;
UnkReflType* fn_710025d654();  // the list type of the class attribute
static inline const CType* AnyListType() { return reinterpret_cast<const CType*>(fn_710025d654()); }

// Attribute keys used only here (their strings are unknown).
const base::reflection::UnkReflAttrKey& fn_710025d550();
const base::reflection::UnkReflAttrKey& fn_710008fd60();
const base::reflection::UnkReflAttrKey& fn_710008fb40();
const base::reflection::UnkReflAttrKey& fn_71000901a0();
const base::reflection::UnkReflAttrKey& fn_7100234780();
const base::reflection::UnkReflAttrKey& fn_7100092b10();
const base::reflection::UnkReflAttrKey& fn_710008ff80();
const base::reflection::UnkReflAttrKey& fn_710025d990();
const base::reflection::UnkReflAttrKey& fn_710025daa0();
const base::reflection::UnkReflAttrKey& fn_710025dbb0();
const base::reflection::UnkReflAttrKey& fn_710025dcc0();
const base::reflection::UnkReflAttrKey& fn_710008f920();

// The getters' invokers and the functions' set-up (each given the function's name).
void fn_7100260928();
void fn_7100260950();
void fn_710025915c(UnkReflFunction* fn, const char* name);
void fn_71002592d0(UnkReflFunction* fn, const char* name);
void fn_7100259444(UnkReflFunction* fn, const char* name);
void fn_71002595c4(UnkReflFunction* fn, const char* name);
void fn_710025973c(UnkReflFunction* fn, const char* name);

static inline const CType* TypeOf(UnkReflType& t) { return reinterpret_cast<const CType*>(&t); }
static inline const CType* TypeOf(UnkEnumType& t) { return reinterpret_cast<const CType*>(&t); }

// The getter function of a dictionary member, returning `type` through `invoker`.
static inline void AddGetter(CClass* cls, const char* name, const CType* type, void (*invoker)()) {
    if (UnkReflFunction* fn = fn_7100097550(cls, CStrId(name, true))) {
        fn->name = base::global::UnkStrId(name, true);
        fn->return_type = type;
        fn->field_0xb2 = 1;
        fn->field_0x40 = nullptr;
        fn->invoker = reinterpret_cast<void*>(invoker);
    }
}

// A function that takes a sublayer's name (parameter 0, tagged "sName").
static inline void AddByNameFunction(CClass* cls, const char* name, void (*setup)(UnkReflFunction*, const char*),
                                     const char* description) {
    if (UnkReflFunction* fn = fn_7100097550(cls, CStrId(name, true))) {
        setup(fn, name);
        fn->Attrs()->Add<const char*>(fn_7100007e80(), static_cast<const char*>(description));  // a fresh temporary
        fn_710009ae5c(fn, fn->params.count);
        fn->ParamAttrs(0)->Add<CStrId>(fn_710008f920(), CStrId("sName", true));
    }
}

// A hook function with no attributes.
static inline void AddHook(CClass* cls, const char* name, void (*setup)(UnkReflFunction*, const char*)) {
    if (UnkReflFunction* fn = fn_7100097550(cls, CStrId(name, true)))
        setup(fn, name);
}

void fn_71002584a0(CClass* cls) {
    // A class attribute: a list holding one string id, "sName".
    fn_710009dd6c(cls)->AddTyped<UnkReflAnyList, AnyListType>(
        fn_710025d550(), UnkReflAnyList(UnkReflAnyValue(CStrId("sName", true), fn_7100009658())));

    AddGetter(cls, "_m_dctSublayers_GetterFunction", TypeOf(g_unk_71726a0628), fn_7100260928);
    UnkReflField* sublayers = fn_7100096524(cls, CStrId("dctSublayers", true), TypeOf(g_unk_71726a0628), 0xffffffff,
                                            cls->m_array_0x110.size() - 1, -1);
    sublayers->attrs.Add<CStrId>(fn_710008fd60(), CStrId("CreateNewSublayer", true));
    sublayers->attrs.Add<CStrId>(fn_710008fb40(), CStrId("DeleteSublayerByName", true));
    sublayers->attrs.Add<CStrId>(fn_71000901a0(), CStrId("sName", true));
    sublayers->attrs.Add<bool>(fn_7100234780(), true);
    sublayers->attrs.Add<bool>(fn_7100092b10(), true);
    sublayers->attrs_0x70.Add<const char*>(fn_710008ff80(), "AfterSublayerDeserialization");
    sublayers->attrs_0x70.Add<s32>(fn_710025d990(), TypeOf(g_unk_7172756f90), 2);

    AddGetter(cls, "_m_dctActorGroups_GetterFunction", TypeOf(g_unk_717269c300), fn_7100260950);
    UnkReflField* groups = fn_7100096524(cls, CStrId("dctActorGroups", true), TypeOf(g_unk_717269c300), 0xffffffff,
                                         cls->m_array_0x110.size() - 1, -1);
    groups->attrs.Add<const char*>(fn_710008ff80(), "AfterActorGroupDeserialization");
    groups->attrs.Add<s32>(fn_710025d990(), TypeOf(g_unk_7172756f90), 2);
    groups->attrs_0x70.Add<u32>(fn_710025daa0(), TypeOf(g_unk_71727bc110), 0);
    groups->attrs_0x70.Add<bool>(fn_710025dbb0(), true);
    groups->attrs_0x70.Add<const char*>(fn_710008ff80(), "AfterActorGroupLinkDeserialization");
    groups->attrs_0x70.Add<s32>(fn_710025dcc0(), TypeOf(g_unk_7172756f90), 1);

    AddHook(cls, "AfterActorGroupDeserialization", fn_710025915c);
    AddHook(cls, "AfterActorGroupLinkDeserialization", fn_71002592d0);
    AddByNameFunction(cls, "CreateNewSublayer", fn_7100259444, "Create Sublayer");
    AddByNameFunction(cls, "DeleteSublayerByName", fn_71002595c4, "Delete Sublayer");
    AddHook(cls, "AfterSublayerDeserialization", fn_710025973c);
}
