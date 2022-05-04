#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <iostream>
#include <fstream>

using namespace std;
int pagina;
char* userCode;
const char* userType;

void readFile(const char* arquivo){
    ifstream ip(arquivo);
    if(!ip.is_open()){
        printf("\n\n\nErro ao obter eventos dispon�veis\n\n\n");
    }else{
        if(strcmp( arquivo, "events.csv") == 0){

                string code, name, local, hours;

                while(ip.good()){
                    getline(ip, code, ',');
                    getline(ip, name, ',');
                    getline(ip, local, ',');
                    getline(ip, hours, '\n');
                    printf("%s\t\t\t%s\t\t\t%s\t\t\t%s\n", code.c_str(), name.c_str(), local.c_str(), hours.c_str());
                };
        }else if(strcmp( arquivo, "users.csv") == 0){
                string code, type;
                while(ip.good()){
                    getline(ip, code, ',');
                    getline(ip, type, ',');
                    if(code == userCode){

                        break;
                    };
                };
            };
        };
};
void readEventsFile(){
    readFile("events.csv");
}
void readUsersFile(){
    readFile("users.csv");
}

void writeFile(FILE* arquivo, const char* tipo){
    if(arquivo == NULL){
        printf("\n\n\nErro ao obter eventos disponíveis\n\n\n");
    }else{
        if(strcmp( tipo, "events") == 0){
            char codigo[128], nome[128], local[128];
            int qtdHoras = 0;
            printf("\nDigite o codigo do evento:\n");
            scanf("%s",codigo);
            printf("\nDigite o nome do evento:\n");
            scanf("%s",nome);
            printf("\nDigite o local do evento:\n");
            scanf("%s",local);
            printf("\nDigite a qtd. de horas extracurriculares do evento:\n");
            scanf("%d", &qtdHoras);

            fprintf(arquivo, "%s,%s,%s,%d\n", codigo,nome,local,qtdHoras);
        }else if(strcmp( tipo, "users") == 0){
            char nome[128], email[128], senha[128];
            printf("\nDigite o nome do usuario:\n" );
            scanf("%s",nome);
            printf("\nDigite o email do usuario:\n");
            scanf("%s",email);
            printf("\nDigite a senha do usuario:\n");
            scanf("%s",senha);

            fprintf(arquivo, "%s,%s,%s\n", nome,email,senha);
        }else if(strcmp( tipo, "tickets") == 0){
                char id[128], usuario[128], evento[128];
                printf("\nDigite o id do ingresso:\n");
                scanf("%s",id);
                printf("\nDigite o codigo do usuario:\n");
                scanf("%s",usuario);
                printf("\nDigite o codigo do evento:\n");
                scanf("%s",evento);

                fprintf(arquivo, "%s,%s,%s\n", id,usuario,evento);
        }else{
            printf("Tipo de escrita nao encontrado");
        };

        fclose(arquivo);
    }
}
void writeTicketsFile(){
    FILE* arquivo = fopen("tickets.csv", "a+");
    writeFile(arquivo, "tickets");
}
void writeEventsFile(){
    FILE* arquivo = fopen("events.csv", "a+");
    writeFile(arquivo, "events");
}

void paginacao(){
    printf("%s", userType);
    if(strcmp( userType, "admin") == 0){
        printf("\nDigite o valor referente a Funcionalidade: \n1 ==== Visualizar Eventos\n2 ==== Adicionar Evento\n3 ==== Sair\n\n\nFuncionalidade Escolhida: ");
        scanf("%d", &pagina);

        switch(pagina){
        case 1:
            readEventsFile();
            break;
        case 2:
            printf("\nAdquirir Ingresso!\n");
            writeEventsFile();
            break;
        case 3:
            break;
        default:
            printf("\nPágina não existente!\n");
        }

        while(pagina != 3){
            paginacao();
        };
    }else{
        printf("\nDigite o valor referente a Funcionalidade: \n1 ==== Visualizar Eventos\n2 ==== Adquirir Ingresso\n3 ==== Sair\n\n\nFuncionalidade Escolhida: ");
        scanf("%d", &pagina);

        switch(pagina){
        case 1:
            readEventsFile();
            break;
        case 2:
            printf("\nAdquirir Ingresso!\n");
            writeEventsFile();
            break;
        case 3:
            break;
        default:
            printf("\nPágina não existente!\n");
        }

        while(pagina != 3){
            paginacao();
        };
    }


}

int main(){
    setlocale(LC_ALL, "Portuguese");
    //Login
    printf("Digite seu código de usuário:");
    scanf("%s",&userCode);
    readUsersFile();

    //Navegação
    paginacao();

    printf("\n\n\nObrigado por utilizar nosso sistema!\n\n\n");

    return 0;
}
