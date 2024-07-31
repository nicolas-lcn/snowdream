#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "model.h"
#include "transform.h"

// std
#include <memory>

namespace Vengine
{

	class GameObject
	{
	public:
		using id_t = unsigned long;

		static GameObject createGameObject()
		{
			static id_t currentId = 0;
			return GameObject(currentId++);
		}

		GameObject(const GameObject&) = delete;
		GameObject &operator=(const GameObject&) = delete;
		GameObject(GameObject&&) = default;
		GameObject &operator=(GameObject&&) = default;

		inline id_t getId() {return id;}

		std::shared_ptr<Model> model;
		Transform transform;


	private:
		GameObject(id_t objId) : id(objId){}

		id_t id;
	}
}


#endif // GAME_OBJECT_H