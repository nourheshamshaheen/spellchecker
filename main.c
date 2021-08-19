#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct
{
    char * word;
    struct node *left;
    struct node *right;
} node;


node* newNode(char*);
int size(node*);
int height(node*);
node* insert(node*, char*);
node* readDictionary(char*);
node* search(node*,char*);
void getPreSuc(node*, node**, node**, char[]);
node* findMin(node*);
node* findMax(node*);
void spellChecker(char*, node*);
node* getLeafNode(node*,char*);


node * newNode(char*word)
{
    node * n = malloc(sizeof(node));
    n->word=malloc(strlen(word)+1);
    strcpy(n->word,word);
    n->left=NULL;
    n->right=NULL;
    return n;
}

node * readDictionary(char * filePath)
{
    node * t = NULL;
    //open specified file
    FILE * fp = fopen(filePath,"r");
    char line[256];
    if(fp == NULL)
    {
        printf("Error in loading file.");
    }
    else
    {
        //insert the words in the file as nodes in the BST
        while(!feof(fp))
        {
            fscanf(fp,"%s",line);
            t=insert(t,line);
        }
        printf("File loaded successfully.");

        fclose(fp);
    }
    return t;

}

// function to get size of a tree
int size(node * n)
{
    if(n==NULL)
        return 0;
    else
        return 1+size(n->left)+size(n->right);
}
// function to get height of a tree
int height(node * n)
{
    int left = 0;
    int right = 0;
    if(!n)
        return -1;
    left = 1 + height(n->left);
    right = 1 + height(n->right);
    return left>right?left:right;
}
//function to insert node in tree
node* insert(node* root, char * word)
{
    if (root == NULL)
        return newNode(word);

    if (strcasecmp(word,root->word)<0)
        root->left = insert(root->left, word);

    else if (strcasecmp(word,root->word)>0)
        root->right = insert(root->right, word);

    return root;
}
//to get leaf node
node* getLeafNode(node* root, char* word)
{
    if(root->left == NULL && (strcasecmp(word, root->word)) < 0) return root;

    if(root->right == NULL && (strcasecmp(word, root->word)) > 0) return root;

    if(root->left == NULL && root->right == NULL) return root;

    if(strcasecmp(word, root->word) < 0) return getLeafNode(root->left, word);

    if(strcasecmp(word, root->word) > 0) return getLeafNode(root->right, word);
}
//to get predecessor and successor nodes
void getPreSuc(node* root, node** pre, node** suc, char word[])
{
    if (root == NULL)  return ;

    if (strcasecmp(root->word, word) == 0)
    {
        // the maximum value in left subtree is predecessor
        if (root->left != NULL)
        {
            *pre = findMax(root->left);
        }
        // the minimum value in right subtree is successor
        if(root->right != NULL)
        {
            *suc = findMin(root->right);
        }
        return ;
    }

    // If word is smaller than word in root, go to left subtree
    if (strcasecmp(root->word,word) > 0)
    {
        *suc = root ;
        getPreSuc(root->left, pre, suc, word) ;
    }
    else // go to right subtree
    {
        *pre = root ;
        getPreSuc(root->right, pre, suc, word) ;
    }
}

void spellChecker(char * sentence, node * t)
{
    char * token;
    char *phrase[100];

    token = strtok(sentence," \n");
    int i = 0;
    while(token)
    {
        phrase[i] = malloc(strlen(token)+1);
        phrase[i] = token;
        token = strtok(NULL," \n");
        node * temp = NULL;
        temp = search(t,phrase[i]);
        if(temp!=NULL)
        {
            printf("%s - CORRECT\n",phrase[i]);
        }
        else
        {
            node* leaf = getLeafNode(t, phrase[i]);
            node* pre;
            node* suc;
            getPreSuc(t, &pre, &suc, leaf->word);
            printf("%s - INCORRECT - %s %s or %s?\n",phrase[i], leaf->word, suc->word, pre->word);
        }
        i++;
    }

}

node * search(node *n, char* word)
{
    if(n==NULL)
        return NULL;
    if(strcasecmp(word,n->word)==0)
        return n;
    else if(strcasecmp(word,n->word)<0)
        return search(n->left,word);
    else if(strcasecmp(word,n->word)>0)
        return search(n->right,word);
}

node * findMin(node*root)
{
    if(root==NULL)
        return NULL;
    while(root->left!=NULL)
    {
        root = root->left;
    }
    return root;
}

node * findMax(node*root)
{
    if(root==NULL)
        return NULL;
    while(root->right!=NULL)
    {
        root = root->right;
    }
    return root;
}

int main()
{
    char sentence[256]= "";
    char file[256]="";
    node * t = readDictionary("EN-US-Dictionary.txt");
    printf("\n.................................");
    while(t==NULL)
    {
        printf("\nPlease enter the correct file path: \n");
        scanf("%s",file);
        t = readDictionary(file);
    }
    printf("\nSize = %d\n",size(t));
    printf(".................................");
    printf("\nHeight = %d\n",height(t));
    printf(".................................");
    printf("\nEnter a sentence or Ctrl+Z to exit: \n");

   // inOrder(t);
    while(fgets(sentence,255,stdin)!=NULL)
    {
        spellChecker(sentence,t);
        printf("\nEnter a sentence or Ctrl+Z to exit: \n");
    }

    return 0;
}
