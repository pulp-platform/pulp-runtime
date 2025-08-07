/*
 * Copyright (C) 2019 ETH Zurich, University of Bologna
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __POS_IMPLEM_ALLOC_POOL_H__
#define __POS_IMPLEM_ALLOC_POOL_H__

void pos_allocs_init();

void alloc_init_l1(int cid);

/*
 * pos_alloc_user_data and pos_free_user_data will allocate memory in the best
 * location based on the available hardware. The preferred memories are (in
 * order): FC_TCDM > L2_private_bank_0 > L2_private_bank_1 > L2_share_banks
 */
void *pos_alloc_user_data(int size);

void pos_free_user_data(void *_chunk, int size);

#endif
