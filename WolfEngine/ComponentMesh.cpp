#include "ComponentMesh.h"
#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include "OpenGL.h"
#include "GameObject.h"
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_sdl_gl3.h"

ComponentMesh::ComponentMesh(GameObject* parent) : Component(Component::Type::MESH, parent)
{
}

ComponentMesh::~ComponentMesh()
{
	RELEASE_ARRAY(vertices);
	RELEASE_ARRAY(vertices_bind);
	RELEASE_ARRAY(normals);
	RELEASE_ARRAY(tex_coords);
	RELEASE_ARRAY(indices);

	if (has_bones)
	{
		for (std::vector<Bone*>::iterator it = bones.begin(); it != bones.end(); ++it)
		{
			RELEASE_ARRAY((*it)->weights);
			RELEASE(*it);
		}
		bones.clear();
	}

	glDeleteBuffers(1, (GLuint*) &(vertices_id));
	if (has_normals)
		glDeleteBuffers(1, (GLuint*) &(normals_id));
	if (has_tex_coords)
		glDeleteBuffers(1, (GLuint*) &(texture_id));
	glDeleteBuffers(1, (GLuint*) &(indices_id));
}

void ComponentMesh::Load(aiMesh* mesh)
{
	num_vertices = mesh->mNumVertices;
	vertices = new float[3 * num_vertices];
	vertices_bind = new float[3 * num_vertices];
	unsigned c = 0;
	for (size_t i = 0; i < num_vertices; ++i)
		for (size_t j = 0; j < 3; ++j)
			vertices[c++] = mesh->mVertices[i][j];
	if (c != 3 * mesh->mNumVertices)
		LOG("Error loading meshes: Incorrect number of vertices");

	memcpy(vertices_bind, vertices, 3 * num_vertices * sizeof(float));

	glGenBuffers(1, (GLuint*) &(vertices_id));
	glBindBuffer(GL_ARRAY_BUFFER, vertices_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * num_vertices, vertices, GL_STATIC_DRAW);

	//Creating BoundingBox from vertices points
	parent->bbox.SetNegativeInfinity();
	parent->bbox.Enclose((float3*)vertices, num_vertices);

	has_normals = mesh->HasNormals();
	if (has_normals) 
	{
		normals = new float[3 * num_vertices];
		normals_bind = new float[3 * num_vertices];
		c = 0;
		for (size_t i = 0; i < num_vertices; ++i)
			for (size_t j = 0; j < 3; ++j)
				normals[c++] = mesh->mNormals[i][j];
		if (c != 3 * mesh->mNumVertices)
			LOG("Error loading meshes: Incorrect number of normals");

		memcpy(normals_bind, normals, 3 * num_vertices * sizeof(float));

		glGenBuffers(1, (GLuint*) &(normals_id));
		glBindBuffer(GL_ARRAY_BUFFER, normals_id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * num_vertices, normals, GL_STATIC_DRAW);
	}

	has_tex_coords = mesh->HasTextureCoords(0);
	if (has_tex_coords) 
	{
		tex_coords = new float[2 * num_vertices];
		c = 0;
		for (size_t i = 0; i < num_vertices; ++i)
			for (size_t j = 0; j < 2; ++j)
				tex_coords[c++] = mesh->mTextureCoords[0][i][j];
		if (c != 2 * mesh->mNumVertices)
			LOG("Error loading meshes: Incorrect number of texture coordinates");

		glGenBuffers(1, (GLuint*) &(texture_id));
		glBindBuffer(GL_ARRAY_BUFFER, texture_id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * num_vertices, tex_coords, GL_STATIC_DRAW);
	}

	num_indices = 3 * mesh->mNumFaces;
	indices = new unsigned[num_indices];

	c = 0;
	for (size_t j = 0; j < mesh->mNumFaces; ++j)
		for (size_t k = 0; k < 3; ++k)
			indices[c++] = mesh->mFaces[j].mIndices[k];
	if (c != 3 * mesh->mNumFaces)
		LOG("Error loading meshes: Incorrect number of indices");

	glGenBuffers(1, (GLuint*) &(indices_id));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * num_indices, indices, GL_STATIC_DRAW);

	if (mesh->HasBones())
	{
		has_bones = true;
		for (int i = 0; i < mesh->mNumBones; i++)
		{
			aiBone* scene_bone = mesh->mBones[i];
			Bone* bone = new Bone;
			bone->name = scene_bone->mName;

			memcpy(bone->bind.v, &scene_bone->mOffsetMatrix.a1, 16 * sizeof(float));

			bone->num_weights = scene_bone->mNumWeights;
			bone->weights = new Weight[bone->num_weights];
			for (int j = 0; j < bone->num_weights; j++)
			{
				bone->weights[j].weight = scene_bone->mWeights[j].mWeight;
				bone->weights[j].vertex = scene_bone->mWeights[j].mVertexId;
			}
			bones.push_back(bone);
		}
	}
}

void ComponentMesh::LoadBones()
{
	if (has_bones)
	{
		for (std::vector<Bone*>::iterator it = bones.begin(); it != bones.end(); ++it)
		{
			(*it)->bone_object = parent->root->FindByName((*it)->name.data);
		}
	}
}

bool ComponentMesh::OnUpdate()
{
	if (has_bones)
	{
		memset(vertices, 0, 3 * num_vertices * sizeof(float));
		if (has_normals)
			memset(normals, 0, 3 * num_vertices * sizeof(float));

		for (std::vector<Bone*>::iterator it = bones.begin(); it != bones.end(); ++it)
		{
			float4x4 animation_transform = float4x4::identity;
			(*it)->bone_object->RecursiveGetBoneGlobalTransformMatrix(animation_transform);
			animation_transform = animation_transform *(*it)->bind;

			for (int j = 0; j < (*it)->num_weights; j++)
			{
				unsigned index = (*it)->weights[j].vertex;

				float3 vertex_end = animation_transform.TransformPos(float3(&vertices_bind[3 * index])) * (*it)->weights[j].weight;

				vertices[3 * index] += vertex_end.x;
				vertices[3 * index + 1] += vertex_end.y;
				vertices[3 * index + 2] += vertex_end.z;

				if (has_normals)
				{
					float3 normals_end = animation_transform.TransformPos(float3(&normals_bind[3 * index])) * (*it)->weights[j].weight;

					normals[3 * index] += normals_end.x;
					normals[3 * index + 1] += normals_end.y;
					normals[3 * index + 2] += normals_end.z;
				}
			}
		}
	}

	return true;
}

bool ComponentMesh::OnDraw() const
{
	if (has_bones)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vertices_id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * num_vertices, vertices, GL_STATIC_DRAW);
		if (has_normals)
		{
			glBindBuffer(GL_ARRAY_BUFFER, normals_id);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_vertices, normals, GL_STATIC_DRAW);
		}
	}
		
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, vertices_id);	
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	if (has_normals)
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnable(GL_LIGHTING);
		glBindBuffer(GL_ARRAY_BUFFER, normals_id);
		glNormalPointer(GL_FLOAT, 0, NULL);
	}
	else
		glDisable(GL_LIGHTING);
		
	if (has_tex_coords)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, texture_id);
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_id);
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, NULL);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	return true;
}

bool ComponentMesh::OnEditor()
{
	if (ImGui::CollapsingHeader("Mesh"))
	{
		ImGui::Checkbox("Active", &enable);

		ImGui::SameLine();

		if(ImGui::Button("Delete"))
			this->~ComponentMesh();

		ImGui::Text(folder_path.C_Str());
	}

	return ImGui::IsItemClicked();
}
