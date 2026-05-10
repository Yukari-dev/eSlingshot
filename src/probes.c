#include "probes.h"
#include "CelestialBody.h"
#include <stdlib.h>

Probe *probes[MAX_PROBES] = {0};
int probeCount = 0;

void AddProbe(Vector3 position) {
    if (probeCount >= MAX_PROBES) return;
    Vector3 drag = {0};
    probes[probeCount] = CreateProbe(position, &drag);
    probeCount++;
}

void UpdateProbes(float dt) {
    for (int i = 0; i < probeCount; i++)
        UpdateProbe(probes[i], dt);
}

void DrawProbes(void) {
    for (int i = 0; i < probeCount; i++)
        DrawProbe(probes[i]);
}

void DrawProbesTraces(void) {
    for (int i = 0; i < probeCount; i++)
        DrawProbeTraces(probes[i]);
}

void FreeProbes(void) {
    for (int i = 0; i < probeCount; i++) {
        if (probes[i]->trace) {
            free(probes[i]->trace);
            probes[i]->trace = NULL;
        }
        free(probes[i]);
        probes[i] = NULL;
    }
    probeCount = 0;
}
