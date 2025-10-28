#include "Field.h"
#include "../Shape/Ray.h"
#include "../Shape/Line.h"
#include "../Collider/BoundingSphere.h"
#include "../Actor/FieldActor/FieldActor.h"

// �R���X�g���N�^
Field::Field(GSuint octree, GSuint collider, GSuint skybox) :
    octree_{ octree }, collider_{ collider }, skybox_{ skybox } {
}

// �f�X�g���N�^
Field::~Field() {
    clear();
}

// �X�V
void Field::update(float delta_time) {
    actors_.update(delta_time);
    actors_.remove();
}

// �`��
void Field::draw() const {
    gsDrawSkyboxCubemap(skybox_);
    gsDrawOctree(octree_);
    // �t�B�[���h�A�N�^�[�̕`��
    draw_actors();
}

// �t�B�[���h�p�A�N�^�[�̕`��
void Field::draw_actors() const {
    actors_.draw();
}

// ���C�Ƃ̏Փ˔���
bool Field::collide(const Ray& ray, float max_distance, GSvector3* intersect, GSplane* plane, Actor** actor) const {
    Line line;
    line.start = ray.position;
    line.end = ray.position + (ray.direction.normalized() * max_distance);
    return collide(line, intersect, plane, actor);
}

// �����Ƃ̏Փ˔���
bool Field::collide(const Line& line, GSvector3* intersect, GSplane* plane, Actor** actor) const {
    // �I�N�g���[�Ƃ̌�������
    GSvector3 octree_intersection_point;
    GSplane octree_intersection_plane;
    bool intersection_octree = gsOctreeCollisionLine(
        gsGetOctree(collider_),
        &line.start, &line.end, &octree_intersection_point, &octree_intersection_plane) == GS_TRUE;
    // �A�N�^�[�Ƃ̌�������
    GSvector3 actor_intersection_point;
    GSplane actor_intersection_plane;
    Actor* intersection_actor = actors_.collide(line, &actor_intersection_point, &actor_intersection_plane);
    // �I�N�g���[�ƃA�N�^�[�����ɏՓ˂��Ă��邩�H
    if (intersection_octree && intersection_actor != nullptr) {
        // �n�_�ƌ�_�̋������v�Z����
        float distance_to_actor = line.start.distance(actor_intersection_point);
        float distance_to_octree = line.start.distance(octree_intersection_point);
        if (distance_to_octree < distance_to_actor) {
            // �I�N�g���[�̕����߂��ꍇ�̓A�N�^�[�ɏՓ˂��Ă��Ȃ����Ƃɂ���
            intersection_actor = nullptr;
        }
        else {
            // �A�N�^�[�̕����߂��ꍇ�I�N�g���[�ɏՓ˂��Ă��Ȃ����Ƃɂ���
            intersection_octree = false;
        }
    }
    // �I�N�g���[�ɏՓ˂��Ă��邩�H
    if (intersection_octree) {
        if (intersect != nullptr) {
            *intersect = octree_intersection_point;
        }
        if (plane != nullptr) {
            *plane = octree_intersection_plane;
        }
        if (actor != nullptr) {
            *actor = nullptr;
        }
        return true;
    }
    // �A�N�^�[�ɏՓ˂��Ă��邩�H
    if (intersection_actor) {
        if (intersect != nullptr) {
            *intersect = actor_intersection_point;
        }
        if (plane != nullptr) {
            *plane = actor_intersection_plane;
        }
        if (actor != nullptr) {
            *actor = intersection_actor;
        }
        return true;
    }
    // �����Փ˂��Ȃ�����
    return false;
}

// ���̂Ƃ̏Փ˔���
bool Field::collide(const BoundingSphere& sphere, GSvector3* center, Actor** actor) const {
    // �Փ˔����̔��a
    GSvector3 collided_center = sphere.center;
    // �t�B�[���h�p�̃A�N�^�[�Ƃ̏Փ˔�����s��
    Actor* collided_actor = actors_.collide(sphere, &collided_center);
    // �I�N�g���[�ƏՓ˔�����s��
    bool collided_octree = gsOctreeCollisionSphere(
        gsGetOctree(collider_),
        &collided_center, sphere.radius, &collided_center) == GS_TRUE;
    if (center != nullptr) {
        *center = collided_center;
    }
    if (actor != nullptr) {
        *actor = collided_actor;
    }
    // �I�N�g���[���A�N�^�[�ǂ��炩�ɏՓ˂��Ă������H
    return collided_octree || collided_actor != nullptr;
}

// �t�B�[���h�p�A�N�^�[�̒ǉ�
void Field::add(Actor* actor) {
    actors_.add(actor);
}

// �t�B�[���h�p�A�N�^�[�̏���
void Field::clear() {
    actors_.clear();
}

