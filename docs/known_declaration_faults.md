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
| `fn_7100fbc4e0` | `void` | returns a value; called through a function-pointer cast in `UnkTU2666_lua_ovb0157`/`ovb0159` | b0157 |
| `fn_71011a32d8`, `fn_71011a3574` | `void` | return values that are used; reached as `bool` through `asm`-label aliases in `UnkTU2577_SubAreaMgr_ovb0189.cpp` | b0189 |
| `fn_7100ef0cdc`, `fn_7100ef11f4` | `void()` in `function_inits_02.cpp` | not stated | b0189 |
| `fn_7100e791a4` | `void` | returns `u32` (`fn_7100e95130` matches with `u32`; its draft is in `drafts/`) | later batch |

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
