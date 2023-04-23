#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) 
{
  TreeMap* tree = (TreeMap*) malloc(sizeof(TreeMap));
  tree->root = NULL;
  tree->current = NULL;
  tree->lower_than = lower_than;
  
  return tree;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) 
{
  TreeNode* current = tree->root;
  TreeNode* newSon = NULL;
  while (current != NULL)
  {
    newSon = current;
    if (is_equal(tree, key, current->pair->key))
    {
      return;
    }
    
    if (tree->lower_than(key, current->pair->key))
    {
      current = current->left;  
    }else
    {
      current = current->right;
    }  
  }

  TreeNode* node = createTreeNode(key, value);

  node->parent = newSon;
  if (newSon == NULL) {
        tree->root = node;
  } else if (tree->lower_than(key, newSon->pair->key)) 
    {
        newSon->left = node;
    } else 
    {
      newSon->right = node;
    }
  
  tree->current = node;

  
}



TreeNode * minimum(TreeNode * x){

  if (x->left == NULL)
  {
    return x;
  }else{
    return minimum(x->left);
  }
}


void removeNode(TreeMap * tree, TreeNode* node) 
{
  if (node->left == NULL && node->right == NULL)
  {
    if (node->parent != NULL)
    {
      if (node->parent->left == node)
      {
        node->parent->left = NULL;
      }
      else
      {
        node->parent->right = NULL;
      }
    }else{
      tree->root = NULL;
    }
    free(node);
    return;
  }

  if(node->left == NULL || node->right == NULL)
  {
    TreeNode* aux = node->left != NULL ? node->left : node->right;
    if(node->parent != NULL)
    {
      if (node->parent->left == node) {
        node->parent->left = aux;
      } else {
        node->parent->right = aux;
      }
      aux->parent = node->parent;
    } else {
      tree->root = aux;
      aux->parent = NULL;
    }
    free(node);
    return;
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) 
{
  if (tree->root == NULL)
  {
    return NULL;
  }
  
  tree->current = tree->root;

  while(tree->current != NULL)
    {
      if (is_equal(tree,key,tree->current->pair->key))
      {
        return tree->current->pair;
      }

      else if (tree->lower_than(key, tree->current->pair->key))
      {
        tree->current = tree->current->left;
      }else{
        tree->current = tree->current->right;
      }
    }
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) 
{
  TreeNode* current = tree->root;
  
  TreeNode* node = NULL;
  
  while (current != NULL) 
  {
    if (is_equal(tree, current->pair->key, key)) 
    {
      return current->pair;
    } else if (tree->lower_than(key, current->pair->key)) {
      node = current;
      current = current->left;
    } else {
      current = current->right;
    }
  }
  
  if (node == NULL) 
  {      
    return NULL;
  } else {
      return node->pair;
  }
}

Pair * firstTreeMap(TreeMap * tree) 
{
  if (tree == NULL)
  {
    return NULL;
  }

  TreeNode* min = minimum(tree->root);

  tree->current = min;
  
    return min->pair;
}

Pair * nextTreeMap(TreeMap * tree) 
{
  if(tree == NULL || tree-> current == NULL) return NULL;   
  TreeNode* current = tree->current;
  
  if(current ->right != NULL)
  {
    current = current->right;
    
    while(current->left !=NULL)
    {
       current = current->left;   
    }    
  }else{
    TreeNode* father = current->parent;
    
    while (father != NULL && current == father->right)
    {
      current = father;
      father = father->parent;
    }

    current = father;
  }

  tree->current = current;

  return (current != NULL) ? current->pair : NULL;
}
