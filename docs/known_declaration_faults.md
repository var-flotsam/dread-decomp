# Known faults in declarations

Declarations that are known or suspected to be wrong, and the matches that work around them. The agent
runs of 2026-11-09 to 2026-11-11 collected this list as they hit each one (their `header_fixes.txt`, which
existed only in one worktree's `build/` folder); the batch ids say which batch reported it.

It matters twice. A wrong declaration blocks other matches or forces a workaround, and on x86-64 it is a
crash, not a comment: a result declared too small or an argument in the wrong class corrupts the caller in
the port (journal, 2026-11-08 and 2026-11-09). The entries are the workers' reports and have not all been
checked against the original. Remove an entry when its declaration is corrected, in the same change.

## Wrong return type

| Symbol | Declared | The original | Reported by |
|---|---|---|---|
| `fn_71011a3574`, `fn_71011d9e90`, `fn_71010e655c`, `fn_7100362324`, `fn_710000f750` | `void` | returns a value | b0005, b0001, b0026, b0032 |
| `fn_710007b518` | `void` (in a header) | returns a pointer the caller uses | b0099 |
| `fn_71011a32d8`, `fn_71011a3574` | `void` | return values that are used; reached as `bool` through `asm`-label aliases in `UnkTU2577_SubAreaMgr_ovb0189.cpp` | b0189 |
| `fn_7100ef0cdc`, `fn_7100ef11f4` | `void()` in `function_inits_02.cpp` | not stated | b0189 |

## Wrong parameters

| Symbol | Declared | The original | Reported by |
|---|---|---|---|
| `fn_7100004578` | a count parameter that is not `u64` (`include/base/UnkResType.h`) | matches when it is `u64` | b0033 |
| `fn_71007616e8` | as `src/clones/clones_02.cpp` declares it | defined with another signature (matters for the port's link) | b0028 |
| `fn_71011a1d00` | no `self` parameter (`src/game/CScenario.cpp`) | takes `self` | b0032 |
| `fn_710076438c` | lacks a `bool` argument | takes it | b0001, b0008 |
| `fn_710014580c` | one argument (`src/game/CFXComponent/fn_7101152b90.cpp`) | two; b0052 casts at the call | b0052 |
| `fn_71010d4234` | `char*(char*)` in 4 files | a caller passes a float | b0048 |
| `fn_7101105e14` | with an `f32` (`CQuetzoaAIComponent_Dodge_A3_b76.cpp`) | the call passes none; blocks `fn_710113430c` | b0050 |
| `fn_7100caacd0` | `void()` (`CBTObserverComponent.cpp`) | `(self, const u32*)`; matched in scratch, blocked | b0055 |
| `fn_7100ca7a64` | `(void*, u16)` (`game_s2/fn_71002b3828.cpp`) | takes `u32`; matched in scratch, blocked | b0055 |
| `fn_7100f0f4d0` | `void()` (`src/game/CPlayerInfo.cpp`) | `(self, message)` | b0071 |
| `fn_71007b70d8` | `void(void*)` (`CDoorEmmyFXComponent.cpp`) | two parameters | b0074 |
| `fn_71011b7ec0` | 2 parameters (`include/game/UnkGuiFrames.h`) | a caller passes 3 | b0080 |
| `fn_71009ea3e4` | with an `f32` | the original leaves it unset | b0103 |
| `fn_7100df31b8` | third argument `bool` | the caller passes 2; its matching draft is held back in `drafts/` | b0103 |
| `fn_7100219558` | third argument `bool` | `u32`; called through a function-pointer cast in `UnkTU2666_lua_ovb0157`/`ovb0159` | b0157 |
| `fn_7100eb33c8`, `fn_7100eb2eac` | one argument | need a second | later batch |
| `fn_7100eaf72c` | no parameters | takes some | later batch |
| `fn_7100331c7c` | 2 arguments | 1; `UnkTU437_Aim_ovb0198.cpp` declares it through an `asm` label | b0198 |
| `fn_7100b43b30` | not stated | `UnkTU1689_MapMenu_ovb0203.cpp` declares it through an alias | b0203 |
| `fn_71001cde84` | takes `bool` | `u32` first argument; `UnkRenderCalls_b0253` reaches it as a static member | b0253 |
| `fn_71000e88c0` | a `CClass*` getter in the generated `include/base/reflection/UnkClassGetters.h` | a `void` descriptor initialiser: the generator names it wrongly | b0136 |

## Matches to look at again

| Where | What |
|---|---|
| `fn_71011990e8` (`UnkTU2730_Lists_ovb0188.cpp`) | passes an uninitialised first argument to `fn_7101198e88` to match; the intended argument is unknown |
| `fn_7100b532a0` (`UnkTU1703_Title_ovb0215.cpp`) | the switch's state-to-callee order is a guess: jump tables are not compared by `check.py` |
| `fn_71007121e0` | defined `__attribute__((weak))` so its caller in the same file keeps the original's reload |
| `fn_710002ec40`, `fn_71005fd088`, `fn_71007b5c48` and others with function-local guarded statics | the static is a compiler-named symbol, not a `g_unk_` one, so its address is not checked (as in the older `fn_7100adcf10.cpp`) |
| `fn_71007a6b58` (`NON_MATCHING` minor) | the element-move loop may match written as `Entry* old = src++; old->~Entry();` |
| `UnkTU811_Emmy_ovb0193.cpp`, `CPerceptionComponent_Ctor_ovb0266.cpp`, the weak destructor pair of `fn_710077afa4`/`fn_710077b1e8` | `trymatch.py` reports a function "not mapped to an address" |

Resolved since the runs: the two string literals of `fn_7100775358` (`UnkTU1176_Central_ovb0230.cpp`) that
were taken from a truncated listing are the original's (`check.py` compares string contents since #355);
`fn_7100ac82c0`'s `void` declaration was corrected to `s32` in the lean run.

## Structs defined in several files

The same struct written out in more than one `.cpp`, each a separate definition in the port's one program:
`UnkResolvedValue` (b0106), `UnkRef_c3` (b0065), `UnkFxObject_ovb0041` (b0041, b0046), `UnkShotWeakRef` (b0006),
`UnkSamusSettings` (b0004). These are the ones the workers noticed; `tools/decl_check.py` does not look at
struct definitions, and 1,020 struct names are defined in more than one file.

## Shared functions the mechanical header migration could not move

`tools/decl_plan.py` and `tools/hub_migrate.py` moved 1,353 shared functions' declarations into unit headers on
2026-11-11. These 75 were tried and left with every local copy in place, because moving them changed a
file or stopped it compiling. Each needs a person to settle the type. (`tools/decl_plan.py --exclude` takes
this list; remove a symbol here when it has moved.)

**48: the header's forward declaration clashes with a type of another kind that a file defines under the same name (a typedef, a template alias, a type in a namespace).**
`fn_710007b444` `fn_710007cee4` `fn_710007f564` `fn_7100083804` `fn_710009fb38` `fn_710009ffd4` `fn_71000fbce0` `fn_71001039f8` `fn_71002295ac` `fn_7100229688` `fn_7100231b40` `fn_7100232864` `fn_710026d2e4` `fn_710027fc88` `fn_7100287598` `fn_71002af070` `fn_71002af6fc` `fn_71002b3114` `fn_71002b3328` `fn_71002b52a4` `fn_71002f239c` `fn_7100353be8` `fn_7100407170` `fn_7100d45fc4` `fn_7100d45fd4` `fn_7100d46804` `fn_7100d46c60` `fn_7100d476e4` `fn_7100d59dd8` `fn_7100ee34f0` `fn_7100ee62bc` `fn_7100ee6884` `fn_7100ef1e1c` `fn_7100f84868` `fn_7100f85f00` `fn_7100f86020` `fn_7100f86254` `fn_7100faa3d8` `fn_7100fac120` `fn_7100ffbad8` `fn_7101005d8c` `fn_710119a110` `fn_710120806c` `fn_710120b198` `fn_710120e5d0` `fn_7101233f2c` `fn_7101234588` `fn_7101234dcc`

**13: a file defines it, or declares it in a way the declaration check does not see, with another return type than the shared declaration (the compiler: "functions that differ only in their return type").**
`fn_7100017ce8` `fn_710002384c` `fn_7100026f70` `fn_7100108a84` `fn_7100108abc` `fn_7100108ae8` `fn_7100969058` `fn_710096cdec` `fn_710096d788` `fn_7100d8d854` `fn_7100d8db30` `fn_7100d8e398` `fn_7100f011c4`

**8: the file's compiled output changed.**
`fn_71005c1008` `fn_7100895498` `fn_710098ae04` `fn_71009a60fc` `fn_71009a64cc` `fn_71009cde54` `fn_710111b230` `fn_71011201e4`

**6: a file uses a type the header does not bring.**
`fn_71007142e8` `fn_7100872908` `fn_710099d6f0` `fn_71009a1f40` `fn_71009b0800` `fn_71010d1598`

## Shared globals the mechanical header migration could not move

`tools/decl_plan.py` and `tools/hub_migrate.py` moved 674 shared symbols' declarations (672 globals and two
functions) into unit headers on 2026-11-11, each global into the header of the unit whose data span holds it
(`tools/data_map.py`). These 37 globals were tried and left with every local copy in place. (They are in the
`--exclude` list with the functions above; remove a symbol here when it has moved.)

**19: the header's forward declaration of the global's type clashes with a type of the same name in a namespace that a file opens (the compiler: "reference is ambiguous").**
`g_unk_7101a45f90` `g_unk_7101a45fe8` `g_unk_7108b6d8f0` `g_unk_71726dfca0` `g_unk_71727115d0` `g_unk_7172731200` `g_unk_71727313a4` `g_unk_71729f0188` `g_unk_71729f0190` `g_unk_71729f0198` `g_unk_71729f01a0` `g_unk_71729f01a8` `g_unk_71729f01c8` `g_unk_71729f0228` `g_unk_71729f0238` `g_unk_71729f0240` `g_unk_71729f0248` `g_unk_71729f0298` `g_unk_71729f02a8`

**15: the global's unit header also declares a function that one of the files declares with another return type (the functions of the list above), so that file cannot include the header.**
`g_unk_7101d47100` `g_unk_7101d47320` `g_unk_7101d47328` `g_unk_7101d47330` `g_unk_7101d47338` `g_unk_7101d47340` `g_unk_7101d47358` `g_unk_7101d47368` `g_unk_7101d47370` `g_unk_7101d47378` `g_unk_7101d47380` `g_unk_7101d4b418` `g_unk_7101d4bbc0` `g_unk_7101d4c138` `g_unk_7101d4c8d0`

**3: a file's `using` declaration or typedef clashes with the header's forward declaration.**
`g_unk_7172730858` `g_unk_7172735f58` `g_unk_7172795e08`

Not tried in this pass: 328 globals whose type comes from a shared header (`UnkReflType`, `UnkConverter`,
`CStrId`, `UnkProperty`, `UnkTUHeader`), 95 that lie between two units' data spans, 92 with conflicting
signatures, and 2 whose copies differ in a visibility attribute.

## Definitions that disagree with their declaration

`python tools/decl_check.py --definitions` lists every hand-written definition whose result or parameters
are not what a declaration of the same function in another file says. On AArch64 such a pair still matches,
because the registers hold the right values anyway; in the PC port the call is wrong.
`tools/decl_check.py --base` fails when a pull request adds one. To settle one, read what the original does
with the result and argument registers on both sides of the call, give the header the true signature, and
make the defining file include that header.

There were 312 on 2026-11-11, 168 of them with `void` on one side and a value on the other. 100 of those
are settled: in 96 the definition returns a value (it matches, and the original sets the result register
or a caller reads it) and the `void` declarations took its result type; in 4 the definition is `void`, no
caller in the original reads a result, and the declarations became `void`. Every touched file builds to
the same output. Six more were settled by reading the original (#372): `fn_7100362324` returns the entry
`fn_71003646b0` returns, and `fn_7100017ba0`, `fn_710063c670`, `fn_710081b510` and `fn_7100d8e0a0` return nothing,
so the functions that end by calling them return nothing either. 211 are left: 95 with a parameter of another
class, 62 `void` against a value, 35 with another number of parameters, 19 with a result of another class. The 62:

**46: a file stores the function's address in a pointer, or hands it to a template, typed for the `void` form; the pointer's type has to change with the declaration.**
`fn_71000272cc` `fn_7100027418` `fn_71000cc800` `fn_71000cc890` `fn_71000ccca8` `fn_71000ccd84` `fn_71000cce60` `fn_71000ccf08` `fn_71000cd2a4` `fn_71000cd338` `fn_71002332e0` `fn_7100248b38` `fn_7100339d48` `fn_71004c31f8` `fn_71004cb0e8` `fn_71006895a0` `fn_71007124d8` `fn_7100712538` `fn_71007ab7d8` `fn_71007f5d24` `fn_7100a52ebc` `fn_7100ad37c0` `fn_7100adc4c0` `fn_7100aec000` `fn_7100caad50` `fn_7100caadb0` `fn_7100d86e94` `fn_7100da5184` `fn_7100eac64c` `fn_7100eac82c` `fn_7100eb31f0` `fn_7100eb3278` `fn_7100eb331c` `fn_7100f796b8` `fn_7100f87480` `fn_7100f89fd8` `fn_7100f94d0c` `fn_7100fedf08` `fn_7100feefa0` `fn_7101048588` `fn_71010c6cdc` `fn_7101130850` `fn_710117a018` `fn_710117b3f8` `fn_71011e7d60` `fn_71011e7e8c`

**10: the definition returns a struct by value (`UnkReflValue` and others) that a declaring file does not define.**
`python tools/decl_check.py --definitions` shows them.

**6: the definition is `void`, a declaration returns a value, and a hand-written caller uses it.** `fn_7100202e7c` forwards `fn_71001ef494`'s result to a caller that returns it as a `bool`, but written that way it gains an `and w0, w0, #1` the original does not have; `fn_71000e88c0` is declared as a class getter by the generated `UnkClassGetters.h` and is an initialiser that returns nothing.
`fn_71000e88c0` `fn_7100202e7c` `fn_71005026d0` `fn_71007038f4` `fn_7100e4fd1c` `fn_7100e5493c`

## Names that point to more than one address

`python tools/name_check.py --version 2.1.0` compares, for every name `tools/check.py` cannot resolve to an
address (a named constructor, a vtable by class name, a local static), where the original points at each
reference to it (a 4-byte function that only branches on counts as its target, so a call through an import's
thunk and a call to its stub agree). On 2026-11-11, 109,340 references under 11,763 names agree, and 5 names do
not (22 references; a sixth, a string destructor's name on a call the original makes to `fn_710026ead0` itself, was
corrected in #369). Each is a reference to the wrong thing, or one name used for different things:

| Name | Usually | Also | Reading |
|---|---|---|---|
| `base::refl_IterSync<UnkOwnedList, unsigned int>` | five addresses, two uses each | | Five instantiations over five file-local types all called `UnkOwnedList` (`CAcidBlobsLaunchPattern/fn_71005da4f8.cpp`, `CCooldownXBossFireBallDef/fn_71004bf9a8.cpp`, `fn_71004bfaa8.cpp`): the port's link sees one symbol. |
| vtables of `unk_any_props::UnkPropAny`, `UnkPropList`, `UnkEntry` | five addresses each | | Five sets of classes under one name, one per file (`fn_7100442780.cpp`, `fn_710048b740.cpp`, `fn_71004c8f58.cpp`, `fn_7100510548.cpp` and the first user). |
| `std::__1::__throw_bad_function_call` | `0x7100a2e300` (4 uses) | `0x7100008bb0` (2, `CBlendSpace1D/UnkSharedFunctionDtors.cpp`) | The original has two copies; the two uses name the other one. |

`--strict` makes the tool fail on any of them; it is not a gate yet.
