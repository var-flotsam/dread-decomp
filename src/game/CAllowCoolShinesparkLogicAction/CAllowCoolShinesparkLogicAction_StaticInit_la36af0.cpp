// Static initialiser of CAllowCoolShinesparkLogicAction's unit: the header, the class registration, and the
// vectors of EShinesparkTravellingDirection and ECoolShinesparkSituation with their iterator value types.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflTypeGlobals.h"
#include "game/UnkTUInit.h"

// The shared helpers see a reflected type through the placeholder layout (UnkReflType).
static inline UnkReflType* AsUnk(base::reflection::CType* t) { return reinterpret_cast<UnkReflType*>(t); }

extern UnkTUHeaderAndClass g_unk_7172748548 __attribute__((visibility("hidden")));

extern char g_unk_71727474d0;
extern char g_unk_71727474d1;
extern char g_unk_71727474d2;
extern UnkReflType g_unk_71727474d8;
extern char g_unk_7172747610;
extern char g_unk_7172747611;
extern UnkReflType g_unk_7172747618;
extern char g_unk_7172747750;
extern char g_unk_7172747751;
extern UnkReflType g_unk_7172747758;
extern char g_unk_71727478c8;
extern char g_unk_71727478c9;
extern char g_unk_71727478ca;
extern UnkReflType g_unk_71727478d0;
extern char g_unk_7172747a08;
extern char g_unk_7172747a09;
extern UnkReflType g_unk_7172747a10;
extern char g_unk_7172747b48;
extern char g_unk_7172747b49;
extern UnkReflType g_unk_7172747b50;
extern char g_unk_7172747cc0;

UnkReflType* fn_7100a31378();  // refl_GetType<...CRntVector<EShinesparkTravellingDirection>>::TElementIterator>
UnkReflType* fn_7100a31454();  // refl_GetType<...CRntVector<EShinesparkTravellingDirection>>::TElementConstIterator>
void fn_7100a31530();
void fn_7100a318b4(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
UnkReflType* fn_7100a31a38();
void fn_7100a31b14();
void fn_7100a31e98(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
UnkReflType* fn_7100a3201c();
void fn_7100a320f8(UnkReflType* self, const CStrId& name, void* a, void* b);
UnkReflType* fn_7100a322ec();
UnkReflType* fn_7100a323c8();
void fn_7100a324a4();
void fn_7100a32828(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
UnkReflType* fn_7100a329ac();
void fn_7100a32a88();
void fn_7100a32e0c(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
UnkReflType* fn_7100a32f90();
void fn_7100a3306c(UnkReflType* self, const CStrId& name, void* a, void* b);

void fn_7100a36af0() {
    InitHeader(g_unk_7172748548.header);
    fn_7100a30cd4();
    fn_710009ec00(&g_unk_71727474d0, &g_unk_7172748548.type);
    fn_710009ec00(&g_unk_71727474d1, fn_7100a31378());
    fn_710009ec00(&g_unk_71727474d2, fn_7100a31454());

    InitClass(&g_unk_71727474d8,
              "base::reflection::CollectionTypeMapper<base::global::CRntVector<EShinesparkTravellingDirection>>::"
              "TKeyElementIterator::value_type",
              fn_7100a318b4, fn_7100a31530, AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(&g_unk_7172747610, &g_unk_71727474d8);
    fn_710009ec00(&g_unk_7172747611, fn_7100a31a38());
    InitClass(&g_unk_7172747618,
              "base::reflection::CollectionTypeMapper<base::global::CRntVector<EShinesparkTravellingDirection>>::"
              "TKeyElementConstIterator::value_type",
              fn_7100a31e98, fn_7100a31b14, AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(&g_unk_7172747750, &g_unk_7172747618);
    fn_710009ec00(&g_unk_7172747751, fn_7100a3201c());
    InitVector(&g_unk_7172747758, "base::global::CRntVector<EShinesparkTravellingDirection>", fn_7100a320f8);
    fn_710009ec00(&g_unk_71727478c8, &g_unk_7172747758);

    fn_710009ec00(&g_unk_71727478c9, fn_7100a322ec());
    fn_710009ec00(&g_unk_71727478ca, fn_7100a323c8());
    InitClass(&g_unk_71727478d0,
              "base::reflection::CollectionTypeMapper<base::global::CRntVector<ECoolShinesparkSituation>>::"
              "TKeyElementIterator::value_type",
              fn_7100a32828, fn_7100a324a4, AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(&g_unk_7172747a08, &g_unk_71727478d0);
    fn_710009ec00(&g_unk_7172747a09, fn_7100a329ac());
    InitClass(&g_unk_7172747a10,
              "base::reflection::CollectionTypeMapper<base::global::CRntVector<ECoolShinesparkSituation>>::"
              "TKeyElementConstIterator::value_type",
              fn_7100a32e0c, fn_7100a32a88, AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(&g_unk_7172747b48, &g_unk_7172747a10);
    fn_710009ec00(&g_unk_7172747b49, fn_7100a32f90());
    InitVector(&g_unk_7172747b50, "base::global::CRntVector<ECoolShinesparkSituation>", fn_7100a3306c);
    fn_710009ec00(&g_unk_7172747cc0, &g_unk_7172747b50);
}
