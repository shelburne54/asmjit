// This file is part of AsmJit project <https://asmjit.com>
//
// See asmjit.h or LICENSE.md for license and copyright information
// SPDX-License-Identifier: Zlib

#ifndef ASMJIT_ARM_A32GLOBALS_H_INCLUDED
#define ASMJIT_ARM_A32GLOBALS_H_INCLUDED

#include "../arm/armglobals.h"

//! \namespace asmjit::a32
//! \ingroup asmjit_a32
//!
//! AArch64 backend.

ASMJIT_BEGIN_SUB_NAMESPACE(a32)

// a32 uses everything from arm namespace and adds into it.
using namespace arm;

//! \addtogroup asmjit_a32
//! \{

//! ARM32/THUMB instruction.
//!
//! \note Only used to hold ARM-specific enumerations and static functions.
struct Inst {
  //! Instruction id.
  enum Id : uint32_t {
    // ${a32::InstId:Begin}
    // ------------------- Automatically generated, do not edit -------------------
    kIdNone = 0,
    kIdAdc,
    kIdAdcs,
    kIdAdd,
    kIdAdds,
    kIdAdr,
    kIdAesd,
    kIdAese,
    kIdAesimc,
    kIdAesmc,
    kIdAnd,
    kIdAnds,
    kIdAsr,
    kIdAsrs,
    kIdB,
    kIdBfc,
    kIdBfi,
    kIdBic,
    kIdBics,
    kIdBkpt,
    kIdBl,
    kIdBlx,
    kIdBx,
    kIdBxj,
    kIdClrex,
    kIdClz,
    kIdCmn,
    kIdCmp,
    kIdCps,
    kIdCpsid,
    kIdCpsie,
    kIdCrc32b,
    kIdCrc32cb,
    kIdCrc32ch,
    kIdCrc32cw,
    kIdCrc32h,
    kIdCrc32w,
    kIdDbg,
    kIdDmb,
    kIdDsb,
    kIdEor,
    kIdEors,
    kIdEret,
    kIdHlt,
    kIdHvc,
    kIdIsb,
    kIdLda,
    kIdLdab,
    kIdLdaex,
    kIdLdaexb,
    kIdLdaexd,
    kIdLdaexh,
    kIdLdah,
    kIdLdm,
    kIdLdmda,
    kIdLdmdb,
    kIdLdmib,
    kIdLdr,
    kIdLdrb,
    kIdLdrbt,
    kIdLdrd,
    kIdLdrex,
    kIdLdrexb,
    kIdLdrexd,
    kIdLdrexh,
    kIdLdrh,
    kIdLdrht,
    kIdLdrsb,
    kIdLdrsbt,
    kIdLdrsh,
    kIdLdrsht,
    kIdLdrt,
    kIdLsl,
    kIdLsls,
    kIdLsr,
    kIdLsrs,
    kIdMcr,
    kIdMcr2,
    kIdMcrr,
    kIdMcrr2,
    kIdMla,
    kIdMlas,
    kIdMls,
    kIdMov,
    kIdMovs,
    kIdMovt,
    kIdMovw,
    kIdMrc,
    kIdMrc2,
    kIdMrrc,
    kIdMrrc2,
    kIdMrs,
    kIdMsr,
    kIdMul,
    kIdMuls,
    kIdMvn,
    kIdMvns,
    kIdNop,
    kIdOrr,
    kIdOrrs,
    kIdPkhbt,
    kIdPkhtb,
    kIdPld,
    kIdPldw,
    kIdPli,
    kIdPop,
    kIdPush,
    kIdQadd,
    kIdQadd16,
    kIdQadd8,
    kIdQasx,
    kIdQdadd,
    kIdQdsub,
    kIdQsax,
    kIdQsub,
    kIdQsub16,
    kIdQsub8,
    kIdRbit,
    kIdRev,
    kIdRev16,
    kIdRevsh,
    kIdRfe,
    kIdRfeda,
    kIdRfedb,
    kIdRfeib,
    kIdRor,
    kIdRors,
    kIdRrx,
    kIdRrxs,
    kIdRsb,
    kIdRsbs,
    kIdRsc,
    kIdRscs,
    kIdSadd16,
    kIdSadd8,
    kIdSasx,
    kIdSbc,
    kIdSbcs,
    kIdSbfx,
    kIdSdiv,
    kIdSel,
    kIdSetend,
    kIdSev,
    kIdSevl,
    kIdSha1c,
    kIdSha1h,
    kIdSha1m,
    kIdSha1p,
    kIdSha1su0,
    kIdSha1su1,
    kIdSha256h,
    kIdSha256h2,
    kIdSha256su0,
    kIdSha256su1,
    kIdShadd16,
    kIdShadd8,
    kIdShasx,
    kIdShsax,
    kIdShsub16,
    kIdShsub8,
    kIdSmc,
    kIdSmlabb,
    kIdSmlabt,
    kIdSmlad,
    kIdSmladx,
    kIdSmlal,
    kIdSmlalbb,
    kIdSmlalbt,
    kIdSmlald,
    kIdSmlaldx,
    kIdSmlals,
    kIdSmlaltb,
    kIdSmlaltt,
    kIdSmlatb,
    kIdSmlatt,
    kIdSmlawb,
    kIdSmlawt,
    kIdSmlsd,
    kIdSmlsdx,
    kIdSmlsld,
    kIdSmlsldx,
    kIdSmmla,
    kIdSmmlar,
    kIdSmmls,
    kIdSmmlsr,
    kIdSmmul,
    kIdSmmulr,
    kIdSmuad,
    kIdSmuadx,
    kIdSmulbb,
    kIdSmulbt,
    kIdSmull,
    kIdSmulls,
    kIdSmultb,
    kIdSmultt,
    kIdSmulwb,
    kIdSmulwt,
    kIdSmusd,
    kIdSmusdx,
    kIdSrs,
    kIdSrsda,
    kIdSrsdb,
    kIdSrsib,
    kIdSsat,
    kIdSsat16,
    kIdSsax,
    kIdSsub16,
    kIdSsub8,
    kIdStl,
    kIdStlb,
    kIdStlex,
    kIdStlexb,
    kIdStlexd,
    kIdStlexh,
    kIdStlh,
    kIdStm,
    kIdStmda,
    kIdStmdb,
    kIdStmib,
    kIdStr,
    kIdStrb,
    kIdStrbt,
    kIdStrd,
    kIdStrex,
    kIdStrexb,
    kIdStrexd,
    kIdStrexh,
    kIdStrh,
    kIdStrht,
    kIdStrt,
    kIdSub,
    kIdSubs,
    kIdSvc,
    kIdSxtab,
    kIdSxtab16,
    kIdSxtah,
    kIdSxtb,
    kIdSxtb16,
    kIdSxth,
    kIdTeq,
    kIdTst,
    kIdUadd16,
    kIdUadd8,
    kIdUasx,
    kIdUbfx,
    kIdUdf,
    kIdUdiv,
    kIdUhadd16,
    kIdUhadd8,
    kIdUhasx,
    kIdUhsax,
    kIdUhsub16,
    kIdUhsub8,
    kIdUmaal,
    kIdUmlal,
    kIdUmlals,
    kIdUmull,
    kIdUmulls,
    kIdUqadd16,
    kIdUqadd8,
    kIdUqasx,
    kIdUqsax,
    kIdUqsub16,
    kIdUqsub8,
    kIdUsad8,
    kIdUsada8,
    kIdUsat,
    kIdUsat16,
    kIdUsax,
    kIdUsub16,
    kIdUsub8,
    kIdUxtab,
    kIdUxtab16,
    kIdUxtah,
    kIdUxtb,
    kIdUxtb16,
    kIdUxth,
    kIdVaba,
    kIdVabal,
    kIdVabd,
    kIdVabdl,
    kIdVabs,
    kIdVacge,
    kIdVacgt,
    kIdVacle,
    kIdVaclt,
    kIdVadd,
    kIdVaddhn,
    kIdVaddl,
    kIdVaddw,
    kIdVand,
    kIdVbic,
    kIdVbif,
    kIdVbit,
    kIdVbsl,
    kIdVcadd,
    kIdVceq,
    kIdVcge,
    kIdVcgt,
    kIdVcle,
    kIdVcls,
    kIdVclt,
    kIdVclz,
    kIdVcmla,
    kIdVcmp,
    kIdVcmpe,
    kIdVcnt,
    kIdVcvt,
    kIdVcvta,
    kIdVcvtb,
    kIdVcvtm,
    kIdVcvtn,
    kIdVcvtp,
    kIdVcvtr,
    kIdVcvtt,
    kIdVdiv,
    kIdVdot,
    kIdVdup,
    kIdVeor,
    kIdVext,
    kIdVfma,
    kIdVfmab,
    kIdVfmal,
    kIdVfmat,
    kIdVfms,
    kIdVfmsl,
    kIdVfnma,
    kIdVfnms,
    kIdVhadd,
    kIdVhsub,
    kIdVins,
    kIdVjcvt,
    kIdVld1,
    kIdVld1r,
    kIdVld2,
    kIdVld2r,
    kIdVld3,
    kIdVld3r,
    kIdVld4,
    kIdVld4r,
    kIdVldr,
    kIdVmax,
    kIdVmaxnm,
    kIdVmin,
    kIdVminnm,
    kIdVmla,
    kIdVmlal,
    kIdVmls,
    kIdVmlsl,
    kIdVmmla,
    kIdVmov,
    kIdVmovl,
    kIdVmovn,
    kIdVmovx,
    kIdVmul,
    kIdVmull,
    kIdVmvn,
    kIdVneg,
    kIdVnmla,
    kIdVnmls,
    kIdVnmul,
    kIdVorn,
    kIdVorr,
    kIdVpadal,
    kIdVpadd,
    kIdVpaddl,
    kIdVpmax,
    kIdVpmin,
    kIdVqabs,
    kIdVqadd,
    kIdVqdmlal,
    kIdVqdmlsl,
    kIdVqdmulh,
    kIdVqdmull,
    kIdVqmovn,
    kIdVqmovun,
    kIdVqneg,
    kIdVqrdmlah,
    kIdVqrdmlsh,
    kIdVqrdmulh,
    kIdVqrshl,
    kIdVqrshrn,
    kIdVqrshrun,
    kIdVqshl,
    kIdVqshlu,
    kIdVqshrn,
    kIdVqshrun,
    kIdVqsub,
    kIdVraddhn,
    kIdVrecpe,
    kIdVrecps,
    kIdVrev16,
    kIdVrev32,
    kIdVrev64,
    kIdVrhadd,
    kIdVrinta,
    kIdVrintm,
    kIdVrintn,
    kIdVrintp,
    kIdVrintr,
    kIdVrintx,
    kIdVrintz,
    kIdVrshl,
    kIdVrshr,
    kIdVrshrn,
    kIdVrsqrte,
    kIdVrsqrts,
    kIdVrsra,
    kIdVrsubhn,
    kIdVsdot,
    kIdVseleq,
    kIdVselge,
    kIdVselgt,
    kIdVselvs,
    kIdVshl,
    kIdVshll,
    kIdVshr,
    kIdVshrn,
    kIdVsli,
    kIdVsmmla,
    kIdVsqrt,
    kIdVsra,
    kIdVsri,
    kIdVst1,
    kIdVst2,
    kIdVst3,
    kIdVst4,
    kIdVstr,
    kIdVsub,
    kIdVsubhn,
    kIdVsubl,
    kIdVsubw,
    kIdVsudot,
    kIdVswp,
    kIdVtbl,
    kIdVtbx,
    kIdVtrn,
    kIdVtst,
    kIdVudot,
    kIdVummla,
    kIdVusdot,
    kIdVusmmla,
    kIdVuzp,
    kIdVzip,
    kIdWfe,
    kIdWfi,
    kIdYield,
    _kIdCount
    // ----------------------------------------------------------------------------
    // ${a32::InstId:End}
  };

  //! Tests whether the `instId` is defined (counts also Inst::kIdNone, which must be zero).
  //!
  //! \note This function required identifier to be without modifiers. If the given instruction id contains modifiers
  //! it would return false as modifiers overflow `_kIdCount`.
  static constexpr inline bool isDefinedId(InstId instId) noexcept {
    return instId < _kIdCount;
  }
};

//! \}

ASMJIT_END_SUB_NAMESPACE

#endif // ASMJIT_ARM_A32GLOBALS_H_INCLUDED
