#include "panel.h"

void PanelRender(Rectangle bounds)
{
    DrawRectangleRec(bounds, (Color){56, 56, 56, 255});
    DrawRectangleLinesEx(bounds, 2, WHITE);
}