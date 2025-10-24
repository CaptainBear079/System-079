#include <stdio.h>
#include <stdbool.h>

typedef struct _USER_ {
    bool B_MSG;
    char* msg;
} USER;

typedef struct _BOT_ {
    char* user_msg;
    char* msg;
    char* last_token;
} BOT;

int Bot();

// Get message from user
void get_msg(char* buffer, int pos);
// Send message (for chatbot)
int send_msg(const char* msg, bool v, void* value);
// Format message (for chatbot)
void format_msg();
// Send user message to chatbot
void bot_msg(BOT* bot, char* user_msg, int buffer_length);
