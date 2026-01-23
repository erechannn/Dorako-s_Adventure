#include "CameraRotateAround.h"
#include "World/IWorld.h"
#include "Shape/Line.h"
#include "World/Field.h"
#include "../LookRotation.h"

#include <iostream>
#include <imgui/imgui.h>


const GSvector3 PlayerOffset{ 0.0f,0.0f,-7.0f };

const GSvector3 ReferencePointOffset{ 0.0f,2.0f,0.0f };

CameraRotateAround::CameraRotateAround(IWorld* world,
	const GSvector3& position, const GSvector3& at) {
	world_ = world;
	tag_ = "CameraTag";
	name_ = "Camera";
	collider_ = BoundingSphere{ 0.5f,{0.0f,0.0f,0.0f} };
	transform_.position(position);
	first_position_ = position;
	transform_.lookAt(at);
	pitch_ = (at - position).getPitch();
	yaw_ = (at - position).getYaw();
}
void CameraRotateAround::update(float delta_time) {
	gsXBoxPadGetRightAxis(0, &result);
	//スティックの移動量の数値化
	float result_normalize = std::sqrt(result.x * result.x + result.y * result.y);
	//カメラ用のプレイヤーを参照(正面ベクトルがずれないため)
	Actor* dummy_player = world_->find_actor("DummyPlayer");
	if (dummy_player == nullptr)return;
	player_up_ = dummy_player->transform().up();
	if (transform_.forward().magnitude() < 0.1f) {
		transform_.forward(dummy_player->transform().forward());
	}

	//デバック用のキーボード操作
	if (gsGetKeyState(GKEY_LEFT)) yaw_ -= 0.2f * delta_time;
	if (gsGetKeyState(GKEY_RIGHT))yaw_ += 0.2f * delta_time;
	if (gsGetKeyState(GKEY_UP))pitch_ -= 0.5f * delta_time;
	if (gsGetKeyState(GKEY_DOWN))pitch_ += 0.5f * delta_time;

	//右スティックの移動量からカメラの移動
	if (is_reverse_result_x_) {
		if (result.x > 0.0f) yaw_ += 2.0f * result_normalize * delta_time;
		if (result.x < 0.0f)yaw_ -= 2.0f * result_normalize * delta_time;
	}
	else {
		if (result.x < 0.0f) yaw_ += 2.0f * result_normalize * delta_time;
		if (result.x > 0.0f)yaw_ -= 2.0f * result_normalize * delta_time;
	}
	if (is_reverse_result_y_) {
		if (result.y < 0.0f)pitch_ += 1.0f * result_normalize * delta_time;
		if (result.y > 0.0f) pitch_ -= 1.0f * result_normalize * delta_time;
	}
	else {
		if (result.y > 0.0f)pitch_ += 1.0f * result_normalize * delta_time;
		if (result.y < 0.0f) pitch_ -= 1.0f * result_normalize * delta_time;
	}
	//上下の上限を持たせる
	pitch_ = CLAMP(pitch_, -70.0f, 30.0f);

	//注視点の適応
	GSvector3 at = dummy_player->transform().transformPoint(ReferencePointOffset);
	//ピッチとヨウの単一ベクトル
	GSvector3 view = GSvector3::createFromPitchYaw(pitch_, yaw_)*10.0f;
	//プレイヤーの上方向ベクトルを軸に回転
	GSvector3 position = dummy_player->transform().transformPoint(view);
	//カメラがフィールド内にあるオブジェクトに当たったらカメラを近づける
	GSvector3 center;
	if (world_->field()->collide(collider(), &center)) {
		//フィールドとの当たり判定
		Line line{ at, position };
		GSvector3 intersect;
		if (world_->field()->collide(line, &intersect)) {
			//当たった位置にカメラの位置を補正する
			position = intersect;
		}
	}

	//カメラのリセット
	if (gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_RIGHT_THUMB) || gsGetKeyState(GKEY_L)) {
		yaw_ = -180.0f;
		pitch_ = -20.0f;
	}

	//カメラの移動
	transform_.position(position);

	//カメラ用のプレイヤーを軸にローカル軸で上方向ベクトルを更新する
	GSvector3 up=dummy_player->transform().transformVector(GSvector3{0.0f,1.0f,0.0f});
	transform_.lookAt(at,up);

	//ImGui::Begin("Camera");
	//ImGui::Text("transform_position:x:%f y:%f z:%f", transform_.position().x, transform_.position().y, transform_.position().z);
	//ImGui::Text("Up:x:%f y:%f z:%f", transform_.up().x, transform_.up().y, transform_.up().z);
	//ImGui::Text("forward:x:%f y:%f z:%f", transform_.forward().x, transform_.forward().y, transform_.forward().z);
	//ImGui::Text("at:x:%f y:%f z:%f", at.x, at.y, at.z);
	//ImGui::Text("view:x:%f y:%f z:%f", view.x, view.y, view.z);
	//ImGui::Text("position:x:%f y:%f z:%f", position.x, position.y, position.z);
	//ImGui::Text("yaw:%f", yaw_);
	//ImGui::Text("pitch:%f", pitch_);

	//ImGui::End();

}
void CameraRotateAround::draw()const {
	GSvector3 eye = transform_.position();
	GSvector3 at = eye + transform_.forward();
	GSvector3 up = transform_.up();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		eye.x, eye.y, eye.z,     // 視点の位置
		at.x, at.y, at.z,     // 注視点の位置
		up.x, up.y, up.z      // 視点の上方向
	);


}