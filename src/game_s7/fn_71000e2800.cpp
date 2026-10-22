// Registration of the reflected collection types of a unit (CRntVector, CRntSmallDictionary and
// so on, with their iterators' value types): each type getter's result is added to its list
// (fn_710009ec00), each class object is built from its name (a CClass constructor), its
// destructor registered and the object added to its list, and conversions between types are
// registered (helpers in UnkStaticInitS7.h). Every global is reached through the GOT. Layouts are
// placeholders.
#include "UnkStaticInitS7.h"

using namespace unk_static_init;

extern char g_unk_7101d5bddc[];
UnkEnumType* fn_710000fd7c();
extern char g_unk_7101d5bddd[];
UnkEnumType* fn_71000a6eb0();
extern char g_unk_7101d5bdde[];
UnkEnumType* fn_71000a6f8c();
extern char g_unk_7101d5bddf[];
UnkEnumType* fn_7100007f84();
extern char g_unk_7101d5bde0[];
UnkEnumType* fn_71000a706c();
extern char g_unk_7101d5bde1[];
UnkEnumType* fn_710009f200();
extern char g_unk_7101d5bde2[];
UnkEnumType* fn_710009f69c();
extern char g_unk_7101d5bde3[];
UnkEnumType* fn_710009fb38();
extern char g_unk_7101d5bde4[];
UnkEnumType* fn_710009ffd4();
extern char g_unk_7101d5bde5[];
UnkEnumType* fn_71000a0318();
extern char g_unk_7101d5bde6[];
UnkEnumType* fn_7100009658();
extern char g_unk_7101d5bde7[];
UnkEnumType* fn_71000a74a8();
extern char g_unk_7101d5bde8[];
UnkEnumType* fn_71000a7584();
extern char g_unk_7101d5bde9[];
UnkEnumType* fn_710000ad9c();
extern char g_unk_7101d5bdea[];
UnkEnumType* fn_71000a7664();
extern char g_unk_7101d5bdeb[];
UnkEnumType* fn_71000a7740();
extern char g_unk_7101d5bdec[];
UnkEnumType* fn_7100024504();
extern char g_unk_7101d5bded[];
UnkEnumType* fn_71000246e4();
extern char g_unk_7101d5bdee[];
UnkEnumType* fn_71000248c4();
extern char g_unk_7101d5bdef[];
UnkEnumType* fn_71000a7828();
extern char g_unk_7101d5bdf0[];
UnkEnumType* fn_71000a7904();
extern char g_unk_7101d5bdf8[];
extern char g_unk_7101d5e2e0[];
void fn_71000a79e0();
void fn_71000a7d64(void* obj, const CStrId& name, void* base, void* a, void (*factory)(), void* b);
void fn_7100096604(void*);
extern char g_unk_7101d5bf30[];
extern char g_unk_7101d5bf31[];
UnkEnumType* fn_71000a7ee8();
extern char g_unk_7101d5bf38[];
void fn_71000a7fc4();
void fn_71000a8348(void* obj, const CStrId& name, void* base, void* a, void (*factory)(), void* b);
extern char g_unk_7101d5c070[];
extern char g_unk_7101d5c071[];
UnkEnumType* fn_71000a84cc();
extern char g_unk_7101d5c078[];
void fn_71000a85a8(void* obj, const CStrId& name, void* a, void* b);
void fn_710009891c(void*);
extern char g_unk_7101d5c1e8[];
extern char g_unk_7101d5c1e9[];
UnkEnumType* fn_7100067f5c();
extern char g_unk_7101d5c1ea[];
UnkEnumType* fn_7100068204();
extern char g_unk_7101d5c1eb[];
UnkEnumType* fn_71000a41b4();
extern char g_unk_7101d5c1ec[];
UnkEnumType* fn_71000a87a4();
void fn_71000cc6c0();
extern UnkConversion g_unk_7101d5c1f0;
void fn_71000cc730();
extern UnkConversion g_unk_7101d5c210;
UnkEnumType* fn_7100005178();
void fn_71000cc7b0();
extern UnkConversion g_unk_7101d5c230;
void fn_71000cc7c4();
extern UnkConversion g_unk_7101d5c250;
void fn_71000cc7d8();
extern UnkConversion g_unk_7101d5c270;
void fn_71000cc7ec();
extern UnkConversion g_unk_7101d5c290;
UnkEnumType* fn_71000081e8();
void fn_71000a8990();
extern UnkConversion g_unk_7101d5c2b0;
void fn_71000cc800();
extern UnkConversion g_unk_7101d5c2d0;
void fn_71000cc890();
extern UnkConversion g_unk_7101d5c2f0;
void fn_71000cc920();
extern UnkConversion g_unk_7101d5c310;
void fn_71000cc9a4();
extern UnkConversion g_unk_7101d5c330;
void fn_71000cca28();
extern UnkConversion g_unk_7101d5c350;
void fn_71000ccaac();
extern UnkConversion g_unk_7101d5c370;
void fn_71000ccb38();
extern UnkConversion g_unk_7101d5c390;
void fn_71000ccbc4();
extern UnkConversion g_unk_7101d5c3b0;
void fn_71000ccc10();
extern UnkConversion g_unk_7101d5c3d0;
void fn_71000ccc5c();
extern UnkConversion g_unk_7101d5c3f0;
void fn_71000ccca8();
extern UnkConversion g_unk_7101d5c410;
void fn_71000ccd38();
extern UnkConversion g_unk_7101d5c430;
void fn_71000ccd84();
extern UnkConversion g_unk_7101d5c450;
void fn_71000a89b0();
extern UnkConversion g_unk_7101d5c470;
void fn_71000cce14();
extern UnkConversion g_unk_7101d5c490;
void fn_71000a89d0();
extern UnkConversion g_unk_7101d5c4b0;
void fn_71000a8a10();
extern UnkConversion g_unk_7101d5c4d0;
void fn_71000a8a50();
extern UnkConversion g_unk_7101d5c4f0;
void fn_71000bc608() __attribute__((visibility("hidden")));
extern UnkConversion g_unk_7101d5c510;
void fn_71000cce60();
extern UnkConversion g_unk_7101d5c530;
void fn_71000a8a90();
extern UnkConversion g_unk_7101d5c550;
void fn_71000a8ad0();
extern UnkConversion g_unk_7101d5c570;
void fn_71000a8b10();
extern UnkConversion g_unk_7101d5c590;
void fn_71000bc638() __attribute__((visibility("hidden")));
extern UnkConversion g_unk_7101d5c5b0;
void fn_71000ccf08();
extern UnkConversion g_unk_7101d5c5d0;
void fn_71000ccfb0();
extern UnkConversion g_unk_7101d5c5f0;
void fn_71000cd000();
extern UnkConversion g_unk_7101d5c610;
void fn_71000cd084();
extern UnkConversion g_unk_7101d5c630;
void fn_71000cd108();
extern UnkConversion g_unk_7101d5c650;
void fn_71000cd18c();
extern UnkConversion g_unk_7101d5c670;
void fn_71000cd218();
extern UnkConversion g_unk_7101d5c690;
void fn_71000cd2a4();
extern UnkConversion g_unk_7101d5c6b0;
void fn_71000cd338();
extern UnkConversion g_unk_7101d5c6d0;
UnkEnumType* fn_7100010e58();
void fn_71000cd4b8();
extern UnkConversion g_unk_7101d5c6f0;
extern char g_unk_7101d5c710[];
UnkEnumType* fn_71000a8b4c();
extern char g_unk_7101d5c711[];
UnkEnumType* fn_71000a8c28();
extern char g_unk_7101d5c718[];
void fn_71000a8d04();
void fn_71000a9088(void* obj, const CStrId& name, void* base, void* a, void (*factory)(), void* b);
extern char g_unk_7101d5c850[];
extern char g_unk_7101d5c851[];
UnkEnumType* fn_71000a920c();
extern char g_unk_7101d5c858[];
void fn_71000a92e8();
void fn_71000a966c(void* obj, const CStrId& name, void* base, void* a, void (*factory)(), void* b);
extern char g_unk_7101d5c990[];
extern char g_unk_7101d5c991[];
UnkEnumType* fn_71000a97f0();
extern char g_unk_7101d5c998[];
void fn_71000a98cc(void* obj, const CStrId& name, void* a, void* b);
extern char g_unk_7101d5cb08[];
extern char g_unk_7101d5cb09[];
UnkEnumType* fn_71000a9abc();
extern char g_unk_7101d5cb0a[];
UnkEnumType* fn_71000a9b98();
extern char g_unk_7101d5cb10[];
void fn_71000a9c74();
void fn_71000a9ff8(void* obj, const CStrId& name, void* base, void* a, void (*factory)(), void* b);
extern char g_unk_7101d5cc48[];
extern char g_unk_7101d5cc49[];
UnkEnumType* fn_71000aa17c();
extern char g_unk_7101d5cc50[];
void fn_71000aa258();
void fn_71000aa5dc(void* obj, const CStrId& name, void* base, void* a, void (*factory)(), void* b);
extern char g_unk_7101d5cd88[];
extern char g_unk_7101d5cd89[];
UnkEnumType* fn_71000aa760();
extern char g_unk_7101d5cd90[];
void fn_71000aa83c(void* obj, const CStrId& name, void* a, void* b);
extern char g_unk_7101d5cf00[];
extern char g_unk_7101d5cf01[];
UnkEnumType* fn_71000aaa2c();
extern char g_unk_7101d5cf02[];
UnkEnumType* fn_71000aab08();
extern char g_unk_7101d5cf08[];
void fn_71000aabe4();
void fn_71000aaf68(void* obj, const CStrId& name, void* base, void* a, void (*factory)(), void* b);
extern char g_unk_7101d5d040[];
extern char g_unk_7101d5d041[];
UnkEnumType* fn_71000ab0ec();
extern char g_unk_7101d5d048[];
void fn_71000ab1c8();
void fn_71000ab54c(void* obj, const CStrId& name, void* base, void* a, void (*factory)(), void* b);
extern char g_unk_7101d5d180[];
extern char g_unk_7101d5d181[];
UnkEnumType* fn_71000ab6d0();
extern char g_unk_7101d5d188[];
void fn_71000ab7ac(void* obj, const CStrId& name, void* a, void* b);
extern char g_unk_7101d5d2f8[];
extern char g_unk_7101d5d2f9[];
UnkEnumType* fn_71000ab99c();
extern char g_unk_7101d5d2fa[];
UnkEnumType* fn_71000aba78();
extern char g_unk_7101d5d300[];
void fn_71000abb54();
void fn_71000abed8(void* obj, const CStrId& name, void* base, void* a, void (*factory)(), void* b);
extern char g_unk_7101d5d438[];
extern char g_unk_7101d5d439[];
UnkEnumType* fn_71000ac05c();
extern char g_unk_7101d5d440[];
void fn_71000ac138();
void fn_71000ac4bc(void* obj, const CStrId& name, void* base, void* a, void (*factory)(), void* b);
extern char g_unk_7101d5d578[];
extern char g_unk_7101d5d579[];
UnkEnumType* fn_71000ac640();
extern char g_unk_7101d5d580[];
void fn_71000ac71c(void* obj, const CStrId& name, void* a, void* b);
extern char g_unk_7101d5d6f0[];
extern char g_unk_7101d5d6f1[];
UnkEnumType* fn_71000ac90c();
extern char g_unk_7101d5d6f2[];
UnkEnumType* fn_71000ac9e8();
extern char g_unk_7101d5d6f8[];
void fn_71000acac4();
void fn_71000ace48(void* obj, const CStrId& name, void* base, void* a, void (*factory)(), void* b);
extern char g_unk_7101d5d830[];
extern char g_unk_7101d5d831[];
UnkEnumType* fn_71000acfcc();
extern char g_unk_7101d5d838[];
void fn_71000ad0a8();
void fn_71000ad42c(void* obj, const CStrId& name, void* base, void* a, void (*factory)(), void* b);
extern char g_unk_7101d5d970[];
extern char g_unk_7101d5d971[];
UnkEnumType* fn_71000ad5b0();
extern char g_unk_7101d5d978[];
void fn_71000ad68c(void* obj, const CStrId& name, void* a, void* b);
extern char g_unk_7101d5dae8[];
extern char g_unk_7101d5dae9[];
UnkEnumType* fn_71000ad87c();
extern char g_unk_7101d5daea[];
UnkEnumType* fn_71000ad958();
extern char g_unk_7101d5daf0[];
void fn_71000ada34();
void fn_71000addb8(void* obj, const CStrId& name, void* base, void* a, void (*factory)(), void* b);
extern char g_unk_7101d5dc28[];
extern char g_unk_7101d5dc29[];
UnkEnumType* fn_71000adf3c();
extern char g_unk_7101d5dc30[];
void fn_71000ae018();
void fn_71000ae39c(void* obj, const CStrId& name, void* base, void* a, void (*factory)(), void* b);
extern char g_unk_7101d5dd68[];
extern char g_unk_7101d5dd69[];
UnkEnumType* fn_71000ae520();
extern char g_unk_7101d5dd70[];
void fn_71000ae5fc(void* obj, const CStrId& name, void* a, void* b);
extern char g_unk_7101d5dee0[];
extern char g_unk_7101d5dee1[];
UnkEnumType* fn_71000ae7ec();
extern char g_unk_7101d5dee2[];
UnkEnumType* fn_71000ae8c8();
extern char g_unk_7101d5dee8[];
void fn_71000ae9a4();
void fn_71000aed28(void* obj, const CStrId& name, void* base, void* a, void (*factory)(), void* b);
extern char g_unk_7101d5e020[];
extern char g_unk_7101d5e021[];
UnkEnumType* fn_71000aeeac();
extern char g_unk_7101d5e028[];
void fn_71000aef88();
void fn_71000af30c(void* obj, const CStrId& name, void* base, void* a, void (*factory)(), void* b);
extern char g_unk_7101d5e160[];
extern char g_unk_7101d5e161[];
UnkEnumType* fn_71000af490();
extern char g_unk_7101d5e168[];
void fn_71000af56c(void* obj, const CStrId& name, void* a, void* b);
extern char g_unk_7101d5e2d8[];
void fn_71000af75c();
void fn_71000af760(void* obj, const CStrId& name, void* base, void* a, void (*factory)(), void* b);
extern char g_unk_7101d5e418[];
void fn_71000c18d8() __attribute__((visibility("hidden")));
extern UnkConversion g_unk_7101d5e420;
extern char g_unk_7101d5e440[];
UnkEnumType* fn_710009d884();
extern char g_unk_7101d5e441[];
UnkEnumType* fn_71000af8e0();
extern char g_unk_7101d5e442[];
UnkEnumType* fn_71000af9bc();
extern char g_unk_7101d5e443[];
UnkEnumType* fn_71000afa98();
extern char g_unk_7101d5e448[];
void fn_71000afb74();
void fn_71000afef8(void* obj, const CStrId& name, void* base, void* a, void (*factory)(), void* b);
extern char g_unk_7101d5e580[];
extern char g_unk_7101d5e581[];
UnkEnumType* fn_71000b007c();
extern char g_unk_7101d5e588[];
void fn_71000b0158();
void fn_71000b04dc(void* obj, const CStrId& name, void* base, void* a, void (*factory)(), void* b);
extern char g_unk_7101d5e6c0[];
extern char g_unk_7101d5e6c1[];
UnkEnumType* fn_71000b0660();
extern char g_unk_7101d5e6c8[];
void fn_71000b073c(void* obj, const CStrId& name, void* a, void* b);
extern char g_unk_7101d5e838[];
extern char g_unk_7101d5e839[];
UnkEnumType* fn_71000b092c();
extern char g_unk_7101d5e83a[];
UnkEnumType* fn_71000b0a08();
extern char g_unk_7101d5e840[];
void fn_71000b0ae4();
void fn_71000b0e68(void* obj, const CStrId& name, void* base, void* a, void (*factory)(), void* b);
extern char g_unk_7101d5e978[];
extern char g_unk_7101d5e979[];
UnkEnumType* fn_71000b0fec();
extern char g_unk_7101d5e980[];
void fn_71000b10c8();
void fn_71000b144c(void* obj, const CStrId& name, void* base, void* a, void (*factory)(), void* b);
extern char g_unk_7101d5eab8[];
extern char g_unk_7101d5eab9[];
UnkEnumType* fn_71000b15d0();
extern char g_unk_7101d5eac0[];
void fn_71000b16ac(void* obj, const CStrId& name, void* a, void* b);
extern char g_unk_7101d5ec30[];
extern char g_unk_7101d5ec31[];
UnkEnumType* fn_71000b187c();
extern char g_unk_7101d5ec32[];
UnkEnumType* fn_71000b1958();
extern char g_unk_7101d5ec38[];
void fn_71000b1a34();
void fn_71000b1db8(void* obj, const CStrId& name, void* base, void* a, void (*factory)(), void* b);
extern char g_unk_7101d5ed70[];
extern char g_unk_7101d5ed71[];
UnkEnumType* fn_71000b1f3c();
extern char g_unk_7101d5ed78[];
void fn_71000b2018();
void fn_71000b239c(void* obj, const CStrId& name, void* base, void* a, void (*factory)(), void* b);
extern char g_unk_7101d5eeb0[];
extern char g_unk_7101d5eeb1[];
UnkEnumType* fn_71000b2520();
extern char g_unk_7101d5eeb8[];
void fn_71000b25fc(void* obj, const CStrId& name, void* a, void* b);
extern char g_unk_7101d5f028[];
extern char g_unk_7101d5f029[];
UnkEnumType* fn_71000b27f0();
extern char g_unk_7101d5f02a[];
UnkEnumType* fn_71000b28cc();
extern char g_unk_7101d5f030[];
void fn_71000b29a8();
void fn_71000b2d2c(void* obj, const CStrId& name, void* base, void* a, void (*factory)(), void* b);
extern char g_unk_7101d5f168[];
extern char g_unk_7101d5f169[];
UnkEnumType* fn_71000b2eb0();
extern char g_unk_7101d5f170[];
void fn_71000b2f8c();
void fn_71000b3310(void* obj, const CStrId& name, void* base, void* a, void (*factory)(), void* b);
extern char g_unk_7101d5f2a8[];
extern char g_unk_7101d5f2a9[];
UnkEnumType* fn_71000b3494();
extern char g_unk_7101d5f2b0[];
void fn_71000b3570(void* obj, const CStrId& name, void* a, void* b);
extern char g_unk_7101d5f420[];
extern char g_unk_7101d5f421[];
UnkEnumType* fn_71000b3764();
extern char g_unk_7101d5f422[];
UnkEnumType* fn_71000b3840();
extern char g_unk_7101d5f428[];
void fn_71000b391c();
void fn_71000b3ca0(void* obj, const CStrId& name, void* base, void* a, void (*factory)(), void* b);
extern char g_unk_7101d5f560[];
extern char g_unk_7101d5f561[];
UnkEnumType* fn_71000b3e24();
extern char g_unk_7101d5f568[];
void fn_71000b3f00();
void fn_71000b4284(void* obj, const CStrId& name, void* base, void* a, void (*factory)(), void* b);
extern char g_unk_7101d5f6a0[];
extern char g_unk_7101d5f6a1[];
UnkEnumType* fn_71000b4408();
extern char g_unk_7101d5f6a8[];
void fn_71000b44e4(void* obj, const CStrId& name, void* a, void* b);
extern char g_unk_7101d5f818[];
extern char g_unk_7101d5f819[];
UnkEnumType* fn_71000b46d8();
extern char g_unk_7101d5f81a[];
UnkEnumType* fn_71000b47b4();
extern char g_unk_7101d5f81b[];
UnkEnumType* fn_71000b4890();
extern char g_unk_7101d5f81c[];
UnkEnumType* fn_71000b496c();
extern char g_unk_7101d5f81d[];
UnkEnumType* fn_71000b4a48();
extern char g_unk_7101d5f81e[];
UnkEnumType* fn_71000b4b24();
extern char g_unk_7101d5f820[];
void fn_71000b4c00();
void fn_71000b4f84(void* obj, const CStrId& name, void* base, void* a, void (*factory)(), void* b);
extern char g_unk_7101d5f958[];
extern char g_unk_7101d5f959[];
UnkEnumType* fn_71000b5108();
extern char g_unk_7101d5f960[];
void fn_71000b51e4();
void fn_71000b5568(void* obj, const CStrId& name, void* base, void* a, void (*factory)(), void* b);
extern char g_unk_7101d5fa98[];
extern char g_unk_7101d5fa99[];
UnkEnumType* fn_71000b56ec();
extern char g_unk_7101d5faa0[];
void fn_71000b57c8(void* obj, const CStrId& name, void* a, void* b);
extern char g_unk_7101d5fc10[];
extern char g_unk_7101d5fc11[];
UnkEnumType* fn_71000b59c0();
extern char g_unk_7101d5fc12[];
UnkEnumType* fn_71000b5a9c();
extern char g_unk_7101d5fc18[];
void fn_71000b5b78();
void fn_71000b5efc(void* obj, const CStrId& name, void* base, void* a, void (*factory)(), void* b);
extern char g_unk_7101d5fd50[];
extern char g_unk_7101d5fd51[];
UnkEnumType* fn_71000b6080();
extern char g_unk_7101d5fd58[];
void fn_71000b615c();
void fn_71000b64e0(void* obj, const CStrId& name, void* base, void* a, void (*factory)(), void* b);
extern char g_unk_7101d5fe90[];
extern char g_unk_7101d5fe91[];
UnkEnumType* fn_71000b6664();
extern char g_unk_7101d5fe98[];
void fn_71000b6740(void* obj, const CStrId& name, void* a, void* b);
extern char g_unk_7101d60008[];
extern char g_unk_7101d60009[];
UnkEnumType* fn_71000b6938();
extern char g_unk_7101d6000a[];
UnkEnumType* fn_71000b6a14();
extern char g_unk_7101d60010[];
void fn_71000b6af0();
void fn_71000b6e74(void* obj, const CStrId& name, void* base, void* a, void (*factory)(), void* b);
extern char g_unk_7101d60148[];
extern char g_unk_7101d60149[];
UnkEnumType* fn_71000b6ff8();
extern char g_unk_7101d60150[];
void fn_71000b70d4();
void fn_71000b7458(void* obj, const CStrId& name, void* base, void* a, void (*factory)(), void* b);
extern char g_unk_7101d60288[];
extern char g_unk_7101d60289[];
UnkEnumType* fn_71000b75dc();
extern char g_unk_7101d60290[];
void fn_71000b76b8(void* obj, const CStrId& name, void* a, void* b);
extern char g_unk_7101d60400[];
extern char g_unk_7101d60401[];
UnkEnumType* fn_71000b78ac();
extern char g_unk_7101d60402[];
UnkEnumType* fn_71000b7988();
extern char g_unk_7101d60408[];
void fn_71000b7a64();
void fn_71000b7de8(void* obj, const CStrId& name, void* base, void* a, void (*factory)(), void* b);
extern char g_unk_7101d60540[];
extern char g_unk_7101d60541[];
UnkEnumType* fn_71000b7f6c();
extern char g_unk_7101d60548[];
void fn_71000b8048();
void fn_71000b83cc(void* obj, const CStrId& name, void* base, void* a, void (*factory)(), void* b);
extern char g_unk_7101d60680[];
extern char g_unk_7101d60681[];
UnkEnumType* fn_71000b8550();
extern char g_unk_7101d60688[];
void fn_71000b862c(void* obj, const CStrId& name, void* a, void* b);
extern char g_unk_7101d607f8[];
extern char g_unk_7101d607f9[];
UnkEnumType* fn_71000b8820();
extern char g_unk_7101d607fa[];
UnkEnumType* fn_71000b88fc();
extern char g_unk_7101d60800[];
void fn_71000b89d8();
void fn_71000b8d5c(void* obj, const CStrId& name, void* base, void* a, void (*factory)(), void* b);
extern char g_unk_7101d60938[];
extern char g_unk_7101d60939[];
UnkEnumType* fn_71000b8ee0();
extern char g_unk_7101d60940[];
void fn_71000b8fbc();
void fn_71000b9340(void* obj, const CStrId& name, void* base, void* a, void (*factory)(), void* b);
extern char g_unk_7101d60a78[];
extern char g_unk_7101d60a79[];
UnkEnumType* fn_71000b94c4();
extern char g_unk_7101d60a80[];
void fn_71000b95a0(void* obj, const CStrId& name, void* a, void* b);
extern char g_unk_7101d60bf0[];
extern char g_unk_7101d60bf1[];
UnkEnumType* fn_71000b9790();
extern char g_unk_7101d60bf2[];
UnkEnumType* fn_71000b986c();
extern char g_unk_7101d60bf8[];
void fn_71000b9948();
void fn_71000b9ccc(void* obj, const CStrId& name, void* base, void* a, void (*factory)(), void* b);
extern char g_unk_7101d60d30[];
extern char g_unk_7101d60d31[];
UnkEnumType* fn_71000b9e50();
extern char g_unk_7101d60d38[];
void fn_71000b9f2c();
void fn_71000ba2b0(void* obj, const CStrId& name, void* base, void* a, void (*factory)(), void* b);
extern char g_unk_7101d60e70[];
extern char g_unk_7101d60e71[];
UnkEnumType* fn_71000ba434();
extern char g_unk_7101d60e78[];
void fn_71000ba510(void* obj, const CStrId& name, void* a, void* b);
extern char g_unk_7101d60fe8[];
extern char g_unk_7101d60ff0[];
void fn_71000ba700();
void fn_71000ba704(void* obj, const CStrId& name, void* base, void* a, void (*factory)(), void* b);
extern char g_unk_7101d61128[];
extern char g_unk_7101d61130[];
void fn_71000ba880();
void fn_71000ba884(void* obj, const CStrId& name, void* base, void* a, void (*factory)(), void* b);
extern char g_unk_7101d61268[];
void fn_71000e23ac();
extern UnkConversion g_unk_7101d61270;
void fn_71000e24b0();
extern UnkConversion g_unk_7101d61290;
void fn_71000e26f8();
extern UnkConversion g_unk_7101d612b0;

void fn_71000e2800() {
    fn_710009ec00(g_unk_7101d5bddc, fn_710000fd7c());
    fn_710009ec00(g_unk_7101d5bddd, fn_71000a6eb0());
    fn_710009ec00(g_unk_7101d5bdde, fn_71000a6f8c());
    fn_710009ec00(g_unk_7101d5bddf, fn_7100007f84());
    fn_710009ec00(g_unk_7101d5bde0, fn_71000a706c());
    fn_710009ec00(g_unk_7101d5bde1, fn_710009f200());
    fn_710009ec00(g_unk_7101d5bde2, fn_710009f69c());
    fn_710009ec00(g_unk_7101d5bde3, fn_710009fb38());
    fn_710009ec00(g_unk_7101d5bde4, fn_710009ffd4());
    fn_710009ec00(g_unk_7101d5bde5, fn_71000a0318());
    fn_710009ec00(g_unk_7101d5bde6, fn_7100009658());
    fn_710009ec00(g_unk_7101d5bde7, fn_71000a74a8());
    fn_710009ec00(g_unk_7101d5bde8, fn_71000a7584());
    fn_710009ec00(g_unk_7101d5bde9, fn_710000ad9c());
    fn_710009ec00(g_unk_7101d5bdea, fn_71000a7664());
    fn_710009ec00(g_unk_7101d5bdeb, fn_71000a7740());
    fn_710009ec00(g_unk_7101d5bdec, fn_7100024504());
    fn_710009ec00(g_unk_7101d5bded, fn_71000246e4());
    fn_710009ec00(g_unk_7101d5bdee, fn_71000248c4());
    fn_710009ec00(g_unk_7101d5bdef, fn_71000a7828());
    fn_710009ec00(g_unk_7101d5bdf0, fn_71000a7904());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<base::math::CVector3D>>::TKeyElementIterator::value_type", true);
        fn_71000a7d64(g_unk_7101d5bdf8, n, g_unk_7101d5e2e0, nullptr, fn_71000a79e0, nullptr);
    }
    __cxa_atexit(fn_7100096604, g_unk_7101d5bdf8, g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d5bf30, g_unk_7101d5bdf8);
    fn_710009ec00(g_unk_7101d5bf31, fn_71000a7ee8());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<base::math::CVector3D>>::TKeyElementConstIterator::value_type", true);
        fn_71000a8348(g_unk_7101d5bf38, n, g_unk_7101d5e2e0, nullptr, fn_71000a7fc4, nullptr);
    }
    __cxa_atexit(fn_7100096604, g_unk_7101d5bf38, g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d5c070, g_unk_7101d5bf38);
    fn_710009ec00(g_unk_7101d5c071, fn_71000a84cc());
    {
        CStrId n("base::global::CRntVector<base::math::CVector3D>", true);
        fn_71000a85a8(g_unk_7101d5c078, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, g_unk_7101d5c078, g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d5c1e8, g_unk_7101d5c078);
    fn_710009ec00(g_unk_7101d5c1e9, fn_7100067f5c());
    fn_710009ec00(g_unk_7101d5c1ea, fn_7100068204());
    fn_710009ec00(g_unk_7101d5c1eb, fn_71000a41b4());
    fn_710009ec00(g_unk_7101d5c1ec, fn_71000a87a4());
    InitConversion(&g_unk_7101d5c1f0, fn_71000246e4, fn_7100067f5c, fn_71000cc6c0);
    InitConversion(&g_unk_7101d5c210, fn_71000248c4, fn_7100068204, fn_71000cc730);
    InitConversion(&g_unk_7101d5c230, fn_7100005178, fn_71000a41b4, fn_71000cc7b0);
    InitConversion(&g_unk_7101d5c250, fn_7100005178, fn_71000a87a4, fn_71000cc7c4);
    InitConversion(&g_unk_7101d5c270, fn_71000a41b4, fn_71000a87a4, fn_71000cc7d8);
    InitConversion(&g_unk_7101d5c290, fn_71000a87a4, fn_71000a41b4, fn_71000cc7ec);
    InitConversion(&g_unk_7101d5c2b0, fn_7100007f84, fn_71000081e8, fn_71000a8990);
    InitConversion(&g_unk_7101d5c2d0, fn_7100007f84, fn_7100009658, fn_71000cc800);
    InitConversion(&g_unk_7101d5c2f0, fn_7100007f84, fn_710000ad9c, fn_71000cc890);
    InitConversion(&g_unk_7101d5c310, fn_7100007f84, fn_710009f200, fn_71000cc920);
    InitConversion(&g_unk_7101d5c330, fn_7100007f84, fn_710009f69c, fn_71000cc9a4);
    InitConversion(&g_unk_7101d5c350, fn_7100007f84, fn_710009fb38, fn_71000cca28);
    InitConversion(&g_unk_7101d5c370, fn_7100007f84, fn_710009ffd4, fn_71000ccaac);
    InitConversion(&g_unk_7101d5c390, fn_7100007f84, fn_71000a0318, fn_71000ccb38);
    InitConversion(&g_unk_7101d5c3b0, fn_710009f200, fn_7100007f84, fn_71000ccbc4);
    InitConversion(&g_unk_7101d5c3d0, fn_710009f69c, fn_7100007f84, fn_71000ccc10);
    InitConversion(&g_unk_7101d5c3f0, fn_710009fb38, fn_7100007f84, fn_71000ccc5c);
    InitConversion(&g_unk_7101d5c410, fn_710009fb38, fn_7100009658, fn_71000ccca8);
    InitConversion(&g_unk_7101d5c430, fn_710009ffd4, fn_7100007f84, fn_71000ccd38);
    InitConversion(&g_unk_7101d5c450, fn_710009ffd4, fn_7100009658, fn_71000ccd84);
    InitConversion(&g_unk_7101d5c470, fn_710009ffd4, fn_71000081e8, fn_71000a89b0);
    InitConversion(&g_unk_7101d5c490, fn_71000a0318, fn_7100007f84, fn_71000cce14);
    InitConversion(&g_unk_7101d5c4b0, fn_7100009658, fn_7100007f84, fn_71000a89d0);
    InitConversion(&g_unk_7101d5c4d0, fn_7100009658, fn_710009fb38, fn_71000a8a10);
    InitConversion(&g_unk_7101d5c4f0, fn_7100009658, fn_710009ffd4, fn_71000a8a50);
    InitConversion(&g_unk_7101d5c510, fn_7100009658, fn_71000081e8, fn_71000bc608);
    InitConversion(&g_unk_7101d5c530, fn_7100009658, fn_710000ad9c, fn_71000cce60);
    InitConversion(&g_unk_7101d5c550, fn_710000ad9c, fn_7100007f84, fn_71000a8a90);
    InitConversion(&g_unk_7101d5c570, fn_710000ad9c, fn_710009fb38, fn_71000a8ad0);
    InitConversion(&g_unk_7101d5c590, fn_710000ad9c, fn_710009ffd4, fn_71000a8b10);
    InitConversion(&g_unk_7101d5c5b0, fn_710000ad9c, fn_71000081e8, fn_71000bc638);
    InitConversion(&g_unk_7101d5c5d0, fn_710000ad9c, fn_7100009658, fn_71000ccf08);
    InitConversion(&g_unk_7101d5c5f0, fn_71000081e8, fn_7100007f84, fn_71000ccfb0);
    InitConversion(&g_unk_7101d5c610, fn_71000081e8, fn_710009f200, fn_71000cd000);
    InitConversion(&g_unk_7101d5c630, fn_71000081e8, fn_710009f69c, fn_71000cd084);
    InitConversion(&g_unk_7101d5c650, fn_71000081e8, fn_710009fb38, fn_71000cd108);
    InitConversion(&g_unk_7101d5c670, fn_71000081e8, fn_710009ffd4, fn_71000cd18c);
    InitConversion(&g_unk_7101d5c690, fn_71000081e8, fn_71000a0318, fn_71000cd218);
    InitConversion(&g_unk_7101d5c6b0, fn_71000081e8, fn_7100009658, fn_71000cd2a4);
    InitConversion(&g_unk_7101d5c6d0, fn_71000081e8, fn_710000ad9c, fn_71000cd338);
    InitConversion(&g_unk_7101d5c6f0, fn_7100010e58, fn_710000fd7c, fn_71000cd4b8);
    fn_710009ec00(g_unk_7101d5c710, fn_71000a8b4c());
    fn_710009ec00(g_unk_7101d5c711, fn_71000a8c28());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<base::global::CStrId>>::TKeyElementIterator::value_type", true);
        fn_71000a9088(g_unk_7101d5c718, n, g_unk_7101d5e2e0, nullptr, fn_71000a8d04, nullptr);
    }
    __cxa_atexit(fn_7100096604, g_unk_7101d5c718, g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d5c850, g_unk_7101d5c718);
    fn_710009ec00(g_unk_7101d5c851, fn_71000a920c());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<base::global::CStrId>>::TKeyElementConstIterator::value_type", true);
        fn_71000a966c(g_unk_7101d5c858, n, g_unk_7101d5e2e0, nullptr, fn_71000a92e8, nullptr);
    }
    __cxa_atexit(fn_7100096604, g_unk_7101d5c858, g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d5c990, g_unk_7101d5c858);
    fn_710009ec00(g_unk_7101d5c991, fn_71000a97f0());
    {
        CStrId n("base::global::CRntVector<base::global::CStrId>", true);
        fn_71000a98cc(g_unk_7101d5c998, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, g_unk_7101d5c998, g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d5cb08, g_unk_7101d5c998);
    fn_710009ec00(g_unk_7101d5cb09, fn_71000a9abc());
    fn_710009ec00(g_unk_7101d5cb0a, fn_71000a9b98());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<base::global::CName>>::TKeyElementIterator::value_type", true);
        fn_71000a9ff8(g_unk_7101d5cb10, n, g_unk_7101d5e2e0, nullptr, fn_71000a9c74, nullptr);
    }
    __cxa_atexit(fn_7100096604, g_unk_7101d5cb10, g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d5cc48, g_unk_7101d5cb10);
    fn_710009ec00(g_unk_7101d5cc49, fn_71000aa17c());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<base::global::CName>>::TKeyElementConstIterator::value_type", true);
        fn_71000aa5dc(g_unk_7101d5cc50, n, g_unk_7101d5e2e0, nullptr, fn_71000aa258, nullptr);
    }
    __cxa_atexit(fn_7100096604, g_unk_7101d5cc50, g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d5cd88, g_unk_7101d5cc50);
    fn_710009ec00(g_unk_7101d5cd89, fn_71000aa760());
    {
        CStrId n("base::global::CRntVector<base::global::CName>", true);
        fn_71000aa83c(g_unk_7101d5cd90, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, g_unk_7101d5cd90, g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d5cf00, g_unk_7101d5cd90);
    fn_710009ec00(g_unk_7101d5cf01, fn_71000aaa2c());
    fn_710009ec00(g_unk_7101d5cf02, fn_71000aab08());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<base::global::CFilePathStrId>>::TKeyElementIterator::value_type", true);
        fn_71000aaf68(g_unk_7101d5cf08, n, g_unk_7101d5e2e0, nullptr, fn_71000aabe4, nullptr);
    }
    __cxa_atexit(fn_7100096604, g_unk_7101d5cf08, g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d5d040, g_unk_7101d5cf08);
    fn_710009ec00(g_unk_7101d5d041, fn_71000ab0ec());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<base::global::CFilePathStrId>>::TKeyElementConstIterator::value_type", true);
        fn_71000ab54c(g_unk_7101d5d048, n, g_unk_7101d5e2e0, nullptr, fn_71000ab1c8, nullptr);
    }
    __cxa_atexit(fn_7100096604, g_unk_7101d5d048, g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d5d180, g_unk_7101d5d048);
    fn_710009ec00(g_unk_7101d5d181, fn_71000ab6d0());
    {
        CStrId n("base::global::CRntVector<base::global::CFilePathStrId>", true);
        fn_71000ab7ac(g_unk_7101d5d188, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, g_unk_7101d5d188, g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d5d2f8, g_unk_7101d5d188);
    fn_710009ec00(g_unk_7101d5d2f9, fn_71000ab99c());
    fn_710009ec00(g_unk_7101d5d2fa, fn_71000aba78());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<unsigned short>>::TKeyElementIterator::value_type", true);
        fn_71000abed8(g_unk_7101d5d300, n, g_unk_7101d5e2e0, nullptr, fn_71000abb54, nullptr);
    }
    __cxa_atexit(fn_7100096604, g_unk_7101d5d300, g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d5d438, g_unk_7101d5d300);
    fn_710009ec00(g_unk_7101d5d439, fn_71000ac05c());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<unsigned short>>::TKeyElementConstIterator::value_type", true);
        fn_71000ac4bc(g_unk_7101d5d440, n, g_unk_7101d5e2e0, nullptr, fn_71000ac138, nullptr);
    }
    __cxa_atexit(fn_7100096604, g_unk_7101d5d440, g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d5d578, g_unk_7101d5d440);
    fn_710009ec00(g_unk_7101d5d579, fn_71000ac640());
    {
        CStrId n("base::global::CRntVector<unsigned short>", true);
        fn_71000ac71c(g_unk_7101d5d580, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, g_unk_7101d5d580, g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d5d6f0, g_unk_7101d5d580);
    fn_710009ec00(g_unk_7101d5d6f1, fn_71000ac90c());
    fn_710009ec00(g_unk_7101d5d6f2, fn_71000ac9e8());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<unsigned>>::TKeyElementIterator::value_type", true);
        fn_71000ace48(g_unk_7101d5d6f8, n, g_unk_7101d5e2e0, nullptr, fn_71000acac4, nullptr);
    }
    __cxa_atexit(fn_7100096604, g_unk_7101d5d6f8, g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d5d830, g_unk_7101d5d6f8);
    fn_710009ec00(g_unk_7101d5d831, fn_71000acfcc());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<unsigned>>::TKeyElementConstIterator::value_type", true);
        fn_71000ad42c(g_unk_7101d5d838, n, g_unk_7101d5e2e0, nullptr, fn_71000ad0a8, nullptr);
    }
    __cxa_atexit(fn_7100096604, g_unk_7101d5d838, g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d5d970, g_unk_7101d5d838);
    fn_710009ec00(g_unk_7101d5d971, fn_71000ad5b0());
    {
        CStrId n("base::global::CRntVector<unsigned>", true);
        fn_71000ad68c(g_unk_7101d5d978, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, g_unk_7101d5d978, g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d5dae8, g_unk_7101d5d978);
    fn_710009ec00(g_unk_7101d5dae9, fn_71000ad87c());
    fn_710009ec00(g_unk_7101d5daea, fn_71000ad958());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<uint64>>::TKeyElementIterator::value_type", true);
        fn_71000addb8(g_unk_7101d5daf0, n, g_unk_7101d5e2e0, nullptr, fn_71000ada34, nullptr);
    }
    __cxa_atexit(fn_7100096604, g_unk_7101d5daf0, g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d5dc28, g_unk_7101d5daf0);
    fn_710009ec00(g_unk_7101d5dc29, fn_71000adf3c());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<uint64>>::TKeyElementConstIterator::value_type", true);
        fn_71000ae39c(g_unk_7101d5dc30, n, g_unk_7101d5e2e0, nullptr, fn_71000ae018, nullptr);
    }
    __cxa_atexit(fn_7100096604, g_unk_7101d5dc30, g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d5dd68, g_unk_7101d5dc30);
    fn_710009ec00(g_unk_7101d5dd69, fn_71000ae520());
    {
        CStrId n("base::global::CRntVector<uint64>", true);
        fn_71000ae5fc(g_unk_7101d5dd70, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, g_unk_7101d5dd70, g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d5dee0, g_unk_7101d5dd70);
    fn_710009ec00(g_unk_7101d5dee1, fn_71000ae7ec());
    fn_710009ec00(g_unk_7101d5dee2, fn_71000ae8c8());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<float32>>::TKeyElementIterator::value_type", true);
        fn_71000aed28(g_unk_7101d5dee8, n, g_unk_7101d5e2e0, nullptr, fn_71000ae9a4, nullptr);
    }
    __cxa_atexit(fn_7100096604, g_unk_7101d5dee8, g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d5e020, g_unk_7101d5dee8);
    fn_710009ec00(g_unk_7101d5e021, fn_71000aeeac());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<float32>>::TKeyElementConstIterator::value_type", true);
        fn_71000af30c(g_unk_7101d5e028, n, g_unk_7101d5e2e0, nullptr, fn_71000aef88, nullptr);
    }
    __cxa_atexit(fn_7100096604, g_unk_7101d5e028, g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d5e160, g_unk_7101d5e028);
    fn_710009ec00(g_unk_7101d5e161, fn_71000af490());
    {
        CStrId n("base::global::CRntVector<float32>", true);
        fn_71000af56c(g_unk_7101d5e168, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, g_unk_7101d5e168, g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d5e2d8, g_unk_7101d5e168);
    {
        CStrId n("base::global::CCollectionKeyElementRefBase", true);
        fn_71000af760(g_unk_7101d5e2e0, n, nullptr, nullptr, fn_71000af75c, nullptr);
    }
    __cxa_atexit(fn_7100096604, g_unk_7101d5e2e0, g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d5e418, g_unk_7101d5e2e0);
    InitConversion(&g_unk_7101d5e420, fn_7100009658, fn_710000fd7c, fn_71000c18d8);
    fn_710009ec00(g_unk_7101d5e440, fn_710009d884());
    fn_710009ec00(g_unk_7101d5e441, fn_71000af8e0());
    fn_710009ec00(g_unk_7101d5e442, fn_71000af9bc());
    fn_710009ec00(g_unk_7101d5e443, fn_71000afa98());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<base::reflection::CTypedValue>>::TKeyElementIterator::value_type", true);
        fn_71000afef8(g_unk_7101d5e448, n, g_unk_7101d5e2e0, nullptr, fn_71000afb74, nullptr);
    }
    __cxa_atexit(fn_7100096604, g_unk_7101d5e448, g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d5e580, g_unk_7101d5e448);
    fn_710009ec00(g_unk_7101d5e581, fn_71000b007c());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<base::reflection::CTypedValue>>::TKeyElementConstIterator::value_type", true);
        fn_71000b04dc(g_unk_7101d5e588, n, g_unk_7101d5e2e0, nullptr, fn_71000b0158, nullptr);
    }
    __cxa_atexit(fn_7100096604, g_unk_7101d5e588, g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d5e6c0, g_unk_7101d5e588);
    fn_710009ec00(g_unk_7101d5e6c1, fn_71000b0660());
    {
        CStrId n("base::global::CRntVector<base::reflection::CTypedValue>", true);
        fn_71000b073c(g_unk_7101d5e6c8, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, g_unk_7101d5e6c8, g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d5e838, g_unk_7101d5e6c8);
    fn_710009ec00(g_unk_7101d5e839, fn_71000b092c());
    fn_710009ec00(g_unk_7101d5e83a, fn_71000b0a08());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntDictionary<base::global::CStrId, base::reflection::CTypedValue>>::TKeyElementIterator::value_type", true);
        fn_71000b0e68(g_unk_7101d5e840, n, g_unk_7101d5e2e0, nullptr, fn_71000b0ae4, nullptr);
    }
    __cxa_atexit(fn_7100096604, g_unk_7101d5e840, g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d5e978, g_unk_7101d5e840);
    fn_710009ec00(g_unk_7101d5e979, fn_71000b0fec());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntDictionary<base::global::CStrId, base::reflection::CTypedValue>>::TKeyElementConstIterator::value_type", true);
        fn_71000b144c(g_unk_7101d5e980, n, g_unk_7101d5e2e0, nullptr, fn_71000b10c8, nullptr);
    }
    __cxa_atexit(fn_7100096604, g_unk_7101d5e980, g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d5eab8, g_unk_7101d5e980);
    fn_710009ec00(g_unk_7101d5eab9, fn_71000b15d0());
    {
        CStrId n("base::global::CRntDictionary<base::global::CStrId, base::reflection::CTypedValue>", true);
        fn_71000b16ac(g_unk_7101d5eac0, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, g_unk_7101d5eac0, g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d5ec30, g_unk_7101d5eac0);
    fn_710009ec00(g_unk_7101d5ec31, fn_71000b187c());
    fn_710009ec00(g_unk_7101d5ec32, fn_71000b1958());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntSmallDictionary<base::global::CStrId, base::reflection::CTypedValue>>::TKeyElementIterator::value_type", true);
        fn_71000b1db8(g_unk_7101d5ec38, n, g_unk_7101d5e2e0, nullptr, fn_71000b1a34, nullptr);
    }
    __cxa_atexit(fn_7100096604, g_unk_7101d5ec38, g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d5ed70, g_unk_7101d5ec38);
    fn_710009ec00(g_unk_7101d5ed71, fn_71000b1f3c());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntSmallDictionary<base::global::CStrId, base::reflection::CTypedValue>>::TKeyElementConstIterator::value_type", true);
        fn_71000b239c(g_unk_7101d5ed78, n, g_unk_7101d5e2e0, nullptr, fn_71000b2018, nullptr);
    }
    __cxa_atexit(fn_7100096604, g_unk_7101d5ed78, g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d5eeb0, g_unk_7101d5ed78);
    fn_710009ec00(g_unk_7101d5eeb1, fn_71000b2520());
    {
        CStrId n("base::global::CRntSmallDictionary<base::global::CStrId, base::reflection::CTypedValue>", true);
        fn_71000b25fc(g_unk_7101d5eeb8, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, g_unk_7101d5eeb8, g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d5f028, g_unk_7101d5eeb8);
    fn_710009ec00(g_unk_7101d5f029, fn_71000b27f0());
    fn_710009ec00(g_unk_7101d5f02a, fn_71000b28cc());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntSmallDictionary<base::global::CStrId, base::global::CStrId>>::TKeyElementIterator::value_type", true);
        fn_71000b2d2c(g_unk_7101d5f030, n, g_unk_7101d5e2e0, nullptr, fn_71000b29a8, nullptr);
    }
    __cxa_atexit(fn_7100096604, g_unk_7101d5f030, g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d5f168, g_unk_7101d5f030);
    fn_710009ec00(g_unk_7101d5f169, fn_71000b2eb0());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntSmallDictionary<base::global::CStrId, base::global::CStrId>>::TKeyElementConstIterator::value_type", true);
        fn_71000b3310(g_unk_7101d5f170, n, g_unk_7101d5e2e0, nullptr, fn_71000b2f8c, nullptr);
    }
    __cxa_atexit(fn_7100096604, g_unk_7101d5f170, g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d5f2a8, g_unk_7101d5f170);
    fn_710009ec00(g_unk_7101d5f2a9, fn_71000b3494());
    {
        CStrId n("base::global::CRntSmallDictionary<base::global::CStrId, base::global::CStrId>", true);
        fn_71000b3570(g_unk_7101d5f2b0, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, g_unk_7101d5f2b0, g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d5f420, g_unk_7101d5f2b0);
    fn_710009ec00(g_unk_7101d5f421, fn_71000b3764());
    fn_710009ec00(g_unk_7101d5f422, fn_71000b3840());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntSmallDictionary<base::global::CStrId, base::global::CFilePathStrId>>::TKeyElementIterator::value_type", true);
        fn_71000b3ca0(g_unk_7101d5f428, n, g_unk_7101d5e2e0, nullptr, fn_71000b391c, nullptr);
    }
    __cxa_atexit(fn_7100096604, g_unk_7101d5f428, g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d5f560, g_unk_7101d5f428);
    fn_710009ec00(g_unk_7101d5f561, fn_71000b3e24());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntSmallDictionary<base::global::CStrId, base::global::CFilePathStrId>>::TKeyElementConstIterator::value_type", true);
        fn_71000b4284(g_unk_7101d5f568, n, g_unk_7101d5e2e0, nullptr, fn_71000b3f00, nullptr);
    }
    __cxa_atexit(fn_7100096604, g_unk_7101d5f568, g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d5f6a0, g_unk_7101d5f568);
    fn_710009ec00(g_unk_7101d5f6a1, fn_71000b4408());
    {
        CStrId n("base::global::CRntSmallDictionary<base::global::CStrId, base::global::CFilePathStrId>", true);
        fn_71000b44e4(g_unk_7101d5f6a8, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, g_unk_7101d5f6a8, g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d5f818, g_unk_7101d5f6a8);
    fn_710009ec00(g_unk_7101d5f819, fn_71000b46d8());
    fn_710009ec00(g_unk_7101d5f81a, fn_71000b47b4());
    fn_710009ec00(g_unk_7101d5f81b, fn_71000b4890());
    fn_710009ec00(g_unk_7101d5f81c, fn_71000b496c());
    fn_710009ec00(g_unk_7101d5f81d, fn_71000b4a48());
    fn_710009ec00(g_unk_7101d5f81e, fn_71000b4b24());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntSmallDictionary<base::global::CName, base::reflection::CVariable>>::TKeyElementIterator::value_type", true);
        fn_71000b4f84(g_unk_7101d5f820, n, g_unk_7101d5e2e0, nullptr, fn_71000b4c00, nullptr);
    }
    __cxa_atexit(fn_7100096604, g_unk_7101d5f820, g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d5f958, g_unk_7101d5f820);
    fn_710009ec00(g_unk_7101d5f959, fn_71000b5108());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntSmallDictionary<base::global::CName, base::reflection::CVariable>>::TKeyElementConstIterator::value_type", true);
        fn_71000b5568(g_unk_7101d5f960, n, g_unk_7101d5e2e0, nullptr, fn_71000b51e4, nullptr);
    }
    __cxa_atexit(fn_7100096604, g_unk_7101d5f960, g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d5fa98, g_unk_7101d5f960);
    fn_710009ec00(g_unk_7101d5fa99, fn_71000b56ec());
    {
        CStrId n("base::global::CRntSmallDictionary<base::global::CName, base::reflection::CVariable>", true);
        fn_71000b57c8(g_unk_7101d5faa0, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, g_unk_7101d5faa0, g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d5fc10, g_unk_7101d5faa0);
    fn_710009ec00(g_unk_7101d5fc11, fn_71000b59c0());
    fn_710009ec00(g_unk_7101d5fc12, fn_71000b5a9c());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntSmallDictionary<base::global::CStrId, base::reflection::CVariable>>::TKeyElementIterator::value_type", true);
        fn_71000b5efc(g_unk_7101d5fc18, n, g_unk_7101d5e2e0, nullptr, fn_71000b5b78, nullptr);
    }
    __cxa_atexit(fn_7100096604, g_unk_7101d5fc18, g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d5fd50, g_unk_7101d5fc18);
    fn_710009ec00(g_unk_7101d5fd51, fn_71000b6080());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntSmallDictionary<base::global::CStrId, base::reflection::CVariable>>::TKeyElementConstIterator::value_type", true);
        fn_71000b64e0(g_unk_7101d5fd58, n, g_unk_7101d5e2e0, nullptr, fn_71000b615c, nullptr);
    }
    __cxa_atexit(fn_7100096604, g_unk_7101d5fd58, g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d5fe90, g_unk_7101d5fd58);
    fn_710009ec00(g_unk_7101d5fe91, fn_71000b6664());
    {
        CStrId n("base::global::CRntSmallDictionary<base::global::CStrId, base::reflection::CVariable>", true);
        fn_71000b6740(g_unk_7101d5fe98, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, g_unk_7101d5fe98, g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d60008, g_unk_7101d5fe98);
    fn_710009ec00(g_unk_7101d60009, fn_71000b6938());
    fn_710009ec00(g_unk_7101d6000a, fn_71000b6a14());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntSmallDictionary<base::global::CStrId, base::reflection::CFunction>>::TKeyElementIterator::value_type", true);
        fn_71000b6e74(g_unk_7101d60010, n, g_unk_7101d5e2e0, nullptr, fn_71000b6af0, nullptr);
    }
    __cxa_atexit(fn_7100096604, g_unk_7101d60010, g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d60148, g_unk_7101d60010);
    fn_710009ec00(g_unk_7101d60149, fn_71000b6ff8());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntSmallDictionary<base::global::CStrId, base::reflection::CFunction>>::TKeyElementConstIterator::value_type", true);
        fn_71000b7458(g_unk_7101d60150, n, g_unk_7101d5e2e0, nullptr, fn_71000b70d4, nullptr);
    }
    __cxa_atexit(fn_7100096604, g_unk_7101d60150, g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d60288, g_unk_7101d60150);
    fn_710009ec00(g_unk_7101d60289, fn_71000b75dc());
    {
        CStrId n("base::global::CRntSmallDictionary<base::global::CStrId, base::reflection::CFunction>", true);
        fn_71000b76b8(g_unk_7101d60290, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, g_unk_7101d60290, g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d60400, g_unk_7101d60290);
    fn_710009ec00(g_unk_7101d60401, fn_71000b78ac());
    fn_710009ec00(g_unk_7101d60402, fn_71000b7988());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntSmallDictionary<base::global::CStrId, float>>::TKeyElementIterator::value_type", true);
        fn_71000b7de8(g_unk_7101d60408, n, g_unk_7101d5e2e0, nullptr, fn_71000b7a64, nullptr);
    }
    __cxa_atexit(fn_7100096604, g_unk_7101d60408, g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d60540, g_unk_7101d60408);
    fn_710009ec00(g_unk_7101d60541, fn_71000b7f6c());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntSmallDictionary<base::global::CStrId, float>>::TKeyElementConstIterator::value_type", true);
        fn_71000b83cc(g_unk_7101d60548, n, g_unk_7101d5e2e0, nullptr, fn_71000b8048, nullptr);
    }
    __cxa_atexit(fn_7100096604, g_unk_7101d60548, g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d60680, g_unk_7101d60548);
    fn_710009ec00(g_unk_7101d60681, fn_71000b8550());
    {
        CStrId n("base::global::CRntSmallDictionary<base::global::CStrId, float>", true);
        fn_71000b862c(g_unk_7101d60688, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, g_unk_7101d60688, g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d607f8, g_unk_7101d60688);
    fn_710009ec00(g_unk_7101d607f9, fn_71000b8820());
    fn_710009ec00(g_unk_7101d607fa, fn_71000b88fc());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<base::global::CRntString>>::TKeyElementIterator::value_type", true);
        fn_71000b8d5c(g_unk_7101d60800, n, g_unk_7101d5e2e0, nullptr, fn_71000b89d8, nullptr);
    }
    __cxa_atexit(fn_7100096604, g_unk_7101d60800, g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d60938, g_unk_7101d60800);
    fn_710009ec00(g_unk_7101d60939, fn_71000b8ee0());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<base::global::CRntString>>::TKeyElementConstIterator::value_type", true);
        fn_71000b9340(g_unk_7101d60940, n, g_unk_7101d5e2e0, nullptr, fn_71000b8fbc, nullptr);
    }
    __cxa_atexit(fn_7100096604, g_unk_7101d60940, g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d60a78, g_unk_7101d60940);
    fn_710009ec00(g_unk_7101d60a79, fn_71000b94c4());
    {
        CStrId n("base::global::CRntVector<base::global::CRntString>", true);
        fn_71000b95a0(g_unk_7101d60a80, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, g_unk_7101d60a80, g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d60bf0, g_unk_7101d60a80);
    fn_710009ec00(g_unk_7101d60bf1, fn_71000b9790());
    fn_710009ec00(g_unk_7101d60bf2, fn_71000b986c());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<bool>>::TKeyElementIterator::value_type", true);
        fn_71000b9ccc(g_unk_7101d60bf8, n, g_unk_7101d5e2e0, nullptr, fn_71000b9948, nullptr);
    }
    __cxa_atexit(fn_7100096604, g_unk_7101d60bf8, g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d60d30, g_unk_7101d60bf8);
    fn_710009ec00(g_unk_7101d60d31, fn_71000b9e50());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<bool>>::TKeyElementConstIterator::value_type", true);
        fn_71000ba2b0(g_unk_7101d60d38, n, g_unk_7101d5e2e0, nullptr, fn_71000b9f2c, nullptr);
    }
    __cxa_atexit(fn_7100096604, g_unk_7101d60d38, g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d60e70, g_unk_7101d60d38);
    fn_710009ec00(g_unk_7101d60e71, fn_71000ba434());
    {
        CStrId n("base::global::CRntVector<bool>", true);
        fn_71000ba510(g_unk_7101d60e78, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, g_unk_7101d60e78, g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d60fe8, g_unk_7101d60e78);
    {
        CStrId n("CompleteInfo", true);
        fn_71000ba704(g_unk_7101d60ff0, n, nullptr, nullptr, fn_71000ba700, nullptr);
    }
    __cxa_atexit(fn_7100096604, g_unk_7101d60ff0, g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d61128, g_unk_7101d60ff0);
    {
        CStrId n("CAfterElementDeserializationWrapper", true);
        fn_71000ba884(g_unk_7101d61130, n, nullptr, nullptr, fn_71000ba880, nullptr);
    }
    __cxa_atexit(fn_7100096604, g_unk_7101d61130, g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d61268, g_unk_7101d61130);
    InitConversion(&g_unk_7101d61270, fn_71000081e8, reinterpret_cast<UnkEnumType*>(g_unk_7101d61130), fn_71000e23ac);
    InitConversion(&g_unk_7101d61290, fn_7100007f84, reinterpret_cast<UnkEnumType*>(g_unk_7101d61130), fn_71000e24b0);
    InitConversion(&g_unk_7101d612b0, fn_7100009658, reinterpret_cast<UnkEnumType*>(g_unk_7101d61130), fn_71000e26f8);
}
