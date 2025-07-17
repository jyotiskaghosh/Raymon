#include "progress_bar.h"

void ProgressBarRender(ProgressBar bar)
{
    DrawRectangle(bar.bounds.x, bar.bounds.y, bar.bounds.width * (bar.value / bar.max), bar.bounds.height, bar.color);
    DrawRectangleLinesEx(bar.bounds, 1, BLACK);
}