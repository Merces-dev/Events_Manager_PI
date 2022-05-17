#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <iostream>
#include <fstream>

using namespace std;
int pagina, userType = 0;
float horasAcumuladas = 0;
char userCode[20], validationCode[20];

char* generateTicket (char* tipo){
    char* ch = new char;
    strcpy(ch,tipo);
    strcat(ch,userCode);
    return ch;
}

void readFile(std::ifstream &ip, const char *tipo)
{
    if (!ip.is_open())
    {
        printf("\n\n\nErro ao ler arquivo\n\n\n");
    }
    else
    {
        FILE *ticketsBackup;
        if(strcmp(tipo, "validationTickets") == 0){
            ticketsBackup = fopen("./dados/backup/tickets.csv","a+");
        }
        while (ip.good()){
            if (strcmp(tipo, "events") == 0 )
            {
                string code, name, local, hours;

                getline(ip, code, ',');
                getline(ip, name, ',');
                getline(ip, local, ',');
                getline(ip, hours, '\n');
                printf("\n\n%s\t%s\t%s\t%s\n", code.c_str(), name.c_str(), local.c_str(), hours.c_str());
            }
            else if (strcmp(tipo, "ownTickets") == 0 )
            {
                string eventCode, eventId, hours, status, userId;

                getline(ip, eventCode, ',');
                getline(ip, userId, ',');
                getline(ip, eventId, ',');
                getline(ip, hours, ',');
                getline(ip, status, '\n');
                if (strcmp(userId.c_str(), userCode) == 0){
                    printf("%s\t%s\t%s\t%s\t%s\n", eventCode.c_str(), userId.c_str(), eventId.c_str(), hours.c_str(), status.c_str());
                    if(status == "1"){
                        horasAcumuladas += std::stoi( hours );
                    };
                };
            }
            else if (strcmp(tipo, "validationTickets") == 0 )
            {
                string ticketCode, eventId, status, userId;

                getline(ip, ticketCode, ',');
                getline(ip, userId, ',');
                getline(ip, eventId, ',');
                getline(ip, status, '\n');
                if(ticketCode.c_str() != NULL){
                        string res;
                        res.append(ticketCode.c_str());
                        res.append(",");
                        res.append(userId.c_str());
                        res.append(",");
                        res.append(eventId.c_str());
                        res.append(",");

                        if (strcmp(ticketCode.c_str(), validationCode) == 0){
                            res.append("1");
                        }else{
                            res.append(status.c_str());
                        }
                        if(res != ",,,"){
                            res.append("\n");
                            cout << res;
                            fprintf(ticketsBackup, res.c_str());
                        };
                    };
            }
            else if (strcmp(tipo, "users") == 0)
            {
                string code, type;
                getline(ip, code, ',');
                getline(ip, type, '\n');
                if (strcmp(code.c_str(), userCode) == 0)
                {
                    if (strcmp(type.c_str(), "admin") == 0)
                    {
                        userType = 1;
                    }
                    else
                    {
                        userType = 2;
                    };
                };
            };
        };
        ip.close();

        if(strcmp(tipo, "validationTickets") == 0){
            fclose(ticketsBackup);
            FILE *ticketBackup = fopen("./dados/backup/tickets.csv","r");
            FILE *tickets = fopen("./dados/tickets.csv","a+");
            char leitor[1000];

            while(fgets(leitor, 1000, ticketBackup) != NULL);
            fputs(leitor, tickets);
        }
    };
};


void readEventsFile()
{
    ifstream ip("./dados/events.csv");
    readFile(ip, "events");
}
void readTicketsFile()
{
    ifstream ip("./dados/tickets.csv");
    readFile(ip, "ownTickets");
}
void ticketValidation()
{
    printf("Digite o codigo de validacao:");
    scanf("%s", validationCode);
    ifstream ip("./dados/tickets.csv");
    readFile(ip, "validationTickets");
}
void readUsersFile()
{
    ifstream ip("./dados/users.csv");
    readFile(ip, "users");
}


void login(){
    setlocale(LC_ALL, "Portuguese");
    printf("Digite seu codigo de usuario:");
    scanf("%s", userCode);

    readUsersFile();
}


void writeFile(FILE *arquivo, const char *tipo)
{
    if (arquivo == NULL)
    {
        printf("\n\n\nErro ao obter eventos disponiveis\n\n\n");
    }
    else
    {
        if (strcmp(tipo, "events") == 0)
        {
            char codigo[128], nome[128], local[128];
            int qtdHoras = 0;
            printf("\nDigite o codigo do evento:\n");
            scanf("%s", codigo);
            printf("\nDigite o nome do evento:\n");
            scanf("%s", nome);
            printf("\nDigite o local do evento:\n");
            scanf("%s", local);
            printf("\nDigite a qtd. de horas extracurriculares do evento:\n");
            scanf("%d", &qtdHoras);

            fprintf(arquivo, "%s,%s,%s,%d\n", codigo, nome, local, qtdHoras);
        }
        else if (strcmp(tipo, "users") == 0)
        {
            char nome[128], email[128], senha[128];
            printf("\nDigite o nome do usuario:\n");
            scanf("%s", nome);
            printf("\nDigite o email do usuario:\n");
            scanf("%s", email);
            printf("\nDigite a senha do usuario:\n");
            scanf("%s", senha);

            fprintf(arquivo, "%s,%s,%s\n", nome, email, senha);
        }
        else if (strcmp(tipo, "tickets") == 0)
        {
            char evento[128];
            printf("\nDigite o codigo do evento:\n");
            scanf("%s", evento);
            char* id = generateTicket(evento);
            float hours = 0;
            //TODO: Verificar se o evento existe, se o usuário já não se cadastrou no mesmo e pegar as horas do evento
            printf("\n\n\n\n##### Anote seu ingresso #####\nCodigo: %s\n\n\n", id);
            fprintf(arquivo, "%s,%s,%s,%2.f,%d\n", id, userCode, evento, hours, 0);
        }
        else
        {
            printf("Tipo de escrita nao encontrado");
        };

        fclose(arquivo);
    }
}


void writeTicketsFile()
{
    FILE *arquivo = fopen("./dados/tickets.csv", "a+");
    writeFile(arquivo, "tickets");
}


void writeEventsFile()
{
    FILE *arquivo = fopen("./dados/events.csv", "a+");
    writeFile(arquivo, "events");
}


void paginacao()
{
    switch(userType) {
        case 1:
            printf("\nDigite o valor referente a Funcionalidade: \n1 ==== Visualizar Eventos\n2 ==== Adicionar Evento\n3 ==== Validar Ingresso\n4 ==== Sair\n\n\nFuncionalidade Escolhida: ");
            scanf("%d", &pagina);

            switch (pagina)
            {
            case 1:
                readEventsFile();
                break;
            case 2:
                writeEventsFile();
                break;
            case 3:
                ticketValidation();
                break;
            case 4:
                break;
            default:
                printf("\nPagina nao existente!\n");
            }

            while (pagina != 4)
            {
                paginacao();
            };
            break;
        case 2:
            printf("\nDigite o valor referente a Funcionalidade: \n1 ==== Visualizar Eventos\n2 ==== Adquirir Ingresso\n3 ==== Minhas Horas Complementares\n4 ==== Sair\n\n\nFuncionalidade Escolhida: ");
            scanf("%d", &pagina);

            switch (pagina)
            {
            case 1:
                readEventsFile();
                break;
            case 2:
                writeTicketsFile();
                break;
            case 3:
                readTicketsFile();
                break;
            case 4:
                break;
            default:
                printf("\nPagina nao existente!\n");
            }
            while (pagina != 4)
            {
                paginacao();
            }
            break;
        default:
            printf("Tipo de usuario nao encontrado. Tipo:%d",userType);
    }
}

int main()
{
    // Login
    login();

    // Navegação
    paginacao();

    printf("\n\n\nObrigado por utilizar nosso sistema!\n\n\n");

    return 0;
}
