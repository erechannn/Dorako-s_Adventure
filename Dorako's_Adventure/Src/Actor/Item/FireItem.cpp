#include "FireItem.h"
#include "../../World/IWorld.h"
#include "../Player/Player.h"
#include "../../Assets.h"

//コンストラクタ
FireItem::FireItem(IWorld* world, GSvector3 position) :
	Character{Mesh_Fire}{
	world_ = world;//ワールド
	tag_ = "ItemTag";//タグ
	name_ = "FireItem";//名前
	transform_.position(position);//位置
	collider_ = BoundingSphere{ 0.5f,GSvector3{0.0f,0.0f,0.0f} };//当たり判定
	mesh_->transform(transform_.localToWorldMatrix());
}

//更新
void FireItem::update(float delta_time) {

}
//描画
void FireItem::draw()const {

}
//当たった時の判定
void FireItem::react(Actor& other) {
	if (other.tag() == "PlayerTag") {
		
	}
}

//プレイヤーに当たった時の判定
void FireItem::player_react(Player& player) {
	if (player.fire_count() <= 3) return;
	player.add_fire_count();
}