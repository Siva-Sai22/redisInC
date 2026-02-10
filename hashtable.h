#pragma once

#include <cstddef>
#include <cstdint>

// Hashtable node, should be embedded into the payload
struct HNode {
	HNode *next = NULL;
	uint64_t hcode = 0;
};

// A simple resizable Hashtable
struct HTab {
	HNode **tab = NULL; // Array of slots
	size_t mask = 0;	// 2^n - 1
	size_t size = 0;	// Number of keys
};

// Real Hashtable Interface
// 2 Hashtables for progressive rehashing
struct HMap {
	HTab newer;
	HTab older;
	size_t migrate_pos = 0;
};

HNode *hm_lookup(HMap *hmap, HNode *key, bool (*eq)(HNode *, HNode *));
void hm_insert(HMap *hmap, HNode *key);
HNode *hm_delete(HMap *hmap, HNode *key, bool (*eq)(HNode *, HNode *));
void hm_clear(HMap *hmap);
size_t hm_size(HMap *hmap);
// Invoke callback on each node until it returns false
void hm_foreach(HMap *hmap, bool (*f)(HNode *, void *), void *arg);
