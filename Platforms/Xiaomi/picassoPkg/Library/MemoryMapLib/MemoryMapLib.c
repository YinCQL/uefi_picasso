#include <Library/MemoryMapLib.h>

// Memory Map for Xiaomi Redmi K30 5G (picasso) - Snapdragon 765G (SM7250 / lito)
// Sources:
//   - DDR layout & reserved regions: picasso devicetree /reserved-memory (V13.0.5.0.SGICNXM)
//   - Peripheral bases: picasso devicetree /soc nodes
//   - XBL heap/FD/stack placement: free gap between modem_wlan end (0x9B800000)
//     and cdsp_sec start (0x9F400000), following Project-Silicium conventions

STATIC
EFI_MEMORY_REGION_DESCRIPTOR
gMemoryDescriptor[] = {
  // Name, Address, Length, HobOption, ResourceType, ResourceAttribute, MemoryType, ArmAttribute

  // ---------------- DDR Reserved Regions (from devicetree reserved-memory) ----------------
  {"HYP",               0x80000000, 0x00600000, AddMem, MEM_RES, UNCACHEABLE, Reserv, UNCACHED_UNBUFFERED_XN},
  {"XBL AOP",           0x80600000, 0x00260000, AddMem, MEM_RES, UNCACHEABLE, Reserv, UNCACHED_UNBUFFERED_XN},
  {"MPSS_EFS",          0x80860000, 0x00020000, AddMem, MEM_RES, UNCACHEABLE, Reserv, UNCACHED_UNBUFFERED_XN},
  {"SEC Apps",          0x808FF000, 0x00001000, AddMem, MEM_RES, UNCACHEABLE, Reserv, UNCACHED_UNBUFFERED_XN},
  {"SMEM",              0x80900000, 0x00200000, AddMem, MEM_RES, UNCACHEABLE, Reserv, UNCACHED_UNBUFFERED_XN},
  {"Removed Mem",       0x80B00000, 0x01300000, AddMem, MEM_RES, UNCACHEABLE, Reserv, UNCACHED_UNBUFFERED_XN},
  {"TZApps",            0x81E00000, 0x04200000, AddMem, MEM_RES, UNCACHEABLE, Reserv, UNCACHED_UNBUFFERED_XN},
  {"PIL Camera",        0x86000000, 0x00500000, AddMem, MEM_RES, UNCACHEABLE, Reserv, UNCACHED_UNBUFFERED_XN},
  {"PIL NPU",           0x86500000, 0x00500000, AddMem, MEM_RES, UNCACHEABLE, Reserv, UNCACHED_UNBUFFERED_XN},
  {"PIL Video",         0x86A00000, 0x00500000, AddMem, MEM_RES, UNCACHEABLE, Reserv, UNCACHED_UNBUFFERED_XN},
  {"PIL CDSP",          0x87400000, 0x01E00000, AddMem, MEM_RES, UNCACHEABLE, Reserv, UNCACHED_UNBUFFERED_XN},
  {"PIL ADSP",          0x89200000, 0x02800000, AddMem, MEM_RES, UNCACHEABLE, Reserv, UNCACHED_UNBUFFERED_XN},
  {"PIL WLAN FW",       0x8BA00000, 0x00200000, AddMem, MEM_RES, UNCACHEABLE, Reserv, UNCACHED_UNBUFFERED_XN},
  {"PIL IPA FW",        0x8BC00000, 0x00010000, AddMem, MEM_RES, UNCACHEABLE, Reserv, UNCACHED_UNBUFFERED_XN},
  {"PIL Reserved",      0x8BC10000, 0x003F0000, AddMem, MEM_RES, UNCACHEABLE, Reserv, UNCACHED_UNBUFFERED_XN},
  {"Modem WLAN",        0x8C000000, 0x0F800000, AddMem, MEM_RES, UNCACHEABLE, Reserv, UNCACHED_UNBUFFERED_XN},
  {"CDSP Sec",          0x9F400000, 0x00C00000, AddMem, MEM_RES, UNCACHEABLE, Reserv, UNCACHED_UNBUFFERED_XN},
  {"Display Reserved",  0xA0000000, 0x02300000, AddMem, MEM_RES, SYS_MEM_CAP, Reserv, WRITE_THROUGH_XN},
  {"DFPS Data",         0xA2300000, 0x00100000, AddMem, MEM_RES, UNCACHEABLE, Reserv, UNCACHED_UNBUFFERED_XN},
  {"DISP Rdump",        0xAC000000, 0x01000000, AddMem, MEM_RES, UNCACHEABLE, Reserv, UNCACHED_UNBUFFERED_XN},

  // The Android boot image ramdisk (UniXNU payload) is loaded by the ABL into
  // the last free DRAM gap of bank 1. Nothing covered that gap, so the SEC MMU
  // left it unmapped and any read faulted: FatPkg got a translation fault at
  // 0xB7BFE000 while reading the ramdisk, and the payload driver faulted when it
  // tried to set attributes over the same range. Map the whole gap
  // 0xAD000000-0xBCC00000 (252 MiB, the tail of the 0x80000000+0x3CC00000 bank)
  // as cached system memory so the ramdisk window is reachable wherever the ABL
  // places it.
  {"Boot Ramdisk Gap",  0xAD000000, 0x0F000000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK_XN},
  // ---------------- UEFI Regions ----------------
  // DXE Heap relocated to vendor uefiplat.cfg location (0xA2400000 + 0x04600000 = 70MB).
  // The old 49MB slot at 0x9B800000 was too small: once UFSDxe actually enumerates, the
  // fragmented heap could no longer satisfy TzDxe's 520KB contiguous uncached ShmBridge
  // allocation -> UncachedMemoryAllocationLib.c:389 Out of Resources ASSERT. Vendor places
  // DXE Heap at 0xA2400000 (70MB), a genuinely large free region (0xA2400000-0xAC000000).
  {"DXE Heap",          0xA2400000, 0x04600000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK_XN},
  {"Sched Heap",        0x9E900000, 0x00400000, AddMem, SYS_MEM, SYS_MEM_CAP, BsData, WRITE_BACK_XN},
  {"FD Reserved",       0x9F000000, 0x00100000, AddMem, SYS_MEM, SYS_MEM_CAP, BsData, WRITE_BACK},
  {"UEFI FD",           0x9F100000, 0x00200000, AddMem, SYS_MEM, SYS_MEM_CAP, BsData, WRITE_BACK},
  {"SEC Heap",          0x9F300000, 0x0008C000, AddMem, SYS_MEM, SYS_MEM_CAP, BsData, WRITE_BACK_XN},
  {"MMU PageTables",    0x9F38D000, 0x00003000, AddMem, SYS_MEM, SYS_MEM_CAP, BsData, WRITE_BACK_XN},
  {"UEFI Stack",        0x9F390000, 0x00040000, AddMem, SYS_MEM, SYS_MEM_CAP, BsData, WRITE_BACK_XN},
  {"Log Buffer",        0x9F3F7000, 0x00008000, AddMem, SYS_MEM, SYS_MEM_CAP, RtData, WRITE_BACK_XN},
  {"Info Blk",          0x9F3FF000, 0x00001000, AddMem, SYS_MEM, SYS_MEM_CAP, RtData, WRITE_BACK_XN},

  // ---------------- Other Memory Regions ----------------
  {"AOP_SS_MSG_RAM",    0x0C300000, 0x00100000, NoHob,  MMAP_IO, INITIALIZED, Conv,  NS_DEVICE},
  {"IMEM Base",         0x14680000, 0x0002B000, NoHob,  MMAP_IO, INITIALIZED, Conv,  NS_DEVICE},
  {"IMEM Cookie Base",  0x146AB000, 0x00001000, AddDev, MMAP_IO, INITIALIZED, Conv,  NS_DEVICE},

  // ---------------- Register Regions (verified against picasso devicetree /soc) ----------------
  {"GCC CLK CTL",       0x00100000, 0x00200000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},  // qcom,gcc@100000 +0x1f0000
  {"SECURITY CONTROL",  0x00780000, 0x00007000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},  // qfprom@780000
  {"PRNG_CFG_PRNG",     0x00793000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},  // qrng@793000
  {"SDC1_REG",          0x007C4000, 0x00020000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},  // sdhci@7c4000 (eMMC/SD)
  {"QUPV3_0_GSI",       0x00800000, 0x00100000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},  // gpi-dma@800000 + qupv3_0
  {"QUPV3_1_GSI",       0x00900000, 0x00100000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},  // gpi-dma@900000 + qupv3_1
  {"UFS UFS REGS",      0x01D84000, 0x00020000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},  // ufshc@1d84000
  {"CRYPTO0 CRYPTO",    0x01DC0000, 0x00040000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},  // qcrypto@1de0000
  {"TCSR_TCSR_REGS",    0x01FC0000, 0x00040000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},  // tcsr (same family addr)
  {"TLMM",              0x0F000000, 0x01000000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},  // pinctrl@f000000
  {"GPU_CC",            0x03D90000, 0x00009000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},  // qcom,gpucc@3d90000
  {"GPU_GMU",           0x03D6A000, 0x00030000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},  // qcom,gmu@3d6a000
  {"RPMH_CPRF",         0x0B220000, 0x00030000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},  // interrupt-controller@b220000 (rpmh)
  {"QUPV3_SSC_GSI",     0x62B00000, 0x00060000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},  // sensor SSC (not in dt, keep family value)
  {"PERIPH_SS",         0x08800000, 0x00200000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},  // USB HS/SS PHYs @0x88e2000-0x88eb000
  {"MCCC_MCCC_MSTR",    0x090B0000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},  // syscon@90b0000 region
  {"USB30_PRIM",        0x0A600000, 0x00100000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},  // ssusb@a600000
  {"USB_RUMI",          0x0A720000, 0x00010000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},  // keep family value (not in dt)
  {"PDC_DISPLAY",       0x0B2A0000, 0x00010000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},  // keep family value
  {"PDC_DISP_SEQ",      0x0B4A0000, 0x00010000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},  // keep family value
  {"RPMH_BCM_BCM_TOP",  0x0BA00000, 0x00200000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},  // keep family value
  {"SLP_CNTR_TSENS",    0x0C221000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},  // qcom,mpm2-sleep-counter@0xc221000
  {"TSENS SROT",        0x0C222000, 0x00002000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},  // tsens@c222000 + tsens@c223000 SROT (both sensors)
  {"TSENS TM0",         0x0C263000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},  // tsens@c222000 TM (ends before restart@0x264000)
  {"PSHOLD Reset",      0x0C264000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},  // restart@c264000 (watchdog/pshold, accessed by panic handlers)
  {"TSENS TM1",         0x0C265000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},  // tsens@c223000 TM
  {"PMIC ARB SPMI",     0x0C400000, 0x02800000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},  // qcom,spmi@c440000 parent base
  {"VIDEO_CC",          0x0AB00000, 0x00010000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},  // qcom,videocc@ab00000
  {"NPU_CC",            0x09980000, 0x00010000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},  // qcom,npucc@9980000
  {"TITAN_CAM_CC",      0x0AD00000, 0x00010000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},  // qcom,camcc@ad00000
  {"DISP_CC",           0x0AF00000, 0x00020000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},  // qcom,dispcc@af00000
  {"SILVER_CLK_CTL",    0x18280000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},  // near rsc@18200000 family
  {"GOLD_CLK_CTL",      0x18282000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"L3_CLK_CTL",        0x18284000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"APSS_RSC_RSCCR",    0x18200000, 0x00030000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},  // rsc@18200000
  {"APSS_GIC500_GICD",  0x17A00000, 0x00010000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},  // interrupt-controller@17a00000
  {"APSS_GIC500_GICR",  0x17A60000, 0x00100000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"QTIMER",            0x17C00000, 0x00110000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},  // covers wdt@17c10000 + timer@17c20000
  {"APSS_ACTPM_WRAP",   0x18300000, 0x000B0000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},  // cpufreq/lmh region
  {"MDSS",              0x0AE00000, 0x00134000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},  // qcom,mdss_mdp (spans 0xae00000-0xaf14000)
  {"SMMU",              0x15000000, 0x00100000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},  // apps-smmu@15000000 (devicetree reg 0)
  {"SMMU_TBU",          0x15182000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE}   // apps-smmu@15000000 (devicetree reg 1, 0x20 -> page aligned)
};

VOID
GetMemoryMap (
  OUT EFI_MEMORY_REGION_DESCRIPTOR **MemoryDescriptor,
  OUT UINT8                         *MemoryDescriptorCount)
{
  // Pass Data
  *MemoryDescriptor      = gMemoryDescriptor;
  *MemoryDescriptorCount = ARRAY_SIZE (gMemoryDescriptor);
}
