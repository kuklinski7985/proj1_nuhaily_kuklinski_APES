/**
 *  @file hw2n7.h
 *  @author Adam Nuhaily
 *  @date 3 Feb 2018
 *  @brief hw2 problem 7 header file
 *  This source file implements cmocka unit tests for the linked list code
 *  provided in the linkedlists/ folder.
 *  With the accompanying makefile, type "make" and then "make unittests" to
 *  build unittests.elf for execution of unit tests.
 */

#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <cmocka.h>
#include "main.h"

static void test_counts_to_lux(void** state);
static void test_display_c(void** state);
static void test_display_f(void** state);
static void test_manage_ipc_msg(void** state);
