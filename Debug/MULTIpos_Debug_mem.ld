/*
 * GENERATED FILE - DO NOT EDIT
 * (c) Code Red Technologies Ltd, 2008-2020
 * (c) NXP Semiconductors 2013-2020
 * Linker script memory definitions
 * Created from LinkMemoryTemplate
 * By LPCXpresso v7.4.0 [Build 229] [2014-09-16]  on Tue Sep 22 16:19:50 CEST 2020)
*/

MEMORY
{
  /* Define each memory region */
  MFlash256 (rx) : ORIGIN = 0x4000, LENGTH = 0x3c000 /* 240K bytes */
  RamLoc32 (rwx) : ORIGIN = 0x10000000, LENGTH = 0x8000 /* 32K bytes */
  RamAHB32 (rwx) : ORIGIN = 0x2007c000, LENGTH = 0x8000 /* 32K bytes */


}
  /* Define a symbol for the top of each memory region */
  __top_MFlash256 = 0x4000 + 0x3c000;
  __top_RamLoc32 = 0x10000000 + 0x8000;
  __top_RamAHB32 = 0x2007c000 + 0x8000;



