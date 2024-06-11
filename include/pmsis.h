#ifndef __FAKE_PMSIS_H__
#define __FAKE_PMSIS_H__

// simply include the runtime equivalent of pmsis.h: pulp.h
#include "pulp.h"
#define pi_core_id get_core_id
#define pi_cl_team_barrier(x) synch_barrier()

#endif
