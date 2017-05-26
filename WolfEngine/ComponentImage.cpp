#include "ComponentImage.h"
#include "Interface.h"
#include "OpenGL.h"
#include "FreeType.h"
#include "Application.h"
#include "ModuleTextures.h"

ComponentImage::ComponentImage(GameObject* parent) : Component(Component::Type::IMAGE, parent)
{
	texture = App->textures->LoadTexture(aiString(path));
}

ComponentImage::~ComponentImage()
{
}

void ComponentImage::OnDraw() const
{
	glPushAttrib(GL_TRANSFORM_BIT);
	GLint   viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(viewport[0], viewport[2], viewport[1], viewport[3]);
	glPopAttrib();

	glPushAttrib(GL_LIST_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT | GL_TRANSFORM_BIT);
	glMatrixMode(GL_MODELVIEW);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	float modelview_matrix[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix);

	//clear and draw quad with texture
	//glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();
	glLoadIdentity();
	glBindTexture(GL_TEXTURE_2D, App->textures->texture_debug);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glTexCoord2i(0, 0);
	glVertex2i(0, 0);
	glTexCoord2i(1, 0);
	glVertex2i(100, 0);
	glTexCoord2i(1, 1);
	glVertex2i(100, 100);
	glTexCoord2i(0, 1);
	glVertex2i(0, 100);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFlush();

	glPopAttrib();

	glPushAttrib(GL_TRANSFORM_BIT);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopAttrib();
}

bool ComponentImage::OnEditor()
{
	bool node_open = ImGui::CollapsingHeader("Image");

	if (node_open)
	{
	}

	return ImGui::IsItemClicked();
}