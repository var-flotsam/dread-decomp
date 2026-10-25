// Static initialiser of the unit of base::global::CPooledList<CEntity*>,
// base::global::CRntVector<base::global::CWeakPtr<CEntity>>, base::global::CRntVector<CEntity*>
// and others: the header ids, then the unit's own statics and registrations (helpers in
// UnkStaticInitS7.h). Layouts are placeholders.
#include "UnkStaticInitS7.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkTUInit.h"

using namespace unk_static_init;

struct UnkStatics_7100d8a790 {
    HeaderIds ids;
    u8 pad_0x18[0x10];
    f32 field_0x28;
    f32 field_0x2c;
    bool field_0x30;
    u8 pad_0x31[0x77];
    u8 field_0xa8[8];
};
extern UnkStatics_7100d8a790 g_unk_71727b85d8 __attribute__((visibility("hidden")));
extern char g_unk_71727b5288[];
extern char g_unk_71727b5289[];
UnkEnumType* fn_7100a0536c();
extern char g_unk_71727b528a[];
UnkEnumType* fn_7100407170();
extern char g_unk_71727b528b[];
UnkEnumType* fn_7100d71e14();
extern char g_unk_71727b528c[];
UnkEnumType* fn_7100d71ef0();
extern UnkEnumType g_unk_71727b5290;
void fn_7100d72350(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_7100096604(void* type);  // the type's destructor, as registered
extern char g_unk_7101d5e2e0[];
void fn_7100d71fcc();
extern char g_unk_71727b53c8[];
extern char g_unk_71727b53c9[];
UnkEnumType* fn_7100d724d4();
extern UnkEnumType g_unk_71727b53d0;
void fn_7100d72934(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_7100d725b0();
extern char g_unk_71727b5508[];
extern char g_unk_71727b5509[];
UnkEnumType* fn_7100d72ab8();
extern UnkEnumType g_unk_71727b5510;
void fn_7100d72b94(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_71727b5680[];
extern char g_unk_71727b5681[];
UnkEnumType* fn_7100d72d7c();
extern char g_unk_71727b5682[];
UnkEnumType* fn_7100d72e58();
extern UnkEnumType g_unk_71727b5688;
void fn_7100d732b8(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_7100d72f34();
extern char g_unk_71727b57c0[];
extern char g_unk_71727b57c1[];
UnkEnumType* fn_7100d7343c();
extern UnkEnumType g_unk_71727b57c8;
void fn_7100d7389c(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_7100d73518();
extern char g_unk_71727b5900[];
extern char g_unk_71727b5901[];
UnkEnumType* fn_7100d73a20();
extern UnkEnumType g_unk_71727b5908;
void fn_7100d73afc(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_71727b5a78[];
extern char g_unk_71727b5a79[];
UnkEnumType* fn_7100d73cec();
extern char g_unk_71727b5a7a[];
UnkEnumType* fn_7100d73dc8();
extern UnkEnumType g_unk_71727b5a80;
void fn_7100d74228(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_7100d73ea4();
extern char g_unk_71727b5bb8[];
extern char g_unk_71727b5bb9[];
UnkEnumType* fn_7100d743ac();
extern UnkEnumType g_unk_71727b5bc0;
void fn_7100d7480c(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_7100d74488();
extern char g_unk_71727b5cf8[];
extern char g_unk_71727b5cf9[];
UnkEnumType* fn_7100d74990();
extern UnkEnumType g_unk_71727b5d00;
void fn_7100d74a6c(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_71727b5e70[];
extern char g_unk_71727b5e71[];
UnkEnumType* fn_7100d74c5c();
extern char g_unk_71727b5e72[];
UnkEnumType* fn_7100d74d38();
extern UnkEnumType g_unk_71727b5e78;
void fn_7100d75198(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_7100d74e14();
extern char g_unk_71727b5fb0[];
extern char g_unk_71727b5fb1[];
UnkEnumType* fn_7100d7531c();
extern UnkEnumType g_unk_71727b5fb8;
void fn_7100d7577c(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_7100d753f8();
extern char g_unk_71727b60f0[];
extern char g_unk_71727b60f1[];
UnkEnumType* fn_7100d75900();
extern UnkEnumType g_unk_71727b60f8;
void fn_7100d759dc(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_71727b6268[];
extern UnkEnumType g_unk_71727b6270;
void fn_7100d75c70(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
extern char g_unk_71727bbe68[];
void fn_7100d75bd0();
extern char g_unk_71727b63a8[];
extern char g_unk_71727b63a9[];
UnkEnumType* fn_7100d75f70();
extern char g_unk_71727b63aa[];
UnkEnumType* fn_7100d7604c();
extern UnkEnumType g_unk_71727b63b0;
void fn_7100d764ac(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_7100d76128();
extern char g_unk_71727b64e8[];
extern char g_unk_71727b64e9[];
UnkEnumType* fn_7100d76630();
extern UnkEnumType g_unk_71727b64f0;
void fn_7100d76a90(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_7100d7670c();
extern char g_unk_71727b6628[];
extern char g_unk_71727b6629[];
UnkEnumType* fn_7100d76c14();
extern UnkEnumType g_unk_71727b6630;
void fn_7100d76cf0(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_71727b67a0[];
extern char g_unk_71727b67a1[];
UnkEnumType* fn_7100d76ee4();
extern char g_unk_71727b67a2[];
UnkEnumType* fn_7100d76fc0();
extern UnkEnumType g_unk_71727b67a8;
void fn_7100d77420(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_7100d7709c();
extern char g_unk_71727b68e0[];
extern char g_unk_71727b68e1[];
UnkEnumType* fn_7100d775a4();
extern UnkEnumType g_unk_71727b68e8;
void fn_7100d77a04(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_7100d77680();
extern char g_unk_71727b6a20[];
extern char g_unk_71727b6a21[];
UnkEnumType* fn_7100d77b88();
extern UnkEnumType g_unk_71727b6a28;
void fn_7100d77c64(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_71727b6b98[];
extern UnkConversion g_unk_71727b6ba0;
void fn_7100d8a39c();
extern UnkConversion g_unk_71727b6bc0;
void fn_7100d8a538();
UnkEnumType* fn_7100231c1c();
extern UnkNamedValue g_unk_71727b6be0;

void fn_7100d8a790() {
    InitHeaderIds(&g_unk_71727b85d8.ids);
    g_unk_71727b85d8.field_0x28 = -1.0f;
    g_unk_71727b85d8.field_0x2c = -1.0f;
    g_unk_71727b85d8.field_0x30 = true;
    fn_7100d70c20();
    fn_710009ec00(g_unk_71727b5288, g_unk_71727b85d8.field_0xa8);
    fn_710009ec00(g_unk_71727b5289, fn_7100a0536c());
    fn_710009ec00(g_unk_71727b528a, fn_7100407170());
    fn_710009ec00(g_unk_71727b528b, fn_7100d71e14());
    fn_710009ec00(g_unk_71727b528c, fn_7100d71ef0());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CPooledList<CEntity*>>::TKeyElementIterator::value_type", true);
        fn_7100d72350(&g_unk_71727b5290, n, g_unk_7101d5e2e0, nullptr, fn_7100d71fcc, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727b5290, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727b53c8, &g_unk_71727b5290);
    fn_710009ec00(g_unk_71727b53c9, fn_7100d724d4());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CPooledList<CEntity*>>::TKeyElementConstIterator::value_type", true);
        fn_7100d72934(&g_unk_71727b53d0, n, g_unk_7101d5e2e0, nullptr, fn_7100d725b0, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727b53d0, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727b5508, &g_unk_71727b53d0);
    fn_710009ec00(g_unk_71727b5509, fn_7100d72ab8());
    {
        CStrId n("base::global::CPooledList<CEntity*>", true);
        fn_7100d72b94(&g_unk_71727b5510, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_71727b5510, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727b5680, &g_unk_71727b5510);
    fn_710009ec00(g_unk_71727b5681, fn_7100d72d7c());
    fn_710009ec00(g_unk_71727b5682, fn_7100d72e58());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<base::global::CWeakPtr<CEntity>>>::TKeyElementIterator::value_type", true);
        fn_7100d732b8(&g_unk_71727b5688, n, g_unk_7101d5e2e0, nullptr, fn_7100d72f34, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727b5688, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727b57c0, &g_unk_71727b5688);
    fn_710009ec00(g_unk_71727b57c1, fn_7100d7343c());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<base::global::CWeakPtr<CEntity>>>::TKeyElementConstIterator::value_type", true);
        fn_7100d7389c(&g_unk_71727b57c8, n, g_unk_7101d5e2e0, nullptr, fn_7100d73518, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727b57c8, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727b5900, &g_unk_71727b57c8);
    fn_710009ec00(g_unk_71727b5901, fn_7100d73a20());
    {
        CStrId n("base::global::CRntVector<base::global::CWeakPtr<CEntity>>", true);
        fn_7100d73afc(&g_unk_71727b5908, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_71727b5908, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727b5a78, &g_unk_71727b5908);
    fn_710009ec00(g_unk_71727b5a79, fn_7100d73cec());
    fn_710009ec00(g_unk_71727b5a7a, fn_7100d73dc8());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<CEntity*>>::TKeyElementIterator::value_type", true);
        fn_7100d74228(&g_unk_71727b5a80, n, g_unk_7101d5e2e0, nullptr, fn_7100d73ea4, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727b5a80, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727b5bb8, &g_unk_71727b5a80);
    fn_710009ec00(g_unk_71727b5bb9, fn_7100d743ac());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<CEntity*>>::TKeyElementConstIterator::value_type", true);
        fn_7100d7480c(&g_unk_71727b5bc0, n, g_unk_7101d5e2e0, nullptr, fn_7100d74488, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727b5bc0, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727b5cf8, &g_unk_71727b5bc0);
    fn_710009ec00(g_unk_71727b5cf9, fn_7100d74990());
    {
        CStrId n("base::global::CRntVector<CEntity*>", true);
        fn_7100d74a6c(&g_unk_71727b5d00, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_71727b5d00, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727b5e70, &g_unk_71727b5d00);
    fn_710009ec00(g_unk_71727b5e71, fn_7100d74c5c());
    fn_710009ec00(g_unk_71727b5e72, fn_7100d74d38());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntSmallDictionary<base::global::CStrId, CEntity*>>::TKeyElementIterator::value_type", true);
        fn_7100d75198(&g_unk_71727b5e78, n, g_unk_7101d5e2e0, nullptr, fn_7100d74e14, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727b5e78, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727b5fb0, &g_unk_71727b5e78);
    fn_710009ec00(g_unk_71727b5fb1, fn_7100d7531c());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntSmallDictionary<base::global::CStrId, CEntity*>>::TKeyElementConstIterator::value_type", true);
        fn_7100d7577c(&g_unk_71727b5fb8, n, g_unk_7101d5e2e0, nullptr, fn_7100d753f8, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727b5fb8, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727b60f0, &g_unk_71727b5fb8);
    fn_710009ec00(g_unk_71727b60f1, fn_7100d75900());
    {
        CStrId n("base::global::CRntSmallDictionary<base::global::CStrId, CEntity*>", true);
        fn_7100d759dc(&g_unk_71727b60f8, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_71727b60f8, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727b6268, &g_unk_71727b60f8);
    {
        CStrId n("CGameLink<CEntity>", true);
        fn_7100d75c70(&g_unk_71727b6270, n, g_unk_71727bbe68, nullptr, fn_7100d75bd0, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727b6270, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727b63a8, &g_unk_71727b6270);
    fn_710009ec00(g_unk_71727b63a9, fn_7100d75f70());
    fn_710009ec00(g_unk_71727b63aa, fn_7100d7604c());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<CGameLink<CEntity>>>::TKeyElementIterator::value_type", true);
        fn_7100d764ac(&g_unk_71727b63b0, n, g_unk_7101d5e2e0, nullptr, fn_7100d76128, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727b63b0, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727b64e8, &g_unk_71727b63b0);
    fn_710009ec00(g_unk_71727b64e9, fn_7100d76630());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<CGameLink<CEntity>>>::TKeyElementConstIterator::value_type", true);
        fn_7100d76a90(&g_unk_71727b64f0, n, g_unk_7101d5e2e0, nullptr, fn_7100d7670c, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727b64f0, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727b6628, &g_unk_71727b64f0);
    fn_710009ec00(g_unk_71727b6629, fn_7100d76c14());
    {
        CStrId n("base::global::CRntVector<CGameLink<CEntity>>", true);
        fn_7100d76cf0(&g_unk_71727b6630, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_71727b6630, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727b67a0, &g_unk_71727b6630);
    fn_710009ec00(g_unk_71727b67a1, fn_7100d76ee4());
    fn_710009ec00(g_unk_71727b67a2, fn_7100d76fc0());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntDictionary<base::global::CStrId, base::global::CRntVector<CGameLink<CEntity>>>>::TKeyElementIterator::value_type", true);
        fn_7100d77420(&g_unk_71727b67a8, n, g_unk_7101d5e2e0, nullptr, fn_7100d7709c, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727b67a8, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727b68e0, &g_unk_71727b67a8);
    fn_710009ec00(g_unk_71727b68e1, fn_7100d775a4());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntDictionary<base::global::CStrId, base::global::CRntVector<CGameLink<CEntity>>>>::TKeyElementConstIterator::value_type", true);
        fn_7100d77a04(&g_unk_71727b68e8, n, g_unk_7101d5e2e0, nullptr, fn_7100d77680, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727b68e8, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727b6a20, &g_unk_71727b68e8);
    fn_710009ec00(g_unk_71727b6a21, fn_7100d77b88());
    {
        CStrId n("base::global::CRntDictionary<base::global::CStrId, base::global::CRntVector<CGameLink<CEntity>>>", true);
        fn_7100d77c64(&g_unk_71727b6a28, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_71727b6a28, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727b6b98, &g_unk_71727b6a28);
    InitConversion(&g_unk_71727b6ba0, TypeOf<fn_7100009658>, &g_unk_71727b6270, fn_7100d8a39c);
    InitConversion(&g_unk_71727b6bc0, fn_7100231c1c, &g_unk_71727b6270, fn_7100d8a538);
    InitNamedValue(&g_unk_71727b6be0, "oBBox", TypeOf<fn_7100009658>, fn_710010c424);
}
