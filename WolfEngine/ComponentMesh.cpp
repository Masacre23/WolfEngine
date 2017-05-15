#include "ComponentMesh.h"
#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include "OpenGL.h"
#include "GameObject.h"
#include "Color.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleLevel.h"
#include "ModuleProgramShaders.h"
#include "Primitive.h"
#include "Interface.h"

ComponentMesh::ComponentMesh(GameObject* parent) : Component(Component::Type::MESH, parent)
{
}

ComponentMesh::~ComponentMesh()
{
	RELEASE_ARRAY(vertices);
	RELEASE_ARRAY(normals);
	RELEASE_ARRAY(tex_coords);
	RELEASE_ARRAY(indices);
	RELEASE_ARRAY(buffer);

	if (has_bones)
	{
		for (int i = 0; i < num_bones; i++)
		{
			RELEASE_ARRAY(bones[i].weights);
		}
		RELEASE_ARRAY(bones);
	}

	glDeleteBuffers(1, (GLuint*) &(vertices_id));
	if (has_normals)
		glDeleteBuffers(1, (GLuint*) &(normals_id));
	if (has_tex_coords)
		glDeleteBuffers(1, (GLuint*) &(texture_id));
	glDeleteBuffers(1, (GLuint*) &(indices_id));
}

void ComponentMesh::Load(aiMesh* mesh, bool is_dynamic)
{
	if (is_dynamic)
		draw_mode = GL_DYNAMIC_DRAW;

	int float_dimension = 3;
	has_normals = mesh->HasNormals();
	if (has_normals)
		float_dimension += 3;
	has_tex_coords = mesh->HasTextureCoords(0);
	if (has_tex_coords)
		float_dimension += 2;

	num_vertices = mesh->mNumVertices;
	buffer = new float[float_dimension * num_vertices];
	vertices = new float3[num_vertices];
	unsigned c = 0;
	for (size_t i = 0; i < num_vertices; ++i)
		for (size_t j = 0; j < 3; ++j)
		{
			buffer[c++] = mesh->mVertices[i][j];
			vertices[i][j] = mesh->mVertices[i][j];
		}

	SetAABB();

	normals = new float3[num_vertices];
	if (has_normals)
	{
		for (size_t i = 0; i < num_vertices; ++i)
			for (size_t j = 0; j < 3; ++j)
			{
				buffer[c++] = mesh->mNormals[i][j];
				normals[i][j] = mesh->mNormals[i][j];
			}
	}

	tex_coords = new float2[num_vertices];
	if (has_tex_coords)
	{
		for (size_t i = 0; i < num_vertices; ++i)
			for (size_t j = 0; j < 2; ++j)
			{
				buffer[c++] = mesh->mTextureCoords[0][i][j];
				tex_coords[i][j] = mesh->mTextureCoords[0][i][j];
			}
	}

	glGenBuffers(1, (GLuint*) &(buffer_id));
	glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
	glBufferData(GL_ARRAY_BUFFER, float_dimension * sizeof(float) * num_vertices, buffer, draw_mode);

	num_indices = 3 * mesh->mNumFaces;
	indices = new unsigned[num_indices];

	c = 0;
	for (size_t j = 0; j < mesh->mNumFaces; ++j)
		for (size_t k = 0; k < 3; ++k)
			indices[c++] = mesh->mFaces[j].mIndices[k];
	if (c != 3 * mesh->mNumFaces)
		APPLOG("Error loading meshes: Incorrect number of indices");

	glGenBuffers(1, (GLuint*) &(indices_id));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * num_indices, indices, draw_mode);

	if (mesh->HasBones())
	{
		has_bones = true;
		num_bones = mesh->mNumBones;
		bones = new Bone[num_bones];
		for (int i = 0; i < mesh->mNumBones; i++)
		{
			aiBone* scene_bone = mesh->mBones[i];
			bones[i].name = scene_bone->mName;

			memcpy(bones[i].bind.v, &scene_bone->mOffsetMatrix.a1, 16 * sizeof(float));

			bones[i].num_weights = scene_bone->mNumWeights;
			bones[i].weights = new Weight[bones[i].num_weights];
			for (int j = 0; j < bones[i].num_weights; j++)
			{
				bones[i].weights[j].weight = scene_bone->mWeights[j].mWeight;
				bones[i].weights[j].vertex = scene_bone->mWeights[j].mVertexId;
			}
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ComponentMesh::Load(const Primitive& primitive)
{
	num_vertices = primitive.GetNumVertices();
	num_indices = primitive.GetNumIndices();

	int float_dimension = 3;
	has_normals = true;
	has_tex_coords = true;
	if (has_normals)
		float_dimension += 3;
	if (has_tex_coords)
		float_dimension += 2;
	buffer = new float[float_dimension * num_vertices];

	vertices = new float3[num_vertices];
	normals = new float3[num_vertices];
	tex_coords = new float2[num_vertices];
	indices = new unsigned[num_indices];

	primitive.LoadMesh(vertices, tex_coords, normals, indices);

	unsigned c = 0;
	for (size_t i = 0; i < num_vertices; ++i)
		for (size_t j = 0; j < 3; ++j)
		{
			buffer[c++] = vertices[i][j];
		}
	for (size_t i = 0; i < num_vertices; ++i)
		for (size_t j = 0; j < 3; ++j)
		{
			buffer[c++] = normals[i][j];
		}
	for (size_t i = 0; i < num_vertices; ++i)
		for (size_t j = 0; j < 2; ++j)
		{
			buffer[c++] = tex_coords[i][j];
		}

	SetAABB();

	glGenBuffers(1, (GLuint*) &(buffer_id));
	glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
	glBufferData(GL_ARRAY_BUFFER, float_dimension * sizeof(float) * num_vertices, buffer, draw_mode);

	glGenBuffers(1, (GLuint*) &(indices_id));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * num_indices, indices, draw_mode);

	has_bones = false;

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ComponentMesh::LoadBones()
{
	if (has_bones)
	{
		for (int i = 0; i < num_bones; i++)
		{
			GameObject* bone = parent->root->FindByName(bones[i].name.data);
			if (bone != nullptr)
			{
				bones[i].bone_object = bone;
				bones[i].bone_object->is_bone = true;
			}
		}
	}
}

bool ComponentMesh::OnUpdate()
{
	BROFILER_CATEGORY("ComponentMesh-OnUpdate", Profiler::Color::Aqua);

	float3* vertex_pointer;
	float3* normals_pointer;

	if (has_bones && parent->root->IsPlayingAnimation())
	{
		glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
		float* buffer_pointer = (float*) glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
		vertex_pointer = (float3*)buffer_pointer;
		memset(vertex_pointer, 0, num_vertices * sizeof(float3));
		if (has_normals)
		{
			normals_pointer = (float3*) &(buffer_pointer[3 * num_vertices]);
			memset(normals_pointer, 0, num_vertices * sizeof(float3));
		}
			
		float4x4 animation_transform = float4x4::identity;

		for (int i = 0; i < num_bones; i++)
		{
			animation_transform = bones[i].bone_object->GetGlobalTransformMatrix();
			animation_transform = bones[i].bone_object->root->GetLocalTransformMatrix().Inverted() * animation_transform;
			animation_transform = animation_transform * bones[i].bind;

			float3x3 rotation = animation_transform.Float3x3Part();

			for (int j = 0; j < bones[i].num_weights; j++)
			{
				vertex_pointer[bones[i].weights[j].vertex] += animation_transform.TransformPos(vertices[bones[i].weights[j].vertex]) * bones[i].weights[j].weight;

				if (has_normals)
					normals_pointer[bones[i].weights[j].vertex] += rotation * normals[bones[i].weights[j].vertex] * bones[i].weights[j].weight;
			}
		}

		glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
		glUnmapBuffer(GL_ARRAY_BUFFER);

	}

	return true;
}

bool ComponentMesh::OnDraw() const
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
	float* buffer_pointer = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
	
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	int offset = 3;

	if (use_normals)
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnable(GL_LIGHTING);
		glNormalPointer(GL_FLOAT, 0, (char*) (offset * num_vertices * sizeof(float)));
		offset += 3;
	}
	else
		glDisable(GL_LIGHTING);

	if (has_tex_coords)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, 0, (char*)(offset * num_vertices * sizeof(float)));
		offset += 2;
	}

	glUnmapBuffer(GL_ARRAY_BUFFER);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_id);
	App->program_shaders->UseProgram("Prueba");
	glUniform4f(App->program_shaders->GetUniformLocation("Prueba", "light_position"), 0, 2, -2, 0);
	glUniform1i(App->program_shaders->GetUniformLocation("Prueba", "tex_coord"), 0);
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, NULL);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	App->program_shaders->UnuseProgram();

	return true;
}

bool ComponentMesh::OnDebugDraw() const
{
	if (draw_normals)
		DrawNormals();

	if (draw_mesh)
		DrawMesh();

	return true;
}

bool ComponentMesh::OnEditor()
{
	if (on_editor)
	{
		if (ImGui::CollapsingHeader("Mesh"))
		{
			ImGui::Checkbox("Active", &enable);

			ImGui::SameLine();

			if (ImGui::Button("Delete"))
				parent->DeleteComponent(this);

			ImGui::Checkbox("Draw normals", &draw_normals);

			ImGui::Checkbox("Draw mesh", &draw_mesh);
		}

		return ImGui::IsItemClicked();
	}

	return false;
}

void ComponentMesh::SaveComponent()
{
}

void ComponentMesh::RestoreComponent()
{
	ResetMesh();

	enable = true;
	on_editor = true;
	draw_normals = false;
	draw_mesh = false;
}

void ComponentMesh::ResetMesh()
{
	if (has_bones)
	{
		float3* vertex_pointer;
		float3* normals_pointer;

		glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
		float* buffer_pointer = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
		vertex_pointer = (float3*)buffer_pointer;
		memcpy(vertex_pointer, vertices, num_vertices * sizeof(float3));
		if (has_normals)
		{
			normals_pointer = (float3*) &(buffer_pointer[3 * num_vertices]);
			memcpy(normals_pointer, normals, num_vertices * sizeof(float3));
		}
	}
}

void ComponentMesh::DrawNormals() const
{
	App->renderer->debug_drawer->SetColor(Colors::Yellow);

	for (int i = 0; i < num_vertices; i++)
	{
		glBegin(GL_LINES);
		glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
		glVertex3f(vertices[i].x + normals[i].x, vertices[i].y + normals[i].y, vertices[i].z + normals[i].z);
		glEnd();
	}

	App->renderer->debug_drawer->SetColor(Colors::Black);
}

void ComponentMesh::DrawMesh() const
{
	App->renderer->debug_drawer->SetColor(Colors::Fuchsia);

	unsigned num_triangles = num_indices / 3;
	for (int i = 0; i < num_triangles; i++)
	{
		unsigned first = indices[3 *i];
		unsigned second = indices[3 * i + 1];
		unsigned third = indices[3 * i + 2];

		glBegin(GL_LINES);
		glVertex3f(vertices[first].x, vertices[first].y, vertices[first].z);
		glVertex3f(vertices[second].x, vertices[second].y, vertices[second].z);

		glVertex3f(vertices[second].x, vertices[second].y, vertices[second].z);
		glVertex3f(vertices[third].x, vertices[third].y, vertices[third].z);

		glVertex3f(vertices[third].x, vertices[third].y, vertices[third].z);
		glVertex3f(vertices[first].x, vertices[first].y, vertices[first].z);
		glEnd();
	}

	App->renderer->debug_drawer->SetColor(Colors::Black);
}

void ComponentMesh::SetAABB() const
{
	//Creating BoundingBox from vertices points
	parent->initial_bbox.SetNegativeInfinity();
	parent->initial_bbox.Enclose((float3*)vertices, num_vertices);
	parent->bbox = parent->initial_bbox;
	App->level->InsertGameObjectQuadTree(parent);
}
