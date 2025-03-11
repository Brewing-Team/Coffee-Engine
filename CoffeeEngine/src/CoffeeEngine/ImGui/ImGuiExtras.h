#pragma once

#include <imgui.h>
#include <imgui_internal.h>
#include <vector>


// Estructura de puntos del gradiente
struct GradientPoint
{
    float position; // Rango 0.0 - 1.0 (posición en la barra de gradiente)
    ImVec4 color;   // Color en RGBA
};

struct CurvePoint
{
    float time;  // Rango 0.0 - 1.0
    float value; // Escala de tamaño
};

// 🟢 Clase estática GradientEditor
class GradientEditor
{
  public:
    // Dibuja la vista previa del gradiente
    static void DrawGradientBar(const std::vector<GradientPoint>& points, ImVec2 canvas_pos, ImVec2 canvas_size);

    // Maneja los puntos de control del gradiente
    static void EditGradientPoints(std::vector<GradientPoint>& points, ImVec2 canvas_pos, ImVec2 canvas_size);

    // Muestra el editor de gradiente en ImGui
    static void ShowGradientEditor(std::vector<GradientPoint>& points);
};

class CurveEditor
{
  public:
    // Dibuja la curva en una ventana de ImGui
    static void DrawCurve(const char* label, std::vector<CurvePoint>& points, ImVec2 graph_size = ImVec2(200, 50));
    static float GetCurveValue(float time, const std::vector<CurvePoint>& points);
};