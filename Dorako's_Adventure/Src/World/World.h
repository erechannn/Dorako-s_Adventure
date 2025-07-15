#ifndef WORLD_H_
#define WORLD_H_

#include "IWorld.h"
#include"Actor/ActorManager.h"

class World :public IWorld {
public:
	World() = default;
	~World();

	void update(float delta_time);

	void draw()const;

	void clear();

	void add_camera(Actor* camera);

	void add_light(Actor* light);

	void add_field(Field* field);

	virtual void add_actor(Actor* actor)override;
	virtual Actor* find_actor(const std::string& name)const override;
	virtual std::vector<Actor*>find_actor_with_tag(const std::string& tag)const override;
	virtual int count_actor()const override;
	virtual int count_actor_with_tag(const std::string& tag)const override;
	virtual void send_message(const std::string& message, void* param = nullptr)override;

	virtual Actor* camera()override;
	virtual Actor* light()override;
	virtual Field* field()override;

	World(const World& other) = delete;
	World& operator=(const World& other) = delete;


private:
	ActorManager actors_;
	Actor* light_{ nullptr };
	Actor* camera_{ nullptr };
	Field* field_{ nullptr };
};



#endif // 
