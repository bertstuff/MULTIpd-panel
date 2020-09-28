#include "array.h"
#include <Core/Debug/Debug_Terminal.h>

int explode(char ***arr_ptr, char *str, char delimiter)
{
  char *src = str, *end, *dst;
  char **arr;
  int size = 1, i;

  // Find number of strings
  while ((end = strchr(src, delimiter)) != NULL)
    {
      ++size;
      src = end + 1;
    }

  arr = malloc(size * sizeof(char *) + (strlen(str) + 1) * sizeof(char));

  if(arr == NULL){
	  printf("Memory allocation failed(Heap full)\r\n");
  }

  src = str;
  dst = (char *) arr + size * sizeof(char *);
  for (i = 0; i < size; ++i)
    {
      if ((end = strchr(src, delimiter)) == NULL)
        end = src + strlen(src);
      arr[i] = dst;
      strncpy(dst, src, end - src);
      dst[end - src] = '\0';
      dst += end - src + 1;
      src = end + 1;
    }
  *arr_ptr = arr;

  return size;
}
