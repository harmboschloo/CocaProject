// Copyright (C) 2002-2009 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "IrrCompileConfig.h"

#ifdef _IRR_COMPILE_WITH_OPENGL_

#include "irrTypes.h"
#include "COpenGLTexture.h"
#include "COpenGLDriver.h"
#include "os.h"
#include "CImage.h"
#include "CColorConverter.h"

#include "irrString.h"

namespace irr
{
namespace video
{

//! constructor for usual textures
COpenGLTexture::COpenGLTexture(IImage* origImage, const core::string<c16>& name, COpenGLDriver* driver)
	: ITexture(name), ColorFormat(ECF_A8R8G8B8), Driver(driver), Image(0),
	TextureName(0), InternalFormat(GL_RGBA), PixelFormat(GL_BGRA_EXT),
	PixelType(GL_UNSIGNED_BYTE),
	IsRenderTarget(false), AutomaticMipmapUpdate(false),
	ReadOnlyLock(false), KeepImage(true)
{
	#ifdef _DEBUG
	setDebugName("COpenGLTexture");
	#endif

	HasMipMaps = Driver->getTextureCreationFlag(ETCF_CREATE_MIP_MAPS);
	getImageData(origImage);

	glGenTextures(1, &TextureName);

	if (ImageSize==TextureSize)
		Image = new CImage(ColorFormat, origImage);
	else
	{
		Image = new CImage(ColorFormat, TextureSize);
		// scale texture
		origImage->copyToScaling(Image);
	}
	copyTexture();
	if (!KeepImage)
	{
		Image->drop();
		Image=0;
	}
}

//! constructor for basic setup (only for derived classes)
COpenGLTexture::COpenGLTexture(const core::string<c16>& name, COpenGLDriver* driver)
	: ITexture(name), ColorFormat(ECF_A8R8G8B8), Driver(driver), Image(0),
	TextureName(0), InternalFormat(GL_RGBA), PixelFormat(GL_BGRA_EXT),
	PixelType(GL_UNSIGNED_BYTE),
	HasMipMaps(true), IsRenderTarget(false), AutomaticMipmapUpdate(false),
	ReadOnlyLock(false), KeepImage(true)
{
	#ifdef _DEBUG
	setDebugName("COpenGLTexture");
	#endif
}


//! destructor
COpenGLTexture::~COpenGLTexture()
{
	if (TextureName)
		glDeleteTextures(1, &TextureName);
	if (Image)
		Image->drop();
}


ECOLOR_FORMAT COpenGLTexture::getBestColorFormat(ECOLOR_FORMAT format)
{
	ECOLOR_FORMAT destFormat = ECF_A8R8G8B8;
	switch (format)
	{
		case ECF_A1R5G5B5:
			if (!Driver->getTextureCreationFlag(ETCF_ALWAYS_32_BIT))
				destFormat = ECF_A1R5G5B5;
		break;
		case ECF_R5G6B5:
			if (!Driver->getTextureCreationFlag(ETCF_ALWAYS_32_BIT))
				destFormat = ECF_A1R5G5B5;
		break;
		case ECF_A8R8G8B8:
			if (Driver->getTextureCreationFlag(ETCF_ALWAYS_16_BIT) ||
					Driver->getTextureCreationFlag(ETCF_OPTIMIZED_FOR_SPEED))
				destFormat = ECF_A1R5G5B5;
		break;
		case ECF_R8G8B8:
			if (Driver->getTextureCreationFlag(ETCF_ALWAYS_16_BIT) ||
					Driver->getTextureCreationFlag(ETCF_OPTIMIZED_FOR_SPEED))
				destFormat = ECF_A1R5G5B5;
		break;
	}
	if (Driver->getTextureCreationFlag(ETCF_NO_ALPHA_CHANNEL))
	{
		switch (destFormat)
		{
			case ECF_A1R5G5B5:
				destFormat = ECF_R5G6B5;
			break;
			case ECF_A8R8G8B8:
				destFormat = ECF_R8G8B8;
			break;
			default:
			break;
		}
	}
	return destFormat;
}


void COpenGLTexture::getImageData(IImage* image)
{
	if (!image)
	{
		os::Printer::log("No image for OpenGL texture.", ELL_ERROR);
		return;
	}

	ImageSize = image->getDimension();

	if ( !ImageSize.Width || !ImageSize.Height)
	{
		os::Printer::log("Invalid size of image for OpenGL Texture.", ELL_ERROR);
		return;
	}

	const f32 ratio = (f32)ImageSize.Width/(f32)ImageSize.Height;
	if ((ImageSize.Width>Driver->MaxTextureSize) && (ratio >= 1.0f))
	{
		ImageSize.Width = Driver->MaxTextureSize;
		ImageSize.Height = (u32)(Driver->MaxTextureSize/ratio);
	}
	else if (ImageSize.Height>Driver->MaxTextureSize)
	{
		ImageSize.Height = Driver->MaxTextureSize;
		ImageSize.Width = (u32)(Driver->MaxTextureSize*ratio);
	}
	TextureSize=ImageSize.getOptimalSize(!Driver->queryFeature(EVDF_TEXTURE_NPOT));

	ColorFormat = getBestColorFormat(image->getColorFormat());
}


//! copies the the texture into an open gl texture.
void COpenGLTexture::copyTexture(bool newTexture)
{
	if (!Image)
	{
		os::Printer::log("No image for OpenGL texture to upload", ELL_ERROR);
		return;
	}

	switch (ColorFormat)
	{
		case ECF_A1R5G5B5:
			InternalFormat=GL_RGBA;
			PixelFormat=GL_BGRA_EXT;
			PixelType=GL_UNSIGNED_SHORT_1_5_5_5_REV;
			break;
		case ECF_R5G6B5:
			InternalFormat=GL_RGB;
			PixelFormat=GL_BGR;
			PixelType=GL_UNSIGNED_SHORT_5_6_5_REV;
			break;
		case ECF_R8G8B8:
			InternalFormat=GL_RGB;
			PixelFormat=GL_BGR;
			PixelType=GL_UNSIGNED_BYTE;
			break;
		case ECF_A8R8G8B8:
			InternalFormat=GL_RGBA;
			PixelFormat=GL_BGRA_EXT;
			if (Driver->Version > 101)
				PixelType=GL_UNSIGNED_INT_8_8_8_8_REV;
			break;
		default:
			os::Printer::log("Unsupported texture format", ELL_ERROR);
			break;
	}

	Driver->setTexture(0, this);
	if (Driver->testGLError())
		os::Printer::log("Could not bind Texture", ELL_ERROR);

	if (newTexture)
	{
#ifndef DISABLE_MIPMAPPING
#ifdef GL_SGIS_generate_mipmap
		if (HasMipMaps && Driver->queryFeature(EVDF_MIP_MAP_AUTO_UPDATE))
		{
			if (Driver->getTextureCreationFlag(ETCF_OPTIMIZED_FOR_SPEED))
				glHint(GL_GENERATE_MIPMAP_HINT_SGIS, GL_FASTEST);
			else if (Driver->getTextureCreationFlag(ETCF_OPTIMIZED_FOR_QUALITY))
				glHint(GL_GENERATE_MIPMAP_HINT_SGIS, GL_NICEST);
			else
				glHint(GL_GENERATE_MIPMAP_HINT_SGIS, GL_DONT_CARE);
			// automatically generate and update mipmaps
			glTexParameteri( GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE );
			AutomaticMipmapUpdate=true;
		}
		else
#endif
		{
			AutomaticMipmapUpdate=false;
			regenerateMipMapLevels();
		}
		if (HasMipMaps) // might have changed in regenerateMipMapLevels
		{
			// enable bilinear mipmap filter
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		else
		#else
			HasMipMaps=false;
			os::Printer::log("Did not create OpenGL texture mip maps.", ELL_ERROR);
		#endif
		{
			// enable bilinear filter without mipmaps
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
	}

	void* source = Image->lock();
	if (newTexture)
		glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, Image->getDimension().Width,
			Image->getDimension().Height, 0, PixelFormat, PixelType, source);
	else
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, Image->getDimension().Width,
			Image->getDimension().Height, PixelFormat, PixelType, source);
	Image->unlock();

	if (Driver->testGLError())
		os::Printer::log("Could not glTexImage2D", ELL_ERROR);
}


//! lock function
void* COpenGLTexture::lock(bool readOnly)
{
	ReadOnlyLock |= readOnly;

	if (!Image || IsRenderTarget)
	{
		// prepare the data storage if necessary
		if (!Image)
			Image = new CImage(ECF_A8R8G8B8, ImageSize);
		if (!Image)
			return 0;

		u8* pPixels = static_cast<u8*>(Image->lock());
		if (!pPixels)
		{
			return 0;
		}
		// we need to keep the correct texture bound...
		GLint tmpTexture;
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &tmpTexture);
		glBindTexture(GL_TEXTURE_2D, TextureName);

		// allows to read pixels in top-to-bottom order
#ifdef GL_MESA_pack_invert
		if (Driver->queryOpenGLFeature(COpenGLExtensionHandler::IRR_MESA_pack_invert))
			glPixelStorei(GL_PACK_INVERT_MESA, GL_TRUE);
#endif

		glGetTexImage(GL_TEXTURE_2D, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, pPixels);

#ifdef GL_MESA_pack_invert
		if (Driver->queryOpenGLFeature(COpenGLExtensionHandler::IRR_MESA_pack_invert))
			glPixelStorei(GL_PACK_INVERT_MESA, GL_FALSE);
		else
#endif
		{
			// opengl images are horizontally flipped, so we have to fix that here.
			const s32 pitch=Image->getPitch();
			u8* p2 = pPixels + (ImageSize.Height - 1) * pitch;
			u8* tmpBuffer = new u8[pitch];
			for (u32 i=0; i < ImageSize.Height; i += 2)
			{
				memcpy(tmpBuffer, pPixels, pitch);
				memcpy(pPixels, p2, pitch);
				memcpy(p2, tmpBuffer, pitch);
				pPixels += pitch;
				p2 -= pitch;
			}
			delete [] tmpBuffer;
		}
		Image->unlock();

		//reset old bound texture
		glBindTexture(GL_TEXTURE_2D, tmpTexture);
	}
	return Image->lock();
}


//! unlock function
void COpenGLTexture::unlock()
{
	if (!Image)
		return;
	Image->unlock();
	if (!ReadOnlyLock)
		copyTexture(false);
	ReadOnlyLock = false;
	if (!KeepImage)
	{
		Image->drop();
		Image=0;
	}
}


//! Returns size of the original image.
const core::dimension2d<u32>& COpenGLTexture::getOriginalSize() const
{
	return ImageSize;
}


//! Returns size of the texture.
const core::dimension2d<u32>& COpenGLTexture::getSize() const
{
	return TextureSize;
}


//! returns driver type of texture, i.e. the driver, which created the texture
E_DRIVER_TYPE COpenGLTexture::getDriverType() const
{
	return EDT_OPENGL;
}


//! returns color format of texture
ECOLOR_FORMAT COpenGLTexture::getColorFormat() const
{
	return ColorFormat;
}


//! returns pitch of texture (in bytes)
u32 COpenGLTexture::getPitch() const
{
	if (Image)
		return Image->getPitch();
	else
		return 0;
}


//! return open gl texture name
GLuint COpenGLTexture::getOpenGLTextureName() const
{
	return TextureName;
}


//! Returns whether this texture has mipmaps
//! return true if texture has mipmaps
bool COpenGLTexture::hasMipMaps() const
{
	return HasMipMaps;
}


//! Regenerates the mip map levels of the texture. Useful after locking and
//! modifying the texture
void COpenGLTexture::regenerateMipMapLevels()
{
	if (AutomaticMipmapUpdate || !HasMipMaps || !Image)
		return;
	if ((Image->getDimension().Width==1) && (Image->getDimension().Height==1))
		return;

	// Manually create mipmaps
	u32 width=Image->getDimension().Width;
	u32 height=Image->getDimension().Height;
	u32 i=0;
	u8* target = new u8[Image->getImageDataSizeInBytes()];
	do
	{
		if (width>1)
			width>>=1;
		if (height>1)
			height>>=1;
		++i;
		Image->copyToScaling(target, width, height, Image->getColorFormat());
		glTexImage2D(GL_TEXTURE_2D, i, InternalFormat, width, height,
				0, PixelFormat, PixelType, target);
	}
	while (width!=1 || height!=1);
	delete [] target;
	Image->unlock();
}


bool COpenGLTexture::isRenderTarget() const
{
    return IsRenderTarget;
}


void COpenGLTexture::setIsRenderTarget(bool isTarget)
{
    IsRenderTarget = isTarget;
}


bool COpenGLTexture::isFrameBufferObject() const
{
	return false;
}


//! Bind Render Target Texture
void COpenGLTexture::bindRTT()
{
}


//! Unbind Render Target Texture
void COpenGLTexture::unbindRTT()
{
	Driver->setTexture(0, this);

	// Copy Our ViewPort To The Texture
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, getSize().Width, getSize().Height);
}


/* FBO Textures */

// helper function for render to texture
static bool checkFBOStatus(COpenGLDriver* Driver);

//! RTT ColorFrameBuffer constructor
COpenGLFBOTexture::COpenGLFBOTexture(const core::dimension2d<u32>& size,
                                const core::string<c16>& name,
                                COpenGLDriver* driver,
								const ECOLOR_FORMAT format)
	: COpenGLTexture(name, driver), DepthTexture(0), ColorFrameBuffer(0)
{
	#ifdef _DEBUG
	setDebugName("COpenGLTexture_FBO");
	#endif

	ImageSize = size;
	TextureSize = size;

	GLint FilteringType;
	InternalFormat = getOpenGLFormatAndParametersFromColorFormat(format, FilteringType, PixelFormat, PixelType);

	HasMipMaps = false;
	IsRenderTarget = true;

#ifdef GL_EXT_framebuffer_object
	// generate frame buffer
	Driver->extGlGenFramebuffers(1, &ColorFrameBuffer);
	Driver->extGlBindFramebuffer(GL_FRAMEBUFFER_EXT, ColorFrameBuffer);

	// generate color texture
	glGenTextures(1, &TextureName);
	Driver->setTexture(0, this);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, FilteringType);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, ImageSize.Width,
		ImageSize.Height, 0, PixelFormat, PixelType, 0);

	// attach color texture to frame buffer
	Driver->extGlFramebufferTexture2D(GL_FRAMEBUFFER_EXT,
						GL_COLOR_ATTACHMENT0_EXT,
						GL_TEXTURE_2D,
						TextureName,
						0);
#endif
	unbindRTT();
}

GLint COpenGLFBOTexture::getOpenGLFormatAndParametersFromColorFormat(ECOLOR_FORMAT format, 
																	 GLint& filtering, 
																	 GLenum& colorformat, 
																	 GLenum& type)
{
   switch(format)
   {
      // Floating Point texture formats. Thanks to Patryk "Nadro" Nadrowski.
      case ECF_R16F:
      {         
         filtering = GL_NEAREST;
         colorformat = GL_RED;
         type = GL_FLOAT;

         return GL_RGB16F_ARB;
      }
      case ECF_G16R16F:
      {
         // We haven't got support for this type specifically, so we should use RGB for a close match.
         filtering = GL_NEAREST;
         colorformat = GL_RGB;
         type = GL_FLOAT;

         return GL_RGB16F_ARB;
      }
      case ECF_A16B16G16R16F:
      {
         filtering = GL_NEAREST;
         colorformat = GL_RGBA;
         type = GL_FLOAT;

         return GL_RGBA16F_ARB;
      }
      case ECF_R32F:
      {         
         filtering = GL_NEAREST;
         colorformat = GL_RED;
         type = GL_FLOAT;

         return GL_RGB32F_ARB;
      }
      case ECF_G32R32F:
      {
         // We haven't got support for this type specifically, so we should use RGB for a close match.
         filtering = GL_NEAREST;
         colorformat = GL_RGB;
         type = GL_FLOAT;

         return GL_RGB32F_ARB;
      }
      case ECF_A32B32G32R32F:
      {
         filtering = GL_NEAREST;
         colorformat = GL_RGBA;
         type = GL_FLOAT;

         return GL_RGBA32F_ARB;
      }
   }

   filtering = GL_LINEAR;
   colorformat = GL_RGBA;
   type = GL_UNSIGNED_BYTE;

   return GL_RGBA8;
} 


//! destructor
COpenGLFBOTexture::~COpenGLFBOTexture()
{
	if (DepthTexture)
		if (DepthTexture->drop())
			Driver->removeDepthTexture(DepthTexture);
	if (ColorFrameBuffer)
		Driver->extGlDeleteFramebuffers(1, &ColorFrameBuffer);
}


bool COpenGLFBOTexture::isFrameBufferObject() const
{
	return true;
}


//! Bind Render Target Texture
void COpenGLFBOTexture::bindRTT()
{
#ifdef GL_EXT_framebuffer_object
	if (ColorFrameBuffer != 0)
		Driver->extGlBindFramebuffer(GL_FRAMEBUFFER_EXT, ColorFrameBuffer);
#endif
}


//! Unbind Render Target Texture
void COpenGLFBOTexture::unbindRTT()
{
#ifdef GL_EXT_framebuffer_object
	if (ColorFrameBuffer != 0)
		Driver->extGlBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
#endif
}


/* FBO Depth Textures */

//! RTT DepthBuffer constructor
COpenGLFBODepthTexture::COpenGLFBODepthTexture(
		const core::dimension2d<u32>& size,
		const core::string<c16>& name,
		COpenGLDriver* driver,
		bool useStencil)
	: COpenGLFBOTexture(size, name, driver), DepthRenderBuffer(0),
	StencilRenderBuffer(0), UseStencil(useStencil)
{
#ifdef _DEBUG
	setDebugName("COpenGLTextureFBO_Depth");
#endif

	ImageSize = size;
	TextureSize = size;
	InternalFormat = GL_RGBA;
	PixelFormat = GL_RGBA;
	PixelType = GL_UNSIGNED_BYTE;
	HasMipMaps = false;

	if (useStencil)
	{
		glGenTextures(1, &DepthRenderBuffer);
		glBindTexture(GL_TEXTURE_2D, DepthRenderBuffer);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
#ifdef GL_EXT_packed_depth_stencil
		if (Driver->queryOpenGLFeature(COpenGLExtensionHandler::IRR_EXT_packed_depth_stencil))
		{
			// generate packed depth stencil texture
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_STENCIL_EXT, ImageSize.Width,
				ImageSize.Height, 0, GL_DEPTH_STENCIL_EXT, GL_UNSIGNED_INT_24_8_EXT, 0);
			StencilRenderBuffer = DepthRenderBuffer; // stencil is packed with depth
		}
		else // generate separate stencil and depth textures
#endif
		{
			// generate depth texture
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, ImageSize.Width,
				ImageSize.Height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);

			// we 're in trouble! the code below does not complete
			// the FBO currently...  stencil buffer is only
			// supported with EXT_packed_depth_stencil extension
			// (above)

//			// generate stencil texture
//			glGenTextures(1, &StencilRenderBuffer);
//			glBindTexture(GL_TEXTURE_2D, StencilRenderBuffer);
//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//			glTexImage2D(GL_TEXTURE_2D, 0, GL_STENCIL_INDEX, ImageSize.Width,
//			ImageSize.Height, 0, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, 0);
//			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
	}
#ifdef GL_EXT_framebuffer_object
	else
	{
		// generate depth buffer
		Driver->extGlGenRenderbuffers(1, &DepthRenderBuffer);
		Driver->extGlBindRenderbuffer(GL_RENDERBUFFER_EXT, DepthRenderBuffer);
		Driver->extGlRenderbufferStorage(GL_RENDERBUFFER_EXT,
				GL_DEPTH_COMPONENT, ImageSize.Width,
				ImageSize.Height);
	}
#endif
}


//! destructor
COpenGLFBODepthTexture::~COpenGLFBODepthTexture()
{
	if (DepthRenderBuffer && UseStencil)
		glDeleteTextures(1, &DepthRenderBuffer);
	else
		Driver->extGlDeleteRenderbuffers(1, &DepthRenderBuffer);
	if (StencilRenderBuffer && StencilRenderBuffer != DepthRenderBuffer)
		glDeleteTextures(1, &StencilRenderBuffer);
}


//combine depth texture and rtt
bool COpenGLFBODepthTexture::attach(ITexture* renderTex)
{
	if (!renderTex)
		return false;
	video::COpenGLFBOTexture* rtt = static_cast<video::COpenGLFBOTexture*>(renderTex);
	rtt->bindRTT();
#ifdef GL_EXT_framebuffer_object
	if (UseStencil)
	{
		// attach stencil texture to stencil buffer
		Driver->extGlFramebufferTexture2D(GL_FRAMEBUFFER_EXT,
						GL_STENCIL_ATTACHMENT_EXT,
						GL_TEXTURE_2D,
						StencilRenderBuffer,
						0);

		// attach depth texture to depth buffer
		Driver->extGlFramebufferTexture2D(GL_FRAMEBUFFER_EXT,
						GL_DEPTH_ATTACHMENT_EXT,
						GL_TEXTURE_2D,
						DepthRenderBuffer,
						0);
	}
	else
	{
		// attach depth renderbuffer to depth buffer
		Driver->extGlFramebufferRenderbuffer(GL_FRAMEBUFFER_EXT,
						GL_DEPTH_ATTACHMENT_EXT,
						GL_RENDERBUFFER_EXT,
						DepthRenderBuffer);
	}
#endif
	// check the status
	if (!checkFBOStatus(Driver))
	{
		os::Printer::log("FBO incomplete");
		return false;
	}
	rtt->DepthTexture=this;
	grab(); // grab the depth buffer, not the RTT
	rtt->unbindRTT();
	return true;
}


//! Bind Render Target Texture
void COpenGLFBODepthTexture::bindRTT()
{
}


//! Unbind Render Target Texture
void COpenGLFBODepthTexture::unbindRTT()
{
}


bool checkFBOStatus(COpenGLDriver* Driver)
{
#ifdef GL_EXT_framebuffer_object
	GLenum status = Driver->extGlCheckFramebufferStatus(GL_FRAMEBUFFER_EXT);

	switch (status)
	{
		//Our FBO is perfect, return true
		case GL_FRAMEBUFFER_COMPLETE_EXT:
			return true;

		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
			os::Printer::log("FBO has invalid read buffer", ELL_ERROR);
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
			os::Printer::log("FBO has invalid draw buffer", ELL_ERROR);
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
			os::Printer::log("FBO has one or several incomplete image attachments", ELL_ERROR);
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
			os::Printer::log("FBO has one or several image attachments with different internal formats", ELL_ERROR);
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
			os::Printer::log("FBO has one or several image attachments with different dimensions", ELL_ERROR);
			break;

// not part of fbo_object anymore, but won't harm as it is just a return value
#ifdef GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT_EXT
		case GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT_EXT:
			os::Printer::log("FBO has a duplicate image attachment", ELL_ERROR);
			break;
#endif

		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
			os::Printer::log("FBO missing an image attachment", ELL_ERROR);
			break;

		case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
			os::Printer::log("FBO format unsupported", ELL_ERROR);
			break;

		default:
			break;
	}
#endif
	os::Printer::log("FBO error", ELL_ERROR);
	return false;
}


} // end namespace video
} // end namespace irr

#endif // _IRR_COMPILE_WITH_OPENGL_

