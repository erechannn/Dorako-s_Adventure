#include"Player.h"
#include"Assets.h"
#include"World/IWorld.h"
#include"World/Field.h"

Player::Player(IWorld* world, GSvector3 position) :
	Actor{ Mesh_Player } {

}
void Player::update(float delta_time) {

}
void Player::draw()const {

}
void Player::react(Actor& other) {

}