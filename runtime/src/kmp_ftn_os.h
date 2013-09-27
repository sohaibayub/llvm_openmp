/*
 * kmp_ftn_os.h -- KPTS Fortran defines header file.
 * $Revision: 42478 $
 * $Date: 2013-07-02 15:15:08 -0500 (Tue, 02 Jul 2013) $
 */


//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.txt for details.
//
//===----------------------------------------------------------------------===//


#ifndef KMP_FTN_OS_H
#define KMP_FTN_OS_H

// KMP_FNT_ENTRIES may be one of: KMP_FTN_PLAIN, KMP_FTN_UPPER, KMP_FTN_APPEND, KMP_FTN_UAPPEND.


/* -------------------------- External definitions ------------------------ */

#if KMP_FTN_ENTRIES == KMP_FTN_PLAIN

    #define FTN_SET_STACKSIZE                    kmp_set_stacksize
    #define FTN_SET_STACKSIZE_S                  kmp_set_stacksize_s
    #define FTN_GET_STACKSIZE                    kmp_get_stacksize
    #define FTN_GET_STACKSIZE_S                  kmp_get_stacksize_s
    #define FTN_SET_BLOCKTIME                    kmp_set_blocktime
    #define FTN_GET_BLOCKTIME                    kmp_get_blocktime
    #define FTN_SET_LIBRARY_SERIAL               kmp_set_library_serial
    #define FTN_SET_LIBRARY_TURNAROUND           kmp_set_library_turnaround
    #define FTN_SET_LIBRARY_THROUGHPUT           kmp_set_library_throughput
    #define FTN_SET_LIBRARY                      kmp_set_library
    #define FTN_GET_LIBRARY                      kmp_get_library
    #define FTN_SET_DEFAULTS                     kmp_set_defaults
    #define FTN_SET_AFFINITY                     kmp_set_affinity
    #define FTN_GET_AFFINITY                     kmp_get_affinity
    #define FTN_GET_AFFINITY_MAX_PROC            kmp_get_affinity_max_proc
    #define FTN_CREATE_AFFINITY_MASK             kmp_create_affinity_mask
    #define FTN_DESTROY_AFFINITY_MASK            kmp_destroy_affinity_mask
    #define FTN_SET_AFFINITY_MASK_PROC           kmp_set_affinity_mask_proc
    #define FTN_UNSET_AFFINITY_MASK_PROC         kmp_unset_affinity_mask_proc
    #define FTN_GET_AFFINITY_MASK_PROC           kmp_get_affinity_mask_proc

    #define FTN_MALLOC                           kmp_malloc
    #define FTN_CALLOC                           kmp_calloc
    #define FTN_REALLOC                          kmp_realloc
    #define FTN_FREE                             kmp_free

    #define FTN_GET_NUM_KNOWN_THREADS            kmp_get_num_known_threads

    #define FTN_SET_NUM_THREADS                  omp_set_num_threads
    #define FTN_GET_NUM_THREADS                  omp_get_num_threads
    #define FTN_GET_MAX_THREADS                  omp_get_max_threads
    #define FTN_GET_THREAD_NUM                   omp_get_thread_num
    #define FTN_GET_NUM_PROCS                    omp_get_num_procs
    #define FTN_SET_DYNAMIC                      omp_set_dynamic
    #define FTN_GET_DYNAMIC                      omp_get_dynamic
    #define FTN_SET_NESTED                       omp_set_nested
    #define FTN_GET_NESTED                       omp_get_nested
    #define FTN_IN_PARALLEL                      omp_in_parallel
    #define FTN_GET_THREAD_LIMIT                 omp_get_thread_limit
    #define FTN_SET_SCHEDULE                     omp_set_schedule
    #define FTN_GET_SCHEDULE                     omp_get_schedule
    #define FTN_SET_MAX_ACTIVE_LEVELS            omp_set_max_active_levels
    #define FTN_GET_MAX_ACTIVE_LEVELS            omp_get_max_active_levels
    #define FTN_GET_ACTIVE_LEVEL                 omp_get_active_level
    #define FTN_GET_LEVEL                        omp_get_level
    #define FTN_GET_ANCESTOR_THREAD_NUM          omp_get_ancestor_thread_num
    #define FTN_GET_TEAM_SIZE                    omp_get_team_size
    #define FTN_IN_FINAL                         omp_in_final
//  #define FTN_SET_PROC_BIND                    omp_set_proc_bind
    #define FTN_GET_PROC_BIND                    omp_get_proc_bind
//  #define FTN_CURR_PROC_BIND                   omp_curr_proc_bind
#if OMP_40_ENABLED
    #define FTN_GET_NUM_TEAMS                    omp_get_num_teams
    #define FTN_GET_TEAM_NUM                     omp_get_team_num
#endif
    #define FTN_INIT_LOCK                        omp_init_lock
    #define FTN_DESTROY_LOCK                     omp_destroy_lock
    #define FTN_SET_LOCK                         omp_set_lock
    #define FTN_UNSET_LOCK                       omp_unset_lock
    #define FTN_TEST_LOCK                        omp_test_lock
    #define FTN_INIT_NEST_LOCK                   omp_init_nest_lock
    #define FTN_DESTROY_NEST_LOCK                omp_destroy_nest_lock
    #define FTN_SET_NEST_LOCK                    omp_set_nest_lock
    #define FTN_UNSET_NEST_LOCK                  omp_unset_nest_lock
    #define FTN_TEST_NEST_LOCK                   omp_test_nest_lock

    #define FTN_SET_WARNINGS_ON                  kmp_set_warnings_on
    #define FTN_SET_WARNINGS_OFF                 kmp_set_warnings_off

    #define FTN_GET_WTIME                        omp_get_wtime
    #define FTN_GET_WTICK                        omp_get_wtick

#if OMP_40_ENABLED
#if KMP_MIC || KMP_OS_DARWIN
    #define FTN_GET_DEFAULT_DEVICE               omp_get_default_device
    #define FTN_SET_DEFAULT_DEVICE               omp_set_default_device
    #define FTN_GET_NUM_DEVICES                  omp_get_num_devices
#endif
#endif

#endif /* KMP_FTN_PLAIN */

/* ------------------------------------------------------------------------ */

#if KMP_FTN_ENTRIES == KMP_FTN_APPEND

    #define FTN_SET_STACKSIZE                    kmp_set_stacksize_
    #define FTN_SET_STACKSIZE_S                  kmp_set_stacksize_s_
    #define FTN_GET_STACKSIZE                    kmp_get_stacksize_
    #define FTN_GET_STACKSIZE_S                  kmp_get_stacksize_s_
    #define FTN_SET_BLOCKTIME                    kmp_set_blocktime_
    #define FTN_GET_BLOCKTIME                    kmp_get_blocktime_
    #define FTN_SET_LIBRARY_SERIAL               kmp_set_library_serial_
    #define FTN_SET_LIBRARY_TURNAROUND           kmp_set_library_turnaround_
    #define FTN_SET_LIBRARY_THROUGHPUT           kmp_set_library_throughput_
    #define FTN_SET_LIBRARY                      kmp_set_library_
    #define FTN_GET_LIBRARY                      kmp_get_library_
    #define FTN_SET_DEFAULTS                     kmp_set_defaults_
    #define FTN_SET_AFFINITY                     kmp_set_affinity_
    #define FTN_GET_AFFINITY                     kmp_get_affinity_
    #define FTN_GET_AFFINITY_MAX_PROC            kmp_get_affinity_max_proc_
    #define FTN_CREATE_AFFINITY_MASK             kmp_create_affinity_mask_
    #define FTN_DESTROY_AFFINITY_MASK            kmp_destroy_affinity_mask_
    #define FTN_SET_AFFINITY_MASK_PROC           kmp_set_affinity_mask_proc_
    #define FTN_UNSET_AFFINITY_MASK_PROC         kmp_unset_affinity_mask_proc_
    #define FTN_GET_AFFINITY_MASK_PROC           kmp_get_affinity_mask_proc_

    #define FTN_MALLOC                           kmp_malloc_
    #define FTN_CALLOC                           kmp_calloc_
    #define FTN_REALLOC                          kmp_realloc_
    #define FTN_FREE                             kmp_free_

    #define FTN_GET_NUM_KNOWN_THREADS            kmp_get_num_known_threads_

    #define FTN_SET_NUM_THREADS                  omp_set_num_threads_
    #define FTN_GET_NUM_THREADS                  omp_get_num_threads_
    #define FTN_GET_MAX_THREADS                  omp_get_max_threads_
    #define FTN_GET_THREAD_NUM                   omp_get_thread_num_
    #define FTN_GET_NUM_PROCS                    omp_get_num_procs_
    #define FTN_SET_DYNAMIC                      omp_set_dynamic_
    #define FTN_GET_DYNAMIC                      omp_get_dynamic_
    #define FTN_SET_NESTED                       omp_set_nested_
    #define FTN_GET_NESTED                       omp_get_nested_
    #define FTN_IN_PARALLEL                      omp_in_parallel_
    #define FTN_GET_THREAD_LIMIT                 omp_get_thread_limit_
    #define FTN_SET_SCHEDULE                     omp_set_schedule_
    #define FTN_GET_SCHEDULE                     omp_get_schedule_
    #define FTN_SET_MAX_ACTIVE_LEVELS            omp_set_max_active_levels_
    #define FTN_GET_MAX_ACTIVE_LEVELS            omp_get_max_active_levels_
    #define FTN_GET_ACTIVE_LEVEL                 omp_get_active_level_
    #define FTN_GET_LEVEL                        omp_get_level_
    #define FTN_GET_ANCESTOR_THREAD_NUM          omp_get_ancestor_thread_num_
    #define FTN_GET_TEAM_SIZE                    omp_get_team_size_
    #define FTN_IN_FINAL                         omp_in_final_
//  #define FTN_SET_PROC_BIND                    omp_set_proc_bind_
    #define FTN_GET_PROC_BIND                    omp_get_proc_bind_
//  #define FTN_CURR_PROC_BIND                   omp_curr_proc_bind_
#if OMP_40_ENABLED
    #define FTN_GET_NUM_TEAMS                    omp_get_num_teams_
    #define FTN_GET_TEAM_NUM                     omp_get_team_num_
#endif
    #define FTN_INIT_LOCK                        omp_init_lock_
    #define FTN_DESTROY_LOCK                     omp_destroy_lock_
    #define FTN_SET_LOCK                         omp_set_lock_
    #define FTN_UNSET_LOCK                       omp_unset_lock_
    #define FTN_TEST_LOCK                        omp_test_lock_
    #define FTN_INIT_NEST_LOCK                   omp_init_nest_lock_
    #define FTN_DESTROY_NEST_LOCK                omp_destroy_nest_lock_
    #define FTN_SET_NEST_LOCK                    omp_set_nest_lock_
    #define FTN_UNSET_NEST_LOCK                  omp_unset_nest_lock_
    #define FTN_TEST_NEST_LOCK                   omp_test_nest_lock_

    #define FTN_SET_WARNINGS_ON                  kmp_set_warnings_on_
    #define FTN_SET_WARNINGS_OFF                 kmp_set_warnings_off_

    #define FTN_GET_WTIME                        omp_get_wtime_
    #define FTN_GET_WTICK                        omp_get_wtick_

#if OMP_40_ENABLED
#if KMP_MIC || KMP_OS_DARWIN
    #define FTN_GET_DEFAULT_DEVICE               omp_get_default_device_
    #define FTN_SET_DEFAULT_DEVICE               omp_set_default_device_
    #define FTN_GET_NUM_DEVICES                  omp_get_num_devices_
#endif
#endif


#endif /* KMP_FTN_APPEND */

/* ------------------------------------------------------------------------ */

#if KMP_FTN_ENTRIES == KMP_FTN_UPPER

    #define FTN_SET_STACKSIZE                    KMP_SET_STACKSIZE
    #define FTN_SET_STACKSIZE_S                  KMP_SET_STACKSIZE_S
    #define FTN_GET_STACKSIZE                    KMP_GET_STACKSIZE
    #define FTN_GET_STACKSIZE_S                  KMP_GET_STACKSIZE_S
    #define FTN_SET_BLOCKTIME                    KMP_SET_BLOCKTIME
    #define FTN_GET_BLOCKTIME                    KMP_GET_BLOCKTIME
    #define FTN_SET_LIBRARY_SERIAL               KMP_SET_LIBRARY_SERIAL
    #define FTN_SET_LIBRARY_TURNAROUND           KMP_SET_LIBRARY_TURNAROUND
    #define FTN_SET_LIBRARY_THROUGHPUT           KMP_SET_LIBRARY_THROUGHPUT
    #define FTN_SET_LIBRARY                      KMP_SET_LIBRARY
    #define FTN_GET_LIBRARY                      KMP_GET_LIBRARY
    #define FTN_SET_DEFAULTS                     KMP_SET_DEFAULTS
    #define FTN_SET_AFFINITY                     KMP_SET_AFFINITY
    #define FTN_GET_AFFINITY                     KMP_GET_AFFINITY
    #define FTN_GET_AFFINITY_MAX_PROC            KMP_GET_AFFINITY_MAX_PROC
    #define FTN_CREATE_AFFINITY_MASK             KMP_CREATE_AFFINITY_MASK
    #define FTN_DESTROY_AFFINITY_MASK            KMP_DESTROY_AFFINITY_MASK
    #define FTN_SET_AFFINITY_MASK_PROC           KMP_SET_AFFINITY_MASK_PROC
    #define FTN_UNSET_AFFINITY_MASK_PROC         KMP_UNSET_AFFINITY_MASK_PROC
    #define FTN_GET_AFFINITY_MASK_PROC           KMP_GET_AFFINITY_MASK_PROC

    #define FTN_MALLOC                           KMP_MALLOC
    #define FTN_CALLOC                           KMP_CALLOC
    #define FTN_REALLOC                          KMP_REALLOC
    #define FTN_FREE                             KMP_FREE

    #define FTN_GET_NUM_KNOWN_THREADS            KMP_GET_NUM_KNOWN_THREADS

    #define FTN_SET_NUM_THREADS                  OMP_SET_NUM_THREADS
    #define FTN_GET_NUM_THREADS                  OMP_GET_NUM_THREADS
    #define FTN_GET_MAX_THREADS                  OMP_GET_MAX_THREADS
    #define FTN_GET_THREAD_NUM                   OMP_GET_THREAD_NUM
    #define FTN_GET_NUM_PROCS                    OMP_GET_NUM_PROCS
    #define FTN_SET_DYNAMIC                      OMP_SET_DYNAMIC
    #define FTN_GET_DYNAMIC                      OMP_GET_DYNAMIC
    #define FTN_SET_NESTED                       OMP_SET_NESTED
    #define FTN_GET_NESTED                       OMP_GET_NESTED
    #define FTN_IN_PARALLEL                      OMP_IN_PARALLEL
    #define FTN_GET_THREAD_LIMIT                 OMP_GET_THREAD_LIMIT
    #define FTN_SET_SCHEDULE                     OMP_SET_SCHEDULE
    #define FTN_GET_SCHEDULE                     OMP_GET_SCHEDULE
    #define FTN_SET_MAX_ACTIVE_LEVELS            OMP_SET_MAX_ACTIVE_LEVELS
    #define FTN_GET_MAX_ACTIVE_LEVELS            OMP_GET_MAX_ACTIVE_LEVELS
    #define FTN_GET_ACTIVE_LEVEL                 OMP_GET_ACTIVE_LEVEL
    #define FTN_GET_LEVEL                        OMP_GET_LEVEL
    #define FTN_GET_ANCESTOR_THREAD_NUM          OMP_GET_ANCESTOR_THREAD_NUM
    #define FTN_GET_TEAM_SIZE                    OMP_GET_TEAM_SIZE
    #define FTN_IN_FINAL                         OMP_IN_FINAL
//  #define FTN_SET_PROC_BIND                    OMP_SET_PROC_BIND
    #define FTN_GET_PROC_BIND                    OMP_GET_PROC_BIND
//  #define FTN_CURR_PROC_BIND                   OMP_CURR_PROC_BIND
#if OMP_40_ENABLED
    #define FTN_GET_NUM_TEAMS                    OMP_GET_NUM_TEAMS
    #define FTN_GET_TEAM_NUM                     OMP_GET_TEAM_NUM
#endif
    #define FTN_INIT_LOCK                        OMP_INIT_LOCK
    #define FTN_DESTROY_LOCK                     OMP_DESTROY_LOCK
    #define FTN_SET_LOCK                         OMP_SET_LOCK
    #define FTN_UNSET_LOCK                       OMP_UNSET_LOCK
    #define FTN_TEST_LOCK                        OMP_TEST_LOCK
    #define FTN_INIT_NEST_LOCK                   OMP_INIT_NEST_LOCK
    #define FTN_DESTROY_NEST_LOCK                OMP_DESTROY_NEST_LOCK
    #define FTN_SET_NEST_LOCK                    OMP_SET_NEST_LOCK
    #define FTN_UNSET_NEST_LOCK                  OMP_UNSET_NEST_LOCK
    #define FTN_TEST_NEST_LOCK                   OMP_TEST_NEST_LOCK

    #define FTN_SET_WARNINGS_ON                  KMP_SET_WARNINGS_ON
    #define FTN_SET_WARNINGS_OFF                 KMP_SET_WARNINGS_OFF

    #define FTN_GET_WTIME                        OMP_GET_WTIME
    #define FTN_GET_WTICK                        OMP_GET_WTICK

#if OMP_40_ENABLED
#if KMP_MIC || KMP_OS_DARWIN
    #define FTN_GET_DEFAULT_DEVICE               OMP_GET_DEFAULT_DEVICE
    #define FTN_SET_DEFAULT_DEVICE               OMP_SET_DEFAULT_DEVICE
    #define FTN_GET_NUM_DEVICES                  OMP_GET_NUM_DEVICES
#endif
#endif


#endif /* KMP_FTN_UPPER */

/* ------------------------------------------------------------------------ */

#if KMP_FTN_ENTRIES == KMP_FTN_UAPPEND

    #define FTN_SET_STACKSIZE                    KMP_SET_STACKSIZE_
    #define FTN_SET_STACKSIZE_S                  KMP_SET_STACKSIZE_S_
    #define FTN_GET_STACKSIZE                    KMP_GET_STACKSIZE_
    #define FTN_GET_STACKSIZE_S                  KMP_GET_STACKSIZE_S_
    #define FTN_SET_BLOCKTIME                    KMP_SET_BLOCKTIME_
    #define FTN_GET_BLOCKTIME                    KMP_GET_BLOCKTIME_
    #define FTN_SET_LIBRARY_SERIAL               KMP_SET_LIBRARY_SERIAL_
    #define FTN_SET_LIBRARY_TURNAROUND           KMP_SET_LIBRARY_TURNAROUND_
    #define FTN_SET_LIBRARY_THROUGHPUT           KMP_SET_LIBRARY_THROUGHPUT_
    #define FTN_SET_LIBRARY                      KMP_SET_LIBRARY_
    #define FTN_GET_LIBRARY                      KMP_GET_LIBRARY_
    #define FTN_SET_DEFAULTS                     KMP_SET_DEFAULTS_
    #define FTN_SET_AFFINITY                     KMP_SET_AFFINITY_
    #define FTN_GET_AFFINITY                     KMP_GET_AFFINITY_
    #define FTN_GET_AFFINITY_MAX_PROC            KMP_GET_AFFINITY_MAX_PROC_
    #define FTN_CREATE_AFFINITY_MASK             KMP_CREATE_AFFINITY_MASK_
    #define FTN_DESTROY_AFFINITY_MASK            KMP_DESTROY_AFFINITY_MASK_
    #define FTN_SET_AFFINITY_MASK_PROC           KMP_SET_AFFINITY_MASK_PROC_
    #define FTN_UNSET_AFFINITY_MASK_PROC         KMP_UNSET_AFFINITY_MASK_PROC_
    #define FTN_GET_AFFINITY_MASK_PROC           KMP_GET_AFFINITY_MASK_PROC_

    #define FTN_MALLOC                           KMP_MALLOC_
    #define FTN_CALLOC                           KMP_CALLOC_
    #define FTN_REALLOC                          KMP_REALLOC_
    #define FTN_FREE                             KMP_FREE_

    #define FTN_GET_NUM_KNOWN_THREADS            KMP_GET_NUM_KNOWN_THREADS_

    #define FTN_SET_NUM_THREADS                  OMP_SET_NUM_THREADS_
    #define FTN_GET_NUM_THREADS                  OMP_GET_NUM_THREADS_
    #define FTN_GET_MAX_THREADS                  OMP_GET_MAX_THREADS_
    #define FTN_GET_THREAD_NUM                   OMP_GET_THREAD_NUM_
    #define FTN_GET_NUM_PROCS                    OMP_GET_NUM_PROCS_
    #define FTN_SET_DYNAMIC                      OMP_SET_DYNAMIC_
    #define FTN_GET_DYNAMIC                      OMP_GET_DYNAMIC_
    #define FTN_SET_NESTED                       OMP_SET_NESTED_
    #define FTN_GET_NESTED                       OMP_GET_NESTED_
    #define FTN_IN_PARALLEL                      OMP_IN_PARALLEL_
    #define FTN_GET_THREAD_LIMIT                 OMP_GET_THREAD_LIMIT_
    #define FTN_SET_SCHEDULE                     OMP_SET_SCHEDULE_
    #define FTN_GET_SCHEDULE                     OMP_GET_SCHEDULE_
    #define FTN_SET_MAX_ACTIVE_LEVELS            OMP_SET_MAX_ACTIVE_LEVELS_
    #define FTN_GET_MAX_ACTIVE_LEVELS            OMP_GET_MAX_ACTIVE_LEVELS_
    #define FTN_GET_ACTIVE_LEVEL                 OMP_GET_ACTIVE_LEVEL_
    #define FTN_GET_LEVEL                        OMP_GET_LEVEL_
    #define FTN_GET_ANCESTOR_THREAD_NUM          OMP_GET_ANCESTOR_THREAD_NUM_
    #define FTN_GET_TEAM_SIZE                    OMP_GET_TEAM_SIZE_
    #define FTN_IN_FINAL                         OMP_IN_FINAL_
//  #define FTN_SET_PROC_BIND                    OMP_SET_PROC_BIND_
    #define FTN_GET_PROC_BIND                    OMP_GET_PROC_BIND_
//  #define FTN_CURR_PROC_BIND                   OMP_CURR_PROC_BIND_
#if OMP_40_ENABLED
    #define FTN_GET_NUM_TEAMS                    OMP_GET_NUM_TEAMS_
    #define FTN_GET_TEAM_NUM                     OMP_GET_TEAM_NUM_
#endif
    #define FTN_INIT_LOCK                        OMP_INIT_LOCK_
    #define FTN_DESTROY_LOCK                     OMP_DESTROY_LOCK_
    #define FTN_SET_LOCK                         OMP_SET_LOCK_
    #define FTN_UNSET_LOCK                       OMP_UNSET_LOCK_
    #define FTN_TEST_LOCK                        OMP_TEST_LOCK_
    #define FTN_INIT_NEST_LOCK                   OMP_INIT_NEST_LOCK_
    #define FTN_DESTROY_NEST_LOCK                OMP_DESTROY_NEST_LOCK_
    #define FTN_SET_NEST_LOCK                    OMP_SET_NEST_LOCK_
    #define FTN_UNSET_NEST_LOCK                  OMP_UNSET_NEST_LOCK_
    #define FTN_TEST_NEST_LOCK                   OMP_TEST_NEST_LOCK_

    #define FTN_SET_WARNINGS_ON                  KMP_SET_WARNINGS_ON_
    #define FTN_SET_WARNINGS_OFF                 KMP_SET_WARNINGS_OFF_

    #define FTN_GET_WTIME                        OMP_GET_WTIME_
    #define FTN_GET_WTICK                        OMP_GET_WTICK_

#if OMP_40_ENABLED
#if KMP_MIC || KMP_OS_DARWIN
    #define FTN_GET_DEFAULT_DEVICE               OMP_GET_DEFAULT_DEVICE_
    #define FTN_SET_DEFAULT_DEVICE               OMP_SET_DEFAULT_DEVICE_
    #define FTN_GET_NUM_DEVICES                  OMP_GET_NUM_DEVICES_
#endif
#endif


#endif /* KMP_FTN_UAPPEND */

#endif /* KMP_FTN_OS_H */

