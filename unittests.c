/**
 *  @file hw2n7.c
 *  @author Adam Nuhaily
 *  @date 3 Feb 2018
 *  @brief hw2 problem 7 source file
 *  This source file implements cmocka unit tests for the linked list code
 *  provided in the linkedlists/ folder.
 *  With the accompanying makefile, type "make" and then "make unittests" to
 *  build unittests.elf for execution of unit tests.
 */

#include "unittests.h"

extern ipcmessage_t ipc_msg;

static void test_counts_to_lux(void** state)
{
  int pass_count = 0;
  assert_int_equal(pass_count, 3);
}

static void test_display_c(void** state)
{
  int pass_count = 0;

  assert_int_equal(pass_count, 3);
}

static void test_display_f(void** state)
{
  int pass_count = 0;

  assert_int_equal(pass_count, 3);
}

static void test_display_k(void** state)
{
  int pass_count = 0;

  assert_int_equal(pass_count, 3);
}

static void test_manage_ipc_msg(void** state)
{
  char log_str[256];
  char test_str[256] = "11:11:11 > \n3\n100\n4\nTEST";
  ipc_queue_init();
  strcpy(ipc_msg.timestamp, "11:11:11 > ");
  ipc_msg.source = IPC_MAIN;
  ipc_msg.src_pid = 100;
  ipc_msg.destination = IPC_USER;
  strcpy(ipc_msg.payload, "TEST");

  manage_ipc_msg(ipc_msg, log_str);
  //shuffler_king();

  assert_int_equal(strcmp(log_str, test_str), 0);
}

int main()
{
  const struct CMUnitTest prj1_tests[] =
  {
    cmocka_unit_test(test_display_c),
    cmocka_unit_test(test_display_f),
    cmocka_unit_test(test_display_k),
    cmocka_unit_test(test_manage_ipc_msg),
  };

  return cmocka_run_group_tests(prj1_tests, NULL, NULL);
}
