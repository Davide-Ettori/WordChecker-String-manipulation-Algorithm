#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*----------------------------------------------------------------------*/

#define ALPHA_SIZE 64
#define CHAR_SIZE 128
#define COMMAND_SIZE 18
#define True 1
#define False 0
#define LOW 1
#define MEDIUM 2
#define HIGH 4

/*----------------------------------------------------------------------*/

typedef char * string;

struct node {
    string str;
    struct node * left;
    struct node * right;
};

typedef struct node BST_node;
typedef BST_node * BST;

struct Node{
    string str;
    struct Node * next;
};

typedef struct Node Node;
typedef Node * List;

/*----------------------------------------------------------------------*/

void nuova_partita(void);
char get_char(int);
BST new_node(string);
BST insert(BST, string);
int search(BST root, string str);
int max(int, int);
int are_equal(string, string);
void aggiungi_parole(void);
void stampa_filtrate(void);
void stampa_confronto(string, string);
void reset_str(void);
void stampa_num_parole_rimaste(void);
void reset_filtri(void);
int match(string);
void end_game(void);
void count_string(string);
int compare_str(string, string);
void printa_str(string);
void printa_int(int);
void printa_tree(BST);
void copy_tree_in_list(BST);
List append(List, string);
void dealloc_list(List);
void printa_list(List);
void merge_sort(List *);
void merge(List *, List *, List *, List *);
int get_length(List);
void dealloc_tree(BST);
void dealloc_all(void);
void filtra_tree(BST);

/*----------------------------------------------------------------------*/

BST store = NULL;
List cache = NULL;

const char alpha[] = "-0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz";
int map[CHAR_SIZE] = {0};

char new_game[] = "+nuova_partita";
char filter[] = "+stampa_filtrate";
char start_ins[] = "+inserisci_inizio";
char end_ins[] = "+inserisci_fine";
char not_exist[] = "not_exists";
char ok[] = "ok";
char ko[] = "ko";

string obj;
string str;
string res;
string char_precisi;

int compare_hash_map[ALPHA_SIZE] = {0};
int obj_hash_map[ALPHA_SIZE] = {0};
int string_hash_map[ALPHA_SIZE] = {0};
int char_non_presenti[ALPHA_SIZE] = {0};
int minimum_filter[ALPHA_SIZE] = {0};
int minimum_filter_temp[ALPHA_SIZE] = {0};
int precise_filter[ALPHA_SIZE] = {0};

int len;
int max_len;
int try_left;
int filter_count;
int memo_size;
int is_game = False;
int sorted = True;
int num_parole = 0;
int count_parole = 0;

int ** char_wrong_place;

/*----------------------------------------------------------------------*/

int main(){
    if(scanf("%d", &len) == EOF){
        end_game();
    }
    max_len = max(len + 1, COMMAND_SIZE);

    str = (string)malloc(sizeof(char) * max_len);
    res = (string)malloc(sizeof(char) * (len + 1));
    obj = (string)malloc(sizeof(char) * (len + 1));
    char_precisi = (string)malloc(sizeof(char) * (len + 1));

    char_wrong_place = (int **)malloc(sizeof(int *) * len);
    for(int i = 0; i < len; i++){
        char_wrong_place[i] = (int *)malloc(sizeof(int) * ALPHA_SIZE);
    }

    for(int i = 0; i < ALPHA_SIZE; i++){
        map[(int)alpha[i]] = i;
    }

    while(True){
        if(scanf("%s", str) == EOF){
            end_game();
        }
        else if(are_equal(str, new_game)){
            if(count_parole > 0.8 * pow(10, 5)){
                memo_size = HIGH;
            }
            else if(count_parole > 5 * pow(10, 4)){
                memo_size = MEDIUM;
            }
            else{
                memo_size = LOW;
            }
            count_parole = -1;
            nuova_partita();
        }
        else if(are_equal(str, start_ins)){
            aggiungi_parole();
        }
        else if(are_equal(str, filter)){
            printa_tree(store);
        }
        else{
            count_parole++;
            store = insert(store, str);
        }
    }
    return 0;
}

/*----------------------------------------------------------------------*/

List append(List head, string str){
    List node = (List)malloc(sizeof(Node));
    node->str = (string)malloc(sizeof(sizeof(char) * (len + 1)));

    for(int i = 0; i < len; i++){
        node->str[i] = str[i];
    }
    node->str[len] = '\0';
    node->next = head;
    return node;
}

int get_length(List cur){
    int count = 0;
    
    while (cur != NULL) {
        cur = cur->next;
        count++;
    }
    return count;
}
 
void merge(List * node_1, List * node_3, List * node_2, List * node_4){
    List temp = NULL;
    if(compare_str((*node_1)->str, (*node_2)->str) == -1){
        temp = *node_1;
        *node_1 = *node_2;
        *node_2 = temp;

        temp = *node_3;
        *node_3 = *node_4;
        *node_4 = temp;
    }
 
    List node_5 = *node_1;
    List node_last = *node_3;
    List node_6 = *node_2;
    List node_ref = (*node_4)->next;

    while(node_5 != node_last && node_6 != node_ref){
        if(compare_str(node_5->next->str, node_6->str) == -1){
            temp = node_6->next;
            node_6->next = node_5->next;
            node_5->next = node_6;
            node_6 = temp;
        }
        node_5 = node_5->next;
    }
    if(node_5 == node_last){
        node_5->next = node_6;
    }
    else{
        *node_4 = *node_3;
    }
    return;
}

void merge_sort(List * head){
    sorted = True;
    if(*head == NULL){
        return;
    }

    List node_1 = NULL;
    List node_3 = NULL;
    List node_2 = NULL;
    List node_4 = NULL;
    List prec = NULL;
    List temp = NULL;

    int len = get_length(*head);
    int is_first_iter;
    int counter;
 
    for(int jump = 1; jump < len; jump *= 2){
        node_1 = *head;
        while(node_1 != NULL){
            is_first_iter = False;

            if(node_1 == *head){
                is_first_iter = True;
            }
            counter = jump;
            node_3 = node_1;
            while(--counter > 0 && node_3->next != NULL){
                node_3 = node_3->next;
            }
 
            node_2 = node_3->next;
            if(node_2 == NULL){
                break;
            }
            counter = jump;
            node_4 = node_2;

            while(--counter > 0 && node_4->next != NULL){
                node_4 = node_4->next;
            }
 
            temp = node_4->next;
 
            merge(&node_1, &node_3, &node_2, &node_4);
 
            if(is_first_iter){
                *head = node_1;
            }
            else{
                prec->next = node_1;
            }
            prec = node_4;
            node_1 = temp;
        }
        prec->next = node_1;
    }
    return;
}

void dealloc_list(List head){
    List temp;
    if(head == NULL){
        return;
    }
    while(head != NULL){
        temp = head->next;
        free(head->str);
        free(head);
        head = temp;
    }
    return;
}

void printa_str(string s){
    printf("%s\n", s);
    return;
}

void printa_int(int n){
    printf("%d\n", n);
    return;
}

void printa_list(List head){
    List cur = head;
    while(cur != NULL){
        printa_str(cur->str);
        cur = cur->next;
    }
    return;
}

void printa_tree(BST root){
    if(root == NULL){
        return;
    }
    if(root->left != NULL){
        printa_tree(root->left);
    }

    if(match(root->str)){
        printa_str(root->str);
    }

    if(root->right != NULL){
        printa_tree(root->right);
    }
    return;
}

char get_char(int i){
    return alpha[i];
}

int compare_str(string s1, string s2){
    for(int i = 0; i < len; i++){
        if(s1[i] < s2[i]){
            return 1;
        }
        if(s1[i] > s2[i]){
            return -1;
        }
    }
    return 0;
}

BST new_node(string str){
    BST temp = (BST)malloc(sizeof(BST_node));
    temp->str = (string)malloc(sizeof(char) * (len + 1));

    for(int i = 0; i < len; i++){
        temp->str[i] = str[i];
    }
    temp->str[len] = '\0';
    temp->left = NULL;
    temp->right = NULL;
    return temp;
}

BST insert(BST root, string str){
    BST temp = root;
    BST node = new_node(str);
    if(root == NULL){
        return node;
    }

    while(True){
        if(compare_str(str, temp->str) == -1){
            if(temp->right == NULL){
                temp->right = node;
                break;
            }
            temp = temp->right;
        }
        else{
            if(temp->left == NULL){
                temp->left = node;
                break;
            }
            temp = temp->left;
        }
    }
    return root;
}

int search(BST root, string str){   
    if(root == NULL){
        return False;
    }
    BST node = root;
    while(node != NULL){
        if(compare_str(node->str, str) == 0){
            return True;
        }
        if(compare_str(node->str, str) == 1){
            node = node->right;
        }
        else{
            node = node->left;
        }
    }
    return False;
}

int max(int a, int b){
    if (a >= b){
        return a;
    }
    return b;
}

int are_equal(string s1, string s2){
    for(int i = 0; i < len; i++){
        if(s1[i] != s2[i]){
            return False;
        }
    }
    return True;
}

void count_string(string s){
    memset(string_hash_map, 0, ALPHA_SIZE * sizeof(int));
    for(int i = 0; i < len; i++){
        string_hash_map[map[(int)s[i]]]++;
    }
    return;
}

void aggiungi_parole(){
    while(True){
        if(scanf("%s", str) == EOF){
            end_game();
        }
        if(are_equal(str, end_ins)){
            break;
        }
        if(are_equal(str, filter)){
            printa_tree(store);
            continue;
        }
        store = insert(store, str);
        if(count_parole != -1){
            count_parole++;
        }
        if(is_game && num_parole >= memo_size && match(str)){
            sorted = False;
            cache = append(cache, str);
        }
    }
    return;
}

void stampa_filtrate(){
    if(num_parole < memo_size){
        printa_tree(store);
        return;
    }
    if(!sorted){
        merge_sort(&cache);
    }
    printa_list(cache);
    return;
}

void reset_str(){
    memset(res, '.', (len + 1) * sizeof(char));
    for(int i = 0; i < ALPHA_SIZE; i++){
        compare_hash_map[i] = obj_hash_map[i];
    }
    return;
}

void stampa_num_parole_rimaste(){
    filter_count = 0;
    if(num_parole < memo_size){
        filtra_tree(store);
        printa_int(filter_count);
        return;
    }
    if(num_parole == memo_size){
        dealloc_list(cache);
        cache = NULL;
        copy_tree_in_list(store);
    }
    List cur = cache;
    List prec;
    List temp;

    if(cur == NULL){
        printa_int(filter_count);
        return;
    }
    if(cur->next == NULL){
        if(match(cur->str)){
            filter_count++;
            printa_int(filter_count);
            return;
        }
        else{
            printa_int(filter_count);
            free(cur->str);
            free(cur);
            cache = NULL;
            return;
        }
    }

    while(cur != NULL && !match(cur->str)){
        temp = cur->next;
        free(cur->str);
        free(cur);
        cache = temp;
        cur = temp;
    }
    if(cur == NULL){
        printa_int(filter_count);
        cache =  NULL;
        return;
    }
    if(cur->next == NULL){
        filter_count++;
        printa_int(filter_count);
        cache = cur;
        return;
    }

    filter_count++;
    prec = cur;
    cur = cur->next;
    while(cur != NULL){
        if(match(cur->str)){
            filter_count++;
            cur = cur->next;
            prec = prec->next;
        }
        else{
            prec->next = cur->next;
            temp = cur->next;
            free(cur->str);
            free(cur);
            cur = temp;
        }
    }
    printa_int(filter_count);
    return;
}

void reset_filtri(){
    for(int i = 0; i < len; i++){
        char_precisi[i] = '.';
        for(int j = 0; j < ALPHA_SIZE; j++){
            char_wrong_place[i][j] = 0;
            if(i == 0){
                string_hash_map[j] = 0;
                char_non_presenti[j] = 0;
                minimum_filter[j] = -1;
                minimum_filter_temp[j] = -1;
                precise_filter[j] = -1;
                obj_hash_map[j] = 0;
            }
        }
        obj_hash_map[map[(int)obj[i]]]++;
    }

    dealloc_list(cache);
    cache = NULL;
    return;
}

int match(string s){
    int c;

    for(int i = 0; i < len; i++){
        if(char_precisi[i] != '.' && char_precisi[i] != s[i]){
            return False;
        }
        c = map[(int)s[i]];
        if(char_non_presenti[c]){
            return False;
        }
        if(char_wrong_place[i][c]){
            return False;
        }
    }

    count_string(s);

    for(int i = 0; i < ALPHA_SIZE; i++){
        if(precise_filter[i] != -1 && precise_filter[i] != string_hash_map[i]){
            return False;
        }
        if(minimum_filter[i] != -1 && minimum_filter[i] > string_hash_map[i]){
            return False;
        }
    }
    return True;
}

void stampa_confronto(string s, string r){
    reset_str();
    int c;

    for(int i = 0; i < len; i++){
        if(s[i] == r[i]){
            res[i] = '+';
            char_precisi[i] = s[i];
            compare_hash_map[map[(int)s[i]]]--;
        }
    }

    for(int i = 0; i < len; i++){
        if(res[i] == '+'){
            continue;
        }
        c = map[(int)s[i]];
        if(compare_hash_map[c] > 0){
            res[i] = '|';
            compare_hash_map[c]--;
            char_wrong_place[i][c] = 1;
        }
        else{
            res[i] = '/';
            char_non_presenti[c] = 1;
            char_wrong_place[i][c] = 1;
        }
    }

    for(int i = 0; i < len; i++){
        if(res[i] == '+' || res[i] == '|'){
            c = map[(int)s[i]];
            char_non_presenti[c] = 0;
            if(minimum_filter_temp[c] == -1){
                minimum_filter_temp[c] = 1;
            }
            else{
                minimum_filter_temp[c]++;
            }
        }
    }

    for(int i = 0; i < ALPHA_SIZE; i++){
        minimum_filter[i] = max(minimum_filter[i], minimum_filter_temp[i]);
        minimum_filter_temp[i] = -1;
    }

    count_string(s);

    for(int i = 0; i < ALPHA_SIZE; i++){
        if(minimum_filter[i] != -1 && minimum_filter[i] < string_hash_map[i]){
            precise_filter[i] = minimum_filter[i];
        }
    }

    res[len] = '\0';
    printa_str(res);
    return;
}

void copy_tree_in_list(BST root){
    if(root == NULL){
        return;
    }
    if(root->right){
        copy_tree_in_list(root->right);
    }
    if(match(root->str)){
        cache = append(cache, root->str);
    }
    if(root->left){
        copy_tree_in_list(root->left);
    }
    return;
}

void filtra_tree(BST root){
    if(root == NULL){
        return;
    }
    if(match(root->str)){
        filter_count++;
    }
    if(root->left){
        filtra_tree(root->left);
    }
    if(root->right){
        filtra_tree(root->right);
    }
    return;
}

void nuova_partita(){
    RESTART:; /**/
    is_game = True;
    num_parole = 0;
    if(scanf("%s", obj) == EOF){
        end_game();
    }
    if(scanf("%d", &try_left) == EOF){
        end_game();
    }
    reset_filtri();

    while(True){
        if(scanf("%s", str) == EOF){
            end_game();
        }

        if(are_equal(str, start_ins)){
            aggiungi_parole();
        }
        else if(are_equal(str, filter)){
            stampa_filtrate();
        }
        else if(are_equal(str, new_game)){
            goto RESTART;
        }
        else{
            if(are_equal(str, obj)){
                is_game = False;
                printa_str(ok);
                break;
            }
            else if(!search(store, str)){
                printa_str(not_exist);
            }
            else{
                if(num_parole <= memo_size){
                    num_parole++;
                }

                stampa_confronto(str, obj);
                stampa_num_parole_rimaste();
                try_left--;
                if(try_left == 0){
                    is_game = False;
                    printa_str(ko);
                    break;
                }
            }
        }
    }

    while(True){
        if(scanf("%s", str) == EOF){
            end_game();
        }
        if(are_equal(str, start_ins)){
            aggiungi_parole();
        }
        else if(are_equal(str, new_game)){
            goto RESTART;
        }
        else if(are_equal(str, filter)){
            printa_tree(store);
        }
        else{
            end_game();
        }
    }
    return;
}

void dealloc_tree(BST root){
    if(root == NULL){
        return;
    }
    if(root->left != NULL){
        dealloc_tree(root->left);
    }
    if(root->right != NULL){
        dealloc_tree(root->right);
    }
    free(root->str);
    free(root);
    return;
}

void dealloc_all(){
    free(str);
    free(res);
    free(obj);
    free(char_precisi);
    for(int i = 0; i < len; i++){
        free(char_wrong_place[i]);
    }
    free(char_wrong_place);
    dealloc_list(cache);
    dealloc_tree(store);
    return;
}

void end_game(){
    dealloc_all();
    exit(0);
    return;
}