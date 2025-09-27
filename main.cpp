// Standard C++ libaries
#include <iostream>
#include <string>
#include <random>
#include <thread>

// Temp core.chaosstudios.net
#include "core.chaosstudios.net.h"

// Custom Headers
#include "BanSupervisor.hpp"

// Definitions
#define SelfHost 0

// structures
typedef struct _User_ {
    std::string Adress, IPvCMN;
    int ProcessNumber, ProcessThread;
} User;
typedef struct _Entity_ {
    int Type;
    std::string Adress, IPvCMN;
    int ProcessNumber, ProcessThread;
} Entity;
typedef struct _RET_ {
    int ret_value = 0;
    std::string ret_msg;
} RET;

// Enumberations
// Console Program Start Input
enum CPSI {
	ENCRYPTION = "-E",
	EXTERN_ENCRYPTION = "Extern",
	INTERN_ENCRYPTION = "Intern"
}

// Classes
class Neuron {
private:
    // LIF
	double membranePotential;     // Membranpotenzial [mV]
    double restingPotential;      // Ruhepotenzial [mV]
    double threshold;             // Schwellenwert zum Feuern [mV]
    double resistance;            // Membranwiderstand [MΩ]
    double capacitance;           // Membrankapazität [nF]
    double leakRate;              // Leckstrom (passives Abfließen) [1/ms]
    double resetPotential;        // Potenzial nach Spike
    double refractoryPeriod;      // Refraktärzeit [ms]
    double lastSpikeTime;         // Zeitpunkt des letzten Spikes [ms]
	double lastSpikeSize;         // Letzter Spike Wert
    double currentTime;
public:
    Neuron(unsigned int random_number)
        : membranePotential(-65.0),
          restingPotential(-65.0),
          threshold(-50.0),
          resistance(10.0),
          capacitance(1.0),
          leakRate(0.1),
          resetPotential(-70.0),
          refractoryPeriod(5.0),
          lastSpikeTime(-100.0),
          currentTime(0.0) {
		srand(((unsigned int)time(NULL) + random_number) / 2);
        this->weight = rand() / 2;
        srand((unsigned int)time(NULL) + (random_number / 2));
        this->bias = rand() / 2;
	}
	
    // Classic
    double weight, bias;
    double Calculate(double input) {
        return ((input * this->weight) + this->bias);
    }
	
	// LIF

    bool update(double inputCurrent, double dt) {
        currentTime += dt;

        // Check for refractory period
        if (currentTime - lastSpikeTime < refractoryPeriod) {
            return false;
        }

        // dV/dt = -(V - V_rest) / (RC) + I / C
        double dV = (-(membranePotential - restingPotential) / (resistance * capacitance)
                     + inputCurrent / capacitance) * dt;
        membranePotential += dV;

        // Spike check
        if (membranePotential >= threshold) {
            membranePotential = resetPotential;
            lastSpikeTime = currentTime;
            return true;
        }
        return false;
	}
    
	double getPotential() const {
        return membranePotential;
    }

    double getTime() const {
        return currentTime;
    }
};

// Global Variables
User CaptainBearUser = {
  "cmn://core.chaosstudios.net/Users/CaptainBear/ConnectMask.boot.apk",
  "000.000.000.000",
  NULL,
  001
};
Entity TheBearEntity = {
  SelfHost,
  "cmn://core.chaosstudios.net/SYSTEM/TheBear/TheBear.boot.apk",
  "000.000.000.000",
  NULL,
  002
};
Entity GlitchBunnyEntity = {
  SelfHost,
  "cmn://core.chaosstudios.net/SYSTEM/GlitchBunny/GlitchBunny.boot.apk",
  "000.000.000.000",
  NULL,
  003
};
RET ret = { 0, "" };
RET ret2 = { 0, "" };

// Forward Declerations
RET RenderEntity(Entity entity, RET* ret, int argc, char* cpsi[]);
int ERROR(int err_code);

int main(int argc, char* argv[]) {
    std::thread TheBear079(RenderEntity, TheBearEntity, &ret2, argc, argv);
    TheBear079.join(); // Wait for TheBear079
	system("PAUSE");
    return ret.ret_value; // Exit program with return value(ret)
}

RET RenderEntity(Entity entity, RET* ret, int argc, char* cpsi[]) {
	for(int i = 0; i < argc; i++) {
	    if(cpsi[i] == "-TB") {
			for(int c = i + 1; c < argc; c++) {
				i++;
	            switch(cpsi[i]) {
		            case ENCRYPTION: {
					    i++;
			            switch(cpsi[i]) {
				        case EXTERN_ENCRYPTION: {
					            TheBearEntity.flags.ioEEncryption = true;
				            } break;
				            case INTERN_ENCRYPTION: {
					            TheBearEntity.flags.ioEEncryption = false;
							    i++;
							    TheBearEntity.flags.EncryptionKey = cpsi[i];
				            } break;
				            default: {
					            ERROR();
					            return ;
				            } break;
			            }
					    i++;
					    switch(cpsi[i]) {
						    case CONSOLE_OUTPUT_ENCRYPTION: {
							    TheBearEntity.flags.ioCOEncryption = true;
						    } break;
						    default: {
							    ERROR();
							    return ;
						    }
					    }
		            } break;
				    default: {
					    i = argc;
				    } break;
	            }
			}
		}
	}
    Neuron* neuron = new Neuron(rand());
    double o = neuron->Calculate(2.4536);
    std::cout << o << std::endl;
	system("PAUSE");
	neuron->update(0.002, 0.0);
	o = neuron->getPotential();
	std::cout << o << std::endl;
    *ret = { 0, " got shutdown with exit code: " };
    return *ret;
}

int ERROR(int err_code) {
    // Error handling
    std::string error = "";
    std::string error_code_hex = "";
    if(err_code == 1) {
        error = "Wrong system information.";
        error_code_hex = "0x0001";
    }
    else if(err_code == 2) {
        error = "Invalid input.";
        error_code_hex = "0x0002";
    }
    __CLEAR
    std::cerr << ":<\nAn ERROR has occurred\nThis program will be closed in 90 seconds\n\n"
              << "ERROR: " << error << "\n" << "ERROR CODE: " << error_code_hex << "\n";
    std::this_thread::sleep_for(std::chrono::seconds(90));
    return err_code;
}