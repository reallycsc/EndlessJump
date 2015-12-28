#pragma once
#include "CommonDefinition.h"

NS_CSC_BEGIN
// string functions
inline string eraseStringNonNumber(const string &str)
{
	string tmpStr = str;
	int index = tmpStr.size() - 1;
	while (index != -1)
	{
		if (tmpStr[index] < '0' || tmpStr[index] > '9')
		{
			tmpStr.erase(index, 1);
			index--;
		}
		else
		{
			break;
		}
	}
	return tmpStr;
}

inline string& replace_all_distinct(string& str, string& old_value, string& new_value)
{
	for (string::size_type pos(0); pos != string::npos; pos += new_value.length()) {
		if ((pos = str.find(old_value, pos)) != string::npos)
			str.replace(pos, old_value.length(), new_value);
		else   break;
	}
	return   str;
}

// cocos2d functions
inline void setScrollViewTextAutoWrap(ScrollView* scrollView, Text* text)
{
	text->ignoreContentAdaptWithSize(false);
	Size textSize = text->getContentSize();
	int textWidthNew = scrollView->getInnerContainerSize().width;
	text->setTextAreaSize(Size(textWidthNew, textSize.height * (static_cast<int>(textSize.width) / static_cast<int>(textWidthNew) + 1)));
	scrollView->setInnerContainerSize(text->getVirtualRendererSize());
	text->setAnchorPoint(Point(0, 1));
	text->setPosition(Point(0, scrollView->getInnerContainerSize().height));
}

// percent: 0 - gray, 1 - color
inline void spriteToGray(Node* pNode, float percent)
{
	ImageView* imgView = dynamic_cast<ImageView*>(pNode);
	Sprite* imgView2 = dynamic_cast<Sprite*>(pNode);
	Sprite* sp = nullptr;
	if (imgView) {
		Scale9Sprite* scale9sp = dynamic_cast<Scale9Sprite*>(imgView->getVirtualRenderer());
		if (scale9sp) {
			sp = scale9sp->getSprite();
		}
	}
	else if (imgView2) {
		sp = imgView2;
	}
	if (!sp) {
		return;
	}

	const char* programStr =
		"#ifdef GL_ES \n\
			precision mediump float; \n\
		#endif \n\
		varying vec2 v_texCoord; \n\
		varying vec4 v_fragmentColor; \n\
		void main(void) \n\
		{ \n\
			// Convert to greyscale using NTSC weightings \n\
			vec4 col = v_fragmentColor * texture2D(CC_Texture0, v_texCoord); \n\
			float gray = dot(col.rgb, vec3(0.299, 0.587, 0.114)); \n\
			float r = (col.r-gray)*%f + gray; \n\
			float g = (col.g-gray)*%f + gray; \n\
			float b = (col.b-gray)*%f + gray; \n\
			gl_FragColor = vec4(r, g, b, col.a); \n\
		}";
	if (percent < 0)
		percent = 0;
	if (percent > 1)
		percent = 1;
	string buf = StringUtils::format(programStr, percent, percent, percent);
	const GLchar* pszFragSource = buf.c_str();

	GLProgram* pProgram = new GLProgram();
	pProgram->initWithByteArrays(ccPositionTextureColor_noMVP_vert, pszFragSource);
	sp->setGLProgram(pProgram);
	pProgram->release();
	CHECK_GL_ERROR_DEBUG();
	sp->getGLProgram()->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
	sp->getGLProgram()->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
	sp->getGLProgram()->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORD);
	CHECK_GL_ERROR_DEBUG();
	sp->getGLProgram()->link();
	CHECK_GL_ERROR_DEBUG();
	sp->getGLProgram()->updateUniforms();
	CHECK_GL_ERROR_DEBUG();
}

NS_CSC_END