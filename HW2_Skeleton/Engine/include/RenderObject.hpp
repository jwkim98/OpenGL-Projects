#pragma once

#include <iostream>
#include <exception>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Camera.hpp>
#include <Mesh.hpp>
#include <Material.hpp>
#include <vector>

namespace Engine
{
	using vec3 = glm::vec3;
    class RenderObject
    {
    private:
        glm::vec3 _position;
        glm::mat4 _orientation;
        glm::vec3 _scale;

		float _rotation_speed;


    protected:
        Mesh* _mesh;
        Material* _material;
		int _index;

		//TO DO implement hierarchical structure
		RenderObject *parent = NULL;

    public:
        RenderObject();
        RenderObject(Mesh* mesh, Material* material);

		void ChangeMaterial(Material* mat);
        void Render(Camera* cam);
        void SetMesh(Mesh* mesh) { _mesh = mesh; }
        void SetMaterial(Material* material) { _material = material; }
        glm::vec3 GetPosition() { return _position; }
        void SetPosition(glm::vec3 position) { _position = position; }
        glm::mat4 GetOrientation() { return _orientation; }
        void SetOrientation(glm::mat4 orientation) { _orientation = orientation; }
        glm::vec3 GetScale() { return _scale; }
        void SetScale(glm::vec3 scale) { _scale = scale; }
        glm::mat4 GetWorldTransform();

		int GetIndex() { return _index; }
		void SetIndex(int index) { _index = index; }

		void AddParent(RenderObject *parent_object) { parent = parent_object; };

		virtual void UpdateOrientation(bool direction)
		{
			// do nothing
		}

		virtual void SetProperties(double radius, double verticalSpeed, double horizontalSpeed, double radianSpeed){}


    };

	class SnowFlake : public RenderObject
	{
	public:
		SnowFlake(Mesh* mesh, Material* material);
		void SetProperties(double radius, double verticalSpeed, double horizontalSpeed, double radianSpeed);
		void UpdateOrientation(bool direction) override;
	private:
		const double _gravity = 9.8;
		double _radius = 0;
		double _verticalSpeed = 0;
		double _horizontalSpeed = 0;
		double _radianSpeed = 0;
		double _currentRadius = 0;
	};
}