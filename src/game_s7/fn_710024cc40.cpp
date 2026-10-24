// Static initialiser of the unit of base::global::CRntVector<CActor*>,
// base::global::CPooledList<CActor*>, base::global::CRntVector<base::global::CWeakPtr<CActor>> and
// others: the header ids, then the unit's own statics and registrations (helpers in
// UnkStaticInitS7.h). Layouts are placeholders.
#include "UnkStaticInitS7.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

struct UnkStatics_710024cc40 {
    HeaderIds ids;
    u8 pad_0x18[0x18];
    u8 field_0x30[8];
};
extern UnkStatics_710024cc40 g_unk_717269f420 __attribute__((visibility("hidden")));
extern char g_unk_717269a348[];
extern char g_unk_717269a349[];
UnkEnumType* fn_71002295ac();
extern char g_unk_717269a34a[];
UnkEnumType* fn_7100229688();
extern char g_unk_717269a34b[];
UnkEnumType* fn_7100229764();
extern char g_unk_717269a34c[];
UnkEnumType* fn_7100229840();
extern char g_unk_717269a34d[];
UnkEnumType* fn_710022991c();
extern UnkEnumType g_unk_717269a350;
void fn_7100229d7c(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_7100096604(void* type);  // the type's destructor, as registered
extern char g_unk_7101d5e2e0[];
void fn_71002299f8();
extern char g_unk_717269a488[];
extern char g_unk_717269a489[];
UnkEnumType* fn_7100229f00();
extern UnkEnumType g_unk_717269a490;
void fn_710022a360(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_7100229fdc();
extern char g_unk_717269a5c8[];
extern char g_unk_717269a5c9[];
UnkEnumType* fn_710022a4e4();
extern UnkEnumType g_unk_717269a5d0;
void fn_710022a5c0(UnkEnumType* type, const CStrId& name, void*, void*);
void fn_710009891c(void* type);  // the type's destructor, as registered
extern char g_unk_717269a740[];
extern char g_unk_717269a741[];
UnkEnumType* fn_710022a7b0();
extern char g_unk_717269a742[];
UnkEnumType* fn_710022a88c();
extern UnkEnumType g_unk_717269a748;
void fn_710022acec(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_710022a968();
extern char g_unk_717269a880[];
extern char g_unk_717269a881[];
UnkEnumType* fn_710022ae70();
extern UnkEnumType g_unk_717269a888;
void fn_710022b2d0(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_710022af4c();
extern char g_unk_717269a9c0[];
extern char g_unk_717269a9c1[];
UnkEnumType* fn_710022b454();
extern UnkEnumType g_unk_717269a9c8;
void fn_710022b530(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_717269ab38[];
extern char g_unk_717269ab39[];
UnkEnumType* fn_710022b718();
extern char g_unk_717269ab3a[];
UnkEnumType* fn_710022b7f4();
extern UnkEnumType g_unk_717269ab40;
void fn_710022bc54(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_710022b8d0();
extern char g_unk_717269ac78[];
extern char g_unk_717269ac79[];
UnkEnumType* fn_710022bdd8();
extern UnkEnumType g_unk_717269ac80;
void fn_710022c238(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_710022beb4();
extern char g_unk_717269adb8[];
extern char g_unk_717269adb9[];
UnkEnumType* fn_710022c3bc();
extern UnkEnumType g_unk_717269adc0;
void fn_710022c498(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_717269af30[];
extern char g_unk_717269af31[];
UnkEnumType* fn_710022c688();
extern char g_unk_717269af32[];
UnkEnumType* fn_710022c764();
extern UnkEnumType g_unk_717269af38;
void fn_710022cbc4(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_710022c840();
extern char g_unk_717269b070[];
extern char g_unk_717269b071[];
UnkEnumType* fn_710022cd48();
extern UnkEnumType g_unk_717269b078;
void fn_710022d1a8(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_710022ce24();
extern char g_unk_717269b1b0[];
extern char g_unk_717269b1b1[];
UnkEnumType* fn_710022d32c();
extern UnkEnumType g_unk_717269b1b8;
void fn_710022d408(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_717269b328[];
extern char g_unk_717269b329[];
UnkEnumType* fn_710022d5d8();
extern UnkConversion g_unk_717269b330;
void fn_7100243298();
extern char g_unk_717269b350[];
UnkEnumType* fn_710022d6b4();
extern char g_unk_717269b351[];
UnkEnumType* fn_710022d790();
extern UnkEnumType g_unk_717269b358;
void fn_710022dbf0(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_710022d86c();
extern char g_unk_717269b490[];
extern char g_unk_717269b491[];
UnkEnumType* fn_710022dd74();
extern UnkEnumType g_unk_717269b498;
void fn_710022e1d4(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_710022de50();
extern char g_unk_717269b5d0[];
extern char g_unk_717269b5d1[];
UnkEnumType* fn_710022e358();
extern UnkEnumType g_unk_717269b5d8;
void fn_710022e434(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_717269b748[];
extern char g_unk_717269b749[];
UnkEnumType* fn_710022e614();
extern char g_unk_717269b74a[];
UnkEnumType* fn_710022e6f0();
extern UnkEnumType g_unk_717269b750;
void fn_710022eb50(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_710022e7cc();
extern char g_unk_717269b888[];
extern char g_unk_717269b889[];
UnkEnumType* fn_710022ecd4();
extern UnkEnumType g_unk_717269b890;
void fn_710022f134(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_710022edb0();
extern char g_unk_717269b9c8[];
extern char g_unk_717269b9c9[];
UnkEnumType* fn_710022f2b8();
extern UnkEnumType g_unk_717269b9d0;
void fn_710022f394(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_717269bb40[];
extern UnkEnumType g_unk_717269bb48;
void fn_710022f608(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
extern char g_unk_71727bbe68[];
void fn_710022f568();
extern char g_unk_717269bc80[];
extern char g_unk_717269bc81[];
UnkEnumType* fn_710022f908();
extern char g_unk_717269bc82[];
UnkEnumType* fn_710022f9e4();
extern UnkEnumType g_unk_717269bc88;
void fn_710022fe44(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_710022fac0();
extern char g_unk_717269bdc0[];
extern char g_unk_717269bdc1[];
UnkEnumType* fn_710022ffc8();
extern UnkEnumType g_unk_717269bdc8;
void fn_7100230428(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_71002300a4();
extern char g_unk_717269bf00[];
extern char g_unk_717269bf01[];
UnkEnumType* fn_71002305ac();
extern UnkEnumType g_unk_717269bf08;
void fn_7100230688(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_717269c078[];
extern char g_unk_717269c079[];
UnkEnumType* fn_710023087c();
extern char g_unk_717269c07a[];
UnkEnumType* fn_7100230958();
extern UnkEnumType g_unk_717269c080;
void fn_7100230db8(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_7100230a34();
extern char g_unk_717269c1b8[];
extern char g_unk_717269c1b9[];
UnkEnumType* fn_7100230f3c();
extern UnkEnumType g_unk_717269c1c0;
void fn_710023139c(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_7100231018();
extern char g_unk_717269c2f8[];
extern char g_unk_717269c2f9[];
UnkEnumType* fn_7100231520();
extern UnkEnumType g_unk_717269c300;
void fn_71002315fc(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_717269c470[];
extern char g_unk_717269c471[];
UnkEnumType* fn_71002317d0();
extern char g_unk_717269c472[];
UnkEnumType* fn_71002318ac();
extern char g_unk_717269c473[];
UnkEnumType* fn_7100231988();
extern char g_unk_717269c474[];
UnkEnumType* fn_7100231a64();
extern UnkConversion g_unk_717269c478;
void fn_7100248b38();
extern UnkConversion g_unk_717269c498;
UnkEnumType* fn_7100231b40();
void fn_7100248cd4();
extern UnkConversion g_unk_717269c4b8;
void fn_710023de90() __attribute__((visibility("hidden")));
UnkEnumType* fn_7100231c1c();
extern UnkNamedValue g_unk_717269c4e0;
void fn_710010c424(void* value);  // its destructor, as registered
extern UnkNamedValue g_unk_717269c4f0;
extern UnkNamedValue g_unk_717269c500;

void fn_710024cc40() {
    InitHeaderIds(&g_unk_717269f420.ids);
    fn_7100227448();
    fn_710009ec00(g_unk_717269a348, g_unk_717269f420.field_0x30);
    fn_710009ec00(g_unk_717269a349, fn_71002295ac());
    fn_710009ec00(g_unk_717269a34a, fn_7100229688());
    fn_710009ec00(g_unk_717269a34b, fn_7100229764());
    fn_710009ec00(g_unk_717269a34c, fn_7100229840());
    fn_710009ec00(g_unk_717269a34d, fn_710022991c());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<CActor*>>::TKeyElementIterator::value_type", true);
        fn_7100229d7c(&g_unk_717269a350, n, g_unk_7101d5e2e0, nullptr, fn_71002299f8, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_717269a350, g_unk_7101a19000);
    fn_710009ec00(g_unk_717269a488, &g_unk_717269a350);
    fn_710009ec00(g_unk_717269a489, fn_7100229f00());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<CActor*>>::TKeyElementConstIterator::value_type", true);
        fn_710022a360(&g_unk_717269a490, n, g_unk_7101d5e2e0, nullptr, fn_7100229fdc, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_717269a490, g_unk_7101a19000);
    fn_710009ec00(g_unk_717269a5c8, &g_unk_717269a490);
    fn_710009ec00(g_unk_717269a5c9, fn_710022a4e4());
    {
        CStrId n("base::global::CRntVector<CActor*>", true);
        fn_710022a5c0(&g_unk_717269a5d0, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_717269a5d0, g_unk_7101a19000);
    fn_710009ec00(g_unk_717269a740, &g_unk_717269a5d0);
    fn_710009ec00(g_unk_717269a741, fn_710022a7b0());
    fn_710009ec00(g_unk_717269a742, fn_710022a88c());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CPooledList<CActor*>>::TKeyElementIterator::value_type", true);
        fn_710022acec(&g_unk_717269a748, n, g_unk_7101d5e2e0, nullptr, fn_710022a968, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_717269a748, g_unk_7101a19000);
    fn_710009ec00(g_unk_717269a880, &g_unk_717269a748);
    fn_710009ec00(g_unk_717269a881, fn_710022ae70());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CPooledList<CActor*>>::TKeyElementConstIterator::value_type", true);
        fn_710022b2d0(&g_unk_717269a888, n, g_unk_7101d5e2e0, nullptr, fn_710022af4c, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_717269a888, g_unk_7101a19000);
    fn_710009ec00(g_unk_717269a9c0, &g_unk_717269a888);
    fn_710009ec00(g_unk_717269a9c1, fn_710022b454());
    {
        CStrId n("base::global::CPooledList<CActor*>", true);
        fn_710022b530(&g_unk_717269a9c8, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_717269a9c8, g_unk_7101a19000);
    fn_710009ec00(g_unk_717269ab38, &g_unk_717269a9c8);
    fn_710009ec00(g_unk_717269ab39, fn_710022b718());
    fn_710009ec00(g_unk_717269ab3a, fn_710022b7f4());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<base::global::CWeakPtr<CActor>>>::TKeyElementIterator::value_type", true);
        fn_710022bc54(&g_unk_717269ab40, n, g_unk_7101d5e2e0, nullptr, fn_710022b8d0, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_717269ab40, g_unk_7101a19000);
    fn_710009ec00(g_unk_717269ac78, &g_unk_717269ab40);
    fn_710009ec00(g_unk_717269ac79, fn_710022bdd8());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<base::global::CWeakPtr<CActor>>>::TKeyElementConstIterator::value_type", true);
        fn_710022c238(&g_unk_717269ac80, n, g_unk_7101d5e2e0, nullptr, fn_710022beb4, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_717269ac80, g_unk_7101a19000);
    fn_710009ec00(g_unk_717269adb8, &g_unk_717269ac80);
    fn_710009ec00(g_unk_717269adb9, fn_710022c3bc());
    {
        CStrId n("base::global::CRntVector<base::global::CWeakPtr<CActor>>", true);
        fn_710022c498(&g_unk_717269adc0, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_717269adc0, g_unk_7101a19000);
    fn_710009ec00(g_unk_717269af30, &g_unk_717269adc0);
    fn_710009ec00(g_unk_717269af31, fn_710022c688());
    fn_710009ec00(g_unk_717269af32, fn_710022c764());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntDictionary<base::global::CStrId, CActor*>>::TKeyElementIterator::value_type", true);
        fn_710022cbc4(&g_unk_717269af38, n, g_unk_7101d5e2e0, nullptr, fn_710022c840, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_717269af38, g_unk_7101a19000);
    fn_710009ec00(g_unk_717269b070, &g_unk_717269af38);
    fn_710009ec00(g_unk_717269b071, fn_710022cd48());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntDictionary<base::global::CStrId, CActor*>>::TKeyElementConstIterator::value_type", true);
        fn_710022d1a8(&g_unk_717269b078, n, g_unk_7101d5e2e0, nullptr, fn_710022ce24, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_717269b078, g_unk_7101a19000);
    fn_710009ec00(g_unk_717269b1b0, &g_unk_717269b078);
    fn_710009ec00(g_unk_717269b1b1, fn_710022d32c());
    {
        CStrId n("base::global::CRntDictionary<base::global::CStrId, CActor*>", true);
        fn_710022d408(&g_unk_717269b1b8, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_717269b1b8, g_unk_7101a19000);
    fn_710009ec00(g_unk_717269b328, &g_unk_717269b1b8);
    fn_710009ec00(g_unk_717269b329, fn_710022d5d8());
    InitConversion(&g_unk_717269b330, fn_71002295ac, fn_710022d5d8, fn_7100243298);
    fn_710009ec00(g_unk_717269b350, fn_710022d6b4());
    fn_710009ec00(g_unk_717269b351, fn_710022d790());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<TActorUniquePtr>>::TKeyElementIterator::value_type", true);
        fn_710022dbf0(&g_unk_717269b358, n, g_unk_7101d5e2e0, nullptr, fn_710022d86c, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_717269b358, g_unk_7101a19000);
    fn_710009ec00(g_unk_717269b490, &g_unk_717269b358);
    fn_710009ec00(g_unk_717269b491, fn_710022dd74());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<TActorUniquePtr>>::TKeyElementConstIterator::value_type", true);
        fn_710022e1d4(&g_unk_717269b498, n, g_unk_7101d5e2e0, nullptr, fn_710022de50, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_717269b498, g_unk_7101a19000);
    fn_710009ec00(g_unk_717269b5d0, &g_unk_717269b498);
    fn_710009ec00(g_unk_717269b5d1, fn_710022e358());
    {
        CStrId n("base::global::CRntVector<TActorUniquePtr>", true);
        fn_710022e434(&g_unk_717269b5d8, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_717269b5d8, g_unk_7101a19000);
    fn_710009ec00(g_unk_717269b748, &g_unk_717269b5d8);
    fn_710009ec00(g_unk_717269b749, fn_710022e614());
    fn_710009ec00(g_unk_717269b74a, fn_710022e6f0());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntDictionary<base::global::CStrId, base::global::CRntVector<base::global::CWeakPtr<CActor>>>>::TKeyElementIterator::value_type", true);
        fn_710022eb50(&g_unk_717269b750, n, g_unk_7101d5e2e0, nullptr, fn_710022e7cc, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_717269b750, g_unk_7101a19000);
    fn_710009ec00(g_unk_717269b888, &g_unk_717269b750);
    fn_710009ec00(g_unk_717269b889, fn_710022ecd4());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntDictionary<base::global::CStrId, base::global::CRntVector<base::global::CWeakPtr<CActor>>>>::TKeyElementConstIterator::value_type", true);
        fn_710022f134(&g_unk_717269b890, n, g_unk_7101d5e2e0, nullptr, fn_710022edb0, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_717269b890, g_unk_7101a19000);
    fn_710009ec00(g_unk_717269b9c8, &g_unk_717269b890);
    fn_710009ec00(g_unk_717269b9c9, fn_710022f2b8());
    {
        CStrId n("base::global::CRntDictionary<base::global::CStrId, base::global::CRntVector<base::global::CWeakPtr<CActor>>>", true);
        fn_710022f394(&g_unk_717269b9d0, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_717269b9d0, g_unk_7101a19000);
    fn_710009ec00(g_unk_717269bb40, &g_unk_717269b9d0);
    {
        CStrId n("CGameLink<CActor>", true);
        fn_710022f608(&g_unk_717269bb48, n, g_unk_71727bbe68, nullptr, fn_710022f568, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_717269bb48, g_unk_7101a19000);
    fn_710009ec00(g_unk_717269bc80, &g_unk_717269bb48);
    fn_710009ec00(g_unk_717269bc81, fn_710022f908());
    fn_710009ec00(g_unk_717269bc82, fn_710022f9e4());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<CGameLink<CActor>>>::TKeyElementIterator::value_type", true);
        fn_710022fe44(&g_unk_717269bc88, n, g_unk_7101d5e2e0, nullptr, fn_710022fac0, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_717269bc88, g_unk_7101a19000);
    fn_710009ec00(g_unk_717269bdc0, &g_unk_717269bc88);
    fn_710009ec00(g_unk_717269bdc1, fn_710022ffc8());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<CGameLink<CActor>>>::TKeyElementConstIterator::value_type", true);
        fn_7100230428(&g_unk_717269bdc8, n, g_unk_7101d5e2e0, nullptr, fn_71002300a4, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_717269bdc8, g_unk_7101a19000);
    fn_710009ec00(g_unk_717269bf00, &g_unk_717269bdc8);
    fn_710009ec00(g_unk_717269bf01, fn_71002305ac());
    {
        CStrId n("base::global::CRntVector<CGameLink<CActor>>", true);
        fn_7100230688(&g_unk_717269bf08, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_717269bf08, g_unk_7101a19000);
    fn_710009ec00(g_unk_717269c078, &g_unk_717269bf08);
    fn_710009ec00(g_unk_717269c079, fn_710023087c());
    fn_710009ec00(g_unk_717269c07a, fn_7100230958());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntDictionary<base::global::CStrId, base::global::CRntVector<CGameLink<CActor>>>>::TKeyElementIterator::value_type", true);
        fn_7100230db8(&g_unk_717269c080, n, g_unk_7101d5e2e0, nullptr, fn_7100230a34, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_717269c080, g_unk_7101a19000);
    fn_710009ec00(g_unk_717269c1b8, &g_unk_717269c080);
    fn_710009ec00(g_unk_717269c1b9, fn_7100230f3c());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntDictionary<base::global::CStrId, base::global::CRntVector<CGameLink<CActor>>>>::TKeyElementConstIterator::value_type", true);
        fn_710023139c(&g_unk_717269c1c0, n, g_unk_7101d5e2e0, nullptr, fn_7100231018, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_717269c1c0, g_unk_7101a19000);
    fn_710009ec00(g_unk_717269c2f8, &g_unk_717269c1c0);
    fn_710009ec00(g_unk_717269c2f9, fn_7100231520());
    {
        CStrId n("base::global::CRntDictionary<base::global::CStrId, base::global::CRntVector<CGameLink<CActor>>>", true);
        fn_71002315fc(&g_unk_717269c300, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_717269c300, g_unk_7101a19000);
    fn_710009ec00(g_unk_717269c470, &g_unk_717269c300);
    fn_710009ec00(g_unk_717269c471, fn_71002317d0());
    fn_710009ec00(g_unk_717269c472, fn_71002318ac());
    fn_710009ec00(g_unk_717269c473, fn_7100231988());
    fn_710009ec00(g_unk_717269c474, fn_7100231a64());
    InitConversion(&g_unk_717269c478, TypeOf<fn_7100009658>, &g_unk_717269bb48, fn_7100248b38);
    InitConversion(&g_unk_717269c498, fn_7100231b40, fn_71002295ac, fn_7100248cd4);
    InitConversion(&g_unk_717269c4b8, fn_7100231c1c, &g_unk_717269bb48, fn_710023de90);
    InitNamedValue(&g_unk_717269c4e0, "vPos", TypeOf<fn_7100009658>, fn_710010c424);
    InitNamedValue(&g_unk_717269c4f0, "sName", TypeOf<fn_7100009658>, fn_710010c424);
    InitNamedValue(&g_unk_717269c500, "pComponents", TypeOf<fn_7100009658>, fn_710010c424);
}
