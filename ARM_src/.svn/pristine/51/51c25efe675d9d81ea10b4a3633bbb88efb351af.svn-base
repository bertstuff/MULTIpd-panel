 /**
 * \addtogroup Peripherals
 * @{
 */

/**
 * \defgroup Net_Dev_Init
 *
 * Net init for tcp/ip connections
 *
 * @{
 */

/**
 * \file
 *       This file contains functions to initialize the tcp/ip connection
 * \author
 *         Mark van der Heijden <m.vanderheijden@custommate.eu>
 *
 */
#ifndef NET_DEV_INIT
#define NET_DEV_INIT

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <Core/protocol/FAT/ff.h>
#include <Core/protocol/Net/uip_arp.h>
#include <Core/protocol/Net/uip.h>
#include <Core/protocol/Net/emac/emac-driver.h>
#include <Core/protocol/Net/dhcp/dhcpc.h>
#include <Core/protocol/Net/dns/resolv.h>

typedef struct {
  //Ethernet
  bool Offline;
  bool dhcp;
  uip_ipaddr_t IP_address;
  uip_ipaddr_t NetMask;
  uip_ipaddr_t Def_Gateway;
  uip_ipaddr_t DNS_Address;
  uip_eth_addr MAC_Address;

  //ini
  bool setup_done;

} Net_Config_t;

Net_Config_t Net_settings;

void Net_Dev_init();

bool ini_getipaddr(const TCHAR *Section, const TCHAR *Key, const TCHAR *DefValue, const TCHAR *Description,
		uip_ipaddr_t *ip_addr_ptr, const TCHAR *Filename);

void ini_resolv_ipaddr(void ** Resolv_handel, const TCHAR *Section, const TCHAR *Key, const TCHAR *DefValue, const TCHAR *Description, const TCHAR *Filename);
bool get_resolv_ipaddr(void ** Resolv_handel, uip_ipaddr_t *ip_addr_ptr, process_data_t data);

int ini_getmacaddr(const TCHAR *Section, const TCHAR *Key, const TCHAR *DefValue, const TCHAR *Description, uip_eth_addr * ethaddr_ptr, const TCHAR *Filename);

#endif
 /**
 * 	@}
 * @}
 **/
