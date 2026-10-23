##
#  Copyright (c) 2011 - 2022, ARM Limited. All rights reserved.
#  Copyright (c) 2014, Linaro Limited. All rights reserved.
#  Copyright (c) 2015 - 2020, Intel Corporation. All rights reserved.
#  Copyright (c) 2018, Bingxing Wang. All rights reserved.
#  Copyright (c) Microsoft Corporation.
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
##

################################################################################
#
# Defines Section - statements that will be processed to create a Makefile.
#
################################################################################
[Defines]
  PLATFORM_NAME                  = picasso
  PLATFORM_GUID                  = 7C1E5A2B-3D4F-4E5A-9B0C-1D2E3F4A5B6C
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/picassoPkg
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = RELEASE|DEBUG
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = picassoPkg/picasso.fdf
  USE_CUSTOM_DISPLAY_DRIVER      = 0

!include LitoPkg/LitoPkg.dsc.inc

[PcdsFixedAtBuild]
  #
  # DDR Memory
  #
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x80000000

  #
  # UEFI Stack
  #
  gArmPlatformTokenSpaceGuid.PcdCPUCoresStackBase|0x9F390000
  gArmPlatformTokenSpaceGuid.PcdCPUCorePrimaryStackSize|0x40000

  #
  # SMBIOS
  #
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Snapdragon (TM) 765G @ 2.40 GHz"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorPartNumber|"SM7250"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemManufacturer|"Xiaomi"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"Redmi K30 5G"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"picasso"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"M2001G7AE"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemBoardModel|"G7AE"

  #
  # Simple Frame Buffer
  #
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferWidth|1080
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferHeight|2400
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferColorDepth|32

  #
  # Platform PEI
  #
  gQcomPkgTokenSpaceGuid.PcdPlatformType|"LA"

[LibraryClasses]
  #
  # Memory Libraries
  #
  MemoryMapLib|picassoPkg/Library/MemoryMapLib/MemoryMapLib.inf

  #
  # QCOM Libraries
  #
  ConfigurationMapLib|picassoPkg/Library/ConfigurationMapLib/ConfigurationMapLib.inf

  #
  # UniXNU Libraries
  #
  UniXNULib|UniXNUPkg/Library/UniXNULib/UniXNULib.inf
  FdtLib|MdePkg/Library/BaseFdtLib/BaseFdtLib.inf

[Components]
  #
  # ACPI Tables
  #
  picasso/AcpiTables.inf

  # UniXNU
  # Initrd block device: publishes /chosen linux,initrd-start/-end as a volume
  picassoPkg/Drivers/PicassoBootMemoryDxe/PicassoBootMemoryDxe.inf

  UniXNUPkg/Application/UniXNU/UniXNU.inf
  UniXNUPkg/Driver/UniXNUDxe/UniXNUDxe.inf
  DarwinBootPkg/Driver/DarwinBoot.inf
