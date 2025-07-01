#pragma once
#include "Component.h"
#include <vector>
#include <chrono>
#include <algorithm>
#include <iostream>
namespace dae
{
	struct TransformTest
	{
		float matrix[16] = {
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1 };
	};

	class GameObject3D
	{
	public:
		TransformTest transform;
		int ID;
	};

	class GameObject3DAlt {
	public:
		std::vector<TransformTest> transforms;
		std::vector<int> IDs;

		GameObject3DAlt(size_t size) : transforms(size), IDs(size) {}
	};


	class ImGuiComponent final : public Component
	{
	public:
		ImGuiComponent(GameObject& pOwner);
		void Update() override; 
		void Render() const override;
		void ImGuiRender();
	private:
		void TrashTheCache(int num);
		void TrashTheCacheGameObject3D(int num);
		void TrashTheCacheGameObject3DAlt(int num);

		std::vector<long long> m_PlotDataEx1;
		std::vector<long long> m_PlotDataEx2;
		std::vector<long long> m_PlotDataEx3;
	};
}

