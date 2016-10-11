#ifndef CEPH_CRUSH_MAPPER_H
#define CEPH_CRUSH_MAPPER_H

/*
 * CRUSH functions for find rules and then mapping an input to an
 * output set.
 *
 * LGPL2
 */

#include "crush.h"

<<<<<<< HEAD
extern int crush_find_rule(struct crush_map *map, int pool, int type, int size);
extern int crush_do_rule(struct crush_map *map,
			 int ruleno,
			 int x, int *result, int result_max,
			 int forcefeed,    /* -1 for none */
			 __u32 *weights);
=======
extern int crush_find_rule(const struct crush_map *map, int ruleset, int type, int size);
extern int crush_do_rule(const struct crush_map *map,
			 int ruleno,
			 int x, int *result, int result_max,
			 int forcefeed,    /* -1 for none */
			 const __u32 *weights);
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4

#endif
