#include "Plugin.h"
#include "DeviceComponent.h"
#include "SceneManagerComponent.h"
#include "VideoDriverComponent.h"
#include "WindowCaptionComponent.h"
#include "SceneNodeComponent.h"
#include "AxisSystemComponent.h"
#include "CameraSceneNodeComponent.h"
#include "FPSCameraSceneNodeComponent.h"
#include "BulletDebugDrawComponent.h"
#include "irrlicht_io.h"
#include <coca/IFactory.h>
#include <coca/additional/components.h>

COCA_EXPORT_PLUGIN( Plugin );

Plugin::Plugin()
{
    _info.getVersion().set( 0, 0, 1, "alpha" );
    _info.setName( "cocaIrrlicht" );
    _info.setAuthor( "H.W. Boschloo" );
    _info.setEmail( "harm@boschloo.net" );
    _info.setWebsite( "www.boschloo.net" );
    _info.setDescription( "An Irrlicht plugin for the coca library." );
}

Plugin::~Plugin()
{
}

bool Plugin::init()
{
    coca::IFactory& factory = coca::getFactory();

    factory.registerComponent<coca::ConstantComponent<irr::core::dimension2du> >( "irrDimension2du" );
    factory.registerComponent<coca::ConstantComponent<irr::core::vector3df> >( "irrVector3df" );
    factory.registerComponent<coca::ConstantComponent<irr::video::SColor> >( "irrColor" );
    factory.registerComponent<coca::ConstantComponent<irr::video::E_DRIVER_TYPE> >( "irrDriverType" );

    factory.registerComponent<DeviceComponent>( "irrDevice" );
    factory.registerComponent<SceneManagerComponent>( "irrSceneManager" );
    factory.registerComponent<VideoDriverComponent>( "irrVideoDriver" );
    factory.registerComponent<WindowCaptionComponent>( "irrWindowCaption" );
    factory.registerComponent<SceneNodeComponent>( "irrSceneNode" );
    factory.registerComponent<AxisSystemComponent>( "irrAxisSystemSceneNode" );
    factory.registerComponent<CameraSceneNodeComponent>( "irrCameraSceneNode" );
    factory.registerComponent<FPSCameraSceneNodeComponent>( "irrCameraSceneNodeFPS" );
    factory.registerComponent<BulletDebugDrawComponent>( "irrBulletDebugDrawer" );

    return true;
}

