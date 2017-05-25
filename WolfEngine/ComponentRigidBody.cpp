#include "Application.h"
#include "ModulePhysics.h"
#include "ModuleTimeController.h"
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

		int type = motion_type;
		ImGui::RadioButton("Static", &type, 0);
		ImGui::RadioButton("Kinematic", &type, 1);
		ImGui::RadioButton("Dynamic", &type, 2);
		motion_type = MotionType(type);

		if (motion_type == MotionType::DYNAMIC)
			ImGui::DragFloat("Mass", &mass, 0.1f, 0.0f, 100.0f);
		ImGui::Separator();

		int current_selected = -1;
		if (collider != nullptr)
			current_selected = collider->GetType();

		int new_selected = current_selected;
		const char* collider_names[] = { "Box", "Sphere", "Capsule", "Mesh" };
		if (ImGui::Button("Collider.."))
			ImGui::OpenPopup("Collider Type");
		ImGui::SameLine();
		ImGui::Text(current_selected == -1 ? "<None>" : collider_names[current_selected]);
		if (collider != nullptr)
		{
			ImGui::SameLine();
			if (ImGui::Button("Delete"))
			{
				if (!App->time_controller->IsStopped())
					this->OnStop();
				RELEASE(collider);
			}
		}

		if (ImGui::BeginPopup("Collider Type"))
		{
			ImGui::Text("Collider Types");
			ImGui::Separator();
			for (int i = 0; i < IM_ARRAYSIZE(collider_names); i++)
				if (ImGui::Selectable(collider_names[i]))
					new_selected = i;
			ImGui::EndPopup();
		}

		if (new_selected != current_selected)
		{
			current_selected = new_selected;
			parent->LoadCollider(Collider::Type(current_selected));
		}

		if (collider != nullptr)
			collider->OnEditor();
	}

	return true;
}

void ComponentRigidBody::OnPlay()
{
	if (collider != nullptr)
	{
		rigid_body = App->physics->AddRigidBody(this, parent->transform->GetScale());
		collider->SetCollisionShape(App->physics->GetCollisionShape(rigid_body));
	}
	else
		APPLOG("Warning: Rigidbody on GameObject %s does not have a collider", parent->name);
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

void ComponentRigidBody::LoadCollider(Collider::Type collider_type, std::vector<ComponentMesh*>& meshes)
{
	LoadCollider(collider_type);
	collider->SetMeshes(meshes);
	if (collider_type != Collider::Type::MESH)
		collider->SetShapeOnMeshes();
}

void ComponentRigidBody::getWorldTransform(btTransform& worldTrans) const
{
	float4x4 object_global = parent->transform->GetGlobalTransformMatrix();
	float4x4 collider_global = object_global * collider->GetLocalTransform();
	float3 scale = collider_global.ExtractScale();
	scale = float3::one.Div(scale);
	collider_global = float4x4::FromTRS(collider_global.TranslatePart(), collider_global.RotatePart(), scale);

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
	static_assert(Collider::Type::UNKNOWN == 4, "Update collider factory code");

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
	case Collider::MESH:
		ret = new ColliderMesh(this);
		break;
	default:
		break;
	}

	return ret;
}
