#ifndef INTERNALNODE_H
#define INTERNALNODE_H
/*
 * Internal Node Header Layout
 */
#include <iostream>

#include "leafNode.h"

const uint32_t INTERNAL_NODE_NUM_KEYS_SIZE = sizeof(uint32_t);
const uint32_t INTERNAL_NODE_NUM_KEYS_OFFSET = COMMON_NODE_HEADER_SIZE;
const uint32_t INTERNAL_NODE_RIGHT_CHILD_SIZE = sizeof(uint32_t);
const uint32_t INTERNAL_NODE_RIGHT_CHILD_OFFSET =
    INTERNAL_NODE_NUM_KEYS_OFFSET + INTERNAL_NODE_NUM_KEYS_SIZE;
const uint32_t INTERNAL_NODE_HEADER_SIZE = COMMON_NODE_HEADER_SIZE +
                                           INTERNAL_NODE_NUM_KEYS_SIZE +
                                           INTERNAL_NODE_RIGHT_CHILD_SIZE;

/*
 * Internal Node Body Layout
 */
const uint32_t INTERNAL_NODE_KEY_SIZE = sizeof(uint32_t);
const uint32_t INTERNAL_NODE_CHILD_SIZE = sizeof(uint32_t);
const uint32_t INTERNAL_NODE_CELL_SIZE =
    INTERNAL_NODE_CHILD_SIZE + INTERNAL_NODE_KEY_SIZE;

const uint32_t INTERNAL_NODE_MAX_CELLS = 3;

uint32_t* internal_node_num_keys(void* node);

uint32_t* internal_node_right_child(void* node);

uint32_t* internal_node_cell(void* node, uint32_t cell_num);

uint32_t* internal_node_child(void* node, uint32_t child_num);

uint32_t* internal_node_key(void* node, uint32_t key_num);

uint32_t get_node_max_key(void* node);

void initialize_internal_node(void* node);

uint32_t internal_node_find_child(void* node, uint32_t key);

#endif