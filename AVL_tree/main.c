#include <stdio.h>
#include <malloc.h>
#define Tree struct tree

Tree {
    int item;
    Tree *left;
    Tree *right;
};

Tree *bg_left(Tree *T){
    Tree *p;
    p = (Tree *) malloc (sizeof(Tree));
    p -> left = NULL;
    p -> right = NULL;
    p -> item = T -> item;
    T -> item = T -> left -> right -> item;
    free(T -> left -> right);
    T -> left -> right = NULL;
    T -> right = p;
    return (T);
}

Tree *bg_right(Tree *T){
    Tree *p;
    p = (Tree *) malloc (sizeof(Tree));
    p -> left = NULL;
    p -> right = NULL;
    p -> item = T -> item;
    T -> item = T -> right -> left -> item;
    free(T -> right -> left);
    T -> right -> left = NULL;
    T -> left = p;
    return (T);
}


Tree *sm_left(Tree *T){
    Tree *p;
    p = (Tree *) malloc (sizeof(Tree));
    p -> left = NULL;
    p -> right = NULL;
    p -> item = T -> item;
    T -> item = T -> left -> item;
    T -> left -> item = T -> left -> left -> item;
    free(T -> left -> left);
    T -> left -> left = NULL;
    T -> right = p;
    return(T);
}

Tree *sm_right(Tree *T){
    Tree *p;
    p = (Tree *) malloc (sizeof(Tree));
    p -> left = NULL;
    p -> right = NULL;
    p -> item = T -> item;
    T -> item = T -> right -> item;
    T -> right -> item = T -> right -> right -> item;
    free(T -> right -> left);
    T -> right -> right = NULL;
    T -> left = p;
    return(T);
}

Tree *balance(Tree *T){
    if (T -> left) {
        if (T->left->right) {
            T = bg_left(T);
            return (T);
        }
        if (T->left->left) {
            T = sm_left(T);
            return (T);
        }
    }

    if (T -> right) {
        if (T->right->left) {
            T = bg_right(T);
            return (T);
        }
        if (T->right->right) {
            T = sm_right(T);
            return (T);
        }
    }
}

Tree *insert(Tree *T, Tree *new_node){
    if (!T){
        T = new_node;
        return T;
    }

    if (new_node -> item < T -> item)
        if (T -> left)
            insert(T -> left, new_node);
        else{
            T -> left = new_node;
            return T;

        }
    else
          if (T -> right)
            insert(T -> right, new_node);
        else{
            T -> right = new_node;
            return T;
        }
}

int check(Tree *T, int high_l, int high_r){
    if (T -> left) {
     high_l = check(T -> left, high_l, high_r);
        high_l++;
        return high_l;
    }

    if (T -> right) {
      high_r = check(T -> right, high_l, high_r);
        high_r++;
        if (T -> left && T -> right || T) {
            if ((high_l - high_r) > 1)
                balance(T);
        }
        return high_r;
    }
}

Tree*  creation(int value){
    Tree *n = (Tree *) malloc (sizeof(Tree));
    n -> item = value;
    n -> left = NULL;
    n -> right = NULL;
    return n;
}

void in_order(Tree *T, FILE *m){
        if (T -> left)
            in_order(T->left, m);
        fprintf(m, "%d ", T->item);

        if (T->right)
          in_order(T -> right, m);
}

void clear(FILE *m, FILE *n, Tree *T){
    fclose(m);
    fclose(n);
    free(T);
}

int main() {
    int N, i, value;
    Tree *T, *p;
    T = NULL;

    FILE *fin, *fout;
    fin = fopen("in.txt", "r");
    if (! fin) {
        puts("error io");
        return 0;
    }
    fout = fopen("out.txt", "w");

    fscanf(fin, "%d", &N);
    if (N == 0) {
        clear(fin, fout, T);
        return 0;
    }

     for (i = 0; i < N; i ++) {
        fscanf(fin, "%d", &value);
         if (!T) {
             T = insert(T, creation(value));
             p = T;
         }
         else
             insert(p, creation (value));
             check(p, 0 ,0);
    }

    if (T -> left == NULL && T -> right == NULL) {
        fprintf(fout, "%d", T->item);
        clear(fin, fout, T);
        return 0;
    }

    in_order(p, fout);
    clear(fin, fout, T);
    return 0;
}
