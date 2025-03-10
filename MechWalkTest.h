#include "BaseApplication.h"
#include <stdio.h>
#include <math.h>

class MechWalkTest : public BaseApplication
{
public:
	MechWalkTest(void);
	virtual ~MechWalkTest(void);

protected:
	virtual void createFrameListener(void);
	virtual void createViewports(void);
	virtual void createCamera(void);
	virtual void createScene(void);
	// call every time a frame is about to be render:
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	virtual bool keyPressed(const OIS::KeyEvent &arg);

	Ogre::SceneNode* _scn_Mech;



	// store the radius of the circle that camera will move:
	Ogre::Real rel_OrbitRadius;
	// angle, each key press will move the camera:
	Ogre::Real rel_OrbitIncrementRadians;
	// camera angle on the circle:
	Ogre::Real rel_CameraAngle;

	Ogre::AnimationState* as_Mech;

	Ogre::Real rel_MechDirection;
	Ogre::Real rel_Speed;
	Ogre::Real rel_SpeedChange;
	OgreBites::ParamsPanel* pp_SpeedPanel;

private:
	// function to adding Spotlight 
	void addSpotlight(const Ogre::String name, const Ogre::Real xPos, const Ogre::Real zPos);
	// will swing our camera depending on witch key is pressed
	bool processUnbufferedInput(const Ogre::FrameEvent& evt);
	bool frameStarted(const Ogre::FrameEvent &evt);
	void checkBounds(const Ogre::Vector3& position);
};