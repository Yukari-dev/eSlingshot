#ifndef PROBE_H
#define PROBE_H
#define TRACE_LENGTH 1500
#define PREDICT_STEPS 500
#include <raylib.h>
#include <stdbool.h>
#include "region.h"

typedef struct {
    char    *name;
    Vector3  position;
    Vector3  velocity;
    bool     active;
    bool     launched;
    Vector3 *trace;
    int      traceIndex;
    int      traceCount;
} Probe;

Probe  *CreateProbe(Vector3 position, Vector3 *dragStart);
void    DrawProbe(Probe *p);
void    DrawProbeTraces(Probe *p);
void    UpdateProbe(Probe *p, float dt);
void    PredictTrajectory(Probe *p, Vector3 initVel, Vector3 *out, int *outCount);
void    DrawPrediction(Vector3 *points, int count);
void    DrawPredictionLine(Camera3D *camera, Region *simRegion);
bool    IsMouseOverProbe(Camera *camera, Region *r, Probe *p);

#endif
