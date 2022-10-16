#include "Cursor.h"

#include "table.h"

void Cursor::cursor_advance() {
    // uint32_t right_page_num = page_num;
    // if (!page_num) {
    //     right_page_num = 1;
    // }
    void* node = table->get_page(page_num);
    cell_num += 1;
    if (cell_num >= (*leaf_node_num_cells(node))) {
        /* Advance to next leaf node */
        uint32_t next_page_num = *leaf_node_next_leaf(node);
        if (next_page_num == 0) {
            /* This was rightmost leaf */
            end_of_table = true;
        } else {
            page_num = next_page_num;
            cell_num = 0;
        }
    }
}
void* Cursor::cursor_value() {
    void* page = table->get_page(page_num);

    return leaf_node_value(page, cell_num);
}

void Cursor::leaf_node_insert(uint32_t key, Row* value) {
    void* node = table->get_page(page_num);

    uint32_t num_cells = *leaf_node_num_cells(node);
    if (num_cells >= LEAF_NODE_MAX_CELLS) {
        // Node full
        leaf_node_split_and_insert(key, value);
        return;
    }

    if (cell_num < num_cells) {
        // Make room for new cell
        for (uint32_t i = num_cells; i > cell_num; i--) {
            memcpy(leaf_node_cell(node, i),
                   leaf_node_cell(node, i - 1),
                   LEAF_NODE_CELL_SIZE);
        }
    }

    *(leaf_node_num_cells(node)) += 1;
    *(leaf_node_key(node, cell_num)) = key;
    serialize_row(value, leaf_node_value(node, cell_num));
}

void Cursor::leaf_node_find(uint32_t p_num, uint32_t key) {
    void* node = table->get_page(p_num);
    uint32_t num_cells = *leaf_node_num_cells(node);

    page_num = p_num;

    // Binary search
    uint32_t min_index = 0;
    uint32_t one_past_max_index = num_cells;

    while (one_past_max_index != min_index) {
        uint32_t index = (min_index + one_past_max_index) / 2;
        uint32_t key_at_index = *leaf_node_key(node, index);
        if (key == key_at_index) {
            cell_num = index;
            return;
        }
        if (key < key_at_index) {
            one_past_max_index = index;
        } else {
            min_index = index + 1;
        }
    }

    cell_num = min_index;
}

void Cursor::leaf_node_split_and_insert(uint32_t key, Row* value) {
    /**
        Create a new node and move half the cells over.
        Insert the new value in one of the two nodes.
        Update parent or create a new parent.
    */

    void* old_node = table->get_page(page_num);
    uint32_t old_max = get_node_max_key(old_node);
    uint32_t new_page_num = get_unused_page_num(table->pager);
    void* new_node = table->get_page(new_page_num);
    initialize_leaf_node(new_node);
    *node_parent(new_node) = *node_parent(old_node);
    *leaf_node_next_leaf(new_node) = *leaf_node_next_leaf(old_node);
    *leaf_node_next_leaf(old_node) = new_page_num;

    /*
        All existing keys plus new key should be divided
        evenly between old (left) and new (right) nodes.
        Starting from the right, move each key to correct position.
    */
    for (int32_t i = LEAF_NODE_MAX_CELLS; i >= 0; i--) {
        void* destination_node;
        if (i >= LEAF_NODE_LEFT_SPLIT_COUNT) {
            destination_node = new_node;
        } else {
            destination_node = old_node;
        }
        uint32_t index_within_node = i % LEAF_NODE_LEFT_SPLIT_COUNT;
        void* destination = leaf_node_cell(destination_node,
                                           index_within_node);

        if (i == cell_num) {
            serialize_row(value,
                          leaf_node_value(destination_node, index_within_node));
            *leaf_node_key(destination_node, index_within_node) = key;
        } else if (i > cell_num) {
            memcpy(destination,
                   leaf_node_cell(old_node, i - 1),
                   LEAF_NODE_CELL_SIZE);
        } else {
            memcpy(destination,
                   leaf_node_cell(old_node, i),
                   LEAF_NODE_CELL_SIZE);
        }
    }

    /* Update cell count on both leaf nodes */
    *(leaf_node_num_cells(old_node)) = LEAF_NODE_LEFT_SPLIT_COUNT;
    *(leaf_node_num_cells(new_node)) = LEAF_NODE_RIGHT_SPLIT_COUNT;

    if (is_node_root(old_node)) {
        return create_new_root(new_page_num);
    } else {
        uint32_t parent_page_num = *node_parent(old_node);
        uint32_t new_max = get_node_max_key(old_node);
        void* parent = table->get_page(parent_page_num);

        update_internal_node_key(parent, old_max, new_max);
        internal_node_insert(parent_page_num, new_page_num);
        return;
    }
}

void Cursor::create_new_root(uint32_t right_child_page_num) {
    /*
        Handle splitting the root.
        Old root copied to new page, becomes left child.
        Address of right child passed in.
        Re-initialize root page to contain the new root node.
        New root node points to two children.
    */

    void* root = table->get_page(table->root_page_num);
    void* right_child = table->get_page(right_child_page_num);
    uint32_t left_child_page_num = get_unused_page_num(table->pager);
    void* left_child = table->get_page(left_child_page_num);

    /* Left child has data copied from old root */
    memcpy(left_child, root, PAGE_SIZE);
    set_node_root(left_child, false);

    /* Root node is a new internal node with one key and two children */
    initialize_internal_node(root);
    set_node_root(root, true);
    *internal_node_num_keys(root) = 1;
    *internal_node_child(root, 0) = left_child_page_num;
    uint32_t left_child_max_key = get_node_max_key(left_child);
    *internal_node_key(root, 0) = left_child_max_key;
    *internal_node_right_child(root) = right_child_page_num;
    *node_parent(left_child) = table->root_page_num;
    *node_parent(right_child) = table->root_page_num;
}

void Cursor::internal_node_find(uint32_t p_num, uint32_t key) {
    void* node = table->get_page(p_num);

    uint32_t child_index = internal_node_find_child(node, key);
    uint32_t child_num = *internal_node_child(node, child_index);
    void* child = table->get_page(child_num);
    switch (get_node_type(child)) {
        case NODE_LEAF:
            return leaf_node_find(child_num, key);
        case NODE_INTERNAL:
            return internal_node_find(child_num, key);
    }
}

uint32_t get_unused_page_num(const Pager& pager) {
    return pager.num_pages;
}

uint32_t* node_parent(void* node) {
    return (uint32_t*)(node + PARENT_POINTER_OFFSET);
}

void update_internal_node_key(void* node, uint32_t old_key, uint32_t new_key) {
    uint32_t old_child_index = internal_node_find_child(node, old_key);
    *internal_node_key(node, old_child_index) = new_key;
}

void Cursor::internal_node_insert(uint32_t parent_page_num,
                                  uint32_t child_page_num) {
    /*
    Add a new child/key pair to parent that corresponds to child
    */

    void* parent = table->get_page(parent_page_num);
    void* child = table->get_page(child_page_num);
    uint32_t child_max_key = get_node_max_key(child);
    uint32_t index = internal_node_find_child(parent, child_max_key);

    uint32_t original_num_keys = *internal_node_num_keys(parent);
    *internal_node_num_keys(parent) = original_num_keys + 1;

    if (original_num_keys >= INTERNAL_NODE_MAX_CELLS) {
        printf("Need to implement splitting internal node\n");
        exit(EXIT_FAILURE);
    }
    uint32_t right_child_page_num = *internal_node_right_child(parent);
    void* right_child = table->get_page(right_child_page_num);

    if (child_max_key > get_node_max_key(right_child)) {
        /* Replace right child */
        *internal_node_child(parent, original_num_keys) = right_child_page_num;
        *internal_node_key(parent, original_num_keys) =
            get_node_max_key(right_child);
        *internal_node_right_child(parent) = child_page_num;
    } else {
        /* Make room for the new cell */
        for (uint32_t i = original_num_keys; i > index; i--) {
            void* destination = internal_node_cell(parent, i);
            void* source = internal_node_cell(parent, i - 1);
            memcpy(destination, source, INTERNAL_NODE_CELL_SIZE);
        }
        *internal_node_child(parent, index) = child_page_num;
        *internal_node_key(parent, index) = child_max_key;
    }
}