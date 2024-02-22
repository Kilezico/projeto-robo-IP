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

void delay(int delayTime) {
    usleep(delayTime * 1000);
}

void initSocket(int* sock, char* buffer){

    struct sockaddr_in serv_addr;

    // Create a socket
    if ((*sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        exit(1);
    }

    // Set the server address and port
    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, SERVER_ADDRESS, &serv_addr.sin_addr) <= 0) {
        printf("\n Invalid address/ Address not supported \n");
        exit(1);
    }

    // Connect to the server
    if (connect(*sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\n Connection Failed \n");
        exit(1);
    }
}

void movJoints(int sock, char* buffer, float j1, float j2, float j3, float j4, float j5,float j6,int state){
    sprintf(buffer, "%f,%f,%f,%f,%f,%f, %d", j1, j2, j3, j4, j5, j6, state);
    send(sock, buffer, strlen(buffer), 0);
    int valread = read(sock, buffer, 2048);
    printf("Result: %s\n", buffer);
}

// Nossas funções
// Função de mover juntas, mas que recebe um vetor, em vez de 6 inteiros
void movJointsVet(int sock, char* buffer, float pos[], int state) {
    movJoints(sock, buffer, pos[0], pos[1], pos[2], pos[3], pos[4], pos[5], state);
}

// Função de descobrir o próximo movimento
int *proxMovimento(int tabuleiro[][]) {

}

int main(int argc, char *argv[]) {
    
    //variaveis para conexcao com o robo NAO MEXER
    int sock = 0;
    char buffer[2048] = {0};
    
    //conexao com o robo NAO MEXER
    initSocket(&sock, buffer);
    
    
    //Programe aqui ----------------------------------------------------
    
    char casas[3][3];
    for(int i=0; i<3; i++) {
        for(int j=0; j<3;j++)
            casas[i][j] = 'n';
    }
    float angulospos[3][3][6] = {
        {
            {-0.27, -0.46, -0.67, -0.16, -0.37, -0.15},
            {-0.05, -0.45, -0.72, 0.05, -0.41, -0.07},
            {0.21, -0.46, -0.66, 0.19, -0.46, 0.1}
        },
        {
            {-0.23, -0.66, -0.34, -0.13, -0.56, -0.17},
            {-0.07, -0.64, -0.41, 0.11, -0.52, -0.17},
            {0.13, -0.69, -0.32, 0.18, -0.58, -0.04}
        },
        {
            {-0.16, -0.82, 0.01, -0.23, -0.74, 0.11},
            {-0.02, -0.81, -0.02, -0.11, -0.74, 0.11},
            {0.11, -0.77, -0.06, 0.07, -0.66, 0.07}
        }
    };

    float anguloPecaClose[6] = {-1.58,-0.47,-0.75,0.02,0.08,-0.01};
    float anguloPecaOpen[6] = {-1.59,-0.37,-0.72,0.00,-0.10,0.01};

    // Movimento
    for (int i=0; i<3; i++) {
        for (int j=0; j<3; j++) {
            movJointsVet(sock, buffer, anguloPecaOpen, OPEN);
            delay(500);
            movJointsVet(sock, buffer, anguloPecaClose, CLOSE);
            delay(500);
            movJointsVet(sock, buffer, anguloPecaOpen, CLOSE);
            delay(1000);
            movJointsVet(sock, buffer, angulospos[i][j], OPEN);
            delay(200);
        }
    }

    //-----------------------------------------------------------------
    // Encerra a conexao NAO MEXER
    close(sock);

    return 0;
}
