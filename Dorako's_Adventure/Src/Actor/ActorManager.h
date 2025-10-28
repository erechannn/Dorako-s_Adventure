#ifndef ACTOR_MANAGER_H_
#define ACTOR_MANAGER_H_

#include <list>
#include <vector>
#include<string>

class Actor;

class ActorManager {
public:
	ActorManager() = default;
	virtual ~ActorManager();
	void add(Actor* actor);
	void update(float delta_time);
	void late_update(float delta_time);
	void draw()const;
	void draw_transparent()const;
	void draw_gui()const;
	void collide();
	void remove();
	Actor* find(const std::string& name)const;
	std::vector<Actor*>find_with_tag(const std::string& tag)const;
	int count()const;
	int count_with_tag(const std::string& tag)const;
	void send_message(const std::string& message, void* param);
	void clear();
	ActorManager(const ActorManager& other) = delete;
	ActorManager& operator=(const ActorManager& other) = delete;

protected:
	std::list<Actor*>actors_;
};


#endif // !ACTOR_MANAGER_H_
