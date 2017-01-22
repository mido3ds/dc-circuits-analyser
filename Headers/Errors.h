/*
    Different errors that may occur
    and their handling messages and functions.

        used in exception handling.
*/
#pragma once
#include "Colors.h"
#include <cassert>
#include <iostream>
using namespace std;
#define FOR_DEBUGGING 0


// identify different errors that might happen
enum error {
    SAME_POLARITY, 
    DUPLICATE_WITH_DIFF_VALUES, // TODO: see why this is not going to appear
    DUPLICATE_ELEMENT, 
    BAD_TYPE_NAME, 
    NEGATIVE_RESISTANCE, 
    INVALID_STORED_TYPE, // when a type is stored against rules, happens when accessing invalid memory
    LONELY_ELEMENT,
    DEREF_NULL_PTR,
    DEL_ELEMENT_FROM_WRONG_NODE,
    NODE_ID_IN_ELEM_UNASSIGNED,
    INVALID_NODE_ID,
    INVALID_INPUT
};

// strings to handle errors
#define HANDLE_SAME_POLARITY RED "\tTheir is two source elements with the same polarity, deleting the last one\n" WHITE
#define HANDLE_DUPLICATE_ELEMENT RED "\tDuplicate element, deleting it\n" WHITE
#define HANDLE_DUPLICATE_WITH_DIFF_VALUES RED "\tFound two elements having same ID and differnt values, deleting the last one\n" WHITE
#define HANDLE_BAD_TYPE_NAME RED "\tInvalid Type, please re-enter it\n" WHITE
#define HANDLE_NEGATIVE_RESISTANCE RED "\tResistance can't be negative, deleted it\n" WHITE
#define HANDLE_EMPTY_NODE RED "\tLast node is empty, deleting it\n" WHITE
#define HANDLE_NODE_WITH_ONE_ELEM RED "\tFound node with one element, deleting the node and the element\n" WHITE
#define HANDLE_INVALID_INPUT RED "\tInvalid input, type h to see valid commands\n" WHITE

// print the appropriate message for the error 
// with color RED if colors are enabled
// if exception is not listed, it asserts for debugging printing that error is not handled
void HandleError(const error& err);