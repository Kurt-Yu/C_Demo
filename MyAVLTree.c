#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

// all the basic data structures and functions are included in this template
// you can add your own auxiliary functions as you like 

// data type for avl tree nodes
typedef struct AVLTreeNode {
	int key; //key of this item
	int  value;  //value (int) of this item 
	int height; //height of the subtree rooted at this node
	int balanceFactor;
	struct AVLTreeNode *parent; //pointer to parent
	struct AVLTreeNode *left; //pointer to left child
	struct AVLTreeNode *right; //pointer to right child
} AVLTreeNode;

//data type for AVL trees
typedef struct AVLTree{
	int  size;      // count of items in avl tree
	AVLTreeNode *root; // root
} AVLTree;

// create a new AVLTreeNode
AVLTreeNode *newAVLTreeNode(int k, int v ){
	AVLTreeNode *new;
	new = malloc(sizeof(AVLTreeNode));
	assert(new != NULL);
	new->key = k;
	new->value = v;
	new->height = 0; // height of this new node is set to 0
	new->left = NULL; // this node has no child
	new->right = NULL;
	new->parent = NULL; // no parent
	return new;
}

// create a new empty avl tree
AVLTree *newAVLTree(){
	AVLTree *T;
	T = malloc(sizeof (AVLTree));
	assert (T != NULL);
	T->size = 0;
	T->root = NULL;
	return T;
}

// put your time complexity analysis of CreateAVLTree() here

static int max(int a,int b){
    return a > b ? a : b;
}

static void updateNode(AVLTreeNode * node){
    int leftHeight;
    int rightHeight;
    if(node->right != NULL){
        rightHeight = node->right->height;
    }
    else{
        rightHeight = -1;
    }
    if(node->left != NULL){
        leftHeight = node->left->height;
    }
    else{
        leftHeight = -1;
    }
    /*The current node's height is equal to the height of its children plus one.*/
    node->height = max(leftHeight,rightHeight) + 1;
    node->balanceFactor = rightHeight - leftHeight;
}

static AVLTreeNode *rebalanceRR(AVLTreeNode *node){
    AVLTreeNode *newSubroot = node->right;
    node->right = newSubroot->left;
    newSubroot->left = node;
    /*Updating node and subroot for which heights and balance factors may have changed.*/
    updateNode(node);
    updateNode(newSubroot);
    return newSubroot;
}

static AVLTreeNode *rebalanceLL(AVLTreeNode *node){
    AVLTreeNode *newSubroot = node->left;
    node->left = newSubroot->right;
    newSubroot->right = node;
    /*Updating node positions that may have changed.*/
    updateNode(node);
    updateNode(newSubroot);
    return newSubroot;
}

static AVLTreeNode *rebalanceRL(AVLTreeNode *node){
    /*Retrieving the right node and the new subroot, the right's left child.*/
    AVLTreeNode *right = node->right;
    AVLTreeNode *newSubroot = right->left;
    /*Setting up the new children of the subroot*/
    right->left = newSubroot->right;
    node->right = newSubroot->left;
    /*Now getting the subroot to point to its new children.*/
    newSubroot->right = right;
    newSubroot->left = node;
    /*Updating the nodes since their height and balance may have changed.*/
    updateNode(node);
    updateNode(right);
    updateNode(newSubroot);
    return newSubroot;
}

static AVLTreeNode *rebalanceLR(AVLTreeNode *node){
    /*Retrieving the left node and the new subroot, the left's right child.*/
    AVLTreeNode *left = node->left;
    AVLTreeNode *newSubroot = left->right;
    /*Setting up the new children of the subroot.*/
    left->right = newSubroot->left;
    node->left = newSubroot->right;
    /*Now getting the new subroot to point to its children.*/
    newSubroot->left = left;
    newSubroot->right = node;
    /*Updating the subroot and its children since positions may have changed.*/
    updateNode(left);
    updateNode(node);
    updateNode(newSubroot);
    return newSubroot;
}

static AVLTreeNode *addNode(AVLTreeNode *node, int k, int v){
	if(node == NULL){
        node = newAVLTreeNode(k, v);
	}
	else{
		if(k > node->key){
			node->right = addNode(node->right, k, v);
			updateNode(node);
			if(node->balanceFactor > 1){
				if(node->right->balanceFactor > 0){
				node = rebalanceRR(node);
				}
        		else{
					node = rebalanceRL(node);
				}
			}	
		}
		else if (k == node->key ){
			if (v > node->value) {
				node->right = addNode(node->right, k, v);
				updateNode(node);	
			}	
			else if (v == node->value){
				return NULL;
			}
			
			else{
				node->left = addNode(node->left, k, v);
				updateNode(node);	
			}
			
		}
		else{
            node->left = addNode(node->left, k, v);
            updateNode(node);
            if(node->balanceFactor < -1){
                if(node->left->balanceFactor < 0){
                    node = rebalanceLL(node);
                }
                else{
                    node = rebalanceLR(node);
                }
			}
		}
		
	}
	return node;
}




AVLTree *CreateAVLTree(const char *filename) {
	int a, b; 
	AVLTreeNode * root;
	int flag = 0;
	int size = 0;
	
	FILE *file = fopen(filename, "r");
         
    if (! file ) {  
        printf("oops, file can't be read\n"); 
        exit(-1);
	}
	while ( fscanf(file, "%d", &a) != EOF){
		fscanf(file, "(%d,%d)",&a,&b);
		size = size + 1;
		if (flag == 0){
			root = addNode(NULL, a, b);
			flag = 1;
		} 
		else {
			root = addNode(root, a, b);
		} 
    }
	fclose(file);
	AVLTree * res = newAVLTree();
	res->root = root;
	res->size = size;
	return res;
}

// put your time complexity analysis for CloneAVLTree() here
// AVLTree *CloneAVLTree(AVLTree *T)
// {
//  // put your code here
  
// }
 
// // put your time complexity for ALVTreesUNion() here
// AVLTree *AVLTreesUnion(AVLTree *T1, AVLTree *T2)
// {
// 	//put your code here
// }
 
// // put your time complexity for ALVTreesIntersection() here
// AVLTree *AVLTreesIntersection(AVLTree *T1, AVLTree *T2)
// {
// 	 //put your code here
// }

// // put the time complexity analysis for InsertNode() here



// int InsertNode(AVLTree *T, int k, int v)
// {
// 	AVLTreeNode * root;
// 	root = T->root;
// 	AVLTreeNode * res;
// 	res = addNode(root, k, v);
// 	return res == NULL ? 0 : 1;

// }

// put your time complexity for DeleteNode() here

AVLTreeNode * FindMin(AVLTreeNode * node){
	while(node->left != NULL){
		node = node->left;
	}
	return node;
}

// put your time complexity analysis for Search() here
AVLTreeNode * Search_helper(AVLTreeNode * root, int k, int v){
	if(root == NULL) return NULL;
	if(root->key == k){
		if(root->value < v){
			return Search_helper(root->right, k, v);
		}
		else if (root->value == v){
			return root;
		}
		else{
			return Search_helper(root->left, k, v);
		}
	}
	else if (root->key < k){
		return Search_helper(root->right, k, v);
	}
	return Search_helper(root->left, k, v);
}

AVLTreeNode *Search(AVLTree *T, int k, int v){
  AVLTreeNode * root = T->root;
  AVLTreeNode * res = Search_helper(root, k, v);
  return res;
}

AVLTreeNode *Delete_helper(AVLTreeNode * node, int k, int v){
	if (node == NULL) return NULL;
	if (k < node-> key){
		node->left = Delete_helper(node->left, k, v);
	}
	else if (k > node -> key){
		node->right = Delete_helper(node-> right, k, v);
	}
	else{
		if (v < node->value){
			node->left = Delete_helper(node->left, k, v);
		}
		else if (v > node->value){
			node->right = Delete_helper(node->right, k, v);
		}
		else{
			if(node->left == NULL || node->right == NULL){
				AVLTreeNode * temp = node->left ? node->left : node->right;
				if(temp == NULL){
					temp = node;
					node = NULL;
				}
				else *node = *temp;
				free(temp);	
			}
			else{
				AVLTreeNode * minnode = FindMin(node->right);
				node->key = minnode->key;
				node->value = minnode->value;
				node->right = Delete_helper(node->right, node->key, node->value);
			}
		}
	}
	return node;
}

int DeleteNode(AVLTree *T, int k, int v){
	AVLTreeNode *node = Search(T, k, v);
	if (node == NULL) return 0;
	AVLTreeNode * temp = Delete_helper(T->root, k, v);
	T->root = temp;
	return 1;
}



// // put your time complexity analysis for freeAVLTree() here
// static void clearNodes(AVLTreeNode *node){
//     if(node->right != NULL){
//         clearNodes(node->right);
//         free(node->right);
//     }
//     if(node->left != NULL){
//         clearNodes(node->left);
//         free(node->left);
//     }
// }

// void FreeAVLTree(AVLTree *T){
// 	if(T->root == NULL)return;
//     clearNodes(T->root);
//     free(T->root);
//     T->root = NULL;	
// }

// put your time complexity analysis for PrintAVLTree() here
void print_helper(AVLTreeNode * node) {	
	if(node == NULL) return;
	print_helper(node->left);
	printf("(%d,%d),%d \n", node->key, node->value, node->height);
    print_helper(node->right);
}

void PrintAVLTree(AVLTree *T) {
	AVLTreeNode * node;
	node = T -> root;
	print_helper(node);
}

int main() { 
	int i,j;
	AVLTree *tree1, *tree2, *tree3, *tree4, *tree5, *tree6, *tree7, *tree8;
	AVLTreeNode *node1;
	tree2 = CreateAVLTree("file1.txt"); 
	PrintAVLTree(tree2);

	AVLTreeNode *res = Search(tree2, 9, 30);
	printf("Test search function is working: %d, %d\n",res->key,res->value);

	printf("--------------- Test delete: ");
	j = DeleteNode(tree2, 9,45);
	printf("j = %d\n",j);
	printf("The tree after delete: \n");
	PrintAVLTree(tree2);
 
	//  tree1=CreateAVLTree("stdin");
	//  PrintAVLTree(tree1);
	//  FreeAVLTree(tree1);
	//you need to create the text file file1.txt
	// to store a set of items without duplicate items

	//  tree3=CloneAVLTree(tree2);
	//  PrintAVLTree(tree3);
	//  FreeAVLTree(tree2);
	//  FreeAVLTree(tree3);
	//  //Create tree4 
	//  tree4=newAVLTree();
	//  j=InsertNode(tree2, -5, -40);
	//  printf("%d", j);

	//  for (i=0; i<15; i++)
	//   {
	//    j=InsertNode(tree4, i, i);
	//    if (j==0) printf("(%d, %d) already exists\n", i, i);
	//   }
	//   PrintAVLTree(tree4);
	//   node1=Search(tree4,20,20);
	//   if (node1!=NULL)
	//     printf("key= %d value= %d\n",node1->key,node1->value);
	//   else 
	//     printf("Key 20 does not exist\n");
	
	//   for (i=17; i>0; i--)
	//   {
	//     j=DeleteNode(tree4, i, i);
	// 	if (j==0) 
	// 	  printf("Key %d does not exist\n",i);  
	//     PrintAVLTree(tree4);
	//   }
	//  FreeAVLTree(tree4);
	//  //Create tree5
	//  tree5=newAVLTree();
	//  j=InsertNode(tree5, 6, 25);
	//  j=InsertNode(tree5, 6, 10);
	//  j=InsertNode(tree5, 6, 12);
	//  j=InsertNode(tree5, 6, 20);
	//  j=InsertNode(tree5, 9, 25);
	//  j=InsertNode(tree5, 10, 25);
	//  PrintAVLTree(tree5);
	//  //Create tree6
	//  tree6=newAVLTree();
	//  j=InsertNode(tree6, 6, 25);
	//  j=InsertNode(tree6, 5, 10);
	//  j=InsertNode(tree6, 6, 12);
	//  j=InsertNode(tree6, 6, 20);
	//  j=InsertNode(tree6, 8, 35);
	//  j=InsertNode(tree6, 10, 25);
	//  PrintAVLTree(tree6);
	//  tree7=AVLTreesIntersection(tree5, tree6);
	//  tree8=AVLTreesUnion(tree5,tree6);
	//  PrintAVLTree(tree7);
	//  PrintAVLTree(tree8);
	return 0; 
}
