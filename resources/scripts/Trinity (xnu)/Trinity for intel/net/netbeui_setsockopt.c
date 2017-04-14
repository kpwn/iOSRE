#include "net.h"

#define SOL_NETBEUI 267

void netbeui_setsockopt(struct sockopt *so)
{
	so->level = SOL_NETBEUI;
}
