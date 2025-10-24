#include "MSG.h"

// Get message from user
void get_msg(char* buffer, int pos) {
	buffer[pos] = (char)getchar();
	return;
}

// Send message (for chatbot)
int send_msg(const char* msg, bool v, void* value) {
	if(v) {
		return printf(msg, value);
	}
	else {
		return printf(msg);
	}
}

// Format message (for chatbot)
void format_msg() {}

// Send user message to chatbot
void bot_msg(BOT* bot, char* user_msg, int buffer_length) {
    for(int i = 0; i < buffer_length; i++) {
        bot->user_msg[i] = user_msg[i];
    }
    return;
}
