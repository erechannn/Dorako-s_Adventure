#include"World.h"
#include"Field.h"
#include"Actor/Actor.h"
#include "../Delay/DelayManager.h"
#include "../Assets.h"

#include <GSstandard_shader.h>
#include <GSeffect.h>

World::~World() {
	clear();
}

void World::update(float delta_time) {
	field_->update(delta_time);
	actors_.update(delta_time);
	actors_.collide();
	actors_.late_update(delta_time);
	actors_.remove();
	camera_->update(delta_time);
	light_->update(delta_time);
	DelayManager::get_instance().update();
	gsUpdateEffect(delta_time);
}

void World::draw()const {
	camera_->draw();
	gsSetEffectCamera();
	light_->draw();
	gsDrawShadowMap(World::shadow_map_callback, (void*)this);

    //field_->draw();

    // 背景のみレンダーターゲットに描画
    gsBeginRenderTarget(Rt_Field);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    field_->draw();
    gsEndRenderTarget();

    // シャドウマップ用のシェーダーをシルエット用のシェーダーとして利用する
    // （シルエットになる部分だけ白色で描画）
    GSuint current_skin_shader = gsGetBindDefaultSkinMeshShader();  // シェーダーの退避
    GSuint current_mesh_shader = gsGetBindDefaultMeshShader();      // シェーダーの退避
    gsBindDefaultSkinMeshShader(GS_SHADOW_MAP_SKIN_MESH_SHADER);
    gsBindDefaultMeshShader(GS_SHADOW_MAP_MESH_SHADER);
    // シルエット用のレンダーターゲットにアクターを描画
    gsBeginRenderTarget(Rt_Silhouette);
    // シルエット以外は黒
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    // カラーバッファを消去
    glClear(GL_COLOR_BUFFER_BIT);
    // デプスバッファの設定を変更
    glDepthFunc(GL_GREATER);    // 遮蔽物があれば描画
    glDepthMask(GL_FALSE);      // デプスバッファを更新しない
    // アクターの描画（シルエットの対象）
    //actors_.draw();
    //プレイヤーのみ標示
    Actor* player = find_actor("Player");
    if(player!=nullptr)
    player->draw();
    // デプスバッファの設定を元に戻す
    glDepthFunc(GL_LESS);       // 遮蔽物がなければ描画
    glDepthMask(GL_TRUE);       // デプスバッファを更新する
    gsEndRenderTarget();
    // シェーダーの復帰
    gsBindDefaultSkinMeshShader(current_skin_shader);
    gsBindDefaultMeshShader(current_mesh_shader);

    // ベースシーンの描画
    gsBeginRenderTarget(Rt_BaseScene);
    // シルエット用のシェーダーを有効にする
    gsBeginShader(Shader_Silhouette);
    // 背景用のテクスチャの設定
    gsBindRenderTargetTextureEx(Rt_Field, 0, 0);
    gsSetShaderParamTexture("u_RenderTexture", 0);
    // シルエット用のテクスチャの設定
    gsBindRenderTargetTextureEx(Rt_Silhouette, 0, 1);
    gsSetShaderParamTexture("u_SilhouetteTexture", 1);
    // レンダーターゲットを描画(背景とシルエットを合成したもの）
    gsDrawRenderTargetEx(Rt_BaseScene);
    // テクスチャのバインド解除
    gsUnbindRenderTargetTextureEx(Rt_Field, 0, 0);
    gsUnbindRenderTargetTextureEx(Rt_Silhouette, 0, 1);
    // シェーダーを無効にする
    gsEndShader();

    //アクターの標示
    actors_.draw();
	actors_.draw_transparent();
	gsDrawEffect();
	actors_.draw_gui();
	score_.draw();
    gsEndRenderTarget();

    // 最終的なベースシーンの描画
    gsBindRenderTargetTexture(Rt_BaseScene, 0);
    gsDrawRenderTarget(Rt_BaseScene);

}
void World::clear() {
	actors_.clear();
	score_.clear();
	delete camera_;
	camera_ = nullptr;
	delete light_;
	light_ = nullptr;
	delete field_;
	field_ = nullptr;
	DelayManager::get_instance().clear();
}

void World::add_camera(Actor* camera) {
	delete camera_;
	camera_ = camera;
}
void World::add_light(Actor* light) {
	delete light_;
	light_ = light;
}
void World::add_field(Field* field) {
	delete field_;
	field_ = field;
}
void World::add_score(int score) {
	score_.add(score);
}

void World::add_actor(Actor* actor) {
	actors_.add(actor);
}
Actor* World::find_actor(const std::string& name)const {
	return actors_.find(name);
}
std::vector<Actor*>World::find_actor_with_tag(const std::string& tag)const {
	return actors_.find_with_tag(tag);
}
int World::count_actor()const {
	return actors_.count();
}
int World::count_actor_with_tag(const std::string& tag)const {
	return actors_.count_with_tag(tag);
}
void World::send_message(const std::string& message, void* param) {
	actors_.send_message(message, param);
}
int World::get_score()const {
	return score_.get();
}

Actor* World::camera() {
	return camera_;
}

Actor* World::light() {
	return light_;
}

Field* World::field() {
	return field_;
}
void World::shadow_map_callback(void* param, const GSmatrix4*, const GSmatrix4*) {
	World* self = (World*)param;
	// シャドウマップにはアクターのみ描画

	self->actors_.draw();
}
