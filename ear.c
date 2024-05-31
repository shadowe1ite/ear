#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

#define DEFAULT_ADDRESS "2C:BE:EB:DE:F0:55"
#define DEFAULT_CHANNEL 15

void send_commands(const char *address, int channel, const char *commands[], int num_commands) {
    struct sockaddr_rc addr = { 0 };
    int sock, status;

    // Allocate a socket
    sock = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    if (sock < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set the connection parameters (RFCOMM channel)
    addr.rc_family = AF_BLUETOOTH;
    str2ba(address, &addr.rc_bdaddr);
    addr.rc_channel = (uint8_t) channel;

    // Connect to the server
    status = connect(sock, (struct sockaddr *)&addr, sizeof(addr));
    if (status < 0) {
        perror("connect");
        close(sock);
        exit(EXIT_FAILURE);
    }

    // Send commands
    for (int i = 0; i < num_commands; ++i) {
        printf("Sending command: %s\n", commands[i]);
        
        // Create a modifiable copy of the command
        char command_copy[64];
        strncpy(command_copy, commands[i], sizeof(command_copy) - 1);
        command_copy[sizeof(command_copy) - 1] = '\0';

        // Convert hex string to bytes
        unsigned char data_bytes[64];
        int len = 0;
        char *token = strtok(command_copy, ":");
        while (token != NULL) {
            data_bytes[len++] = (unsigned char)strtol(token, NULL, 16);
            token = strtok(NULL, ":");
        }

        // Send the bytes
        status = write(sock, data_bytes, len);
        if (status < 0) {
            perror("write");
            close(sock);
            exit(EXIT_FAILURE);
        }

        sleep(2); // Adjust sleep time as needed
    }

    // Close the connection
    close(sock);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <command> [--address <address>] [--channel <channel>] [value]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *command = argv[1];
    const char *address = DEFAULT_ADDRESS;
    int channel = DEFAULT_CHANNEL;
    const char *value = NULL;

    // Parse optional arguments
    for (int i = 2; i < argc; ++i) {
        if (strcmp(argv[i], "--address") == 0) {
            address = argv[++i];
        } else if (strcmp(argv[i], "--channel") == 0) {
            channel = atoi(argv[++i]);
        } else {
            value = argv[i];
        }
    }

    const char *commands_trans[] = {"55:60:01:0f:f0:03:00:0e:01:07:00:f9:63"};
    const char *commands_anc[] = {"55:60:01:0f:f0:03:00:10:01:01:00:fc:eb"};
    const char *commands_off[] = {"55:60:01:0f:f0:03:00:0f:01:05:00:f9:ff"};

    const char *commands_bass_0[] = {"55:60:01:51:f0:02:00:b6:00:0a:46:8c"};
    const char *commands_bass_1[] = {"55:60:01:51:f0:02:00:b1:01:02:f7:1b"};
    const char *commands_bass_2[] = {"55:60:01:51:f0:02:00:b2:01:04:87:19"};
    const char *commands_bass_3[] = {"55:60:01:51:f0:02:00:b3:01:06:57:18"};
    const char *commands_bass_4[] = {"55:60:01:51:f0:02:00:b4:01:08:67:1d"};
    const char *commands_bass_5[] = {"55:60:01:51:f0:02:00:b5:01:0a:b7:1c"};

    const char *commands_in_ear_on[] = {"55:60:01:04:f0:03:00:cd:01:01:01:46:f4"};
    const char *commands_in_ear_off[] = {"55:60:01:04:f0:03:00:ce:01:01:00:87:70"};

    const char *commands_low_lag_on[] = {"55:60:01:40:f0:02:00:cf:01:00:17:c3"};
    const char *commands_low_lag_off[] = {"55:60:01:40:f0:02:00:d0:02:00:26:f5"};

    const char *commands_find_device_right_on[] = {"55:60:01:02:f0:02:00:d6:02:01:65:30"};
    const char *commands_find_device_right_off[] = {"55:60:01:02:f0:02:00:d7:02:00:f5:30"};

    const char *commands_find_device_left_on[] = {"55:60:01:02:f0:02:00:d8:03:01:05:63"};
    const char *commands_find_device_left_off[] = {"55:60:01:02:f0:02:00:d9:03:00:95:63"};

    if (strcmp(command, "trans") == 0) {
        send_commands(address, channel, commands_trans, 1);
    } else if (strcmp(command, "anc") == 0) {
        send_commands(address, channel, commands_anc, 1);
    } else if (strcmp(command, "off") == 0) {
        send_commands(address, channel, commands_off, 1);
    } else if (strcmp(command, "bass") == 0) {
        if (!value) {
            fprintf(stderr, "Value for bass command is required.\n");
            exit(EXIT_FAILURE);
        }
        if (strcmp(value, "0") == 0) {
            send_commands(address, channel, commands_bass_0, 1);
        } else if (strcmp(value, "1") == 0) {
            send_commands(address, channel, commands_bass_1, 1);
        } else if (strcmp(value, "2") == 0) {
            send_commands(address, channel, commands_bass_2, 1);
        } else if (strcmp(value, "3") == 0) {
            send_commands(address, channel, commands_bass_3, 1);
        } else if (strcmp(value, "4") == 0) {
            send_commands(address, channel, commands_bass_4, 1);
        } else if (strcmp(value, "5") == 0) {
            send_commands(address, channel, commands_bass_5, 1);
        } else {
            fprintf(stderr, "Invalid value for bass command.\n");
            exit(EXIT_FAILURE);
        }
    } else if (strcmp(command, "in_ear") == 0) {
        if (strcmp(value, "on") == 0) {
            send_commands(address, channel, commands_in_ear_on, 1);
        } else if (strcmp(value, "off") == 0) {
            send_commands(address, channel, commands_in_ear_off, 1);
        } else {
            fprintf(stderr, "Invalid value for in_ear command.\n");
            exit(EXIT_FAILURE);
        }
    } else if (strcmp(command, "low_lag") == 0) {
        if (strcmp(value, "on") == 0) {
            send_commands(address, channel, commands_low_lag_on, 1);
        } else if (strcmp(value, "off") == 0) {
            send_commands(address, channel, commands_low_lag_off, 1);
        } else {
            fprintf(stderr, "Invalid value for low_lag command.\n");
            exit(EXIT_FAILURE);
        }
    } else if (strcmp(command, "find_device_right") == 0) {
        if (strcmp(value, "on") == 0) {
            send_commands(address, channel, commands_find_device_right_on, 1);
        } else if (strcmp(value, "off") == 0) {
            send_commands(address, channel, commands_find_device_right_off, 1);
        } else {
            fprintf(stderr, "Invalid value for find_device_right command.\n");
            exit(EXIT_FAILURE);
        }
    } else if (strcmp(command, "find_device_left") == 0) {
        if (strcmp(value, "on") == 0) {
            send_commands(address, channel, commands_find_device_left_on, 1);
        } else if (strcmp(value, "off") == 0) {
            send_commands(address, channel, commands_find_device_left_off, 1);
        } else {
            fprintf(stderr, "Invalid value for find_device_left command.\n");
            exit(EXIT_FAILURE);
        }
    } else {
        fprintf(stderr, "Invalid command.\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}
