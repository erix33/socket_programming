#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
    #include <winsock2.h>
#endif
int main() {
    // 1. Chiediamo al sistema operativo un socket TCP (IPv4)
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        perror("Errore nella creazione del socket");
        return EXIT_FAILURE;
    }

    printf("Socket creato con successo! Il file descriptor è: %d\n", sockfd);

    // 2. Lo chiudiamo subito (per ora ci interessava solo crearlo)
    close(sockfd);
    printf("Socket chiuso correttamente.\n");

    return EXIT_SUCCESS;
}