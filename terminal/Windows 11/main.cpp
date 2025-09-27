#include <iostream>

class Console_Instance {
private:
	bool admin, disable_info;
public:
	bool getInfo(int i) {
		switch (i) {
		case 1: {
			return admin;
			break;
		}
		case 2: {
			return disable_info;
			break;
		}
		}
	}
	void setFlags(bool run_as_admin, bool disable_info__f) {
		this->admin = run_as_admin;
		this->disable_info = disable_info__f;
		return;
	}
};

Console_Instance check_terminal_argv(char* argv, Console_Instance console) {
	if (argv == "") {
		return 0;
	}
	else if (argv == "-disable_info") {
		console.setFlags(console.getInfo(1), true);
		return console;
	}
	else if (argv == "-run_as_admin") {
		console.setFlags(true, console.getInfo(2));
		return console;
	}
	return -1;
}

int terminal(char* argv[3], Console_Instance *console) {
	for (int i = 1;i < 3;i++) {
		check_terminal_argv(argv[i], &console);
	}
	std::cout << "Type in a command:\n$/"
	std::cin >> input;
}

int main(int argc, char* argv[]) {
	if(terminal(argv[1], ) != 0) {
		return -1;
	}
	return 0;
}