// $Id$

#include "ProxyConsumer.h"
#include "Worker_Task.h"

#include "Event_Manager.h"
#include "Method_Request_Lookup.h"

ACE_INLINE int 
TAO_NS_ProxyConsumer::is_connected (void)
{
  return supplier_ == 0 ? 0 : 1;
}
