#ifndef ACTOR_H_
#define ACTOR_H_

#include<gslib.h>
#include<GStransform.h>
#include<string>
#include"Collider/BoundingSphere.h"
#include"../Mesh/AnimatedMesh.h"

class IWorld;

class Actor {
public:
	// コンストラクタ
	Actor() {};
	Actor(GSuint mesh);
	// 仮想デストラクタ
	virtual ~Actor();

	virtual void update(float delta_time);

	virtual void late_update(float delta_time);

	virtual void draw()const;

	virtual void draw_transparent()const;

	virtual void draw_gui()const;

	virtual void react(Actor& other);

	virtual void handle_message(const std::string& message, void* param);

	void collide(Actor& other);

	void die();

	bool is_collide(const Actor& other)const;

	bool is_dead()const;

	const std::string& name()const;

	const std::string& tag()const;

	const GStransform& transform()const;

	GStransform& transform();

	GSvector3 velocity()const;
	BoundingSphere collider()const;


	AnimatedMesh* mesh_{};

	Actor(const Actor& other) = delete;
	Actor& operator=(const Actor& other) = delete;

protected:
	IWorld* world_{ nullptr };
	std::string tag_;
	std::string name_;
	GStransform transform_;
	GSvector3 velocity_{ 0.0f,0.0f,0.0f };
	bool enable_collider_{ true };
	BoundingSphere collider_;
	bool dead_{ false };
};



#endif // !ACTOR_H_
