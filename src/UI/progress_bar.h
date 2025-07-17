#ifndef PROGRESS_BAR_H
#define PROGRESS_BAR_H

#include <raylib.h>

typedef struct {
    Rectangle bounds;
    Color color;
    int value;
    int max;
} ProgressBar;

void ProgressBarRender(ProgressBar bar);

#endif