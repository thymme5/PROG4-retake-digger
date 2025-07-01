#include "ImGuiComponent.h"
#include "imgui.h"
#include "imgui_plot.h"

using namespace dae;

ImGuiComponent::ImGuiComponent(GameObject& pOwner) :
	Component(pOwner)
{

}
void ImGuiComponent::Update()
{

}
void ImGuiComponent::Render() const
{

}
//void ImGuiComponent::ImGuiRender()
//{
//    // Ex 2: GameObject3D graph
//    ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_Once);
//    ImGui::Begin("Excercise 2");
//
//    static int samples = 10;
//    ImGui::InputInt("##Samples", &samples);
//    ImGui::SameLine();
//    ImGui::Text("# samples");
//
//    if (ImGui::Button("Trash the cache"))
//    {
//        TrashTheCache(samples);
//    }
//
//    if (!m_PlotDataEx1.empty())
//    {
//        ImGui::Text("Graph: GameObject3D Execution Time");
//
//        std::vector<float> plotDataGameObject3D;
//        for (long long t : m_PlotDataEx1)
//        {
//            plotDataGameObject3D.push_back(static_cast<float>(t));
//        }
//
//        // Plot the data for GameObject3D
//        ImGui::PlotLines("GameObject3D", plotDataGameObject3D.data(), static_cast<int>(plotDataGameObject3D.size()), 0, NULL, FLT_MAX, FLT_MAX, ImVec2(0, 150));
//    }
//
//    ImGui::End();
//
//    //Ex 3: GameObject3D graph
//    ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_Once);
//    ImGui::Begin("Excercise 3");
//
//    static int secondSamples = 10;
//
//    ImGui::InputInt("##Samples", &secondSamples);
//    ImGui::SameLine();
//    ImGui::Text("# samples");
//
//    if (ImGui::Button("Trash the cache with GameObject3D"))
//    {
//        TrashTheCacheGameObject3D(secondSamples);
//    }
//
//    if (ImGui::Button("Trash the cache with GameObject3DAlt"))
//    {
//        TrashTheCacheGameObject3DAlt(secondSamples);
//    }
//
//    // Plot the data for GameObject3DAlt
//    if (!m_PlotDataEx2.empty())
//    {
//        ImGui::Text("Graph: GameObject3DAlt Execution Time");
//
//        std::vector<float> plotDataGameObject3DAlt;
//        for (long long t : m_PlotDataEx2)
//        {
//            plotDataGameObject3DAlt.push_back(static_cast<float>(t));
//        }
//
//        // Plot the data for GameObject3DAlt
//        ImGui::PlotLines("GameObject3DAlt", plotDataGameObject3DAlt.data(), static_cast<int>(plotDataGameObject3DAlt.size()), 0, NULL, FLT_MAX, FLT_MAX, ImVec2(0, 150));
//    }
//
//    if (!m_PlotDataEx2.empty() && !m_PlotDataEx3.empty())
//    {
//        ImGui::Text("Combined Graph: GameObject3D vs GameObject3DAlt");
//
//        std::vector<float> plotDataGameObject3D;
//        std::vector<float> plotDataGameObject3DAlt;
//
//        for (long long t : m_PlotDataEx2)
//        {
//            plotDataGameObject3D.push_back(static_cast<float>(t));
//        }
//
//        for (long long t : m_PlotDataEx3)
//        {
//            plotDataGameObject3DAlt.push_back(static_cast<float>(t));
//        }
//
//        // Find the minimum size between the two datasets
//        int dataCount = static_cast<int>(std::min(plotDataGameObject3D.size(), plotDataGameObject3DAlt.size()));
//
//        // Plot the first line (GameObject3D data)
//        ImGui::PlotLines("GameObject3D", plotDataGameObject3D.data(), dataCount, 0, NULL, FLT_MAX, FLT_MAX, ImVec2(0, 150));
//
//        // Plot the second line (GameObject3DAlt data)
//        ImGui::PlotLines("GameObject3DAlt", plotDataGameObject3DAlt.data(), dataCount, 0, NULL, FLT_MAX, FLT_MAX, ImVec2(0, 150));
//    }
//
//    ImGui::End();
//}
void ImGuiComponent::ImGuiRender()
{
    ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_Once);
    ImGui::Begin("Exercise 2");

    static int samples = 10;
    ImGui::PushItemWidth(150);
    ImGui::InputInt("##Samples", &samples);
    ImGui::PopItemWidth();
    ImGui::SameLine();
    ImGui::Text("# samples");

    if (ImGui::Button("Trash the cache"))
    {
        TrashTheCache(samples);
    }

    if (!m_PlotDataEx1.empty())
    {
        std::vector<float> plotDataGameObject3D;
        for (long long t : m_PlotDataEx1)
        {
            plotDataGameObject3D.push_back(static_cast<float>(t));
        }

        ImGui::PlotLines("GameObject3D", plotDataGameObject3D.data(), static_cast<int>(plotDataGameObject3D.size()), 0, NULL, FLT_MAX, FLT_MAX, ImVec2(0, 150));
    }

    ImGui::End();

    ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_Once);
    ImGui::Begin("Exercise 3");

    static int secondSamples = 10;
    ImGui::PushItemWidth(150);
    ImGui::InputInt("##Samples2", &secondSamples);
    ImGui::PopItemWidth();
    ImGui::SameLine();
    ImGui::Text("# samples");

    if (ImGui::Button("Trash the cache with GameObject3D"))
    {
        TrashTheCacheGameObject3D(secondSamples);
    }

    if (ImGui::Button("Trash the cache with GameObject3DAlt"))
    {
        TrashTheCacheGameObject3DAlt(secondSamples);
    }

    if (!m_PlotDataEx2.empty())
    {
        std::vector<float> plotDataGameObject3D;
        for (long long t : m_PlotDataEx2)
        {
            plotDataGameObject3D.push_back(static_cast<float>(t));
        }

        ImGui::PlotLines("GameObject3D", plotDataGameObject3D.data(), static_cast<int>(plotDataGameObject3D.size()), 0, NULL, FLT_MAX, FLT_MAX, ImVec2(0, 150));
    }

    if (!m_PlotDataEx3.empty())
    {
        std::vector<float> plotDataGameObject3DAlt;
        for (long long t : m_PlotDataEx3)
        {
            plotDataGameObject3DAlt.push_back(static_cast<float>(t));
        }

        ImGui::PlotLines("GameObject3DAlt", plotDataGameObject3DAlt.data(), static_cast<int>(plotDataGameObject3DAlt.size()), 0, NULL, FLT_MAX, FLT_MAX, ImVec2(0, 150));
    }

    if (!m_PlotDataEx2.empty() && !m_PlotDataEx3.empty())
    {
        int dataCount = static_cast<int>(std::min(m_PlotDataEx2.size(), m_PlotDataEx3.size()));

        if (dataCount == 0) return;

        std::vector<float> y_data1(dataCount);
        std::vector<float> y_data2(dataCount);

        for (int i = 0; i < dataCount; i++)
        {
            y_data1[i] = static_cast<float>(m_PlotDataEx2[i]);
            y_data2[i] = static_cast<float>(m_PlotDataEx3[i]);
        }

        std::vector<float> x_data(dataCount);
        for (int i = 0; i < dataCount; i++)
        {
            x_data[i] = static_cast<float>(1 << i);
        }

        const float* y_list[2] = { y_data1.data(), y_data2.data() };

        //color
        ImU32 colors[2] = { IM_COL32(255, 0, 0, 255), // Red
                             IM_COL32(0, 255, 0, 255)  // Green
        };

        ImGui::PlotConfig conf;
        conf.values.ys_list = y_list;  
        conf.values.ys_count = 2;       
        conf.values.count = dataCount;  
        conf.values.colors = colors;
        conf.values.xs = x_data.data();       
        conf.frame_size = ImVec2(400, 150);
        conf.line_thickness = 2.0f;
        conf.tooltip.show = true;
        conf.overlay_text = "Red: GameObject3D, Green: GameObject3DAlt";

        conf.scale.min = 0.0f;  
        conf.scale.max = 350000.0f;  

        ImGui::Plot("Combined Comparison", conf);
    }


    ImGui::End();
}




void ImGuiComponent::TrashTheCache(int num)
{
    //heap ah
    const int ARRAY_SIZE = 1 << 26;
    std::vector<int> arr(ARRAY_SIZE, 1);

    for (int step = 1; step <= 1024; step *= 2)
    {
        std::vector<long long> times;

        //run multiple times
        for (int run = 0; run < num; ++run)
        {
            const auto start = std::chrono::high_resolution_clock::now();

            //modify array
            for (int i = 0; i < arr.size(); i += step)
            {
                arr[i] *= 2;
            }

            const auto end = std::chrono::high_resolution_clock::now();
            const auto total = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
            times.push_back(total);
        }

        //remove highest and lowest
        std::sort(times.begin(), times.end());
        times.erase(times.begin());
        times.erase(times.end() - 1);

        //average it out
        long long avg_time = 0;
        for (long long t : times) avg_time += t;
        avg_time /= times.size();
        m_PlotDataEx1.push_back(avg_time);
    }
}

void ImGuiComponent::TrashTheCacheGameObject3D(int num)
{
    const int ARRAY_SIZE = 1 << 26;
    std::vector<GameObject3D> arr(ARRAY_SIZE);

    for (int step = 1; step <= 1024; step *= 2)
    {
        std::vector<long long> times;

        for (int run = 0; run < num; ++run)
        {
            const auto start = std::chrono::high_resolution_clock::now();

            for (int i = 0; i < arr.size(); i += step)
            {
                arr[i].ID *= 2;
            }

            const auto end = std::chrono::high_resolution_clock::now();
            const auto total = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
            times.push_back(total);
        }

        std::sort(times.begin(), times.end());
        times.erase(times.begin());
        times.erase(times.end() - 1);

        long long avg_time = 0;
        for (long long t : times) avg_time += t;
        avg_time /= times.size();

        m_PlotDataEx2.push_back(avg_time);
    }
}

void ImGuiComponent::TrashTheCacheGameObject3DAlt(int num)
{
    const int ARRAY_SIZE = 1 << 26;
    GameObject3DAlt arr(ARRAY_SIZE);

    for (int step = 1; step <= 1024; step *= 2) {
        std::vector<long long> times;

        for (int run = 0; run < num; ++run) {
            const auto start = std::chrono::high_resolution_clock::now();

            for (int i = 0; i < arr.IDs.size(); i += step) {
                arr.IDs[i] *= 2;
            }

            const auto end = std::chrono::high_resolution_clock::now();
            const auto total = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
            times.push_back(total);
        }

        std::sort(times.begin(), times.end());
        times.erase(times.begin());
        times.erase(times.end() - 1);

        long long avg_time = 0;
        for (long long t : times) avg_time += t;
        avg_time /= times.size();

        m_PlotDataEx3.push_back(avg_time);
    }
}