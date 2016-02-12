#include "net.h"

#define SOL_NFC 280

void nfc_setsockopt(struct sockopt *so)
{
	so->level = SOL_NFC;
}
