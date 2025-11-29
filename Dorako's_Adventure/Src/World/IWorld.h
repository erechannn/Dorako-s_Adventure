#ifndef IWORLD_H_
#define IWORLD_H_

#include<string>
#include<vector>

class Actor;
class Field;

class IWorld {
public:
	virtual ~IWorld() = default;

	virtual void add_actor(Actor* actor) = 0;
	virtual Actor* find_actor(const std::string& name)const = 0;
	virtual void add_score(int score) = 0;
	virtual std::vector<Actor*>find_actor_with_tag(const std::string& tag)const = 0;
	virtual int count_actor()const = 0;
	virtual int count_actor_with_tag(const std::string& tag)const = 0;
	virtual void send_message(const std::string& message, void* param = nullptr) = 0;
	virtual int get_score()const = 0;

	virtual Field* field() = 0;
	virtual Actor* camera() = 0;
	virtual Actor* light() = 0;
};



#endif // !IWORLD_H_
