#ifndef WIDGET_H
#define WIDGET_H
#include "panel.h"
#include <stdbool.h>

void WidgetLabel(Panel *p, const char *text);
void WidgetValue(Panel *p, const char *label, double value);
void WidgetDivider(Panel *p);
bool WidgetButton(Panel *p, const char *label);
void WidgetProgressBar(Panel *p, const char *label, float value);
void WidgetMass(Panel *p, float mass, float maxMass);
void WidgetRadius(Panel *p, float radius, float maxRadius);

#endif
