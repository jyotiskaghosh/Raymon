#include "panel.h"

void PanelRender(Rectangle rect)
{
    DrawRectangleRec(rect, (Color){56, 56, 56, 255});
    DrawRectangleLinesEx(rect, 2, WHITE);
}