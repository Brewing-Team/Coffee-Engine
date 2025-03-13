﻿#include "ImGuiExtras.h"
#include <algorithm>
#include <string>
#include <unordered_map>


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
            // Color updated
        }

        ImGui::PopID();

        // Drag gradient points
        ImGui::SetCursorScreenPos(handle_pos);
        ImGui::InvisibleButton("DragHandle", ImVec2(10, 10),
                               ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);

        // If the user is moving the point
        if (ImGui::IsItemActive() && ImGui::IsMouseDragging(0))
        {
            float new_pos = (ImGui::GetMousePos().x - canvas_pos.x) / canvas_size.x;
            p.position = ImClamp(new_pos, 0.0f, 1.0f);
        }

        // If the user right-clicks a point, mark it for deletion
        if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
        {
            pointToDelete = static_cast<int>(i);
        }
    }

    // Add a new point by clicking on the gradient bar
    if (ImGui::InvisibleButton("AddGradientPoint", canvas_size, ImGuiButtonFlags_MouseButtonLeft) &&
        ImGui::IsMouseClicked(0))
    {
        newPointPos = (ImGui::GetMousePos().x - canvas_pos.x) / canvas_size.x;
        addNewPoint = true;
    }

    // Add the new point if a click on the bar was detected
    if (addNewPoint)
    {
        points.push_back({ImClamp(newPointPos, 0.0f, 1.0f), ImVec4(1, 1, 1, 1)});

        // Sort points by position to keep the gradient clean
        std::sort(points.begin(), points.end(),
                  [](const GradientPoint& a, const GradientPoint& b) { return a.position < b.position; });
    }

    // Delete the point if it was marked for deletion
    if (pointToDelete >= 0 && points.size() > 2)
    { // Always keep at least 2 points
        points.erase(points.begin() + pointToDelete);
    }
}


void GradientEditor::ShowGradientEditor(std::vector<GradientPoint>& points)
{
    if (ImGui::TreeNodeEx("Gradient Editor", ImGuiTreeNodeFlags_None))
    {
        // Draw gradient
        ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
        ImVec2 canvas_size = ImVec2(ImGui::GetContentRegionAvail().x - 10, 30);
        DrawGradientBar(points, canvas_pos, canvas_size);
        ImGui::InvisibleButton("GradientCanvas", canvas_size);

        // Edit gradient points
        EditGradientPoints(points, canvas_pos, canvas_size);

        ImGui::TreePop();
    }
}


void CurveEditor::DrawCurve(const char* label, std::vector<CurvePoint>& points, ImVec2 graph_size)
{
    ImVec2 graph_pos = ImGui::GetCursorScreenPos();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImU32 lineColor = IM_COL32(255, 165, 0, 255);     // Orange
    ImU32 pointColor = IM_COL32(0, 200, 255, 255);    // Bright blue
    ImU32 borderColor = IM_COL32(255, 255, 255, 255); // White for the border
    ImU32 gridColor = IM_COL32(100, 100, 100, 100);   // Light gray for grid lines
    float radius = 5.0f;                              // Point size

    // Unique selection map per curve
    static std::unordered_map<std::string, int> selectedPoints;
    int& selectedPoint = selectedPoints[label]; // Each curve has its own selection

    // Sort points by time before drawing
    std::sort(points.begin(), points.end(), [](const CurvePoint& a, const CurvePoint& b) { return a.time < b.time; });

    // Convert points to graph space
    auto ToGraphSpace = [&](const CurvePoint& p) -> ImVec2 {
        return ImVec2(graph_pos.x + p.time * graph_size.x, graph_pos.y + (1.0f - p.value) * graph_size.y);
    };

    // Draw the graph border
    draw_list->AddRect(graph_pos, ImVec2(graph_pos.x + graph_size.x, graph_pos.y + graph_size.y), borderColor, 0.0f,
                       ImDrawFlags_None, 2.0f);

    // Draw horizontal reference lines (0.0, 0.5, 1.0)
    for (float i = 0.0f; i <= 1.0f; i += 0.5f)
    {
        float y_pos = graph_pos.y + (1.0f - i) * graph_size.y;
        draw_list->AddLine(ImVec2(graph_pos.x, y_pos), ImVec2(graph_pos.x + graph_size.x, y_pos), gridColor);
    }

    // Draw vertical reference lines (0.0, 0.5, 1.0)
    for (float i = 0.0f; i <= 1.0f; i += 0.5f)
    {
        float x_pos = graph_pos.x + i * graph_size.x;
        draw_list->AddLine(ImVec2(x_pos, graph_pos.y), ImVec2(x_pos, graph_pos.y + graph_size.y), gridColor);
    }

    // Draw the curve line
    for (size_t i = 1; i < points.size(); i++)
    {
        ImVec2 p1 = ToGraphSpace(points[i - 1]);
        ImVec2 p2 = ToGraphSpace(points[i]);
        draw_list->AddLine(p1, p2, lineColor, 2.0f);
    }

    // Interaction handling: detect clicks and drag
    ImVec2 mouse_pos = ImGui::GetIO().MousePos;
    bool mouse_clicked = ImGui::IsMouseClicked(0);
    bool mouse_held = ImGui::IsMouseDown(0);
    bool mouse_remove = ImGui::IsMouseClicked(1);

    for (size_t i = 0; i < points.size(); i++)
    {
        ImVec2 point_screen = ToGraphSpace(points[i]);

        // Check if the user clicks on a point
        if (mouse_clicked &&
            ImLengthSqr(ImVec2(mouse_pos.x - point_screen.x, mouse_pos.y - point_screen.y)) < (radius * radius * 4))
        {
            selectedPoint = (int)i;
        }

        // If the user right-clicks, delete the point
        if (mouse_remove &&
            ImLengthSqr(ImVec2(mouse_pos.x - point_screen.x, mouse_pos.y - point_screen.y)) < (radius * radius * 4))
        {
            points.erase(points.begin() + i);
            --i;
        }

        // If a point is selected and the mouse is held, move the point
        if (selectedPoint == (int)i && mouse_held)
        {
            float newTime = (mouse_pos.x - graph_pos.x) / graph_size.x;
            float newValue = 1.0f - (mouse_pos.y - graph_pos.y) / graph_size.y;
            points[i].time = std::clamp(newTime, 0.0f, 1.0f);
            points[i].value = std::clamp(newValue, 0.0f, 1.5f); // Configurable upper limit
        }

        // Draw control points
        draw_list->AddCircleFilled(point_screen, radius, pointColor);
    }

    // Release selection when the mouse is released
    if (!mouse_held)
    {
        selectedPoint = -1;
    }

    // UI space for the graph
    ImGui::Dummy(graph_size);

    // Button to add new points
    if (ImGui::Button(std::string("Add Point##").append(label).c_str())) // Unique label for each button
    {
        points.push_back({0.5f, 1.0f}); // New point in the middle
    }
}


float CurveEditor::GetCurveValue(float time, const std::vector<CurvePoint>& points)
{
    if (points.empty())
        return 0.0f; 
    if (time <= points.front().time)
        return points.front().value; 
    if (time >= points.back().time)
        return points.back().value; 

   
    for (size_t i = 1; i < points.size(); i++)
    {
        if (time <= points[i].time)
        {
            const CurvePoint& p1 = points[i - 1];
            const CurvePoint& p2 = points[i];

            // Linear interpolation
            float t = (time - p1.time) / (p2.time - p1.time);
            return p1.value * (1.0f - t) + p2.value * t; // LERP
        }
    }

    return 0.0f; // Should not reach here
}