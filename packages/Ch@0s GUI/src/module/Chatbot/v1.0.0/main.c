#include "Chatbot.h"

int main(int argc, char* argv[]) {
	if(argc > 1) {
		if(strcmp(argv[1], "MT") == 0) {
			MakeTrainingsData();
		}
		else if(strcmp(argv[1], "T") == 0) {
			// Train();
		}
	}
	else {
		Bot();
	}
	return 0;
}
