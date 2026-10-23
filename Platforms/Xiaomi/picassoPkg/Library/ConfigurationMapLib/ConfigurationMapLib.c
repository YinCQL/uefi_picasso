#include <Library/ConfigurationMapLib.h>

// Configuration Map for Xiaomi Redmi K30 5G (picasso) - Snapdragon 765G (SM7250 / lito)
// Sources:
//   - SharedIMEMBaseAddr / DloadCookieAddr: picasso devicetree (msm-imem@146ab000, restart second reg 0x1fd3000)
//   - TzAppsRegnAddr/Size: picasso devicetree qtee_apps_region@81e00000 (size 0x4200000)
//   - Remaining entries: Qualcomm platform-common values (same as SM7125/Rennell family),
//     reviewed against miatoll (SM7125) known-good configuration

STATIC
EFI_CONFIGURATION_ENTRY_DESCRIPTOR
gConfigurationDescriptor[] = {
  // Configuration Map
  {"NumCpusFuseAddr", 0x5C04C},
  {"EnableShell", 0x1},
  {"SharedIMEMBaseAddr", 0x146AB000},
  {"DloadCookieAddr", 0x01FD3000},
  {"DloadCookieValue", 0x10},
  {"NumCpus", 4},
  {"NumActiveCores", 8},
  {"MaxLogFileSize", 0x400000},
  {"UefiMemUseThreshold", 0xC8},
  {"ShmBridgememSize", 0xA00000},
  {"USBHS1_Config", 0x0},
  {"UsbFnIoRevNum", 0x00010001},
  {"PwrBtnShutdownFlag", 0x0},
  {"Sdc1GpioConfigOn", 0x1E92},
  {"Sdc2GpioConfigOn", 0x1E92},
  {"Sdc1GpioConfigOff", 0xA00},
  {"Sdc2GpioConfigOff", 0xA00},
  {"EnableSDHCSwitch", 0x1},
  {"EnableUfsIOC", 0},
  {"UfsSmmuConfigForOtherBootDev", 1},
  {"SecurityFlag", 0xC4},
  {"TzAppsRegnAddr", 0x81E00000},
  {"TzAppsRegnSize", 0x04200000},
  {"EnableLogFsSyncInRetail", 0x0},
  {"DetectRetailUserAttentionHotkey", 0x00},
  {"DetectRetailUserAttentionHotkeyCode", 0x17},
  {"EnableOEMSetupAppInRetail", 0x0},
  {"twoabl", 2},
  {"EnableMultiThreading", 1},
  {"MaxCoreCnt", 8},
  {"EarlyInitCoreCnt", 1},
  {"EnableDisplayThread", 0},
  {"AllowNonPersistentVarsInRetail", 0x1},
  {"EnableUefiSecAppDebugLogDump", 0x0},
  {"DDRInfoNotifyFlag", 0x1}
};

VOID
GetConfigurationMap (
  OUT EFI_CONFIGURATION_ENTRY_DESCRIPTOR **ConfigurationDescriptor,
  OUT UINT8                               *ConfigurationDescriptorCount)
{
  // Pass Data
  *ConfigurationDescriptor      = gConfigurationDescriptor;
  *ConfigurationDescriptorCount = ARRAY_SIZE (gConfigurationDescriptor);
}
