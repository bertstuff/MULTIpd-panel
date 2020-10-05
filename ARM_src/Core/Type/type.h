#ifndef __TYPE_H__
#define __TYPE_H__

#include <Driver/include/lpc_types.h>

typedef enum{
	NL,
	EN,
	DE,
	FR,
	NOT_SET
}Language_t;

#define LANG_CNT 4


typedef unsigned int clock_time_t;
typedef unsigned int uip_stats_t;

typedef struct{
  uint16_t x;
  uint16_t y;
}Point_t;

typedef enum{
	EBM_BASIS = 0,
	EBM_DOUCHE = 1
}EBM_type_t;

typedef enum{
  MODE_DEFAULT = 0,
  MODE_FREE = 1,
  MODE_FREE_WITH_ID = 2,
  MODE_CLOSE = 3,
  MODE_OPEN = 4
}Mode_t;

#define AUTH_NO 0
#define AUTH_ADMIN 1
#define AUTH_SERVICE 2
#define AUTH_MANUFACTURE 4
#define AUTH_DEBUG 8

#endif  /* __TYPE_H__ */
