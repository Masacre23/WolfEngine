#include "Application.h"
#include "ModulePhysics.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "ComponentRigidBody.h"
#include "Interface.h"

ComponentRigidBody::ComponentRigidBody(GameObject * parent) : Component(Component::Type::RIGIDBODY, parent)
{
}

ComponentRigidBody::~ComponentRigidBody()
{
	RELEASE(collider);
}

void ComponentRigidBody::OnDebugDraw() const
{
	if (collider != nullptr)
		collider->OnDebugDraw();
}

bool ComponentRigidBody::OnEditor()
{
	if (ImGui::CollapsingHeader("RigidBody"))
	{

		ImGui::TextWrapped("Motion type:");
		switch (motion_type)
		{
		case ComponentRigidBody::STATIC:
			ImGui::TextWrapped("Static");
			break;
		case ComponentRigidBody::KINEMATIC:
			ImGui::TextWrapped("Kinematic");
			break;
		case ComponentRigidBody::DYNAMIC:
			ImGui::TextWrapped("Dynamic");
			break;
		default:
			break;
		}

		ImGui::DragFloat("Mass", &mass, 0.1f, 0.0f, 100.0f);

		if (collider != nullptr)
		{
			ImGui::Separator();
			collider->OnEditor();
		}
	}

	return true;
}

void ComponentRigidBody::OnPlay()
{
	rigid_body = App->physics->AddRigidBody(this);
}

void ComponentRigidBody::OnStop()
{
	if (rigid_body != nullptr)
		App->physics->DeleteRigidBody(rigid_body);
}

void ComponentRigidBody::LoadRigidBody(Collider::Type collider_type, float mass, MotionType motion_type)
{
	this->motion_type = motion_type;
	this->mass = mass;

	if (collider != nullptr)
		RELEASE(collider);

	collider = CreateCollider(collider_type);
}

void ComponentRigidBody::getWorldTransform(btTransform& worldTrans) const
{
	worldTrans.setFromOpenGLMatrix(parent->transform->GetGlobalTransformMatrix().Transposed().ptr());
}

void ComponentRigidBody::setWorldTransform(const btTransform& worldTrans)
{
	btVector3 position = worldTrans.getOrigin();
	btQuaternion rotation = worldTrans.getRotation();

	float4x4 global_transform = float4x4(rotation, position);
	float4x4 local_transform = global_transform * parent->GetParent()->GetLocalTransformMatrix().Inverted();

	float3 pos;
	float3 scale;
	Quat rot;
	local_transform.Decompose(pos, rot, scale);
	parent->SetLocalTransform(pos, scale, rot);
}

Collider* ComponentRigidBody::CreateCollider(Collider::Type type)
{
	static_assert(Collider::Type::UNKNOWN == 3, "Update collider factory code");

	Collider* ret = nullptr;

	switch (type)
	{
	case Collider::BOX:
		ret = new ColliderBox();
		break;
	case Collider::SPHERE:
		ret = new ColliderSphere();
		break;
	case Collider::CAPSULE:
		ret = new ColliderCapsule();
		break;
	default:
		break;
	}

	return ret;
}
