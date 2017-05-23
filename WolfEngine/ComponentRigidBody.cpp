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
		ImGui::Separator();

		if (collider != nullptr)
			collider->OnEditor();
		else
			ImGui::TextWrapped("No collider found");
	}

	return true;
}

void ComponentRigidBody::OnPlay()
{
	rigid_body = App->physics->AddRigidBody(this, parent->transform->GetScale());
	if (collider != nullptr)
		collider->SetCollisionShape(App->physics->GetCollisionShape(rigid_body));
}

void ComponentRigidBody::OnStop()
{
	btCollisionShape* collision_shape = nullptr;
	if (collider != nullptr)
		collision_shape = collider->GetCollisionShape();
	if (rigid_body != nullptr)
		App->physics->DeleteRigidBody(rigid_body, collision_shape);
	if (collider != nullptr)
		collider->UnsetCollisionShape();
}

void ComponentRigidBody::LoadRigidBody(Collider::Type collider_type, float mass, MotionType motion_type)
{
	this->motion_type = motion_type;
	this->mass = mass;

	LoadCollider(collider_type);
}

void ComponentRigidBody::LoadRigidBody(float mass, MotionType motion_type)
{
	this->motion_type = motion_type;
	this->mass = mass;
	if (motion_type != MotionType::DYNAMIC)
		this->mass = 0.0f;
}

void ComponentRigidBody::LoadCollider(Collider::Type collider_type)
{
	if (collider != nullptr)
		RELEASE(collider);

	collider = CreateCollider(collider_type);
}

void ComponentRigidBody::LoadCollider(Collider::Type collider_type, float3* vertices, unsigned num_vertices)
{
	LoadCollider(collider_type);
	collider->SetOnVertices(vertices, num_vertices);
}

void ComponentRigidBody::getWorldTransform(btTransform& worldTrans) const
{
	float4x4 object_global = parent->transform->GetGlobalTransformMatrix();
	float4x4 collider_global = object_global * collider->GetLocalTransform();

	worldTrans.setFromOpenGLMatrix(collider_global.Transposed().ptr());
}

void ComponentRigidBody::setWorldTransform(const btTransform& worldTrans)
{
	float4x4 collider_global;
	worldTrans.getOpenGLMatrix(collider_global.ptr());
	collider_global.Transpose();

	float3 shape_scaling = App->physics->GetCollisionShapeScale(collider->GetCollisionShape());

	float4x4 local_collider = collider->GetLocalTransform().Inverted();
	float3 positions_scaled = local_collider.TranslatePart() * shape_scaling;
	local_collider.SetTranslatePart(positions_scaled);

	float4x4 object_global = collider_global * local_collider;
	float4x4 local_transform = object_global * parent->GetParent()->GetGlobalTransformMatrix().Inverted();

	float3 pos;
	float3 scale;
	Quat rot;
	local_transform.Decompose(pos, rot, scale);
	parent->SetLocalTransform(pos, rot);
}

Collider* ComponentRigidBody::CreateCollider(Collider::Type type)
{
	static_assert(Collider::Type::UNKNOWN == 3, "Update collider factory code");

	Collider* ret = nullptr;

	switch (type)
	{
	case Collider::BOX:
		ret = new ColliderBox(this);
		break;
	case Collider::SPHERE:
		ret = new ColliderSphere(this);
		break;
	case Collider::CAPSULE:
		ret = new ColliderCapsule(this);
		break;
	default:
		break;
	}

	return ret;
}
