#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
    // Configurazione specifica per Windows
    #include <winsock2.h>
    #include <ws2tcpip.h> // Include funzioni moderne per la gestione degli indirizzi
    
    // Dice al compilatore (se usi MSVC/Visual Studio) di collegare la libreria di rete
    #pragma comment(lib, "ws2_32.lib") 
#else
    // Configurazione specifica per Linux / macOS
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <unistd.h>     // Necessario per close()

    // Uniformiamo i tipi di dati definendo i termini di Windows su Linux
    typedef int SOCKET;
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
    #define closesocket close
#endif

int main() {
    // ---------------------------------------------------------
    // 0. INIZIALIZZAZIONE RETE (Solo per Windows)
    // ---------------------------------------------------------
#ifdef _WIN32
    WSADATA wsaData;
    // Richiediamo la versione 2.2 di Winsock
    int wsa_err = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (wsa_err != 0) {
        fprintf(stderr, "Errore WSAStartup fallito. Codice: %d\n", wsa_err);
        return EXIT_FAILURE;
    }
#endif

    // ---------------------------------------------------------
    // 1. CREAZIONE DEL SOCKET (Portabile)
    // ---------------------------------------------------------
    // Chiediamo un socket IPv4 (AF_INET) di tipo TCP (SOCK_STREAM)
    SOCKET sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd == INVALID_SOCKET) {
        // Su Windows perror() non legge gli errori di rete, usiamo un controllo pulito
#ifdef _WIN32
        fprintf(stderr, "Errore nella creazione del socket. Codice Winsock: %d\n", WSAGetLastError());
        WSACleanup();
#endif
#ifndef _WIN32
        perror("Errore nella creazione del socket");
#endif
        return EXIT_FAILURE;
    }

    // Il cast a (int) serve a stampare correttamente il descrittore su entrambe le piattaforme
    printf("Socket creato con successo! Il descrittore e': %d\n", (int)sockfd);

    // ---------------------------------------------------------
    // 2. CHIUSURA DEL SOCKET E PULIZIA (Portabile)
    // ---------------------------------------------------------
    // closesocket() su Windows, mappato su close() su Linux grazie alla macro in alto
    closesocket(sockfd);
    printf("Socket chiuso correttamente.\n");

#ifdef _WIN32
    // Rilascia le risorse di Winsock prima di uscire
    WSACleanup();
#endif

    return EXIT_SUCCESS;
}
