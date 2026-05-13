#ifndef WIDGET_H
#define WIDGET_H
#include "panel.h"
#include "CelestialBody.h"
#include <stdbool.h>

void WidgetLabel(Panel *p, const char *text);
void WidgetValue(Panel *p, const char *label, double value);
void WidgetDivider(Panel *p);
bool WidgetButton(Panel *p, const char *label);
void WidgetProgressBar(Panel *p, const char *label, float value);
void WidgetMass(Panel *p, float mass, float maxMass);
void WidgetRadiusDistance(Panel *p, CelestialBody *body, CelestialBody *host);
void WidgetVelocity(Panel *p, Vector3 velocity);
void WidgetOrbitalClock(Panel *p, CelestialBody *body);
void WidgetSpeedGraph(Rectangle dst);

#endif
