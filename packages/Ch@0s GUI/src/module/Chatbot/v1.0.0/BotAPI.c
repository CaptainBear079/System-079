#include "BotAPI.h"

bool exit_flag = false;
bool termination = false;
int bot_ret;

int MakeTrainingsData() {
	VARIANTS Variants[1024];
	int CurrentVariant = 0;
	char buffer[512];
	while(!exit_flag) {
		fgets(buffer, sizeof(buffer), stdin);
		// Check if exit
		if(strncmp(buffer, "$(//EXIT)", 9) == 0) {
			exit_flag = true;
			break;
		}
		for(int i = 0; i < 511; i++) {
			// Parse
		}
	}
}

bool Read_CONFIG(int* NeuronCount, LIFNeuron* Neurons, double** Weights, char* Bot) {
	FILE* fptr = fopen("./CONFIG", "r");
	if(fptr == NULL) {
		printf("[CONFIG][ERROR] File doesn't exists or couldn't get read by fopen.");
		return false;
	}
	int c = fgetc(fptr);
	if(c == EOF) {
		printf("[CONFIG][ERROR] CONFIG file is empty.");
		return false;
	}
	// Jump over white space characters
	while((char)c == ' ' || (char)c == '\n' || (char)c == '\t') {
		c = fgetc(fptr);
		if(c == EOF) {
			printf("[CONFIG][ERROR] End of file reached before completting config.");
			return false;
		}
	}
	if((char)c == '{') {
		// Find entry
		while((char)c != '\"') {
			c = fgetc(fptr);
			if(c == EOF) {
				printf("[CONFIG][ERROR] End of file reached before completting config.");
				return false;
			}
		}
		char c_A[256] = { 0 };
		int i = 0;
		c = fgetc(fptr);
		if(c == EOF) {
			printf("[CONFIG][ERROR] End of file reached before completting config.");
			return false;
		}
		while((char)c != '\"') {
			c_A[i] = (char)c;
			c = fgetc(fptr);
			if(c == EOF) {
				printf("[CONFIG][ERROR] End of file reached before completting config.");
				return false;
			}
		}
		while((char)c != ':') {
			c = fgetc(fptr);
			if(c == EOF) {
				printf("[CONFIG][ERROR] End of file reached before completting config.");
				return false;
			}
		}
		while((char)c != '{') {
			c = fgetc(fptr);
			if(c == EOF) {
				printf("[CONFIG][ERROR] End of file reached before completting config.");
				return false;
			}
		}
	}
	return false;
}

void BOT__THREAD(void* arg) {
	double Time = 0.0;
	double DeltaTime = 0.0;
	int NeuronCount = MAX_NEURONS;
	LIFNeuron* Neurons = calloc((size_t)MAX_NEURONS, sizeof(LIFNeuron));
	double** Weights = calloc((size_t)MAX_NEURONS, sizeof(double*));
	for(int i = 0; i < MAX_NEURONS; i++) {
		Weights[i] = calloc((size_t)MAX_NEURONS, sizeof(double));
	}
	int n = 0;
	LIFNeuron* temp = NULL;
	double input = 0.0;
	{
		char** _temp = arg;
		if(!Read_CONFIG(&NeuronCount, Neurons, Weights, _temp[0])) {
			printf("[ERROR] Config not found, incomplete or couldn't get read.\n\tCONFIG: %s\n", _temp[0]);
			termination = true;
			return;
		}
	}
	while(!exit_flag) {
		for(int _n = 0; _n < NeuronCount; _n++) {
			temp = Neurons[n].Inputs;
			for(int i = 0; i < NeuronCount; i++) {
				if(temp == NULL) {
					break;
				}
				if(temp->Spiked) {
					input += Weights[i][temp->WeightID];
				}
				temp = temp->Inputs;
			}
			update_LIF_neuron(&Neurons[n], input, DeltaTime);
			n++;
			DeltaTime += 1.0;
			Time += 1.0;
		}
		printf("[LOG] Time: %f, DeltaTime: %f\n", Time, DeltaTime);
		for(int i = 0; i < NeuronCount; i++) {
			printf("[LOg] Neuron %d, Spiked: %d\n", i, Neurons[i].Spiked);
		}
		system("clear");
	}

	// Free memory
	for(int i = 0; i < MAX_NEURONS; i++) {
		free(Weights[i]);
	}
	free(Weights);
	free(Neurons);
	return;
}

int Bot() {
	char user_msg[256];
	char bot__user_msg[256];
	char msg[256];
	char last_token[256];
	USER user = { .B_MSG = false, .msg = user_msg };
	BOT bot = { .user_msg = bot__user_msg, .msg = msg, .last_token = last_token };
	void* bot_ret_ptr = &bot_ret;
	char arg_0[256] = "TheBear";
	__ptr_t* arg = calloc((size_t)1, sizeof(char*));
	arg[0] = &arg_0;
	pthread_t bot_thread;
	pthread_create(&bot_thread, NULL, (void*)BOT__THREAD, arg);
	pthread_detach(bot_thread);
	while(!exit_flag) {
		if(termination) {
			printf("Program encountered an error.\n Program terminated.\n");
			return -1;
		}
		if(user.B_MSG) {
			send_msg(bot.msg, false, NULL);
			bot.msg[0] = '-';
			send_msg(bot.msg, false, NULL);
			send_msg("[%s]", true, bot.last_token);
			bot_msg(&bot, user_msg, 256);
		}
	}
	pthread_join(bot_thread, &bot_ret_ptr);
	return 0;
}
