#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT 8888

#define OPEN 1
#define CLOSE 0
void delay(int delayTime)
{
    usleep(delayTime * 1000);
}

void initSocket(int *sock, char *buffer)
{

    struct sockaddr_in serv_addr;

    // Create a socket
    if ((*sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        exit(1);
    }

    // Set the server address and port
    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, SERVER_ADDRESS, &serv_addr.sin_addr) <= 0)
    {
        printf("\n Invalid address/ Address not supported \n");
        exit(1);
    }

    // Connect to the server
    if (connect(*sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\n Connection Failed \n");
        exit(1);
    }
}

void movJoints(int sock, char *buffer, float j1, float j2, float j3, float j4, float j5, float j6, int state)
{
    sprintf(buffer, "%f,%f,%f,%f,%f,%f, %d", j1, j2, j3, j4, j5, j6, state);
    send(sock, buffer, strlen(buffer), 0);
    int valread = read(sock, buffer, 2048);
    printf("Result: %s\n", buffer);
}

// Nossas funções
// Movimentos de juntas para jogar uma peça
void colocaPeca(int sock, char *buffer, float posJogada[6], float pecaOpen[6], float pecaClose[6], float posIni[6]) {
    movJoints(sock, buffer, pecaOpen[0], pecaOpen[1], pecaOpen[2], pecaOpen[3], pecaOpen[4], pecaOpen[5], OPEN);
    delay(500);
    movJoints(sock, buffer, pecaClose[0], pecaClose[1], pecaClose[2], pecaClose[3], pecaClose[4], pecaClose[5], CLOSE);
    delay(500);
    movJoints(sock, buffer, pecaOpen[0], pecaOpen[1], pecaOpen[2], pecaOpen[3], pecaOpen[4], pecaOpen[5], CLOSE);
    delay(1000);
    movJoints(sock, buffer, posJogada[0], posJogada[1], posJogada[2], posJogada[3], posJogada[4], posJogada[5], OPEN);
    delay(200);
    movJoints(sock, buffer, posIni[0], posIni[1], posIni[2], posIni[3], posIni[4], posIni[5], OPEN);
}

char conversor(int i) // Para printar no terminal.
{
    switch (i)
    {
    case -1:
        return 'X';
    case 0:
        return ' ';
    case 1:
        return 'O';
    }
}

void printando(int board[3][3])
{
    printf(" %c | %c | %c\n", conversor(board[0][0]), conversor(board[0][1]), conversor(board[0][2]));
    printf("---+---+---\n");
    printf(" %c | %c | %c\n", conversor(board[1][0]), conversor(board[1][1]), conversor(board[1][2]));
    printf("---+---+---\n");
    printf(" %c | %c | %c\n", conversor(board[2][0]), conversor(board[2][1]), conversor(board[2][2]));
}

int vitoria(const int board[3][3])
{
    // Determina se um jogador venceu, retorna 0 caso contrário.
    unsigned vitorias[8][3] = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}, {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, {0, 4, 8}, {2, 4, 6}};
    int i;
    for (i = 0; i < 8; i++)
    {
        int a = vitorias[i][0], b = vitorias[i][1], c = vitorias[i][2];
        if (board[a / 3][a % 3] != 0 &&
            board[a / 3][a % 3] == board[b / 3][b % 3] &&
            board[a / 3][a % 3] == board[c / 3][c % 3])
            return board[c / 3][c % 3];
    }
    return 0;
}

int minimax(int board[3][3], int jogador) // para a maquina vai ser jogador=1 e para vc é jogador=-1
{
    int acabou = vitoria(board); // verifica se ja acabou ou não a partida
    if (acabou != 0)
        return acabou * jogador;

    int move_i = -1, move_j = -1;
    int melhorpontuacao = -2; // Movimentos que levam à derrota são preferidos a não fazer nada
    // coloquei o score sendo =-2 pois se fosse maior que alguma das possibilidades poderia ocasionar problemas com o codigo, sendo assim ele é -2 apenas para inicializar
    int i, j;
    int pontuacao;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (board[i][j] == 0) // se tiver livre para poder fazer jogadas, entra nesse if
            {
                board[i][j] = jogador;
                pontuacao = -minimax(board, jogador * -1); // aqui começo a puxar a recursão do minmax
                                                         // EXPLICAÇÂO
                                                         /*Colocamos o "-" antes do minmax para poder inverter o valor da pontuacao do jogador a frente, pois como faço jogador*-1 para poder trocar quem vai jogar no min e max
                                                         se nao tivesse esse - o valor vinha positivo quando fosse para ser negativo, dessa forma fazendo com que o min e max perca eficiencia
                                                         Com esse pequeno ajuste quando ele for jogar para prever possibilidades do player não ira cofundir com a maquina e será feliz!*/
                if (melhorpontuacao < pontuacao)
                {
                    melhorpontuacao = pontuacao;
                    move_i = i;
                    move_j = j;
                }
                board[i][j] = 0; // reseta apos tentativa de min e max
            }
        }
    }
    if (move_i == -1)
        return 0;
    return melhorpontuacao;
}

int movrobo(int board[3][3], int *roboi, int *roboj)
{ // A função movrobo é responsável por determinar qual será a próxima jogada do computador, colocando cada casa vazia como algo e chamando o minmax para poder simular o valor do tabuleiro
    // fazendo assim a melhor escolha para o robo
    int move_i = -1, move_j = -1;
    int maxpont = -2;
    int i, j;
    for (i = 0; i < 3; ++i)
    {
        for (j = 0; j < 3; ++j)
        {
            if (board[i][j] == 0)
            {
                board[i][j] = 1;
                int pontuacao = -minimax(board, -1);
                board[i][j] = 0;
                if (pontuacao > maxpont)
                {
                    maxpont = pontuacao;
                    move_i = i;
                    move_j = j;
                }
            }
        }
    }
    *roboi = move_i;
    *roboj = move_j;
    
    // Faz a jogada do computador
    board[move_i][move_j] = 1;
}

int main(int argc, char *argv[])
{

    // variaveis para conexao com o robo NAO MEXER
    int sock = 0;
    char buffer[2048] = {0};

    // conexao com o robo NAO MEXER
    initSocket(&sock, buffer);

    // Programe aqui ----------------------------------------------------
    
    int jogador;
    int jogadap_i, jogadap_j, jogadac_i, jogadac_j;

    int casas[3][3] = {0}; // Inicia todas as casas com 0 

    float anguloCasas[3][3][6] = {
        {{-0.27, -0.46, -0.67, -0.16, -0.37, -0.15},
         {-0.05, -0.45, -0.72, 0.05, -0.41, -0.07},
         {0.21, -0.46, -0.66, 0.19, -0.46, 0.1}},
        {{-0.23, -0.66, -0.34, -0.13, -0.56, -0.17},
         {-0.07, -0.64, -0.41, 0.11, -0.52, -0.17},
         {0.13, -0.69, -0.32, 0.18, -0.58, -0.04}},
        {{-0.16, -0.82, 0.01, -0.23, -0.74, 0.11},
         {-0.02, -0.81, -0.02, -0.11, -0.74, 0.11},
         {0.11, -0.77, -0.06, 0.07, -0.66, 0.07}}};

    float anguloPecaOpen[6] = {-1.59, -0.37, -0.72, 0.00, -0.10, 0.01};
    float anguloPecaClose[6] = {-1.58, -0.47, -0.75, 0.02, 0.08, -0.01};
    float anguloPosInicial[6] = {0}; // Posição que o robô vai ficar enquanto espera a jogada do adversário.

    printf("Deseja jogar primeiro? (1)Sim (2)Não: ");
    scanf("%d", &jogador);
    for (int round = 0; round < 9 && vitoria(casas) == 0; round++)
    {
        if ((round + jogador) % 2 == 0)
        {
            // Pega jogada do robo
            movrobo(casas, &jogadac_i, &jogadac_j);
            // Move
            colocaPeca(sock, buffer, anguloCasas[jogadac_i][jogadac_j], anguloPecaOpen, anguloPecaClose, anguloPosInicial);
        }
        else
        {
            // Pega jogada do jogador
            printf("Digite sua jogada na forma(x,y): ");
            scanf("(%d,%d)", &jogadap_i, &jogadap_j);
            // Move  
            colocaPeca(sock, buffer, anguloCasas[jogadap_i][jogadap_j], anguloPecaOpen, anguloPecaClose, anguloPosInicial);
        }
    }

    // Printa o resultado final.
    if (vitoria(casas) == 1) printf("O ROBO venceu esse embate!\n");
    else if (vitoria(casas) == -1) printf("Os seres humanos ganharam essa...\n");
    else printf("Eita! Deu velha!\n");

    //-----------------------------------------------------------------
    // Encerra a conexao NAO MEXER
    close(sock);

    return 0;
}
