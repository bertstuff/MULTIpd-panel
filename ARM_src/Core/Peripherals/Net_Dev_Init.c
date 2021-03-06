#include "Net_Dev_Init.h"
#include <Core/Process/Process.h>
#include <Core/Timer/etimer.h>
#include <Core/Debug/Debug_Terminal.h>
#include <Global.h>
#include <Core/Extensions/ini/minGlue-FatFs.h>
#include <Core/Extensions/ini/minIni.h>
#include <Core/Util/array.h>
#include <Core/Protocol/Net/dhcp/dhcp.h>
#include <Core/Protocol/Net/dns/resolv.h>
#include <Core/Protocol/Net/uiplib.h>
#include <Core/Memory/IAP.h>


const char inifile_com[] = "SD1:/NET.cfg";

bool ini_getipaddr(const TCHAR *Section, const TCHAR *Key, const TCHAR *DefValue, const TCHAR *Description,
	uip_ipaddr_t *ip_addr_ptr, const TCHAR *Filename){
	char str[50];

	ini_gets(Section ,Key , DefValue, Description, str, sizeof(str), Filename);
	if(uiplib_ipaddrconv(str,ip_addr_ptr) != 0){
		return true;
	}
	return false;
}

void ini_resolv_ipaddr(void ** Resolv_handel, const TCHAR *Section, const TCHAR *Key, const TCHAR *DefValue, const TCHAR *Description, const TCHAR *Filename){
	char str[50];
	uip_ipaddr_t ip_addr_resolv_ptr;

	ini_gets(Section ,Key , DefValue, Description, str, sizeof(str), Filename);
	if(*Resolv_handel != NULL){
		free(*Resolv_handel);
	}
	*Resolv_handel = malloc(sizeof(str));
	memmove (*Resolv_handel,str, sizeof(str));

	if(uiplib_ipaddrconv(str,&ip_addr_resolv_ptr) != 0){
		process_post(PROCESS_BROADCAST, resolv_event_found, * Resolv_handel);
		return;
	}

	resolv_query(str);
	return;
}

bool get_resolv_ipaddr(void ** Resolv_handel, uip_ipaddr_t *ip_addr_ptr, process_data_t data){
	resolv_status_t dns_status;
	uip_ipaddr_t *ip_addr_resolv_ptr;

	if(strcmp (data,*Resolv_handel) != 0){
		return false;
	}

	if(uiplib_ipaddrconv(*Resolv_handel,ip_addr_ptr) != 0){
		free(*Resolv_handel);
		return true;
	}

	dns_status = resolv_lookup(*Resolv_handel, &ip_addr_resolv_ptr);
	if(dns_status == RESOLV_STATUS_CACHED){
		uip_ipaddr_copy(ip_addr_ptr, ip_addr_resolv_ptr);
		free(*Resolv_handel);
		return true;
	}

	return false;
}

int ini_getmacaddr(const TCHAR *Section, const TCHAR *Key, const TCHAR *DefValue, const TCHAR *Description,
		uip_eth_addr * ethaddr_ptr, const TCHAR *Filename){
	  char **arr;
	  int size, i;
	  char str[50];


	  ini_gets(Section ,Key , DefValue, Description, str, sizeof(str), Filename);
	  size = explode(&arr, str, ':');
	  for (i = 0; (i < size)&&(i < 6); ++i){
		  ethaddr_ptr->addr[i] = (uint8_t)strtol(arr[i], NULL, 16);
	  }
	  free(arr);
	return size;
}


bool Config_ini_load(void){
  unsigned  Serial_Number[5];
  uint8_t mac[6];
  char mac_str[20];
  uint8_t i;

  //Ethernet
  Net_settings.Offline = ini_getbool("Ethernet","Offline", false,"1:Offline 0:Online", inifile_com);
  if(Net_settings.Offline == true){
	  Net_settings.setup_done = true;
	  return true;
  }

  Net_settings.dhcp = ini_getbool("Ethernet","DHCP", true,"1:DHCP on 0:DHCP off", inifile_com);
  if( Net_settings.dhcp == false){
	  ini_getipaddr("Ethernet", "IP_address", "172.16.0.200", "ip address:", &Net_settings.IP_address, inifile_com);
	  ini_getipaddr("Ethernet", "Netmask", "255.255.255.0", "Netmask:", &Net_settings.NetMask, inifile_com);
	  ini_getipaddr("Ethernet", "Def_Gateway", "172.16.0.254", "Gatway to connect:",&Net_settings.Def_Gateway, inifile_com);
	  ini_getipaddr("Ethernet", "DNS_address", "172.16.0.254", "Address of DNS server:", &Net_settings.DNS_Address, inifile_com);
  }

  //Generate mac number from chip uid
  read_serial_number(Serial_Number);
  //FNV-1a hash to try to get a short unique mac address
  uint64_t hash = 0xcbf29ce484222325;
  for(i = 0; i < 5; i++){
 	 hash ^= 0x100000001b3;
 	 hash *= Serial_Number[i];
  }
  for(i = 0;i < 6;i++){
	  mac[i] = ((hash >> (i*8)) & 0xff);
  }
  mac[0] = 0x02 | (mac[0] & 0xFC);//local mac
  sprintf(mac_str,"%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3],mac[4], mac[5]);
  ini_getmacaddr("Ethernet", "MAC_address", mac_str,NULL, &Net_settings.MAC_Address, inifile_com);

  if(Net_settings.dhcp == false){
	  Net_settings.setup_done = true;
  }else{
	  Net_settings.setup_done = false;
  }
  return (Net_settings.setup_done);
}


void Net_Dev_init(){


	Config_ini_load();

	if(Net_settings.Offline == true){
		return;
	}

	// init MAC address
	uip_setethaddr(Net_settings.MAC_Address);
	process_start(&etimer_process, NULL);
	printf("\nStarting EMAC service\n");
	process_start(&emac_lpc1768, NULL);
	printf("Starting TCP/IP service\n");
	process_start(&tcpip_process, NULL);// invokes uip_init();


	if(Net_settings.dhcp){
		printf("DHCP: started\r\n");
		process_start(&dhcp_process, NULL);
	}else{
		printf("IP Address:  %d.%d.%d.%d\n", uip_ipaddr_to_quad(&Net_settings.IP_address));
		uip_sethostaddr(&Net_settings.IP_address);
		printf("Subnet Mask: %d.%d.%d.%d\n", uip_ipaddr_to_quad(&Net_settings.NetMask));
		uip_setnetmask(&Net_settings.NetMask);
		printf("Def. Router: %d.%d.%d.%d\n", uip_ipaddr_to_quad(&Net_settings.Def_Gateway));
		uip_setdraddr(&Net_settings.Def_Gateway);
		printf("DNS Address: %d.%d.%d.%d\n", uip_ipaddr_to_quad(&Net_settings.DNS_Address));
		resolv_conf(&Net_settings.DNS_Address);
	}
	process_start(&resolv_process, NULL);
	return;
}


