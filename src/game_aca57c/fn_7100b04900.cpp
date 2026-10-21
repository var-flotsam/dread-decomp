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
UnkReflType* fn_7100005178();  // refl_GetType<unsigned>, the flag sets' integer type

__attribute__((always_inline)) static inline void InitEnumConverters(UnkReflType* type, UnkConverter& to_int,
                                                                     void (*to_int_fn)(), UnkConverter& from_int,
                                                                     void (*from_int_fn)(),
                                                                     UnkReflType* (*int_type)() = fn_7100014768) {
    to_int = {type->info->field_0x30, int_type()->info->field_0x30, to_int_fn, nullptr};
    fn_71000a16c4(&to_int);
    from_int = {int_type()->info->field_0x30, type->info->field_0x30, from_int_fn, nullptr};
    fn_71000a16c4(&from_int);
}

// A flag-set type: constructed from its name by `ctor`.
void fn_710009a368(void* type);  // a flag-set type's destructor
typedef void (*UnkFlagSetCtor)(UnkReflType* self, const CStrId& name, void* extra);

__attribute__((always_inline)) static inline void InitFlagSet(UnkReflType* type, const char* name, UnkFlagSetCtor ctor) {
    {
        CStrId id(name, true);
        ctor(type, id, nullptr);
    }
    fn_7100000250(fn_710009a368, type, &g_unk_7101a19000);
}

// TU data that follows the header: the class type a getter builds, or other values.
struct UnkTUHeaderAndClass {
    UnkTUHeader header;
    u8 unk_0x18[0x8];
    UnkReflType type;
};

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

extern UnkTUHeader g_unk_7172772d18 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_7172772bb0;
void fn_7100bcdd30();
void fn_7100bcde7c(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern char g_unk_7172772cc8;
void fn_7100bce500() __attribute__((visibility("hidden")));
void fn_7100bce50c() __attribute__((visibility("hidden")));
extern UnkConverter g_unk_7172772cd0;
extern UnkConverter g_unk_7172772cf0;
void fn_7100bce0e4();  // CharClassSakaiModifyShieldEvent::refl_GetClass
extern char g_unk_7172772d10;
extern UnkReflType g_unk_7172772d38 __attribute__((visibility("hidden")));

void fn_7100bce6b0() {
    InitHeader(g_unk_7172772d18);
    InitEnum(&g_unk_7172772bb0, "EShieldAction", fn_7100bcde7c, fn_7100bcdd30);
    fn_710009ec00(&g_unk_7172772cc8, &g_unk_7172772bb0);
    InitEnumConverters(&g_unk_7172772bb0, g_unk_7172772cd0, fn_7100bce500, g_unk_7172772cf0, fn_7100bce50c);
    fn_7100bce0e4();
    fn_710009ec00(&g_unk_7172772d10, &g_unk_7172772d38);
}

extern UnkTUHeader g_unk_7172791e30 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_7172791c90;
void fn_7100c65ffc();
void fn_7100c66688(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern char g_unk_7172791da8;
void fn_7100c66808() __attribute__((visibility("hidden")));
void fn_7100c66814() __attribute__((visibility("hidden")));
extern UnkConverter g_unk_7172791db0;
extern UnkConverter g_unk_7172791dd0;
void fn_7100c66820() __attribute__((visibility("hidden")));
void fn_7100c6682c() __attribute__((visibility("hidden")));
extern UnkConverter g_unk_7172791df0;
extern UnkConverter g_unk_7172791e10;

void fn_7100c668b0() {
    InitHeader(g_unk_7172791e30);
    InitEnum(&g_unk_7172791c90, "game::logic::collision::EColMat", fn_7100c66688, fn_7100c65ffc);
    fn_710009ec00(&g_unk_7172791da8, &g_unk_7172791c90);
    InitEnumConverters(&g_unk_7172791c90, g_unk_7172791db0, fn_7100c66808, g_unk_7172791dd0, fn_7100c66814);
    InitEnumConverters(&g_unk_7172791c90, g_unk_7172791df0, fn_7100c66820, g_unk_7172791e10, fn_7100c6682c, fn_7100005178);
}

extern UnkTUHeader g_unk_7172758ad0 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_7172758838;
void fn_7100adf0bc();
void fn_7100adf388(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern char g_unk_7172758950;
void fn_7100adf658() __attribute__((visibility("hidden")));
void fn_7100adf664() __attribute__((visibility("hidden")));
extern UnkConverter g_unk_7172758958;
extern UnkConverter g_unk_7172758978;
extern UnkReflType g_unk_7172758998;
void fn_7100adf508(UnkReflType* self, const CStrId& name, void* extra);
extern char g_unk_7172758a88;
void fn_7100adf670() __attribute__((visibility("hidden")));
void fn_7100adf67c() __attribute__((visibility("hidden")));
extern UnkConverter g_unk_7172758a90;
extern UnkConverter g_unk_7172758ab0;

void fn_7100adf770() {
    InitHeader(g_unk_7172758ad0);
    InitEnum(&g_unk_7172758838, "EPickingMode", fn_7100adf388, fn_7100adf0bc);
    fn_710009ec00(&g_unk_7172758950, &g_unk_7172758838);
    InitEnumConverters(&g_unk_7172758838, g_unk_7172758958, fn_7100adf658, g_unk_7172758978, fn_7100adf664);
    InitFlagSet(&g_unk_7172758998, "TPickingModeFlagSet", fn_7100adf508);
    fn_710009ec00(&g_unk_7172758a88, &g_unk_7172758998);
    InitEnumConverters(&g_unk_7172758998, g_unk_7172758a90, fn_7100adf670, g_unk_7172758ab0, fn_7100adf67c, fn_7100005178);
}

extern UnkTUHeader g_unk_71727c0fb8 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_71727c0bc0;
void fn_7100e16e60();
void fn_7100e1766c(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern char g_unk_71727c0cd8;
void fn_7100e1b5c8() __attribute__((visibility("hidden")));
void fn_7100e1b5d4() __attribute__((visibility("hidden")));
extern UnkConverter g_unk_71727c0ce0;
extern UnkConverter g_unk_71727c0d00;
extern UnkReflType g_unk_71727c0d20;
void fn_7100e177ec(UnkReflType* self, const CStrId& name, void* extra);
extern char g_unk_71727c0e10;
void fn_7100e1b5e0() __attribute__((visibility("hidden")));
void fn_7100e1b5ec() __attribute__((visibility("hidden")));
extern UnkConverter g_unk_71727c0e18;
extern UnkConverter g_unk_71727c0e38;
extern UnkReflType g_unk_71727c0e58;
void fn_7100e1793c();
void fn_7100e17b48(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern char g_unk_71727c0f70;
void fn_7100e1b5f8() __attribute__((visibility("hidden")));
void fn_7100e1b604() __attribute__((visibility("hidden")));
extern UnkConverter g_unk_71727c0f78;
extern UnkConverter g_unk_71727c0f98;

void fn_7100e1b8b0() {
    InitHeader(g_unk_71727c0fb8);
    InitEnum(&g_unk_71727c0bc0, "ETooltipCond", fn_7100e1766c, fn_7100e16e60);
    fn_710009ec00(&g_unk_71727c0cd8, &g_unk_71727c0bc0);
    InitEnumConverters(&g_unk_71727c0bc0, g_unk_71727c0ce0, fn_7100e1b5c8, g_unk_71727c0d00, fn_7100e1b5d4);
    InitFlagSet(&g_unk_71727c0d20, "TTooltipConditions", fn_7100e177ec);
    fn_710009ec00(&g_unk_71727c0e10, &g_unk_71727c0d20);
    InitEnumConverters(&g_unk_71727c0d20, g_unk_71727c0e18, fn_7100e1b5e0, g_unk_71727c0e38, fn_7100e1b5ec, fn_7100005178);
    InitEnum(&g_unk_71727c0e58, "ETooltipType", fn_7100e17b48, fn_7100e1793c);
    fn_710009ec00(&g_unk_71727c0f70, &g_unk_71727c0e58);
    InitEnumConverters(&g_unk_71727c0e58, g_unk_71727c0f78, fn_7100e1b5f8, g_unk_71727c0f98, fn_7100e1b604);
}

extern UnkTUHeader g_unk_71727ba980 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_71727ba2e0;
void fn_7100d975d8();
void fn_7100d977e4(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern char g_unk_71727ba3f8;
void fn_7100d9b7a4() __attribute__((visibility("hidden")));
void fn_7100d9b7b0() __attribute__((visibility("hidden")));
extern UnkConverter g_unk_71727ba400;
extern UnkConverter g_unk_71727ba420;
void fn_7100d9b7bc() __attribute__((visibility("hidden")));
void fn_7100d9b7c8() __attribute__((visibility("hidden")));
extern UnkConverter g_unk_71727ba440;
extern UnkConverter g_unk_71727ba460;
extern UnkReflType g_unk_71727ba480;
void fn_7100d97964();
void fn_7100d97c30(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern char g_unk_71727ba598;
void fn_7100d9b7d4() __attribute__((visibility("hidden")));
void fn_7100d9b7e0() __attribute__((visibility("hidden")));
extern UnkConverter g_unk_71727ba5a0;
extern UnkConverter g_unk_71727ba5c0;
void fn_7100d9b7ec() __attribute__((visibility("hidden")));
void fn_7100d9b7f8() __attribute__((visibility("hidden")));
extern UnkConverter g_unk_71727ba5e0;
extern UnkConverter g_unk_71727ba600;
extern UnkReflType g_unk_71727ba620;
void fn_7100d97db0();
void fn_7100d99934(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
extern char g_unk_71727ba758;
UnkReflType* fn_7100d9af70();  // fn_7100d9af70
extern char g_unk_71727ba759;
UnkReflType* fn_7100d9b04c();  // fn_7100d9b04c
extern char g_unk_71727ba75a;

void fn_7100d9caf0() {
    InitHeader(g_unk_71727ba980);
    InitEnum(&g_unk_71727ba2e0, "ELightType", fn_7100d977e4, fn_7100d975d8);
    fn_710009ec00(&g_unk_71727ba3f8, &g_unk_71727ba2e0);
    InitEnumConverters(&g_unk_71727ba2e0, g_unk_71727ba400, fn_7100d9b7a4, g_unk_71727ba420, fn_7100d9b7b0);
    InitEnumConverters(&g_unk_71727ba2e0, g_unk_71727ba440, fn_7100d9b7bc, g_unk_71727ba460, fn_7100d9b7c8, fn_7100005178);
    InitEnum(&g_unk_71727ba480, "ELightAttachment", fn_7100d97c30, fn_7100d97964);
    fn_710009ec00(&g_unk_71727ba598, &g_unk_71727ba480);
    InitEnumConverters(&g_unk_71727ba480, g_unk_71727ba5a0, fn_7100d9b7d4, g_unk_71727ba5c0, fn_7100d9b7e0);
    InitEnumConverters(&g_unk_71727ba480, g_unk_71727ba5e0, fn_7100d9b7ec, g_unk_71727ba600, fn_7100d9b7f8, fn_7100005178);
    InitClass(&g_unk_71727ba620, "CLightInfo", fn_7100d99934, fn_7100d97db0);
    fn_710009ec00(&g_unk_71727ba758, &g_unk_71727ba620);
    fn_710009ec00(&g_unk_71727ba759, fn_7100d9af70());
    fn_710009ec00(&g_unk_71727ba75a, fn_7100d9b04c());
}

// The TU's data: the header, then a pair of floats (-1, -1) and a flag.
struct UnkLoadingScreenData {
    UnkTUHeader header;
    u8 unk_0x18[0x80];
    f32 field_0x98;
    f32 field_0x9c;
    bool field_0xa0;
};
extern UnkLoadingScreenData g_unk_71727c0870 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_71727c0710;
void fn_7100e0f028();
void fn_7100e0f4d4(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern char g_unk_71727c0828;
void fn_7100e1389c() __attribute__((visibility("hidden")));
void fn_7100e138a8() __attribute__((visibility("hidden")));
extern UnkConverter g_unk_71727c0830;
extern UnkConverter g_unk_71727c0850;

void fn_7100e13f10() {
    InitHeader(g_unk_71727c0870.header);
    g_unk_71727c0870.field_0x98 = -1.0f;
    g_unk_71727c0870.field_0x9c = -1.0f;
    g_unk_71727c0870.field_0xa0 = true;
    InitEnum(&g_unk_71727c0710, "ELoadingScreen", fn_7100e0f4d4, fn_7100e0f028);
    fn_710009ec00(&g_unk_71727c0828, &g_unk_71727c0710);
    InitEnumConverters(&g_unk_71727c0710, g_unk_71727c0830, fn_7100e1389c, g_unk_71727c0850, fn_7100e138a8);
}


extern UnkTUHeaderAndClass g_unk_717279cc60 __attribute__((visibility("hidden")));
void fn_7100cd12b8();  // CCharClassSetCameraNearEvent::refl_GetClass
extern char g_unk_717279caf8;
extern UnkReflType g_unk_717279cb00;
void fn_7100cd17a8();
void fn_7100cd1954(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern char g_unk_717279cc18;
void fn_7100cd1c90() __attribute__((visibility("hidden")));
void fn_7100cd1c9c() __attribute__((visibility("hidden")));
extern UnkConverter g_unk_717279cc20;
extern UnkConverter g_unk_717279cc40;

void fn_7100cd1fe0() {
    InitHeader(g_unk_717279cc60.header);
    fn_7100cd12b8();
    fn_710009ec00(&g_unk_717279caf8, &g_unk_717279cc60.type);
    InitEnum(&g_unk_717279cb00, "CCharClassSetCameraNearEvent::EDir", fn_7100cd1954, fn_7100cd17a8);
    fn_710009ec00(&g_unk_717279cc18, &g_unk_717279cb00);
    InitEnumConverters(&g_unk_717279cb00, g_unk_717279cc20, fn_7100cd1c90, g_unk_717279cc40, fn_7100cd1c9c);
}

extern UnkTUHeaderAndClass g_unk_71727a0898 __attribute__((visibility("hidden")));
void fn_7100ceadf8();  // CCharClassAnalogAimTrack::refl_GetClass
extern char g_unk_71727a0730;
extern UnkReflType g_unk_71727a0738;
void fn_7100ceb15c();
void fn_7100ceb308(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern char g_unk_71727a0850;
void fn_7100ceb4cc() __attribute__((visibility("hidden")));
void fn_7100ceb4d8() __attribute__((visibility("hidden")));
extern UnkConverter g_unk_71727a0858;
extern UnkConverter g_unk_71727a0878;

void fn_7100ceb650() {
    InitHeader(g_unk_71727a0898.header);
    fn_7100ceadf8();
    fn_710009ec00(&g_unk_71727a0730, &g_unk_71727a0898.type);
    InitEnum(&g_unk_71727a0738, "CCharClassAnalogAimTrack::EAbortAnalogAimInput", fn_7100ceb308, fn_7100ceb15c);
    fn_710009ec00(&g_unk_71727a0850, &g_unk_71727a0738);
    InitEnumConverters(&g_unk_71727a0738, g_unk_71727a0858, fn_7100ceb4cc, g_unk_71727a0878, fn_7100ceb4d8);
}
