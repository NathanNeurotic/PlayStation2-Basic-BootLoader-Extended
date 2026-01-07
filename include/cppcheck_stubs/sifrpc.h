#pragma once
// Stub for Codacy/Cppcheck static analysis; not used in PS2SDK builds.

#include "tamtypes.h"

typedef struct SifRpcClientData {
    int dummy;
} SifRpcClientData_t;

int SifCallRpc(SifRpcClientData_t *client, int rpc_number, int mode, void *send, int ssize, void *recv, int rsize,
               void *endfunc, void *endparam);
int SifInitRpc(int mode);
