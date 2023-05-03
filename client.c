#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

void die_with_error(char *error_msg)
{
    printf("%s", error_msg);
    exit(-1);
}

void send_to_server(char buffer[], int client_sock)
{
    // Ask for input
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

void recv_from_server(char buffer[], int client_sock)
{
    // Set buffer to null
    bzero(buffer, 256);

    // Recieve from the server
    int res = recv(client_sock, buffer, 255, 0);

    // If (recieving) failed
    if (res < 0)
        die_with_error("Error: send() Failed.");

    // Print the server message
    printf("[server] > %s", buffer);
}

int main(int argc, char *argv[])
{
    int client_sock, port_no;
    struct sockaddr_in server_addr;
    struct hostent *server;

    char buffer[256];

    // Checks if the argument is supplied upon running the program
    if (argc < 3)
    {
        printf("Usage: %s <HOSTNAME> <PORT_NUMBER>\n", argv[0]);
        exit(1);
    }

    printf("Connecting...\n");

    // Create a socket using TCP
    client_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (client_sock < 0)
        die_with_error("Error: socket() Failed.");

    // Looking for host...
    server = gethostbyname(argv[1]);
    if (server == NULL)
    {
        die_with_error("Error: No such host.");
    }

    // Establish a connection to server
    port_no = atoi(argv[2]);
    bzero((char *)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&server_addr.sin_addr.s_addr,
          server->h_length);

    server_addr.sin_port = htons(port_no);

    // If the connection failed
    if (connect(client_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        die_with_error("Error: connect() Failed.");

    // Start of communication
    printf("Connection has been successful.\n");
    printf("-------------------------------\n");

    while (1)
    {
        recv_from_server(buffer, client_sock);
        send_to_server(buffer, client_sock);
    }

    return 0;
}