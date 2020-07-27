#include <stdio.h>
#include <malloc.h>
#include <string.h>
#define Node struct Node
#define Stack_t struct stack_t
#define Tree struct tree
#define List struct list
#define empty 0

List {
    Node *head;
};

Node {
    Node *next;
    int value;
    unsigned char symbol;
    int *b_code;
};

Stack_t {
    Stack_t *next;;
    int v1;
    Tree *t;
};

Tree {
    unsigned char symbol;
    Tree *left;
    Tree *right;
};

Node *element(unsigned char b, int count){
    Node *new;
    new  = (Node*)malloc(sizeof(Node));
    new -> symbol = b;
    new -> value = count;
    new -> next = NULL;
    return (new);
}

Tree *create_list(unsigned char s){
    Tree *new_node = (Tree*)malloc(sizeof(Tree));
    new_node -> right = NULL;
    new_node -> left = NULL;
    new_node -> symbol =  s;
    return new_node;
}

Stack_t *up_code(Stack_t *f, unsigned char a, int v1){
    f = (Stack_t*)malloc(sizeof(Stack_t));
    f -> v1 = v1;
    f -> t = create_list(a);
    f -> next = NULL;
    return(f);
}

Stack_t *create_node_t(Stack_t *f, unsigned char a, int v1) {
    Stack_t *new, *tmp;
    new = up_code(new, a,v1);
    if (!f) {
        f = up_code(f, a, v1);
        return f;
    }
    else
        tmp = f;                                                         //this function just add trees in order

    if (!tmp -> next || f -> v1 > new -> v1)
        if (tmp -> v1 > new -> v1) {
            new -> next = f;
            f = new;
            return f;
        } else {
            if (f -> next)
                new -> next = f -> next;
            f -> next = new;
            return f;
        }

    while (tmp -> next && tmp -> next -> v1 <= new -> v1)
        tmp = tmp -> next;
    new -> next = tmp -> next;
    tmp -> next = new;
    return(f);
}

Stack_t *create_code_t(Stack_t *f, List *pr) {
    Node *cur = pr -> head;
    while (cur) {
        f = create_node_t(f, cur -> symbol, cur -> value);
        cur = cur -> next;
    }
    return(f);
}

List *fill_priority (FILE *fin, List *priority) {
    unsigned char a = 'x';
    fscanf(fin, "%c", &a);
    if (a == 'x')
        return priority;
    priority -> head = element(a, 1);
    while (fscanf(fin, "%c", &a) != EOF) {
        if (a == '\r' )
            fscanf(fin, "%c", &a);
        int count = 0;
        Node *tmp = priority -> head;
        while (tmp) {
            if (tmp -> symbol == a) {                                                 //check if it needs one new variable
                tmp -> value += 1;                                                  //or just can add plus to variable
                count ++;                                                            //which already exists
            }
            tmp = tmp->next;
        }

        if (count == 0) {
            Node *cur = priority -> head;
            while (cur -> next)
                cur = cur -> next;
            cur -> next = element(a, 1);
        }
    }
    return (priority);
}

Stack_t *pop(Stack_t *f){                                   //free useless trees, because of unification
    Stack_t *old= f;
    f  = f -> next;
    free(old);
    return (f);
}

Stack_t *unification(Stack_t *f){
    Tree *new = (Tree*)malloc(sizeof(Tree));
    new -> symbol = empty;
    new -> right = f -> next -> t;
    new -> left = f -> t;
    f -> next -> t = new;
    f = pop(f);
    return f;
}

Stack_t *put(Stack_t *code){                                      //put leaf in needed tree
    int max = code -> v1;
    Stack_t *new = (Stack_t*)malloc(sizeof(Stack_t));
    new -> v1 = code -> v1;
    new -> t = code -> t;
    new -> next = NULL;
    if (code -> next && new -> v1 > code -> next -> v1){
        code = code -> next;
        Stack_t *tmp = code;
        Stack_t *cur = code -> next;
        while (tmp -> next && tmp -> next -> v1 < max) {
            tmp = tmp -> next;
            cur = cur -> next;
        }
        new -> next = cur;
        tmp -> next = new;
    }
    return code;
}

Stack_t *binary(Stack_t *code) {
    while (code -> next){
        code -> next -> v1 = code -> next -> v1 + code -> v1;
        code = unification(code);
        code = put(code);
    }
    return code;
}

int *make_needed(int *needed){                      //make new binary first code
    int i;
    for (i = 0; i < 256; i++)
        *(needed + i) = 2;
    return needed;
}

List *make_bc(Tree *t, List *pr, int *needed, int i) {                  //make special code for every variable
    if (t -> right){                                                    //with recursion
        *(needed + i) = 1;
        make_bc(t -> right, pr, needed, i + 1);
        *(needed + i) = 2;
    }
    if (t -> left){
        *(needed + i) = 0;
        make_bc(t -> left, pr, needed, i + 1);
        *(needed + i) = 2;
    }
    Node *cur = pr -> head;

    if (t -> symbol != empty) {
        while (cur -> symbol != t -> symbol)
            cur = cur -> next;
        for (i = 0; i < 256; i++)
            *(cur -> b_code + i) = *(needed + i);
    }
    return pr;
}

int *symbol(int *s, FILE *fout){                                        //remove array of 8 to recoding symbol
    int i, numb = 0, power = 1;
    for (i = 7; i > -1; i --){
        numb = numb + ((*(s + i))*power);
        power *= 2;
    }
    fprintf(fout, "%c", (unsigned char)numb);
    return s;
}

void complete(int *numb, FILE *fout, int i) {                          //this function needs for situation, when
    int k = i;                                                         //array not full in the end
    while (i != 8){                                                    //just add zero
        *(numb + i) = 0;
        i++;
    }
    symbol(numb, fout);
    fprintf(fout, "\n%d", 8 - k);
    free(numb);
}

void second_time(List *pr, FILE *fin, FILE *fout) {
    fseek(fin, 0, SEEK_SET);
    unsigned char a;
    int *numb = (int*)malloc(8*sizeof(int));
    int i = 0, j = 0;
    while (fscanf(fin, "%c", &a) != EOF) {                 //make recoding text and read text second time
        if (a == '\r')
            fscanf(fin, "%c", &a);
        j = 0;
        Node *cur = pr -> head;
        while (cur -> symbol != a)
            cur = cur -> next;

        while (*(cur -> b_code + j) != 2 && j < 256) {
            *(numb + i) = *(cur -> b_code + j);
            i++;
            if (i == 8) {
                numb = symbol(numb, fout);
                i = 0;
            }
            j ++;
        }
    }
    if (i != 0)
        complete(numb, fout, i);
    else
        fprintf(fout, "\n%d", 0);
}

List *new_p(List *new){                                                   //puts arrays for symbols
    Node *cur = new -> head;
    while (cur){
        cur -> value = 0;
        cur -> b_code = (int*)malloc(255*sizeof(int));
        cur = cur -> next;
    }
    return (new);
}

unsigned char depth(Tree *t){                                              //find leafs of finish tree
    unsigned char a;                                                        //and indificate length of code
    if ( t -> left)
        if (t -> left -> left == NULL && t -> left -> right == NULL) {
            a = t -> left -> symbol;
            t -> left = NULL;
            return a;
        }
        else
            return (depth (t -> left));

    if (t -> right){
        if (t -> right ->  left == NULL && t -> right -> right == NULL){
            a = t -> right -> symbol;
            t -> right = NULL;
            return a;
        }
        else
            return(depth( t-> right));
    }
}

int f_count(Node *cur){
    int k = 0;
    while (*(cur -> b_code + k) == 1 || *(cur -> b_code + k) == 0)
        k ++;
    return k;
}

Node *n_symbol(List *p, unsigned char a) {
    Node *cur = p -> head;
    while (cur -> symbol != a)
        cur = cur -> next;
    return cur;
}

int _fprintf_dict(List *p, unsigned char *s, int i, FILE *fout){
    unsigned char b = *(s + 0), a;
    Node *cur;
    int k_2 = 0, k, j;
    cur = n_symbol(p,b);
    k_2 = f_count(cur);

    for (j = 1; j < i; j++) {                                      //print to out dictionary
        a = *(s + j);                                              //puts length of code and symbol
        cur = n_symbol(p, a);                                      //if two symbols has similar codes
        k = f_count(cur);                                          //make out less
        if (k != k_2){
            fprintf(fout, "%d", k_2);
            fprintf(fout, "%c", b);
            b = a;
            k_2 = k;
        }
        else {
            fprintf(fout, "%d", k_2);
            fprintf(fout, "%c" "%c", b, a);
            j ++;
            if (j < i) {
                b = *(s + j);
                cur = n_symbol(p, b);
                k_2 = f_count(cur);
            }
            else
                return 1;
        }
    }
    fprintf(fout, "%d", k_2);
    fprintf(fout, "%c", b);
    free(s);
}

int dictionary(List *p, FILE *fout, Tree *code_t){
    unsigned char *s = (unsigned char*)malloc(255*sizeof(unsigned char));
    int i = 0;
    unsigned char a = depth(code_t);
    while (code_t -> right || code_t -> left){
        if (a != '\0') {
            *(s + i) = a;
            a = depth(code_t);
            i++;
        } else
            a = depth(code_t);
    }
    if (i == 1) {
        *(s + i) = a;
        i ++;
    }
    _fprintf_dict(p, s, i, fout);
    fprintf(fout, "%c", '\n');
}

int *plus(int *n, int numb){                                                //plus one to binary code of symbol
    for (int i = numb - 1; i > -1; i--){
        if (*(n + i) == 1)
            *(n + i) = 0;
        else {
            *(n + i) = 1;
            i = -1;
        }
    }
    return n;
}

int *for_first(int *old, int number){
    int i;
    old = (int*)malloc(255*sizeof(int));
    for (i = 0; i < number; i ++)
        *(old + i) = 0;
    while (i != 256) {
        *(old + i) = 2;
        i ++;
    }
    return old;
}

int *cipher(int number, int *old, int old_n) {
    int i;
    if (! old)
        old = for_first(old, number);
    else
    if (old_n < number) {                                   //recode dictionary, if symbol has less(more) length
        old = plus(old, old_n);                             //then it makes needed length
        i = old_n;
        while (i != number - 1){
            *(old + i) = 0;
            i ++;
        }
        *(old + number - 1) = 0;
    }
    else
       if (old_n != number) {
              old = plus(old, old_n);
              i = old_n;
          while ( i != number - 1) {
              *(old + i) = 2;
              i --;
        }
    }
       else
          old = plus(old, old_n);
    return old;
}

int recode(List *pr, FILE *sec){                                        //read dictionary from output
    int count = 0;
    unsigned char a;
    int number, old_n;
    int *old = NULL;
    Node *cur = pr -> head;
    while(cur) {
        fscanf(sec, "%c", &a);
        if (a == '\r')
            fscanf(sec, "%c", &a);
        count ++;
        if (a <= '9' && a >= '0') {
            number = a - '0';
            fscanf(sec, "%c", &a);
            if (a <= '9' && a >= '0') {
                number = number * 10 + a - '0';
                count ++;
            }
            else
                fseek(sec, - 1, SEEK_CUR);
        } else {
            cur -> symbol = a;
            old = cipher(number, old, old_n);
            for (int i = 0; i < 256; i++)
                *(cur -> b_code + i) = *(old + i);
            old_n = number;
            cur = cur -> next;
        }
    }
    return count;
}

List *make_pr_new_again(List *p){
    Node *cur  = p -> head;
    while (cur){
        for (int i = 0; i < 256; i++)
            *(cur -> b_code + i) = 2;
        cur = cur -> next;
    }
}

size_t allowable_length(List *p){                                             //find length of smallest symbol
    int i = 0;                                                                //because we need to know
    size_t k;                                                                 //which length we can start to compare
    Node *cur = p -> head;
    while (*(cur -> b_code + i) == 1 || *(cur -> b_code + i) == 0)
        i++;
    k = (size_t) i * 4;
    return k;
}

int finish (FILE *fin, int size, int *nas, List *p, size_t k, FILE *out) {               //last function for print
   unsigned char c;                                                                      //first text
    int j = 0, v, i = 0;
    size_t old = k;
    Node *cur = p -> head;

    fscanf(fin, "\n%c", &c);
    v = c - '0';
    while (j != size - v + 1) {
        int *m = nas + j;
        while (cur && (memcmp((char*)cur -> b_code,(char*) m, k) != 0)) {               //compare main array with codes
            cur = cur -> next;                                                          //of symbols
            if (!cur )
                return 0;
            i = 0;
            while (*(cur -> b_code + i) == 1 || *(cur -> b_code + i) == 0)
                i ++;
            k = (size_t)i * 4;
        }
        fprintf(out, "%c", cur -> symbol);
        cur = p -> head;
        j = j + k/4;
        k = old;
    }
}

int out(FILE *fin, FILE *out, List *p, int size) {                 //make main array for second recoding to first text
    unsigned char c;
    int j = -1, a;
    size_t k = 0;
    int *nas = (int*)malloc(size*sizeof(int));
    k = allowable_length(p);
    fscanf(fin, "\n%c", &c);
    if (c == '\r')
        fscanf(fin, "%c", &c);

    while (j != size) {
        a = (int)c;
        int l = j + 8;
        j = l;
        while ((a != 0 || l != j-8)) {
            *(nas + l) = a % 2;
            a = a / 2;
            l--;
            if (l == 0)
                *(nas + l) = a % 2;
        }
        fscanf(fin, "%c", &c);
        if (c == '\r')
            fscanf(fin, "%c", &c);
    }
    fseek(fin, -2, SEEK_END);
    finish(fin, size, nas, p, k, out);
    free(nas);
}


int _size(FILE *fin_second, int count){
    int size = ftell(fin_second);
    size = (size - 3 - count)*8 - 1;
    return size;
}

void clean(List *pr, int *needed, Stack_t *code) {
    free(pr);
    free(needed);
    free(code);
}

int check_file(FILE *f){
    if (!f) {
        puts("error io");
        return 1;
    }
    else
        return 0;
}

void free_file(FILE *fout, FILE *out_second){
    fclose(fout);
    fclose(fout);
}

int main() {
    FILE *fin, *fout, *fin_second, *out_second;
    fin = fopen("in.txt", "rb");
    fout = fopen("out.txt", "wb");
    if (check_file(fin) == 1)
        return 1;

    List *priority = (List*)malloc(sizeof(Node));
    priority -> head = NULL;
    priority = fill_priority(fin, priority);                //count frequency of occurrence
    if (! priority -> head) {
        out_second = fopen("out_2.txt", "wb");
        free_file(fout, out_second);
        return 0;
    }
    Stack_t *code_t = NULL;
    code_t = create_code_t(code_t, priority);                   //creating trees with symbols
    code_t = binary(code_t);                                    //unificate all trees to one tree
    priority = new_p(priority);                                 //includes to priority memory for array
    int *needed = (int*)malloc(255*sizeof(int));
    needed = make_needed(needed);                              // this array will used in next function for helping with creating code
    priority = make_bc(code_t -> t, priority, needed, 0);      //this function make unique code for every symbol
    dictionary(priority, fout, code_t -> t);
    second_time(priority, fin, fout);                         //write to out.txt recode text
    fclose(fout);
    fclose(fin);

    fin_second = fopen("out.txt", "rb");
    if (check_file(fin_second) == 1)
        return 1;
    make_pr_new_again(priority);                         //use priority second time for codes
    int count = recode(priority, fin_second);             // read dictionary
    fseek(fin_second, 0, SEEK_END);
    int size = _size(fin_second, count);
    fseek(fin_second, count + 1, SEEK_SET);
    out_second = fopen("out_2.txt", "wb");
    out(fin_second, out_second, priority, size);
    fclose(fin_second);
    fclose(out_second);
    clean(priority, needed, code_t);
    return 0;
}