#ifndef ACTOR_H_
#define ACTOR_H_

#include<gslib.h>
#include<GStransform.h>
#include<string>
#include"Collider/BoundingSphere.h"
#include"../Mesh/AnimatedMesh.h"
#include "../Tween/Tween.h"

class IWorld;

class Actor {
public:
	// コンストラクタ
	Actor() {};
	Actor(GSuint mesh);
	// 仮想デストラクタ
	virtual ~Actor();
	//更新
	virtual void update(float delta_time);
	//遅延更新
	virtual void late_update(float delta_time);
	//描画
	virtual void draw()const;
	//
	virtual void draw_transparent()const;
	//GUIの描画
	virtual void draw_gui()const;
	//オブジェクトに当たった時のリアクション
	virtual void react(Actor& other);

	virtual void handle_message(const std::string& message, void* param);
	//当たり判定
	void collide(Actor& other);
	//死亡
	void die();
	//トゥーイン
	TweenUnit& move_to(const GSvector3& to, float duration);
	//コライダーの有無
	bool is_collide(const Actor& other)const;
	//死んでいるかの有無
	bool is_dead()const;
	//名前を返す
	const std::string& name()const;
	//タグを返す
	const std::string& tag()const;
	//トランスフォームを返す
	const GStransform& transform()const;
	//トランスフォームを変更する
	GStransform& transform();
	//移動量を返す
	GSvector3 velocity()const;
	//丸形当たり判定を返す
	BoundingSphere collider()const;
	//アニメーション
	AnimatedMesh* mesh_{};
	//コピー禁止
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
