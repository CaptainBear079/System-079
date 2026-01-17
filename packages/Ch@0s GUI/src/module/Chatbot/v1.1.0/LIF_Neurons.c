#include "LIF_Neurons.h"

bool update_LIF_neuron(LIFNeuron *n, double I, double dt) {
    // If neuron is in refractory period, decrement timer and skip update
    if (n->refractory_timer > 0.0) {
        n->refractory_timer -= dt;
        return false;
    }

    // Update membrane potential using Euler method
    double dV = (-(n->V - n->V_rest) + n->Rm * I) * (dt / n->tau_m);
    n->V += dV;

    // Check for spike
    if (n->V >= n->V_th) {
        // Fire spike and reset
        n->V = n->V_reset;
        n->refractory_timer = n->refractory_time;
        n->Spiked = true;
        return true;  // neuron fired
    }

    return false;  // no spike
}
