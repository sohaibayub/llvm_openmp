// RUN: %libomp-compile-and-run | FileCheck %s
// RUN: %libomp-compile-and-run | %sort-threads | FileCheck --check-prefix=THREADS %s
// REQUIRES: ompt
#include "callback.h"
#include <omp.h>

int main()
{
  omp_set_nested(1);

  #pragma omp parallel num_threads(4)
  {
    print_ids(0);
    print_ids(1);
    #pragma omp parallel num_threads(1)
    {
      print_ids(0);
      print_ids(1);
      print_ids(2);
      #pragma omp parallel num_threads(4)
      {
        print_ids(0);
        print_ids(1);
        print_ids(2);
        print_ids(3);
      }
    }
  }

  // this patch version is expected to have broken behaviour and provides no parent_task_frame.reentry address
  // Broken-THREADS is the intended behaviour, provided by next patch
  // Broken: THREADS provides a running test for the broken behaviour

  // CHECK: 0: NULL_POINTER=[[NULL:.*$]]
  // CHECK: {{^}}[[MASTER_ID:[0-9]+]]: ompt_event_parallel_begin: parent_task_id=[[PARENT_TASK_ID:[0-9]+]], parent_task_frame.exit=[[NULL]], parent_task_frame.reenter={{0x[0-f]+}}, parallel_id=[[PARALLEL_ID:[0-9]+]], requested_team_size=4, parallel_function=0x{{[0-f]+}}, invoker=[[PARALLEL_INVOKER:.+]]

  // CHECK-DAG: {{^}}[[MASTER_ID]]: ompt_event_implicit_task_begin: parallel_id=[[PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID:[0-9]+]]
  // CHECK-DAG: {{^}}[[MASTER_ID]]: ompt_event_implicit_task_end: parallel_id=[[PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]]

  // Note that we cannot ensure that the worker threads have already called barrier_end and implicit_task_end before parallel_end!

  // CHECK-DAG: {{^}}[[THREAD_ID:[0-9]+]]: ompt_event_implicit_task_begin: parallel_id=[[PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID:[0-9]+]]
  // CHECK-DAG: {{^}}[[THREAD_ID]]: ompt_event_barrier_begin: parallel_id=[[PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]]

  // CHECK-DAG: {{^}}[[THREAD_ID:[0-9]+]]: ompt_event_implicit_task_begin: parallel_id=[[PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID:[0-9]+]]
  // CHECK-DAG: {{^}}[[THREAD_ID]]: ompt_event_barrier_begin: parallel_id=[[PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]]

  // CHECK-DAG: {{^}}[[THREAD_ID:[0-9]+]]: ompt_event_implicit_task_begin: parallel_id=[[PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID:[0-9]+]]
  // CHECK-DAG: {{^}}[[THREAD_ID]]: ompt_event_barrier_begin: parallel_id=[[PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]]

  // CHECK: {{^}}[[MASTER_ID]]: ompt_event_parallel_end: parallel_id=[[PARALLEL_ID]], task_id=[[PARENT_TASK_ID]], invoker=[[PARALLEL_INVOKER]]


  // THREADS: 0: NULL_POINTER=[[NULL:.*$]]
  // THREADS: {{^}}[[MASTER_ID:[0-9]+]]: ompt_event_parallel_begin: parent_task_id=[[PARENT_TASK_ID:[0-9]+]], parent_task_frame.exit=[[NULL]], parent_task_frame.reenter=[[TASK_FRAME_ENTER:0x[0-f]+]], parallel_id=[[PARALLEL_ID:[0-9]+]], requested_team_size=4, parallel_function=0x{{[0-f]+}}, invoker=[[PARALLEL_INVOKER:.+]]

  // nested parallel masters
  // THREADS: {{^}}[[MASTER_ID]]: ompt_event_implicit_task_begin: parallel_id=[[PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID:[0-9]+]]
  // THREADS: {{^}}[[MASTER_ID]]: level 0: parallel_id=[[PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]], exit_frame=[[NESTED_TASK_FRAME_EXIT:0x[0-f]+]], reenter_frame=[[NULL]]
  // THREADS: {{^}}[[MASTER_ID]]: level 1: parallel_id=0, task_id=[[PARENT_TASK_ID]], exit_frame=[[NULL]], reenter_frame=[[TASK_FRAME_ENTER]]

  // THREADS: {{^}}[[MASTER_ID]]: ompt_event_parallel_begin: parent_task_id=[[IMPLICIT_TASK_ID]], parent_task_frame.exit=[[NESTED_TASK_FRAME_EXIT]], parent_task_frame.reenter=[[NESTED_TASK_FRAME_ENTER:0x[0-f]+]], parallel_id=[[NESTED_PARALLEL_ID:[0-9]+]], requested_team_size=1, parallel_function=[[NESTED_PARALLEL_FUNCTION:0x[0-f]+]], invoker=[[PARALLEL_INVOKER]]
  // THREADS: {{^}}[[MASTER_ID]]: ompt_event_implicit_task_begin: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[NESTED_IMPLICIT_TASK_ID:[0-9]+]]
  // THREADS: {{^}}[[MASTER_ID]]: level 0: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[NESTED_IMPLICIT_TASK_ID]], exit_frame=[[NESTED_NESTED_TASK_FRAME_EXIT:0x[0-f]+]], reenter_frame=[[NULL]]
  // THREADS: {{^}}[[MASTER_ID]]: level 1: parallel_id=[[PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]], exit_frame=[[NESTED_TASK_FRAME_EXIT]], reenter_frame=[[NESTED_TASK_FRAME_ENTER]]
  // THREADS: {{^}}[[MASTER_ID]]: level 2: parallel_id=0, task_id=[[PARENT_TASK_ID]], exit_frame=[[NULL]], reenter_frame=[[TASK_FRAME_ENTER]]

  // Broken-THREADS: {{^}}[[MASTER_ID]]: ompt_event_parallel_begin: parent_task_id=[[NESTED_IMPLICIT_TASK_ID]], parent_task_frame.exit=[[NESTED_NESTED_TASK_FRAME_EXIT]], parent_task_frame.reenter=[[NESTED_NESTED_TASK_FRAME_ENTER:0x[0-f]+]], parallel_id=[[NESTED_NESTED_PARALLEL_ID:[0-9]+]], requested_team_size=4, parallel_function=[[NESTED_NESTED_PARALLEL_FUNCTION:0x[0-f]+]], invoker=[[PARALLEL_INVOKER]]
  // Broken: THREADS: {{^}}[[MASTER_ID]]: ompt_event_parallel_begin: parent_task_id=[[NESTED_IMPLICIT_TASK_ID]], parent_task_frame.exit=[[NESTED_NESTED_TASK_FRAME_EXIT]], parent_task_frame.reenter=[[NULL]], parallel_id=[[NESTED_NESTED_PARALLEL_ID:[0-9]+]], requested_team_size=4, parallel_function=[[NESTED_NESTED_PARALLEL_FUNCTION:0x[0-f]+]], invoker=[[PARALLEL_INVOKER]]
  // THREADS: {{^}}[[MASTER_ID]]: ompt_event_implicit_task_begin: parallel_id=[[NESTED_NESTED_PARALLEL_ID]], task_id=[[NESTED_NESTED_IMPLICIT_TASK_ID:[0-9]+]]
  // THREADS: {{^}}[[MASTER_ID]]: level 0: parallel_id=[[NESTED_NESTED_PARALLEL_ID]], task_id=[[NESTED_NESTED_IMPLICIT_TASK_ID]], exit_frame=[[NESTED_NESTED_NESTED_TASK_FRAME_EXIT:0x[0-f]+]], reenter_frame=[[NULL]]
  // Broken-THREADS: {{^}}[[MASTER_ID]]: level 1: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[NESTED_IMPLICIT_TASK_ID]], exit_frame=[[NESTED_NESTED_TASK_FRAME_EXIT]], reenter_frame=[[NESTED_NESTED_TASK_FRAME_ENTER]]
  // Broken: THREADS: {{^}}[[MASTER_ID]]: level 1: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[NESTED_IMPLICIT_TASK_ID]], exit_frame=[[NESTED_NESTED_TASK_FRAME_EXIT]], reenter_frame=[[NULL]]
  // THREADS: {{^}}[[MASTER_ID]]: level 2: parallel_id=[[PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]], exit_frame=[[NESTED_TASK_FRAME_EXIT]], reenter_frame=[[NESTED_TASK_FRAME_ENTER]]
  // THREADS: {{^}}[[MASTER_ID]]: level 3: parallel_id=0, task_id=[[PARENT_TASK_ID]], exit_frame=[[NULL]], reenter_frame=[[TASK_FRAME_ENTER]]
  // THREADS-NOT: {{^}}[[MASTER_ID]]: ompt_event_implicit_task_end
  // THREADS: {{^}}[[MASTER_ID]]: ompt_event_barrier_begin: parallel_id=[[NESTED_NESTED_PARALLEL_ID]], task_id=[[NESTED_NESTED_IMPLICIT_TASK_ID]]
  // THREADS: {{^}}[[MASTER_ID]]: ompt_event_barrier_end: parallel_id=[[NESTED_NESTED_PARALLEL_ID]], task_id=[[NESTED_NESTED_IMPLICIT_TASK_ID]]
  // THREADS: {{^}}[[MASTER_ID]]: ompt_event_implicit_task_end: parallel_id=[[NESTED_NESTED_PARALLEL_ID]], task_id=[[NESTED_NESTED_IMPLICIT_TASK_ID]]
  // THREADS: {{^}}[[MASTER_ID]]: ompt_event_parallel_end: parallel_id=[[NESTED_NESTED_PARALLEL_ID]], task_id=[[NESTED_IMPLICIT_TASK_ID]], invoker=[[PARALLEL_INVOKER]]
  // THREADS-NOT: {{^}}[[MASTER_ID]]: ompt_event_implicit_task_end
  // THREADS: {{^}}[[MASTER_ID]]: ompt_event_implicit_task_end: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[NESTED_IMPLICIT_TASK_ID]]
  // THREADS: {{^}}[[MASTER_ID]]: ompt_event_parallel_end: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]], invoker=[[PARALLEL_INVOKER]]
  // THREADS-NOT: {{^}}[[MASTER_ID]]: ompt_event_implicit_task_end
  // THREADS: {{^}}[[MASTER_ID]]: ompt_event_barrier_begin: parallel_id=[[PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]]
  // THREADS: {{^}}[[MASTER_ID]]: ompt_event_barrier_end: parallel_id=[[PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]]
  // THREADS: {{^}}[[MASTER_ID]]: ompt_event_implicit_task_end: parallel_id=[[PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]]

  // THREADS: {{^}}[[THREAD_ID:[0-9]+]]: ompt_event_implicit_task_begin: parallel_id=[[PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID:[0-9]+]]
  // THREADS: {{^}}[[THREAD_ID]]: level 0: parallel_id=[[PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]], exit_frame=[[NESTED_TASK_FRAME_EXIT:0x[0-f]+]], reenter_frame=[[NULL]]
  // THREADS: {{^}}[[THREAD_ID]]: level 1: parallel_id=0, task_id=[[PARENT_TASK_ID]], exit_frame=[[NULL]], reenter_frame=[[TASK_FRAME_ENTER]]
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_parallel_begin: parent_task_id=[[IMPLICIT_TASK_ID]], parent_task_frame.exit=[[NESTED_TASK_FRAME_EXIT]], parent_task_frame.reenter=[[NESTED_TASK_FRAME_ENTER:0x[0-f]+]], parallel_id=[[NESTED_PARALLEL_ID:[0-9]+]], requested_team_size=1, parallel_function=[[NESTED_PARALLEL_FUNCTION]], invoker=[[PARALLEL_INVOKER]]
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_implicit_task_begin: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[NESTED_IMPLICIT_TASK_ID:[0-9]+]]
  // THREADS: {{^}}[[THREAD_ID]]: level 0: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[NESTED_IMPLICIT_TASK_ID]], exit_frame=[[NESTED_NESTED_TASK_FRAME_EXIT:0x[0-f]+]], reenter_frame=[[NULL]]
  // THREADS: {{^}}[[THREAD_ID]]: level 1: parallel_id=[[PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]], exit_frame=[[NESTED_TASK_FRAME_EXIT]], reenter_frame=[[NESTED_TASK_FRAME_ENTER]]
  // THREADS: {{^}}[[THREAD_ID]]: level 2: parallel_id=0, task_id=[[PARENT_TASK_ID]], exit_frame=[[NULL]], reenter_frame=[[TASK_FRAME_ENTER]]
  // Broken-THREADS: {{^}}[[THREAD_ID]]: ompt_event_parallel_begin: parent_task_id=[[NESTED_IMPLICIT_TASK_ID]], parent_task_frame.exit=[[NESTED_NESTED_TASK_FRAME_EXIT]], parent_task_frame.reenter=[[NULL]], parallel_id=[[NESTED_NESTED_PARALLEL_ID:[0-9]+]], requested_team_size=4, parallel_function=[[NESTED_NESTED_PARALLEL_FUNCTION]], invoker=[[PARALLEL_INVOKER]]
  // Broken: THREADS: {{^}}[[THREAD_ID]]: ompt_event_parallel_begin: parent_task_id=[[NESTED_IMPLICIT_TASK_ID]], parent_task_frame.exit=[[NESTED_NESTED_TASK_FRAME_EXIT]], parent_task_frame.reenter=[[NULL]], parallel_id=[[NESTED_NESTED_PARALLEL_ID:[0-9]+]], requested_team_size=4, parallel_function=[[NESTED_NESTED_PARALLEL_FUNCTION]], invoker=[[PARALLEL_INVOKER]]
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_implicit_task_begin: parallel_id=[[NESTED_NESTED_PARALLEL_ID]], task_id=[[NESTED_NESTED_IMPLICIT_TASK_ID:[0-9]+]]
  // THREADS: {{^}}[[THREAD_ID]]: level 0: parallel_id=[[NESTED_NESTED_PARALLEL_ID]], task_id=[[NESTED_NESTED_IMPLICIT_TASK_ID]], exit_frame=[[NESTED_NESTED_NESTED_TASK_FRAME_EXIT:0x[0-f]+]], reenter_frame=[[NULL]]
  // Broken-THREADS: {{^}}[[THREAD_ID]]: level 1: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[NESTED_IMPLICIT_TASK_ID]], exit_frame=[[NESTED_NESTED_TASK_FRAME_EXIT]], reenter_frame=[[NESTED_NESTED_TASK_FRAME_ENTER]]
  // Broken: THREADS: {{^}}[[THREAD_ID]]: level 1: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[NESTED_IMPLICIT_TASK_ID]], exit_frame=[[NESTED_NESTED_TASK_FRAME_EXIT]], reenter_frame=[[NULL]]
  // THREADS: {{^}}[[THREAD_ID]]: level 2: parallel_id=[[PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]], exit_frame=[[NESTED_TASK_FRAME_EXIT]], reenter_frame=[[NESTED_TASK_FRAME_ENTER]]
  // THREADS: {{^}}[[THREAD_ID]]: level 3: parallel_id=0, task_id=[[PARENT_TASK_ID]], exit_frame=[[NULL]], reenter_frame=[[TASK_FRAME_ENTER]]
  // THREADS-NOT: {{^}}[[THREAD_ID]]: ompt_event_implicit_task_end
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_barrier_begin: parallel_id=[[NESTED_NESTED_PARALLEL_ID]], task_id=[[NESTED_NESTED_IMPLICIT_TASK_ID]]
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_barrier_end: parallel_id=[[NESTED_NESTED_PARALLEL_ID]], task_id=[[NESTED_NESTED_IMPLICIT_TASK_ID]]
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_implicit_task_end: parallel_id=[[NESTED_NESTED_PARALLEL_ID]], task_id=[[NESTED_NESTED_IMPLICIT_TASK_ID]]
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_parallel_end: parallel_id=[[NESTED_NESTED_PARALLEL_ID]], task_id=[[NESTED_IMPLICIT_TASK_ID]], invoker=[[PARALLEL_INVOKER]]
  // THREADS-NOT: {{^}}[[THREAD_ID]]: ompt_event_implicit_task_end
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_implicit_task_end: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[NESTED_IMPLICIT_TASK_ID]]
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_parallel_end: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]], invoker=[[PARALLEL_INVOKER]]
  // THREADS-NOT: {{^}}[[THREAD_ID]]: ompt_event_implicit_task_end
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_barrier_begin: parallel_id=[[PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]]
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_barrier_end: parallel_id=[[PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]]
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_implicit_task_end: parallel_id=[[PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]]

  // THREADS: {{^}}[[THREAD_ID:[0-9]+]]: ompt_event_implicit_task_begin: parallel_id=[[PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID:[0-9]+]]
  // THREADS: {{^}}[[THREAD_ID]]: level 0: parallel_id=[[PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]], exit_frame=[[NESTED_TASK_FRAME_EXIT:0x[0-f]+]], reenter_frame=[[NULL]]
  // THREADS: {{^}}[[THREAD_ID]]: level 1: parallel_id=0, task_id=[[PARENT_TASK_ID]], exit_frame=[[NULL]], reenter_frame=[[TASK_FRAME_ENTER]]
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_parallel_begin: parent_task_id=[[IMPLICIT_TASK_ID]], parent_task_frame.exit=[[NESTED_TASK_FRAME_EXIT]], parent_task_frame.reenter=[[NESTED_TASK_FRAME_ENTER:0x[0-f]+]], parallel_id=[[NESTED_PARALLEL_ID:[0-9]+]], requested_team_size=1, parallel_function=[[NESTED_PARALLEL_FUNCTION]], invoker=[[PARALLEL_INVOKER]]
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_implicit_task_begin: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[NESTED_IMPLICIT_TASK_ID:[0-9]+]]
  // THREADS: {{^}}[[THREAD_ID]]: level 0: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[NESTED_IMPLICIT_TASK_ID]], exit_frame=[[NESTED_NESTED_TASK_FRAME_EXIT:0x[0-f]+]], reenter_frame=[[NULL]]
  // THREADS: {{^}}[[THREAD_ID]]: level 1: parallel_id=[[PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]], exit_frame=[[NESTED_TASK_FRAME_EXIT]], reenter_frame=[[NESTED_TASK_FRAME_ENTER]]
  // THREADS: {{^}}[[THREAD_ID]]: level 2: parallel_id=0, task_id=[[PARENT_TASK_ID]], exit_frame=[[NULL]], reenter_frame=[[TASK_FRAME_ENTER]]
  // Broken-THREADS: {{^}}[[THREAD_ID]]: ompt_event_parallel_begin: parent_task_id=[[NESTED_IMPLICIT_TASK_ID]], parent_task_frame.exit=[[NESTED_NESTED_TASK_FRAME_EXIT]], parent_task_frame.reenter=[[NULL]], parallel_id=[[NESTED_NESTED_PARALLEL_ID:[0-9]+]], requested_team_size=4, parallel_function=[[NESTED_NESTED_PARALLEL_FUNCTION]], invoker=[[PARALLEL_INVOKER]]
  // Broken: THREADS: {{^}}[[THREAD_ID]]: ompt_event_parallel_begin: parent_task_id=[[NESTED_IMPLICIT_TASK_ID]], parent_task_frame.exit=[[NESTED_NESTED_TASK_FRAME_EXIT]], parent_task_frame.reenter=[[NULL]], parallel_id=[[NESTED_NESTED_PARALLEL_ID:[0-9]+]], requested_team_size=4, parallel_function=[[NESTED_NESTED_PARALLEL_FUNCTION]], invoker=[[PARALLEL_INVOKER]]
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_implicit_task_begin: parallel_id=[[NESTED_NESTED_PARALLEL_ID]], task_id=[[NESTED_NESTED_IMPLICIT_TASK_ID:[0-9]+]]
  // THREADS: {{^}}[[THREAD_ID]]: level 0: parallel_id=[[NESTED_NESTED_PARALLEL_ID]], task_id=[[NESTED_NESTED_IMPLICIT_TASK_ID]], exit_frame=[[NESTED_NESTED_NESTED_TASK_FRAME_EXIT:0x[0-f]+]], reenter_frame=[[NULL]]
  // Broken-THREADS: {{^}}[[THREAD_ID]]: level 1: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[NESTED_IMPLICIT_TASK_ID]], exit_frame=[[NESTED_NESTED_TASK_FRAME_EXIT]], reenter_frame=[[NESTED_NESTED_TASK_FRAME_ENTER]]
  // Broken: THREADS: {{^}}[[THREAD_ID]]: level 1: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[NESTED_IMPLICIT_TASK_ID]], exit_frame=[[NESTED_NESTED_TASK_FRAME_EXIT]], reenter_frame=[[NULL]]
  // THREADS: {{^}}[[THREAD_ID]]: level 2: parallel_id=[[PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]], exit_frame=[[NESTED_TASK_FRAME_EXIT]], reenter_frame=[[NESTED_TASK_FRAME_ENTER]]
  // THREADS: {{^}}[[THREAD_ID]]: level 3: parallel_id=0, task_id=[[PARENT_TASK_ID]], exit_frame=[[NULL]], reenter_frame=[[TASK_FRAME_ENTER]]
  // THREADS-NOT: {{^}}[[THREAD_ID]]: ompt_event_implicit_task_end
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_barrier_begin: parallel_id=[[NESTED_NESTED_PARALLEL_ID]], task_id=[[NESTED_NESTED_IMPLICIT_TASK_ID]]
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_barrier_end: parallel_id=[[NESTED_NESTED_PARALLEL_ID]], task_id=[[NESTED_NESTED_IMPLICIT_TASK_ID]]
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_implicit_task_end: parallel_id=[[NESTED_NESTED_PARALLEL_ID]], task_id=[[NESTED_NESTED_IMPLICIT_TASK_ID]]
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_parallel_end: parallel_id=[[NESTED_NESTED_PARALLEL_ID]], task_id=[[NESTED_IMPLICIT_TASK_ID]], invoker=[[PARALLEL_INVOKER]]
  // THREADS-NOT: {{^}}[[THREAD_ID]]: ompt_event_implicit_task_end
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_implicit_task_end: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[NESTED_IMPLICIT_TASK_ID]]
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_parallel_end: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]], invoker=[[PARALLEL_INVOKER]]
  // THREADS-NOT: {{^}}[[THREAD_ID]]: ompt_event_implicit_task_end
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_barrier_begin: parallel_id=[[PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]]
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_barrier_end: parallel_id=[[PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]]
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_implicit_task_end: parallel_id=[[PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]]

  // THREADS: {{^}}[[THREAD_ID:[0-9]+]]: ompt_event_implicit_task_begin: parallel_id=[[PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID:[0-9]+]]
  // THREADS: {{^}}[[THREAD_ID]]: level 0: parallel_id=[[PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]], exit_frame=[[NESTED_TASK_FRAME_EXIT:0x[0-f]+]], reenter_frame=[[NULL]]
  // THREADS: {{^}}[[THREAD_ID]]: level 1: parallel_id=0, task_id=[[PARENT_TASK_ID]], exit_frame=[[NULL]], reenter_frame=[[TASK_FRAME_ENTER]]
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_parallel_begin: parent_task_id=[[IMPLICIT_TASK_ID]], parent_task_frame.exit=[[NESTED_TASK_FRAME_EXIT]], parent_task_frame.reenter=[[NESTED_TASK_FRAME_ENTER:0x[0-f]+]], parallel_id=[[NESTED_PARALLEL_ID:[0-9]+]], requested_team_size=1, parallel_function=[[NESTED_PARALLEL_FUNCTION]], invoker=[[PARALLEL_INVOKER]]
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_implicit_task_begin: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[NESTED_IMPLICIT_TASK_ID:[0-9]+]]
  // THREADS: {{^}}[[THREAD_ID]]: level 0: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[NESTED_IMPLICIT_TASK_ID]], exit_frame=[[NESTED_NESTED_TASK_FRAME_EXIT:0x[0-f]+]], reenter_frame=[[NULL]]
  // THREADS: {{^}}[[THREAD_ID]]: level 1: parallel_id=[[PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]], exit_frame=[[NESTED_TASK_FRAME_EXIT]], reenter_frame=[[NESTED_TASK_FRAME_ENTER]]
  // THREADS: {{^}}[[THREAD_ID]]: level 2: parallel_id=0, task_id=[[PARENT_TASK_ID]], exit_frame=[[NULL]], reenter_frame=[[TASK_FRAME_ENTER]]
  // Broken-THREADS: {{^}}[[THREAD_ID]]: ompt_event_parallel_begin: parent_task_id=[[NESTED_IMPLICIT_TASK_ID]], parent_task_frame.exit=[[NESTED_NESTED_TASK_FRAME_EXIT]], parent_task_frame.reenter=[[NULL]], parallel_id=[[NESTED_NESTED_PARALLEL_ID:[0-9]+]], requested_team_size=4, parallel_function=[[NESTED_NESTED_PARALLEL_FUNCTION]], invoker=[[PARALLEL_INVOKER]]
  // Broken: THREADS: {{^}}[[THREAD_ID]]: ompt_event_parallel_begin: parent_task_id=[[NESTED_IMPLICIT_TASK_ID]], parent_task_frame.exit=[[NESTED_NESTED_TASK_FRAME_EXIT]], parent_task_frame.reenter=[[NULL]], parallel_id=[[NESTED_NESTED_PARALLEL_ID:[0-9]+]], requested_team_size=4, parallel_function=[[NESTED_NESTED_PARALLEL_FUNCTION]], invoker=[[PARALLEL_INVOKER]]
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_implicit_task_begin: parallel_id=[[NESTED_NESTED_PARALLEL_ID]], task_id=[[NESTED_NESTED_IMPLICIT_TASK_ID:[0-9]+]]
  // THREADS: {{^}}[[THREAD_ID]]: level 0: parallel_id=[[NESTED_NESTED_PARALLEL_ID]], task_id=[[NESTED_NESTED_IMPLICIT_TASK_ID]], exit_frame=[[NESTED_NESTED_NESTED_TASK_FRAME_EXIT:0x[0-f]+]], reenter_frame=[[NULL]]
  // Broken-THREADS: {{^}}[[THREAD_ID]]: level 1: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[NESTED_IMPLICIT_TASK_ID]], exit_frame=[[NESTED_NESTED_TASK_FRAME_EXIT]], reenter_frame=[[NESTED_NESTED_TASK_FRAME_ENTER]]
  // Broken: THREADS: {{^}}[[THREAD_ID]]: level 1: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[NESTED_IMPLICIT_TASK_ID]], exit_frame=[[NESTED_NESTED_TASK_FRAME_EXIT]], reenter_frame=[[NULL]]
  // THREADS: {{^}}[[THREAD_ID]]: level 2: parallel_id=[[PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]], exit_frame=[[NESTED_TASK_FRAME_EXIT]], reenter_frame=[[NESTED_TASK_FRAME_ENTER]]
  // THREADS: {{^}}[[THREAD_ID]]: level 3: parallel_id=0, task_id=[[PARENT_TASK_ID]], exit_frame=[[NULL]], reenter_frame=[[TASK_FRAME_ENTER]]
  // THREADS-NOT: {{^}}[[THREAD_ID]]: ompt_event_implicit_task_end
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_barrier_begin: parallel_id=[[NESTED_NESTED_PARALLEL_ID]], task_id=[[NESTED_NESTED_IMPLICIT_TASK_ID]]
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_barrier_end: parallel_id=[[NESTED_NESTED_PARALLEL_ID]], task_id=[[NESTED_NESTED_IMPLICIT_TASK_ID]]
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_implicit_task_end: parallel_id=[[NESTED_NESTED_PARALLEL_ID]], task_id=[[NESTED_NESTED_IMPLICIT_TASK_ID]]
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_parallel_end: parallel_id=[[NESTED_NESTED_PARALLEL_ID]], task_id=[[NESTED_IMPLICIT_TASK_ID]], invoker=[[PARALLEL_INVOKER]]
  // THREADS-NOT: {{^}}[[THREAD_ID]]: ompt_event_implicit_task_end
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_implicit_task_end: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[NESTED_IMPLICIT_TASK_ID]]
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_parallel_end: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]], invoker=[[PARALLEL_INVOKER]]
  // THREADS-NOT: {{^}}[[THREAD_ID]]: ompt_event_implicit_task_end
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_barrier_begin: parallel_id=[[PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]]
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_barrier_end: parallel_id=[[PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]]
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_implicit_task_end: parallel_id=[[PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]]

  // nested parallel worker threads
  // THREADS: {{^}}[[THREAD_ID:[0-9]+]]: ompt_event_implicit_task_begin: parallel_id=[[NESTED_PARALLEL_ID:[0-9]+]], task_id=[[IMPLICIT_TASK_ID:[0-9]+]]
  // THREADS: {{^}}[[THREAD_ID]]: level 0: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]], exit_frame={{0x[0-f]+}}, reenter_frame=[[NULL]]
  // can't reliably tell which parallel region is the parent...
  // THREADS: {{^}}[[THREAD_ID]]: level 1: parallel_id={{[0-9]+}}, task_id={{[0-9]+}}
  // THREADS: {{^}}[[THREAD_ID]]: level 2: parallel_id={{[0-9]+}}, task_id={{[0-9]+}}
  // THREADS: {{^}}[[THREAD_ID]]: level 3: parallel_id=0, task_id=[[PARENT_TASK_ID]], exit_frame=[[NULL]], reenter_frame=[[TASK_FRAME_ENTER]]
  // THREADS-NOT: {{^}}[[THREAD_ID]]: ompt_event_implicit_task_end
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_barrier_begin: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]]
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_barrier_end: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]]
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_implicit_task_end: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]]

  // THREADS: {{^}}[[THREAD_ID:[0-9]+]]: ompt_event_implicit_task_begin: parallel_id=[[NESTED_PARALLEL_ID:[0-9]+]], task_id=[[IMPLICIT_TASK_ID:[0-9]+]]
  // THREADS: {{^}}[[THREAD_ID]]: level 0: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]], exit_frame={{0x[0-f]+}}, reenter_frame=[[NULL]]
  // can't reliably tell which parallel region is the parent...
  // THREADS: {{^}}[[THREAD_ID]]: level 1: parallel_id={{[0-9]+}}, task_id={{[0-9]+}}
  // THREADS: {{^}}[[THREAD_ID]]: level 2: parallel_id={{[0-9]+}}, task_id={{[0-9]+}}
  // THREADS: {{^}}[[THREAD_ID]]: level 3: parallel_id=0, task_id=[[PARENT_TASK_ID]], exit_frame=[[NULL]], reenter_frame=[[TASK_FRAME_ENTER]]
  // THREADS-NOT: {{^}}[[THREAD_ID]]: ompt_event_implicit_task_end
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_barrier_begin: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]]
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_barrier_end: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]]
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_implicit_task_end: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]]

  // THREADS: {{^}}[[THREAD_ID:[0-9]+]]: ompt_event_implicit_task_begin: parallel_id=[[NESTED_PARALLEL_ID:[0-9]+]], task_id=[[IMPLICIT_TASK_ID:[0-9]+]]
  // THREADS: {{^}}[[THREAD_ID]]: level 0: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]], exit_frame={{0x[0-f]+}}, reenter_frame=[[NULL]]
  // can't reliably tell which parallel region is the parent...
  // THREADS: {{^}}[[THREAD_ID]]: level 1: parallel_id={{[0-9]+}}, task_id={{[0-9]+}}
  // THREADS: {{^}}[[THREAD_ID]]: level 2: parallel_id={{[0-9]+}}, task_id={{[0-9]+}}
  // THREADS: {{^}}[[THREAD_ID]]: level 3: parallel_id=0, task_id=[[PARENT_TASK_ID]], exit_frame=[[NULL]], reenter_frame=[[TASK_FRAME_ENTER]]
  // THREADS-NOT: {{^}}[[THREAD_ID]]: ompt_event_implicit_task_end
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_barrier_begin: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]]
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_barrier_end: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]]
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_implicit_task_end: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]]

  // THREADS: {{^}}[[THREAD_ID:[0-9]+]]: ompt_event_implicit_task_begin: parallel_id=[[NESTED_PARALLEL_ID:[0-9]+]], task_id=[[IMPLICIT_TASK_ID:[0-9]+]]
  // THREADS: {{^}}[[THREAD_ID]]: level 0: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]], exit_frame={{0x[0-f]+}}, reenter_frame=[[NULL]]
  // can't reliably tell which parallel region is the parent...
  // THREADS: {{^}}[[THREAD_ID]]: level 1: parallel_id={{[0-9]+}}, task_id={{[0-9]+}}
  // THREADS: {{^}}[[THREAD_ID]]: level 2: parallel_id={{[0-9]+}}, task_id={{[0-9]+}}
  // THREADS: {{^}}[[THREAD_ID]]: level 3: parallel_id=0, task_id=[[PARENT_TASK_ID]], exit_frame=[[NULL]], reenter_frame=[[TASK_FRAME_ENTER]]
  // THREADS-NOT: {{^}}[[THREAD_ID]]: ompt_event_implicit_task_end
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_barrier_begin: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]]
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_barrier_end: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]]
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_implicit_task_end: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]]

  // THREADS: {{^}}[[THREAD_ID:[0-9]+]]: ompt_event_implicit_task_begin: parallel_id=[[NESTED_PARALLEL_ID:[0-9]+]], task_id=[[IMPLICIT_TASK_ID:[0-9]+]]
  // THREADS: {{^}}[[THREAD_ID]]: level 0: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]], exit_frame={{0x[0-f]+}}, reenter_frame=[[NULL]]
  // can't reliably tell which parallel region is the parent...
  // THREADS: {{^}}[[THREAD_ID]]: level 1: parallel_id={{[0-9]+}}, task_id={{[0-9]+}}
  // THREADS: {{^}}[[THREAD_ID]]: level 2: parallel_id={{[0-9]+}}, task_id={{[0-9]+}}
  // THREADS: {{^}}[[THREAD_ID]]: level 3: parallel_id=0, task_id=[[PARENT_TASK_ID]], exit_frame=[[NULL]], reenter_frame=[[TASK_FRAME_ENTER]]
  // THREADS-NOT: {{^}}[[THREAD_ID]]: ompt_event_implicit_task_end
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_barrier_begin: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]]
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_barrier_end: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]]
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_implicit_task_end: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]]

  // THREADS: {{^}}[[THREAD_ID:[0-9]+]]: ompt_event_implicit_task_begin: parallel_id=[[NESTED_PARALLEL_ID:[0-9]+]], task_id=[[IMPLICIT_TASK_ID:[0-9]+]]
  // THREADS: {{^}}[[THREAD_ID]]: level 0: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]], exit_frame={{0x[0-f]+}}, reenter_frame=[[NULL]]
  // can't reliably tell which parallel region is the parent...
  // THREADS: {{^}}[[THREAD_ID]]: level 1: parallel_id={{[0-9]+}}, task_id={{[0-9]+}}
  // THREADS: {{^}}[[THREAD_ID]]: level 2: parallel_id={{[0-9]+}}, task_id={{[0-9]+}}
  // THREADS: {{^}}[[THREAD_ID]]: level 3: parallel_id=0, task_id=[[PARENT_TASK_ID]], exit_frame=[[NULL]], reenter_frame=[[TASK_FRAME_ENTER]]
  // THREADS-NOT: {{^}}[[THREAD_ID]]: ompt_event_implicit_task_end
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_barrier_begin: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]]
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_barrier_end: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]]
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_implicit_task_end: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]]

  // THREADS: {{^}}[[THREAD_ID:[0-9]+]]: ompt_event_implicit_task_begin: parallel_id=[[NESTED_PARALLEL_ID:[0-9]+]], task_id=[[IMPLICIT_TASK_ID:[0-9]+]]
  // THREADS: {{^}}[[THREAD_ID]]: level 0: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]], exit_frame={{0x[0-f]+}}, reenter_frame=[[NULL]]
  // can't reliably tell which parallel region is the parent...
  // THREADS: {{^}}[[THREAD_ID]]: level 1: parallel_id={{[0-9]+}}, task_id={{[0-9]+}}
  // THREADS: {{^}}[[THREAD_ID]]: level 2: parallel_id={{[0-9]+}}, task_id={{[0-9]+}}
  // THREADS: {{^}}[[THREAD_ID]]: level 3: parallel_id=0, task_id=[[PARENT_TASK_ID]], exit_frame=[[NULL]], reenter_frame=[[TASK_FRAME_ENTER]]
  // THREADS-NOT: {{^}}[[THREAD_ID]]: ompt_event_implicit_task_end
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_barrier_begin: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]]
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_barrier_end: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]]
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_implicit_task_end: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]]

  // THREADS: {{^}}[[THREAD_ID:[0-9]+]]: ompt_event_implicit_task_begin: parallel_id=[[NESTED_PARALLEL_ID:[0-9]+]], task_id=[[IMPLICIT_TASK_ID:[0-9]+]]
  // THREADS: {{^}}[[THREAD_ID]]: level 0: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]], exit_frame={{0x[0-f]+}}, reenter_frame=[[NULL]]
  // can't reliably tell which parallel region is the parent...
  // THREADS: {{^}}[[THREAD_ID]]: level 1: parallel_id={{[0-9]+}}, task_id={{[0-9]+}}
  // THREADS: {{^}}[[THREAD_ID]]: level 2: parallel_id={{[0-9]+}}, task_id={{[0-9]+}}
  // THREADS: {{^}}[[THREAD_ID]]: level 3: parallel_id=0, task_id=[[PARENT_TASK_ID]], exit_frame=[[NULL]], reenter_frame=[[TASK_FRAME_ENTER]]
  // THREADS-NOT: {{^}}[[THREAD_ID]]: ompt_event_implicit_task_end
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_barrier_begin: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]]
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_barrier_end: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]]
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_implicit_task_end: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]]

  // THREADS: {{^}}[[THREAD_ID:[0-9]+]]: ompt_event_implicit_task_begin: parallel_id=[[NESTED_PARALLEL_ID:[0-9]+]], task_id=[[IMPLICIT_TASK_ID:[0-9]+]]
  // THREADS: {{^}}[[THREAD_ID]]: level 0: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]], exit_frame={{0x[0-f]+}}, reenter_frame=[[NULL]]
  // can't reliably tell which parallel region is the parent...
  // THREADS: {{^}}[[THREAD_ID]]: level 1: parallel_id={{[0-9]+}}, task_id={{[0-9]+}}
  // THREADS: {{^}}[[THREAD_ID]]: level 2: parallel_id={{[0-9]+}}, task_id={{[0-9]+}}
  // THREADS: {{^}}[[THREAD_ID]]: level 3: parallel_id=0, task_id=[[PARENT_TASK_ID]], exit_frame=[[NULL]], reenter_frame=[[TASK_FRAME_ENTER]]
  // THREADS-NOT: {{^}}[[THREAD_ID]]: ompt_event_implicit_task_end
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_barrier_begin: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]]
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_barrier_end: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]]
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_implicit_task_end: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]]

  // THREADS: {{^}}[[THREAD_ID:[0-9]+]]: ompt_event_implicit_task_begin: parallel_id=[[NESTED_PARALLEL_ID:[0-9]+]], task_id=[[IMPLICIT_TASK_ID:[0-9]+]]
  // THREADS: {{^}}[[THREAD_ID]]: level 0: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]], exit_frame={{0x[0-f]+}}, reenter_frame=[[NULL]]
  // can't reliably tell which parallel region is the parent...
  // THREADS: {{^}}[[THREAD_ID]]: level 1: parallel_id={{[0-9]+}}, task_id={{[0-9]+}}
  // THREADS: {{^}}[[THREAD_ID]]: level 2: parallel_id={{[0-9]+}}, task_id={{[0-9]+}}
  // THREADS: {{^}}[[THREAD_ID]]: level 3: parallel_id=0, task_id=[[PARENT_TASK_ID]], exit_frame=[[NULL]], reenter_frame=[[TASK_FRAME_ENTER]]
  // THREADS-NOT: {{^}}[[THREAD_ID]]: ompt_event_implicit_task_end
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_barrier_begin: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]]
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_barrier_end: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]]
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_implicit_task_end: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]]

  // THREADS: {{^}}[[THREAD_ID:[0-9]+]]: ompt_event_implicit_task_begin: parallel_id=[[NESTED_PARALLEL_ID:[0-9]+]], task_id=[[IMPLICIT_TASK_ID:[0-9]+]]
  // THREADS: {{^}}[[THREAD_ID]]: level 0: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]], exit_frame={{0x[0-f]+}}, reenter_frame=[[NULL]]
  // can't reliably tell which parallel region is the parent...
  // THREADS: {{^}}[[THREAD_ID]]: level 1: parallel_id={{[0-9]+}}, task_id={{[0-9]+}}
  // THREADS: {{^}}[[THREAD_ID]]: level 2: parallel_id={{[0-9]+}}, task_id={{[0-9]+}}
  // THREADS: {{^}}[[THREAD_ID]]: level 3: parallel_id=0, task_id=[[PARENT_TASK_ID]], exit_frame=[[NULL]], reenter_frame=[[TASK_FRAME_ENTER]]
  // THREADS-NOT: {{^}}[[THREAD_ID]]: ompt_event_implicit_task_end
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_barrier_begin: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]]
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_barrier_end: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]]
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_implicit_task_end: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]]

  // THREADS: {{^}}[[THREAD_ID:[0-9]+]]: ompt_event_implicit_task_begin: parallel_id=[[NESTED_PARALLEL_ID:[0-9]+]], task_id=[[IMPLICIT_TASK_ID:[0-9]+]]
  // THREADS: {{^}}[[THREAD_ID]]: level 0: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]], exit_frame={{0x[0-f]+}}, reenter_frame=[[NULL]]
  // can't reliably tell which parallel region is the parent...
  // THREADS: {{^}}[[THREAD_ID]]: level 1: parallel_id={{[0-9]+}}, task_id={{[0-9]+}}
  // THREADS: {{^}}[[THREAD_ID]]: level 2: parallel_id={{[0-9]+}}, task_id={{[0-9]+}}
  // THREADS: {{^}}[[THREAD_ID]]: level 3: parallel_id=0, task_id=[[PARENT_TASK_ID]], exit_frame=[[NULL]], reenter_frame=[[TASK_FRAME_ENTER]]
  // THREADS-NOT: {{^}}[[THREAD_ID]]: ompt_event_implicit_task_end
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_barrier_begin: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]]
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_barrier_end: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]]
  // THREADS: {{^}}[[THREAD_ID]]: ompt_event_implicit_task_end: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]]


  return 0;
}
