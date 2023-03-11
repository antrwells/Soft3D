#include "QuickDraw.h"
#include "ShaderEffect.h"
#include <glad/gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "mathhelper.h"
#include "Texture2D.h"

#include "SoftApp.h"

QuickDraw::QuickDraw() {

	vao = vbo = ebo = 0;
	mDrawBasic = new ShaderEffect("data/shader/draw.vs", "data/shader/draw.fs");
	SetViewport(Rect(0, 0, SoftApp::m_This->getWinWidth(), SoftApp::m_This->getWinHeight()));

}

void QuickDraw::DrawRect(Texture2D* tex,Rect rect, Color col) {

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glm::mat4 proj = glm::ortho(0.0f, mViewPort.width, mViewPort.height, 0.0f, 0.0f, 100.0f);

	mDrawBasic->Bind();

	mDrawBasic->SetMatrix("projection", proj);
	mDrawBasic->SetInt("tex", 0);
	mDrawBasic->SetVec4("col", glm::vec4(1, 1, 1, 1));

	tex->Bind(0);

	//std::cout << "Rendering\n";

	float* dat = new float[20];

	//lt
	dat[0] = rect.x;
	dat[1] = rect.y;
	dat[2] = 0.0f;
	dat[3] = 0.0f;
	dat[4] = 0.0f;
	//rt
	dat[5] = rect.x + rect.width;
	dat[6] = rect.y;
	dat[7] = 0.0f;
	dat[8] = 1.0f;
	dat[9] = 0.0f;

	//rb
	dat[10] = rect.x + rect.width;
	dat[11] = rect.y + rect.height;
	dat[12] = 0.0f;
	dat[13] = 1.0f;
	dat[14] = 1.0f;

	//lb
	dat[15] = rect.x;
	dat[16] = rect.y + rect.height;
	dat[17] = 0.0f;
	dat[18] = 0.0f;
	dat[19] = 1.0f;

	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 20 * sizeof(float), &dat[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, nullptr);
	glEnableVertexAttribArray(0);
	//int v = 3 * sizeof(float);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	GLuint* indices = new GLuint[6];

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 2;
	indices[4] = 3;
	indices[5] = 0;

	GLuint ebo = 0;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 6, &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);


}

DrawList* QuickDraw::FindList(Texture2D* texture) {

	for (int i = 0;i < mLists.size();i++) {

		auto l = mLists[i];
		if (l->mImage == texture) {
			return l;
		}

	}

	auto nl = new DrawList;
	nl->mImage = texture;
	mLists.push_back(nl);
	return nl;


}

Draw* QuickDraw::DrawSprite(Texture2D* tex, float x, float y, float w, float h,Color col, Texture2D* aux, float rot, float scale)
{
	auto list = FindList(tex);

	list->mAux = aux;

	Draw* draw = new Draw;


	glm::vec2 p1, p2, p3, p4;

	p1.x = - w / 2;
	p1.y = -h / 2;
	p2.x = w / 2;
	p2.y = -h / 2;
	p3.x = w / 2;
	p3.y = h/ 2;
	p4.x = -w/ 2;
	p4.y = h / 2;

	p1 = rotate(p1, rot) * scale;
	p2 = rotate(p2, rot) * scale;
	p3 = rotate(p3, rot) * scale;
	p4 = rotate(p4, rot) * scale;

	p1.x = (x) + p1.x;
	p1.y = (y) + p1.y;
	p2.x = (x) + p2.x;
	p2.y = (y) + p2.y;
	p3.x = (x) + p3.x;
	p3.y = (y) + p3.y;
	p4.x = (x) + p4.x;
	p4.y = (y) + p4.y;

	//std::cout << "Rendering\n";
	draw->x[0] = p1.x;
	draw->y[0] = p1.y;

	draw->x[1] = p2.x;
	draw->y[1] = p2.y;

	draw->x[2] = p3.x;
	draw->y[2] = p3.y;

	draw->x[3] = p4.x;
	draw->y[3] = p4.y;

	draw->z = mZ;
	mZ = mZ - 0.001f;
	draw->r = col.r;
	draw->g = col.g;
	draw->b = col.b;
	draw->a = col.a;
	draw->flipH = false;

	list->mDraws.push_back(draw);
	return draw;
	return nullptr;
}

Draw* QuickDraw::DrawRectRot(Texture2D* tex, Rect rect, Color col,Texture2D* aux, float rot, float scale,bool fliph) {


	auto list = FindList(tex);

	list->mAux = aux;

	Draw* draw = new Draw;
	

	glm::vec2 p1, p2, p3, p4;

	p1.x = -rect.width / 2;
	p1.y = -rect.height / 2;
	p2.x = rect.width / 2;
	p2.y = -rect.height / 2;
	p3.x = rect.width / 2;
	p3.y = rect.height / 2;
	p4.x = -rect.width / 2;
	p4.y = rect.height / 2;

	p1 = rotate(p1, rot) * scale;
	p2 = rotate(p2, rot) * scale;
	p3 = rotate(p3, rot) * scale;
	p4 = rotate(p4, rot) * scale;

	p1.x = (rect.x+rect.width/2 ) + p1.x;
	p1.y = (rect.y+rect.height/2 ) + p1.y;
	p2.x = (rect.x+rect.width/2 ) + p2.x;
	p2.y = (rect.y+rect.height/2 )+p2.y;
	p3.x = (rect.x+rect.width/2 ) + p3.x;
	p3.y = (rect.y+rect.height/2 ) + p3.y;
	p4.x = (rect.x+rect.width/2 ) + p4.x;
	p4.y = (rect.y+rect.height/2 ) + p4.y;

	//std::cout << "Rendering\n";
	draw->x[0] = p1.x;
	draw->y[0] = p1.y;

	draw->x[1] = p2.x;
	draw->y[1] = p2.y;

	draw->x[2] = p3.x;
	draw->y[2] = p3.y;

	draw->x[3] = p4.x;
	draw->y[3] = p4.y;

	draw->z = mZ;
	mZ = mZ - 0.001f;
	draw->r = col.r;
	draw->g = col.g;
	draw->b = col.b;
	draw->a = col.a;
	draw->flipH = fliph;

	list->mDraws.push_back(draw);
	return draw;

	//
	
	/*
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glm::mat4 proj = glm::ortho(0.0f, mViewPort.width, mViewPort.height, 0.0f, 0.0f, 100.0f);

	mDrawBasic->Bind();

	mDrawBasic->SetMatrix("projection", proj);
	mDrawBasic->SetInt("tex", 0);
	mDrawBasic->SetVec4("col", glm::vec4(1, 1, 1, 1));

	tex->Bind(0);

	glm::vec2 p1, p2, p3, p4;

	p1.x = - rect.width / 2;
	p1.y = - rect.height / 2;
	p2.x = rect.width / 2;
	p2.y = -rect.height / 2;
	p3.x = rect.width / 2;
	p3.y = rect.height / 2;
	p4.x = -rect.width / 2;
	p4.y = rect.height / 2;

	p1 = rotate(p1, rot) * scale;
	p2 = rotate(p2, rot) * scale;
	p3 = rotate(p3, rot) * scale;
	p4 = rotate(p4, rot) * scale;

	p1.x = (rect.x + rect.width/2) + p1.x;
	p1.y = (rect.y + rect.height/2) + p1.y;
	p2.x = (rect.x + rect.width/2) + p2.x;
	p2.y = (rect.y + rect.height/2)+p2.y;
	p3.x = (rect.x +rect.width/2) + p3.x;
	p3.y = (rect.y +rect.height/2)+ p3.y;
	p4.x = (rect.x +rect.width/2)+ p4.x;
	p4.y = (rect.y +rect.height/2)+ p4.y;

	//std::cout << "Rendering\n";

	float* dat = new float[20];

	//lt
	dat[0] = p1.x;
	dat[1] = p1.y;
	dat[2] = 0.0f;
	dat[3] = 0.0f;
	dat[4] = 0.0f;
	//rt
	dat[5] = p2.x;
	dat[6] = p2.y;
	dat[7] = 0.0f;
	dat[8] = 1.0f;
	dat[9] = 0.0f;

	//rb
	dat[10] = p3.x;
	dat[11] = p3.y;
	dat[12] = 0.0f;
	dat[13] = 1.0f;
	dat[14] = 1.0f;

	//lb
	dat[15] = p4.x;
	dat[16] = p4.y;//rect.y + rect.height;
	dat[17] = 0.0f;
	dat[18] = 0.0f;
	dat[19] = 1.0f;

	//GLuint vao = 0;
	if (vao == 0) {
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
	}

	if (vbo == 0) {
		glGenBuffers(1, &vbo);
	}
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	
	glBufferData(GL_ARRAY_BUFFER, 20 * sizeof(float), &dat[0], GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, nullptr);
	glEnableVertexAttribArray(0);
	//int v = 3 * sizeof(float);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	if (ebo == 0) {
		GLuint* indices = new GLuint[6];

		indices[0] = 0;
		indices[1] = 1;
		indices[2] = 2;
		indices[3] = 2;
		indices[4] = 3;
		indices[5] = 0;

	//	GLuint ebo = 0;
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 6, &indices[0], GL_STATIC_DRAW);

	}
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	*/
}

void QuickDraw::Begin() {

	mZ = 0.9f;
	mLists.clear();

}

void QuickDraw::End(ShaderEffect* shader) {
	glm::mat4 proj = glm::ortho(0.0f, mViewPort.width, mViewPort.height, 0.0f, 0.01f, -10.0f);
	glViewport(0, 0, mViewPort.width, mViewPort.height);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	shader->Bind();
	shader->SetMatrix("projection", proj);
	shader->SetInt("tex", 0);
	shader->SetInt("norm", 1);
	shader->SetVec4("col", glm::vec4(1, 1, 1, 1));
	//shader->Bind();
	//shader->Release();
	

	shader->SetMatrix("projection", proj);
	shader->SetInt("tex", 0);
	shader->SetInt("norm", 1);
	shader->SetVec4("col", glm::vec4(1, 1, 1, 1));
	for (int i = 0;i < mLists.size();i++) {

		auto list = mLists[i];

		int num = list->mDraws.size();

		float* val = new float[num * 11 * 4];

		int vn = 0;

		for (int j = 0;j < num;j++) {

			auto d = list->mDraws[j];

			val[vn++] = d->x[0];
			val[vn++] = d->y[0];
			val[vn++] = d->z;
			if (d->flipH) {
				val[vn++] = 1.0f;
				val[vn++] = 0.0f;
			}else{
				val[vn++] = 0.0f;
				val[vn++] = 0.0f;
			}
			val[vn++] = d->r;
			val[vn++] = d->g;
			val[vn++] = d->b;
			val[vn++] = d->a;
			val[vn++] = d->rx[0];
			val[vn++] = d->ry[0];

			//
			val[vn++] = d->x[1];
			val[vn++] = d->y[1];
			val[vn++] = d->z;
			if (d->flipH) {
				val[vn++] = 0.0f;
				val[vn++] = 0.0f;
			}
			else {
				val[vn++] = 1.0f;
				val[vn++] = 0.0f;
			}
			val[vn++] = d->r;
			val[vn++] = d->g;
			val[vn++] = d->b;
			val[vn++] = d->a;
			val[vn++] = d->rx[1];
			val[vn++] = d->ry[1];

			val[vn++] = d->x[2];
			val[vn++] = d->y[2];
			val[vn++] = d->z;
		//	val[vn++] = 1.0f;
			if (d->flipH) {
				val[vn++] = 0.0f;
				val[vn++] = 1.0f;
			}
			else {
				val[vn++] = 1.0f;
				val[vn++] = 1.0f;
			}
			val[vn++] = d->r;
			val[vn++] = d->g;
			val[vn++] = d->b;
			val[vn++] = d->a;
			val[vn++] = d->rx[2];
			val[vn++] = d->ry[2];

			val[vn++] = d->x[3];
			val[vn++] = d->y[3];
			val[vn++] = d->z;
			if (d->flipH) {
				val[vn++] = 1.0f;
				val[vn++] = 1.0f;
			}
			else {
				val[vn++] = 0.0f;
				val[vn++] = 1.0f;
			}
			val[vn++] = d->r;
			val[vn++] = d->g;
			val[vn++] = d->b;
			val[vn++] = d->a;
			val[vn++] = d->rx[3];
			val[vn++] = d->ry[3];

		}
		//render

		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//glDisable(GL_DEPTH_TEST);
		//glDisable(GL_CULL_FACE);


	

		
		list->mImage->Bind(0);
		if (list->mAux != nullptr) {
			list->mAux->Bind(1);
		}

		//if (vao == 0) {
			glGenVertexArrays(1, &vao);
			
		//}
		glBindVertexArray(vao);
		//if (vbo == 0) {
			glGenBuffers(1, &vbo);
		//}
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glBufferData(GL_ARRAY_BUFFER, (num * 11 * 4) * sizeof(float), &val[0], GL_STATIC_DRAW);


		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 11, nullptr);
		glEnableVertexAttribArray(0);
		//int v = 3 * sizeof(float);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 11, (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 11, (void*)(5 * sizeof(float)));
		glEnableVertexAttribArray(2);

		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 11, (void*)(9 * sizeof(float)));
		glEnableVertexAttribArray(3);

		GLuint* indices = new GLuint[6 * num];

		int ic = 0;
		int vc = 0;
		for (int i = 0;i < num;i++) {

			indices[ic++] = vc + 0;
			indices[ic++] = vc + 1;
			indices[ic++] = vc + 2;
			indices[ic++] = vc + 2;
			indices[ic++] = vc + 3;
			indices[ic++] = vc + 0;
			vc += 4;

		}

		//	GLuint ebo = 0;
		//if (ebo == 0) {
			glGenBuffers(1, &ebo);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * (6 * num), &indices[0], GL_STATIC_DRAW);


		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

		glDrawElements(GL_TRIANGLES, 6 * num, GL_UNSIGNED_INT, nullptr);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ebo);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(3);

		list->mImage->Release(0);
		if (list->mAux != nullptr) {
			list->mAux->Release(1);
		}

	}
//	shader->Release();

}

void QuickDraw::End() {
	glm::mat4 proj = glm::ortho(0.0f, mViewPort.width, mViewPort.height, 0.0f, 0.1f, -10.0f);
	glViewport(0, 0, mViewPort.width, mViewPort.height);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	mDrawBasic->Bind();
	//return;
	mDrawBasic->SetMatrix("projection", proj);
	mDrawBasic->SetInt("tex", 0);
	mDrawBasic->SetVec4("col", glm::vec4(1, 1, 1, 1));
	for (int i = 0;i < mLists.size();i++) {

		auto list = mLists[i];

		int num = list->mDraws.size();

		float* val = new float[num * 9*4];

		int vn = 0;

		for (int j = 0;j < num;j++) {

			auto d = list->mDraws[j];

			val[vn++] = d->x[0];
			val[vn++] = d->y[0];
			val[vn++] = d->z;
			if (d->flipH) {
				val[vn++] = 1.0f;
				val[vn++] = 0.0f;
			}
			else {
				val[vn++] = 0.0f;
				val[vn++] = 0.0f;
			}
			val[vn++] = d->r;
			val[vn++] = d->g;
			val[vn++] = d->b;
			val[vn++] = d->a;

			//
			val[vn++] = d->x[1];
			val[vn++] = d->y[1];
			val[vn++] = d->z;
			if (d->flipH) {
				val[vn++] = 0.0f;
				val[vn++] = 0.0f;
			}
			else {
				val[vn++] = 1.0f;
				val[vn++] = 0.0f;
			}
			val[vn++] = d->r;
			val[vn++] = d->g;
			val[vn++] = d->b;
			val[vn++] = d->a;

			val[vn++] = d->x[2];
			val[vn++] = d->y[2];
			val[vn++] = d->z;
			if (d->flipH) {
				val[vn++] = 0.0f;
				val[vn++] = 1.0f;
			}
			else {
				val[vn++] = 1.0f;
				val[vn++] = 1.0f;
			}
			val[vn++] = d->r;
			val[vn++] = d->g;
			val[vn++] = d->b;
			val[vn++] = d->a;

			val[vn++] = d->x[3];
			val[vn++] = d->y[3];
			val[vn++] = d->z;
			if (d->flipH) {
				val[vn++] = 1.0f;
				val[vn++] = 1.0f;
			}
			else {
				val[vn++] = 0.0f;
				val[vn++] = 1.0f;
			}
			val[vn++] = d->r;
			val[vn++] = d->g;
			val[vn++] = d->b;
			val[vn++] = d->a;

		}
		//render
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//return;



		list->mImage->Bind(0);
		if(list->mAux!=nullptr){
		list->mAux->Bind(1);
		}


		//if (vao == 0) {
			glGenVertexArrays(1, &vao);
			
		//}
		glBindVertexArray(vao);
		//if (vbo == 0) {
			glGenBuffers(1, &vbo);
		//}
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glBufferData(GL_ARRAY_BUFFER, (num*9*4) * sizeof(float), &val[0], GL_STATIC_DRAW);


		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, nullptr);
		glEnableVertexAttribArray(0);
		//int v = 3 * sizeof(float);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)(5 * sizeof(float)));
		glEnableVertexAttribArray(2);

	
			GLuint* indices = new GLuint[6*num];

			int ic = 0;
			int vc = 0;
			for (int i = 0;i < num;i++) {
			
				indices[ic++] = vc+0;
				indices[ic++] = vc+1;
				indices[ic++] = vc+2;
				indices[ic++] = vc+2;
				indices[ic++] = vc+3;
				indices[ic++] = vc+0;
				vc += 4;
			
			}

			//	GLuint ebo = 0;
			glGenBuffers(1, &ebo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * (6*num), &indices[0], GL_STATIC_DRAW);

		
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

		glDrawElements(GL_TRIANGLES, 6*num, GL_UNSIGNED_INT, nullptr);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ebo);


		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		if (list->mAux != nullptr) {
			list->mAux->Release(1);
		}

	}

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	mDrawBasic->Release();

}

void QuickDraw::DrawTex(int x, int y, int w, int h, Texture2D* tex, float r, float g, float b, float a) {

	DrawRectRot(tex, Rect(x, y, w, h), Color(r, g, b, a), nullptr, 0.0f, 1.0f);

}