#include <stdbool.h>

typedef struct _LIFNeuron_ {
    double V;         // Current membrane potential
    double V_rest;    // Resting potential
    double V_th;      // Threshold potential
    double V_reset;   // Reset potential after spike
    double Rm;        // Membrane resistance
    double tau_m;     // Membrane time constant
    double refractory_time;  // Refractory period duration (ms)
    double refractory_timer; // Remaining refractory time (ms)

    bool Spiked;    // Spike flag

    void* Inputs; // Pointer to input neurons
    int WeightID;      // ID for weight matrix
} LIFNeuron;

// Update LIF neuron state
bool update_LIF_neuron(LIFNeuron *n, double I, double dt);
