#include "ComponentRigidBody.h"
#include "Interface.h"

ComponentRigidBody::ComponentRigidBody(GameObject * parent) : Component(Component::Type::RIGIDBODY, parent)
{
}

ComponentRigidBody::~ComponentRigidBody()
{
	RELEASE(collider);
}

bool ComponentRigidBody::OnDebugDraw() const
{
	if (collider != nullptr)
		collider->OnDebugDraw();

	return true;
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

void ComponentRigidBody::LoadRigidBody(Collider::Type collider_type, float mass, MotionType motion_type)
{
	this->motion_type = motion_type;
	this->mass = mass;

	if (collider != nullptr)
		RELEASE(collider);

	collider = CreateCollider(collider_type);
}

Collider* ComponentRigidBody::CreateCollider(Collider::Type type)
{
	static_assert(Collider::Type::UNKNOWN == 3, "Update factory code");

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
