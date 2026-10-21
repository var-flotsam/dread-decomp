// Per-TU static initialisers (unguarded; destructors registered with fn_7100000250, the
// __cxa_atexit stub). Each TU first builds the same header data (a word and the "binaries" and
// "sources" ids), then registers its reflected types: a class or enum type built from its name
// (destroyed at exit) and noted in the TU's registry flags, and for enums a pair of converter
// records (to and from the integer type) registered with fn_71000a16c4.
#include <new>

#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

extern char g_unk_7101a19000 __attribute__((visibility("hidden")));  // the DSO handle
int fn_7100000250(void (*dtor)(void*), void* object, void* dso);     // __cxa_atexit
void fn_710000051c(void* id);                                         // a CStrId's destructor

struct UnkReflTypeInfo {
    u8 unk_0x0[0x30];
    void* field_0x30;
};
struct UnkReflType {
    void* vtable;
    UnkReflTypeInfo* info;
};
void fn_710009ec00(void* flag, UnkReflType* type);
void fn_7100096604(void* type);  // a class type's destructor
void fn_7100099468(void* type);  // an enum type's destructor

// The header data every TU's initialiser starts with.
struct UnkTUHeader {
    u32 field_0x0;
    CStrId binaries;
    CStrId sources;
};

__attribute__((always_inline)) static inline void InitHeader(UnkTUHeader& h) {
    h.field_0x0 = 6;
    new (&h.binaries) CStrId("binaries", true);
    fn_7100000250(fn_710000051c, &h.binaries, &g_unk_7101a19000);
    new (&h.sources) CStrId("sources", true);
    fn_7100000250(fn_710000051c, &h.sources, &g_unk_7101a19000);
}

// A class type: constructed from its name by `ctor` with its base type (if any) and an init
// function.
typedef void (*UnkClassCtor)(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(),
                             void* b);

__attribute__((always_inline)) static inline void InitClass(UnkReflType* type, const char* name, UnkClassCtor ctor,
                                                            void (*init)(), UnkReflType* base = nullptr) {
    {
        CStrId id(name, true);
        ctor(type, id, base, nullptr, init, nullptr);
    }
    fn_7100000250(fn_7100096604, type, &g_unk_7101a19000);
}

// An enum type: constructed from its name by `ctor` with an init function.
typedef void (*UnkEnumCtor)(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);

__attribute__((always_inline)) static inline void InitEnum(UnkReflType* type, const char* name, UnkEnumCtor ctor,
                                                           void (*init)()) {
    {
        CStrId id(name, true);
        ctor(type, id, init, nullptr);
    }
    fn_7100000250(fn_7100099468, type, &g_unk_7101a19000);
}

// A converter between two types, registered with fn_71000a16c4.
struct UnkConverter {
    void* from;
    void* to;
    void (*convert)();
    void* field_0x18;
};
void fn_71000a16c4(UnkConverter* converter);
UnkReflType* fn_7100014768();  // the enums' integer type

__attribute__((always_inline)) static inline void InitEnumConverters(UnkReflType* type, UnkConverter& to_int,
                                                                     void (*to_int_fn)(), UnkConverter& from_int,
                                                                     void (*from_int_fn)()) {
    to_int = {type->info->field_0x30, fn_7100014768()->info->field_0x30, to_int_fn, nullptr};
    fn_71000a16c4(&to_int);
    from_int = {fn_7100014768()->info->field_0x30, type->info->field_0x30, from_int_fn, nullptr};
    fn_71000a16c4(&from_int);
}

extern UnkTUHeader g_unk_71727bc230 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_71727bc110;
void fn_7100da56a4();
void fn_7100da5c70(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern char g_unk_71727bc228;

void fn_7100da5e70() {
    InitHeader(g_unk_71727bc230);
    InitEnum(&g_unk_71727bc110, "SLinkType::ELinkType", fn_7100da5c70, fn_7100da56a4);
    fn_710009ec00(&g_unk_71727bc228, &g_unk_71727bc110);
}

extern UnkTUHeader g_unk_717275e1e0 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_717275de80;
void fn_7100b03ecc();
void fn_7100b03f60(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
extern char g_unk_717275dfb8;
UnkReflType* fn_7100b042c4();  // base::reflection::refl_GetType<std::unique_ptr<GUI::CDisplayObjectAnimationDef>>
extern char g_unk_717275dfb9;

void fn_7100b04900() {
    InitHeader(g_unk_717275e1e0);
    InitClass(&g_unk_717275de80, "GUI::CDisplayObjectAnimationDef", fn_7100b03f60, fn_7100b03ecc);
    fn_710009ec00(&g_unk_717275dfb8, &g_unk_717275de80);
    fn_710009ec00(&g_unk_717275dfb9, fn_7100b042c4());
}

extern UnkTUHeader g_unk_7172768ad0 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_7172768990;
void fn_7100b87b5c();
void fn_7100b87e10(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
extern char g_unk_7172768ac8;
UnkReflType* fn_7100b040c0();  // base::reflection::refl_GetType<std::unique_ptr<GUI::CTrackSet>>
extern char g_unk_7172768ac9;

void fn_7100b88910() {
    InitHeader(g_unk_7172768ad0);
    InitClass(&g_unk_7172768990, "GUI::CTrackSet", fn_7100b87e10, fn_7100b87b5c);
    fn_710009ec00(&g_unk_7172768ac8, &g_unk_7172768990);
    fn_710009ec00(&g_unk_7172768ac9, fn_7100b040c0());
}

extern UnkTUHeader g_unk_7172792168 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_7172792008;
void fn_7100c79130();
void fn_7100c797bc(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern char g_unk_7172792120;
void fn_7100c7a8c0() __attribute__((visibility("hidden")));
void fn_7100c7a8cc() __attribute__((visibility("hidden")));
extern UnkConverter g_unk_7172792128;
extern UnkConverter g_unk_7172792148;

void fn_7100c7a950() {
    InitHeader(g_unk_7172792168);
    InitEnum(&g_unk_7172792008, "game::curves::EProcessorType", fn_7100c797bc, fn_7100c79130);
    fn_710009ec00(&g_unk_7172792120, &g_unk_7172792008);
    InitEnumConverters(&g_unk_7172792008, g_unk_7172792128, fn_7100c7a8c0, g_unk_7172792148, fn_7100c7a8cc);
}

extern UnkTUHeader g_unk_7172795408 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_71727952a8;
void fn_7100c94244();
void fn_7100c94510(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern char g_unk_71727953c0;
void fn_7100c94690() __attribute__((visibility("hidden")));
void fn_7100c9469c() __attribute__((visibility("hidden")));
extern UnkConverter g_unk_71727953c8;
extern UnkConverter g_unk_71727953e8;

void fn_7100c94720() {
    InitHeader(g_unk_7172795408);
    InitEnum(&g_unk_71727952a8, "EInFrustumMode", fn_7100c94510, fn_7100c94244);
    fn_710009ec00(&g_unk_71727953c0, &g_unk_71727952a8);
    InitEnumConverters(&g_unk_71727952a8, g_unk_71727953c8, fn_7100c94690, g_unk_71727953e8, fn_7100c9469c);
}

// FAIL 0x7100c77cd0: unknown token 8: ('got', '7172791e48')

extern UnkTUHeader g_unk_717277e0f8 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_717277df90;
void fn_7100c10ce0();
void fn_7100c10e2c(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern char g_unk_717277e0a8;
void fn_7100c1140c() __attribute__((visibility("hidden")));
void fn_7100c11418() __attribute__((visibility("hidden")));
extern UnkConverter g_unk_717277e0b0;
extern UnkConverter g_unk_717277e0d0;
void fn_7100c11094();  // CCharClassCommanderWingsTrack::refl_GetClass
extern char g_unk_717277e0f0;
extern UnkReflType g_unk_717277e118 __attribute__((visibility("hidden")));

void fn_7100c11580() {
    InitHeader(g_unk_717277e0f8);
    InitEnum(&g_unk_717277df90, "EWings", fn_7100c10e2c, fn_7100c10ce0);
    fn_710009ec00(&g_unk_717277e0a8, &g_unk_717277df90);
    InitEnumConverters(&g_unk_717277df90, g_unk_717277e0b0, fn_7100c1140c, g_unk_717277e0d0, fn_7100c11418);
    fn_7100c11094();
    fn_710009ec00(&g_unk_717277e0f0, &g_unk_717277e118);
}

extern UnkTUHeader g_unk_71727ae078 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_71727adf10;
void fn_7100d29ec0();
void fn_7100d2a00c(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern char g_unk_71727ae028;
void fn_7100d2aa68() __attribute__((visibility("hidden")));
void fn_7100d2aa74() __attribute__((visibility("hidden")));
extern UnkConverter g_unk_71727ae030;
extern UnkConverter g_unk_71727ae050;
void fn_7100d2a190();  // CCharClassToStateTrack::refl_GetClass
extern char g_unk_71727ae070;
extern UnkReflType g_unk_71727ae098 __attribute__((visibility("hidden")));

void fn_7100d2ac00() {
    InitHeader(g_unk_71727ae078);
    InitEnum(&g_unk_71727adf10, "CCharClassToStateTrack::EMaintainFrameMode", fn_7100d2a00c, fn_7100d29ec0);
    fn_710009ec00(&g_unk_71727ae028, &g_unk_71727adf10);
    InitEnumConverters(&g_unk_71727adf10, g_unk_71727ae030, fn_7100d2aa68, g_unk_71727ae050, fn_7100d2aa74);
    fn_7100d2a190();
    fn_710009ec00(&g_unk_71727ae070, &g_unk_71727ae098);
}

extern UnkTUHeader g_unk_71727bba50 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_71727bb8e8;
void fn_7100da3810();
void fn_7100da3a1c(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern char g_unk_71727bba00;
void fn_7100da3e00() __attribute__((visibility("hidden")));
void fn_7100da3e0c() __attribute__((visibility("hidden")));
extern UnkConverter g_unk_71727bba08;
extern UnkConverter g_unk_71727bba28;
void fn_7100da3ba0();  // IPath::refl_GetClass
extern char g_unk_71727bba48;
extern UnkReflType g_unk_71727bba70 __attribute__((visibility("hidden")));

void fn_7100da3ed0() {
    InitHeader(g_unk_71727bba50);
    InitEnum(&g_unk_71727bb8e8, "IPath::EType", fn_7100da3a1c, fn_7100da3810);
    fn_710009ec00(&g_unk_71727bba00, &g_unk_71727bb8e8);
    InitEnumConverters(&g_unk_71727bb8e8, g_unk_71727bba08, fn_7100da3e00, g_unk_71727bba28, fn_7100da3e0c);
    fn_7100da3ba0();
    fn_710009ec00(&g_unk_71727bba48, &g_unk_71727bba70);
}

extern UnkTUHeader g_unk_71727b9018 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_71727b8dd8;
void fn_7100d911d8();
void fn_7100d91564(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern char g_unk_71727b8ef0;
extern UnkReflType g_unk_71727b8ef8;
void fn_7100d916e4();
void fn_7100d91d10(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern char g_unk_71727b9010;

void fn_7100d91f80() {
    InitHeader(g_unk_71727b9018);
    InitEnum(&g_unk_71727b8dd8, "EFXType", fn_7100d91564, fn_7100d911d8);
    fn_710009ec00(&g_unk_71727b8ef0, &g_unk_71727b8dd8);
    InitEnum(&g_unk_71727b8ef8, "ELinkType", fn_7100d91d10, fn_7100d916e4);
    fn_710009ec00(&g_unk_71727b9010, &g_unk_71727b8ef8);
}

extern UnkTUHeader g_unk_71727be710 __attribute__((visibility("hidden")));
UnkReflType* fn_7100dded9c();  // fn_7100dded9c
extern char g_unk_71727be380;
UnkReflType* fn_7100ddee78();  // fn_7100ddee78
extern char g_unk_71727be381;
extern UnkReflType g_unk_71727be388;
void fn_7100ddef54();
void fn_7100ddf1c0(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern char g_unk_71727be4a0;
void fn_7100de1b0c() __attribute__((visibility("hidden")));
void fn_7100de1b18() __attribute__((visibility("hidden")));
extern UnkConverter g_unk_71727be4a8;
extern UnkConverter g_unk_71727be4c8;
void fn_7100ddf344();  // CExplosionGfx::refl_GetClass
extern char g_unk_71727be4e8;
extern UnkReflType g_unk_71727be730 __attribute__((visibility("hidden")));

void fn_7100de25a0() {
    InitHeader(g_unk_71727be710);
    fn_710009ec00(&g_unk_71727be380, fn_7100dded9c());
    fn_710009ec00(&g_unk_71727be381, fn_7100ddee78());
    InitEnum(&g_unk_71727be388, "CExplosionGfx::ETransitionOperationMode", fn_7100ddf1c0, fn_7100ddef54);
    fn_710009ec00(&g_unk_71727be4a0, &g_unk_71727be388);
    InitEnumConverters(&g_unk_71727be388, g_unk_71727be4a8, fn_7100de1b0c, g_unk_71727be4c8, fn_7100de1b18);
    fn_7100ddf344();
    fn_710009ec00(&g_unk_71727be4e8, &g_unk_71727be730);
}

// FAIL 0x7100adcf10: unknown token 8: ('got', '710000cf00')
// FAIL 0x7100db21d0: expected got at 10: ('bl', '7100000250', 'fn_7100000250')
// FAIL 0x7100da54b0: unknown token 22: ('bl', '7100009658', 'base::reflection::refl_GetType<base::global::CStrId>')
// FAIL 0x7100c668b0: unknown token 27: ('bl', '7100005178', 'base::reflection::refl_GetType<unsigned>')
// FAIL 0x7100acb290: unknown token 8: ('got', '7172755b00')

extern UnkTUHeader g_unk_71727bf998 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_71727bf6f8;
void fn_7100e06ce0();
void fn_7100e06e8c(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern char g_unk_71727bf810;
void fn_7100e073d4() __attribute__((visibility("hidden")));
void fn_7100e073e0() __attribute__((visibility("hidden")));
extern UnkConverter g_unk_71727bf818;
extern UnkConverter g_unk_71727bf838;
extern UnkReflType g_unk_71727bf858;
void fn_7100e0700c();
void fn_7100e07250(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
extern char g_unk_71727bf990;

void fn_7100e07920() {
    InitHeader(g_unk_71727bf998);
    InitEnum(&g_unk_71727bf6f8, "EEntryType", fn_7100e06e8c, fn_7100e06ce0);
    fn_710009ec00(&g_unk_71727bf810, &g_unk_71727bf6f8);
    InitEnumConverters(&g_unk_71727bf6f8, g_unk_71727bf818, fn_7100e073d4, g_unk_71727bf838, fn_7100e073e0);
    InitClass(&g_unk_71727bf858, "CGlobalEventManager::SMissionLogEntry", fn_7100e07250, fn_7100e0700c);
    fn_710009ec00(&g_unk_71727bf990, &g_unk_71727bf858);
}

// FAIL 0x7100adf770: expected got at 30: ('bl', '7100adf508', 'fn_7100adf508')

extern UnkTUHeader g_unk_7172795270 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_7172794fb0;
void fn_7100c932c4();
void fn_7100c93470(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern char g_unk_71727950c8;
void fn_7100c93c48() __attribute__((visibility("hidden")));
void fn_7100c93c54() __attribute__((visibility("hidden")));
extern UnkConverter g_unk_71727950d0;
extern UnkConverter g_unk_71727950f0;
extern UnkReflType g_unk_7172795110;
void fn_7100c935f0();
void fn_7100c9385c(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern char g_unk_7172795228;
void fn_7100c93c60() __attribute__((visibility("hidden")));
void fn_7100c93c6c() __attribute__((visibility("hidden")));
extern UnkConverter g_unk_7172795230;
extern UnkConverter g_unk_7172795250;

void fn_7100c93d60() {
    InitHeader(g_unk_7172795270);
    InitEnum(&g_unk_7172794fb0, "cutscene::ETakePlayMode", fn_7100c93470, fn_7100c932c4);
    fn_710009ec00(&g_unk_71727950c8, &g_unk_7172794fb0);
    InitEnumConverters(&g_unk_7172794fb0, g_unk_71727950d0, fn_7100c93c48, g_unk_71727950f0, fn_7100c93c54);
    InitEnum(&g_unk_7172795110, "cutscene::EDevelopmentState", fn_7100c9385c, fn_7100c935f0);
    fn_710009ec00(&g_unk_7172795228, &g_unk_7172795110);
    InitEnumConverters(&g_unk_7172795110, g_unk_7172795230, fn_7100c93c60, g_unk_7172795250, fn_7100c93c6c);
}

// FAIL 0x7100ebefd0: unknown token 38: ('data', '71729ce058')
// FAIL 0x7100ad39e0: unknown token 11: ('bl', '710025d654', 'base::reflection::refl_GetType<gameeditor::CGameModelIndex>')
// FAIL 0x7100da1ce0: expected bl at 27: ('got', '7100d9dce8')
// FAIL 0x7100b0a100: unknown token 8: ('got', '710000cf00')
// FAIL 0x7100e16af0: unknown token 8: ('got', '71727c0918')
// FAIL 0x7100e0ecb0: unknown token 8: ('got', '710000cf00')
// FAIL 0x7100e1b8b0: expected got at 30: ('bl', '7100e177ec', 'fn_7100e177ec')
// FAIL 0x7100ad28e0: expected bl at 30: ('got', '7100acea3c')
// FAIL 0x7100b0d6c0: expected bl at 33: ('got', '7100b0ab60')
// FAIL 0x7100b797e0: expected bl at 33: ('got', '7100b76544')
// FAIL 0x7100b81290: expected bl at 33: ('got', '7100b7db14')
// FAIL 0x7100da7050: unknown token 45: ('data', '71727bc5b0')
// FAIL 0x7100b2bc80: unknown token 8: ('got', '7172762b18')
// FAIL 0x7100aef910: unknown token 19: ('bl', '7100009658', 'base::reflection::refl_GetType<base::global::CStrId>')
// FAIL 0x7100d9caf0: unknown token 27: ('bl', '7100005178', 'base::reflection::refl_GetType<unsigned>')
// FAIL 0x7100b84970: expected bl at 44: ('got', '7100b81cf8')
// FAIL 0x7100e7c980: unknown token 8: ('got', '71727c8f58')
// FAIL 0x7100d90cd0: unknown token 8: ('got', '710000cf00')
// FAIL 0x7100ad4c90: unknown token 23: ('data', '7172756cd0')
// FAIL 0x7100e1dde0: expected got at 11: ('bl', '7100000250', 'fn_7100000250')
// FAIL 0x7100d6ff80: unknown token 20: ('bl', '71009155a8', 'base::reflection::refl_GetType<game::logic::collision::CShape*>')
// FAIL 0x7100adbc20: expected bl at 62: ('got', '7100ad6f04')
// FAIL 0x7100b5d0c0: expected bl at 21: ('got', '7100b54e20')
// FAIL 0x7100ca6150: unknown token 8: ('got', '7172796388')
// FAIL 0x7100b8e430: expected got at 102: ('bl', '7100b8dd9c', 'fn_7100b8dd9c')
// FAIL 0x7100e06590: unknown token 11: ('got', '71727be870')
// FAIL 0x7100c92b10: expected bl at 24: ('got', '7100c8398c')
