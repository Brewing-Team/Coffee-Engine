#include "ImGuiExtras.h"
#include <algorithm> 
#include <unordered_map>
#include <string>


// 🟢 Dibuja la vista previa del gradiente
void GradientEditor::DrawGradientBar(const std::vector<GradientPoint>& points, ImVec2 canvas_pos, ImVec2 canvas_size)
{
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    for (size_t i = 0; i < points.size() - 1; ++i)
    {
        const auto& p1 = points[i];
        const auto& p2 = points[i + 1];

        ImVec2 pos1 = ImVec2(canvas_pos.x + p1.position * canvas_size.x, canvas_pos.y);
        ImVec2 pos2 = ImVec2(canvas_pos.x + p2.position * canvas_size.x, canvas_pos.y + canvas_size.y);

        draw_list->AddRectFilledMultiColor(
            pos1, pos2, ImGui::ColorConvertFloat4ToU32(p1.color), ImGui::ColorConvertFloat4ToU32(p2.color),
            ImGui::ColorConvertFloat4ToU32(p2.color), ImGui::ColorConvertFloat4ToU32(p1.color));
    }
}

// 🟢 Maneja los puntos de control del gradiente
void GradientEditor::EditGradientPoints(std::vector<GradientPoint>& points, ImVec2 canvas_pos, ImVec2 canvas_size)
{
    int pointToDelete = -1;
    bool addNewPoint = false;
    float newPointPos = 0.0f;

    for (size_t i = 0; i < points.size(); ++i)
    {
        auto& p = points[i];
        float x_pos = canvas_pos.x + p.position * canvas_size.x;
        ImVec2 handle_pos = ImVec2(x_pos, canvas_pos.y + canvas_size.y);

        ImGui::SetCursorScreenPos(ImVec2(handle_pos.x - 5, handle_pos.y - 5));
        ImGui::PushID(i);

        if (ImGui::ColorEdit4("", (float*)&p.color, ImGuiColorEditFlags_NoInputs))
        {
            // Color actualizado
        }

        ImGui::PopID();

        // 🟢 Arrastrar puntos del gradiente
        ImGui::SetCursorScreenPos(handle_pos);
        ImGui::InvisibleButton("DragHandle", ImVec2(10, 10),
                               ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);

        // Si el usuario está moviendo el punto
        if (ImGui::IsItemActive() && ImGui::IsMouseDragging(0))
        {
            float new_pos = (ImGui::GetMousePos().x - canvas_pos.x) / canvas_size.x;
            p.position = ImClamp(new_pos, 0.0f, 1.0f);
        }

        // Si el usuario hace clic derecho sobre un punto, marcarlo para eliminar
        if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
        {
            pointToDelete = static_cast<int>(i);
        }
    }

    // 🟢 Añadir un nuevo punto al hacer clic en la barra del gradiente
    if (ImGui::InvisibleButton("AddGradientPoint", canvas_size, ImGuiButtonFlags_MouseButtonLeft) &&
        ImGui::IsMouseClicked(0))
    {
        newPointPos = (ImGui::GetMousePos().x - canvas_pos.x) / canvas_size.x;
        addNewPoint = true;
    }

    // Agregar el nuevo punto si se detectó un clic en la barra
    if (addNewPoint)
    {
        points.push_back({ImClamp(newPointPos, 0.0f, 1.0f), ImVec4(1, 1, 1, 1)});

        // Ordenar los puntos por posición para mantener el gradiente limpio
        std::sort(points.begin(), points.end(),
                  [](const GradientPoint& a, const GradientPoint& b) { return a.position < b.position; });
    }

    // Eliminar el punto si se marcó para borrado
    if (pointToDelete >= 0 && points.size() > 2)
    { // Siempre mantener al menos 2 puntos
        points.erase(points.begin() + pointToDelete);
    }
}

// 🟢 Muestra el editor de gradiente en ImGui
void GradientEditor::ShowGradientEditor(std::vector<GradientPoint>& points)
{
    if (ImGui::TreeNodeEx("Gradient Editor", ImGuiTreeNodeFlags_None))
    {
        // 🟢 Dibujar gradiente
        ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
        ImVec2 canvas_size = ImVec2(ImGui::GetContentRegionAvail().x-10, 30);
        DrawGradientBar(points, canvas_pos, canvas_size);
        ImGui::InvisibleButton("GradientCanvas", canvas_size);

        // 🟢 Editar puntos del gradiente
        EditGradientPoints(points, canvas_pos, canvas_size);

        ImGui::TreePop();
    }
}




void CurveEditor::DrawCurve(const char* label, std::vector<CurvePoint>& points, ImVec2 graph_size)
{
    ImVec2 graph_pos = ImGui::GetCursorScreenPos();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImU32 lineColor = IM_COL32(255, 165, 0, 255);     // Naranja
    ImU32 pointColor = IM_COL32(0, 200, 255, 255);    // Azul brillante
    ImU32 borderColor = IM_COL32(255, 255, 255, 255); // Blanco para el borde
    ImU32 gridColor = IM_COL32(100, 100, 100, 100);   // Gris claro para las líneas de la cuadrícula
    float radius = 5.0f;                              // Tamaño de los puntos

    // 🟢 Mapa de selección único por curva
    static std::unordered_map<std::string, int> selectedPoints;
    int& selectedPoint = selectedPoints[label]; // Cada curva tiene su propia selección

    // 🟢 Ordenar los puntos por tiempo antes de dibujar
    std::sort(points.begin(), points.end(), [](const CurvePoint& a, const CurvePoint& b) { return a.time < b.time; });

    // Convertir los puntos al espacio de la gráfica
    auto ToGraphSpace = [&](const CurvePoint& p) -> ImVec2 {
        return ImVec2(graph_pos.x + p.time * graph_size.x, graph_pos.y + (1.0f - p.value) * graph_size.y);
    };

    // 🟢 Dibujar el borde del gráfico
    draw_list->AddRect(graph_pos, ImVec2(graph_pos.x + graph_size.x, graph_pos.y + graph_size.y), borderColor, 0.0f,
                       ImDrawFlags_None, 2.0f);

    // 🟢 Dibujar las líneas de referencia horizontales (0.0, 0.5, 1.0)
    for (float i = 0.0f; i <= 1.0f; i += 0.5f)
    {
        float y_pos = graph_pos.y + (1.0f - i) * graph_size.y;
        draw_list->AddLine(ImVec2(graph_pos.x, y_pos), ImVec2(graph_pos.x + graph_size.x, y_pos), gridColor);
    }

    // 🟢 Dibujar las líneas de referencia verticales (0.0, 0.5, 1.0)
    for (float i = 0.0f; i <= 1.0f; i += 0.5f)
    {
        float x_pos = graph_pos.x + i * graph_size.x;
        draw_list->AddLine(ImVec2(x_pos, graph_pos.y), ImVec2(x_pos, graph_pos.y + graph_size.y), gridColor);
    }

    // 🟢 Dibujar la línea de la curva
    for (size_t i = 1; i < points.size(); i++)
    {
        ImVec2 p1 = ToGraphSpace(points[i - 1]);
        ImVec2 p2 = ToGraphSpace(points[i]);
        draw_list->AddLine(p1, p2, lineColor, 2.0f);
    }

    // 🟢 Manejo de interacción: detección de clics y arrastre
    ImVec2 mouse_pos = ImGui::GetIO().MousePos;
    bool mouse_clicked = ImGui::IsMouseClicked(0);
    bool mouse_held = ImGui::IsMouseDown(0);
    bool mouse_remove = ImGui::IsMouseClicked(1);

    for (size_t i = 0; i < points.size(); i++)
    {
        ImVec2 point_screen = ToGraphSpace(points[i]);

        // Verificar si el usuario hace clic en un punto
        if (mouse_clicked &&
            ImLengthSqr(ImVec2(mouse_pos.x - point_screen.x, mouse_pos.y - point_screen.y)) < (radius * radius * 4))
        {
            selectedPoint = (int)i;
        }

        // Si el usuario hace clic derecho, eliminar el punto
        if (mouse_remove &&
            ImLengthSqr(ImVec2(mouse_pos.x - point_screen.x, mouse_pos.y - point_screen.y)) < (radius * radius * 4))
        {
            points.erase(points.begin() + i);
            --i;
        }

        // Si un punto está seleccionado y el mouse está presionado, mover el punto
        if (selectedPoint == (int)i && mouse_held)
        {
            float newTime = (mouse_pos.x - graph_pos.x) / graph_size.x;
            float newValue = 1.0f - (mouse_pos.y - graph_pos.y) / graph_size.y;
            points[i].time = std::clamp(newTime, 0.0f, 1.0f);
            points[i].value = std::clamp(newValue, 0.0f, 1.5f); // Limite superior configurable
        }

        // Dibujar los puntos de control
        draw_list->AddCircleFilled(point_screen, radius, pointColor);
    }

    // Liberar selección cuando se suelta el mouse
    if (!mouse_held)
    {
        selectedPoint = -1;
    }

    // 🟢 Espacio en la UI para el gráfico
    ImGui::Dummy(graph_size);

    // 🟢 Botón para agregar nuevos puntos
    if (ImGui::Button(std::string("Add Point##").append(label).c_str())) // Etiqueta única para cada botón
    {
        points.push_back({0.5f, 1.0f}); // Nuevo punto en el medio
    }
}



