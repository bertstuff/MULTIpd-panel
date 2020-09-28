#include <stdio.h>

#include <Core/Peripherals/Net_Dev_Init.h>
#include <Core/protocol/Net/uip.h>
#include "dhcp.h"
#include "dhcpc.h"

PROCESS(dhcp_process, "DHCP");
struct etimer dhcp_timout;

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(dhcp_process, ev, data)
{
  PROCESS_BEGIN();
  dhcpc_init(uip_lladdr.addr, sizeof(uip_lladdr.addr));
  dhcpc_request();
  etimer_set(&dhcp_timout, CLOCK_SECOND * 15);
  while(1) {
    PROCESS_WAIT_EVENT();
	if((ev == tcpip_event) || (ev == PROCESS_EVENT_TIMER)) {
      dhcpc_appcall(ev, data);
    }
	if(ev == PROCESS_EVENT_TIMER){
		if(etimer_expired(&dhcp_timout)){
			if(Net_settings.setup_done == false){
				printf("DHCP: Timeout retry\r\n");
				dhcpc_init(uip_lladdr.addr, sizeof(uip_lladdr.addr));
				dhcpc_request();
				etimer_set(&dhcp_timout, CLOCK_SECOND * 15);
			}
		}
	}
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
void dhcpc_configured(const struct dhcpc_state *s){
	printf("DHCP: Done\r\n");
	uip_ipaddr_copy(&Net_settings.IP_address,&s->ipaddr);
	uip_ipaddr_copy(&Net_settings.NetMask,&s->netmask);
	uip_ipaddr_copy(&Net_settings.Def_Gateway,&s->default_router);
	uip_ipaddr_copy(&Net_settings.DNS_Address,&s->dnsaddr);

	printf("IP Address:  %d.%d.%d.%d\n", uip_ipaddr_to_quad(&Net_settings.IP_address));
	uip_sethostaddr(&Net_settings.IP_address);
	printf("Subnet Mask: %d.%d.%d.%d\n", uip_ipaddr_to_quad(&Net_settings.NetMask));
	uip_setnetmask(&Net_settings.NetMask);
	printf("Def. Router: %d.%d.%d.%d\n", uip_ipaddr_to_quad(&Net_settings.Def_Gateway));
	uip_setdraddr(&Net_settings.Def_Gateway);
	printf("DNS Address: %d.%d.%d.%d\n", uip_ipaddr_to_quad(&Net_settings.DNS_Address));
	resolv_conf(&Net_settings.DNS_Address);
	Net_settings.setup_done = true;
}

void dhcpc_unconfigured(const struct dhcpc_state *s){
	printf("DHCP: failed\r\n");
	dhcpc_init(&uip_lladdr,sizeof(uip_lladdr));
}
