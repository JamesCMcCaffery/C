// James McCaffery | | Systems Programming AE1 | Binary Search Tree
// 2769929M

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct node_t {
	int value;
	struct node_t * parent;
	struct node_t * right;
	struct node_t * left;
} node_t;

node_t * root;

// HELPER FUNCTIONS //
// FIND SMALLEST NODE IN A TREE (used in deletion when finding smallest in right subtree)
node_t * find_smallest_node(node_t * node) {
    node_t * currentNode = node;
    while(currentNode->left != NULL) {
        currentNode = currentNode->left;
    }
    return currentNode;
}

// REPLACE CHILD (also used in deletion) 
void replace_child(node_t *parent, node_t *oldChild, node_t *newChild) {
	// if the old child's parent is null, we are deleting the root, so set global variable to new child
	if (parent == NULL) {
		root = newChild;
	} else {
		if (parent->left == oldChild) {
			parent->left = newChild;
		} else {
			parent->right = newChild;
		}
	}
	if (newChild != NULL) {
		newChild->parent = parent;
	}
}

// SEARCH FOR NODE //
node_t * search(node_t * node, int toFind) {
	node_t * currentNode = node;
	while(currentNode != NULL) {
		// if root element is what we are looking for
		if(toFind == currentNode->value) {
			return currentNode;
		}

		// go left
		else if(toFind < currentNode->value) {
			// we want to go left, but there is nothing there, so presumably our element does not exist
			if (currentNode->left == NULL) {
				return NULL;
			}

			// we want to go left and it DOES exist, so go to that node and continue loop
			else {
				currentNode = currentNode->left;
			}
		}

		// go right
		else if(toFind > currentNode->value) {
			// we want to go right, but there is nothing there, so presumably our element does not exist
			if(currentNode->right == NULL) {
				return NULL;
			}

			// we want to go right, and it does EXIST, so go
			else {
				currentNode = currentNode->right;
			}
		}
	}
	return NULL;
}

// DELETE NODE //
void delete(node_t * node, int elem) {
	node_t *nodeToDelete = search(node, elem);
	bool hasLeftNode = (nodeToDelete->left != NULL);
	bool hasRightNode = (nodeToDelete->right != NULL);

	// if it is a leaf node
	if (!hasLeftNode && !hasRightNode) {
		replace_child(nodeToDelete->parent, nodeToDelete, NULL);
		free(nodeToDelete);
		return;
	}

	// if it only has a left child
	if (hasLeftNode && !hasRightNode) {
		replace_child(nodeToDelete->parent, nodeToDelete, nodeToDelete->left);
		free(nodeToDelete);
		return;
	}

	// if it only has a right child
	if (!hasLeftNode && hasRightNode) {
		replace_child(nodeToDelete->parent, nodeToDelete, nodeToDelete->right);
		free(nodeToDelete);
		return;
	}

	// if it has two children, we find the successor by locating the smallest (leftmost) node in the right subtree
	if (hasLeftNode && hasRightNode) {
		node_t * successor = find_smallest_node(nodeToDelete->right);

		// remove successor node from parent and replace with it's right child (we can guarantee there won't be a left child, as successor is the leftmost node)
		if (successor->parent != nodeToDelete) {
			replace_child(successor->parent, successor, successor->right);

			successor->right = nodeToDelete->right;
			successor->right->parent = successor;
		}

		// now do the actual replacement
		replace_child(nodeToDelete->parent, nodeToDelete, successor);

		// update children and parents
		successor->left = nodeToDelete->left;
		successor->left->parent = successor;

		free(nodeToDelete);
		return;
	}
}

// CREATE NEW TREE //
node_t * createTree(int firstElem) {
	node_t *newNode  = (node_t *)malloc(sizeof(node_t));
	if (newNode){
	 	newNode -> value = firstElem;
    	newNode-> parent = NULL;
    	newNode -> right = NULL;
    	newNode -> left = NULL;
    	root = newNode;
    	return newNode;   
	} else return NULL;
    
}

// DESTROY TREE //
void destroyTree(node_t * node) {
    if (node == NULL) {
        return;
    }
    
    // post-order traversal for memory safety; detailed in report 
    destroyTree(node->left);
    destroyTree(node->right);
    delete(node, node->value);
}


// INSERT NODE //
void insert(node_t * node, int elem) {
	node_t * currentNode = node;
	while(currentNode!= NULL) {
		// if current element is what we want to insert, meaning duplicate, so return
		if(elem == currentNode->value) {
			return;
		}


		// if elem we want to insert is less than current element, go left
		else if(elem < currentNode->value) {
			if(currentNode->left==NULL) {
				struct node_t * newNode = (node_t*)malloc(sizeof(node_t));
				if(newNode){
				    newNode->value = elem;
    				newNode->parent = currentNode;
    				newNode->right = NULL;
    				newNode->left = NULL;
    				currentNode->left = newNode;
    				return;
				} else return; 
			} else {
				currentNode = currentNode->left;
			}
		}

		// if element we want to insert is bigger, go right
		else if(elem > currentNode->value) {
			if(currentNode->right==NULL) {
				node_t * newNode = (node_t*)malloc(sizeof(node_t));
				if(newNode){
				    newNode->value = elem;
    				newNode->parent = currentNode;
    				newNode->right = NULL;
    				newNode->left = NULL;
    				currentNode->right = newNode;
    				return;
				} else return;
			} else {
				currentNode = currentNode->right;
			}
		}

	}

}
