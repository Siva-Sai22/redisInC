#include "zset.h"
#include "avl.h"
#include "hashtable.h"
#include "utils.h"
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>

static ZNode *znode_new(const char *name, size_t len, double score) {
	ZNode *node = (ZNode *)malloc(sizeof(ZNode) + len);
	assert(node); // Add proper error handling

	avl_init(&node->tree);
	node->hmap.next = NULL;
	node->hmap.hcode = str_hash((uint8_t *)name, len);
	node->score = score;
	node->len = len;

	memcpy(&node->name[0], name, len);
	return node;
}

static void znode_del(ZNode *node) { free(node); }

// Compare by the (score, name) tuple
static bool zless(AVLNode *lhs, double score, const char *name, size_t len) {
	ZNode *zl = container_of(lhs, ZNode, tree);
	if (zl->score != score) {
		return zl->score < score;
	}

	int rv = memcmp(zl->name, name, std::min(zl->len, len));
	if (rv != 0) {
		return rv < 0;
	}

	return zl->len < len;
}

static bool zless(AVLNode *lhs, AVLNode *rhs) {
	ZNode *zr = container_of(rhs, ZNode, tree);
	return zless(lhs, zr->score, zr->name, zr->len);
}

// Insert into avl tree
static void tree_insert(ZSet *zset, ZNode *node) {
	AVLNode *parent = NULL;		  // insert under this node
	AVLNode **from = &zset->root; // the incoming pointer to the next node
	while (*from) {				  // tree search
		parent = *from;
		from = zless(&node->tree, parent) ? &parent->left : &parent->right;
	}

	*from = &node->tree;
	node->tree.parent = parent;
	zset->root = avl_fix(&node->tree);
}

// Update the score of an existing node
static void zset_update(ZSet *zset, ZNode *node, double score) {
	if (node->score == score) {
		return;
	}

	// detach the tree node
	zset->root = avl_del(&node->tree);
	avl_init(&node->tree);

	// reinsert the tree node
	node->score = score;
	tree_insert(zset, node);
}

// Add new (score, name) tuple, or update existing one
bool zset_insert(ZSet *zset, const char *name, size_t len, double score) {
	ZNode *node = zset_lookup(zset, name, len);
	if (node) {
		zset_update(zset, node, score);
		return false;
	} else {
		node = znode_new(name, len, score);
		hm_insert(&zset->hmap, &node->hmap);
		tree_insert(zset, node);
		return true;
	}
}

// Helper struct for the hashtable lookup
struct HKey {
	HNode node;
	const char *name = NULL;
	size_t len = 0;
};

static bool hcmp(HNode *node, HNode *key) {
	ZNode *znode = container_of(node, ZNode, hmap);
	HKey *hkey = container_of(key, HKey, node);
	if (znode->len != hkey->len) {
		return false;
	}

	return 0 == memcmp(znode->name, hkey->name, znode->len);
}

// Lookup by name
ZNode *zset_lookup(ZSet *zset, const char *name, size_t len) {
	if (!zset->root) {
		return NULL;
	}

	HKey key;
	key.node.hcode = str_hash((uint8_t *)name, len);
	key.name = name;
	key.len = len;

	HNode *found = hm_lookup(&zset->hmap, &key.node, &hcmp);
	return found ? container_of(found, ZNode, hmap) : NULL;
}

// Delete a node
void zset_delete(ZSet *zset, ZNode *node) {
	// From hashtable
	HKey key;
	key.node.hcode = node->hmap.hcode;
	key.name = node->name;
	key.len = node->len;
	HNode *found = hm_delete(&zset->hmap, &key.node, &hcmp);
	assert(found);

	// From tree
	zset->root = avl_del(&node->tree);

	znode_del(node);
}

// Find the first (score, name) tuple that is >= key
ZNode *zset_seekge(ZSet *zset, double score, const char *name, size_t len) {
	AVLNode *found = NULL;
	for (AVLNode *node = zset->root; node;) {
		if (zless(node, score, name, len)) {
			node = node->right; // node < key
		} else {
			found = node; // candidate
			node = node->left;
		}
	}
	return found ? container_of(found, ZNode, tree) : NULL;
}

// offset into the succeeding or preceding node
ZNode *znode_offset(ZNode *node, int64_t offset) {
	AVLNode *tnode = node ? avl_offset(&node->tree, offset) : NULL;
	return tnode ? container_of(tnode, ZNode, tree) : NULL;
}

static void tree_dispose(AVLNode *node) {
	if (!node) {
		return;
	}

	tree_dispose(node->left);
	tree_dispose(node->right);
	znode_del(container_of(node, ZNode, tree));
}

// destroy the zset
void zset_clear(ZSet *zset) {
	hm_clear(&zset->hmap);
	tree_dispose(zset->root);
	zset->root = NULL;
}
