#include "ModuleLevel.h"
#include "OpenGL.h"

void ModuleLevel::DrawNode(Node * node)
{
	if (node != nullptr) {
		glPushMatrix();
		glTranslatef(node->position.x, node->position.y, node->position.z);
		glMultMatrixf(node->rotation.GetMatrix()[0]);

		for (int i = 0; i < node->meshes.size(); ++i)
		{
			Mesh mesh = meshes[node->meshes[i]];
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_NORMAL_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			glVertexPointer(3, GL_FLOAT, 0, mesh.vertices);
			glNormalPointer(GL_FLOAT, 0, mesh.normals);

			glBindTexture(GL_TEXTURE_2D, materials[mesh.material].texture);
			glTexCoordPointer(2, GL_FLOAT, sizeof(aiVector3D), mesh.tex_coords);

			glMaterialfv(GL_FRONT, GL_AMBIENT, &materials[mesh.material].ambient[0]);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, &materials[mesh.material].diffuse[0]);
			glMaterialfv(GL_FRONT, GL_SPECULAR, &materials[mesh.material].specular[0]);
			glMaterialf(GL_FRONT, GL_SHININESS, materials[mesh.material].shiness);

			glDrawElements(GL_TRIANGLES, mesh.num_indices, GL_UNSIGNED_INT, mesh.indices);

			glBindTexture(GL_TEXTURE_2D, 0);

			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			glDisableClientState(GL_NORMAL_ARRAY);
			glDisableClientState(GL_VERTEX_ARRAY);
		}

		for (int i = 0; i < node->childs.size(); ++i)
		{
			DrawNode(node->childs[i]);
		}

		glPopMatrix();
	}
}

Node * ModuleLevel::FindNode(Node * node, const char * name)
{
	Node* ret = nullptr;
	if (node != nullptr) 
	{
		bool founded = false;
		for (int i = 0; i < node->childs.size() && ret != nullptr; ++i)
		{
			if (node->childs[i]->name.compare(name) == 0) 
			{
				ret = node->childs[i];
			}
			else {
				ret = FindNode(node->childs[i], name);
			}
		}
	}
	return ret;
}

ModuleLevel::ModuleLevel() : Module(MODULE_LEVEL)
{
}

ModuleLevel::~ModuleLevel()
{
}

void ModuleLevel::Load(const char * folder, const char * file)
{
}

void ModuleLevel::Clear()
{
}

void ModuleLevel::Draw()
{
	DrawNode(root);
}

Node * ModuleLevel::FindNode(const char * name)
{
	Node* ret = FindNode(root, name);
	return ret;
}

void ModuleLevel::LinkNode(Node * node, Node * destination)
{
	node->childs.push_back(destination);
	destination->parent = node;
}

