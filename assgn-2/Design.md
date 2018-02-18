# Design Document for Lottery Scheduling



## Modified Data Structure

- struct runq
- struct 


### runq

We add sereral fileds to `struct runq`. 

| Added Field  | Type        | Usage                                               |
| ------------ | ----------- | --------------------------------------------------- |
| rq_tickets   | u_long      | Hold the total lottery ticket of this queue.        |
| rq_rnd_pool  | u_long[256] | Hold the many random numbers as a pool.             |
| rq_rnd_piov  | int         | Indicate the current random number index            |
| rq_rnd_dirty | char        | Indicate if the pool requires update (regeneration) |

The random is managed in `runq`. Each time it generates a random number for lottery, we will pick up a number under the `rq_rnd_piov` from `rq_rnd_pool` and mark the pool dirty by `rq_rnd_dirty`.

The dirty pool will be updated when new thread comes in or thread goes out. Notice that only the index before



