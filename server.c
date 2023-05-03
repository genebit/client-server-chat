#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

void die_with_error(char *error_msg)
{
    printf("%s", error_msg);
    exit(-1);
}

void send_to_client(char buffer[], int client_sock)
{
    printf("< ");

    // Set buffer to null
    bzero(buffer, 256);

    // Read the input of user and assign it to buffer
    fgets(buffer, 255, stdin);

    // Send a message
    int res = send(client_sock, buffer, strlen(buffer), 0);

    // If (sending) failed
    if (res < 0)
        die_with_error("Error: send() Failed.");
}

void recv_from_client(char buffer[], int client_sock)
{
    // Set buffer to null
    bzero(buffer, 256);

    // Recieve from the server
    int res = recv(client_sock, buffer, 255, 0);

    // If (recieving) failed
    if (res < 0)
        die_with_error("Error: recv() Failed.");

    // Print the server message
    printf("[client] > %s", buffer);
}

int main(int argc, char *argv[])
{
    int server_sock, client_sock, port_no, client_size;
    char buffer[256];
    struct sockaddr_in server_addr, client_addr;

    // Checks if the argument is supplied upon running the program
    if (argc < 2)
    {
        printf("Usage: %s <PORT_NUMBER>\n", argv[0]);
        exit(1);
    }

    printf("Server is starting...\n");

    // Create a socket for incoming connections
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0)
        die_with_error("Error: socket() Failed.");

    // Bind socket to a port
    bzero((char *)&server_addr, sizeof(server_addr));
    port_no = atoi(argv[1]);
    server_addr.sin_family = AF_INET;         // Internet address
    server_addr.sin_addr.s_addr = INADDR_ANY; // Any incoming interface
    server_addr.sin_port = htons(port_no);    // Local port

    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        die_with_error("Error: bind() Failed.");

    // Mark the socket so it will listen for incoming connections
    listen(server_sock, 5);

    // Accept new connection
    client_size = sizeof(client_addr);
    client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_size);
    if (client_sock < 0)
        die_with_error("Error: accept() Failed.");

    // Start of communication
    printf("Client successfully connected.\n");
    printf("------------------------------\n");

    while (1)
    {
        send_to_client(buffer, client_sock);
        recv_from_client(buffer, client_sock);
    }

    return 0;
}