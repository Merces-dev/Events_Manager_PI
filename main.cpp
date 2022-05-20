#include <conio.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include <fstream>
#include <iostream>
#include <string>
// SOFTWARE PARA ADMINISTRAÇÃO DOS EVENTOS DA FACULDADE
using namespace std;
int pagina, userType = 0, horasAcumuladas = 0, eventoHoras = 0, validacaoStatus = 0;
char userCode[20], userPwd[20], validationCode[20];
string eventCodeValidation;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

char *generateTicket(char *tipo) {
    char *ch = new char;
    strcpy(ch, tipo);
    strcat(ch, "-");
    strcat(ch, userCode);
    return ch;
}

void readFile(std::ifstream &ip, const char *tipo) {
    int inc = 0;
    if (!ip.is_open()) {
        printf("\n\n\nErro ao ler arquivo\n\n\n");
    } else {
        FILE *ticketsBackup;
        if (strcmp(tipo, "validationTickets") == 0) {
            ticketsBackup = fopen("./dados/backup/tickets.csv", "w+");
        }
        validacaoStatus = 0;
        while (ip.good()) {
            if (strcmp(tipo, "events") == 0) {
                string code, name, local, hours;

                getline(ip, code, ',');
                getline(ip, name, ',');
                getline(ip, local, ',');
                getline(ip, hours, '\n');
                if (name.length() > 0) {
                    inc++;
                    SetConsoleTextAttribute(hConsole, 8);
                    printf("Codigo do evento: %s\n", code.c_str());
                    SetConsoleTextAttribute(hConsole, 7);
                    printf("Nome do evento: %s\nLocal: %s\nHoras curriculares: %s\n\n", name.c_str(), local.c_str(), hours.c_str());
                };
            } else if (strcmp(tipo, "eventsHours") == 0) {
                inc++;
                string code, name, local, hours;

                getline(ip, code, ',');
                getline(ip, name, ',');
                getline(ip, local, ',');
                getline(ip, hours, '\n');
                if (strcmp(code.c_str(), eventCodeValidation.c_str()) == 0) {
                    eventoHoras = stoi(hours);
                };
            } else if (strcmp(tipo, "ownTickets") == 0) {
                string eventCode, eventId, hours, status, userId;

                getline(ip, eventCode, ',');
                getline(ip, userId, ',');
                getline(ip, eventId, ',');
                getline(ip, hours, ',');
                getline(ip, status, '\n');
                if (strcmp(userId.c_str(), userCode) == 0) {
                    inc++;
                    SetConsoleTextAttribute(hConsole, 8);
                    printf("Codigo do ingresso: %s\n", eventCode.c_str());
                    SetConsoleTextAttribute(hConsole, 7);
                    printf("Codigo Evento: %s\nHoras complementares: %s\nValido: %s\n\n", eventId.c_str(), hours.c_str(), status.c_str());
                    if (status == "1") {
                        // Incrementa horas acumuladas dos tickets ativos do usuario
                        int horasAcumuladasAtivas = stoi(hours);
                        horasAcumuladas += horasAcumuladasAtivas;
                    };
                };
            } else if (strcmp(tipo, "validationTickets") == 0) {
                string ticketCode, eventId, status, hours, userId;

                getline(ip, ticketCode, ',');
                getline(ip, userId, ',');
                getline(ip, eventId, ',');
                getline(ip, hours, ',');
                getline(ip, status, '\n');
                if (ticketCode.c_str() != NULL) {
                    string res;
                    res.append(ticketCode.c_str());
                    res.append(",");
                    res.append(userId.c_str());
                    res.append(",");
                    res.append(eventId.c_str());
                    res.append(",");
                    res.append(hours.c_str());
                    res.append(",");

                    if (strcmp(ticketCode.c_str(), validationCode) == 0) {
                        validacaoStatus++;
                        res.append("1");
                    } else {
                        res.append(status.c_str());
                    }
                    if (res != ",,,,") {
                        res.append("\n");
                        inc++;
                        fprintf(ticketsBackup, res.c_str());
                    };
                };
            } else if (strcmp(tipo, "users") == 0) {
                inc++;
                string code, pwd, type;
                getline(ip, code, ',');
                getline(ip, pwd, ',');
                getline(ip, type, '\n');
                if (strcmp(code.c_str(), userCode) == 0 && strcmp(pwd.c_str(), userPwd) == 0) {
                    if (strcmp(type.c_str(), "admin") == 0) {
                        userType = 1;
                    } else {
                        userType = 2;
                    };
                };
            };
        };
        ip.close();
        if (inc <= 0) {
            printf("\n\nSEM DADOS PARA VISUALIZACAO");
        } else if (strcmp(tipo, "validationTickets") == 0) {
            fclose(ticketsBackup);
            ifstream ticketBackup("./dados/backup/tickets.csv");
            FILE *tickets = fopen("./dados/tickets.csv", "w+");
            while (ticketBackup.good()) {
                string ticketCode, userId, eventId, hours, status;
                getline(ticketBackup, ticketCode, ',');
                getline(ticketBackup, userId, ',');
                getline(ticketBackup, eventId, ',');
                getline(ticketBackup, hours, ',');
                getline(ticketBackup, status, '\n');
                if (ticketCode.length() > 0) {
                    fprintf(tickets, "%s,%s,%s,%s,%s\n", ticketCode.c_str(), userId.c_str(), eventId.c_str(), hours.c_str(), status.c_str());
                };
            };
            ticketBackup.close();
            fclose(tickets);
        };
    };
};

void readEventsFile() {
    SetConsoleTextAttribute(hConsole, 6);
    printf("\n\n####### EVENTOS #######\n\n");
    SetConsoleTextAttribute(hConsole, 7);
    ifstream ip("./dados/events.csv");
    readFile(ip, "events");
}
void readTicketsFile() {
    horasAcumuladas = 0;
    SetConsoleTextAttribute(hConsole, 6);
    printf("\n\n####### INGRESSOS #######\n\n");
    SetConsoleTextAttribute(hConsole, 7);
    ifstream ip("./dados/tickets.csv");
    readFile(ip, "ownTickets");
    SetConsoleTextAttribute(hConsole, 3);
    printf("\nHoras Complementares Ativas: %d\n\n\n\n", horasAcumuladas);
    SetConsoleTextAttribute(hConsole, 7);
}

void readUsersFile() {
    ifstream ip("./dados/users.csv");
    readFile(ip, "users");
}

void readEventHoursFile() {
    ifstream ip("./dados/events.csv");
    readFile(ip, "eventsHours");
}

void ticketValidation() {
    int validacaoIngressoExistente = 0;

    SetConsoleTextAttribute(hConsole, 6);
    printf("\n\n####### VALIDAR INGRESSO #######\n\n");
    SetConsoleTextAttribute(hConsole, 7);

    printf("Digite o codigo de validacao: ");
    scanf("%s", validationCode);
    ifstream ip("./dados/tickets.csv");
    while (ip.good()) {
        string ticketCode, userId, eventId, eventHours, status;

        getline(ip, ticketCode, ',');
        getline(ip, userId, ',');
        getline(ip, eventId, ',');
        getline(ip, eventHours, ',');
        getline(ip, status, '\n');
        if (ticketCode == validationCode && status == "1") {
            validacaoIngressoExistente++;
        };
    };
    ip.close();
    if (validacaoIngressoExistente == 0) {
        ifstream ip("./dados/tickets.csv");
        readFile(ip, "validationTickets");
        if (validacaoStatus <= 0) {
            SetConsoleTextAttribute(hConsole, 12);
            printf("\nIngresso nao encontrado. Ingresso: %s\n", validationCode);
            SetConsoleTextAttribute(hConsole, 7);
        }else{
            SetConsoleTextAttribute(hConsole, 6);
            printf("\nIngresso validado com sucesso. Ingresso: %s\n", validationCode);
            SetConsoleTextAttribute(hConsole, 7);
        };
    } else {
        SetConsoleTextAttribute(hConsole, 12);
        printf("\nIngresso ja validado. Ingresso: %s\n", validationCode);
        SetConsoleTextAttribute(hConsole, 7);
    };

    printf("\n\n\n\n");
}

void login() {
    do {
        SetConsoleTextAttribute(hConsole, 6);
        printf("\n\n\n\n\n####### LOGIN #######\n\n");
        SetConsoleTextAttribute(hConsole, 7);
        setlocale(LC_ALL, "Portuguese");
        printf("Digite seu codigo de usuario: ");
        scanf("%s", userCode);
        printf("Digite sua senha: ");
        scanf("%s", userPwd);
        readUsersFile();
        if (userType == 0) {
            SetConsoleTextAttribute(hConsole, 12);
            printf("\nCodigo ou Senha invalidos\n");
            SetConsoleTextAttribute(hConsole, 7);
        };
    } while (userType == 0);

    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
}
int ifEventExistsReturnItsHours() {
    eventoHoras = 0;
    readEventHoursFile();
    return eventoHoras;
}

void writeFile(FILE *arquivo, const char *tipo) {
    if (arquivo == NULL) {
        printf("\n\n\nErro ao obter eventos disponiveis\n\n\n");
    } else {
        if (strcmp(tipo, "events") == 0) {
            char codigo[128], nome[128], local[128];
            int qtdHoras = 0;
            printf("\nDigite o codigo do evento: ");
            scanf("%s", codigo);
            printf("\nDigite o nome do evento: ");
            scanf("%s", nome);
            printf("\nDigite o local do evento: ");
            scanf("%s", local);
            printf("\nDigite a qtd. de horas extracurriculares do evento: ");
            scanf("%d", &qtdHoras);

            fprintf(arquivo, "%s,%s,%s,%d\n", codigo, nome, local, qtdHoras);
        } else if (strcmp(tipo, "users") == 0) {
            char nome[128], email[128], senha[128];
            printf("\nDigite o nome do usuario: ");
            scanf("%s", nome);
            printf("\nDigite o email do usuario: ");
            scanf("%s", email);
            printf("\nDigite a senha do usuario: ");
            scanf("%s", senha);

            fprintf(arquivo, "%s,%s,%s\n", nome, email, senha);
        } else if (strcmp(tipo, "tickets") == 0) {
            char evento[128];
            int validacaoIngressoExistente = 0;
            printf("\nDigite o codigo do evento: ");
            scanf("%s", evento);
            char *id = generateTicket(evento);
            // TODO: Verificar se o evento existe, se o usuário já não se cadastrou no mesmo e pegar as horas do evento
            eventCodeValidation = evento;
            int hours = ifEventExistsReturnItsHours();
            ifstream ip("./dados/tickets.csv");
            while (ip.good()) {
                string ticketCode, userId, eventId, eventHours, status;

                getline(ip, ticketCode, ',');
                getline(ip, userId, ',');
                getline(ip, eventId, ',');
                getline(ip, eventHours, ',');
                getline(ip, status, '\n');
                if (ticketCode == id) {
                    validacaoIngressoExistente++;
                };
            };
            ip.close();
            if (validacaoIngressoExistente != 0) {
                SetConsoleTextAttribute(hConsole, 12);
                printf("\nIngresso ja existente. Ingresso: %s\n", id);
                SetConsoleTextAttribute(hConsole, 7);
            } else if (hours == 0) {
                SetConsoleTextAttribute(hConsole, 12);
                printf("\nEvento nao encontrado! Evento: %s\n", evento);
                SetConsoleTextAttribute(hConsole, 7);
            } else {
                SetConsoleTextAttribute(hConsole, 3);
                printf("\n\n\n\n##### Anote seu ingresso #####");
                SetConsoleTextAttribute(hConsole, 7);
                printf("\nCodigo: %s\n\n\n", id);
                fprintf(arquivo, "%s,%s,%s,%d,%d\n", id, userCode, evento, hours, 0);
            };
        } else {
            SetConsoleTextAttribute(hConsole, 12);
            printf("\nTipo de escrita nao encontrado\n");
            SetConsoleTextAttribute(hConsole, 7);
        };

        fclose(arquivo);
    };
};

void writeTicketsFile() {
    SetConsoleTextAttribute(hConsole, 6);
    printf("\n\n####### ADQUIRIR INGRESSO #######\n\n");
    SetConsoleTextAttribute(hConsole, 7);
    FILE *arquivo = fopen("./dados/tickets.csv", "a+");
    writeFile(arquivo, "tickets");
}

void writeEventsFile() {
    SetConsoleTextAttribute(hConsole, 6);
    printf("\n\n####### ADICIONAR EVENTO #######\n\n");
    SetConsoleTextAttribute(hConsole, 7);
    FILE *arquivo = fopen("./dados/events.csv", "a+");
    writeFile(arquivo, "events");
}

void paginacao() {
    SetConsoleTextAttribute(hConsole, 6);
    printf("\n\n####### HOME #######\n\n");
    SetConsoleTextAttribute(hConsole, 7);
    switch (userType) {
        case 1:
            printf("\nDigite o valor referente a Funcionalidade: \n1 ==== Visualizar Eventos\n2 ==== Adicionar Evento\n3 ==== Validar Ingresso\n4 ==== Sair\n\n\nFuncionalidade Escolhida: ");
            scanf("%d", &pagina);

            switch (pagina) {
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
                    SetConsoleTextAttribute(hConsole, 12);
                    printf("\nPagina nao existente!\n");
                    SetConsoleTextAttribute(hConsole, 7);
            }

            while (pagina != 4) {
                paginacao();
            };
            break;
        case 2:
            printf("\nDigite o valor referente a Funcionalidade: \n1 ==== Visualizar Eventos\n2 ==== Adquirir Ingresso\n3 ==== Minhas Horas Complementares\n4 ==== Sair\n\n\nFuncionalidade Escolhida: ");
            scanf("%d", &pagina);

            switch (pagina) {
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
                    SetConsoleTextAttribute(hConsole, 12);
                    printf("\nPagina nao existente!\n");
                    SetConsoleTextAttribute(hConsole, 7);
            }
            while (pagina != 4) {
                paginacao();
            }
            break;
        default:
            SetConsoleTextAttribute(hConsole, 12);
            printf("Tipo de usuario nao encontrado. Tipo:%d", userType);
            SetConsoleTextAttribute(hConsole, 7);
    }
}

int main() {
    SetConsoleTextAttribute(hConsole, 7);
    printf("#################################################################\n#####");
    SetConsoleTextAttribute(hConsole, 6);
    printf(" SOFTWARE PARA ADMINISTRACAO DOS EVENTOS DA FACULDADE ");
    SetConsoleTextAttribute(hConsole, 7);
    printf("######\n#################################################################");
    // Login
    login();

    // Navegação
    paginacao();
    SetConsoleTextAttribute(hConsole, 6);
    printf("\n\n\nObrigado por utilizar nosso sistema!\n\n\n");
    SetConsoleTextAttribute(hConsole, 7);

    return 0;
}
