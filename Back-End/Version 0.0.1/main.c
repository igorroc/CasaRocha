#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/// =-=-=-=-=-=-=-=-=-=-=-= STRUCTS =-=-=-=-=-=-=-=-=-=-=-=

struct Conta{
    char email[256];
    char senha[256];
};

struct Produto{
    char nome[256];
    float preco;
    char codigo[256];
};

/// =-=-=-=-=-=-=-=-=-=-=-= FUNCOES =-=-=-=-=-=-=-=-=-=-=-=

void menu_print();

void txt_struct_conta(FILE*, struct Conta*);
void txt_struct_produto(FILE*, struct Produto*);

int valida_int(char*);
int valida_float(char*);
int valida_resp_menu(char*);
int valida_nome(char*);
int valida_email(char*);

int procura_email(char*, struct Conta*);
int procura_senha(char*, char*, struct Conta*);

void add_produto(struct Produto*, FILE*, char*);

void limpa(char*);
void tolower(char*);

/// =-=-=-=-=-=-=-=-=-=-=-=-= MAIN =-=-=-=-=-=-=-=-=-=-=-=-=

int main(){
    system("mode con:cols=72 lines=55");
    srand(time(NULL));

    char resp[256] = {'\0'};
    int opcao = 0;
    struct Conta contas[300];
    struct Produto produtos[300];
    for(int i = 0; i < 300; i++){
        limpa(contas[i].email);
        limpa(contas[i].senha);

        limpa(produtos[i].nome);
        limpa(produtos[i].codigo);
        produtos[i].preco = 0;
    }
    FILE *db_contas = fopen("db_contas.txt", "r+");
    if(db_contas == NULL){
        fopen("db_contas.txt", "w+");
    }
    FILE *db_produtos = fopen("db_produtos.txt", "r+");
    if(db_produtos == NULL){
        fopen("db_produtos.txt", "w+");
    }
    txt_struct_conta(db_contas, contas);
    txt_struct_produto(db_produtos, produtos);




    printf("Teste do App de Feira\n\n");
    do{
        menu_print();
        setbuf(stdin, NULL);
        scanf("%20[^\n]", resp);
        while(!valida_int(resp) || !valida_resp_menu(resp)){
            limpa(resp);
            printf("Digite um valor valido: ");
            setbuf(stdin, NULL);
            scanf("%20[^\n]", resp);
        }
        sscanf(resp, "%d", &opcao);

        switch (opcao){
            case 1:
                printf("Voce ja possui uma conta? (1 -> Sim) (0 -> Nao)\n");
                char email[256] = {'\0'};
                setbuf(stdin, NULL);
                scanf("%20[^\n]", resp);
                while(!valida_int(resp) || !valida_resp_menu(resp)){
                    limpa(resp);
                    printf("Digite um valor valido: ");
                    setbuf(stdin, NULL);
                    scanf("%20[^\n]", resp);
                }
                sscanf(resp, "%d", &opcao);
                if(opcao == 1){
                    printf("Digite seu email: ");
                    setbuf(stdin, NULL);
                    scanf("%100[^\n]", resp);
                    while(!procura_email(resp, contas)){
                        limpa(resp);
                        printf("Email nao encontrado, digite novamente: ");
                        setbuf(stdin, NULL);
                        scanf("%100[^\n]", resp);
                    }
                    strcpy(email, resp);

                    printf("Digite sua senha: ");
                    setbuf(stdin, NULL);
                    scanf("%100[^\n]", resp);
                    while(!procura_senha(email, resp, contas)){
                        limpa(resp);
                        printf("Senha invalida, digite novamente: ");
                        setbuf(stdin, NULL);
                        scanf("%100[^\n]", resp);
                    }
                }else if (opcao == 0){
                    char senha1[256] = {'\0'};
                    printf("Crie seu email: ");
                    setbuf(stdin, NULL);
                    scanf("%100[^\n]", resp);
                    while(!valida_email(resp) || procura_email(resp, contas)){
                        if(procura_email(resp, contas)){
                            limpa(resp);
                            printf("Email ja existente, digite outro: ");
                            setbuf(stdin, NULL);
                            scanf("%100[^\n]", resp);
                        }else{
                            limpa(resp);
                            printf("Email invalido, digite novamente: ");
                            setbuf(stdin, NULL);
                            scanf("%100[^\n]", resp);
                        }

                    }
                    strcpy(email, resp);

                    printf("Crie sua senha: ");
                    setbuf(stdin, NULL);
                    scanf("%100[^\n]", resp);
                    while(!valida_senha(resp)){
                        limpa(resp);
                        printf("A senha deve ter mais de 8 caracteres e nao deve possuir espacos vazios. \nDigite novamente: ");
                        setbuf(stdin, NULL);
                        scanf("%100[^\n]", resp);
                    }
                    strcpy(senha1, resp);

                    printf("Confirme sua senha: ");
                    setbuf(stdin, NULL);
                    scanf("%100[^\n]", resp);
                    while(strcmp(senha1, resp) != 0){
                        limpa(resp);
                        printf("As senhas sao diferentes, digite novamente: ");
                        setbuf(stdin, NULL);
                        scanf("%100[^\n]", resp);
                    }
                    for (int i = 0; i < 300; i++)
                    {
                        if(!contas[i].email[0]){
                            strcpy(contas[i].email, email);
                            strcpy(contas[i].senha, senha1);
                            fprintf(db_contas, "%s ", email);
                            fprintf(db_contas, "%s", senha1);
                            fprintf(db_contas, "\n");
                            break;
                        }
                    }
                }

                add_produto(contas, db_produtos, email);
                break;
            case 2:
                //search_produto();
                break;
            default:
                break;
        }
    } while (opcao != 0);

    fclose(db_contas);


    return 0;
}

/// =-=-=-=-=-=-=-=-=-=-=-= FUNCOES =-=-=-=-=-=-=-=-=-=-=-=

void menu_print(){
    printf("\n--- MENU ---\n");
    printf("Adicionar produto -> 1\n");
    printf("Pesquisar produto -> 2\n");
    printf("Sair -> 0\n\n");
    printf("Escolha uma opcao: ");
}

void txt_struct_conta(FILE* arquivo, struct Conta* contas){
    char email[256] = {'\0'}, senha[256] = {'\0'};
    char aux = fscanf(arquivo, "%s %s", &email, &senha);

    int i = 0;
    while(aux != EOF) // TESTE PARA VER SE CHEGOU AO FIM
    {
        strcpy(contas[i].email, email);
        limpa(email);
        strcpy(contas[i].senha, senha);
        limpa(senha);
        aux = fscanf(arquivo, "%s %s", &email, &senha);
        i++;
    }
}

void txt_struct_produto(FILE* arquivo, struct Produto* produtos){
    char nome[256] = {'\0'}, codigo[256] = {'\0'};
    float preco;
    char aux = fscanf(arquivo, "%s %s %f", &nome, &codigo, &preco);

    int i = 0;
    while(aux != EOF) // TESTE PARA VER SE CHEGOU AO FIM
    {
        strcpy(produtos[i].nome, nome);
        limpa(nome);
        strcpy(produtos[i].codigo, codigo);
        limpa(codigo);
        produtos[i].preco = preco;
        aux = fscanf(arquivo, "%s %s %f", &nome, &codigo, &preco);
        i++;
    }
}

int valida_int(char* texto){
    if(strlen(texto) == 0){
        return 0;
    }
    for(int i = 0; texto[i]; i++){
        if(texto[i] < '0' || texto[i] > '9'){
            if(texto[i] == '-' && i != 0){
                return 0;
            }else if(texto[i] != '-'){
                return 0;
            }
        }
    }
    return 1;
}

int valida_float(char* texto){
    if(strlen(texto) == 0){
        return 0;
    }
    int pontos = 0;
    for(int i = 0; texto[i]; i++){
        if(texto[i] < '0' || texto[i] > '9'){
            if(texto[i] == '-'){
                return 0;
            }else{
                if(texto[i] == '.'){
                    pontos++;
                }else{
                    return 0;
                }
            }
        }
    }
    if(pontos > 1){
        return 0;
    }else{
        return 1;
    }
}

int valida_resp_menu(char* texto){
    if(strlen(texto) != 1){
        return 0;
    }
    return 1;
}

int valida_email(char* texto){
    if(strlen(texto) < strlen("a@gmail.com")){
        return 0;
    }
    int qtarroba = 0;
    int arroba = 0, ponto = 0;
    for(int i = 0; texto[i]; i++){
        if(texto[i] == '@'){
            arroba = i;
            qtarroba++;
        }
        if(texto[i] == '.'){
            ponto = i;
        }
        if(arroba > ponto && ponto > 0){
            return 0;
        }
        if(texto[i] == ' '){
            return 0;
        }
    }
    if(ponto == 0 || arroba == 0 || qtarroba > 1){
        return 0;
    }
    return 1;
}

int valida_senha(char* texto){
    if(strlen(texto) < 8){
        return 0;
    }
    for(int i = 0; texto[i]; i++){
        if(texto[i] == ' '){
            return 0;
        }
    }
    return 1;
}

int valida_nome(char* texto){
    for(int i = 0; texto[i]; i++){
        if(texto[i] < 'a' || texto[i] > 'z'){
            if (texto[i] < 'A' || texto[i] > 'Z'){
                if(texto[i] != ' '){
                    return 0;
                }
            }
        }
    }
    return 1;
}

int procura_email(char* email, struct Conta* contas){
    for(int i = 0; i < 300; i++){
        if(strcmp(email, contas[i].email) == 0){
            return 1;
        }
    }

    return 0;
}

int procura_senha(char* email, char* senha, struct Conta* contas){
    for(int i = 0; i < 300; i++){
        if(strcmp(contas[i].email, email) == 0 && strcmp(contas[i].senha, senha) == 0){
            return 1;
        }
    }
    return 0;
}

void add_produto(struct Produto* produtos, FILE* db_produtos, char* email_criador){
    char resp[256] = {'\0'}, nome[256] = {'\0'}, codigo[12] = {'\0'};
    int igualdade = 1;

    printf("Digite o nome desejado: ");
    setbuf(stdin, NULL);
    scanf("%200[^\n]", resp);
    while (!valida_nome(resp)){
        limpa(resp);
        printf("Digite um nome valido: ");
        setbuf(stdin, NULL);
        scanf("%200[^\n]", resp);
    }
    strcpy(nome, resp);
    tolower(nome);
    limpa(resp);

    printf("Digite o preco desejado: ");
    setbuf(stdin, NULL);
    scanf("%200[^\n]", resp);
    while (!valida_float(resp)){
        limpa(resp);
        printf("Digite um valor valido: ");
        setbuf(stdin, NULL);
        scanf("%200[^\n]", resp);
    }
    do{
        igualdade = 0;
        codigo[0] = '0' + rand()%10;
        codigo[1] = '0' + rand()%10;
        codigo[2] = '0' + rand()%10;
        codigo[3] = '0' + rand()%10;
        codigo[4] = '0' + rand()%10;
        codigo[5] = 'a' + rand()%26;
        codigo[6] = 'a' + rand()%26;
        codigo[7] = 'a' + rand()%26;
        codigo[8] = 'a' + rand()%26;
        codigo[9] = 'a' + rand()%26;
        for(int i = 0; produtos[i].codigo[0]; i++){
            if(strcmp(codigo, produtos[i].codigo) == 0){
                igualdade = 1;
                break;
            }
        }
    }while(igualdade);

    for (int i = 0; i < 300; i++){
        if(!produtos[i].nome[0]){
            strcpy(produtos[i].nome, nome);
            strcpy(produtos[i].codigo, codigo);
            sscanf(resp, "%f", &produtos[i].preco);
            fprintf(db_produtos, "%s ", email_criador);
            fprintf(db_produtos, "%s ", nome);
            fprintf(db_produtos, "%.3f ", produtos[i].preco);
            fprintf(db_produtos, "%s", codigo);
            fprintf(db_produtos, "\n");
            printf("Produto salvo!\n(Produto: %s)\n(Preco: R$ %.2f)\n(Codigo: %s)\n", nome, produtos[i].preco, codigo);
            break;
        }
    }


}

void limpa(char* texto){
    for(int i = 0; i < 256; i++){
        texto[i] = '\0';
    }
}

void tolower(char* texto){
    for(int i = 0; texto[i]; i++){
        if(texto[i] >= 'A' && texto[i] <= 'Z'){
            texto[i] += 32;
        }
    }
}