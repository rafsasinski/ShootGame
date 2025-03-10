#include "MechWalkTest.h"

// ---------------------------------------------------------------------------------
MechWalkTest::MechWalkTest(void):
	rel_Speed(0.0f),
	rel_MechDirection(Ogre::Math::PI/2),
	rel_SpeedChange(0.25),
	rel_CameraAngle(0.0f),
	rel_OrbitRadius(150.0),
	rel_OrbitIncrementRadians(Ogre::Math::PI/250)
{
}
// ---------------------------------------------------------------------------------

MechWalkTest::~MechWalkTest(void)
{
}


void MechWalkTest::createFrameListener(void)
{	
	BaseApplication::createFrameListener();

	Ogre::StringVector stV_items;
	stV_items.push_back("Speed");
	pp_SpeedPanel = mTrayMgr -> createParamsPanel(OgreBites::TL_NONE, "", 200, stV_items);
	mTrayMgr -> moveWidgetToTray(pp_SpeedPanel, OgreBites::TL_TOPLEFT, 0);
	pp_SpeedPanel -> show();
	mTrayMgr -> toggleAdvancedFrameStats();
	
}

void MechWalkTest::checkBounds(const Ogre::Vector3& position)
{
	Ogre::Vector3 newPosition(position.x, position.y, position.z);
	Ogre::Real rel_PlaneSize = 200;
	bool b_changed = false;
	
	if(position.x > rel_PlaneSize/2)
	{
		newPosition.x = rel_PlaneSize/2;
		b_changed = true;
	}
	else if(position.x < -rel_PlaneSize/2)
	{
		newPosition.x = -rel_PlaneSize/2;
		b_changed = true;
	}

	if(position.z > rel_PlaneSize/2)
	{
		newPosition.z = rel_PlaneSize/2;
		b_changed = true;
	}
	else if(position.z < -rel_PlaneSize/2)
	{
		newPosition.z = -rel_PlaneSize/2;
		b_changed = true;
	}


	// if the Mech really go beyond the end of the plane
	// b_changed == true;
	if(b_changed == true)
	{
		_scn_Mech -> setPosition(newPosition);
		as_Mech -> setEnabled(false);
		_scn_Mech -> yaw(Ogre::Radian(Ogre::Math::PI));
		rel_MechDirection = fmod(rel_MechDirection + Ogre::Math::PI, 2 * Ogre::Math::PI);
		rel_Speed = 0;
	}
}

bool MechWalkTest::keyPressed(const OIS::KeyEvent &arg)
{
	return true;
}

bool MechWalkTest::frameRenderingQueued(const Ogre::FrameEvent& evt)
{		 
	//as_Mech -> setEnabled(true);
	//as_Mech -> addTime(evt.timeSinceLastFrame);

	// ---------------------------------------------------------------------------------
	// to make the animation Walking cycle on when the speed is more than 0
	if(rel_Speed > 0)
	{
		as_Mech -> setEnabled(true);
		as_Mech -> addTime(evt.timeSinceLastFrame * rel_Speed/5);
		// to move the mech in our scene
		_scn_Mech -> translate(Ogre::Vector3(Ogre::Math::Cos(rel_MechDirection), 0, Ogre::Math::Sin(rel_MechDirection)) * rel_Speed * evt.timeSinceLastFrame * 2.5);
		// to secure Mech won't get out of the map
		checkBounds(_scn_Mech -> getPosition());
	}
	else
	{
		as_Mech -> setEnabled(false);
	}  

	// to view the speed in the OgreBites panel on the screen
	pp_SpeedPanel -> setParamValue(0, Ogre::StringConverter::toString(rel_Speed));
	// ---------------------------------------------------------------------------------

	bool ret = BaseApplication::frameRenderingQueued(evt);
	if(!processUnbufferedInput(evt)){ return false; }
	else if(!frameStarted(evt)){ return false; }
	return ret;
}

bool MechWalkTest::processUnbufferedInput(const Ogre::FrameEvent& evt)
{
	// ---------------------------------------------------------------------------------
	// For rotating the camera along the Orbit
	if(mKeyboard -> isKeyDown(OIS::KC_Z))
	{
		// Protection if the camera angle (2*PI)rad (360 in degree) than make it 0rad (0 in degree)
		if(rel_CameraAngle == 2*Ogre::Math::PI){ rel_CameraAngle = 0; }
		rel_CameraAngle += rel_OrbitIncrementRadians;
		mCamera -> setPosition(Ogre::Vector3(rel_OrbitRadius * Ogre::Math::Cos(rel_CameraAngle), 200, rel_OrbitRadius * Ogre::Math::Sin(rel_CameraAngle)));
		mCamera -> yaw(Ogre::Radian(-rel_OrbitIncrementRadians));
	}
	else if(mKeyboard -> isKeyDown(OIS::KC_X))
	{
		if(rel_CameraAngle == 0){ rel_CameraAngle = 2*Ogre::Math::PI; }
		rel_CameraAngle -= rel_OrbitIncrementRadians;
		mCamera -> setPosition(Ogre::Vector3(rel_OrbitRadius * Ogre::Math::Cos(rel_CameraAngle), 200, rel_OrbitRadius * Ogre::Math::Sin(rel_CameraAngle)));
		mCamera -> yaw(Ogre::Radian(rel_OrbitIncrementRadians));
	}


	// ---------------------------------------------------------------------------------



	// ---------------------------------------------------------------------------------
	// For Moving the Mesh on the scene

	// To increase or decrease speed of the Mech walking
	if(mKeyboard -> isKeyDown(OIS::KC_UP))
	{
		if(rel_Speed < 10){ rel_Speed += rel_SpeedChange; }
	}
	else if(mKeyboard -> isKeyDown(OIS::KC_DOWN))
	{
		if(rel_Speed > 0){ rel_Speed -= rel_SpeedChange; } 
	}

	// If the Mech is walking turn it left or right
	if(rel_Speed > 0)
	{
		if(mKeyboard -> isKeyDown(OIS::KC_RIGHT))
		{
			//rel_MechDirection += rel_OrbitIncrementRadians * rel_Speed;
			//_scn_Mech -> yaw(Ogre::Radian(-rel_OrbitIncrementRadians) * rel_Speed);
		}
		else if(mKeyboard -> isKeyDown(OIS::KC_LEFT))
		{
			//rel_MechDirection -= rel_OrbitIncrementRadians * rel_Speed;
			//_scn_Mech -> yaw(Ogre::Radian(rel_OrbitIncrementRadians) * rel_Speed);
		}
	}

	


	// ---------------------------------------------------------------------------------


	// ---------------------------------------------------------------------------------
	// Other actions

	// For close the application
	if(mKeyboard -> isKeyDown(OIS::KC_ESCAPE)){ mShutDown = true; }
	// ---------------------------------------------------------------------------------

	
	return true;
}

bool MechWalkTest::frameStarted(const Ogre::FrameEvent &evt)
{
	 if(mKeyboard -> isKeyDown(OIS::KC_SPACE))
	{
		Ogre::Real rel_Pos_X = _scn_Mech -> getPosition().x;
		Ogre::Real rel_Pos_Y = _scn_Mech -> getPosition().y;

		Ogre::Real rel_Mouse_X = mMouse -> getMouseState().X.rel;
		Ogre::Real rel_Mouse_Y = mMouse -> getMouseState().Y.rel;

		Ogre::Real rel_Rotation = atan2((rel_Pos_Y - rel_Mouse_Y),(rel_Pos_X - rel_Mouse_X));

		rel_MechDirection = rel_Rotation;

		//rel_MechDirection += mMouse -> getMouseState().X.rel * evt.timeSinceLastFrame * -1 *rel_Speed;
		//rel_MechDirection += mMouse -> getMouseState().X.rel * rel_Speed;
		_scn_Mech-> yaw(Ogre::Radian(rel_Rotation) * rel_Speed);
	}
	 return true;

}


void MechWalkTest::createViewports(void)
{
	Ogre::Viewport* vp_main = 
		mWindow -> addViewport(mCamera);
	vp_main -> setBackgroundColour(Ogre::ColourValue(0, 0, 0));
	mCamera -> setAspectRatio(Ogre::Real(vp_main -> getActualWidth()) / Ogre::Real(vp_main -> getActualHeight()));
}

void MechWalkTest::createCamera(void)
{
	mCamera =
		mSceneMgr -> createCamera("Main_Player_Camera");
	// To prevent camera jump we add line of code below
	mCamera->setPosition(Ogre::Vector3(rel_OrbitRadius * Ogre::Math::Cos(rel_CameraAngle), 200, rel_OrbitRadius * Ogre::Math::Sin(rel_CameraAngle)));
	mCamera -> lookAt(Ogre::Vector3(0, 0, 0));
	mCamera -> setNearClipDistance(5);
	mCameraMan = new OgreBites::SdkCameraMan(mCamera);
}

void MechWalkTest::addSpotlight(const Ogre::String name, const Ogre::Real xPos, const Ogre::Real zPos)
{
	Ogre::Light* llt_Spotlight =
		mSceneMgr -> createLight(name);
	llt_Spotlight -> setType(Ogre::Light::LT_SPOTLIGHT);
	llt_Spotlight -> setDiffuseColour(1.0, 1.0, 1.0);
	llt_Spotlight -> setSpecularColour(1.0, 1.0, 1.0);
	llt_Spotlight -> setDirection(-xPos/xPos, -1, -zPos/zPos);
	llt_Spotlight -> setPosition(xPos, 250.0, zPos);
	llt_Spotlight -> setAttenuation(500.0f, 1.0f, 0.007f, 0.0f);
	llt_Spotlight -> setSpotlightRange(Ogre::Degree(180), Ogre::Degree(180));
}

void MechWalkTest::createScene(void)
{
	// Ogre Head Mesh and snc
	Ogre::Entity* ent_Mech =
		mSceneMgr -> createEntity("ent_Mech", "Mech.mesh");
	
	_scn_Mech =
		mSceneMgr -> getRootSceneNode() -> createChildSceneNode();
	_scn_Mech -> attachObject(ent_Mech);

	as_Mech = ent_Mech -> getAnimationState("Walkcycle");
	as_Mech -> setLoop(true);

	Ogre::AxisAlignedBox aab_box = 
		ent_Mech -> getBoundingBox();
	_scn_Mech -> setPosition(0, -aab_box.getCorner(Ogre::AxisAlignedBox::FAR_LEFT_BOTTOM).y, 0);
	_scn_Mech -> showBoundingBox(true);
	ent_Mech -> setCastShadows(true);


	

	// Light 
	mSceneMgr -> setAmbientLight(Ogre::ColourValue(0.0, 0.0, 0.0));
	mSceneMgr -> setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
	addSpotlight("llt_Spotlight_01", 250.0, 0);
	addSpotlight("llt_Spotlight_02", 0, -250.0);
	addSpotlight("llt_Spotlight_03", 0, 250.0);
	addSpotlight("llt_Spotlight_04", -250.0 ,0);


	// Plane Creation 
	Ogre::Plane pl_ground(Ogre::Vector3::UNIT_Y, 0);
	Ogre::MeshManager::getSingleton().createPlane("ground",
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		pl_ground, 200, 200, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);

	Ogre::Entity* ent_pl_Ground =
		mSceneMgr -> createEntity("ent_pl_Ground", "ground");
	mSceneMgr -> getRootSceneNode() -> createChildSceneNode() -> attachObject(ent_pl_Ground);
	ent_pl_Ground -> setMaterialName("Examples/BumpyMetal");
	ent_pl_Ground -> setCastShadows(false);
}


