#ifndef PROBES_H
#define PROBES_H
#include "probe.h"

#define MAX_PROBES 64

extern Probe *probes[MAX_PROBES];
extern int probeCount;

void AddProbe(Vector3 position);
void UpdateProbes(float dt);
void DrawProbes(void);
void DrawProbesTraces(void);
void FreeProbes(void);

#endif
