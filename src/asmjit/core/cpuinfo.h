// This file is part of AsmJit project <https://asmjit.com>
//
// See asmjit.h or LICENSE.md for license and copyright information
// SPDX-License-Identifier: Zlib

#ifndef ASMJIT_CORE_CPUINFO_H_INCLUDED
#define ASMJIT_CORE_CPUINFO_H_INCLUDED

#include "../core/archtraits.h"
#include "../core/environment.h"
#include "../core/globals.h"
#include "../core/string.h"
#include "../core/support.h"

ASMJIT_BEGIN_NAMESPACE

//! \addtogroup asmjit_core
//! \{

//! CPU features information.
//!
//! Each feature is represented by a single bit in an embedded bit array.
class CpuFeatures {
public:
  //! A word that is used to represents feature bits.
  typedef Support::BitWord BitWord;
  //! Iterator that can iterate all CPU features set.
  typedef Support::BitVectorIterator<BitWord> Iterator;

  //! \name Constants
  //! \{

  //! \cond INTERNAL
  enum : uint32_t {
    kMaxFeatures = 256,
    kNumBitWords = kMaxFeatures / Support::kBitWordSizeInBits
  };
  //! \endcond

  //! \}

  //! \name Data
  //! \{

  //! CPU features data.
  struct Data {
    //! \name Members
    //! \{

    //! Data bits.
    Support::Array<BitWord, kNumBitWords> _bits;

    //! \}

    //! \name Overloaded Operators
    //! \{

    inline bool operator==(const Data& other) noexcept { return  eq(other); }
    inline bool operator!=(const Data& other) noexcept { return !eq(other); }

    //! \}

    //! \name Accessors
    //! \{

    //! Returns true if there are no features set.
    inline bool empty() const noexcept { return _bits.aggregate<Support::Or>(0) == 0; }

    //! Returns all features as array of bitwords (see \ref Support::BitWord).
    inline BitWord* bits() noexcept { return _bits.data(); }
    //! Returns all features as array of bitwords (const).
    inline const BitWord* bits() const noexcept { return _bits.data(); }

    //! Returns the number of BitWords returned by \ref bits().
    inline size_t bitWordCount() const noexcept { return kNumBitWords; }

    //! Returns \ref Support::BitVectorIterator, that can be used to iterate over all features efficiently.
    inline Iterator iterator() const noexcept { return Iterator(_bits.data(), kNumBitWords); }

    //! Tests whether the feature `featureId` is present.
    template<typename FeatureId>
    ASMJIT_FORCE_INLINE bool has(const FeatureId& featureId) const noexcept {
      ASMJIT_ASSERT(uint32_t(featureId) < kMaxFeatures);

      uint32_t idx = uint32_t(featureId) / Support::kBitWordSizeInBits;
      uint32_t bit = uint32_t(featureId) % Support::kBitWordSizeInBits;

      return bool((_bits[idx] >> bit) & 0x1);
    }

    template<typename FeatureId>
    ASMJIT_FORCE_INLINE bool hasAny(const FeatureId& featureId) const noexcept {
      return has(featureId);
    }

    template<typename FeatureId, typename... Args>
    ASMJIT_FORCE_INLINE bool hasAny(const FeatureId& featureId, Args&&... otherFeatureIds) const noexcept {
      return bool(unsigned(has(featureId)) | unsigned(hasAny(std::forward<Args>(otherFeatureIds)...)));
    }

    //! Tests whether all features as defined by `other` are present.
    ASMJIT_FORCE_INLINE bool hasAll(const Data& other) const noexcept {
      uint32_t result = 1;
      for (uint32_t i = 0; i < kNumBitWords; i++)
        result &= uint32_t((_bits[i] & other._bits[i]) == other._bits[i]);
      return bool(result);
    }

    //! \}

    //! \name Manipulation
    //! \{

    inline void reset() noexcept { _bits.fill(0); }

    //! Adds the given CPU `featureId` to the list of features.
    template<typename FeatureId>
    ASMJIT_FORCE_INLINE void add(const FeatureId& featureId) noexcept {
      ASMJIT_ASSERT(uint32_t(featureId) < kMaxFeatures);

      uint32_t idx = uint32_t(featureId) / Support::kBitWordSizeInBits;
      uint32_t bit = uint32_t(featureId) % Support::kBitWordSizeInBits;

      _bits[idx] |= BitWord(1) << bit;
    }

    template<typename FeatureId, typename... Args>
    ASMJIT_FORCE_INLINE void add(const FeatureId& featureId, Args&&... otherFeatureIds) noexcept {
      add(featureId);
      add(std::forward<Args>(otherFeatureIds)...);
    }

    template<typename FeatureId>
    ASMJIT_FORCE_INLINE void addIf(bool condition, const FeatureId& featureId) noexcept {
      ASMJIT_ASSERT(uint32_t(featureId) < kMaxFeatures);

      uint32_t idx = uint32_t(featureId) / Support::kBitWordSizeInBits;
      uint32_t bit = uint32_t(featureId) % Support::kBitWordSizeInBits;

      _bits[idx] |= BitWord(condition) << bit;
    }

    template<typename FeatureId, typename... Args>
    ASMJIT_FORCE_INLINE void addIf(bool condition, const FeatureId& featureId, Args&&... otherFeatureIds) noexcept {
      addIf(condition, featureId);
      addIf(condition, std::forward<Args>(otherFeatureIds)...);
    }

    //! Removes the given CPU `featureId` from the list of features.
    template<typename FeatureId>
    ASMJIT_FORCE_INLINE void remove(const FeatureId& featureId) noexcept {
      ASMJIT_ASSERT(uint32_t(featureId) < kMaxFeatures);

      uint32_t idx = uint32_t(featureId) / Support::kBitWordSizeInBits;
      uint32_t bit = uint32_t(featureId) % Support::kBitWordSizeInBits;

      _bits[idx] &= ~(BitWord(1) << bit);
    }

    template<typename FeatureId, typename... Args>
    ASMJIT_FORCE_INLINE void remove(const FeatureId& featureId, Args&&... otherFeatureIds) noexcept {
      remove(featureId);
      remove(std::forward<Args>(otherFeatureIds)...);
    }

    //! Tests whether this CPU features data matches `other`.
    ASMJIT_FORCE_INLINE bool eq(const Data& other) const noexcept { return _bits == other._bits; }

    //! \}

  };

  //! X86 specific features data.
  struct X86 : public Data {
    //! X86 CPU feature identifiers.
    enum Id : uint8_t {
      // @EnumValuesBegin{"enum": "CpuFeatures::X86"}@
      kNone,                     //!< No feature (never set, used internally).

      kMT,                       //!< CPU has multi-threading capabilities.
      kNX,                       //!< CPU has Not-Execute-Bit aka DEP (data-execution prevention).
      k3DNOW,                    //!< CPU has 3DNOW            (3DNOW base instructions) {AMD} (deprecated).
      k3DNOW2,                   //!< CPU has 3DNOW2           (enhanced 3DNOW) {AMD} (deprecated).
      kADX,                      //!< CPU has ADX              (multi-precision add-carry instruction extensions).
      kAESNI,                    //!< CPU has AESNI            (AES encode/decode instructions).
      kALTMOVCR8,                //!< CPU has LOCK MOV R<->CR0 (supports `MOV R<->CR8` via `LOCK MOV R<->CR0` in 32-bit mode) {AMD}.
      kAMX_BF16,                 //!< CPU has AMX_BF16         (AMX-BF16 instructions).
      kAMX_COMPLEX,              //!< CPU has AMX_COMPLEX      (AMX-COMPLEX instructions).
      kAMX_FP16,                 //!< CPU has AMX_FP16         (AMX-FP16 instructions).
      kAMX_INT8,                 //!< CPU has AMX_INT8         (AMX-INT8 instructions).
      kAMX_TILE,                 //!< CPU has AMX_TILE         (advanced matrix extensions).
      kAVX,                      //!< CPU has AVX              (advanced vector extensions).
      kAVX2,                     //!< CPU has AVX2             (advanced vector extensions 2).
      kAVX512_4FMAPS,            //!< CPU has AVX512_FMAPS     (FMA packed single).
      kAVX512_4VNNIW,            //!< CPU has AVX512_VNNIW     (vector NN instructions word variable precision).
      kAVX512_BF16,              //!< CPU has AVX512_BF16      (AVX512 BFLOAT16 support instructions).
      kAVX512_BITALG,            //!< CPU has AVX512_BITALG    (AVX512 VPOPCNT[B|W] and VPSHUFBITQMB instructions).
      kAVX512_BW,                //!< CPU has AVX512_BW        (AVX512 integer BYTE|WORD instructions).
      kAVX512_CD,                //!< CPU has AVX512_CD        (AVX512 conflict detection DWORD|QWORD instructions).
      kAVX512_DQ,                //!< CPU has AVX512_DQ        (AVX512 integer DWORD|QWORD instructions).
      kAVX512_ER,                //!< CPU has AVX512_ER        (AVX512 exponential and reciprocal instructions).
      kAVX512_F,                 //!< CPU has AVX512_F         (AVX512 foundation).
      kAVX512_FP16,              //!< CPU has AVX512_FP16      (AVX512 FP16 instructions).
      kAVX512_IFMA,              //!< CPU has AVX512_IFMA      (AVX512 integer fused-multiply-add using 52-bit precision).
      kAVX512_PF,                //!< CPU has AVX512_PF        (AVX512 prefetch instructions).
      kAVX512_VBMI,              //!< CPU has AVX512_VBMI      (AVX152 vector byte manipulation instructions).
      kAVX512_VBMI2,             //!< CPU has AVX512_VBMI2     (AVX512 vector byte manipulation instructions v2).
      kAVX512_VL,                //!< CPU has AVX512_VL        (AVX512 vector length extensions).
      kAVX512_VNNI,              //!< CPU has AVX512_VNNI      (AVX512 vector neural network instructions).
      kAVX512_VP2INTERSECT,      //!< CPU has AVX512_VP2INTERSECT
      kAVX512_VPOPCNTDQ,         //!< CPU has AVX512_VPOPCNTDQ (AVX512 VPOPCNT[D|Q] instructions).
      kAVX_IFMA,                 //!< CPU has AVX_IFMA         (AVX/VEX encoding of vpmadd52huq/vpmadd52luq).
      kAVX_NE_CONVERT,           //!< CPU has AVX_NE_CONVERT.
      kAVX_VNNI,                 //!< CPU has AVX_VNNI         (AVX/VEX encoding of vpdpbusd/vpdpbusds/vpdpwssd/vpdpwssds).
      kAVX_VNNI_INT8,            //!< CPU has AVX_VNNI_INT8.
      kBMI,                      //!< CPU has BMI              (bit manipulation instructions #1).
      kBMI2,                     //!< CPU has BMI2             (bit manipulation instructions #2).
      kCET_IBT,                  //!< CPU has CET-IBT          (indirect branch tracking).
      kCET_SS,                   //!< CPU has CET-SS.
      kCET_SSS,                  //!< CPU has CET-SSS.
      kCLDEMOTE,                 //!< CPU has CLDEMOTE         (cache line demote).
      kCLFLUSH,                  //!< CPU has CLFUSH           (cache Line flush).
      kCLFLUSHOPT,               //!< CPU has CLFUSHOPT        (cache Line flush - optimized).
      kCLWB,                     //!< CPU has CLWB.
      kCLZERO,                   //!< CPU has CLZERO.
      kCMOV,                     //!< CPU has CMOV             (CMOV and FCMOV instructions).
      kCMPCCXADD,                //!< CPU has CMPCCXADD.
      kCMPXCHG16B,               //!< CPU has CMPXCHG16B       (compare-exchange 16 bytes) {X86_64}.
      kCMPXCHG8B,                //!< CPU has CMPXCHG8B        (compare-exchange 8 bytes).
      kENCLV,                    //!< CPU has ENCLV.
      kENQCMD,                   //!< CPU has ENQCMD           (enqueue stores).
      kERMS,                     //!< CPU has ERMS             (enhanced REP MOVSB/STOSB).
      kF16C,                     //!< CPU has F16C             (AVX FP16 conversion instructions).
      kFMA,                      //!< CPU has FMA              (AVX fused-multiply-add - 3 operand form).
      kFMA4,                     //!< CPU has FMA4             (AVX fused-multiply-add - 4 operand form) (deprecated).
      kFPU,                      //!< CPU has FPU              (FPU support).
      kFSGSBASE,                 //!< CPU has FSGSBASE.
      kFSRM,                     //!< CPU has FSRM             (fast short REP MOVSB).
      kFSRC,                     //!< CPU has FSRC             (fast short REP CMPSB|SCASB).
      kFSRS,                     //!< CPU has FSRS             (fast short REP STOSB)
      kFXSR,                     //!< CPU has FXSR             (FXSAVE/FXRSTOR instructions).
      kFXSROPT,                  //!< CPU has FXSROTP          (FXSAVE/FXRSTOR is optimized).
      kFZRM,                     //!< CPU has FZRM             (fast zero-length REP MOVSB).
      kGEODE,                    //!< CPU has GEODE extensions (GEODE 3DNOW additions) (deprecated).
      kGFNI,                     //!< CPU has GFNI             (galois field instructions).
      kHLE,                      //!< CPU has HLE.
      kHRESET,                   //!< CPU has HRESET.
      kI486,                     //!< CPU has I486 features    (I486+ support).
      kLAHFSAHF,                 //!< CPU has LAHF/SAHF        (LAHF/SAHF in 64-bit mode) {X86_64}.
      kLAM,                      //!< CPU has LAM              (linear address masking) {X86_64}.
      kLWP,                      //!< CPU has LWP              (lightweight profiling) {AMD}.
      kLZCNT,                    //!< CPU has LZCNT            (LZCNT instruction).
      kMCOMMIT,                  //!< CPU has MCOMMIT          (MCOMMIT instruction).
      kMMX,                      //!< CPU has MMX              (MMX base instructions) (deprecated).
      kMMX2,                     //!< CPU has MMX2             (MMX2 extensions or initial SSE extensions) (deprecated).
      kMONITOR,                  //!< CPU has MONITOR          (MONITOR/MWAIT instructions).
      kMONITORX,                 //!< CPU has MONITORX         (MONITORX/MWAITX instructions).
      kMOVBE,                    //!< CPU has MOVBE            (move with byte-order swap).
      kMOVDIR64B,                //!< CPU has MOVDIR64B        (move 64 bytes as direct store).
      kMOVDIRI,                  //!< CPU has MOVDIRI          (move dword/qword as direct store).
      kMPX,                      //!< CPU has MPX              (memory protection extensions).
      kMSR,                      //!< CPU has MSR              (RDMSR/WRMSR instructions).
      kMSRLIST,                  //!< CPU has MSRLIST.
      kMSSE,                     //!< CPU has MSSE             (misaligned SSE support).
      kOSXSAVE,                  //!< CPU has OSXSAVE          (XSAVE enabled by OS).
      kOSPKE,                    //!< CPU has OSPKE            (PKE enabled by OS).
      kPCLMULQDQ,                //!< CPU has PCLMULQDQ        (packed carry-less multiplication).
      kPCONFIG,                  //!< CPU has PCONFIG          (PCONFIG instruction).
      kPOPCNT,                   //!< CPU has POPCNT           (POPCNT instruction).
      kPREFETCHITI,              //!< CPU has PREFETCHIIT.
      kPREFETCHW,                //!< CPU has PREFETCHW.
      kPREFETCHWT1,              //!< CPU has PREFETCHWT1.
      kPTWRITE,                  //!< CPU has PTWRITE.
      kRAO_INT,                  //!< CPU has RAO_INT.
      kRDPID,                    //!< CPU has RDPID.
      kRDPRU,                    //!< CPU has RDPRU.
      kRDRAND,                   //!< CPU has RDRAND.
      kRDSEED,                   //!< CPU has RDSEED.
      kRDTSC,                    //!< CPU has RDTSC.
      kRDTSCP,                   //!< CPU has RDTSCP.
      kRTM,                      //!< CPU has RTM.
      kSERIALIZE,                //!< CPU has SERIALIZE.
      kSHA,                      //!< CPU has SHA              (SHA-1 and SHA-256 instructions).
      kSKINIT,                   //!< CPU has SKINIT           (SKINIT/STGI instructions) {AMD}.
      kSMAP,                     //!< CPU has SMAP             (supervisor-mode access prevention).
      kSMEP,                     //!< CPU has SMEP             (supervisor-mode execution prevention).
      kSMX,                      //!< CPU has SMX              (safer mode extensions).
      kSNP,                      //!< CPU has SNP.
      kSSE,                      //!< CPU has SSE              (SSE instructions).
      kSSE2,                     //!< CPU has SSE2             (SSE2 instructions).
      kSSE3,                     //!< CPU has SSE3             (SSE3 instructions).
      kSSE4_1,                   //!< CPU has SSE4.1           (SSE4.1 instructions).
      kSSE4_2,                   //!< CPU has SSE4.2           (SSE4.2 instructions).
      kSSE4A,                    //!< CPU has SSE4A            (SSE4.A instructions) {AMD} (deprecated).
      kSSSE3,                    //!< CPU has SSSE3            (SSSE3 instructions).
      kSVM,                      //!< CPU has SVM              (virtualization) {AMD}.
      kTBM,                      //!< CPU has TBM              (trailing bit manipulation) {AMD}.
      kTSX,                      //!< CPU has TSX.
      kTSXLDTRK,                 //!< CPU has TSXLDTRK.
      kUINTR,                    //!< CPU has UINTR            (user interrupts).
      kVAES,                     //!< CPU has VAES             (vector AES 256|512 bit support).
      kVMX,                      //!< CPU has VMX              (virtualization) {INTEL}.
      kVPCLMULQDQ,               //!< CPU has VPCLMULQDQ       (vector PCLMULQDQ 256|512-bit support).
      kWAITPKG,                  //!< CPU has WAITPKG          (UMONITOR, UMWAIT, TPAUSE).
      kWBNOINVD,                 //!< CPU has WBNOINVD.
      kWRMSRNS,                  //!< CPU has WRMSRNS.
      kXOP,                      //!< CPU has XOP              (XOP instructions) {AMD} (deprecated).
      kXSAVE,                    //!< CPU has XSAVE.
      kXSAVEC,                   //!< CPU has XSAVEC.
      kXSAVEOPT,                 //!< CPU has XSAVEOPT.
      kXSAVES,                   //!< CPU has XSAVES.
      // @EnumValuesEnd@

      kMaxValue = kXSAVES
    };

    #define ASMJIT_X86_FEATURE(FEATURE) \
      inline bool has##FEATURE() const noexcept { return has(X86::k##FEATURE); }

    ASMJIT_X86_FEATURE(MT)
    ASMJIT_X86_FEATURE(NX)
    ASMJIT_X86_FEATURE(3DNOW)
    ASMJIT_X86_FEATURE(3DNOW2)
    ASMJIT_X86_FEATURE(ADX)
    ASMJIT_X86_FEATURE(AESNI)
    ASMJIT_X86_FEATURE(ALTMOVCR8)
    ASMJIT_X86_FEATURE(AMX_BF16)
    ASMJIT_X86_FEATURE(AMX_COMPLEX)
    ASMJIT_X86_FEATURE(AMX_FP16)
    ASMJIT_X86_FEATURE(AMX_INT8)
    ASMJIT_X86_FEATURE(AMX_TILE)
    ASMJIT_X86_FEATURE(AVX)
    ASMJIT_X86_FEATURE(AVX2)
    ASMJIT_X86_FEATURE(AVX512_4FMAPS)
    ASMJIT_X86_FEATURE(AVX512_4VNNIW)
    ASMJIT_X86_FEATURE(AVX512_BF16)
    ASMJIT_X86_FEATURE(AVX512_BITALG)
    ASMJIT_X86_FEATURE(AVX512_BW)
    ASMJIT_X86_FEATURE(AVX512_CD)
    ASMJIT_X86_FEATURE(AVX512_DQ)
    ASMJIT_X86_FEATURE(AVX512_ER)
    ASMJIT_X86_FEATURE(AVX512_F)
    ASMJIT_X86_FEATURE(AVX512_FP16)
    ASMJIT_X86_FEATURE(AVX512_IFMA)
    ASMJIT_X86_FEATURE(AVX512_PF)
    ASMJIT_X86_FEATURE(AVX512_VBMI)
    ASMJIT_X86_FEATURE(AVX512_VBMI2)
    ASMJIT_X86_FEATURE(AVX512_VL)
    ASMJIT_X86_FEATURE(AVX512_VNNI)
    ASMJIT_X86_FEATURE(AVX512_VP2INTERSECT)
    ASMJIT_X86_FEATURE(AVX512_VPOPCNTDQ)
    ASMJIT_X86_FEATURE(AVX_IFMA)
    ASMJIT_X86_FEATURE(AVX_NE_CONVERT)
    ASMJIT_X86_FEATURE(AVX_VNNI)
    ASMJIT_X86_FEATURE(AVX_VNNI_INT8)
    ASMJIT_X86_FEATURE(BMI)
    ASMJIT_X86_FEATURE(BMI2)
    ASMJIT_X86_FEATURE(CET_IBT)
    ASMJIT_X86_FEATURE(CET_SS)
    ASMJIT_X86_FEATURE(CET_SSS)
    ASMJIT_X86_FEATURE(CLDEMOTE)
    ASMJIT_X86_FEATURE(CLFLUSH)
    ASMJIT_X86_FEATURE(CLFLUSHOPT)
    ASMJIT_X86_FEATURE(CLWB)
    ASMJIT_X86_FEATURE(CLZERO)
    ASMJIT_X86_FEATURE(CMOV)
    ASMJIT_X86_FEATURE(CMPXCHG16B)
    ASMJIT_X86_FEATURE(CMPXCHG8B)
    ASMJIT_X86_FEATURE(ENCLV)
    ASMJIT_X86_FEATURE(ENQCMD)
    ASMJIT_X86_FEATURE(ERMS)
    ASMJIT_X86_FEATURE(F16C)
    ASMJIT_X86_FEATURE(FMA)
    ASMJIT_X86_FEATURE(FMA4)
    ASMJIT_X86_FEATURE(FPU)
    ASMJIT_X86_FEATURE(FSGSBASE)
    ASMJIT_X86_FEATURE(FSRM)
    ASMJIT_X86_FEATURE(FSRC)
    ASMJIT_X86_FEATURE(FSRS)
    ASMJIT_X86_FEATURE(FXSR)
    ASMJIT_X86_FEATURE(FXSROPT)
    ASMJIT_X86_FEATURE(FZRM)
    ASMJIT_X86_FEATURE(GEODE)
    ASMJIT_X86_FEATURE(GFNI)
    ASMJIT_X86_FEATURE(HLE)
    ASMJIT_X86_FEATURE(HRESET)
    ASMJIT_X86_FEATURE(I486)
    ASMJIT_X86_FEATURE(LAHFSAHF)
    ASMJIT_X86_FEATURE(LAM)
    ASMJIT_X86_FEATURE(LWP)
    ASMJIT_X86_FEATURE(LZCNT)
    ASMJIT_X86_FEATURE(MCOMMIT)
    ASMJIT_X86_FEATURE(MMX)
    ASMJIT_X86_FEATURE(MMX2)
    ASMJIT_X86_FEATURE(MONITOR)
    ASMJIT_X86_FEATURE(MONITORX)
    ASMJIT_X86_FEATURE(MOVBE)
    ASMJIT_X86_FEATURE(MOVDIR64B)
    ASMJIT_X86_FEATURE(MOVDIRI)
    ASMJIT_X86_FEATURE(MPX)
    ASMJIT_X86_FEATURE(MSR)
    ASMJIT_X86_FEATURE(MSRLIST)
    ASMJIT_X86_FEATURE(MSSE)
    ASMJIT_X86_FEATURE(OSXSAVE)
    ASMJIT_X86_FEATURE(OSPKE)
    ASMJIT_X86_FEATURE(PCLMULQDQ)
    ASMJIT_X86_FEATURE(PCONFIG)
    ASMJIT_X86_FEATURE(POPCNT)
    ASMJIT_X86_FEATURE(PREFETCHITI)
    ASMJIT_X86_FEATURE(PREFETCHW)
    ASMJIT_X86_FEATURE(PREFETCHWT1)
    ASMJIT_X86_FEATURE(PTWRITE)
    ASMJIT_X86_FEATURE(RAO_INT)
    ASMJIT_X86_FEATURE(RDPID)
    ASMJIT_X86_FEATURE(RDPRU)
    ASMJIT_X86_FEATURE(RDRAND)
    ASMJIT_X86_FEATURE(RDSEED)
    ASMJIT_X86_FEATURE(RDTSC)
    ASMJIT_X86_FEATURE(RDTSCP)
    ASMJIT_X86_FEATURE(RTM)
    ASMJIT_X86_FEATURE(SERIALIZE)
    ASMJIT_X86_FEATURE(SHA)
    ASMJIT_X86_FEATURE(SKINIT)
    ASMJIT_X86_FEATURE(SMAP)
    ASMJIT_X86_FEATURE(SMEP)
    ASMJIT_X86_FEATURE(SMX)
    ASMJIT_X86_FEATURE(SNP)
    ASMJIT_X86_FEATURE(SSE)
    ASMJIT_X86_FEATURE(SSE2)
    ASMJIT_X86_FEATURE(SSE3)
    ASMJIT_X86_FEATURE(SSE4_1)
    ASMJIT_X86_FEATURE(SSE4_2)
    ASMJIT_X86_FEATURE(SSE4A)
    ASMJIT_X86_FEATURE(SSSE3)
    ASMJIT_X86_FEATURE(SVM)
    ASMJIT_X86_FEATURE(TBM)
    ASMJIT_X86_FEATURE(TSX)
    ASMJIT_X86_FEATURE(TSXLDTRK)
    ASMJIT_X86_FEATURE(UINTR)
    ASMJIT_X86_FEATURE(VAES)
    ASMJIT_X86_FEATURE(VMX)
    ASMJIT_X86_FEATURE(VPCLMULQDQ)
    ASMJIT_X86_FEATURE(WAITPKG)
    ASMJIT_X86_FEATURE(WBNOINVD)
    ASMJIT_X86_FEATURE(WRMSRNS)
    ASMJIT_X86_FEATURE(XOP)
    ASMJIT_X86_FEATURE(XSAVE)
    ASMJIT_X86_FEATURE(XSAVEC)
    ASMJIT_X86_FEATURE(XSAVEOPT)
    ASMJIT_X86_FEATURE(XSAVES)

    #undef ASMJIT_X86_FEATURE
  };

  //! ARM specific features data.
  //!
  //! Naming reference:
  //!   - https://developer.arm.com/downloads/-/exploration-tools/feature-names-for-a-profile
  struct ARM : public Data {
    //! ARM CPU feature identifiers.
    enum Id : uint8_t {
      // @EnumValuesBegin{"enum": "CpuFeatures::ARM"}@
      kNone = 0,                 //!< No feature (never set, used internally).

      kARMv6,                    //!< CPU is at least ARMv6 {A32}.
      kARMv7,                    //!< CPU is at least ARMv7 {A32}.
      kARMv8a,                   //!< CPU is at least ARMv8A.
      kTHUMB,                    //!< CPU has THUMB        (16-bit THUMB encoding) {A32}.
      kTHUMBv2,                  //!< CPU has THUMBv2      (32-bit THUMB encoding) {A32}.

      kAES,                      //!< CPU has AES          (ASIMD AES instructions).
      kAFP,                      //!< CPU has AFP          (alternate floating-point behavior) {A64}.
      kASIMD,                    //!< CPU has ASIMD        (NEON on ARM/THUMB).
      kBF16,                     //!< CPU has BF16         (BFloat16 instructions) {A64}.
      kBTI,                      //!< CPU has BTI          (branch target identification).
      kCCIDX,                    //!< CPU has CCIDX        (extend of the CCSIDR number of sets).
      kCHK,                      //!< CPU has CHK          (CHKFEAT instruction) {A64}.
      kCLRBHB,                   //!< CPU has CLRBHB       (clear BHB instruction).
      kCPUID,                    //!< CPU has CPUID        (CPUID registers accessible in user-space).
      kCRC32,                    //!< CPU has CRC32        (CRC32 instructions).
      kCSSC,                     //!< CPU has CSSC         (common short sequence compression) {A64}.
      kD128,                     //!< CPU has D128         (128-bit translation tables, 56 bit PA) {A64}.
      kDGH,                      //!< CPU has DGH          (data gathering hint) {A64}.
      kDIT,                      //!< CPU has DIT          (data independent timing of instructions).
      kDOTPROD,                  //!< CPU has DOTPROD      (ASIMD Int8 dot product instructions).
      kDPB,                      //!< CPU has DPB          (DC CVAP instruction) {A64}.
      kDPB2,                     //!< CPU has DPB2         (DC CVADP instruction) {A64}.
      kEBF16,                    //!< CPU has EBF16        (extended BFloat16 mode) {A64}.
      kECV,                      //!< CPU has ECV          (enhanced counter virtualization).
      kEDSP,                     //!< CPU has EDSP         (ARM/THUMB only).
      kFCMA,                     //!< CPU has FCMA         (FCADD/FCMLA).
      kFGT,                      //!< CPU has FGT          (fine-grained traps).
      kFGT2,                     //!< CPU has FGT2         (fine-grained traps 2).
      kFHM,                      //!< CPU has FHM          (half-precision floating-point FMLAL instructions).
      kFLAGM,                    //!< CPU has FLAGM        (condition flag manipulation) {A64}.
      kFLAGM2,                   //!< CPU has FLAGM2       (condition flag manipulation version v2) {A64}.
      kFMAC,                     //!< CPU has FMAC         (ARM/THUMB only).
      kFP,                       //!< CPU has FP           (floating-point) (on 32-bit ARM this means VFPv3).
      kFP16,                     //!< CPU has FP16         (half-precision floating-point data processing).
      kFP16CONV,                 //!< CPU has FP16CONV     (half-precision float conversion).
      kFRINTTS,                  //!< CPU has FRINTTS      (FRINT[32|64][X|Z] instructions) {A64}.
      kGCS,                      //!< CPU has GCS          (guarded control stack extension) {A64}.
      kHBC,                      //!< CPU has HBC          (hinted conditional branches) {A64}
      kHCX,                      //!< CPU has HCX          (support for the HCRX_EL2 register) {A64}.
      kI8MM,                     //!< CPU has I8MM         (int8 matrix multiplication) {A64}.
      kIDIVA,                    //!< CPU has IDIV         (hardware SDIV and UDIV in ARM mode).
      kIDIVT,                    //!< CPU has IDIV         (hardware SDIV and UDIV in THUMB mode).
      kJSCVT,                    //!< CPU has JSCVT        (JavaScript FJCVTS conversion instruction) {A64}.
      kLOR,                      //!< CPU has LOR          (limited ordering regions extension).
      kLRCPC,                    //!< CPU has LRCPC        (load-acquire RCpc instructions) {A64}.
      kLRCPC2,                   //!< CPU has LRCPC2       (load-acquire RCpc instructions v2) {A64}.
      kLRCPC3,                   //!< CPU has LRCPC3       (load-Acquire RCpc instructions v3) {A64}.
      kLS64,                     //!< CPU has LS64         (64 byte loads/stores without return) {A64}.
      kLS64_ACCDATA,             //!< CPU has LS64_ACCDATA (64-byte EL0 stores with return) {A64}.
      kLS64_V,                   //!< CPU has LS64_V       (64-byte stores with return) {A64}.
      kLSE,                      //!< CPU has LSE          (large system extensions) {A64}.
      kLSE128,                   //!< CPU has LSE128       (128-bit atomics) {A64}.
      kLSE2,                     //!< CPU has LSE2         (large system extensions v2) {A64}.
      kMOPS,                     //!< CPU has MOPS         (memcpy and memset acceleration instructions) {A64}.
      kMPAM,                     //!< CPU has MPAM         (memory system partitioning and monitoring extension) {A64}.
      kMTE,                      //!< CPU has MTE          (instruction-only memory tagging extension) {A64}.
      kMTE2,                     //!< CPU has MTE2         (full memory tagging extension) {A64}.
      kMTE3,                     //!< CPU has MTE3         (MTE asymmetric fault handling) {A64}.
      kMTE4,                     //!< CPU has MTE4         (MTE v4) {A64}.
      kNMI,                      //!< CPU has NMI          (non-maskable Interrupt) {A64}.
      kNV,                       //!< CPU has NV           (nested virtualization enchancement) {A64}.
      kNV2,                      //!< CPU has NV2          (enhanced support for nested virtualization) {A64}.
      kPAN,                      //!< CPU has PAN          (privileged access-never extension) {A64}.
      kPAN2,                     //!< CPU has PAN2         (PAN s1e1R and s1e1W variants) {A64}.
      kPAN3,                     //!< CPU has PAN3         (support for SCTLR_ELx.EPAN) {A64}.
      kPAUTH,                    //!< CPU has PAUTH        (pointer authentication extension) {A64}.
      kPMU,                      //!< CPU has PMU          {A64}.
      kPMULL,                    //!< CPU has PMULL        {A64}.
      kPRFMSLC,                  //!< CPU has PRFMSLC      (PRFM instructions support the SLC target) {A64}
      kRAS,                      //!< CPU has RAS          (reliability, availability and serviceability extensions).
      kRAS1_1,                   //!< CPU has RASv1p1      (RAS v1.1).
      kRAS2,                     //!< CPU has RASv2        (RAS v2).
      kRDM,                      //!< CPU has RDM          (rounding double multiply accumulate) {A64}.
      kRME,                      //!< CPU has RME          (memory encryption contexts extension) {A64}.
      kRNG,                      //!< CPU has RNG          (random number generation).
      kRNG_TRAP,                 //!< CPU has RNG_TRAP     (random number trap to EL3 field) {A64}.
      kRPRES,                    //!< CPU has RPRES        (increased precision of reciprocal estimate and RSQRT estimate) {A64}.
      kRPRFM,                    //!! CPU has RPRFM        (range prefetch hint instruction).
      kSB,                       //!< CPU has SB           (speculative barrier).
      kSHA1,                     //!< CPU has SHA1         (ASIMD SHA1 instructions).
      kSHA256,                   //!< CPU has SHA256       (ASIMD SHA256 instructions).
      kSHA3,                     //!< CPU has SHA3         (ASIMD EOR3, RAX1, XAR, and BCAX instructions).
      kSHA512,                   //!< CPU has SHA512       (ASIMD SHA512 instructions).
      kSM3,                      //!< CPU has SM3          (ASIMD SM3 instructions).
      kSM4,                      //!< CPU has SM4          (ASIMD SM4 instructions).
      kSME,                      //!< CPU has SME          (SME v1 - scalable matrix extension) {A64}.
      kSME2,                     //!< CPU has SME2         (SME v2) {A64}.
      kSME2_1,                   //!< CPU has SME2p1       (SME v2.1) {A64}.
      kSME_B16B16,               //!< CPU has SME_B16B16   (SME non-widening BFloat16 to BFloat16 arithmetic) {A64}.
      kSME_B16F32,               //!< CPU has SME_B16F32   {A64}.
      kSME_BI32I32,              //!< CPU has SME_BI32I32  {A64}.
      kSME_F16F16,               //!< CPU has SME_F16F16   (SME2.1 non-widening half-precision FP16 to FP16 arithmetic) {A64}.
      kSME_F16F32,               //!< CPU has SME_F16F32   {A64}.
      kSME_F32F32,               //!< CPU has SME_F32F32   {A64}.
      kSME_F64F64,               //!< CPU has SME_F64F64   {A64}.
      kSME_FA64,                 //!< CPU has SME_FA64     {A64}.
      kSME_I16I32,               //!< CPU has SME_I16I32   {A64}.
      kSME_I16I64,               //!< CPU has SME_I16I64   {A64}.
      kSME_I8I32,                //!< CPU has SME_I8I32    {A64}.
      kSPECRES,                  //!< CPU has SPECRES      (speculation restriction instructions).
      kSPECRES2,                 //!< CPU has SPECRES2     (clear other speculative predictions).
      kSSBS,                     //!< CPU has SSBS         (speculative store bypass safe instruction).
      kSSBS2,                    //!< CPU has SSBS2        (MRS and MSR instructions for SSBS).
      kSVE,                      //!< CPU has SVE          (SVE v1 - scalable vector extension) {A64}.
      kSVE2,                     //!< CPU has SVE2         (SVE v2) {A64}.
      kSVE2_1,                   //!< CPU has SVE2p1       (SVE v2.1) {A64}.
      kSVE_AES,                  //!< CPU has SVE_AES      (SVE AES instructions) {A64}.
      kSVE_B16B16,               //!< CPU has SVE_B16B16   (SVE non-widening BFloat16 to BFloat16 arithmetic) {A64}.
      kSVE_BF16,                 //!< CPU has SVE_BF16     (SVE BF16 instructions) {A64}.
      kSVE_BITPERM,              //!< CPU has SVE_BITPERM  (SVE bit permute) {A64}.
      kSVE_EBF16,                //!< CPU has SVE_EBF16    (SVE extended BFloat16 mode) {A64}.
      kSVE_F32MM,                //!< CPU has SVE_F32MM    (SVE single-precision floating-point matrix multiply instruction) {A64}.
      kSVE_F64MM,                //!< CPU has SVE_F64MM    (SVE double-precision floating-point matrix multiply instruction) {A64}.
      kSVE_I8MM,                 //!< CPU has SVE_I8MM     (SVE int8 matrix multiplication) {A64}.
      kSVE_PMULL128,             //!< CPU has SVE_PMULL128 (SVE PMULL instructions) {A64}.
      kSVE_SHA3,                 //!< CPU has SVE_SHA3     (SVE SHA-3 instructions) {A64}.
      kSVE_SM4,                  //!< CPU has SVE_SM4      (SVE SM4 instructions {A64}.
      kSYSINSTR128,              //!< CPU has SYSINSTR128  (128-bit system instructions) {A64}.
      kSYSREG128,                //!< CPU has SYSREG128    (128-bit system registers) {A64}.
      kTHE,                      //!< CPU has THE          (translation hardening extension).
      kTME,                      //!< CPU has TME          (transactional memory extensions).
      kTRF,                      //!< CPU has TRF          (trace extension).
      kUAO,                      //!< CPU has UAO          (AArch64 v8.2 UAO PState) {A64}.
      kVFP_D32,                  //!< CPU has VFP_D32      (32 VFP-D registers) (ARM/THUMB only).
      kVHE,                      //!< CPU has VHE          (virtual host extension).
      kWFXT,                     //!< CPU has WFxT         (WFE and WFI instructions with timeout) {A64}.
      kXS,                       //!< CPU has XS           (XS attribute in TLBI and DSB instructions) {A64}.
      // @EnumValuesEnd@

      kMaxValue = kXS
    };

    #define ASMJIT_ARM_FEATURE(FEATURE) \
      inline bool has##FEATURE() const noexcept { return has(ARM::k##FEATURE); }

    ASMJIT_ARM_FEATURE(THUMB)
    ASMJIT_ARM_FEATURE(THUMBv2)

    ASMJIT_ARM_FEATURE(ARMv6)
    ASMJIT_ARM_FEATURE(ARMv7)
    ASMJIT_ARM_FEATURE(ARMv8a)

    ASMJIT_ARM_FEATURE(AES)
    ASMJIT_ARM_FEATURE(AFP)
    ASMJIT_ARM_FEATURE(ASIMD)
    ASMJIT_ARM_FEATURE(BF16)
    ASMJIT_ARM_FEATURE(BTI)
    ASMJIT_ARM_FEATURE(CCIDX)
    ASMJIT_ARM_FEATURE(CHK)
    ASMJIT_ARM_FEATURE(CLRBHB)
    ASMJIT_ARM_FEATURE(CPUID)
    ASMJIT_ARM_FEATURE(CRC32)
    ASMJIT_ARM_FEATURE(CSSC)
    ASMJIT_ARM_FEATURE(D128)
    ASMJIT_ARM_FEATURE(DGH)
    ASMJIT_ARM_FEATURE(DIT)
    ASMJIT_ARM_FEATURE(DOTPROD)
    ASMJIT_ARM_FEATURE(DPB)
    ASMJIT_ARM_FEATURE(DPB2)
    ASMJIT_ARM_FEATURE(EBF16)
    ASMJIT_ARM_FEATURE(ECV)
    ASMJIT_ARM_FEATURE(EDSP)
    ASMJIT_ARM_FEATURE(FCMA)
    ASMJIT_ARM_FEATURE(FGT)
    ASMJIT_ARM_FEATURE(FGT2)
    ASMJIT_ARM_FEATURE(FHM)
    ASMJIT_ARM_FEATURE(FLAGM)
    ASMJIT_ARM_FEATURE(FLAGM2)
    ASMJIT_ARM_FEATURE(FMAC)
    ASMJIT_ARM_FEATURE(FP)
    ASMJIT_ARM_FEATURE(FP16)
    ASMJIT_ARM_FEATURE(FP16CONV)
    ASMJIT_ARM_FEATURE(FRINTTS)
    ASMJIT_ARM_FEATURE(GCS)
    ASMJIT_ARM_FEATURE(HBC)
    ASMJIT_ARM_FEATURE(HCX)
    ASMJIT_ARM_FEATURE(I8MM)
    ASMJIT_ARM_FEATURE(IDIVA)
    ASMJIT_ARM_FEATURE(IDIVT)
    ASMJIT_ARM_FEATURE(JSCVT)
    ASMJIT_ARM_FEATURE(LOR)
    ASMJIT_ARM_FEATURE(LRCPC)
    ASMJIT_ARM_FEATURE(LRCPC2)
    ASMJIT_ARM_FEATURE(LRCPC3)
    ASMJIT_ARM_FEATURE(LS64)
    ASMJIT_ARM_FEATURE(LS64_ACCDATA)
    ASMJIT_ARM_FEATURE(LS64_V)
    ASMJIT_ARM_FEATURE(LSE)
    ASMJIT_ARM_FEATURE(LSE128)
    ASMJIT_ARM_FEATURE(LSE2)
    ASMJIT_ARM_FEATURE(MOPS)
    ASMJIT_ARM_FEATURE(MPAM)
    ASMJIT_ARM_FEATURE(MTE)
    ASMJIT_ARM_FEATURE(MTE2)
    ASMJIT_ARM_FEATURE(MTE3)
    ASMJIT_ARM_FEATURE(MTE4)
    ASMJIT_ARM_FEATURE(NMI)
    ASMJIT_ARM_FEATURE(NV)
    ASMJIT_ARM_FEATURE(NV2)
    ASMJIT_ARM_FEATURE(PAN)
    ASMJIT_ARM_FEATURE(PAN2)
    ASMJIT_ARM_FEATURE(PAN3)
    ASMJIT_ARM_FEATURE(PAUTH)
    ASMJIT_ARM_FEATURE(PMU)
    ASMJIT_ARM_FEATURE(PMULL)
    ASMJIT_ARM_FEATURE(PRFMSLC)
    ASMJIT_ARM_FEATURE(RAS)
    ASMJIT_ARM_FEATURE(RAS1_1)
    ASMJIT_ARM_FEATURE(RAS2)
    ASMJIT_ARM_FEATURE(RDM)
    ASMJIT_ARM_FEATURE(RME)
    ASMJIT_ARM_FEATURE(RNG)
    ASMJIT_ARM_FEATURE(RNG_TRAP)
    ASMJIT_ARM_FEATURE(RPRES)
    ASMJIT_ARM_FEATURE(RPRFM)
    ASMJIT_ARM_FEATURE(SB)
    ASMJIT_ARM_FEATURE(SHA1)
    ASMJIT_ARM_FEATURE(SHA256)
    ASMJIT_ARM_FEATURE(SHA3)
    ASMJIT_ARM_FEATURE(SHA512)
    ASMJIT_ARM_FEATURE(SM3)
    ASMJIT_ARM_FEATURE(SM4)
    ASMJIT_ARM_FEATURE(SME)
    ASMJIT_ARM_FEATURE(SME2)
    ASMJIT_ARM_FEATURE(SME2_1)
    ASMJIT_ARM_FEATURE(SME_B16B16)
    ASMJIT_ARM_FEATURE(SME_B16F32)
    ASMJIT_ARM_FEATURE(SME_BI32I32)
    ASMJIT_ARM_FEATURE(SME_F16F16)
    ASMJIT_ARM_FEATURE(SME_F16F32)
    ASMJIT_ARM_FEATURE(SME_F32F32)
    ASMJIT_ARM_FEATURE(SME_F64F64)
    ASMJIT_ARM_FEATURE(SME_FA64)
    ASMJIT_ARM_FEATURE(SME_I16I32)
    ASMJIT_ARM_FEATURE(SME_I16I64)
    ASMJIT_ARM_FEATURE(SME_I8I32)
    ASMJIT_ARM_FEATURE(SPECRES)
    ASMJIT_ARM_FEATURE(SPECRES2)
    ASMJIT_ARM_FEATURE(SSBS)
    ASMJIT_ARM_FEATURE(SSBS2)
    ASMJIT_ARM_FEATURE(SVE)
    ASMJIT_ARM_FEATURE(SVE2)
    ASMJIT_ARM_FEATURE(SVE2_1)
    ASMJIT_ARM_FEATURE(SVE_AES)
    ASMJIT_ARM_FEATURE(SVE_B16B16)
    ASMJIT_ARM_FEATURE(SVE_BF16)
    ASMJIT_ARM_FEATURE(SVE_BITPERM)
    ASMJIT_ARM_FEATURE(SVE_EBF16)
    ASMJIT_ARM_FEATURE(SVE_F32MM)
    ASMJIT_ARM_FEATURE(SVE_F64MM)
    ASMJIT_ARM_FEATURE(SVE_I8MM)
    ASMJIT_ARM_FEATURE(SVE_PMULL128)
    ASMJIT_ARM_FEATURE(SVE_SHA3)
    ASMJIT_ARM_FEATURE(SVE_SM4)
    ASMJIT_ARM_FEATURE(SYSINSTR128)
    ASMJIT_ARM_FEATURE(SYSREG128)
    ASMJIT_ARM_FEATURE(THE)
    ASMJIT_ARM_FEATURE(TME)
    ASMJIT_ARM_FEATURE(TRF)
    ASMJIT_ARM_FEATURE(UAO)
    ASMJIT_ARM_FEATURE(VFP_D32)
    ASMJIT_ARM_FEATURE(VHE)
    ASMJIT_ARM_FEATURE(WFXT)
    ASMJIT_ARM_FEATURE(XS)

    #undef ASMJIT_ARM_FEATURE
  };

  static_assert(uint32_t(X86::kMaxValue) < kMaxFeatures, "The number of X86 CPU features cannot exceed CpuFeatures::kMaxFeatures");
  static_assert(uint32_t(ARM::kMaxValue) < kMaxFeatures, "The number of ARM CPU features cannot exceed CpuFeatures::kMaxFeatures");

  //! \}

  //! \name Members
  //! \{

  Data _data {};

  //! \}

  //! \name Construction & Destruction
  //! \{

  inline CpuFeatures() noexcept {}
  inline CpuFeatures(const CpuFeatures& other) noexcept = default;
  inline explicit CpuFeatures(Globals::NoInit_) noexcept {}

  //! \}

  //! \name Overloaded Operators
  //! \{

  inline CpuFeatures& operator=(const CpuFeatures& other) noexcept = default;

  inline bool operator==(const CpuFeatures& other) noexcept { return  eq(other); }
  inline bool operator!=(const CpuFeatures& other) noexcept { return !eq(other); }

  //! \}

  //! \name Accessors
  //! \{

  //! Returns true if there are no features set.
  inline bool empty() const noexcept { return _data.empty(); }

  //! Casts this base class into a derived type `T`.
  template<typename T = Data>
  inline T& data() noexcept { return static_cast<T&>(_data); }

  //! Casts this base class into a derived type `T` (const).
  template<typename T = Data>
  inline const T& data() const noexcept { return static_cast<const T&>(_data); }

  //! Returns CpuFeatures::Data as \ref CpuFeatures::X86.
  inline X86& x86() noexcept { return data<X86>(); }
  //! Returns CpuFeatures::Data as \ref CpuFeatures::X86 (const).
  inline const X86& x86() const noexcept { return data<X86>(); }

  //! Returns CpuFeatures::Data as \ref CpuFeatures::ARM.
  inline ARM& arm() noexcept { return data<ARM>(); }
  //! Returns CpuFeatures::Data as \ref CpuFeatures::ARM (const).
  inline const ARM& arm() const noexcept { return data<ARM>(); }

  //! Returns all features as array of bitwords (see \ref Support::BitWord).
  inline BitWord* bits() noexcept { return _data.bits(); }
  //! Returns all features as array of bitwords (const).
  inline const BitWord* bits() const noexcept { return _data.bits(); }
  //! Returns the number of BitWords returned by \ref bits().
  inline size_t bitWordCount() const noexcept { return _data.bitWordCount(); }

  //! Returns \ref Support::BitVectorIterator, that can be used to iterate over all features efficiently.
  inline Iterator iterator() const noexcept { return _data.iterator(); }

  //! Tests whether the feature `featureId` is present.
  template<typename FeatureId>
  inline bool has(const FeatureId& featureId) const noexcept { return _data.has(featureId); }

  //! Tests whether any of the features is present.
  template<typename... Args>
  inline bool hasAny(Args&&... args) const noexcept { return _data.hasAny(std::forward<Args>(args)...); }

  //! Tests whether all features as defined by `other` are present.
  inline bool hasAll(const CpuFeatures& other) const noexcept { return _data.hasAll(other._data); }

  //! \}

  //! \name Manipulation
  //! \{

  inline void reset() noexcept { _data.reset(); }

  //! Adds the given CPU `featureId` to the list of features.
  template<typename... Args>
  inline void add(Args&&... args) noexcept { return _data.add(std::forward<Args>(args)...); }

  //! Adds the given CPU `featureId` to the list of features if `condition` is true.
  template<typename... Args>
  inline void addIf(bool condition, Args&&... args) noexcept { return _data.addIf(condition, std::forward<Args>(args)...); }

  //! Removes the given CPU `featureId` from the list of features.
  template<typename... Args>
  inline void remove(Args&&... args) noexcept { return _data.remove(std::forward<Args>(args)...); }

  //! Tests whether this CPU features matches `other`.
  inline bool eq(const CpuFeatures& other) const noexcept { return _data.eq(other._data); }

  //! \}
};

//! CPU information.
class CpuInfo {
public:
  //! \name Members
  //! \{

  //! Architecture.
  Arch _arch;
  //! Sub-architecture.
  SubArch _subArch;
  //! True if the CPU was detected, false if the detection failed or it's not available.
  bool _wasDetected;
  //! Reserved for future use.
  uint8_t _reserved;
  //! CPU family ID.
  uint32_t _familyId;
  //! CPU model ID.
  uint32_t _modelId;
  //! CPU brand ID.
  uint32_t _brandId;
  //! CPU stepping.
  uint32_t _stepping;
  //! Processor type.
  uint32_t _processorType;
  //! Maximum number of addressable IDs for logical processors.
  uint32_t _maxLogicalProcessors;
  //! Cache line size (in bytes).
  uint32_t _cacheLineSize;
  //! Number of hardware threads.
  uint32_t _hwThreadCount;

  //! CPU vendor string.
  FixedString<16> _vendor;
  //! CPU brand string.
  FixedString<64> _brand;
  //! CPU features.
  CpuFeatures _features;

  //! \}

  //! \name Construction & Destruction
  //! \{

  inline CpuInfo() noexcept { reset(); }
  inline CpuInfo(const CpuInfo& other) noexcept = default;

  inline explicit CpuInfo(Globals::NoInit_) noexcept
    : _features(Globals::NoInit) {};

  //! Returns the host CPU information.
  ASMJIT_API static const CpuInfo& host() noexcept;

  //! Initializes CpuInfo architecture and sub-architecture members to `arch` and `subArch`, respectively.
  inline void initArch(Arch arch, SubArch subArch = SubArch::kUnknown) noexcept {
    _arch = arch;
    _subArch = subArch;
  }

  inline void reset() noexcept { memset(this, 0, sizeof(*this)); }

  //! \}

  //! \name Overloaded Operators
  //! \{

  inline CpuInfo& operator=(const CpuInfo& other) noexcept = default;

  //! \}

  //! \name Accessors
  //! \{

  //! Returns the CPU architecture this information relates to.
  inline Arch arch() const noexcept { return _arch; }

  //! Returns the CPU sub-architecture this information relates to.
  inline SubArch subArch() const noexcept { return _subArch; }

  //! Returns whether the CPU was detected successfully.
  //!
  //! If the returned value is false it means that AsmJit either failed to detect the CPU or it doesn't have
  //! implementation targeting the host architecture and operating system.
  inline bool wasDetected() const noexcept { return _wasDetected; }

  //! Returns the CPU family ID.
  //!
  //! The information provided depends on architecture and OS:
  //!   - X86:
  //!     - Family identifier matches the FamilyId read by using CPUID.
  //!   - ARM:
  //!     - Apple - returns Apple Family identifier returned by sysctlbyname("hw.cpufamily").
  inline uint32_t familyId() const noexcept { return _familyId; }

  //! Returns the CPU model ID.
  //!
  //! The information provided depends on architecture and OS:
  //!   - X86:
  //!     - Model identifier matches the ModelId read by using CPUID.
  inline uint32_t modelId() const noexcept { return _modelId; }

  //! Returns the CPU brand id.
  //!
  //! The information provided depends on architecture and OS:
  //!   - X86:
  //!     - Brand identifier matches the BrandId read by using CPUID.
  inline uint32_t brandId() const noexcept { return _brandId; }

  //! Returns the CPU stepping.
  //!
  //! The information provided depends on architecture and OS:
  //!   - X86:
  //!     - Stepping identifier matches the Stepping information read by using CPUID.
  inline uint32_t stepping() const noexcept { return _stepping; }

  //! Returns the processor type.
  //!
  //! The information provided depends on architecture and OS:
  //!   - X86:
  //!     - Processor type identifier matches the ProcessorType read by using CPUID.
  inline uint32_t processorType() const noexcept { return _processorType; }

  //! Returns the maximum number of logical processors.
  inline uint32_t maxLogicalProcessors() const noexcept { return _maxLogicalProcessors; }

  //! Returns the size of a CPU cache line.
  //!
  //! On a multi-architecture system this should return the smallest cache line of all CPUs.
  inline uint32_t cacheLineSize() const noexcept { return _cacheLineSize; }

  //! Returns number of hardware threads available.
  inline uint32_t hwThreadCount() const noexcept { return _hwThreadCount; }

  //! Returns a CPU vendor string.
  inline const char* vendor() const noexcept { return _vendor.str; }
  //! Tests whether the CPU vendor string is equal to `s`.
  inline bool isVendor(const char* s) const noexcept { return _vendor.eq(s); }

  //! Returns a CPU brand string.
  inline const char* brand() const noexcept { return _brand.str; }

  //! Returns CPU features.
  inline CpuFeatures& features() noexcept { return _features; }
  //! Returns CPU features (const).
  inline const CpuFeatures& features() const noexcept { return _features; }

  //! Tests whether the CPU has the given `feature`.
  template<typename FeatureId>
  inline bool hasFeature(const FeatureId& featureId) const noexcept { return _features.has(featureId); }

  //! Adds the given CPU `featureId` to the list of features.
  template<typename... Args>
  inline void addFeature(Args&&... args) noexcept { return _features.add(std::forward<Args>(args)...); }

  //! Removes the given CPU `featureId` from the list of features.
  template<typename... Args>
  inline void removeFeature(Args&&... args) noexcept { return _features.remove(std::forward<Args>(args)...); }

  //! \}
};

//! \}

ASMJIT_END_NAMESPACE

#endif // ASMJIT_CORE_CPUINFO_H_INCLUDED
