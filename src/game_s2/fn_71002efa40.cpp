// Static initialiser of the unit of the CPlaythrough::SCheckpointData, CPlaythrough::SCheckpointData::SValidationInfo, base::reflection::CollectionTypeMapper<CPlaythrough::TDictCheckpointDatas>::TKeyElementIterator::value_type, base::reflection::CollectionTypeMapper<CPlaythrough::TDictCheckpointDatas>::TKeyElementConstIterator::value_type, CPlaythrough::TDictCheckpointDatas.
#include "../game_s7/UnkStaticInitS7.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

struct UnkFnObj {
    u64 field_0x0[3];
    void (*field_0x18)();
    u64 field_0x20[2];
};
void fn_710000cf00();
void fn_710000d470();
void fn_710000d4f4();

// A small-buffer string static (0xc8 bytes): the string object, its inline buffer's vtable at +0x30
// and two cleared words. Layout is a placeholder.
struct UnkBufferStr {
    u8 field_0x0[0x30];
    void** field_0x30;
    u8 pad_0x38[0x80];
    u64 field_0xb8;
    u32 field_0xc0;
    u8 pad_0xc4[4];
};
void fn_710007e6e4(void* self, void* inline_buffer);  // a small buffer's constructor
void fn_710007e77c(void* self, const char* str);      // assigns a string to it
void fn_71002eba1c(void* self);                       // its destructor, as registered
extern void* g_unk_7101a1b750[];

__attribute__((always_inline)) inline void InitBufferStr(UnkBufferStr* b, const char* str) {
    fn_710007e6e4(b, &b->field_0x30);
    b->field_0x30 = g_unk_7101a1b750 + 2;
    b->field_0xb8 = 0;
    b->field_0xc0 = 0;
    fn_710007e77c(b, str);
    __cxa_atexit(fn_71002eba1c, b, g_unk_7101a19000);
}

struct UnkStatics_71002efa40 {
    HeaderIds ids;
    u8 pad_0x18[0x38];
    UnkFnObj field_0x50;
    UnkFnObj field_0x80;
    UnkFnObj field_0xb0;
    UnkBufferStr field_0xe0;
    UnkBufferStr field_0x1a8;
    u8 field_0x270[8];
};
extern UnkStatics_71002efa40 g_unk_71726ad668 __attribute__((visibility("hidden")));
extern char g_unk_71726ac870[];
extern UnkEnumType g_unk_71726ac878;
void fn_71002ea1d0();
void fn_71002ea320(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_7100096604(void* type);  // the type's destructor, as registered
extern char g_unk_71726ac9b0[];
extern UnkEnumType g_unk_71726ac9b8;
void fn_71002ea494();
void fn_71002ea654(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
extern char g_unk_71726acaf0[];
extern char g_unk_71726acaf1[];
UnkEnumType* fn_71002ea7d8();  // base::reflection::refl_GetType<std::unique_ptr<CPlaythrough::SCheckpointData>>
extern char g_unk_71726acaf2[];
UnkEnumType* fn_71002ea9dc();
extern char g_unk_71726acaf3[];
UnkEnumType* fn_71002eaab8();
extern char g_unk_71726acaf4[];
UnkEnumType* fn_71002eab94();
extern UnkEnumType g_unk_71726acaf8;
extern char g_unk_7101d5e2e0[];
void fn_71002eac70();
void fn_71002eaff4(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
extern char g_unk_71726acc30[];
extern char g_unk_71726acc31[];
UnkEnumType* fn_71002eb178();
extern UnkEnumType g_unk_71726acc38;
void fn_71002eb254();
void fn_71002eb5d8(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
extern char g_unk_71726acd70[];
extern char g_unk_71726acd71[];
UnkEnumType* fn_71002eb75c();
extern UnkEnumType g_unk_71726acd78;
void fn_71002eb838(UnkEnumType* type, const CStrId& name, void*, void*);
void fn_710009891c(void* type);  // the type's destructor, as registered
extern char g_unk_71726acee8[];

void fn_71002efa40() {
    InitHeaderIds(&g_unk_71726ad668.ids);
    g_unk_71726ad668.field_0x50 = {{0, 0, 0}, fn_710000cf00, {0, 0}};
    g_unk_71726ad668.field_0x80 = {{0, 0, 0}, fn_710000d470, {0, 0}};
    g_unk_71726ad668.field_0xb0 = {{0, 0, 0}, fn_710000d4f4, {0, 0}};
    fn_71002e970c();
    fn_710009ec00(g_unk_71726ac870, g_unk_71726ad668.field_0x270);
    {
        CStrId n("CPlaythrough::SCheckpointData", true);
        fn_71002ea320(&g_unk_71726ac878, n, nullptr, nullptr, fn_71002ea1d0, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71726ac878, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726ac9b0, &g_unk_71726ac878);
    {
        CStrId n("CPlaythrough::SCheckpointData::SValidationInfo", true);
        fn_71002ea654(&g_unk_71726ac9b8, n, nullptr, nullptr, fn_71002ea494, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71726ac9b8, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726acaf0, &g_unk_71726ac9b8);
    fn_710009ec00(g_unk_71726acaf1, fn_71002ea7d8());
    fn_710009ec00(g_unk_71726acaf2, fn_71002ea9dc());
    fn_710009ec00(g_unk_71726acaf3, fn_71002eaab8());
    fn_710009ec00(g_unk_71726acaf4, fn_71002eab94());
    {
        CStrId n("base::reflection::CollectionTypeMapper<CPlaythrough::TDictCheckpointDatas>::TKeyElementIterator::value_type", true);
        fn_71002eaff4(&g_unk_71726acaf8, n, g_unk_7101d5e2e0, nullptr, fn_71002eac70, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71726acaf8, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726acc30, &g_unk_71726acaf8);
    fn_710009ec00(g_unk_71726acc31, fn_71002eb178());
    {
        CStrId n("base::reflection::CollectionTypeMapper<CPlaythrough::TDictCheckpointDatas>::TKeyElementConstIterator::value_type", true);
        fn_71002eb5d8(&g_unk_71726acc38, n, g_unk_7101d5e2e0, nullptr, fn_71002eb254, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71726acc38, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726acd70, &g_unk_71726acc38);
    fn_710009ec00(g_unk_71726acd71, fn_71002eb75c());
    {
        CStrId n("CPlaythrough::TDictCheckpointDatas", true);
        fn_71002eb838(&g_unk_71726acd78, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_71726acd78, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726acee8, &g_unk_71726acd78);
    InitBufferStr(&g_unk_71726ad668.field_0xe0, "presaveddata/");
    InitBufferStr(&g_unk_71726ad668.field_0x1a8, "playthroughs/");
}
