#include <stdio.h>
#include <stdlib.h>

/*----------------------------------------------------------------------*/

#define ALPHA_SIZE 64
#define LEN_COMMAND 18
#define True 1
#define False 0

/*----------------------------------------------------------------------*/

struct Trie_node{
    int fine_parola;
    struct Trie_node * chars[ALPHA_SIZE];
};

typedef struct Trie_node Trie_Node;
typedef Trie_Node * Trie;

/*----------------------------------------------------------------------*/

void nuova_partita(void);
char get_char(int);
int get_id(char);
Trie build_Trie(void);
Trie get_new_node(void);
void insert(Trie, char *);
int search(Trie, char *);
int max(int, int);
int are_equal(char *, char *);
void aggiungi_parole(void);
void stampa_filtrate(void);
void stampa_confronto(char *, char *);
void attesa(void);
void nuova_partita(void);
void reset_str(void);
void reset_compare_map(void);
void stampa_num_parole_rimaste(void);
void reset_filtri(void);
int match_hard(char *);
int match_soft(char *);
void dfs_count(Trie, int);
void dfs_print(Trie, int);
void end_game(void);
void count_string(char *);
int compare_str(char *, char *);

/*----------------------------------------------------------------------*/

char alpha[] = "-0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz";

char new_game[] = "+nuova_partita";
char filter[] = "+stampa_filtrate";
char start_ins[] = "+inserisci_inizio";
char end_ins[] = "+inserisci_fine";
char not_exist[] = "not_exists";
char ok[] = "ok";
char ko[] = "ko";

int len;
int max_len;
int try_left = -1;
int filter_count;
char * obj;
char * str;
char * res;
char * char_precisi;

Trie store;

int compare_hash_map[ALPHA_SIZE] = {0};
int obj_hash_map[ALPHA_SIZE] = {0};
int string_hash_map[ALPHA_SIZE] = {0};
int char_non_presenti[ALPHA_SIZE] = {0};
int minimum_filter[ALPHA_SIZE] = {0};
int minimum_filter_temp[ALPHA_SIZE] = {0};
int precise_filter[ALPHA_SIZE] = {0};

int ** char_wrong_place;

/*----------------------------------------------------------------------*/

int main(){
    store = build_Trie();

    if(scanf("%d", &len) == EOF){
        end_game();
    }

    /*printf("\n----------------------------------------------------------\n\n");*/

    max_len = max(len + 1, LEN_COMMAND);

    str = (char *)malloc(sizeof(char) * max_len);
    res = (char *)malloc(sizeof(char) * (len + 1));
    obj = (char *)malloc(sizeof(char) * (len + 1));
    char_precisi = (char *)malloc(sizeof(char) * (len + 1));

    char_wrong_place = (int **)malloc(sizeof(int *) * len);
    for(int i = 0; i < len; i++){
        char_wrong_place[i] = (int *)malloc(sizeof(int) * ALPHA_SIZE);
    }

    while(True){
        if(scanf("%s", str) == EOF){
            end_game();
        }
        else if(are_equal(str, new_game)){
            nuova_partita();
        }
        else if(are_equal(str, start_ins)){
            aggiungi_parole();
        }
        else{
            insert(store, str);
        }
    }
    return 0;
}

/*----------------------------------------------------------------------*/

char get_char(int i){
    return alpha[i];
}

int get_id(char c){
    if(c == '-'){
        return 0;
    }
    if('0' <= c && c <= '9'){
        return 1 + (c - '0');
    }
    if('A' <= c && c <= 'Z'){
        return 11 + (c - 'A');
    }
    if(c == '_'){
        return 37;
    }
    if('a' <= c && c <= 'z'){
        return 38 + (c - 'a');
    }
    
    return -1;
}

int compare_str(char * s1, char * s2){
    for(int i = 0; i < len; i++){
        if(get_id(s1[i]) < get_id(s2[i])){
            return 1;
        }
        if(get_id(s1[i]) > get_id(s2[i])){
            return -1;
        }
    }
    return 0;
}

Trie build_Trie(){
    return get_new_node();
}

Trie get_new_node(){
    Trie node = (Trie)malloc(sizeof(Trie_Node));
    node->fine_parola = 0;

    for(int i = 0; i < ALPHA_SIZE; i++){
        node->chars[i] = NULL;
    }
    return node;
}

void insert(Trie root, char * str){
    Trie node = root;
    while(*str){
        if(node->chars[get_id(*str)] == NULL){
            node->chars[get_id(*str)] = get_new_node();
        }
        node = node->chars[get_id(*str)];
        str++;
    }
    node->fine_parola = 1;
    return;
}

int search(Trie root, char * str){
    if(root == NULL){
        return 0;
    }

    Trie node = root;
    while(*str){
        node = node->chars[get_id(*str)];
        if(node == NULL){
            return 0;
        }
        str++;
    }
    return node->fine_parola;
}

int max(int a, int b){
    if (a >= b){
        return a;
    }
    return b;
}

int are_equal(char * s1, char * s2){
    for(int i = 0; i < len; i++){
        if(s1[i] != s2[i]){
            return False;
        }
    }
    return True;
}

void count_string(char * s){
    for(int i = 0; i < ALPHA_SIZE; i++){
        string_hash_map[i] = 0;
    }
    for(int i = 0; i < len; i++){
        string_hash_map[get_id(s[i])]++;
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
        insert(store, str);
    }
    return;
}

void stampa_filtrate(){
    Trie root = store;
    dfs_print(root, 0);
    return;
}

void dfs_print(Trie root, int count){
    if(count == len){
        if(match_soft(res)){
            printf("%s\n", res);
        }
        return;
    }

    char c = res[count];
    char temp;
    for(int i = 0; i < ALPHA_SIZE; i++){
        if(root->chars[i] != NULL){
            temp = get_char(i);
            res[count] = temp;

            if(char_precisi[count] != '.' && char_precisi[count] != temp){
                continue;
            }
            if(char_non_presenti[i]){
                continue;
            }
            if(char_wrong_place[count][i]){
                continue;
            }

            dfs_print(root->chars[i], count + 1);
        }
    }
    res[count] = c;
    return;
}

void reset_str(){
    for(int i = 0; i < len; i++){
        res[i] = '.';
    }
    res[len] = '\0';
    return;
}

int match_soft(char * s){
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

int match_hard(char * s){
    for(int i = 0; i < len; i++){
        if(char_precisi[i] != '.'){
            if(char_precisi[i] != s[i]){
                return False;
            }
        }
        if(char_non_presenti[get_id(s[i])]){
            return False;
        }
        if(char_wrong_place[i][get_id(s[i])]){
            return False;
        }
    }
    return match_soft(s);
}

void attesa(){
    while(True){
        if(scanf("%s", str) == EOF){
            end_game();
        }
        if(are_equal(str, start_ins)){
            aggiungi_parole();
        }
        else if(are_equal(str, new_game)){
            nuova_partita();
        }
        else if(are_equal(str, filter)){
            stampa_filtrate();
        }
        else{
            end_game();
        }
    }
    return;
}

void stampa_num_parole_rimaste(){
    Trie root = store;
    filter_count = 0;
    dfs_count(root, 0);
    printf("%d\n", filter_count);
    return;
}

void dfs_count(Trie root, int count){
    if(count == len){
        if(match_soft(res)){
            filter_count++;
        }
        return;
    }
    char c = res[count];
    char temp;
    for(int i = 0; i < ALPHA_SIZE; i++){
        if(root->chars[i] != NULL){
            temp = get_char(i);
            res[count] = temp;

            if(char_precisi[count] != '.' && char_precisi[count] != temp){
                continue;
            }
            if(char_non_presenti[i]){
                continue;
            }
            if(char_wrong_place[count][i]){
                continue;
            }

            dfs_count(root->chars[i], count + 1);
        }
    }
    res[count] = c;
    return;
}

void reset_filtri(){
    for(int i = 0; i < len; i++){
        char_precisi[i] = '.';
    }
    char_precisi[len] = '\0';

    for(int i = 0; i < ALPHA_SIZE; i++){
        obj_hash_map[i] = 0;
        string_hash_map[i] = 0;
        char_non_presenti[i] = 0;
        minimum_filter[i] = -1;
        minimum_filter_temp[i] = -1;
        precise_filter[i] = -1;
    }

    for(int i = 0; i < len; i++){
        obj_hash_map[get_id(obj[i])]++;
    }

    for(int i = 0; i < len; i++){
        for(int j = 0; j < ALPHA_SIZE; j++){
            char_wrong_place[i][j] = 0;
        }
    }

    return;
}

void reset_compare_map(){
    for(int i = 0; i < ALPHA_SIZE; i++){
        compare_hash_map[i] = obj_hash_map[i];
    }
    return;
}

void stampa_confronto(char * s, char * r){
    reset_str();
    reset_compare_map();

    for(int i = 0; i < len; i++){
        if(s[i] == r[i]){
            res[i] = '+';
            char_precisi[i] = s[i];
            compare_hash_map[get_id(s[i])]--;
        }
    }

    for(int i = 0; i < len; i++){
        if(res[i] == '+'){
            continue;
        }
        if(compare_hash_map[get_id(s[i])] > 0){
            res[i] = '|';
            compare_hash_map[get_id(s[i])]--;
            char_wrong_place[i][get_id(s[i])] = 1;
        }
        else{
            res[i] = '/';
            if(obj_hash_map[get_id(s[i])] == 0){
                char_non_presenti[get_id(s[i])] = 1;
            }
            char_wrong_place[i][get_id(s[i])] = 1;
        }
    }

    for(int i = 0; i < len; i++){
        if(res[i] == '+' || res[i] == '|'){
            if(minimum_filter_temp[get_id(s[i])] == -1){
                minimum_filter_temp[get_id(s[i])] = 1;
            }
            else{
                minimum_filter_temp[get_id(s[i])]++;
            }
        }
    }

    for(int i = 0; i < ALPHA_SIZE; i++){
        minimum_filter[i] = max(minimum_filter[i], minimum_filter_temp[i]);
        minimum_filter_temp[i] = -1;
    }

    count_string(s);

    for(int i = 0; i < ALPHA_SIZE; i++){
        if(minimum_filter[i] < string_hash_map[i]){
            precise_filter[i] = minimum_filter[i];
        }
    }
    printf("%s\n", res);
    return;
}

void nuova_partita(){
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
            nuova_partita();
        }
        else{
            if(are_equal(str, obj)){
                printf("%s\n", ok);
                break;
            }
            else if(!search(store, str)){
                printf("%s\n", not_exist);
            }
            else{
                stampa_confronto(str, obj);
                stampa_num_parole_rimaste();
                try_left--;
                if(try_left == 0){
                    printf("%s\n", ko);
                    break;
                }
            }
        }
    }
    attesa();
    return;
}

void end_game(){
    exit(0);
    return;
}