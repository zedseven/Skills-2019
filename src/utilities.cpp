#include "main.h"
#include <sys/errno.h>
//#include <string>

std::string readableErrno()
{
  switch(errno)
  {
    case EACCES: //Another resource is currently trying to access the port.
      return "EACCES";
    case EINVAL: //The given value is not within a range specific to the device.
      return "EINVAL";
    case ENXIO: //The LCD has not been initialized. Call lcd_initialize first.
      return "ENXIO";
    case EDOM: //size_id is greater than the number of available objects.
      return "EDOM";
    case EHOSTDOWN: //Reading the vision sensor failed for an unknown reason.
      return "EHOSTDOWN";
    default:
      return "UNKNOWN (" + std::to_string(errno) + ")";
  }
}
